#include "ScriptPCH.h"

using namespace std;

enum BossSpells 
{    
         SPELL_GHOUL_SLASH          = 91048,
         SPELL_BONEGRINDER          = 91049,
         SPELL_GORE                 = 91050,
         SPELL_GORE_1               = 91051,
         SPELL_GORE_2               = 91052,   
         SPELL_ROCK_SHIELD          = 50363,
         SPELL_BERSERKER_AURA       = 41107,
         SPELL_DIVINE_SHIELD        = 91053
         
}; 

enum Says
{
        SAY_AGGRO       = 0,
        SAY_KILL_PLAYER = 1,
        SAY_ENTER_EVADE = 2,
        SAY_DIED        = 3,
        SAY_10_PERC     = 4
 
};

class azog_conqueror : public CreatureScript 
{ 
public: 
                 azog_conqueror() : CreatureScript("azog_conqueror") { } 
       
                 CreatureAI* GetAI(Creature* creature) const 
                 { 
                 return new azog_conquerorAI (creature); 
                 } 
       
                 struct azog_conquerorAI : public ScriptedAI 
                 { 
                 azog_conquerorAI(Creature* creature) : ScriptedAI(creature),Summons(me) {} 


         uint8 Phase;
         uint32 GhoulSlashTimer;
         uint32 BonegrinderTimer;
         uint8 health_perc;
         bool berserk;
         bool Evade;
         SummonList Summons;
         uint32 KillCount;
         uint32 GoreTimer;
         uint8 desp_sum;
         uint32 goretimer;
         uint32 gore1timer;
         uint32 gore2timer;


        void Reset() 
        {
            me->RemoveAura(SPELL_BERSERKER_AURA);
            me->CombatStop(true);
            DoCast(me, 38860);
            Phase = 1;
            GhoulSlashTimer = 1000;
            BonegrinderTimer = 5000;
            health_perc = 10;
            berserk = true;
            Evade = true;
            KillCount = 0;
            GoreTimer = 1000;
            desp_sum = 0;
            goretimer = 5000;
            gore1timer = 6000;
            gore2timer = 7000;
        }

                void EnterCombat(Unit* /*who*/)
                {
                  Talk(SAY_AGGRO);
                }

                void EnterEvadeMode()
                {
                    if (Evade && !me->HasAura(SPELL_DIVINE_SHIELD))
                       {
                         me->SetFullHealth();
                         me->CombatStop(true);
                         me->GetMotionMaster()->MoveTargetedHome();
                         Talk(SAY_ENTER_EVADE);
                         Evade = false;
                         Reset();
                       }
                           
                }

                 void KilledUnit(Unit* /*victim*/)
                 {
                   Talk(SAY_KILL_PLAYER);
                 }

                 void JustDied(Unit* /*killer*/) 
                 {
                  Talk(SAY_DIED);
                 }
     
                 void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
                 {
                         {
                            if(!HealthAbovePct(80-health_perc))
                             {
                                DoCast(me, SPELL_ROCK_SHIELD);
                                health_perc +=10;
                             }
                         }
                 }

                 void SummonedCreatureDies(Creature* summoned, Unit* /*killer*/)
                   {
                     //Summons.Despawn(summoned);
                     ++KillCount;

                  if (KillCount > 5)
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->setFaction(14);
                        me->RemoveAura(SPELL_DIVINE_SHIELD);
                        DoCast(me,38860);
                    }

                   }

                   void SummonedCreatureDespawn(Creature* summon) 
                   {
                      if (summon->IsAlive())
                         {
                            me->SetFullHealth();
                            me->CombatStop(true);

                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            me->setFaction(14);
                            me->RemoveAura(SPELL_DIVINE_SHIELD);
                            Reset();
                         }
                   }

     void UpdateAI(uint32 diff) 
                 { 

            if (!UpdateVictim())
               {
                         me->CombatStop(true);
                         me->GetMotionMaster()->MoveTargetedHome(); 
                    return;                 
               }

                  if (GhoulSlashTimer <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, SPELL_GHOUL_SLASH); 
       
                     GhoulSlashTimer = 5000; 
                   } else GhoulSlashTimer -= diff; 

                  if (BonegrinderTimer <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, SPELL_BONEGRINDER); 
       
                     BonegrinderTimer = 10000; 
                   } else BonegrinderTimer-= diff; 

                  if (me->GetHealthPct() < 50.0f && berserk)
                      {
                        DoCast(me, SPELL_BERSERKER_AURA);
                        berserk = false;
                      }

                   if (Phase == 1 && me->GetHealthPct() < 80.0f)
                       {
                           if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                             {
                                   for(int i=0; i<3; i++)
                                     DoCast(target, SPELL_GORE+i); 
                             }

                         Phase = 2;
                       }

                    if(Phase!=1)
                       {
                            if (goretimer <= diff) 
                             { 
                                 if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                                   DoCast(target, SPELL_GORE); 
       
                                  goretimer = 11000; 
                               } else goretimer -= diff; 

                              if (gore1timer <= diff) 
                                { 
                                  if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                                    DoCast(target, SPELL_GORE_1); 
       
                                     gore1timer = 11000; 
                                } else gore1timer -= diff; 

                               if (gore2timer <= diff) 
                                { 
                                  if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                                     DoCast(target, SPELL_GORE_2); 
       
                                    gore2timer = 11000; 
                                } else gore2timer -= diff; 
                       }

                  
                   if (Phase == 2 && me->GetHealthPct() <= 10.0f)
                     {
                       DoCast(me, SPELL_DIVINE_SHIELD);
                       me->GetMotionMaster()->MoveTargetedHome();
                       me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                       me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                       me->setFaction(35);
                       Talk(SAY_10_PERC);
                       Phase = 3;
                     }

                   if (Phase == 3)
                     {
                       me->SummonCreature(100059,-464.141663, -2617.146240, 99.319122,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,30000);
                       me->SummonCreature(100059,-458.307983, -2615.963135, 97.924904,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,30000);
                       me->SummonCreature(100059,-453.894745, -2616.332520, 97.155853,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,30000);
                       me->SummonCreature(100059,-465.222565, -2594.732422, 101.116402,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,30000);
                       me->SummonCreature(100059,-460.280090, -2594.369629, 99.447655,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,30000);
                       me->SummonCreature(100059,-454.712463, -2592.970215, 97.413567,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,30000);
                       Phase = 4;
                     }

                   DoMeleeAttackIfReady(); 
                 } 

};
};

class wolf_az : public CreatureScript 
{ 
public: 
                 wolf_az() : CreatureScript("wolf_az") { } 
       
                 CreatureAI* GetAI(Creature* creature) const 
                 { 
                 return new wolf_azAI (creature); 
                 } 
       
                 struct wolf_azAI : public ScriptedAI 
                 { 
                 wolf_azAI(Creature* creature) : ScriptedAI(creature) {} 

        uint32 BiteTimer;
        uint32 TimeBers;
        bool bers;

        void Reset() 
        {
         bers = true;
         BiteTimer = 1000;
         TimeBers = 3000;
        }

     void UpdateAI(uint32 diff)
                 { 

            if (!UpdateVictim())
                return;

                  if (BiteTimer <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, 91054); 
       
                   BiteTimer = 5000; 
                   } else BiteTimer -= diff; 

                  if (TimeBers <= diff && bers) 
                   { 
                        DoCast(me, 71188);
                        bers = false;
                   } else TimeBers -= diff; 

                   DoMeleeAttackIfReady(); 
                 } 

};
};


void AddSC_azog_conqueror()
{
	new azog_conqueror();
       new wolf_az();
}