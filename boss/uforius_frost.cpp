enum BossSpells 
{  
                   SPELL_FROST_BALL      = 91014,
                   SPELL_ICE_SHOT        = 91015, 
                   SPELL_GLACIAL_BLAST   = 91016, 
                   SPELL_ICY_WINDS       = 91036,
                   SPELL_ARCTIC_CHILL    = 91017,
                   SPELL_ICE_PRISON      = 91019,
}; 

enum Says
{
    SAY_AGGRO                       = 0,
    SAY_SUMMONED_DIES               = 1,
    SAY_PLAYER_KILL                 = 2,
    SAY_DIE                         = 3
};

class uforius_frost : public CreatureScript 
{ 
public: 
                 uforius_frost() : CreatureScript("uforius_frost") { } 
       
                 CreatureAI* GetAI(Creature* creature) const 
                 { 
                 return new uforius_frostAI (creature); 
                 } 
       
                 struct uforius_frostAI : public ScriptedAI 
                 { 
                 uforius_frostAI(Creature* creature) : ScriptedAI(creature) {} 

        bool HasArcticChill;
        
        uint32 GlacialBlast1Timer;
        uint32 GlacialBlast2Timer;
        uint32 GlacialBlast3Timer;
        uint32 GlacialBlast4Timer;
        uint32 FrostBallTimer;
        uint32 IcyWindsTimer;
        uint8 Phase;

        void Reset() 
        {
        HasArcticChill = false;
        GlacialBlast1Timer = 100;
        GlacialBlast2Timer = 100;
        GlacialBlast3Timer = 100;
        GlacialBlast4Timer = 100;
        FrostBallTimer = 1000;
        IcyWindsTimer = 100;
        Phase = 1;
        }

                void EnterCombat(Unit* /*who*/)
                {
                 if (Phase == 1)
                 {
                 Talk(SAY_AGGRO);
                 me->SummonCreature(100014,me->GetPositionX() ,me->GetPositionY()+1, me->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,300000);
                 me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                 me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                 me->setFaction(35);
                
                 if (Phase == 3)
                  {
                   Phase=1;
                  } 
                 }
                }

                void EnterEvadeMode(Creature* summon)
                {
                 if (Phase == 2 && summon->IsAlive())
                 {
                 Phase = 1;
                 summon->DisappearAndDie();
                 me->CombatStop(true);
                 me->GetMotionMaster()->MoveTargetedHome();
                 }
                }

                void JustSummoned(Creature* summoned)
                {
                Phase = 2;
                }

                void SummonedCreatureDies(Creature* summoned, Unit* /*who*/)
                { 
                 Phase = 3;
                 Talk(SAY_SUMMONED_DIES);
                 me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                 me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                 me->setFaction(14);
                }

                 void SummonedCreatureDespawn(Creature* summon)
                 {
                 Phase = 1;
                 }

                 void KilledUnit(Unit* /*victim*/)
                 {
                 Talk(SAY_PLAYER_KILL);
                 if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                 DoCast(target, SPELL_ICE_SHOT);
                 }

                 void JustDied(Unit* /*killer*/) 
                 {
                 Talk(SAY_DIE);
                 DoCastAOE(SPELL_ICE_PRISON, true); 
                 }

     void UpdateAI(uint32 diff)
                 { 

            if (!UpdateVictim())
                return;

                   if (me->GetHealthPct() < 50.0f && !HasArcticChill)
                   {
                   DoCast(me, SPELL_ARCTIC_CHILL);
                   HasArcticChill = true;
                   }

                 if (Phase == 3)
                {
                  if (FrostBallTimer <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, SPELL_FROST_BALL); 
       
                   FrostBallTimer = 15000; 
                   } else FrostBallTimer -= diff; 
                 }

                 if (Phase == 4)
                {
                  if (FrostBallTimer <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, SPELL_FROST_BALL); 
       
                   FrostBallTimer = 7000; 
                   } else FrostBallTimer -= diff; 
                 }

                 if (me->GetHealthPct() < 20.0f && Phase == 3)
                { 
                  Phase = 4;
                  if (IcyWindsTimer <= diff) 
                   { 
                 DoCast(me, SPELL_ICY_WINDS);
       
                   IcyWindsTimer = 110000; 
                   } else IcyWindsTimer -= diff;
                }

                 if (me->GetHealthPct() < 90.0f && Phase == 3)
                {
                  if (GlacialBlast1Timer <= diff) 
                   { 
                 if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                 DoCast(target, SPELL_GLACIAL_BLAST);
       
                   GlacialBlast1Timer = 7200000; 
                   } else GlacialBlast1Timer -= diff;
                }

                 if (me->GetHealthPct() < 80.0f && Phase == 3)
                {
                  if (GlacialBlast2Timer <= diff) 
                   { 
                 if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                 DoCast(target, SPELL_GLACIAL_BLAST);
       
                   GlacialBlast2Timer = 7200000; 
                   } else GlacialBlast2Timer -= diff;
                }

                 if (me->GetHealthPct() < 70.0f && Phase == 3)
                {
                  if (GlacialBlast3Timer <= diff) 
                   { 
                 if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                 DoCast(target, SPELL_GLACIAL_BLAST);
       
                   GlacialBlast3Timer = 7200000; 
                   } else GlacialBlast3Timer -= diff;
                }

                 if (me->GetHealthPct() < 60.0f && Phase == 3)
                {
                  if (GlacialBlast4Timer <= diff) 
                   { 
                 if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                 DoCast(target, SPELL_GLACIAL_BLAST);
       
                   GlacialBlast4Timer = 7200000; 
                   } else GlacialBlast4Timer -= diff;
                }
                   DoMeleeAttackIfReady(); 
                 } 

};
};

class mob_frost : public CreatureScript 
{ 
public: 
                 mob_frost() : CreatureScript("mob_frost") { } 
       
                 CreatureAI* GetAI(Creature* creature) const 
                 { 
                 return new mob_frostAI (creature); 
                 } 
       
                 struct mob_frostAI : public ScriptedAI 
                 { 
                 mob_frostAI(Creature* creature) : ScriptedAI(creature) {} 

        uint32 FrostsBallTimer;

        void Reset() 
        {
        FrostsBallTimer = 1000;
        }

     void UpdateAI(uint32 diff)
                 { 

            if (!UpdateVictim())
                return;

                  if (FrostsBallTimer <= diff) 
                   { 
                   if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, true))
                     DoCast(target, SPELL_FROST_BALL); 
       
                   FrostsBallTimer = 7000; 
                   } else FrostsBallTimer -= diff; 

                   DoMeleeAttackIfReady(); 
                 } 

};
};

void AddSC_uforius_frost()
{
	new uforius_frost();
	new mob_frost();
}