/////////////////////////////
//*** Created by Er0niX ***//
/////////////////////////////

#ifndef TITLE_ENGINE
#define TITLE_ENGINE

#include "TitleMgr.h"

#define BUTTON_BACK             10000
#define BUTTON_NEXT             20000

#define BUTTON_EXIT             30000
#define BUTTON_MENU             40000

#define SERVER_CREDIT_ID        90087
#define SENDER_MAIN             1000

#define COPPER_SILVER			100
#define SILVER_GOLD				100

enum ActionIcon
{
	ICON_CONTENT	= 7,
	ICON_OPTION		= 5
};

enum TitleText
{
	TEXT_MENU	= 1,
	TEXT_ALL	= 2
};

enum ColorType
{
	C_GREY				  = 0,
	C_GREEN				  = 1,
	C_BLUE				  = 2,
	C_PURPLE	   	      = 3,
	C_GOLD		          = 4,
	C_DARK_GREY		      = 5,
	C_BLACK			      = 6,
    C_RED			      = 7,
	C_BLACK_GREY          = 8,
	COLOR_MAX		      = 9
};

enum ArenaSlots
{
    ARENA_2V2			= 0,
	ARENA_3V3			= 1,
	ARENA_5V5			= 2
};

enum TitleMessage
{
	MESSAGE_FREE_TITLE			    = 0,
    MESSAGE_CANT_BUY                = 1,
    MESSAGE_CAN_BUY                 = 2,
    MESSAGE_SUCCESSFULLY_BUY        = 3,
    MESSAGE_ALREADY_BOUGHT          = 4,
    MESSAGE_CANT_BUY2               = 5,
    MESSAGE_MAX                     = 6
};

const std::string ColorCodes[COLOR_MAX] = 
{ 
	"454545", "00ff00", "0000cd", "8a2be2", "ff8c00", "1e1e1e", "000000", "FF0A0A", "4E4E4E"
};

const std::string MessageTypes[MESSAGE_MAX] = 
{
    "Это звание бесплатное в магазине.",
    "Вы не можете купить это звание, так как у вас не хватает валюты.",
    "Вы можете купить это звание.",
    "Новое звание",
    "У вас уже есть такое звание",
    "Не хватает валюты"
};

class TitleEngine
{
public:
    static bool ExistTitle(uint32 titleId) { return (sCharTitlesStore.LookupEntry(titleId)); }
    static bool IsKnownTitle(Player* player, uint32 titleId);

public:
    static bool SameFaction(uint32 faction, uint32 groupFaction) { return (groupFaction == 0 || faction == groupFaction); }
    static uint32 GetFaction(Player* player) { return ((player->GetTeam() == ALLIANCE) + 1); }

public:
    static std::string ConcatTitleName(Player* player, Title* title);
    static std::string ToColor(std::string text, ColorType color);
    static std::string ToString(int32 value);

public:
    static uint32 ToGold(uint32 copper) { return floor(copper / COPPER_SILVER / SILVER_GOLD); }
    static uint32 ToCopper(uint32 gold) { return gold * COPPER_SILVER * SILVER_GOLD; }

public:
    static std::string ObjectiveString(std::string valueName, uint32 value1, uint32 value2);
    static std::string ObjectiveMessage(Player* player, uint32 titleId);

public:
    static void SendText(Player* player, Creature* creature, std::string message, ColorType color = C_BLACK_GREY);
    static void LearnTitle(Player* player, uint32 titleId);

public:
    static bool CreatePage(Player* player, Creature* creature, uint32 tab);
    static void CreateGossip(Player* player, Creature* creature);
};
#endif