#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Common.h"
#include "Language.h"


class vipcommands : public CommandScript
{
public:
    vipcommands() : CommandScript("vipcommands") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand vipCommandTable[] =

        {
			{ "summon",				 SEC_PLAYER,  false, &HandleSummon,					"", NULL },
			{ "appear",				 SEC_PLAYER,  false, &HandleAppear,					"", NULL },
			{ "instance",          	 SEC_PLAYER,  false, &HandleVipMallCommand,         "", NULL },
			{ "activate",            SEC_PLAYER,  false, &HandleActivateCommand,		"", NULL },
			{ "changerace",          SEC_PLAYER,  false, &HandleChangeRaceCommand,      "", NULL },
			{ "changefaction",		 SEC_PLAYER,  false, &HandleChangeFactionCommand,	"", NULL },
			{ "maxskills",			 SEC_PLAYER,  false, &HandleMaxSkillsCommand,		"", NULL },
			{ "customize",			 SEC_PLAYER,  false, &HandleCustomizeCommand,		"", NULL },
            { NULL,             0,                false, NULL,                          "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "vip",	    SEC_PLAYER,   true, NULL,      "",  vipCommandTable},
	       { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }
	static bool HandleSummon(ChatHandler * handler, const char* args)
	{

		return true;
	}

	static bool HandleAppear(ChatHandler * handler, const char* args)
	{

		return true;
	}

static bool HandleActivateCommand(ChatHandler * handler, const char * args)
{
	Player* player = handler->GetSession()->GetPlayer();

	if(player->GetSession()->GetSecurity() >= 1)
	{
		handler->PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tВы уже получили VIP звание.");
		handler->SetSentErrorMessage(true);
		return false;
	}

	if(player->HasItemCount(313370, 1, false)) // Token ID, Count.
	{
                  PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_ACCESS);
                  stmt->setUInt32(0, player->GetSession()->GetAccountId());
                  stmt->setUInt8(1, 1);
                  stmt->setInt32(2, -1);
	         LoginDatabase.Execute(stmt);
		player->AddItem(438544, 1);
		player->DestroyItemCount(313370, 1, true, false); // Token ID, Count.
		handler->PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tВаш VIP рейтинг обновлен. Войдите, чтобы активировать его");
		return true;
	}
	return true;
}
	
	static bool HandlevipCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();

            me->Say("vip command?", LANG_UNIVERSAL);
            return true;
    }


	/* The commands */

	static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		handler->PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tПерелог, чтобы изменить расу вашего персонажа.   --Anyrel-WoW--");
        return true;
    }

		static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		handler->PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tПерелог, чтобы изменить фракцию вашего персонажа.   --Anyrel-WoW--");
        return true;
    }

		static bool HandleMaxSkillsCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->UpdateSkillsForLevel();
		handler->PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tВаши навыки оружия теперь максимальны.   --Anyrel-WoW--");
        return true;
    }

	static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		handler->PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tПерелог для настройки вашего персонажа.   --Anyrel-WoW--");
        return true;
    }

	static bool HandleVipMallCommand(ChatHandler* handler, const char* args)
    {

            Player* me = handler->GetSession()->GetPlayer();

        if (me->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (me->IsInFlight())
        {
            me->GetMotionMaster()->MovementExpired();
            me->CleanupAfterTaxiFlight();
        }
        // stop flight if need
        if (me->IsInFlight())
        {
            me->GetMotionMaster()->MovementExpired();
            me->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            me->SaveRecallPosition();

		me->TeleportTo(585, 7.09f,	-0.45f, -2.8f, 6.22973f); // MapId, X, Y, Z, O
		handler->PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tВы телепортировались!    --Anyrel-WoW--");
        return true;
    }
	    
};

void AddSC_vipcommands()
{
    new vipcommands();
}