//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: barney
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_barney_a_health("sk_barney_a_health", "0");
ConVar	sk_barney_e_health("sk_barney_e_health", "0");

class CNPC_BarneyCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_BarneyCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_barney_ally, CNPC_BarneyCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_BarneyCombat, DT_NPC_BarneyCombat)
END_SEND_TABLE()


void CNPC_BarneyCombat::Precache(void)
{
	m_Type = CT_UNIQUE;
	
	//barney has 2 models, one for default and 1 for episodic, so actually respect this value here
	if (CBaseEntity::GetModelName() == NULL_STRING)
		SetModelName(AllocPooledString("models/barney.mdl"));	//If we got here it failed so just fallback on the HL2 model
	else
		SetModelName(CBaseEntity::GetModelName());

	CNPC_Citizen::Precache();
}

void CNPC_BarneyCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_barney_a_health.GetInt();
}

Class_T	CNPC_BarneyCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_BarneyCombatEnenmy : public CNPC_BarneyCombat
{
	DECLARE_CLASS(CNPC_BarneyCombatEnenmy, CNPC_BarneyCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_barney_enemy, CNPC_BarneyCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_BarneyCombatEnenmy, DT_NPC_BarneyCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_BarneyCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_BarneyCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_barney_e_health.GetInt();
}

