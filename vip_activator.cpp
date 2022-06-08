#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
class vip_activator : public CreatureScript
{
public:
    vip_activator() : CreatureScript("vip_activator") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/Achievement_general:25|tАктивировать VIP", GOSSIP_SENDER_MAIN, 0);
		player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/Achievement_general:25|tЯ потерял свой VIP-Ключ!", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/Achievement_general:25|tУ меня нет VIP заклинания!", GOSSIP_SENDER_MAIN, 2);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
    {
        if (sender == GOSSIP_SENDER_MAIN)
        {
            player->PlayerTalkClass->ClearMenus();
            rbac::RBACData* rbac = player->GetSession()->GetRBACData();
            switch (uiAction)
            {
                case 0:
                    if (player->HasItemCount(9000, 1, false))
                    {
                        player->CLOSE_GOSSIP_MENU();
                        player->DestroyItemCount(9000, 1, true, false);
						player->AddItem(9000, 1);
						player->LearnSpell(71188, false);
						player->LearnSpell(71193, false);
						player->LearnSpell(11305, false);
						sAccountMgr->UpdateAccountAccess(rbac, player->GetSession()->GetAccountId(), uint8(SEC_VIP), -1);
                        creature->Whisper("|TInterface/ICONS/Achievement_general:25|tПоздравляем, теперь у вас есть VIP, пожалуйста, перезайдите!", LANG_UNIVERSAL, player);
                    }
                    else
                    {
                        player->CLOSE_GOSSIP_MENU();
                        creature->Whisper("|TInterface/ICONS/Achievement_general:25|tУ вас нет VIP-токена.", LANG_UNIVERSAL, player);
                        return false;
                    }
                    break;
				case 1:
                    if (player->GetSession()->GetSecurity() >= 1)
                    {
                        player->CLOSE_GOSSIP_MENU();
						player->AddItem(9000, 1);
                        creature->Whisper("|TInterface/ICONS/Achievement_general:25|tТеперь у вас есть VIP-ключ!", LANG_UNIVERSAL, player);
                    }
                    else
                    {
                        player->CLOSE_GOSSIP_MENU();
                        creature->Whisper("|TInterface/ICONS/Achievement_general:25|tВы не VIP.", LANG_UNIVERSAL, player);
                        return false;
                    }
                    break;
					case 2:
                    if (player->GetSession()->GetSecurity() >= 1)
                    {
                        player->CLOSE_GOSSIP_MENU();
						player->LearnSpell(71188, false);
						player->LearnSpell(71193, false);
						player->LearnSpell(11305, false);
                        creature->Whisper("|TInterface/ICONS/Achievement_general:25|tТеперь у вас есть VIP Заклинание!", LANG_UNIVERSAL, player);
                    }
                    else
                    {
                        player->CLOSE_GOSSIP_MENU();
                        creature->Whisper("|TInterface/ICONS/Achievement_general:25|tВы не VIP.", LANG_UNIVERSAL, player);
                        return false;
                    }
                    break;
            }
        }
        return true;
    }
};
void AddSC_vip_activator()
{
    new vip_activator();
}