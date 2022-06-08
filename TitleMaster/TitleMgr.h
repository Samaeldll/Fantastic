/////////////////////////////
//*** Created by Er0niX ***//
/////////////////////////////

#ifndef TITLE_MGR
#define TITLE_MGR

#define TAB_SIZE                10
#define TAB_START               0

enum TitleType
{
    NAME_BEFORE		= 0,
    NAME_AFTER		= 1
};

struct TitleGroup
{
    TitleGroup(uint32 _groupId, std::string _groupName, uint32 _groupIcon, uint32 _groupFaction)
        : groupId(_groupId), groupName(_groupName), groupIcon(_groupIcon),  groupFaction(_groupFaction)
    { }

    uint32 groupId;
    uint32 groupIcon;

    std::string groupName;
	uint32 groupFaction;
};

struct Title
{
public:
    Title(uint32 _titleNumber, uint32 _titleId, std::string _titleName, std::string _titleNameFemale, uint32 _titleType, uint32 _titleGroup, uint32 _creditAmount, uint32 _moneyAmount, uint32 _arenaAmount, uint32 _honorAmount, uint32 _killAmount, uint32 _rating2V2, uint32 _rating3V3, uint32 _rating5V5)
        : titleNumber(_titleNumber), titleId(_titleId), titleName(_titleName), titleNameFemale(_titleNameFemale), titleType((TitleType)_titleType), titleGroup(_titleGroup), creditAmount(_creditAmount), moneyAmount(_moneyAmount), arenaAmount(_arenaAmount), honorAmount(_honorAmount), killAmount(_killAmount), rating2V2(_rating2V2), rating3V3(_rating3V3), rating5V5(_rating5V5)
    { }

	uint32 titleNumber;
    uint32 titleId;

	std::string titleName; 
	std::string titleNameFemale;
	TitleType titleType;
    uint32 titleGroup;

	uint32 creditAmount;
	uint32 moneyAmount;

	uint32 arenaAmount;
    uint32 honorAmount;
    uint32 killAmount;

    uint32 rating2V2;
    uint32 rating3V3;
    uint32 rating5V5;
};

typedef std::vector<TitleGroup*> TitleGroupStore;
typedef std::vector<Title*> TitleStore;

class TitleMgr
{
public:
    static void LoadTitleGroups();
    static void LoadTitleStore();

public:
    static TitleStore GetTitleStore(Player* player, uint32 tab);
    static TitleStore GetTitleStore(Player* player);
    static Title* GetTitle(uint32 titleId);

public:
    static uint32 GetTabsCount(Player* player) { return (ceil((float)(GetTitleStore(player).size()) / (float)(TAB_SIZE)) - 1); }
    static bool HasAllTitle(Player* player) { return (TitleMgr::GetTitleStore(player).empty()); }
    static bool CheckTitle(uint32 titleId) { return (TitleMgr::GetTitle(titleId)); }

public:
    static bool CanBuy(Player* player, uint32 titleId);
    static void Buy(Player* player, uint32 titleId);

public:
    static uint32 GetGroupsCount() { return groupStore.size(); }
    static TitleGroupStore GetGroups() { return groupStore; }
    static uint32 GetTitleCount(Player* player, uint32 groupId);

public:
    static TitleGroup* GetFirstGroup() { return ((TitleMgr::GetGroupsCount()) ? groupStore[0] : NULL); }
    static TitleGroup* GetTitleGroup(uint32 groupId);

private:
    static TitleGroupStore groupStore;
    static TitleStore titleStore;
};
#endif