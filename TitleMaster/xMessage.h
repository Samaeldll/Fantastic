/////////////////////////////
//*** Created by Er0niX ***//
/////////////////////////////

#ifndef x_MESSAGE_H
#define x_MESSAGE_H

enum RewardType
{
    REWARD_ITEM         = 0,
    REWARD_TITLE        = 1
};

struct xMessageContent
{
	uint32 questId;
	std::string title;

    uint32 reward;
    RewardType rewardType;
};

class xMessage
{
protected: 
	xMessage(Player* _player, Creature* _creature, xMessageContent _content, uint32 _action) 
		: player(_player), creature(_creature), content(_content), action(_action)
	{ }

public:
    virtual bool CanBuy(uint32 reward) = 0;
    virtual bool OnMessageAccept() = 0;
	virtual bool OnMessageOpen() = 0;

public:
	bool SendDetails(std::string objective, std::string details = "");
	bool SendRequest(std::string objective, std::string details = "");

protected: 
	bool CheckObjects();
	void ResetObjects();

protected:
	Creature* creature;
	Player* player;

public:
	xMessageContent content;
	uint32 action;
};
#endif