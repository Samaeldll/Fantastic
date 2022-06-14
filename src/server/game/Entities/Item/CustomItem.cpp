#include "CustomItem.h"
#ifdef _CUSTOMITEM_H

#include "Common.h"
#include "WorldSession.h"
#include "WorldSocket.h"
#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "GossipDef.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "GameObjectAI.h"
#include "Language.h"
#include "Util.h"
#include "SpellInfo.h" 
#include "Unit.h"
#include "Spell.h"
#include "Group.h"
#include "LootMgr.h"
#include "AuctionHouseMgr.h"

using namespace std;

string to_string(int param)
{
  std::string str = "";
  for(str = ""; param ; param /= 10)
  str += (char)('0' + param % 10);
  reverse(str.begin(), str.end());
  return str;
}

void CustomItem::SendCustomBuyError(Player* player, Creature* creature, VendorItem const* crItem)
{
    WorldSession* _session = player->GetSession();

    ItemExtendedCostEntry const* iece = sItemExtendedCostStore.LookupEntry(crItem->ExtendedCost);
    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(crItem->item);

    if (!pProto)
        return;

    uint32 reqMoney = pProto->BuyPrice;
    uint32 CustomReqHonor = pProto->CustomReqHonor ? pProto->CustomReqHonor : 0;
    uint32 CustomReqArena = pProto->CustomReqArena ? pProto->CustomReqArena : 0;
    uint32 reqHonor = iece ? (iece->reqhonorpoints + CustomReqHonor) : CustomReqHonor;
    uint32 reqArenaPoints = iece ? (iece->reqarenapoints + CustomReqArena) : CustomReqArena;
    float reqOnlinePoints = pProto->CustomOnlinePoints;
    uint32 questID = 5;

    Quest const* quest = sObjectMgr->GetQuestTemplate(questID);

    std::string questTitle = "|cffFF0000 Fail to buy..";

    std::ostringstream rit;

    rit << "|cffADFF2F   Здравствуйте, $N, если вы хотите купить эту вещь, то для покупки вам понадобится :$b";

    if (reqMoney && player->GetMoney() < reqMoney)
    {
        uint32 needmoremoney = reqMoney - player->GetMoney();
        rit << "$b|cff888888+|rNeed Coins: ";

        if (needmoremoney >= GOLD)
            rit << needmoremoney / GOLD << " gold ";

        if (needmoremoney >= SILVER)
            rit << (needmoremoney % GOLD) / SILVER << " silver ";

        rit << (needmoremoney % GOLD) % SILVER << " copper$b";
    }

    if (iece)
    {
        for (uint8 i = 0; i < MAX_ITEM_EXTENDED_COST_REQUIREMENTS; ++i)
        {
            if (!iece->reqitem[i])
                continue;

            if (player->HasItemCount(iece->reqitem[i], iece->reqitemcount[i]))
                continue;

            ItemTemplate const* proto = sObjectMgr->GetItemTemplate(iece->reqitem[i]);
            rit << "$b|cff888888+|r[";

            rit << proto->Name1.c_str();

            rit << "]: |cffFF0000" << player->GetItemCount(iece->reqitem[i]) << "|r/|cffFF0000" << iece->reqitemcount[i] << "|r$b";
        }
    }

    for (uint8 i = 0; i <= 9; ++i)
    {
        if (pProto->CustomItemCost[i].NeedItem)
        {
            ItemTemplate const* pi = sObjectMgr->GetItemTemplate(pProto->CustomItemCost[i].NeedItem);

            if (!pi)
                continue;

            /*if (player->HasItemCount(pProto->CustomItemCost[i].NeedItem, pProto->CustomItemCost[i].ItemCount))
                continue;*/

            rit << "$b|cff888888+|r[";

            rit << pi->Name1.c_str();

            rit << "]: |cffFF0000" << pProto->CustomItemCost[i].ItemCount << "|r/|cffFF0000" << player->GetItemCount(pProto->CustomItemCost[i].NeedItem) << "|r" << " - (|cffffffffваше кол-во)";
        }
    }

    if (reqHonor)
        rit << "$b|cff888888+|rТребуется очков чести: |cffFF0000" << reqHonor << "/" << player->GetHonorPoints() << "|r" << " - (|cffffffffваше кол-во)";

    if (reqArenaPoints)
        rit << "$b|cff888888+|rТребуется очков арены: |cffFF0000" << reqArenaPoints << "/" << player->GetArenaPoints() << "|r" << " - (|cffffffffваше кол-во)";

    if (reqOnlinePoints && player->GetCustomOnlinePoints() < reqOnlinePoints)
        rit << "$b|cff888888+|rТребуется онлайн поинтов: |cffFF0000" << reqOnlinePoints - player->GetCustomOnlinePoints() << "|r points$b";

    string rit_str = rit.str();
    int32 locale = _session->GetSessionDbLocaleIndex();

    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->RequestItemsText, locale, rit_str);
        }
    }

    WorldPacket data(SMSG_QUESTGIVER_REQUEST_ITEMS, 50);
    data << uint64(creature ? creature->GetGUID() : _session->GetPlayer()->GetGUID());
    data << uint32(quest->GetQuestId());
    data << questTitle;
    data << rit.str();
    data << uint32(0);
    data << uint32(0);
    data << uint32(false);
    data << uint32(0);
    data << uint32(0);
    data << uint32(0);

    data << uint32(0);

    data << uint32(0x00);
    data << uint32(0x04);
    data << uint32(0x08);
    data << uint32(0x10);

    _session->SendPacket(&data);
}

