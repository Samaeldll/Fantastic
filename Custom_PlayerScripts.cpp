#include "ScriptMgr.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Config.h"

class TimedWelcome : public BasicEvent
{
public:
    TimedWelcome(Player* player) : _player(player) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/)
    {
        if (_player && _player->GetSession())
        {
            SendCustomWelcomePacket(_player);

            ChatHandler(_player->GetSession()).PSendSysMessage("www.fantastic-wow.ru");

            std::ostringstream message;
            message << "|cffFFFFFFОнлайн " << "|cff00FF00+";
            message << sWorld->GetPlayerCount();
            int k=0;
            for (int i=0; i < sWorld->GetPlayerCount(); i++, k++)
	     if (sWorld->GetPlayerCount() < 21)
	       {
		    if (k == 20)
			 k = 0;
	       }
	     else
		{
		   if (k == 10)
			k = 0;
		}

            if (k == 1)
             message << " |cffFFFFFFигрок.";   
            else if (k > 1 && k < 5)
             message << " |cffFFFFFFигрока.";   
            else if (k > 4 && k < 21)
             message << " |cffFFFFFFигроков.";   

            ChatHandler(_player->GetSession()).PSendSysMessage(message.str().c_str());
        }
        return true;
    }

private:
    Player* _player;
    static const uint32 QuestID = 80000; // dummy [quest must not exist]

    void SendCustomWelcomePacket(Player* player)
    {
        std::string questTitle = "|cff120A8FДобро пожаловать на Fantastic-WoW";
        std::string questDetails = "Неизвестный";
        std::ostringstream questObjectives;
        std::string questEndText = "|cff120A8FДобро пожаловать на Fantastic-WoW";
 
        questObjectives << "\n### Удачной вам игры и приятного время провождения на нашем сервере :)";
        std::string qoestO_str = questObjectives.str();
        int32 locale = player->GetSession()->GetSessionDbLocaleIndex();

        if (locale >= 0)
        {
            if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(QuestID))
            {
                ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
                ObjectMgr::GetLocaleString(localeData->Details, locale, questDetails);
                ObjectMgr::GetLocaleString(localeData->Objectives, locale, qoestO_str);
                ObjectMgr::GetLocaleString(localeData->EndText, locale, questEndText);
            }
        }

        std::ostringstream cDetails;
        // todo convert to trinity_strings
        cDetails << "### [Приветствуем] " << "" << player->GetName().c_str() << "|r" << "\n \n###Наш сайт:|r www.fantastic-wow.ru|r";

        cDetails << "\n \n###Вы можете задать интересующий вас вопрос в группе [ВК]:https://vk.com/fantastic_wow|r. А так же сюда можно обращаться по всем вопросам пожертвований > |cffCC0000https://vk.com/fantastic_wow";

        questDetails = cDetails.str();

        WorldPacket data(SMSG_QUESTGIVER_QUEST_DETAILS, 100);   // guess size
        data << uint64(player->GetGUID());
        data << uint64(player->GetDivider());
        data << uint32(QuestID);                 // this is quest ID
        data << questTitle;
        data << questDetails;
        data << questObjectives.str();
        data << uint8(0);                  // auto finish 1 : 0
        data << uint32(0);                 // 3.3.3 questFlags
        data << uint32(0);
        data << uint8(0);                  // IsFinished? value is sent back to server in quest accept packet

        data << uint32(0);                 // Rewarded chosen items hidden
        data << uint32(0);                 // Rewarded items hidden
        data << uint32(0);                 // Rewarded money hidden
        data << uint32(0);                 // Rewarded XP hidden

        // rewarded honor points. Multiply with 10 to satisfy client
        data << uint32(0);             // quest xp gain
        data << float(0.0f);           // unk, honor multiplier?
        data << uint32(0);             // reward spell, this spell will display (icon) (cast if RewSpellCast == 0)
        data << int32(0);              // cast spell
        data << uint32(0);             // CharTitleId, new 2.4.0, player gets this title (id from CharTitles)
        data << uint32(0);             // bonus talents
        data << uint32(0);             // reward arena points
        data << uint32(0);             // unk

        for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
            data << uint32(0);

        for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
            data << int32(0);

        for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
            data << int32(0);

        data << uint32(QUEST_EMOTE_COUNT);
        for (uint32 i = 0; i < QUEST_EMOTE_COUNT; ++i)
        {
            data << uint32(0);
            data << uint32(0);       // DetailsEmoteDelay (in ms)
        }
        player->GetSession()->SendPacket(&data);
    }
};

class TimedWelcomeScript : public PlayerScript
{
public:
    TimedWelcomeScript() : PlayerScript("TimedWelcomeScript"){}

    void OnLogin(Player* player)
    {
        player->m_Events.AddEvent(new TimedWelcome(player), player->m_Events.CalculateTime(3 * IN_MILLISECONDS));
    }
};

void AddSC_TimedWelcome()
{
    new TimedWelcomeScript;
}
