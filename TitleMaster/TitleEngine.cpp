/////////////////////////////
//*** Created by Er0niX ***//
/////////////////////////////

#include "TitleEngine.h"
#include "TitleMgr.h"

/////////////////////////////////////////////////////////////////////////////////
//********************************** Title ************************************//
/////////////////////////////////////////////////////////////////////////////////

void TitleEngine::LearnTitle(Player* player, uint32 titleId)
{
    if(Title* title = TitleMgr::GetTitle(titleId))
    {
	    CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(title->titleId);
        player->SetTitle(titleInfo);
    }
}

bool TitleEngine::IsKnownTitle(Player* player, uint32 titleId)
{
	CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(titleId);
	if(!titleInfo)
		return true;

    if(player->HasTitle(titleInfo))
		return true;

	return false;
}   

/////////////////////////////////////////////////////////////////////////////////
//********************************* String ************************************//
/////////////////////////////////////////////////////////////////////////////////

std::string TitleEngine::ConcatTitleName(Player* player, Title* title)
{
    bool canBuy = TitleMgr::CanBuy(player, title->titleId);
	std::string playerName (player->GetName());
	std::stringstream ss;

    if(title->titleType == NAME_BEFORE && player->getGender() == GENDER_FEMALE)
		ss << (canBuy ? "" : "|cff454545") << playerName << "|cff454545" << title->titleNameFemale << "|r";

    else if(title->titleType == NAME_BEFORE && player->getGender() == GENDER_MALE)
		ss << (canBuy ? "" : "|cff454545") << playerName << "|cff454545" << title->titleName << "|r";

    else if(title->titleType == NAME_AFTER && player->getGender() == GENDER_FEMALE)
        ss << "|cff454545" << title->titleNameFemale << "|r " << (canBuy ? "" : "|cff454545") << playerName;

    else
        ss << "|cff454545" << title->titleName << "|r " << (canBuy ? "" : "|cff454545") << playerName;

    return ss.str();
}

std::string TitleEngine::ObjectiveString(std::string valueName, uint32 value1, uint32 value2)
{
	std::stringstream ss;

	ss << TitleEngine::ToColor("+ ", (value1 >= value2) ? C_GREEN : C_RED) << valueName << ": "
	   << TitleEngine::ToString((value1 > value2) ? value2 : value1) << "/" 
	   << TitleEngine::ToString(value2) << "\n\n";

	return ss.str();
}

std::string TitleEngine::ObjectiveMessage(Player* player, uint32 titleId)
{
	std::stringstream message;
	message << "";

    Title* title = TitleMgr::GetTitle(titleId);
    if(!title)
        return message.str();

	if(title->moneyAmount > 0)
		message << ObjectiveString("Gold", TitleEngine::ToGold(player->GetMoney()), title->moneyAmount);
	if(title->creditAmount > 0)
		message << ObjectiveString("Евент монета", player->GetItemCount(SERVER_CREDIT_ID), title->creditAmount);
	if(title->honorAmount > 0)
		message << ObjectiveString("Хонор", player->GetHonorPoints(), title->honorAmount);
	if(title->rating2V2 > 0)
		message << ObjectiveString("Рейтинг2х2", player->GetArenaPersonalRating(ARENA_2V2), title->rating2V2);
	/*if(title->rating3V3 > 0)
		message << ObjectiveString("Рейтинг3х3", player->GetArenaPersonalRating(ARENA_3V3), title->rating3V3);
	if(title->rating5V5 > 0)
		message << ObjectiveString("Рейтинг5х5", player->GetArenaPersonalRating(ARENA_5V5), title->rating5V5);*/
	if(title->arenaAmount > 0)
		message << ObjectiveString("Арена поинты", player->GetArenaPoints(), title->arenaAmount);
    if(title->killAmount > 0)
        message << ObjectiveString("PVP убийств", player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS), title->killAmount);

	return (message.str() == "") ? MessageTypes[MESSAGE_FREE_TITLE] : message.str();
}

