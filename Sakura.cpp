/*
# 	Script Sakura
# 	Scriped Спай
#	Status: work 100%
#
#
*/

#include"ScriptMgr.h"
#include"ScriptedCreature.h"
#include"ScriptedGossip.h"

enum Custom
{
	MODEL_ID_SAKYRA = 28989,
};

enum Spells
{
	SPELL_SAKYRA_PEACE = 62579,
};

class npc_sakyra : public CreatureScript
{
public :
		npc_sakyra( ) : CreatureScript("npc_sakyra") { }
		
		struct npc_sakyraAI : public ScriptedAI
		{
			npc_sakyraAI(Creature* creature) : ScriptedAI(creature)
			{
				SetCombatMovement(false);
			}
			
			void Reset()
			{
				DoCast(me, SPELL_SAKYRA_PEACE, true);
				
				me->SetObjectScale(1.5f);
				me->SetDisplayId(MODEL_ID_SAKYRA);
				me->SetUInt32Value(UNIT_FIELD_BYTES_2, UNIT_FIELD_BASEATTACKTIME);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			}
		};
		
		CreatureAI* GetAI(Creature* creature) const
		{
			return new npc_sakyraAI(creature);
		}
};

void AddSC_npc_sakyra()
{
		new npc_sakyra();
}