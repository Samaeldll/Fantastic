/*
<--------------------------------------------------------------------------->
 - Developer(s): Ghostcrawler336
 - Made By: ??
 - Complete: 100%
 - ScriptName: 'Buff Command'
 - Comment: Tested & Working
<--------------------------------------------------------------------------->
*/
#include "ScriptPCH.h"
#include "Chat.h"
 
 
uint32 auras[] = { 48162, 48074, 48170, 43223, 36880, 467, 48469 };
 
class buffcommand : public CommandScript
{
public:
    buffcommand() : CommandScript("buffcommand") { }
 
        ChatCommand* GetCommands() const
    {
        static ChatCommand IngameCommandTable[] =
        {
        { "buff",           SEC_PLAYER,         false,  &HandleBuffCommand,                "" }
        };
                 return IngameCommandTable;
    }
 
        static bool HandleBuffCommand(ChatHandler * handler, const char * args)
    {
        Player * pl = handler->GetSession()->GetPlayer();
                if(pl->InArena())
                {
                        pl->GetSession()->SendNotification("Вы не можете использовать этот предмет в матче на арене!");
                        return false;
                }
       
                pl->RemoveAurasByType(SPELL_AURA_MOUNTED);
                for(int i = 0; i < 7; i++)
                    pl->AddAura(auras[i], pl);
                handler->PSendSysMessage("|TInterface/ICONS/Achievement_bg_trueavshutout:25|tВы были избалованы, наслаждайтесь!");
                return true;
 
    }
};
 
void AddSC_buffcommand()
{
    new buffcommand();
}
