/////////////////////////////
//*** Created by Er0niX ***//
/////////////////////////////

#include "TitleEngine.h"
#include "TitleMgr.h"

/////////////////////////////////////////////////////////////////////////////////
//****************************** Variable *************************************//
/////////////////////////////////////////////////////////////////////////////////

TitleGroupStore TitleMgr::groupStore;
TitleStore TitleMgr::titleStore;

/////////////////////////////////////////////////////////////////////////////////
//******************************* Loader **************************************//
/////////////////////////////////////////////////////////////////////////////////

void TitleMgr::LoadTitleStore()
{
	titleStore.clear();

	if(QueryResult result = CharacterDatabase.Query("SELECT * FROM title_store ORDER BY title_number"))
    {
	    do
	    {
		    if(Field* fields = result->Fetch())
            {
                if(!TitleEngine::ExistTitle(fields[1].GetUInt32()))
				    continue;

                Title* title = new Title
                (
                    fields[0].GetUInt32(), 
                    fields[1].GetUInt32(),
                    fields[2].GetCString(),
                    fields[3].GetCString(),
                    fields[4].GetUInt32(), 
                    fields[5].GetUInt32(), 

                    fields[6].GetUInt32(), 
                    fields[7].GetUInt32(), 

                    fields[8].GetUInt32(), 
                    fields[9].GetUInt32(), 
                    fields[10].GetUInt32(), 

                    fields[11].GetUInt32(), 
                    fields[12].GetUInt32(),
                    fields[13].GetUInt32()
                );

                titleStore.push_back(title);
            }
	    } while(result->NextRow());
    }
}

void TitleMgr::LoadTitleGroups()
{
	groupStore.clear();

	if(QueryResult result = CharacterDatabase.Query("SELECT * FROM title_groups ORDER BY group_id"))
    {
	    do
	    {
		    if(Field* fields = result->Fetch())
            {
                TitleGroup* group = new TitleGroup
                (
                    fields[0].GetUInt32(), 
                    fields[1].GetCString(),

                    fields[2].GetUInt32(),
                    fields[3].GetUInt32() 
                );

                groupStore.push_back(group);
            }
	    } while(result->NextRow());
    }
}

/////////////////////////////////////////////////////////////////////////////////
//****************************** TitleStore ***********************************//
/////////////////////////////////////////////////////////////////////////////////

TitleStore TitleMgr::GetTitleStore(Player* player, uint32 tab)
{
    TitleStore titleList = TitleMgr::GetTitleStore(player);
    TitleStore result;

    if(titleList.empty())
        return result;

    int32 tabsCount = ceil((float)titleList.size() / (float)TAB_SIZE) - 1;
    if(tabsCount == -1)
        return result;

    if((int32)tab > tabsCount)
        tab = tabsCount;

    uint32 startIndex = tab * TAB_SIZE;
	uint32 endIndex = (tabsCount == 0 || tab == tabsCount) ? titleList.size() : startIndex + TAB_SIZE;

    if(startIndex >= endIndex)
		return result;

    for(uint32 i = startIndex; i < endIndex; i++)
		if(Title* title = titleList[i]) 
			result.push_back(title);

    return result;
}

TitleStore TitleMgr::GetTitleStore(Player* player)
{
	TitleStore result;

    for(uint32 i = 0; i < 2; i++)
        for(TitleStore::iterator itrBegin = titleStore.begin(), itrEnd = titleStore.end(); itrBegin != itrEnd; itrBegin++)
            if(Title* title = (*itrBegin))
                 if(title->titleGroup == player->titleGroup->groupId && !TitleEngine::IsKnownTitle(player, title->titleId))
                    if(TitleMgr::CanBuy(player, title->titleId) != (bool)i) // I should teach this solution!
                         result.push_back(title);

	return result;
}


Title* TitleMgr::GetTitle(uint32 titleId)
{
	for(TitleStore::iterator itrBegin = titleStore.begin(), itrEnd = titleStore.end(); itrBegin != itrEnd; itrBegin++)
		if(Title* title = (*itrBegin))
			if(title->titleId == titleId)
				return title;

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////
//********************************** Buy **************************************//
/////////////////////////////////////////////////////////////////////////////////

bool TitleMgr::CanBuy(Player* player, uint32 titleId) 
{
      if(Title* title = TitleMgr::GetTitle(titleId))
	    return 
		    !(
			    (player->GetMoney() < TitleEngine::ToCopper(title->moneyAmount) && title->moneyAmount > 0)
			    || (player->GetItemCount(SERVER_CREDIT_ID) < title->creditAmount && title->creditAmount > 0)
			    || (player->GetHonorPoints() < title->honorAmount && title->honorAmount > 0)
			    || (player->GetArenaPoints() < title->arenaAmount && title->arenaAmount > 0)
                || (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) < title->killAmount && title->killAmount > 0)
			    || (player->GetArenaPersonalRating(ARENA_2V2) < title->rating2V2 && title->rating2V2 > 0)
			    /*|| (player->GetArenaPersonalRating(ARENA_3V3) < title->rating3V3 && title->rating3V3 > 0)
			    || (player->GetArenaPersonalRating(ARENA_5V5) < title->rating5V5 && title->rating5V5 > 0)*/
		    );

   return false;
}

void TitleMgr::Buy(Player* player, uint32 titleId)
{
    if(Title* title = TitleMgr::GetTitle(titleId))
    {
	    if(title->moneyAmount > 0)
		    player->ModifyMoney(-(int32)TitleEngine::ToCopper(title->moneyAmount)); 
	    if(title->creditAmount > 0)
		    player->DestroyItemCount(SERVER_CREDIT_ID, title->creditAmount, true);
	    if(title->honorAmount > 0)
		    player->ModifyHonorPoints(-(int32)title->honorAmount); 
	    if(title->arenaAmount > 0)
		    player->ModifyArenaPoints(-(int32)title->arenaAmount);
    }
}

/////////////////////////////////////////////////////////////////////////////////
//******************************** Groups *************************************//
/////////////////////////////////////////////////////////////////////////////////

TitleGroup* TitleMgr::GetTitleGroup(uint32 groupId)
{
    for(TitleGroupStore::iterator itrBegin = groupStore.begin(), itrEnd = groupStore.end(); itrBegin != itrEnd; itrBegin++)
        if(TitleGroup* group = (*itrBegin))
            if(group->groupId == groupId)
                return group;

    return NULL;
}

uint32 TitleMgr::GetTitleCount(Player* player, uint32 groupId)
{
    uint32 counter = 0;

    for(TitleStore::iterator itrBegin = titleStore.begin(), itrEnd = titleStore.end(); itrBegin != itrEnd; itrBegin++)
		if(Title* title = (*itrBegin))
            if(title->titleGroup == groupId)
                if(!TitleEngine::IsKnownTitle(player, title->titleId))
                    counter++;

    return counter;
}