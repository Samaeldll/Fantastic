#include "ScriptPCH.h"
#include "ScriptedGossip.h"

class npc_trainer: public CreatureScript
{
public:
    npc_trainer() : CreatureScript("npc_trainer") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
	if (player->IsInCombat())
	{
		player->PlayerTalkClass->SendCloseGossip();
		creature->MonsterWhisper("|TInterface/ICONS/Achievement_general:25|tВы в бою!", player);
                player->CLOSE_GOSSIP_MENU();
		return 0;
	}

           player->PlayerTalkClass->ClearMenus();
	   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface\\ICONS\\inv_misc_book_09:25:25:-18:0|tОткрыть тренера", GOSSIP_SENDER_MAIN, 101);
	   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\ICONS\\inv_offhand_sunwell_d_01:25:25:-18:0|tСбросить таланты", GOSSIP_SENDER_MAIN, 102);
           player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  "|TInterface\\ICONS\\achievement_general:25:25:-18:0|tВыучить двойную специализацию", GOSSIP_SENDER_MAIN, 103);
           player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
           return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
     {
	    player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
		case 101:
                      {
                        switch(player->getClass())
                        {
			     case CLASS_DEATH_KNIGHT:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45079); break;

			     case CLASS_DRUID:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45077); break;

			     case CLASS_HUNTER:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45075); break;

			     case CLASS_MAGE:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45071); break;

			     case CLASS_PALADIN:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45070); break;

			     case CLASS_PRIEST:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45073); break;

			     case CLASS_ROGUE:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45076); break;

			     case CLASS_SHAMAN:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45072); break;

			     case CLASS_WARLOCK:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45078); break;

			    case CLASS_WARRIOR:
				player->GetSession()->SendTrainerList(creature->GetGUID(), 45074); break;
                        }
                      }
                      break;

		case 102:
			{
				player->resetTalents(true);
                            //player->InitTalentForLevel();
                            //player->SendTalentsInfoData(false); 
				creature->MonsterWhisper("|TInterface/ICONS/Achievement_general:25|t|cff1eff00|rЯ сбросил ваши таланты! Пожалуйста, перезайдите в игру.", player);
				player->CLOSE_GOSSIP_MENU();
			}
			break;

               case 103:
                      {
                          if (player->GetSpecsCount() == 1 && player->getLevel() >= sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL))
                            {
                               // Cast spells that teach dual spec
                               // Both are also ImplicitTarget self and must be cast by player
                                 player->CastSpell(player, 63680, true, NULL, NULL, player->GetGUID());
                                 player->CastSpell(player, 63624, true, NULL, NULL, player->GetGUID());

                              // Should show another Gossip text with "Congratulations..."
                             player->PlayerTalkClass->SendCloseGossip();
                            } 
                        player->CLOSE_GOSSIP_MENU();
                      } break;
        }
        return true;

      }
};

void AddSC_npc_trainer()
{
	new npc_trainer;
}
