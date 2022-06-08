/////////////////////////////
//*** Created by Er0niX ***//
/////////////////////////////

#ifndef TITLE_HANDLER
#define TITLE_HANDLER

#include "xMessage.h"

#define QUEST_TITLE     "Новое звание"
#define QUEST_ID        150000

class TitleHandler : public xMessage
{
public:
	TitleHandler(Player* player, Creature* creature, xMessageContent content, uint32 action) 
		: xMessage(player, creature, content, action)
	{ }

public:
    bool CanBuy(uint32 reward);
	bool OnMessageAccept();
	bool OnMessageOpen();
};
#endif