void CustomItem::HandleCustomItemDescription(ItemTemplate const* pProto, std::string &Description)
{
    std::ostringstream CustomDescription;

    if (pProto->CustomCosts || pProto->CustomOnlinePoints || pProto->CustomReqHonor || pProto->CustomReqArena)
    {
        CustomDescription << "\"\n|cffFFFFFFДля покупки нужно:|r (Количество " << pProto->BuyCount << ")";
        if (pProto->CustomOnlinePoints || pProto->CustomReqHonor || pProto->CustomReqArena)
        {
            CustomDescription << "\n";
            if (pProto->BuyPrice)
                CustomDescription << " Золото: " << to_string(pProto->BuyPrice / GOLD);
            if (pProto->CustomReqArena)
                CustomDescription << " Очки арены: " << to_string(pProto->CustomReqArena);
            if (pProto->CustomReqHonor)
                CustomDescription << " Очки чести: " << to_string(pProto->CustomReqHonor);
            if (pProto->CustomOnlinePoints)
                CustomDescription << "|cffFF009E OPs(Онлайн поинты):|r " << pProto->CustomOnlinePoints << " ";
        }
        // listing needed items START
        if (pProto->CustomCosts)
        {
            CustomDescription << "\n|cffFF0000Требуемые предметы:|r";

            for (uint8 i = 0; i <= 9; ++i)
            {
                if (pProto->CustomItemCost[i].NeedItem)
                {
                    ItemTemplate const* pi = sObjectMgr->GetItemTemplate(pProto->CustomItemCost[i].NeedItem);

                    if (!pi)
                        continue;

                    std::ostringstream ss;
                    ss << "|TInterface";
                    const ItemTemplate* temp = sObjectMgr->GetItemTemplate(pi->ItemId);
                    const ItemDisplayInfoEntry* dispInfo = NULL;
                    if (temp)
                    {
                        dispInfo = sItemDisplayInfoStore.LookupEntry(temp->DisplayInfoID);
                        if (dispInfo)
                            ss << "/ICONS/" << dispInfo->inventoryIcon;
                    }
                    if (!dispInfo)
                        ss << "/InventoryItems/WoWUnknownItem01";
                    ss << ":" << "20"/*width*/ << ":" << "20"/*height*/ << ":" << "0"/*x*/ << ":" << "-3"/*y*/ << "|t";

                    std::ostringstream itemcolored;
                    itemcolored << "|c" << std::hex << ItemQualityColors[pi->Quality] << std::dec << "[" << pi->Name1 << "]|r";

                    CustomDescription << "\n* " << "" << ss.str() << " " << itemcolored.str() << "|cffFFE4E1x" << to_string(pProto->CustomItemCost[i].ItemCount);
                }
            }
        }
        // listing needed items END
        if (pProto->CustomBuyChance == 100)
            CustomDescription << "\n|cffF8FF5FШанс покупки: 100%|r";
        else
            CustomDescription << "\n|cffFF5F5FШанс покупки: " << to_string(pProto->CustomBuyChance) << "%|r";
    }

    Description = CustomDescription.str();
}

