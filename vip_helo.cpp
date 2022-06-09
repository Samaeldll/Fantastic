#include "ScriptPCH.h"  
#include "AccountMgr.h"  
#include "Config.h" 

 class vip_hello : public PlayerScript  
 {  
 public:  
        vip_hello() : PlayerScript("vip_hello") {}  

     void OnLogin(Player* player)  
     
          { 
             if (player->GetSession()->IsPremium())  
                  {  
                      std::string sText = ("|cffff0000[V.I.P Игрок]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r входит в Игровой мир.");  
                        sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
                  }  
          }   
 }; 
      
  void OnLogout(Player* player) 
  {      
       if (player->GetSession()->IsPremium())  
              {  
                  std::string sText = ("|cffff0000[V.I.P Игрок]|r |cFF60FF00" + std::string(player->GetSession()->GetPlayerName()) + "|r выходит из Игрового мира.");  
                  sWorld->SendServerMessage(SERVER_MSG_STRING, sText.c_str());  
              }      
 };  

 void AddSC_vip_hello()  
 {  
        new vip_hello();  
 }