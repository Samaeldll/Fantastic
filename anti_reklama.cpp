#include "ScriptPCH.h"
#include "Channel.h"
 
 
class anti_rerlama : public PlayerScript
{
public:
         anti_rerlama() : PlayerScript("anti_rerlama") {}

         void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg)
         {
                 CheckMessage(player, msg, lang, NULL, NULL, NULL, NULL);
         }
        
         void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Player* receiver)
         {
                 CheckMessage(player, msg, lang, receiver, NULL, NULL, NULL);
         }
        
         void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Group* group)
         {
                 CheckMessage(player, msg, lang, NULL, group, NULL, NULL);
         }
        
         void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Guild* guild)
         {
                 CheckMessage(player, msg, lang, NULL, NULL, guild, NULL);
         }
        
         void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Channel* channel)
         {
                 CheckMessage(player, msg, lang, NULL, NULL, NULL, channel);
         }
  
void CheckMessage(Player* player, std::string& msg, uint32 lang, Player* /*receiver*/, Group* /*group*/, Guild* /*guild*/, Channel* channel)
{
if (sWorld->getBoolConfig(CONFIG_CHECK_SPAMM))
         {
     //if (player->isGameMaster() || lang == LANG_ADDON)
             //return;
  
     // transform to lowercase (for simpler checking)
     std::string lower = msg;
     std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
  
     const uint8 cheksSize = 70;
    std::string checks[cheksSize];
    checks[0] ="http://";
    checks[1] =".com";
    checks[2] =".net";
    checks[3] =".org";
    checks[4] =".ru";
    checks[5] ="rondor";
    checks[6] ="www.";
	checks[7] ="loadwow";
    checks[8] ="load-wow";
    checks[9] ="relax-wow";
    checks[10] ="uwow";
    checks[11] =".ua";
    checks[12] ="apael";
	checks[13] ="-wow";
	checks[14] ="wow-";
	checks[15] ="relax";
	checks[16] ="wow-grand";
	checks[17] ="grand-w0w";
	checks[18] ="revolt-wow.net";
	checks[19] ="revolt-wow.ru";
	checks[20] ="revolt-w0w";
	checks[21] ="ussr-wow.ru";
	checks[22] ="ussr-wow";
	checks[23] ="lexer-wow";
	checks[24] =".biz";
	checks[25] ="jet-wow";
	checks[26] =".su";
	checks[27] ="awow.biz";
	checks[28] ="r u";
    checks[29] =". r u";
    checks[30] =".c o m";
    checks[31] =".biz";
    checks[32] ="immortal-wow.biz";
    checks[33] ="unreal-wow.biz";
    checks[34] =". b i z";
    checks[35] ="draenor.su";
    checks[36] ="draenor. s u";
    checks[37] ="draenor . s/u";
    checks[38] ="d r a e n o r . s u";
    checks[39] ="underwow.ru";
    checks[40] ="wow-russian.net";
    checks[41] ="ed-tournament.ru";
    checks[42] ="e d - t o u r n a m e n t . r u";
    checks[43] ="wowdm";
    checks[44] ="rasta-wow.ru";
    checks[45] ="r a s t a";
    checks[46] ="wow-qrsk.ru";
    checks[47] ="wowchrom.ru";
    checks[48] ="w o w c h r o m .r u";
    checks[49] ="wow-odon.ru";
    checks[50] ="w o w - o d o n . r u";
	checks[51] ="u n d e r w o w . r u";
	checks[52] ="wowcool.ru";
	checks[53] ="zerox-wow.ru";
	checks[53] ="z e r o x - w o w";
    checks[54] ="увов";
    checks[55] ="ушош";
    checks[56] ="киркл";
    checks[57] ="circle";
	checks[58] ="unreal-wow";
	checks[59] =" u n r e a l w o w";
	checks[60] ="immortal-wow";
	checks[61] ="i m m o r t a l w o w";
	checks[62] ="msw-wow";
	checks[63] ="m s w w o w";
	checks[64] ="skream";
	checks[65] ="s k r e a m";
	checks[66] ="eternity-wow";
	checks[67] ="етернити вов";
	checks[68] ="smoll-wow";
	checks[69] ="evolution-wow";
     for (int i = 0; i < cheksSize; ++i)
         if (lower.find(checks[i]) != std::string::npos)
         {
             msg = "Я |cffFF66CCлюблю|r Fantastic-WoW [АнтиРеклама]";
             ChatHandler(player).PSendSysMessage("Реклама запрещена!");           
             return;
         }
 		}
}   
};
 
void AddSC_reklama()
{
   new anti_rerlama;
}