// might cause errors without "inline bool"
bool Player::_StoreOrEquipNewItem(uint32 vendorslot, uint32 item, uint8 count, uint8 bag, uint8 slot, int32 price, ItemTemplate const* pProto, Creature* pVendor, VendorItem const* crItem, bool bStore)
{
    ItemPosCountVec vDest;
    uint16 uiDest = 0;
    InventoryResult msg = bStore ?
        CanStoreNewItem(bag, slot, vDest, item, pProto->BuyCount * count) :
        CanEquipNewItem(slot, uiDest, item, false);
    if (msg != EQUIP_ERR_OK)
    {
        SendEquipError(msg, NULL, NULL, item);
        return false;
    }

    bool CustomCanGiveItem = true;

    // if has buy chance && generated number is higher than the buy chance
    if (pProto->CustomBuyChance)
    {
        if (rand() % 100 <= pProto->CustomBuyChance)
        {
            ChatHandler(GetSession()).PSendSysMessage("Item transaction successful");
            GetSession()->SendAreaTriggerMessage("Item transaction successful");
        }
        else
        {
            CustomCanGiveItem = false;
            ChatHandler(GetSession()).PSendSysMessage("Item transaction fail |cffFF0000(chance %u%%)|r - |cffFFFFFF[|Hitem:%u:|h%s|h]|r", pProto->CustomBuyChance, item, pProto->Name1.c_str());
            GetSession()->SendNotification("Item transaction failed");
        }
    }

    ModifyMoney(-price);

    if (crItem->ExtendedCost)                            // case for new honor system
    {
        ItemExtendedCostEntry const* iece = sItemExtendedCostStore.LookupEntry(crItem->ExtendedCost);
        ASSERT(iece);
        if (iece->reqhonorpoints)
            ModifyHonorPoints(-int32(iece->reqhonorpoints * count));

        if (iece->reqarenapoints)
            ModifyArenaPoints(-int32(iece->reqarenapoints * count));

        for (uint8 i = 0; i < MAX_ITEM_EXTENDED_COST_REQUIREMENTS; ++i)
        {
            if (iece->reqitem[i])
                DestroyItemCount(iece->reqitem[i], (iece->reqitemcount[i] * count), true);
        }
    }

    if (pProto->CustomReqHonor)
        ModifyHonorPoints(-int32(pProto->CustomReqHonor));

    if (pProto->CustomReqArena)
        ModifyArenaPoints(-int32(pProto->CustomReqArena));

    for (uint8 i = 0; i <= 9; ++i)
    {
        ItemTemplate const* pi = sObjectMgr->GetItemTemplate(crItem->item);
        if (!pi)
            continue;

        if (pi->CustomItemCost[i].NeedItem)
            DestroyItemCount(pi->CustomItemCost[i].NeedItem, (pi->CustomItemCost[i].ItemCount * count), true);
    }

    if (CustomCanGiveItem)
    {
        Item* it = bStore ?
            StoreNewItem(vDest, item, true) :
            EquipNewItem(uiDest, item, true);
        if (it)
        {
            uint32 new_count = pVendor ? pVendor->UpdateVendorItemCurrentCount(crItem, pProto->BuyCount * count) : 0xFFFFFFFF;

            WorldPacket data(SMSG_BUY_ITEM, (8 + 4 + 4 + 4));
            data << uint64(pVendor ? pVendor->GetGUID() : GetGUID());
            data << uint32(vendorslot + 1);                   // numbered from 1 at client
            data << int32(crItem->maxcount > 0 ? new_count : 0xFFFFFFFF);
            data << uint32(count);
            GetSession()->SendPacket(&data);
            SendNewItem(it, pProto->BuyCount * count, true, false, false);

            if (!bStore)
                AutoUnequipOffhandIfNeed();

            if (pProto->Flags & ITEM_PROTO_FLAG_REFUNDABLE && crItem->ExtendedCost && pProto->GetMaxStackSize() == 1)
            {
                it->SetFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_REFUNDABLE);
                it->SetRefundRecipient(GetGUIDLow());
                it->SetPaidMoney(price);
                it->SetPaidExtendedCost(crItem->ExtendedCost);
                it->SaveRefundDataToDB();
                AddRefundReference(it->GetGUID());
            }
        }
    }
    return true;
}

