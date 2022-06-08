#include "ScriptPCH.h"
#include "Guild.h"

static const bool send_cache_packets = true;    // change player cache?

// Remember to add to GetStatName too
uint32 price = 10;
static const uint32 tokenEntry      = 90072;    // token entry
static const uint32 tokenEntrytoDoubleStat      = 90073;    // double stat token entry
static const uint8 maxReforges      = 2;        // maximum reforges for one item
static const uint32 statTypes[] = { ITEM_MOD_AGILITY, ITEM_MOD_STRENGTH, ITEM_MOD_INTELLECT, ITEM_MOD_STAMINA, /*ITEM_MOD_HASTE_MELEE_RATING, ITEM_MOD_HASTE_RATING,*/ ITEM_MOD_ATTACK_POWER, ITEM_MOD_SPELL_POWER };
static const uint32 stat_type_max = sizeof(statTypes) / sizeof(*statTypes);
static const uint32 stat_to_increase[] = {1000,1000,1500,1000,1060,870};
typedef std::map<uint32, uint32> itemGuidMap;
typedef UNORDERED_MAP<uint32, itemGuidMap> playerItemMap;
static playerItemMap playerItems; // playerItems[plrguid][itemguid] = entry;

namespace
{
    static const char* GetStatName(uint32 ItemStatType)
    {
        switch(ItemStatType)
        {
        case ITEM_MOD_AGILITY                   : return "Ловкость"; break;
        case ITEM_MOD_STRENGTH             	: return "Сила"; break;
        case ITEM_MOD_INTELLECT             	: return "Интелект"; break;
        case ITEM_MOD_STAMINA               	: return "Выносливость"; break;
        //case ITEM_MOD_HASTE_MELEE_RATING        : return "Скорость ближнего боя"; break;
        //case ITEM_MOD_HASTE_RATING              : return "Скорость"; break;
        case ITEM_MOD_ATTACK_POWER             	: return "Сила атаки"; break;
        case ITEM_MOD_SPELL_POWER         	: return "Сила заклинаний"; break;
        default: return NULL;
        }
    }

    static const char* GetSlotName(uint8 slot, WorldSession* session)
    {
        switch (slot)
        {
        case EQUIPMENT_SLOT_HEAD      : return "Голова";
        case EQUIPMENT_SLOT_SHOULDERS : return "Наплечники";
        case EQUIPMENT_SLOT_CHEST     : return "Нагрудник";
        case EQUIPMENT_SLOT_WAIST     : return "Пояс";
        case EQUIPMENT_SLOT_LEGS      : return "Штаны";
        case EQUIPMENT_SLOT_FEET      : return "Ботинки";
        case EQUIPMENT_SLOT_WRISTS    : return "Браслеты";
        case EQUIPMENT_SLOT_HANDS     : return "Рукавицы";
        case EQUIPMENT_SLOT_BACK      : return "Спина";
        case EQUIPMENT_SLOT_MAINHAND  : return "Правая рука";
        case EQUIPMENT_SLOT_OFFHAND   : return "Левая рука";
        case EQUIPMENT_SLOT_RANGED    : return "Дальний бой";
        default: return NULL;
        }
    }

    std::string GetItemName(uint32 entry, WorldSession* session)
    {
        const ItemTemplate* itemTemplate = sObjectMgr->GetItemTemplate(entry);
        std::string name = itemTemplate->Name1;
        int loc_idx = session->GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
            if (ItemLocale const* il = sObjectMgr->GetItemLocale(itemTemplate->ItemId))
                sObjectMgr->GetLocaleString(il->Name, loc_idx, name);
        return name;
    }

    static Item* GetEquippedItem(Player* player, uint32 guidlow)
    {
        for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                if (pItem->GetGUIDLow() == guidlow)
                    return pItem;
        return NULL;
    }

