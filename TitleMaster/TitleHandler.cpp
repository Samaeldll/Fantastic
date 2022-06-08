/////////////////////////////
////***Created by EroniX***//
/////////////////////////////

#include "TitleHandler.h"
#include "TitleEngine.h"
#include "TitleMgr.h"
#include "xMessage.h"

//////////////////////////////////////////////////////////////
//********************* CreatureScript *********************//
//////////////////////////////////////////////////////////////

class NpcTitle : public CreatureScript
{
public:
    NpcTitle() : CreatureScript("NpcTitle") { }

	bool OnGossipHello(Player* player, Creature* creature)
    {
        if(player && player->PlayerTalkClass && creature)
		    TitleEngine::CreateGossip(player, creature);

		return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
		if(!player || !creature)
			return true;

        if(sender == SENDER_MAIN) 
        {
            if(action == BUTTON_EXIT)
		    {
			    player->PlayerTalkClass->SendCloseGossip();
			    return true;
		    }

            player->titleGroup = TitleMgr::GetTitleGroup(action);
            TitleEngine::CreatePage(player, creature, TAB_START);
            return true;
        }

        if(!player->titleGroup)
        {
			player->PlayerTalkClass->SendCloseGossip();
			return true;
        }
        
		if(action == BUTTON_MENU)
		{
			TitleEngine::CreateGossip(player, creature);
			return true;
		}

		if(action >= BUTTON_NEXT)
		{
			TitleEngine::CreatePage(player, creature, sender + 1);
			return true;
		}

		if(action >= BUTTON_BACK)
		{
			TitleEngine::CreatePage(player, creature, sender - 1);
			return true;
		}

        TitleEngine::CreatePage(player, creature, sender);	

        if(TitleEngine::ExistTitle(action) && TitleMgr::CheckTitle(action) && !TitleEngine::IsKnownTitle(player, action))
        {
            xMessageContent content = { QUEST_ID, QUEST_TITLE, action, REWARD_TITLE };
            player->xMessageScript = new TitleHandler(player, creature, content, sender);
            player->xMessageScript->OnMessageOpen();
        }

        return true;
	}	
};

//////////////////////////////////////////////////////////////
//********************* WorldScript ************************//
//////////////////////////////////////////////////////////////

class StartUpTitleNPC : public WorldScript
{
public:
    StartUpTitleNPC() : WorldScript("StartUpTitleNPC") { }

	void OnStartup() 
	{
        TitleMgr::LoadTitleGroups();
        TitleMgr::LoadTitleStore();
	}
};

//////////////////////////////////////////////////////////////
//******************** CommandScript ***********************//
//////////////////////////////////////////////////////////////

class TitleCommand : public CommandScript
{
public:
    TitleCommand() : CommandScript("TitleCommand") { }

	ChatCommand* GetCommands() const
    {
        static ChatCommand IngameCommandTable[] =
        {
			{ "reloadtitlestore",  SEC_CONSOLE,  	true,	&HandleTitleSystemReloadCommand,    "",		NULL },
            { "reloadtitlegroups", SEC_CONSOLE,  	true,	&HandleGroupSystemReloadCommand,    "",		NULL },
            { NULL,				   0,			    false,  NULL,								"",		NULL }
        };
		return IngameCommandTable;
    }

	static bool HandleTitleSystemReloadCommand(ChatHandler* handler, const char* args)
    {
		TitleMgr::LoadTitleStore();
		handler->SendSysMessage("Reloaded table title_store.");
		return true;
    }

    static bool HandleGroupSystemReloadCommand(ChatHandler* handler, const char* args)
    {
		TitleMgr::LoadTitleGroups();
		handler->SendSysMessage("Reloaded table title_groups.");
		return true;
    }
};

//////////////////////////////////////////////////////////////
//*********************** Loader ***************************//
//////////////////////////////////////////////////////////////

void AddSC_NpcTitle()
{
	new StartUpTitleNPC();
	new NpcTitle();
	new TitleCommand();
}

//////////////////////////////////////////////////////////////
//************************ xMessage ************************//
//////////////////////////////////////////////////////////////

bool TitleHandler::OnMessageAccept()
{
    if(!player || !creature)
		return true;

    if(!TitleEngine::ExistTitle(content.reward) || !TitleMgr::CheckTitle(content.reward))
    {
        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

    if(!TitleEngine::IsKnownTitle(player, content.reward))
    {
        if(TitleMgr::CanBuy(player, content.reward))
        {
            TitleEngine::LearnTitle(player, content.reward);
            TitleMgr::Buy(player, content.reward);

            TitleEngine::SendText(player, creature, MessageTypes[MESSAGE_SUCCESSFULLY_BUY]);
            TitleEngine::CreatePage(player, creature, action);
            return true;
        }

        TitleEngine::SendText(player, creature, MessageTypes[MESSAGE_CANT_BUY2]);
        return true;
    }

    TitleEngine::SendText(player, creature, MessageTypes[MESSAGE_ALREADY_BOUGHT]);
	return true;
}

bool TitleHandler::OnMessageOpen()
{
    if(!player || !creature)
		return true;

    if(CanBuy(content.reward)) 
		SendDetails(TitleEngine::ObjectiveMessage(player, content.reward), MessageTypes[MESSAGE_CAN_BUY]);
	else
		SendRequest(TitleEngine::ObjectiveMessage(player, content.reward), MessageTypes[MESSAGE_CANT_BUY]);

	return true;
}

bool TitleHandler::CanBuy(uint32 reward)
{
    return (TitleMgr::CanBuy(player, reward));
}