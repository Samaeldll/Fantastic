#include "ScriptPCH.h"
#include "ScriptedGossip.h"

using namespace std;

string time_conv_to_str;
uint64 guid_bosses[7] = {9465074, 9480422, 9475945, 9477284, 9477471, 10208869, 10242764};

class telenpc : public CreatureScript 
{ 
    public: 
    telenpc() : CreatureScript("telenpc") { } 

  string GetRespTimeBoss(uint32 guid)
   {

          if (Creature* cr = HashMapHolder<Creature>::FindCreature(guid))
             {
                 if (cr->IsAlive())
                     return "Жив";
                 else return time_conv_to_str = secsToTimeString(cr->GetRespawnTimeEx()-time(NULL));
             }

          PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CREATURE_RESPAWNS_TIME);
          stmt->setUInt32(0, guid); 

          if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
            {
              Field* fields = result->Fetch();
              uint32 respawnTime = fields[0].GetUInt32();
              uint32 tempTime = respawnTime - time(NULL);

              if (tempTime > 86400 || tempTime < 1)
                  time_conv_to_str = "Жив";
              else
                  time_conv_to_str = secsToTimeString(respawnTime-time(NULL));  
              return time_conv_to_str;
            }
          else return "Жив";
    }

    bool UpdateRespTime(Player* player)
    {
       player->ADD_GOSSIP_ITEM( GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_a_01:25|tУфориус Форст: " + GetRespTimeBoss(guid_bosses[0]), GOSSIP_SENDER_MAIN, 500);
       player->ADD_GOSSIP_ITEM( GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_a_02:25|tАдский крушитель: " + GetRespTimeBoss(guid_bosses[1]), GOSSIP_SENDER_MAIN, 501); 
       player->ADD_GOSSIP_ITEM( GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_a_03:25|tАрхангел Михаил: " + GetRespTimeBoss(guid_bosses[2]), GOSSIP_SENDER_MAIN, 502); 
       player->ADD_GOSSIP_ITEM( GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_a_04:25|tАзог Завоеватель: " + GetRespTimeBoss(guid_bosses[3]), GOSSIP_SENDER_MAIN, 503);
       player->ADD_GOSSIP_ITEM( GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_a_05:25|tАрхител: " + GetRespTimeBoss(guid_bosses[4]), GOSSIP_SENDER_MAIN, 504);
       player->ADD_GOSSIP_ITEM( GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_a_05:25|tГул'Дан: " + GetRespTimeBoss(guid_bosses[5]), GOSSIP_SENDER_MAIN, 506);
       player->ADD_GOSSIP_ITEM( GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_a_06:25|tРагнарос: " + GetRespTimeBoss(guid_bosses[6]), GOSSIP_SENDER_MAIN, 507);
       player->ADD_GOSSIP_ITEM( 7, "Обновить", GOSSIP_SENDER_MAIN, 505);
    }
    bool OnGossipHello(Player* player, Creature* creature)
    {

       if (player->GetMap()->IsBattlegroundOrArena())
       {
              creature->MonsterWhisper("Вы не можете воспользоваться телепортером пока находитесь на поле боя или арене!", player);
              player->CLOSE_GOSSIP_MENU();
              return false;
	}

              if (player->IsInCombat())
           {
	        creature->MonsterWhisper("Вы в бою!", player);
                player->CLOSE_GOSSIP_MENU();
               return false;
           }

       if (player->getLevel() < 80)
       {
              creature->MonsterWhisper("Вы не можете воспользоваться телепортером пока не достигнете 80 уровня!", player);
              player->CLOSE_GOSSIP_MENU();
              return false;
	}
  
            player->PlayerTalkClass->ClearMenus();

            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_General:30:30:-18:0|tСтарт Локация", GOSSIP_SENDER_MAIN, 5); 
            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_Reputation_KirinTor:30:30:-18:0|tПодземелья", GOSSIP_SENDER_MAIN, 1); 
            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_Boss_LichKing:30:30:-18:0|tМировые боссы", GOSSIP_SENDER_MAIN, 3); 
            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/trade_engineering:30:30:-18:0|tВремя до респавна боссов", GOSSIP_SENDER_MAIN, 7);
			player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/inv_sword_2h_artifactashbringer_d_06:30:30:-18:0|tДобыча Оружия", GOSSIP_SENDER_MAIN, 502);
            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/inv_ingot_03:30:30:-18:0|tФарм Локации", GOSSIP_SENDER_MAIN, 8); 
            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Spell_Shadow_Teleport:30:30:-18:0|tЕвенты", GOSSIP_SENDER_MAIN, 6);
            player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
	    return true;
    }
       void SendDefaultMenu(Player* player, Creature* creature, uint32 action)
	{
		switch(action)
		{	
		case 1:
		    player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/expansionicon_classic:30:30:-18:0|tСтандартные Подземелья", GOSSIP_SENDER_MAIN, 9); 
            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/expansionicon_burningcrusade:30:30:-18:0|tПодземелья V.I.P", GOSSIP_SENDER_MAIN, 10); 
            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/expansionicon_wrathofthelichking:30:30:-18:0|tПодземелья Донат", GOSSIP_SENDER_MAIN, 11); 
			break;
		
		
		case 9:
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_boss_chiefukorzsandscalp:30|tЗул'Фаррак", GOSSIP_SENDER_MAIN, 100, "Вы уверены?", 0, false);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_dungeon_ulduar80_normal:30|tЧертоги Молний", GOSSIP_SENDER_MAIN, 101);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_reputation_wyrmresttemple:30|tРубиновое святилище", GOSSIP_SENDER_MAIN, 102);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_boss_ladyvashj:30|tЗмеиное святилище", GOSSIP_SENDER_MAIN, 103);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_boss_zuljin:30|tЗул'Аман", GOSSIP_SENDER_MAIN, 104);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/misc_drogbartotem02:30|tКузня Душ", GOSSIP_SENDER_MAIN, 105);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_dungeon_the stonecore_ozruk:30|tКрепость Бурь", GOSSIP_SENDER_MAIN, 106);
		    player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
		    break; 
			
		case 10:
			player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/inv_misc_head_dragon_nexus:30|tНексус", GOSSIP_SENDER_MAIN, 108);
			player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/misc_arrowleft:30|tВернутся", GOSSIP_SENDER_MAIN, 1);
			player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
			break; 
			
		case 11:
			player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_boss_gruul:30|tЛогово Груула [Донат]", GOSSIP_SENDER_MAIN, 107);
			player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/misc_arrowleft:30|tВернутся", GOSSIP_SENDER_MAIN, 1);
			player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
			break; 

		case 3:
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_boss_archaedas:25|tУфориус Фрост", GOSSIP_SENDER_MAIN, 200);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_boss_highmaul_fungalgiant:25|tАдский крушитель", GOSSIP_SENDER_MAIN, 201);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_guild_forgreatjusticerank2:25|tАрхангел Михаил", GOSSIP_SENDER_MAIN, 202);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_boss_blackhand:25|tАзог Завоеватель", GOSSIP_SENDER_MAIN, 203);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_boss_mogufemales:25|tАрхител", GOSSIP_SENDER_MAIN, 204);
			player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_thenighthold_guldan:25|tГул'Дан", GOSSIP_SENDER_MAIN, 205);
			player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/achievement_firelands raid_ragnaros:25|tРагнарос", GOSSIP_SENDER_MAIN, 206);
		    player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
		    break; 

		case 4:
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tОзеро Лок", GOSSIP_SENDER_MAIN, 301);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tОзеро Эре'Нору", GOSSIP_SENDER_MAIN, 302);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tОзеро Дыхания Зимы", GOSSIP_SENDER_MAIN, 300);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tРыбацкое судно", GOSSIP_SENDER_MAIN, 303);
		    player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
		    break; 
              
		case 5:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, -1948.368408f, 5387.595703f, -11.216649f, 0.334741f);
		    break;

		case 6:
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tОзеро Лок", GOSSIP_SENDER_MAIN, 400);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tОстров Раназьяр", GOSSIP_SENDER_MAIN, 401);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tОстров Алькац", GOSSIP_SENDER_MAIN, 402);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tОстров ГМ", GOSSIP_SENDER_MAIN, 403);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/Achievement_bg_trueavshutout:25|tТемные берега", GOSSIP_SENDER_MAIN, 404);
		    player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
		    break;
			
		case 8:
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/inv_ore_gold_nugget:25|tЗолота Лихордка", GOSSIP_SENDER_MAIN, 500);
		    player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/monk_stance_whitetiger:25|tЭфириальный Сброс", GOSSIP_SENDER_MAIN, 501);
			player->ADD_GOSSIP_ITEM( 2, "|TInterface/icons/inv_jewelcrafting_70_gem03_blue:25|tКристальная Эфория", GOSSIP_SENDER_MAIN, 502);
			player->ADD_GOSSIP_ITEM( 2, "|TInterface/ICONS/inv_fishingchair:30:30:-18:0|tРыбалка", GOSSIP_SENDER_MAIN, 4); 
		    player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
		    break; 

		case 7:
            UpdateRespTime(player);
		    player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
		    break; 

            case 500:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, -245.759094f, 1188.600830f, 45.619705f, 1.613962f);
		    break;
			
			case 501:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, 3056.436035f, 2201.553711f, 166.860092f, 3.536023f);
		    break;
			
			
			case 502: //Кристалы и Добыча Оружия
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 2861.291016f, 63.938019f, 2.993399f, 3.269071f);
		    break;


              case 505:
                  UpdateRespTime(player);
		    player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
		    break; 

		case 2:
			creature->MonsterWhisper("Буду рад видеть вас снова!", player);
			player->PlayerTalkClass->SendCloseGossip();
			break;

		case 100:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(1, -6822.112793f, -2903.031982f, 9.807702f, 0.855343f);
		    break;

		case 101:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 9091.031250f, -1301.576538f, 1065.399536f, 5.510983f);
		    break;

		case 102:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 3582.708496f, 221.142960f, -114.879456f, 5.368015f);
		    break;

		case 103:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, 784.595764f, 6866.314453f, -63.601387f, 0.002801f);
		    break;

		case 104:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, 6849.972168f, -7950.801758f, 174.789398f, 4.747736f);
		    break;

		case 105:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 5667.403320f, 2008.079834, 799.799011, 5.410361f);
		    break;
			
		case 106:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, 3088.266846f, 1385.229f, 186.697937f, 4.621442f);
		    break;
			
		case 107:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, 3545.965088f, 5088.820312f, 4.964437f, 5.735139f);
		    break;

		case 108: //Nexus
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 3890.144043f, 6985.237793f, 72.762573f, 0.012698f);
		    break;

		case 200:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 7620.651855f, 2634.669922f, 533.403259f, 3.418485f);
		    break;

		case 201:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, -207.093445f, 2105.470215f, 89.873016f, 1.709347f);
		    break;

		case 202:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 7309.056641f, -642.509766f, 788.433655f, 4.734606f);
		    break;

		case 203:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(1, -413.053589f, -2604.054932f, 98.029846f, 3.204008f);
		    break;

		case 204:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(0, -7626.231445f, -710.180603f, 185.350220f, 4.972937f);
		    break;
			
		case 205:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, -248.859543f, 963.352722f, 85.626183f, 4.733812f);
		    break;
			
		case 206: //Рагнарос
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 1268.262323f, -5060.528809, 180.072861f, 5.215916f);
		    break;			

		case 300:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 2324.099121f, -3949.493896f, 220.854584f, 4.617761f);
		    break;

		case 301:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(0, -5192.938477f, -3116.288330f, 303.216736f, 0.737080f);
		    break;

		case 302:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, -3202.868164f, 3908.599609f, 58.304150f, 5.282613f);
		    break;

		case 303:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(571, 1687.982178f, -2557.758301f, 21.998705f, 0.771117f);
		    break;

                case 400:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(0, -5197.634766f, -3124.211426f, 302.330811f, 4.431048f);
		    break;

                case 401:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(1, 145.117615f, 2903.355225f, 23.508709f, 3.794879f);
		    break;

                case 402:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(1, -2704.090332f, -4670.243652f, 11.284291f, 1.584686f);
		    break;

                case 403:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(1, 16219.917969f, 16258.283203f, 16.211115f, 3.240155f);
		    break;

                case 404:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(1, 7361.688965f, -1554.278442f, 165.442261f, 0.559377f);
		    break;

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
void AddSC_telenpc()
{
	new telenpc();
}
