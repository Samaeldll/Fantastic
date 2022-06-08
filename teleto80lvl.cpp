#include "ScriptPCH.h"
#include "ScriptedGossip.h"

class teleto80lvl : public CreatureScript
{
public:
    teleto80lvl() : CreatureScript("teleto80lvl") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
              if (player->IsInCombat())
           {
               player->PlayerTalkClass->SendCloseGossip();
	        creature->MonsterWhisper("|TInterface/ICONS/Achievement_general:25|tВы в бою!", player);
               return true;
           }

           player->PlayerTalkClass->ClearMenus();
	    if (player->getLevel() == 80)
	    {
		    player->ADD_GOSSIP_ITEM( 10, "|TInterface/ICONS/Achievement_general:25|tСтарт локация"     , GOSSIP_SENDER_MAIN, 1000);
	    }	   
           player->ADD_GOSSIP_ITEM( 10, "|TInterface/ICONS/Achievement_general:25|tВыйти"     , GOSSIP_SENDER_MAIN, 1001);
           player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());
	    return true;
    }
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{
		switch(action)
		{
		case 1000:
		    player->PlayerTalkClass->SendCloseGossip();
		    player->TeleportTo(530, -1948.368408f, 5387.595703f, -11.216649f, 0.334741f);

		    break;

		case 1001:
			creature->MonsterWhisper("|TInterface/ICONS/Achievement_general:25|tБуду рад видеть вас снова!", player);
			player->PlayerTalkClass->SendCloseGossip();
			break;
			}
          }

};

void AddSC_teleto80lvl()
{
	new teleto80lvl();
}