    static void RemoveReforgeSave(Player* player, uint32 itemguid, bool update, bool database);
    static void SendReforgePacket(Player* player, uint32 entry, uint32 lowguid = 0)
    {
        ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(entry);
        FakeStatStruct* reforge = NULL;
        if (lowguid)
        {
            if (sObjectMgr->_itemFakeStatStore.find(lowguid) != sObjectMgr->_itemFakeStatStore.end())
                reforge = &sObjectMgr->_itemFakeStatStore[lowguid];
            else
                RemoveReforgeSave(player, lowguid, false, true);
        }

        // Update player cache (self only) pure visual.
        // HandleItemQuerySingleOpcode copy paste
        std::string Name        = pProto->Name1;
        std::string Description = pProto->Description;
        int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (ItemLocale const* il = sObjectMgr->GetItemLocale(pProto->ItemId))
            {
                ObjectMgr::GetLocaleString(il->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(il->Description, loc_idx, Description);
            }
        }
        WorldPacket data(SMSG_ITEM_QUERY_SINGLE_RESPONSE, 600);
        data << pProto->ItemId;
        data << pProto->Class;
        data << pProto->SubClass;
        data << pProto->SoundOverrideSubclass;
        data << Name;
        data << uint8(0x00);                                //pProto->Name2; // blizz not send name there, just uint8(0x00); <-- \0 = empty string = empty name...
        data << uint8(0x00);                                //pProto->Name3; // blizz not send name there, just uint8(0x00);
        data << uint8(0x00);                                //pProto->Name4; // blizz not send name there, just uint8(0x00);
        data << pProto->DisplayInfoID;
        data << pProto->Quality;
        data << pProto->Flags;
        data << pProto->Flags2;
        data << pProto->BuyPrice;
        data << pProto->SellPrice;
        data << pProto->InventoryType;
        data << pProto->AllowableClass;
        data << pProto->AllowableRace;
        data << pProto->ItemLevel;
        data << pProto->RequiredLevel;
        data << pProto->RequiredSkill;
        data << pProto->RequiredSkillRank;
        data << pProto->RequiredSpell;
        data << pProto->RequiredHonorRank;
        data << pProto->RequiredCityRank;
        data << pProto->RequiredReputationFaction;
        data << pProto->RequiredReputationRank;
        data << int32(pProto->MaxCount);
        data << int32(pProto->Stackable);
        data << pProto->ContainerSlots;
        data << pProto->StatsCount;
        for (uint32 i = 0; i < pProto->StatsCount; ++i)
        {
            data << pProto->ItemStat[i].ItemStatType;
             if(sObjectMgr->itemFakeStatStore[lowguid].find(pProto->ItemStat[i].ItemStatType) != sObjectMgr->itemFakeStatStore[lowguid].end())
               data << pProto->ItemStat[i].ItemStatValue + sObjectMgr->itemFakeStatStore[lowguid][pProto->ItemStat[i].ItemStatType];
            else
                data << pProto->ItemStat[i].ItemStatValue;
        }

        data << pProto->ScalingStatDistribution;            // scaling stats distribution
        data << pProto->ScalingStatValue;                   // some kind of flags used to determine stat values column
        for (int i = 0; i < MAX_ITEM_PROTO_DAMAGES; ++i)
        {
            data << pProto->Damage[i].DamageMin;
            data << pProto->Damage[i].DamageMax;
            data << pProto->Damage[i].DamageType;
        }

        // resistances (7)
        data << pProto->Armor;
        data << pProto->HolyRes;
        data << pProto->FireRes;
        data << pProto->NatureRes;
        data << pProto->FrostRes;
        data << pProto->ShadowRes;
        data << pProto->ArcaneRes;

        data << pProto->Delay;
        data << pProto->AmmoType;
        data << pProto->RangedModRange;

        for (int s = 0; s < MAX_ITEM_PROTO_SPELLS; ++s)
        {
            // send DBC data for cooldowns in same way as it used in Spell::SendSpellCooldown
            // use `item_template` or if not set then only use spell cooldowns
            SpellInfo const* spell = sSpellMgr->GetSpellInfo(pProto->Spells[s].SpellId);
            if (spell)
            {
                bool db_data = pProto->Spells[s].SpellCooldown >= 0 || pProto->Spells[s].SpellCategoryCooldown >= 0;

                data << pProto->Spells[s].SpellId;
                data << pProto->Spells[s].SpellTrigger;
                data << uint32(-abs(pProto->Spells[s].SpellCharges));

                if (db_data)
                {
                    data << uint32(pProto->Spells[s].SpellCooldown);
                    data << uint32(pProto->Spells[s].SpellCategory);
                    data << uint32(pProto->Spells[s].SpellCategoryCooldown);
                }
                else
                {
                    data << uint32(spell->RecoveryTime);
                    data << uint32(spell->GetCategory());
                    data << uint32(spell->CategoryRecoveryTime);
                }
            }
            else
            {
                data << uint32(0);
                data << uint32(0);
                data << uint32(0);
                data << uint32(-1);
                data << uint32(0);
                data << uint32(-1);
            }
        }
        data << pProto->Bonding;
        data << Description;
        data << pProto->PageText;
        data << pProto->LanguageID;
        data << pProto->PageMaterial;
        data << pProto->StartQuest;
        data << pProto->LockID;
        data << int32(pProto->Material);
        data << pProto->Sheath;
        data << pProto->RandomProperty;
        data << pProto->RandomSuffix;
        data << pProto->Block;
        data << pProto->ItemSet;
        data << pProto->MaxDurability;
        data << pProto->Area;
        data << pProto->Map;                                // Added in 1.12.x & 2.0.1 client branch
        data << pProto->BagFamily;
        data << pProto->TotemCategory;
        for (int s = 0; s < MAX_ITEM_PROTO_SOCKETS; ++s)
        {
            data << pProto->Socket[s].Color;
            data << pProto->Socket[s].Content;
        }
        data << pProto->socketBonus;
        data << pProto->GemProperties;
        data << pProto->RequiredDisenchantSkill;
        data << pProto->ArmorDamageModifier;
        data << pProto->Duration;                           // added in 2.4.2.8209, duration (seconds)
        data << pProto->ItemLimitCategory;                  // WotLK, ItemLimitCategory
        data << pProto->HolidayId;                          // Holiday.dbc?
        player->GetSession()->SendPacket(&data);
    }

