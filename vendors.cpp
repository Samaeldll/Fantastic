#include "ScriptPCH.h"
#include "ScriptedGossip.h"
#include "Transmogrification.h"
#include "Guild.h"
#include "Pet.h"
#include "CustomItem.h"

uint32 _guidbosses[5] = {9465074, 9480422, 9475945, 9477284, 9477471};

class npc_weapon: public CreatureScript
{
public:
    npc_weapon() : CreatureScript("npc_weapon") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
	if (player->IsInCombat())
	{
		player->PlayerTalkClass->SendCloseGossip();
		creature->MonsterWhisper("|TInterface/ICONS/Achievement_general:25|tВы в бою!", player);
		return 0;
	}

              player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\\ICONS\\inv_mace_87:25:25:-18:0|tPvP Оружие I", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\ICONS\\inv_mace_87:25:25:-18:0|tPvP Оружие II", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\ICONS\\inv_mace_87:25:25:-18:0|tPvP Оружие III", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\ICONS\\inv_mace_87:25:25:-18:0|tPvP Оружие IV", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\ICONS\\inv_mace_87:25:25:-18:0|tPvP Оружие V", GOSSIP_SENDER_MAIN, 5);
                player->PlayerTalkClass->SendGossipMenu(50004,creature->GetGUID());
                return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
     {
	    player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
		case 1:
	        player->GetSession()->SendListInventory(creature->GetGUID(), 100040);
                break;

		case 2:
	        player->GetSession()->SendListInventory(creature->GetGUID(), 100028);
                break;

		case 3:
	        player->GetSession()->SendListInventory(creature->GetGUID(), 100038);
                break;

		case 4:
	       player->GetSession()->SendListInventory(creature->GetGUID(), 100005);
                break;

		case 5:
	       player->GetSession()->SendListInventory(creature->GetGUID(), 100045);
                break;
        }
        return true;

      }
};

class npc_event_rew: public CreatureScript
{
public:
    npc_event_rew() : CreatureScript("npc_event_rew") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
	if (player->IsInCombat())
	{
		player->PlayerTalkClass->SendCloseGossip();
		creature->MonsterWhisper("Вы в бою!", player);
		return 0;
	}

              player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\\ICONS\\inv_helmet_24:25:25:-18:0|tВещи", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\ICONS\\inv_misc_idol_05:25:25:-18:0|tРазное", GOSSIP_SENDER_MAIN, 2);
                player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
                return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
     {
	    player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
		case 1:
	        player->GetSession()->SendListInventory(creature->GetGUID(), 100056);
                break;

		case 2:
	        player->GetSession()->SendListInventory(creature->GetGUID(), 100057);
                break;
        }
        return true;

      }
};


class npc_guild: public CreatureScript
{
public:
    npc_guild() : CreatureScript("npc_guild") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
       uint32 playerGUID = player->GetGUIDLow();
       uint32 guild_talent = 0;
       WorldSession* session = player->GetSession();

	if (player->IsInCombat())
	{
		player->PlayerTalkClass->SendCloseGossip();
		creature->MonsterWhisper("Вы в бою!", player);
		return 0;
	}

