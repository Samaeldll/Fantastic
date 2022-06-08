enum BossSpells 
{  
    SPELL_MIMIRONS_INFERNO                        = 91055,
    SPELL_GREATER_FIREBALL                        = 91056,
    SPELL_FIRE_DEFENSE                            = 91057,
    SPELL_FIRE_DEFENSE_2                          = 91058,
    SPELL_EXPLOSION                               = 91059,
    SPELL_STUN_BOMB                               = 16497,
    SPELL_INCINERATE_FLESH                        = 91060,
    SPELL_FLAME_BREATH                            = 91062,
    SPELL_NETHER_POWER                            = 91063                                      
         
}; 

enum Says
{
    SAY_AGGRO                       = 0,
    SAY_PHASE_3                     = 1,
    SAY_DIED                        = 2
};

class arhitel : public CreatureScript 
{ 
public: 
                 arhitel() : CreatureScript("arhitel") { } 
       
                 CreatureAI* GetAI(Creature* creature) const 
                 { 
                 return new arhitelAI (creature); 
                 } 
       
                 struct arhitelAI : public ScriptedAI 
                 { 
                 arhitelAI(Creature* creature) : ScriptedAI(creature) {} 


           uint8 health_perc;
           uint8 Phase;
           uint32 GreaterFireballTimer;
           uint32 ExplosionTimer;
           uint32 IncinerateFleshTimer;
           uint32 FlameBreathTimer;
           bool defense;
           bool damage;
           bool mimiron;


        void Reset() 
        {
           Phase = 1;
           health_perc = 20;
           GreaterFireballTimer = 1000;
           ExplosionTimer = 1000;
           IncinerateFleshTimer = 1000;
           FlameBreathTimer = 3000;
           defense = false;
           damage = false;
           mimiron = false;
        }

                void EnterCombat(Unit* /*who*/)
                {
                  Talk(SAY_AGGRO);
                  DoCast(me, SPELL_FIRE_DEFENSE);
                }

                 void JustDied(Unit* /*killer*/) 
                 {
                  Talk(SAY_DIED);
                 }
     
                 void DamageTaken(Unit* attacker, uint32& /*damage*/)
                 {
                            if(!HealthAbovePct(100-health_perc))
                             {
                              if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                                  DoCast(target, SPELL_STUN_BOMB); 
                                health_perc +=20;
                             }

                           if (me->GetHealthPct() < 70.0f && Phase == 1)
                            {
                               DoCast(me, SPELL_FIRE_DEFENSE_2);
                               Phase = 2;
                            }

                           if (me->GetHealthPct() < 35.0f && !damage && !mimiron)
                            {
                               DoCast(me, SPELL_NETHER_POWER); 
                              if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                                  DoCast(target, SPELL_MIMIRONS_INFERNO); 

                               damage = true;
                               mimiron = true;
                            }
 
                 }

     void UpdateAI(uint32 diff)
                 { 

            if (!UpdateVictim())
                return;

                  if (GreaterFireballTimer <= diff) 
                   { 
                      if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                        DoCast(target, SPELL_GREATER_FIREBALL); 

                     GreaterFireballTimer = 7000; 
                   } else GreaterFireballTimer -= diff; 


                  if (Phase == 2)
                      {
                            if (ExplosionTimer <= diff) 
                              { 
                                 if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                                   DoCast(target, SPELL_EXPLOSION); 

                                   ExplosionTimer = 3000; 
                              } else ExplosionTimer -= diff; 

                         if (me->GetHealthPct() < 50.0f)
                          {
                             Talk(SAY_PHASE_3);
                             Phase = 3;
                          }
                      }

                  if (Phase == 3)
                     {
                            if (IncinerateFleshTimer <= diff) 
                              { 
                                 if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                                   DoCast(target, SPELL_INCINERATE_FLESH); 

                                   IncinerateFleshTimer = 30000; 
                              } else IncinerateFleshTimer -= diff; 


                          if (me->GetHealthPct() < 20.0f && !defense)
                             {
                               DoCast(me, SPELL_FIRE_DEFENSE_2);
                               defense = true;
                             }

                          if (me->GetHealthPct() < 20.0f)
                           {
                               if (!defense)
                                 {
                                    DoCast(me, SPELL_FIRE_DEFENSE_2);
                                    defense = true;
                                 }

                            if (FlameBreathTimer <= diff) 
                              { 
                                   DoCastVictim(SPELL_FLAME_BREATH); 

                                   FlameBreathTimer = 5000; 
                              } else FlameBreathTimer -= diff; 
                           }

                            if (me->GetHealthPct() < 15.0f && ExplosionTimer <= diff) 
                              { 
                                 if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                                   DoCast(target, SPELL_EXPLOSION); 

                                   ExplosionTimer = 3000; 
                              } else ExplosionTimer -= diff; 
                             
                        
                     }

                   DoMeleeAttackIfReady(); 
                 } 

};
};

void AddSC_arhitel()
{
	new arhitel();
}