    static void SendReforgePackets(Player* player)
    {
        if (!send_cache_packets)
            return;
        if (playerItems.find(player->GetGUIDLow()) == playerItems.end())
            return;
        for (itemGuidMap::const_iterator it = playerItems[player->GetGUIDLow()].begin(); it != playerItems[player->GetGUIDLow()].end();)
        {
            itemGuidMap::const_iterator old_it = it++;
            SendReforgePacket(player, old_it->second, old_it->first);
        }
    }

    static void RemoveReforgeSave(Player* player, uint32 itemguid, bool update, bool database)
    {
        uint32 lowguid = player->GetGUIDLow();
        Item* invItem = update ? player->GetItemByGuid(MAKE_NEW_GUID(itemguid, 0, HIGHGUID_ITEM)) : NULL;
        if (invItem)
            player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
        if (playerItems.find(lowguid) != playerItems.end())
            playerItems[lowguid].erase(itemguid);
        sObjectMgr->itemFakeStatStore.erase(itemguid);
        if (invItem)
            player->_ApplyItemMods(invItem, invItem->GetSlot(), true);

        if (!database)
            return;
        CharacterDatabase.PExecute("DELETE FROM `custom_reforges` WHERE `GUID` = %u", itemguid);
        if (invItem)
            SendReforgePacket(player, invItem->GetEntry());
        player->SaveToDB();
    }

