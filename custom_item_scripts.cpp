class item_mod_talent : public ItemScript
{
public:
    item_mod_talent() : ItemScript("item_mod_talent") { }


    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) OVERRIDE
    {
            if(player->CharSelectAndReplace())
             {
                player->SetFreeTalentPoints(1);
                player->SendTalentsInfoData(false);
                player->DestroyItemCount(90090, 1, true);
             }
             else player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_pvp_a_01:25|t|cffADFF2FУ вас уже есть 5 дополнительных талантов.");
     return true;

    }
};

/*class item_mod_honor_ap : public ItemScript
{
public:
    item_mod_honor_ap() : ItemScript("item_mod_honor_ap") { }


    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) OVERRIDE
    {
        uint32 itemId = item->GetEntry();
         switch(itemId)
           {
                case 90031:
                player->DestroyItemCount(90031,1, true);
                player->ModifyHonorPoints(25000);
                break;

                case 90032:
                player->DestroyItemCount(90032,1, true);
                player->ModifyHonorPoints(50000);
                break;

                case 90033:
                player->DestroyItemCount(90033,1, true);
                player->ModifyHonorPoints(75000);
                break;

                case 90034:
                player->DestroyItemCount(90034,1, true);
                player->ModifyHonorPoints(100000);
                break;

                case 90053:
                player->DestroyItemCount(90053,1, true);
                player->ModifyArenaPoints(500);
                break;

                case 90054:
                player->DestroyItemCount(90054,1, true);
                player->ModifyArenaPoints(1000);
                break;

                case 90056:
                player->DestroyItemCount(90056,1, true);
                player->ModifyArenaPoints(100);
                break;
          }

         return true;
    }
};*/


void AddSC_custom_item_scripts()
{
    new item_mod_talent();
    //new item_mod_honor_ap();
}