              player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface\\PaperDollInfoFrame\\UI-GearManager-Undo:30:30:-18:0|tЯ хочу обменять камни опыта на опыт гильдии", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2, "Введите кол-во " + sTransmogrification->GetItemLink(90097, session) + "которое хотите обменять на опыт гильдии.\n\n 1 камень = 5000 опыта.", 0, true);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\icons\\inv_misc_questionmark:30:30:-18:0|tИнформация про бонусы", EQUIPMENT_SLOT_END+9, 0);
               
               PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CUSTOM_TALENT);
               stmt->setUInt32(0, playerGUID); 
               if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
                 {

                    Field* fields = result->Fetch();
                    guild_talent = fields[1].GetUInt32();
                 }

              if (guild_talent == 0)
              player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\icons\\inv_misc_ribbon_01:30:30:-18:0|tПолучить дополнительный талант", EQUIPMENT_SLOT_END+11, 0);
              player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\icons\\inv_misc_ribbon_01:30:30:-18:0|tНаграды за 8 уровень", EQUIPMENT_SLOT_END+10, 0);
              player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
              return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        uint32 playerGUID = player->GetGUIDLow();
        Guild* guild = player->GetGuild();
        uint8 level;
         if(guild) level = guild->GetLevel();
           else level = 1;
        bool talent = false;

        std::string str = "|TInterface/ICONS/Achievement_general:25|t|cffADFF2FУровень вашей гильдии: |r" + to_string(level) + "|cffADFF2F.|r";
        player->PlayerTalkClass->ClearMenus();
        
        switch(sender)
        {

          case EQUIPMENT_SLOT_END+11:
         {
          if (guild)
           {
            if (level >= 6)
              {
               uint32 player_custom_talent = 0;
               uint32 guild_talent = 0;

               PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CUSTOM_TALENT);
               stmt->setUInt32(0, playerGUID); 
               if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
                 {

                    Field* fields = result->Fetch();
                    player_custom_talent = fields[0].GetUInt32();
                    guild_talent = fields[1].GetUInt32();
                 }

                    player->SetFreeTalentPoints(1);
                    player->SendTalentsInfoData(false);

                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CUSTOM_TALENT);
                    stmt->setUInt32(0, playerGUID);
                    stmt->setUInt32(1, player_custom_talent);
                    stmt->setUInt32(2, 1);
                    CharacterDatabase.Execute(stmt); 
                    player->CLOSE_GOSSIP_MENU();
              } else {
                  player->GetSession()->SendNotification(str.c_str());
                  player->CLOSE_GOSSIP_MENU();
                 } 
           } else {
                player->GetSession()->SendNotification("|cffADFF2FВы не состоите в гильдии.");
                player->CLOSE_GOSSIP_MENU();
               }             

         } break;

          case EQUIPMENT_SLOT_END+10:
           {
            if(guild)
              {
               if (level < 8)
                 {
                  player->GetSession()->SendNotification(str.c_str());
                  player->CLOSE_GOSSIP_MENU();
                 }
               else
               player->GetSession()->SendListInventory(creature->GetGUID(), 100066);
              }
           else 
              {
                player->GetSession()->SendNotification("|cffADFF2FВы не состоите в гильдии.");
                player->CLOSE_GOSSIP_MENU();
              }
           } break;

          case EQUIPMENT_SLOT_END+9:
             //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tНазад..", EQUIPMENT_SLOT_END+1, 0);
             //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Test", EQUIPMENT_SLOT_END+2, 0);
             player->SEND_GOSSIP_MENU(50002, creature->GetGUID());
             break;

        }
 
      return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code)
     {
        player->PlayerTalkClass->ClearMenus();
        WorldSession* session = player->GetSession();  
 
        int count = atoi(code);
        switch (action)
        {

          case GOSSIP_ACTION_INFO_DEF+2:
           {
            if (Guild* guild = player->GetGuild())
             {
              //if(player->GetGUID() == guild->GetLeaderGUID())
               //{
                if (player->GetItemCount(90097) >= count)
                  {
                     uint32 guild_xp_base = 5000;
                     uint32 guild_xp = 5000;
                     std::ostringstream ss;
                     std::string str;

	             player->DestroyItemCount(90097,count,true);
                      if (guild->HasLevelForBonus(GUILD_BONUS_XP_1))
                       guild_xp = uint32(guild_xp_base*1.05f);
                      if (guild->HasLevelForBonus(GUILD_BONUS_XP_2))
                       guild_xp = uint32(guild_xp_base*1.1f);
                    guild->GiveXp(count*guild_xp);

                     ss << "|TInterface/ICONS/Achievement_general:25|t|cffADFF2FВы внесли:|r |cffff0000" << count << " |r" << sTransmogrification->GetItemLink(90097, session)
                        << "|TInterface/ICONS/Achievement_general:25|t|cffADFF2F. Уровень вашей гильдии:|r |cffff0000" << to_string(guild->GetLevel()) << "|r" << "|cffADFF2F. Опыт вашей гильдии:|r |cffff0000" << to_string(guild->GetCurrentXP()) << "|r |cffADFF2F.|r";

                    str = ss.str();
                    player->GetSession()->SendNotification(str.c_str());
                    player->CLOSE_GOSSIP_MENU();
                  }
                else 
                   {
                     player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffADFF2FУ вас недостаточно камней опыта.");
                     player->CLOSE_GOSSIP_MENU();
                   }
               //}
                //else 
                  //{
                     //player->GetSession()->SendNotification("|cffADFF2FВы не лидер гильдии.");
                     //player->CLOSE_GOSSIP_MENU();
                  //}
             }
             else
               {
                 player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffADFF2FВы не состоите в гильдии.");
                 player->CLOSE_GOSSIP_MENU();
               }
           }break;

        }
        return true;
    }

};

