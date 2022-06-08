#include "ScriptPCH.h"
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;
class npc_changer : public CreatureScript
{
public:
npc_changer() : CreatureScript("npc_changer"){}

        bool OnGossipHello(Player * pPlayer, Creature * pCreature)
           {
             pPlayer->PlayerTalkClass->ClearMenus();
             pPlayer->ADD_GOSSIP_ITEM(4, "|TInterface/ICONS/Achievement_doublejeopardy:25|tИзменить мою рассу/фракцию - 1000 апа", GOSSIP_SENDER_MAIN, 1);
             pPlayer->ADD_GOSSIP_ITEM(4, "|TInterface/ICONS/Achievement_doublejeopardy:25|tИзменить мою внешность - 500 апа", GOSSIP_SENDER_MAIN, 2);
             pPlayer->PlayerTalkClass->SendGossipMenu(9425, pCreature->GetGUID());
             return true;
           }

           string to_string(int param)
             {
              string str = "";
              for(str = ""; param ; param /= 10)
              str += (char)('0' + param % 10);
              reverse(str.begin(), str.end());
              return str;
             }

           int cost(int p, Player * Player)
             {
               int i = Player->GetArenaPoints();
               std::string ap;
               ap += "|TInterface/ICONS/Achievement_pvp_g_04:25|t|cffFFC125У вас недостаточно арена поинтов! Для смены нужно ";
               ap += to_string(p);
               ap += ", а у вас "; 
               ap += to_string(i);
               ap += "!";
               Player->GetSession()->SendNotification(ap.c_str());
               return 0;
             }
        bool OnGossipSelect(Player * Player, Creature * Creature, uint32 /*uiSender*/, uint32 uiAction)
           {
              if(!Player)
              return true;

              switch(uiAction)
                 {
                   case 1:
                   if (Player->GetArenaPoints() == 0)
                    {
                     Player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_pvp_g_04:25|t|cffFFC125У вас недостаточно арена поинтов! Для смены нужно 1000, а у вас 0!");
                     Player->PlayerTalkClass->SendCloseGossip();
                     return false;
                    }
                   else if (Player->GetArenaPoints() < 1000)
                      {
                       int p=1000;
                       cost(p, Player);
                       return false;
                      }
                   else
                      {
                        Player->ModifyArenaPoints(-1000);
                        Player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                        CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '64' WHERE guid = %u", Player->GetGUID());
                        Player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_pvp_g_04:25|t|cffFFC125Сделайте релог для смены рассы/фракции!");
                        Player->PlayerTalkClass->SendCloseGossip();
                        break;
                      }

                   case 2:
                   if (Player->GetArenaPoints() == 0)
                    {
                     Player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_pvp_g_04:25|t|cffFFC125У вас недостаточно арена поинтов! Для смены нужно 500, а у вас 0!");
                     Player->PlayerTalkClass->SendCloseGossip();
                     return false;
                    }
                   else if (Player->GetArenaPoints() < 500)
                      {
                       int p=500;
                       cost(p, Player);
                       return false;
                      }
                   else
                      {
                        Player->ModifyArenaPoints(-500);
                        Player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
                        CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '8' WHERE guid = %u", Player->GetGUID());
                        Player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_pvp_g_04:25|t|cffFFC125Сделайте релог для смены внешности!");
                        Player->PlayerTalkClass->SendCloseGossip();
                        break;
                      }
                }

                  return true;
              }

};

void AddSC_npc_changer()
{
new npc_changer();
}