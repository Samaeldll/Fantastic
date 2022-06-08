#include "ScriptPCH.h"
#include "ScriptedGossip.h"

using namespace std;

string time_conv_to_str;
int64 guid_bosses[3] = {9465074, 9469389, 9475945};
uint32 time_resp_guid = 0;

class npc_time : public CreatureScript
{
public:
    npc_time() : CreatureScript("npc_time") { }

  string GetRespTimeBoss(uint32 guid)
   {
       PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CREATURE_RESPAWNS_TIME);
         stmt->setUInt32(0, guid); 
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
       {
            Field* fields = result->Fetch();
            uint32 respawnTime = fields[0].GetUInt32();
            
                time_resp_guid = respawnTime-time(NULL);
                 if (time_resp_guid < 5 || time_resp_guid > 86400)
                   time_conv_to_str = "Alive";
                 else
                time_conv_to_str = secsToTimeString(uint64(time_resp_guid), true);  

           return time_conv_to_str;
        } 
      return "Alive";
    }


    bool OnGossipHello(Player* player, Creature* creature)
      {
              if (player->IsInCombat())
           {
               player->PlayerTalkClass->SendCloseGossip();
	        creature->MonsterWhisper("|TInterface/ICONS/Achievement_general:25|tВы в бою!", player);
               return true; 
           }
          
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM( 10, "|TInterface/ICONS/Achievement_pvp_a_01:25|tУфориус Форст: " + GetRespTimeBoss(guid_bosses[0]), GOSSIP_SENDER_MAIN, 1);
            player->ADD_GOSSIP_ITEM( 10, "|TInterface/ICONS/Achievement_pvp_a_02:25|tАдский крушитель: " + GetRespTimeBoss(guid_bosses[1]), GOSSIP_SENDER_MAIN, 2); 
            player->ADD_GOSSIP_ITEM( 10, "|TInterface/ICONS/Achievement_pvp_a_03:25|tАрхангел Михаил: " + GetRespTimeBoss(guid_bosses[2]), GOSSIP_SENDER_MAIN, 3); 
            player->PlayerTalkClass->SendGossipMenu(85000,creature->GetGUID());

           return true;

      }

};

void AddSC_npc_time()
{
	new npc_time();
}