class npc_bonus_respawn_time: public CreatureScript
{
public:
    npc_bonus_respawn_time() : CreatureScript("npc_bonus_respawn_time") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {

	if (player->IsInCombat())
	{
		player->PlayerTalkClass->SendCloseGossip();
		creature->MonsterWhisper("|TInterface/ICONS/Achievement_general:25|tВы в бою!", player);
		return 0;
	}

                player->PlayerTalkClass->ClearMenus();
		  player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\icons\\inv_helm_mask_zulgurub_d_01:30:30:-18:0|tУфориус Фрост", GOSSIP_SENDER_MAIN, _guidbosses[0]); 
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\icons\\inv_helm_mask_zulgurub_d_01:30:30:-18:0|tАдский крушитель", GOSSIP_SENDER_MAIN, _guidbosses[1]);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\icons\\inv_helm_mask_zulgurub_d_01:30:30:-18:0|tАрхангел Михаил", GOSSIP_SENDER_MAIN, _guidbosses[2]); 
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\icons\\inv_helm_mask_zulgurub_d_01:30:30:-18:0|tАзог Завоеватель", GOSSIP_SENDER_MAIN, _guidbosses[3]); 
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\icons\\inv_helm_mask_zulgurub_d_01:30:30:-18:0|tАрхител", GOSSIP_SENDER_MAIN, _guidbosses[4]);  
                player->SEND_GOSSIP_MENU(50003, creature->GetGUID());
                return true;
    }  

    bool GetRespawnBonusTime(Creature* creature, uint32 guid, Player* player)
     {
               PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CREATURE_RESPAWNS_BONUS_TIME);
               stmt->setUInt32(0, guid); 
               if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
                 {

                    Field* fields = result->Fetch();
                    uint32 resp_time = fields[0].GetUInt32();
                    uint32 bonus_time = fields[1].GetUInt32();
                    uint32 _resp_15 = resp_time - time(NULL);

                    if (_resp_15 < 900)
                        {
                         player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffADFF2FДо респавна босса осталось менее чем |cff00ccff15 минут|r|cffADFF2F. Уменьшить респавн невозможно.");
                         return false; 
                        }

                    if ((_resp_15-300) < 900)
                        {
                         player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffADFF2FУменьшить время респавна босса невозможно. У босса должно оставаться более чем |cff00ccff15 минут|r |cffADFF2Fдо респавна.");
                         return false; 
                        }

                    if (bonus_time >=3600)
                       {
                        player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffADFF2FВремя респавна уменьшено на максимум (|cff00ccff1 час|r|cffADFF2F).");
                        return false;
                       }
                   
                    bonus_time += 300;
                    resp_time -= 300; 

                   stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CREATURE_RESPAWNS_BONUS_TIME);
                   stmt->setUInt32(0, resp_time);
                   stmt->setUInt32(1, bonus_time);
                   stmt->setUInt32(2, guid);
                   CharacterDatabase.Execute(stmt);
                 }  

      return true;

     }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
     {
	    player->PlayerTalkClass->ClearMenus();
           WorldSession* session = player->GetSession();
           uint64 guid = action;
           std::ostringstream ss;
           std::string str;


     if (Creature* cr = HashMapHolder<Creature>::FindCreature(guid))
      {
        if (cr->IsAlive())
            {
              player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffADFF2FНельзя уменьшить время до респавна босса пока он жив.");
              player->CLOSE_GOSSIP_MENU();
              return false;
            }

        if ((player->GetItemCount(90094) >= 5) && (player->GetItemCount(90001) >= 5))
          {
	     if (GetRespawnBonusTime(cr, guid, player))
              {
                 cr->SetRespawnTime(cr->GetRespawnTimeEx()-time(NULL)-300);
                 player->DestroyItemCount(90094, 5, true);
                 player->DestroyItemCount(90001, 5, true);
                 player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffADFF2FВремя до респавна успешно уменьшено на |cff00ccff5 минут|cffADFF2F.");
              }
          }
        else {
              ss << "|TInterface/ICONS/Achievement_general:25|t|cffADFF2FУ вас недостаточно предметов для осуществления этого действия. Вам требуется: |cff00ccff5|r " << sTransmogrification->GetItemLink(90094, session);
              ss << " |TInterface/ICONS/Achievement_general:25|t|cffADFF2Fи " << "|cff00ccff5|r " << sTransmogrification->GetItemLink(90001, session) << "|cffADFF2F.";
              str = ss.str();
              player->GetSession()->SendNotification(str.c_str());
           }
       }
       else  player->GetSession()->SendNotification("|TInterface/ICONS/Achievement_general:25|t|cffADFF2FЧто-то пошло не так. Попробуйте телепортироваться к нужному боссу и повторить.");
           
         player->CLOSE_GOSSIP_MENU();

        return true;

      }

};

 
class Npc_Beastmaster : public CreatureScript
{
public:
        Npc_Beastmaster() : CreatureScript("Npc_Beastmaster") { }
 
