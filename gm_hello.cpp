#include "ScriptPCH.h"  
#include "AccountMgr.h"  
#include "Config.h" 

 class gm_hello : public PlayerScript  
 {  
 public:  
        gm_hello() : PlayerScript("gm_hello") {}  

     void OnLogin(Player* player)  
     {  
         if (sWorld->getBoolConfig(CONFIG_GM_SETTINGS_ON_INPUT)) 
         { 
       if (player->GetSession()->GetSecurity() == SEC_MODERATOR)  
              {  
                  std::string sText = ("|cffff0000[Модератор]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  
if (player->GetSession()->GetSecurity() == SEC_TESTER)  
              {  
                  std::string sText = ("|cffff0000[Тестер]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }
    if (player->GetSession()->GetSecurity() == SEC_GAMEMASTER)  
              {  
                  std::string sText = ("|cffff0000[Эвент-мастер]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  
    if (player->GetSession()->GetSecurity() == SEC_ADMINISTRATOR)  
              {  
                  std::string sText = ("|cffff0000[Гейм-мастер]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              } 
	if (player->GetSession()->GetSecurity() == SEC_CONSOLE)
	{
		std::string sText = ("|cffff0000[Разработчик проекта]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r входит в Игровой мир.");
		sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
	}
    if (player->GetSession()->GetSecurity() == SEC_KURATOR)  
              {  
                  std::string sText = ("|cffff0000[Куратор Проекта]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r входит в Игровой мир.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  
   }   
  }  
      
  void OnLogout(Player* player) 
  {  
         if (sWorld->getBoolConfig(CONFIG_GM_SETTINGS_ON_EXIT)) 
         {  
       if (player->GetSession()->GetSecurity() == SEC_MODERATOR)  
              {  
                  std::string sText = ("|cffff0000[Модератор]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  

if (player->GetSession()->GetSecurity() == SEC_TESTER)  
              {  
                  std::string sText = ("|cffff0000[Тестер]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              } 
    if (player->GetSession()->GetSecurity() == SEC_GAMEMASTER)  
              {  
                  std::string sText = ("|cffff0000[Эвент-мастер]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }  
    if (player->GetSession()->GetSecurity() == SEC_ADMINISTRATOR)  
              {  
                  std::string sText = ("|cffff0000[Гейм-мастер]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              } 
	if (player->GetSession()->GetSecurity() == SEC_CONSOLE)
	{
		std::string sText = ("|cffff0000[Разработчик проекта]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r выходит из Игрового мира.");
		sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());
	}
    if (player->GetSession()->GetSecurity() == SEC_KURATOR)  
              {  
                  std::string sText = ("|cffff0000[Куратор Проекта]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              } 
   }   
  } 
 };  

 void AddSC_gm_hello()  
 {  
        new gm_hello();  
 }