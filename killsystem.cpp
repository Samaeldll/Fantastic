#include "ScriptPCH.h"
#include "Language.h"
#include <cstring>

bool OnlyInBattlegrounds = true; //Set to true if you want to enable this only in battlegrounds.
int32 ItemReward = 90035; //The ItemID of the reward.
int32 AmountOfRewardsOnKillStreak[5] = { 3, 6, 9, 12, 15 }; //With how many items you want to reward the killer when he slays the victim.
int32 HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim = 2; //Name speaks for It self.
const int32 KillerStreak1 = 10; 
const int32 KillerStreak2 = 20; 
const int32 KillerStreak3 = 30; 
const int32 KillerStreak4 = 40; 
const int32 KillerStreak5 = 50; 

struct SystemInfo
{
    uint32 KillStreak;
    uint32 LastGUIDKill;
    uint8 KillCount;
};

static std::map<uint32, SystemInfo> KillingStreak;

class System_OnPvPKill : public PlayerScript
{
    public:
        System_OnPvPKill() : PlayerScript("System_OnPvPKill") {}

        
    void OnPVPKill(Player *pKiller, Player *pVictim)
     {
        uint32 kGUID; 
        uint32 vGUID;
        char msg[500];
        kGUID = pKiller->GetGUID();
        vGUID = pVictim->GetGUID();                 

      if (pKiller->GetSession()->GetRemoteAddress() == pVictim->GetSession()->GetRemoteAddress())
          return;

        if(kGUID == vGUID)
          return;

      if(KillingStreak[kGUID].LastGUIDKill == vGUID)
        {
         KillingStreak[kGUID].KillCount++;
         KillingStreak[vGUID].KillCount = 1;
        }

      if(KillingStreak[kGUID].LastGUIDKill != vGUID)
      {
        KillingStreak[kGUID].KillCount = 1;
        KillingStreak[vGUID].KillCount = 1;
      }

     if(KillingStreak[kGUID].KillCount == HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim)
        return;

      if(OnlyInBattlegrounds == true)
      {
        if(!pKiller->GetMap()->IsBattleground())
           return;
      }

        KillingStreak[kGUID].KillStreak++;
        KillingStreak[kGUID].LastGUIDKill = vGUID;
                
        switch(KillingStreak[kGUID].KillStreak)
        {
        case 5:
        sprintf(msg, "|cffFF0000%s|cffFFFF05 killed |cffFF0000%s|cffFFFF05 and is on a |cffFF00005|cffFFFF05 kill streak. ", pKiller->GetName().c_str(), pVictim->GetName().c_str());
        sWorld->SendWorldText(LANG_PVPKILL_SYSTEM, msg);
        pKiller->AddItem(90006, 3);
        break;

        case KillerStreak1:
        sprintf(msg, "|cffFF0000%s|cffFFFF05 killed |cffFF0000%s|cffFFFF05 and is on a |cffFF000010|cffFFFF05 kill streak. ", pKiller->GetName().c_str(), pVictim->GetName().c_str());
        sWorld->SendWorldText(LANG_PVPKILL_SYSTEM, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[0]);
        pKiller->AddItem(90006, 3);
	 pKiller->ModifyHonorPoints(100);
        break;

        case KillerStreak2:
        sprintf(msg, "|cffFF0000%s|cffFFFF05 killed |cffFF0000%s|cffFFFF05 and is on a |cffFF000020|cffFFFF05 kill streak. ", pKiller->GetName().c_str(), pVictim->GetName().c_str());
        sWorld->SendWorldText(LANG_PVPKILL_SYSTEM, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[1]);
        pKiller->AddItem(90006, 5);
	 pKiller->ModifyHonorPoints(100);
        break;

        case KillerStreak3:
        sprintf(msg, "|cffFF0000%s|cffFFFF05 killed |cffFF0000%s|cffFFFF05 and is on a |cffFF000030|cffFFFF05 kill streak. ", pKiller->GetName().c_str(), pVictim->GetName().c_str());
        sWorld->SendWorldText(LANG_PVPKILL_SYSTEM, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[2]);
        pKiller->AddItem(90006, 5);
	 pKiller->ModifyHonorPoints(100);
        break;

        case KillerStreak4:
        sprintf(msg, "|cffFF0000%s|cffFFFF05 killed |cffFF0000%s|cffFFFF05 and is on a |cffFF000040|cffFFFF05 kill streak. ", pKiller->GetName().c_str(), pVictim->GetName().c_str());
        sWorld->SendWorldText(LANG_PVPKILL_SYSTEM, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[3]);
	 pKiller->ModifyHonorPoints(100);
         pKiller->AddItem(90006, 5);
        break;

        case KillerStreak5:
        sprintf(msg, "|cffFF0000%s|cffFFFF05 killed |cffFF0000%s|cffFFFF05 and is on a |cffFF000050|cffFFFF05 kill streak. ", pKiller->GetName().c_str(), pVictim->GetName().c_str());
        sWorld->SendWorldText(LANG_PVPKILL_SYSTEM, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[4]);
        KillingStreak[kGUID].KillStreak = 0;
	 pKiller->ModifyHonorPoints(100);
        break;
     }
   }
};

void AddSC_System()
{
    new System_OnPvPKill;
}