        void CreatePet(Player *player, Creature * m_creature, uint32 entry) {
 
                if(player->getClass() != CLASS_HUNTER) {
                m_creature->MonsterWhisper("You are not Hunter!", player);
                        return;
                }
 
                if(player->GetPet()) {
                        m_creature->MonsterWhisper("First drop your current Pet!", player);
                        player->PlayerTalkClass->SendCloseGossip();
                        return;
                }
 
                Creature *creatureTarget = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
                if(!creatureTarget) return;
 
                Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
                if(!pet) return;
 
                creatureTarget->setDeathState(JUST_DIED);
                creatureTarget->RemoveCorpse();
                creatureTarget->SetHealth(0);                       // just for nice GM-mode view
 
                pet->SetPower(POWER_HAPPINESS, 1048000);
 
                pet->SetUInt64Value(UNIT_FIELD_CREATEDBY, player->GetGUID());
                pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
 
                pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel() - 1);
                pet->GetMap()->AddToMap(pet->ToCreature());
 
                pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());
 
                pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);
                if(!pet->InitStatsForLevel(player->getLevel()))
                        TC_LOG_ERROR("entities.pet", "Fail: no init stats for entry %u", entry);
 
                pet->UpdateAllStats();
 
                player->SetMinion(pet, true);
 
                pet->SavePetToDB(PET_SAVE_AS_CURRENT);
                pet->InitTalentForLevel();
                player->PetSpellInitialize();
 
                player->PlayerTalkClass->SendCloseGossip();
                m_creature->MonsterWhisper("Pet successfully added!", player);
        }
 
 
        bool OnGossipHello(Player *player, Creature * m_creature)
        {
 
                if(player->getClass() != CLASS_HUNTER)
                {
                        m_creature->MonsterWhisper("You are not Hunter!", player);
                        return true;
                }
                player->ADD_GOSSIP_ITEM(3, "Get a New Pet", GOSSIP_SENDER_MAIN, 30);
                if (player->CanTameExoticPets())
                {
                        player->ADD_GOSSIP_ITEM(7, "Get a New Exotic Pet.", GOSSIP_SENDER_MAIN, 50);
                }
            player->ADD_GOSSIP_ITEM(2, "Visit the Stable", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
                        player->ADD_GOSSIP_ITEM(1, "Foods for my Pet", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
                        player->ADD_GOSSIP_ITEM(4, "Nevermind", GOSSIP_SENDER_MAIN, 150);
                player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
                return true;
        }
 
        bool OnGossipSelect(Player *player, Creature * m_creature, uint32 sender, uint32 action)
        {
                player->PlayerTalkClass->ClearMenus();
                switch (action)
                {
 
                case 100:
                        player->ADD_GOSSIP_ITEM(3, "Get a New Pet", GOSSIP_SENDER_MAIN, 30);
                        if (player->CanTameExoticPets())
                        {
                                player->ADD_GOSSIP_ITEM(7, "Get a New Exotic Pet.", GOSSIP_SENDER_MAIN, 50);
                        }
                        player->ADD_GOSSIP_ITEM(2, "Visit the Stable", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
                        player->ADD_GOSSIP_ITEM(1, "Foods for my Pet", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
                        player->ADD_GOSSIP_ITEM(4, "Nevermind", GOSSIP_SENDER_MAIN, 150);
                        player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
                        break;
 
                case 150:
                        player->CLOSE_GOSSIP_MENU();
                        break;
 
                case 30:
                        player->ADD_GOSSIP_ITEM(4, "<- Main Menu", GOSSIP_SENDER_MAIN, 100);
                        player->ADD_GOSSIP_ITEM(4, "Next Page ->", GOSSIP_SENDER_MAIN, 31);
                        player->ADD_GOSSIP_ITEM(6, "Bat.", GOSSIP_SENDER_MAIN, 18);
                        player->ADD_GOSSIP_ITEM(6, "Bear.", GOSSIP_SENDER_MAIN, 1);
                        player->ADD_GOSSIP_ITEM(6, "Boar.", GOSSIP_SENDER_MAIN, 2);
                        player->ADD_GOSSIP_ITEM(6, "Cat.", GOSSIP_SENDER_MAIN, 4);
                        player->ADD_GOSSIP_ITEM(6, "Carrion Bird.", GOSSIP_SENDER_MAIN, 5);
                        player->ADD_GOSSIP_ITEM(6, "Crab.", GOSSIP_SENDER_MAIN, 6);
                        player->ADD_GOSSIP_ITEM(6, "Crocolisk.", GOSSIP_SENDER_MAIN, 7);
                        player->ADD_GOSSIP_ITEM(6, "Dragonhawk.", GOSSIP_SENDER_MAIN, 17);
                        player->ADD_GOSSIP_ITEM(6, "Gorilla.", GOSSIP_SENDER_MAIN, 8);
                        player->ADD_GOSSIP_ITEM(6, "Hound.", GOSSIP_SENDER_MAIN, 9);
                        player->ADD_GOSSIP_ITEM(6, "Hyena.", GOSSIP_SENDER_MAIN, 10);
                        player->ADD_GOSSIP_ITEM(6, "Moth.", GOSSIP_SENDER_MAIN, 11);
                        player->ADD_GOSSIP_ITEM(6, "Owl.", GOSSIP_SENDER_MAIN, 12);
                        player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
                        break;
 
                case 31:  
                        player->ADD_GOSSIP_ITEM(4, "<- Main Menu", GOSSIP_SENDER_MAIN, 30);
                        player->ADD_GOSSIP_ITEM(4, "<- Previous Page", GOSSIP_SENDER_MAIN, 30);
                        player->ADD_GOSSIP_ITEM(6, "Raptor.", GOSSIP_SENDER_MAIN, 20);
                        player->ADD_GOSSIP_ITEM(6, "Ravager.", GOSSIP_SENDER_MAIN, 19);
                        player->ADD_GOSSIP_ITEM(6, "Strider.", GOSSIP_SENDER_MAIN, 13);
                        player->ADD_GOSSIP_ITEM(6, "Scorpid.", GOSSIP_SENDER_MAIN, 414);
                        player->ADD_GOSSIP_ITEM(6, "Spider.", GOSSIP_SENDER_MAIN, 16);
                        player->ADD_GOSSIP_ITEM(6, "Serpent.", GOSSIP_SENDER_MAIN, 21);  
                        player->ADD_GOSSIP_ITEM(6, "Turtle.", GOSSIP_SENDER_MAIN, 15);
                        player->ADD_GOSSIP_ITEM(6, "Wasp.", GOSSIP_SENDER_MAIN, 93);
                        player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
                        break;
 
                case 50:  
                        player->ADD_GOSSIP_ITEM(4, "<- Main Menu", GOSSIP_SENDER_MAIN, 100);
                        player->ADD_GOSSIP_ITEM(6, "Chimaera.", GOSSIP_SENDER_MAIN, 51);
                        player->ADD_GOSSIP_ITEM(6, "Core Hound.", GOSSIP_SENDER_MAIN, 52);
                        player->ADD_GOSSIP_ITEM(6, "Devilsaur.", GOSSIP_SENDER_MAIN, 53);
                        player->ADD_GOSSIP_ITEM(6, "Rhino.", GOSSIP_SENDER_MAIN, 54);
                        player->ADD_GOSSIP_ITEM(6, "Silithid.", GOSSIP_SENDER_MAIN, 55);
                        player->ADD_GOSSIP_ITEM(6, "Worm.", GOSSIP_SENDER_MAIN, 56);  
                        player->ADD_GOSSIP_ITEM(6, "Loque'nahak.", GOSSIP_SENDER_MAIN, 57);
                        player->ADD_GOSSIP_ITEM(6, "Skoll.", GOSSIP_SENDER_MAIN, 58);
                        player->ADD_GOSSIP_ITEM(6, "Gondria.", GOSSIP_SENDER_MAIN, 59);
                        player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
                        break;
 
                case GOSSIP_OPTION_STABLEPET:
                        player->GetSession()->SendStablePet(m_creature->GetGUID());
                        break;
                case GOSSIP_OPTION_VENDOR:
                        player->GetSession()->SendListInventory(m_creature->GetGUID());
                        break;
                case 51: //chimera
                        CreatePet(player, m_creature, 21879);
                        break;
                case 52: //core hound
                        CreatePet(player, m_creature, 21108);
                        break;
                case 53: //devilsaur
                        CreatePet(player, m_creature, 20931);
                        break;
                case 54: //rhino
                        CreatePet(player, m_creature, 30445);
                        break;
                case 55: //silithid
                        CreatePet(player, m_creature, 5460);
                        break;
                case 56: //worm
                        CreatePet(player, m_creature, 30148);
                        break;
                case 57: //loque'nahak
                        CreatePet(player, m_creature, 32517);
                        break;
                case 58: //skoll
                        CreatePet(player, m_creature, 35189);
                        break;
                case 59: //gondria
                        CreatePet(player, m_creature, 33776);
                        break;
                case 16: //spider
                        CreatePet(player, m_creature, 2349);
                        break;
                case 17: //dragonhawk
                        CreatePet(player, m_creature, 27946);
                        break;
                case 18: //bat
                        CreatePet(player, m_creature, 28233);
                        break;
                case 19: //ravager
                        CreatePet(player, m_creature, 17199);
                        break;
                case 20: //raptor
                        CreatePet(player, m_creature, 14821);
                        break;
                case 21: //serpent
                        CreatePet(player, m_creature, 28358);
                        break;
                case 1: //bear
                        CreatePet(player, m_creature, 29319);
                        break;
                case 2: //boar
                        CreatePet(player, m_creature, 29996);
                        break;
                case 93: //bug
                        CreatePet(player, m_creature, 28085);
                        break;
                case 4: //cat
                        CreatePet(player, m_creature, 28097);
                        break;
                case 5: //carrion
                        CreatePet(player, m_creature, 26838);
                        break;
                case 6: //crab
                        CreatePet(player, m_creature, 24478);
                        break;  
                case 7: //crocolisk
                        CreatePet(player, m_creature, 1417);
                        break;  
                case 8: //gorila
                        CreatePet(player, m_creature, 28213);
                        break;
                case 9: //hound
                        CreatePet(player, m_creature, 29452);
                        break;
                case 10: //hyena
                        CreatePet(player, m_creature, 13036);
                        break;
                case 11: //moth
                        CreatePet(player, m_creature, 27421);
                        break;
                case 12: //owl
                        CreatePet(player, m_creature, 23136);
                        break;
                case 13: //strider
                        CreatePet(player, m_creature, 22807);
                        break;
                case 414: //scorpid
                        CreatePet(player, m_creature, 9698);
                        break;
                case 15: //turtle
                        CreatePet(player, m_creature, 25482);
                        break;
                }
                return true;
        }
};
 
void AddSC_vendors()
{
       new npc_weapon;
       new npc_event_rew;
       new npc_guild;
       new npc_bonus_respawn_time;
       new Npc_Beastmaster();
}
