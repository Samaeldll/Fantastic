enum BossSpells 
{  
    SPELL_DARING                        = 30841,
    SPELL_BLESSING_OF_PROTECTION        = 91041,
    SPELL_BLESSING_OF_SPELL_WARDING     = 91042,
    SPELL_DIVINE_STORM                  = 91037,
    SPELL_HOLY_SMITE                    = 91038,
    SPELL_AVENGING_WRATH                = 66011,
    SPELL_HOLYFORM                      = 91044,
    SPELL_RENEW                         = 91040,
    SPELL_CONSECRATION                  = 91043
     
         
}; 

enum Says
{
    SAY_AGGRO                       = 0,
    SAY_PHASE_2                     = 1,
    SAY_KILL_PLAYER                 = 2,
    SAY_PHASE_3                     = 3,
    SAY_DIED                        = 4,
    SAY_ENTER_EVADE                 = 5
};

class archangel_michael : public CreatureScript 
{ 
public: 
                 archangel_michael() : CreatureScript("archangel_michael") { } 
       
                 CreatureAI* GetAI(Creature* creature) const 
                 { 
                 return new archangel_michaelAI (creature); 
                 } 
       
                 struct archangel_michaelAI : public ScriptedAI 
                 { 
                 archangel_michaelAI(Creature* creature) : ScriptedAI(creature) {} 

        uint8 HealthPct;
        uint8 Phase;
        uint32 DivineStormTimer;
        uint32 DaringTimer;
        uint32 HolySmiteTimer;
        uint32 RenewTimer;
        uint32 BlessingTimer;
        uint32 BlessingTimer1;
        uint32 ConsecrationTimer;
        bool HolyForm;

        void Reset() 
        {
           HolyForm = false;
           ConsecrationTimer = 1000;
           BlessingTimer = 5000;
           BlessingTimer = 20000;
           RenewTimer = 1000;
           HolySmiteTimer = 3000;
           DivineStormTimer = 6000;
           DaringTimer = 10000;

           HealthPct = 1;
           Phase = 1;
        }

                void EnterCombat(Unit* /*who*/)
                {
                  //Talk(SAY_AGGRO);
                }

                void EnterEvadeMode(Creature* summon)
                {
                  //Talk(SAY_ENTER_EVADE);
                }

                 void KilledUnit(Unit* /*victim*/)
                 {
                   Talk(SAY_KILL_PLAYER);
                 }

                 void JustDied(Unit* /*killer*/) 
                 {
                  //Talk(SAY_DIED);
                 }
     
                 void DamageTaken(Unit* attacker, uint32& /*damage*/)
                 {
                     if(!HealthAbovePct(100-30*HealthPct))
                       {
                          DoCast(me, SPELL_AVENGING_WRATH);
                          HealthPct++;
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

              if (me->GetHealthPct() < 65.0f && Phase == 1)
                  Phase = 2;

              if (me->GetHealthPct() < 45.0f && Phase == 2)
                  Phase = 3;

            if (Phase == 3)
              {
                 if (!HolyForm)
                   {
                    DoCast(me, SPELL_HOLYFORM);
                    HolyForm = true;
                   }

                  if (ConsecrationTimer <= diff) 
                   { 
                     DoCastAOE(SPELL_CONSECRATION);
       
                     ConsecrationTimer = 8000; 
                   } else ConsecrationTimer -= diff; 

              }

            if(Phase == 2 || Phase == 3)
              {
                  if (RenewTimer <= diff) 
                   { 
                     DoCast(me,SPELL_RENEW);
       
                     RenewTimer = 30000; 
                   } else RenewTimer -= diff; 

                  if (BlessingTimer <= diff) 
                   { 
                     DoCast(me,SPELL_BLESSING_OF_PROTECTION);
       
                     BlessingTimer = 45000; 
                   } else BlessingTimer -= diff; 

                  if (BlessingTimer1 <= diff) 
                   { 
                     DoCast(me,SPELL_BLESSING_OF_SPELL_WARDING);
       
                     BlessingTimer1 = 45000; 
                   } else BlessingTimer1 -= diff; 

              }
               

                  if (HolySmiteTimer <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, SPELL_HOLY_SMITE); 
       
                   HolySmiteTimer = 6500; 
                   } else HolySmiteTimer -= diff; 


                  if (DivineStormTimer <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, SPELL_DIVINE_STORM); 
       
                   DivineStormTimer = 10000; 
                   } else DivineStormTimer -= diff; 

                  if (DaringTimer <= diff) 
                   { 
                     DoCast(me, SPELL_DARING); 
       
                   DaringTimer = 25000; 
                   } else DaringTimer -= diff; 

                  
                   DoMeleeAttackIfReady(); 
                 } 

};
};

void AddSC_archangel_michael()
{
	new archangel_michael();
}