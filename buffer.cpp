#include "ScriptPCH.h"
#include "ScriptMgr.h"

#define MSG_GOSSIP_TEXT_MAIN_MENU "<- [Вернутся в Главное меню]"

#define CONST_HONOR_1 10000
#define CONST_HONOR_2 15000
#define CONST_AP_1 50
#define CONST_AP_2 75
#define CONST_ITEM_1 3
#define CONST_ITEM_2 6

class npc_buffer : public CreatureScript
{
    public: 
    npc_buffer() : CreatureScript("npc_buffer") { }


    /*inline bool buff(int spellid, Player* player, Creature* creature)
     {
          if (player->HasAura(spellid))
            {
                creature->MonsterWhisper("У вас уже есть подобное заклинание!", player);
                player->CLOSE_GOSSIP_MENU(); 
                return true;
            }
          else return false;
     }*/

        bool OnGossipHello(Player* player, Creature* creature)
   { 
              if (player->IsInCombat())
           {
               player->PlayerTalkClass->SendCloseGossip();
	        creature->MonsterWhisper("Вы в бою!", player);
               return true;
           }

        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/icons/Achievement_PVP_H_15:25:25:-18:0|t[Баффы за хонор] ->", GOSSIP_SENDER_MAIN, 1);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/icons/Achievement_Arena_2v2_7:25:25:-18:0|t[Баффы за ап] ->", GOSSIP_SENDER_MAIN, 2);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/icons/Achievement_Arena_3v3_7:25:25:-18:0|t[Баффы за монеты онлайна] ->", GOSSIP_SENDER_MAIN, 3);
        player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
        return true;
    }
             
        void SendDefaultMenu(Player* player, Creature* creature, uint32 action)
  {
    switch (action)
        {
        case 1:
          player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/icons/Achievement_PVP_H_15:25:25:-18:0|t[Баффы за 10к хонора] ->", GOSSIP_SENDER_MAIN, 4);
          player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/icons/Achievement_PVP_H_15:25:25:-18:0|t[Баффы за 15к хонора] ->", GOSSIP_SENDER_MAIN, 5);
          player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
          break;

        case 2:
          player->PlayerTalkClass->ClearMenus();
          player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/icons/Achievement_Arena_2v2_7:25:25:-18:0|t[Баффы за 50 апа] ->", GOSSIP_SENDER_MAIN, 6);
          player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/icons/Achievement_Arena_2v2_7:25:25:-18:0|t[Баффы за 75 апа] ->", GOSSIP_SENDER_MAIN, 7);
          player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
          break;

        case 3:
          player->PlayerTalkClass->ClearMenus();
          player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/icons/Achievement_Arena_3v3_7:25:25:-18:0|t[Баффы за 3 монеты онлайна] ->", GOSSIP_SENDER_MAIN, 8);
          player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/icons/Achievement_Arena_3v3_7:25:25:-18:0|t[Баффы за 6 монет онлайна] ->", GOSSIP_SENDER_MAIN, 9);
          player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
          break;

        case 4:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_nature_elementalshields:25|tСаронитовая преграда 10% [Бафнуть]", GOSSIP_SENDER_MAIN, 10);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_shadow_unholyfrenzy:25|tБерсерк 5% [Бафнуть]", GOSSIP_SENDER_MAIN, 11);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\ability_rogue_sprint:25|tСпринт 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 12);
                player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
                break;

        case 5:
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_nature_elementalshields:25|tСаронитовая преграда 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 20);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_shadow_unholyfrenzy:25|tБерсерк 10% [Бафнуть]", GOSSIP_SENDER_MAIN, 21);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\ability_rogue_sprint:25|tСпринт 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 22);
                player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID()); 
                break;

        case 6:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_nature_elementalshields:25|tСаронитовая преграда 10% [Бафнуть]", GOSSIP_SENDER_MAIN, 13);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_shadow_unholyfrenzy:25|tБерсерк 5% [Бафнуть]", GOSSIP_SENDER_MAIN, 14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\ability_rogue_sprint:25|tСпринт 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 15);
                player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
                break;

        case 7:
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_nature_elementalshields:25|tСаронитовая преграда 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 23);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_shadow_unholyfrenzy:25|tБерсерк 10% [Бафнуть]", GOSSIP_SENDER_MAIN, 24);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\ability_rogue_sprint:25|tСпринт 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 25);
                player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID()); 
                break;

        case 8:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_nature_elementalshields:25|tСаронитовая преграда 10% [Бафнуть]", GOSSIP_SENDER_MAIN, 16);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_shadow_unholyfrenzy:25|tБерсерк 5% [Бафнуть]", GOSSIP_SENDER_MAIN, 17);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\ability_rogue_sprint:25|tСпринт 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 18);
                player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
                break;

        case 9:
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_nature_elementalshields:25|tСаронитовая преграда 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 26);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\spell_shadow_unholyfrenzy:25|tБерсерк 10% [Бафнуть]", GOSSIP_SENDER_MAIN, 27);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\icons\ability_rogue_sprint:25|tСпринт 15% [Бафнуть]", GOSSIP_SENDER_MAIN, 28);
                player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID()); 
                break;

        case 10:
        if (player->GetHonorPoints() < 10000)
            {
                creature->MonsterWhisper("У вас недостаточно чести для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else {
                player->CastSpell(player, 90018, true);
                player->ModifyHonorPoints(-10000);
                player->CLOSE_GOSSIP_MENU();
                break;
            } 

        case 11:
        if (player->GetHonorPoints() < 10000)
            {
                creature->MonsterWhisper("У вас недостаточно чести для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90020, true);
                player->ModifyHonorPoints(-10000);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 12:
        if (player->GetHonorPoints() < 10000)
            {
                creature->MonsterWhisper("У вас недостаточно чести для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90022, true);
                player->ModifyHonorPoints(-10000);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 13:
        if (player->GetArenaPoints() < 50)
            {
                creature->MonsterWhisper("У вас недостаточно арена поинтов для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90018, true);
                player->ModifyArenaPoints(-50);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 14:
        if (player->GetArenaPoints() < 50)
            {
                creature->MonsterWhisper("У вас недостаточно арена поинтов для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90020, true);
                player->ModifyArenaPoints(-50);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 15:
        if (player->GetArenaPoints() < 50)
            {
                creature->MonsterWhisper("У вас недостаточно арена поинтов для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90022, true);
                player->ModifyArenaPoints(-50);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 16:
        if (player->GetItemCount(90001) < 3)
            {
                creature->MonsterWhisper("У вас недостаточно монет онлайна для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90018, true);
                player->DestroyItemCount(90001,3,true);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 17:
        if (player->GetItemCount(90001) < 3)
            {
                creature->MonsterWhisper("У вас недостаточно монет онлайна для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90020, true);
                player->DestroyItemCount(90001,3,true);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 18:
        if (player->GetItemCount(90001) < 3)
            {
                creature->MonsterWhisper("У вас недостаточно монет онлайна для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90022, true);
                player->DestroyItemCount(90001,3,true);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 20:
        if (player->GetHonorPoints() < 15000)
              {
                creature->MonsterWhisper("У вас недостаточно чести для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90019, true);
                player->ModifyHonorPoints(-15000);
                player->CLOSE_GOSSIP_MENU();
                break;
              }

        case 21:
        if (player->GetHonorPoints() < 15000)
            {
                creature->MonsterWhisper("У вас недостаточно чести для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90021, true);
                player->ModifyHonorPoints(-15000);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 22:
        if (player->GetHonorPoints() < 15000)
            {
                creature->MonsterWhisper("У вас недостаточно чести для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90023, true);
                player->ModifyHonorPoints(-15000);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 23:
        if (player->GetArenaPoints() < 75)
              {
                creature->MonsterWhisper("У вас недостаточно арена поинтов для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90019, true);
                player->ModifyArenaPoints(-75);
                player->CLOSE_GOSSIP_MENU();
                break;
              }

        case 24:
        if (player->GetArenaPoints() < 75)
            {
                creature->MonsterWhisper("У вас недостаточно арена поинтов для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90021, true);
                player->ModifyArenaPoints(-75);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 25:
        if (player->GetArenaPoints() < 75)
            {
                creature->MonsterWhisper("У вас недостаточно арена поинтов для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90023, true);
                player->ModifyArenaPoints(-75);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 26:
        if (player->GetItemCount(90001) < 6)
              {
                creature->MonsterWhisper("У вас недостаточно монет онлайна для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90019, true);
                player->DestroyItemCount(90001,6,true);
                player->CLOSE_GOSSIP_MENU();
                break;
              }

        case 27:
        if (player->GetItemCount(90001) < 6)
            {
                creature->MonsterWhisper("У вас недостаточно монет онлайна для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90021, true);
                player->DestroyItemCount(90001,6,true);
                player->CLOSE_GOSSIP_MENU();
                break;
            }

        case 28:
        if (player->GetItemCount(90001) < 6)
            {
                creature->MonsterWhisper("У вас недостаточно монет онлайна для совершения покупки!", player);
                player->CLOSE_GOSSIP_MENU();
                break;
        }else{
                player->CastSpell(player, 90023, true);
                player->DestroyItemCount(90001,6,true);
                player->CLOSE_GOSSIP_MENU();
                break;
            }
      }
 }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
      {
	    player->PlayerTalkClass->ClearMenus();
	    if (sender == GOSSIP_SENDER_MAIN)
	    SendDefaultMenu(player, creature, action);
	    return true;
      }

   };
void AddSC_npc_buffer()
{
    new npc_buffer;
}