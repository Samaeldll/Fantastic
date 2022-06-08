/////////////////////////////
//*** Created by Er0niX ***//
/////////////////////////////

#include "xMessage.h"

//////////////////////////////////////////////////////////////
//************************ Sender **************************//
//////////////////////////////////////////////////////////////

bool xMessage::SendDetails(std::string objective, std::string details)
{
	if(!CheckObjects())
		return false;

	WorldPacket data(SMSG_QUESTGIVER_QUEST_DETAILS, 100);  
	data << uint64(creature->GetGUID()) << uint64(player->GetDivider()) << uint32(content.questId);

	data << content.title << details << objective;
	data << uint8(1) << uint32(0) << uint32(0) << uint8(0);  

    if(content.reward != 0 && content.rewardType == REWARD_ITEM)                                  
	{
		data << uint32(0) << uint32(1) << uint32(content.reward) << uint32(1);

		if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(content.reward))
			data << uint32(itemTemplate->DisplayInfoID);
		else
			data << uint32(0);
        
		data << uint32(0) << uint32(0);
	}
	else
		data << uint32(0) << uint32(0) << uint32(0) << uint32(0);    

	data << uint32(0) << float(0.0f) << uint32(0) << int32(-1);

    if(content.rewardType == REWARD_TITLE)
        data << uint32(content.reward);
    else
        data << uint32(0);

    data << uint32(0) << uint32(0) << uint32(0);

	for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
		data << uint32(0);
	for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << int32(-1) << int32(-1);

	data << uint32(QUEST_EMOTE_COUNT);
	for (uint32 i = 0; i < QUEST_EMOTE_COUNT; ++i)
		data << uint32(0) << uint32(0);

	player->GetSession()->SendPacket(&data);
	return true;
}

bool xMessage::SendRequest(std::string objective, std::string details)
{
	if(!CheckObjects())
		return false;

	WorldPacket data(SMSG_QUESTGIVER_REQUEST_ITEMS, 50); 
	data << uint64(creature->GetGUID()) << uint32(content.questId);
	data << content.title << (details + "\n\n" + objective); 

	data << uint32(0) << uint32(0) << uint32(0) << uint32(0) << uint32(0) << uint32(0) << uint32(0);
	data << uint32(0x00) << uint32(0x04) /*<< uint32(0x08) << uint32(0x10)*/;

	player->GetSession()->SendPacket(&data);
	return true;
}

//////////////////////////////////////////////////////////////
//************************ Helper **************************//
//////////////////////////////////////////////////////////////

bool xMessage::CheckObjects()
{
	if(player && creature) 
		if(player->IsAlive() && player->GetNPCIfCanInteractWith(creature->GetGUID(), UNIT_NPC_FLAG_NONE))
			return true;

    return false;
}

void xMessage::ResetObjects()
{
	player->xMessageScript = NULL;
	delete this;
}