#include "ScriptPCH.h"
 
class language : public PlayerScript
{
    public:
        language() : PlayerScript("language") {}
       
        void OnLogin(Player* player)
        {
            if (player->GetTeamId() == TEAM_HORDE)
            {
              player->removeSpell(669); 
              player->learnSpell(669, false);
            }

            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
              player->removeSpell(668); 
              player->learnSpell(668, false);
            }
       }

    void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 /*newArea*/)
         {
               if (player->GetTeamId() == TEAM_HORDE)
                 {
                   player->removeSpell(669); 
                   player->learnSpell(669, false);
                 }

              if (player->GetTeamId() == TEAM_ALLIANCE)
                {
                  player->removeSpell(668); 
                  player->learnSpell(668, false);
                }
         }

     void OnMapChanged(Player* player)
         {
               if (player->GetTeamId() == TEAM_HORDE)
                 {
                   player->removeSpell(669); 
                   player->learnSpell(669, false);
                 }

              if (player->GetTeamId() == TEAM_ALLIANCE)
                {
                  player->removeSpell(668); 
                  player->learnSpell(668, false);
                }
         }




};

void AddSC_language()
{
    new language();
}
