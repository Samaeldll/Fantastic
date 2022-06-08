enum BossSpells 
{  
                   FLAME_WAVE      = 91030,
                   BURNING_BILE    = 91031,
                   DRAGONS_BREATH  = 91033,
                   FEL_IMMOLATION  = 91034
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

class infernal_crusher : public CreatureScript 
{ 
public: 
                 infernal_crusher() : CreatureScript("infernal_crusher") { } 
       
                 CreatureAI* GetAI(Creature* creature) const 
                 { 
                 return new infernal_crusherAI (creature); 
                 } 
       
                 struct infernal_crusherAI : public ScriptedAI 
                 { 
                 infernal_crusherAI(Creature* creature) : ScriptedAI(creature) {} 

        uint32 Flame_WaweTIMER;
        uint32 Burning_BileTIMER;
        uint32 Dragons_BreathTIMER;
        bool Spell_Fel_Immolation;
        uint8 Phase;

        void Reset() 
        {
        Flame_WaweTIMER = 10000;
        Burning_BileTIMER = 7000;
        Dragons_BreathTIMER = 100;
        Spell_Fel_Immolation = false;
        Phase = 1;
        }

                void EnterCombat(Unit* /*who*/)
                {
                  Talk(SAY_AGGRO);
                }

                void EnterEvadeMode(Creature* summon)
                {
                  Talk(SAY_ENTER_EVADE);
                }

                 void KilledUnit(Unit* /*victim*/)
                 {
                   Talk(SAY_KILL_PLAYER);
                 }

                 void JustDied(Unit* /*killer*/) 
                 {
                  Talk(SAY_DIED);
                 }

     void UpdateAI(uint32 diff)
                 { 
            if (!UpdateVictim())
                return;
                
                  if (Burning_BileTIMER <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, BURNING_BILE); 
       
                   Burning_BileTIMER = 23000; 
                   } else Burning_BileTIMER -= diff; 

                   if (Phase == 1)
                      {
                        if (Flame_WaweTIMER <= diff) 
                         { 
                         if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                           DoCast(target, FLAME_WAVE); 
       
                         Flame_WaweTIMER = 25000; 
                         } else Flame_WaweTIMER -= diff; 
                      }

                   if (Phase == 1 && me->GetHealthPct() < 60.0f)
                    {
                      Phase = 2;
                      Talk(SAY_PHASE_2);
                    }

                    if (Phase == 2 || Phase == 3)
                       {
                         if (Flame_WaweTIMER <= diff) 
                         { 
                         if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                           DoCast(target, FLAME_WAVE); 
       
                         Flame_WaweTIMER = 15000; 
                         } else Flame_WaweTIMER -= diff; 

                         if (Dragons_BreathTIMER <= diff) 
                         { 
                         if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                           DoCast(target, DRAGONS_BREATH); 
       
                         Dragons_BreathTIMER = 8000; 
                         } else Dragons_BreathTIMER -= diff; 
                       }

                   if (Phase == 2 && me->GetHealthPct() < 30.0f)
                    {
                      Phase = 3;
                      Talk(SAY_PHASE_3);
                    }

                   if (Phase == 3)
                      {
                        if (!Spell_Fel_Immolation)
                          {
                           DoCast(me, FEL_IMMOLATION);
                           Spell_Fel_Immolation = true;
                           }
                      }
                  
                   DoMeleeAttackIfReady(); 
                 } 

};
};

void AddSC_infernal_crusher()
{
	new infernal_crusher();
}