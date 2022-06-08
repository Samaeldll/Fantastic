#include "ScriptPCH.h"
 
class remove_aura : public PlayerScript
{
    public:
        remove_aura() : PlayerScript("remove_aura") {}

     void OnMapChanged(Player* player)
    {
	if (player->GetMap()->IsBattlegroundOrArena())
	{
	   if (player->HasAura(20165))
	       player->RemoveAura(20165);
     }
}
};

void AddSC_remove_aura()
{
    new remove_aura();
}