    static bool IsReforgable(Item* invItem, Player* player)
    {
        /*if (!invItem->IsEquipped())
        return false;*/
        if (invItem->GetOwnerGUID() != player->GetGUID())
            return false;
        if(sObjectMgr->itemFakeStatStore.find(invItem->GetGUIDLow()) != sObjectMgr->itemFakeStatStore.end())

           if (player->HasItemCount(tokenEntrytoDoubleStat,1))
             {
               if(sObjectMgr->itemFakeStatStore[invItem->GetGUIDLow()].size() >= 2)
                 return false;
             }

           else
             {
               if (sObjectMgr->itemFakeStatStore[invItem->GetGUIDLow()].size() >= 1)
                return false;
             }

        const ItemTemplate* pProto = invItem->GetTemplate();
        //if (pProto->ItemLevel < 200) // WOTLK items have less itemlevel than CATA
        //    return false;
        if (pProto->Quality == ITEM_QUALITY_HEIRLOOM) // block heirlooms necessary? probably.
            return false;
        if (!pProto->StatsCount || pProto->StatsCount >= 11)
            return false;
        //if (sObjectMgr->_itemFakeStatStore.find(invItem->GetGUIDLow()) != sObjectMgr->_itemFakeStatStore.end())
            //return false;
        for (uint32 i = 0; i < pProto->StatsCount; ++i)
        {
            if (!GetStatName(pProto->ItemStat[i].ItemStatType))
                continue;

            if (((int32)floorf((float)pProto->ItemStat[i].ItemStatValue * 0.4f)) > 1)
                return true;
        }
        return false;
    }

    static void UpdatePlayerReforgeStats(Item* invItem, Player* player, uint32 decrease, uint32 increase) // stat types
    {
        const ItemTemplate* pProto = invItem->GetTemplate();

        int32 stat_diff = 0;

            for (uint32 i = 0; i < stat_type_max; ++i)
                  if (statTypes[i] == increase)
                   stat_diff = stat_to_increase[i] * decrease;

        if (stat_diff <= 0)
            return; // Should have some kind of diff

        // Update player stats
        player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
        uint32 guidlow = invItem->GetGUIDLow();
        sObjectMgr->itemFakeStatStore[guidlow][increase] = stat_diff;
        sObjectMgr->_itemFakeStatStore[guidlow].increase = increase;
        sObjectMgr->_itemFakeStatStore[guidlow].decrease = decrease;
        sObjectMgr->_itemFakeStatStore[guidlow].stat_value = stat_diff;
        player->_ApplyItemMods(invItem, invItem->GetSlot(), true);
        CharacterDatabase.PExecute("REPLACE INTO `custom_reforges` (`GUID`, `increase`, `decrease`, `stat_value`) VALUES (%u, %u, %u, %i)", guidlow, increase, decrease, stat_diff);
        SendReforgePacket(player, invItem->GetEntry(), guidlow);
        player->SaveToDB();
    }
}

class REFORGE_PLAYER : public PlayerScript
{
public:
    REFORGE_PLAYER() : PlayerScript("REFORGE_PLAYER")
    {
        CharacterDatabase.Execute("DELETE FROM `custom_reforges` WHERE NOT EXISTS (SELECT 1 FROM `item_instance` WHERE `item_instance`.`guid` = `custom_reforges`.`GUID`)");
    }

    class SendRefPackLogin : public BasicEvent
    {
    public:
        SendRefPackLogin(Player* _player) : player(_player)
        {
            _player->m_Events.AddEvent(this, _player->m_Events.CalculateTime(1000));
        }

        bool Execute(uint64, uint32)
        {
            SendReforgePackets(player);
            return true;
        }
        Player* player;
    };

    void OnLogin(Player* player)
    {
        uint32 playerGUID = player->GetGUIDLow();
        QueryResult result = CharacterDatabase.PQuery("SELECT custom_reforges.`GUID`, `increase`, `decrease`, `stat_value`, item_instance.itementry FROM `custom_reforges`, item_instance WHERE item_instance.`owner_guid` = %u and item_instance.`guid` = custom_reforges.GUID", playerGUID);
        if (result)
        {
            do
            {
                uint32 lowGUID = (*result)[0].GetUInt32();
                uint32 entry = (*result)[4].GetUInt32();
                uint32 increase = (*result)[1].GetUInt32();

                Item* invItem = player->GetItemByGuid(MAKE_NEW_GUID(lowGUID, 0, HIGHGUID_ITEM));
                if (invItem)
                    player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
                sObjectMgr->_itemFakeStatStore[lowGUID].increase = (*result)[1].GetUInt32();
                sObjectMgr->_itemFakeStatStore[lowGUID].decrease = (*result)[2].GetUInt32();
                sObjectMgr->itemFakeStatStore[lowGUID][increase] = (*result)[3].GetInt32();
                playerItems[playerGUID][lowGUID] = entry;
                if (invItem)
                    player->_ApplyItemMods(invItem, invItem->GetSlot(), true);
                // SendReforgePacket(player, entry, lowGUID);
            } while (result->NextRow());

            // SendReforgePackets(player);
            new SendRefPackLogin(player);
        }
    }

