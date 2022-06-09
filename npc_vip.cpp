 #include "ScriptPCH.h"
     
class VIP_NPC : public CreatureScript
{
public:
    VIP_NPC() : CreatureScript("VIP_NPC") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetSession()->IsPremium())
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        }
        else
            creature->MonsterWhisper("Ваш аккаунт не является VIP аккаунтом.", player->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();

        if (uiAction == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());
        return true;
    }
};
 
        void AddSC_VIP_NPC()
{
    new VIP_NPC();
}


