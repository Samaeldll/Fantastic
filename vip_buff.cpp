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
#include "Pet.h"

uint32 vip_auras[] = { 20217, 48101, 58449, 54494, 24752, 21562, 48100 };
uint32 auras[] = { 24705, 16609, 22888, 24425, 15366, 43223, 48100 };

class buff_commandscript : public CommandScript
{
public:
    buff_commandscript() : CommandScript("buff_commandscript") { }

	ChatCommand* GetCommands() const
    {
        static ChatCommand IngameCommandTable[] =
        {
			{ "buff",            SEC_PLAYER,  false, &HandleBuffCommand,             "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };
		 return IngameCommandTable;
    }

	static bool HandleBuffCommand(ChatHandler * handler, const char * args)
    {
        Player * me = handler->GetSession()->GetPlayer();
	
		me->RemoveAurasByType(SPELL_AURA_MOUNTED);
		for(int i = 0; i < sizeof(vip_auras) / sizeof(vip_auras[0]); i++)
		    me->AddAura(auras[i], me);
		handler->PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tВы начищены сейчас!   --Fantastic--");
		return true;
    }
};

class npc_buffer : public CreatureScript
{
public:
	npc_buffer() : CreatureScript("npc_buffer") { }

	bool OnGossipHello(Player* player, Creature* creature) override
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_general:25|tБаффны меня!", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_general:25|tВылечи меня!", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_general:25|tУдалить Воскресенскую болезнь!", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_general:25|tНет, спасибо", GOSSIP_SENDER_MAIN, 4);
		
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
	{
		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{
		case 1:
			for (int i = 0; i < sizeof(auras) / sizeof(auras[0]); i++)
				creature->AddAura(auras[i], player);
			player->PlayerTalkClass->SendCloseGossip();
			creature->Say("You have been buffed!", LANG_UNIVERSAL);
			break;
		case 2:
			if (player->IsInCombat() || player->duel)
			{
				ChatHandler(player->GetSession()).PSendSysMessage("|TInterface/ICONS/Achievement_general:25|tВы не можете сделать это в бою!");
				player->PlayerTalkClass->SendCloseGossip();
				return true;
			}
			player->PlayerTalkClass->SendCloseGossip();
			player->SetHealth(uint32(player->GetMaxHealth()));
			player->SetPower(POWER_MANA, uint32(player->GetMaxPower(POWER_MANA)));
			player->SetPower(POWER_ENERGY, uint32(player->GetMaxPower(POWER_ENERGY)));
			if (Pet* Pet = player->GetPet())
			{
				if (Pet && Pet->IsAlive())
					Pet->SetHealth(Pet->GetMaxHealth());
			}
			break;
		case 3:
			player->RemoveAura(15007);
			player->PlayerTalkClass->SendCloseGossip();
			break;
		case 4:
			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		return true;
	}

};

class vip_activator : public CreatureScript
{
public:
	vip_activator() : CreatureScript("vip_activator") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/Achievement_general:25|tАктивировать VIP", GOSSIP_SENDER_MAIN, 0);
		player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/Achievement_general:25|tЯ потерял свой VIP-ключ!", GOSSIP_SENDER_MAIN, 1);
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

void AddSC_buff_commandscript()
{
    new buff_commandscript();
	new npc_buffer();
	new vip_activator();
}