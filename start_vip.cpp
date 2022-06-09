/*
<--------------------------------------------------------------------------->
 - Developer: Krys
 - Complete: 100%
 - ScriptName: 'VIP Area'
 - Comment: Tested & Working.
<--------------------------------------------------------------------------->
*/
#include "ScriptPCH.h"

class playertp : public PlayerScript
{
	public:
    playertp() : PlayerScript("playertp") { }
	uint64 playerGUID;
	void OnCreate(Player* player) 
	{
		playerGUID = player->GetSession()->IsPremium();
	}
	void OnLogin(Player* player)
	{
		if (player->GetSession()->IsPremium()) 
		if(player->GetTotalPlayedTime() < 5)
		{
		player->AddItem(37829, 2000);
		player->AddItem(33998, 1);
		player->AddItem(34094, 1);
		player->AddItem(33997, 1);
		}
	}
	
};

void AddSC_playertp()
{
    new playertp();
} 