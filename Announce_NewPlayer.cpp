#include "ScriptMgr.h"
 
class Announce_NewPlayer : public PlayerScript
{
public:
        Announce_NewPlayer() : PlayerScript("Announce_NewPlayer") {}
 
        void OnLogin(Player * player, bool) //override
        {
                std::ostringstream ss;
 
                if(player->GetTotalPlayedTime() < 5)
                {
            	ss << "|cffCC0000[Администрация Fantastic-WoW]|cffFFFFFF Приветствует нового игрока,|r ""|CFF18BE00["<< ChatHandler(player->GetSession()).GetNameLink() <<"|CFF18BE00]|r";
				sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
                        return;
                }
                else
                {
                }
        }
 
};
void AddSC_Announce_NewPlayer()
{
        new Announce_NewPlayer;
}
