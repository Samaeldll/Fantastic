#include "ScriptPCH.h"

class on_script : public PlayerScript
{
    public:
        on_script() : PlayerScript("on_script") {}
       
        void OnLogin(Player* player)
         {
            player->InitTalentForLevel();
            player->SendTalentsInfoData(false); 
	     player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffffcc00Приветствуем вас на нашем сервере! Удачной игры!");
            ChatHandler(player->GetSession()).PSendSysMessage("|TInterface/ICONS/Achievement_general:25|t|cffFF6EB4Приветствуем вас на нашем сервере! Удачной игры!");

            if(player->GetTotalPlayedTime() < 5)
            {
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffffcc00Спасибо что вы выбрали наш сервер!");
              ChatHandler(player->GetSession()).PSendSysMessage("|TInterface/ICONS/Achievement_general:25|t|cffFF6EB4Спасибо что вы выбрали наш сервер!");
              player->AddItem(90002, 100);
              player->AddItem(90000, 1);
              player->AddItem(21215,1);


                        switch(player->getClass())
                        {
			     case CLASS_DEATH_KNIGHT:
				player->AddItem(101020, 1); break;

			     case CLASS_DRUID:
				player->AddItem(101021, 1);
				player->AddItem(101022, 1); break;

			     case CLASS_HUNTER:
				player->AddItem(101021, 1); break;

			     case CLASS_MAGE:
				player->AddItem(101022, 1); break;

			     case CLASS_PALADIN:
				player->AddItem(101020, 1);
				player->AddItem(101022, 1); break;

			     case CLASS_PRIEST:
				player->AddItem(101022, 1); break;

			     case CLASS_ROGUE:
				player->AddItem(101021, 1); break;

			     case CLASS_SHAMAN:
				player->AddItem(101021, 1);
				player->AddItem(101022, 1); break;

			     case CLASS_WARLOCK:
				player->AddItem(101022, 1); break;

			    case CLASS_WARRIOR:
				player->AddItem(101020, 1); break;
                        }
            }
         }


};

void AddSC_on_script()
{
    new on_script();
}