/////////////////////////////////////////////////////////////////////////////////
//********************************* Page **************************************//
/////////////////////////////////////////////////////////////////////////////////

void TitleEngine::CreateGossip(Player* player, Creature* creature)
{
    player->PlayerTalkClass->ClearMenus();

    bool isPlayer = (player->GetSession()->GetSecurity() == SEC_PLAYER);
    uint32 faction = GetFaction(player);
    uint32 counter = 0;

    if(!TitleMgr::GetGroupsCount())
    {
        player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    TitleGroupStore groupStore = TitleMgr::GetGroups();
    for(TitleGroupStore::iterator itrBegin = groupStore.begin(), itrEnd = groupStore.end(); itrBegin != itrEnd; itrBegin++)
        if(TitleGroup* group = (*itrBegin))
            if(TitleMgr::GetTitleCount(player, group->groupId)) 
                if(SameFaction(faction, group->groupFaction) || !isPlayer)
				{   
					player->ADD_GOSSIP_ITEM(group->groupIcon, group->groupName, SENDER_MAIN, group->groupId);
					counter++;
				}

    if(!counter)
    {
        player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    player->ADD_GOSSIP_ITEM(ICON_OPTION, "Выход", SENDER_MAIN, BUTTON_EXIT);
    player->PlayerTalkClass->SendGossipMenu(TEXT_MENU, creature->GetGUID());
}

bool TitleEngine::CreatePage(Player* player, Creature* creature, uint32 tab)
{
    player->PlayerTalkClass->ClearMenus();
    int32 tabsCount = TitleMgr::GetTabsCount(player);

    if(tabsCount == -1)
    {
        player->PlayerTalkClass->SendCloseGossip();
        return false;
    }

    if((int32)tab > tabsCount)
        tab = tabsCount;

    TitleStore titleStore = TitleMgr::GetTitleStore(player, tab);
    if(titleStore.empty())
    {
        player->PlayerTalkClass->SendCloseGossip();
        return false;
    }

    if(tabsCount > 0 && (uint32)tabsCount > tab)
        player->ADD_GOSSIP_ITEM(ICON_OPTION, "|cff00ff00|TInterface\\icons\\spell_chargepositive:24|t|r Далее >>", tab, BUTTON_NEXT);
    if(tab != 0)
        player->ADD_GOSSIP_ITEM(ICON_OPTION, "|cff00ff00|TInterface\\icons\\spell_chargenegative:24|t|r << Назад", tab, BUTTON_BACK);

    for(TitleStore::iterator itrBegin = titleStore.begin(), itrEnd = titleStore.end(); itrBegin != itrEnd; itrBegin++)
        if(Title* title = (*itrBegin))
			if(title->titleId >28)
			{
				const std::string prefix = "|cff00ff00|TInterface\\icons\\INV_Misc_Note_01:24|t|r ";
				std::string title_name;
				title_name = prefix + TitleEngine::ConcatTitleName(player, title).c_str();
				player->ADD_GOSSIP_ITEM(ICON_CONTENT, title_name, tab, title->titleId);
			}
			else if(title->titleId <29)
				player->ADD_GOSSIP_ITEM(ICON_CONTENT, TitleEngine::ConcatTitleName(player, title).c_str(), tab, title->titleId);

    player->ADD_GOSSIP_ITEM(ICON_OPTION, "|cff00ff00|TInterface\\icons\\spell_shadow_improvedvampiricembrace:24|t|r Меню", tab, BUTTON_MENU);
    player->PlayerTalkClass->SendGossipMenu(TEXT_MENU, creature->GetGUID());
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
//******************************* Helper **************************************//
/////////////////////////////////////////////////////////////////////////////////

std::string TitleEngine::ToString(int32 value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string TitleEngine::ToColor(std::string text, ColorType color)
{
	std::stringstream ss;
	ss << "|cff" << ColorCodes[(uint32)color] << text << "|r";
	return ss.str();
}

void TitleEngine::SendText(Player* player, Creature* creature, std::string message, ColorType color)
{
    creature->MonsterWhisper(ToColor(message, color).c_str(), player, true);
}