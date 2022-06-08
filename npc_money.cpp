#include "ScriptPCH.h"
#include "ScriptedGossip.h"

class npc_money : public CreatureScript
{
public:
    npc_money() : CreatureScript("npc_money") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
              if (player->IsInCombat())
           {
               player->PlayerTalkClass->SendCloseGossip();
	        creature->MonsterWhisper("|TInterface/ICONS/Spell_holy_avenginewrath:25|tВы в бою!", player);
               return true;
           }

           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface\\ICONS\\inv_misc_coin_16:25:25:-18:0|t[Купить] ->", GOSSIP_SENDER_MAIN, 1000);
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface\\ICONS\\inv_misc_coin_16:25:25:-18:0|t[Продать] ->", GOSSIP_SENDER_MAIN, 1002);
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface\\ICONS\\inv_misc_key_15:25:25:-18:0|tВыйти", GOSSIP_SENDER_MAIN, 1001); 
           //player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "[Обменять] ->", GOSSIP_SENDER_MAIN, 1002); 
           player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
           return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
     {
	    player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
           case 1000: player->GetSession()->SendListInventory(creature->GetGUID()); 
           break;

           case 1001:
           player->CLOSE_GOSSIP_MENU();
           break;

           case 1002:
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_pvp_a_01:25|t25к хонора", GOSSIP_SENDER_MAIN, 300);
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_pvp_a_01:25|t50к хонора", GOSSIP_SENDER_MAIN, 301);
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_pvp_a_01:25|t75к хонора", GOSSIP_SENDER_MAIN, 302);
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_pvp_a_01:25|t100к хонора", GOSSIP_SENDER_MAIN, 303);
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_pvp_g_02:25|t100 арена поинтов", GOSSIP_SENDER_MAIN, 304);
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_pvp_g_02:25|t500 арена поинтов", GOSSIP_SENDER_MAIN, 305);
           player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_pvp_g_02:25|t1000 арена поинтов", GOSSIP_SENDER_MAIN, 306);
           player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
           break;

           case 300:
           if (player->GetItemCount(90031) >= 1)
            {
              player->DestroyItemCount(90031,1, true);
              player->ModifyHonorPoints(25000);
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FОперация прошла успешно.");
            } else player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FУ вас нет нужного предмета.");
              player->CLOSE_GOSSIP_MENU();
           break;

           case 301:
           if (player->GetItemCount(90032) >= 1)
            {
              player->DestroyItemCount(90032,1, true);
              player->ModifyHonorPoints(50000);
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FОперация прошла успешно.");
            } else player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FУ вас нет нужного предмета.");
              player->CLOSE_GOSSIP_MENU();
           break;

           case 302:
           if (player->GetItemCount(90033) >= 1)
            {
              player->DestroyItemCount(90033,1, true);
              player->ModifyHonorPoints(75000);
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FОперация прошла успешно.");
            } else player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FУ вас нет нужного предмета.");
              player->CLOSE_GOSSIP_MENU();
           break;

           case 303:
           if (player->GetItemCount(90034) >= 1)
            {
              player->DestroyItemCount(90034,1, true);
              player->ModifyHonorPoints(100000);
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FОперация прошла успешно.");
            } else player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FУ вас нет нужного предмета.");
              player->CLOSE_GOSSIP_MENU();
           break;

           case 304:
           if (player->GetItemCount(90056) >= 1)
            {
              player->DestroyItemCount(90056,1, true);
              player->ModifyArenaPoints(100);
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FОперация прошла успешно.");
            } else player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FУ вас нет нужного предмета.");
              player->CLOSE_GOSSIP_MENU();
           break;

           case 305:
           if (player->GetItemCount(90053) >= 1)
            {
              player->DestroyItemCount(90053,1, true);
              player->ModifyArenaPoints(500);
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FОперация прошла успешно.");
            } else player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FУ вас нет нужного предмета.");
              player->CLOSE_GOSSIP_MENU();
           break;

           case 306:
           if (player->GetItemCount(90054) >= 1)
            {
              player->DestroyItemCount(90054,1, true);
              player->ModifyArenaPoints(1000);
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FОперация прошла успешно.");
            } else player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_bg_trueavshutout:25|t|cffADFF2FУ вас нет нужного предмета.");
              player->CLOSE_GOSSIP_MENU();
           break;
           
        }
        return true;

      }
};

void AddSC_npc_money()
{
	new npc_money();
}