// Return true is the bought item has a max count to force refresh of window by caller
bool Player::BuyItemFromVendorSlot(uint64 vendorguid, uint32 vendorslot, uint32 item, uint8 count, uint8 bag, uint8 slot)
{
    // cheating attempt
    if (count < 1) count = 1;

    // cheating attempt
    if (slot > MAX_BAG_SIZE && slot != NULL_SLOT)
        return false;

    if (!IsAlive())
        return false;

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item);
    if (!pProto)
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, NULL, item, 0);
        return false;
    }

    if (!(pProto->AllowableClass & getClassMask()) && pProto->Bonding == BIND_WHEN_PICKED_UP && !IsGameMaster())
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, NULL, item, 0);
        return false;
    }

    if (!IsGameMaster() && ((pProto->Flags2 & ITEM_FLAGS_EXTRA_HORDE_ONLY && GetTeam() == ALLIANCE) || (pProto->Flags2 == ITEM_FLAGS_EXTRA_ALLIANCE_ONLY && GetTeam() == HORDE)))
        return false;

    Creature* creature = NULL;
    if (vendorguid != GetGUID())
    {
        creature = GetNPCIfCanInteractWith(vendorguid, UNIT_NPC_FLAG_VENDOR);
        if (!creature)
        {
            TC_LOG_DEBUG("network", "WORLD: BuyItemFromVendor - %s not found or you can't interact with him.", vendorguid);
            SendBuyError(BUY_ERR_DISTANCE_TOO_FAR, NULL, item, 0);
            return false;
        }
    }

    if (vendorguid != GetGUID())
    {
        ConditionList conditions = sConditionMgr->GetConditionsForNpcVendorEvent(creature->GetEntry(), item);
        if (!sConditionMgr->IsObjectMeetToConditions(this, creature, conditions))
        {
            TC_LOG_DEBUG("condition", "BuyItemFromVendor: conditions not met for creature entry %u item %u", creature->GetEntry(), item);
            SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
            return false;
        }
    }
    VendorItemData const* vItems = GetSession()->GetCurrentVendor() ? sObjectMgr->GetNpcVendorItemList(GetSession()->GetCurrentVendor()) : creature->GetVendorItems();
    if (!vItems || vItems->Empty())
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
        return false;
    }

    if (vendorslot >= vItems->GetItemCount())
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
        return false;
    }

    VendorItem const* crItem = vItems->GetItem(vendorslot);
    // store diff item (cheating)
    if (!crItem || crItem->item != item)
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature, item, 0);
        return false;
    }

    // check current item amount if it limited
    if (creature && crItem->maxcount != 0)
    {
        if (creature->GetVendorItemCurrentCount(crItem) < pProto->BuyCount * count)
        {
            SendBuyError(BUY_ERR_ITEM_ALREADY_SOLD, creature, item, 0);
            return false;
        }
    }

    if (pProto->RequiredReputationFaction && (uint32(GetReputationRank(pProto->RequiredReputationFaction)) < pProto->RequiredReputationRank))
    {
        SendBuyError(BUY_ERR_REPUTATION_REQUIRE, creature, item, 0);
        return false;
    }

    // custom honor points
    if (GetHonorPoints() < pProto->CustomReqHonor)
    {
        sCustomItem->SendCustomBuyError(this, creature, crItem);
        SendEquipError(EQUIP_ERR_NOT_ENOUGH_HONOR_POINTS, NULL, NULL);
        return false;
    }

    // custom arena points
    if (GetArenaPoints() < pProto->CustomReqArena)
    {
        sCustomItem->SendCustomBuyError(this, creature, crItem);
        SendEquipError(EQUIP_ERR_NOT_ENOUGH_ARENA_POINTS, NULL, NULL);
        return false;
    }

    // custom online points
    if (GetCustomOnlinePoints() < pProto->CustomOnlinePoints)
    {
        sCustomItem->SendCustomBuyError(this, creature, crItem);
        SendEquipError(EQUIP_ERR_ITEM_LOCKED, NULL, NULL);
        return false;
    }

    // custom item cost
    for (uint8 i = 0; i <= 9; ++i)
    {
        if (pProto->CustomItemCost[i].NeedItem > 0 && !HasItemCount(pProto->CustomItemCost[i].NeedItem, pProto->CustomItemCost[i].ItemCount * count))
        {
            if (ItemTemplate const* pi = sObjectMgr->GetItemTemplate(pProto->CustomItemCost[i].NeedItem))
            {
                sCustomItem->SendCustomBuyError(this, creature, crItem);
                SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, NULL, NULL);
                return false;
            }
        }
    }

    if (crItem->ExtendedCost)
    {
        ItemExtendedCostEntry const* iece = sItemExtendedCostStore.LookupEntry(crItem->ExtendedCost);
        if (!iece)
        {
            TC_LOG_ERROR("entities.player", "Item %u have wrong ExtendedCost field value %u", pProto->ItemId, crItem->ExtendedCost);
            return false;
        }

        // honor points price
        if (GetHonorPoints() < (iece->reqhonorpoints * count))
        {
            sCustomItem->SendCustomBuyError(this, creature, crItem);
            SendEquipError(EQUIP_ERR_NOT_ENOUGH_HONOR_POINTS, NULL, NULL);
            return false;
        }

        // arena points price
        if (GetArenaPoints() < (iece->reqarenapoints * count))
        {
            sCustomItem->SendCustomBuyError(this, creature, crItem);
            SendEquipError(EQUIP_ERR_NOT_ENOUGH_ARENA_POINTS, NULL, NULL);
            return false;
        }

        // item base price
        for (uint8 i = 0; i < MAX_ITEM_EXTENDED_COST_REQUIREMENTS; ++i)
        {
            if (iece->reqitem[i] && !HasItemCount(iece->reqitem[i], (iece->reqitemcount[i] * count)))
            {
                sCustomItem->SendCustomBuyError(this, creature, crItem);
                SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, NULL, NULL);
                return false;
            }
        }

        // check for personal arena rating requirement
        if (GetMaxPersonalArenaRatingRequirement(iece->reqarenaslot) < iece->reqpersonalarenarating)
        {
            // probably not the proper equip err
            SendEquipError(EQUIP_ERR_CANT_EQUIP_RANK, NULL, NULL);
            return false;
        }
    }

    uint32 price = 0;
    if (crItem->IsGoldRequired(pProto) && pProto->BuyPrice > 0) //Assume price cannot be negative (do not know why it is int32)
    {
        uint32 maxCount = MAX_MONEY_AMOUNT / pProto->BuyPrice;
        if ((uint32)count > maxCount)
        {
            TC_LOG_ERROR("entities.player", "Player %s tried to buy %u item id %u, causing overflow", GetName().c_str(), (uint32)count, pProto->ItemId);
            count = (uint8)maxCount;
        }
        price = pProto->BuyPrice * count; //it should not exceed MAX_MONEY_AMOUNT

        // reputation discount
        price = creature ? uint32(floor(price * GetReputationPriceDiscount(creature))) : price;

        if (!HasEnoughMoney(price))
        {
            sCustomItem->SendCustomBuyError(this, creature, crItem);
            SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, item, 0);
            return false;
        }
    }

    if ((bag == NULL_BAG && slot == NULL_SLOT) || IsInventoryPos(bag, slot))
    {
        if (!_StoreOrEquipNewItem(vendorslot, item, count, bag, slot, price, pProto, creature, crItem, true))
            return false;
    }
    else if (IsEquipmentPos(bag, slot))
    {
        if (pProto->BuyCount * count != 1)
        {
            SendEquipError(EQUIP_ERR_ITEM_CANT_BE_EQUIPPED, NULL, NULL);
            return false;
        }
        if (!_StoreOrEquipNewItem(vendorslot, item, count, bag, slot, price, pProto, creature, crItem, false))
            return false;
    }
    else
    {
        SendEquipError(EQUIP_ERR_ITEM_DOESNT_GO_TO_SLOT, NULL, NULL);
        return false;
    }

    return crItem->maxcount != 0;
}
#endif