    void OnLogout(Player* player)
    {
        if (playerItems.find(player->GetGUIDLow()) == playerItems.end())
            return;
        for (itemGuidMap::const_iterator it = playerItems[player->GetGUIDLow()].begin(); it != playerItems[player->GetGUIDLow()].end();)
        {
            itemGuidMap::const_iterator old_it = it++;
            RemoveReforgeSave(player, old_it->first, false, false);
        }
    }
};

class REFORGER_NPC : public CreatureScript
{
public:
    REFORGER_NPC() : CreatureScript("REFORGER_NPC") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        selectedItem.erase(player->GetGUIDLow());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Выберите какую вещь будем улучшать:", MAIN_MENU, 0);
        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
        {
            if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                if (IsReforgable(invItem, player))
                    if (const char* slotname = GetSlotName(slot, player->GetSession()))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, slotname, SELECT_STAT_INCREASE, slot);
        }
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Сбросить улучшение", SELECT_RESTORE, 0);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Меню апдейтов", MAIN_MENU, 0);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(sender)
        {
        case MAIN_MENU: OnGossipHello(player, creature); break;
        case SELECT_STAT_INCREASE:
            // action = StatsCount id
            if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action))
            {
                if (IsReforgable(invItem, player))
                {
                   selectedItem[player->GetGUIDLow()] = invItem->GetGUIDLow();
                   const ItemTemplate* pProto = invItem->GetTemplate();
                   char label[250];

                    snprintf(label, 250, "Выбранный предмет:\n%s", invItem->GetTemplate()->Name1.c_str());
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, label, sender, action);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Выберите какой стат будем увеличивать:", sender, action);
                    bool hasfakestats = ((sObjectMgr->itemFakeStatStore.find(invItem->GetGUIDLow()) != sObjectMgr->itemFakeStatStore.end()) ? true : false);

                    for (uint32 i = 0; i < stat_type_max; ++i)
                    {
                        bool cont = false;
                        for (uint32 j = 0; j < pProto->StatsCount; ++j)
                        {
                            if (j==pProto->StatsCount-1 && statTypes[i] != pProto->ItemStat[j].ItemStatType)
                                cont = true;

                            if (statTypes[i] == pProto->ItemStat[j].ItemStatType) // skip non-existing stats on item
                                break;
                        }

                        if (cont)
                            continue;

                       if(hasfakestats && sObjectMgr->itemFakeStatStore[invItem->GetGUIDLow()].find(statTypes[i]) != sObjectMgr->itemFakeStatStore[invItem->GetGUIDLow()].end())
                            continue; // already modded this stat, skip.

                        if (const char* stat_name = GetStatName(statTypes[i]))
                         player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, stat_name, SELECT_TOKEN_COST, i);
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Назад..", MAIN_MENU, 0);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                }
                else
                {
                    player->GetSession()->SendNotification("Ошибка улучшения");
                    OnGossipHello(player, creature);
                }
              }
            else
            {
                player->GetSession()->SendNotification("Предмет для улучшения не выбран");
                OnGossipHello(player, creature);
            }
            break;

        case SELECT_TOKEN_COST:
            // action = stat_count_id
            {
                Item* invItem = GetEquippedItem(player, selectedItem[player->GetGUIDLow()]);
                if (invItem && invItem->IsInWorld() && IsReforgable(invItem, player))
                {
                    const ItemTemplate* proto = invItem->GetTemplate();
                    char label[250];
                    char popup[250];

                    snprintf(label, 250, "Выбранный предмет:\n%s", invItem->GetTemplate()->Name1.c_str());
                    snprintf(label, 250, "Выбранный стат: %s", GetStatName(statTypes[action]));
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, label, sender, action);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Выберите как будем улучшать:", sender, action);
 
                    price = 10;
                    //Guild-Level-System (Bonus: Reforging)
                    if(Guild* guild = player->GetGuild())
                       if (guild->HasLevelForBonus(GUILD_BONUS_PRICE_REFORGING))
                             price -=2;

                    for (uint32 i = 1; i <= 2; ++i)
                    {

                         if (player->HasItemCount(tokenEntrytoDoubleStat,1))
                         {
                          snprintf(label, 250, "+ %i за %u x %s ", stat_to_increase[action]*i, i*price, GetItemName(tokenEntry, player->GetSession()).c_str());
                          snprintf(popup, 250, "Вы уверены что хотите улучшить этот предмет\n\n за %i х %s ?\n\n", i*price, GetItemName(tokenEntry, player->GetSession()).c_str());
                          player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, label, i, statTypes[action], popup, 0, false); // sender = token_count, action = stat_count_id
                         }
                         else
                         {
                          snprintf(label, 250, "+ %i за %u x %s ", stat_to_increase[action]*i, i*price, GetItemName(tokenEntry, player->GetSession()).c_str());
                          snprintf(popup, 250, "Вы уверены что хотите улучшить этот предмет\n\n за %i х %s ?\n\n", i*price, GetItemName(tokenEntry, player->GetSession()).c_str());
                          player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, label, i, statTypes[action], popup, 0, false); // sender = token_count, action = stat_count_id
                          break;
                         }
                    }
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Назад..", SELECT_STAT_INCREASE, invItem->GetSlot());
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                }
                else
                {
                    player->GetSession()->SendNotification("Неверно выбранный предмет");
                    OnGossipHello(player, creature);
                }
            }
            break;

        case SELECT_RESTORE:
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Выберите предмет для которого хотите сбросить улучшение:", sender, action);
                for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
                {
                    if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                        if (sObjectMgr->itemFakeStatStore.find(invItem->GetGUIDLow()) != sObjectMgr->itemFakeStatStore.end())
                            if (const char* slotname = GetSlotName(slot, player->GetSession()))
                                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, slotname, RESTORE, slot, "Действительно сбросить улучшение для\n\n"+std::string(slotname), 0, false);
                }
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Меню апдейтов", sender, action);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Назад..", MAIN_MENU, 0);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            break;
        case RESTORE:
            // action = slot
            {
                if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action))
                {
                    if (sObjectMgr->_itemFakeStatStore.find(invItem->GetGUIDLow()) != sObjectMgr->_itemFakeStatStore.end())
                        RemoveReforgeSave(player, invItem->GetGUIDLow(), true, true);
                }
                OnGossipHello(player, creature);
            }
            break;
        default: // Reforge
            // sender = stat type to decrease, action = stat type to increase
            {
                Item* invItem = GetEquippedItem(player, selectedItem[player->GetGUIDLow()]);
                if (invItem && IsReforgable(invItem, player))
                {
                    if (sender)
                    {
                        if(player->HasItemCount(tokenEntry, sender*price))
                        {
                           player->DestroyItemCount(tokenEntry, sender*price, true);
                           UpdatePlayerReforgeStats(invItem, player, sender, action); // rewrite this function
                        }
                         else
                        {
                            player->GetSession()->SendNotification("Не хватает эмблем");        
                            player->PlayerTalkClass->SendCloseGossip();
                            return true;
                        }
                    }
                      else
                        player->GetSession()->SendNotification("Неверное количество эмблем");
                 }
                else
                    player->GetSession()->SendNotification("Ошибка улучшения предмета");
                OnGossipHello(player, creature);
            }
        }
        return true;
    }

    enum Senders
    {
        MAIN_MENU = 100,
        SELECT_ITEM,
        SELECT_STAT_INCREASE,
        SELECT_TOKEN_COST,
        SELECT_RESTORE,
        RESTORE,
        REFORGE,
    };

private:

    UNORDERED_MAP<uint32, uint32> selectedItem;
};

void AddSC_REFORGER_NPC()
{
    new REFORGER_NPC;
    new REFORGE_PLAYER;
}

#undef FOR_REFORGE_ITEMS
