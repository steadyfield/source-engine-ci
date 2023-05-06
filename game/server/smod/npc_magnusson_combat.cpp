//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: magnusson
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_magnusson_a_health("sk_magnusson_a_health", "0");
ConVar	sk_magnusson_e_health("sk_magnusson_e_health", "0");

class CNPC_MagnussonCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_MagnussonCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_magnusson_ally, CNPC_MagnussonCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_MagnussonCombat, DT_NPC_MagnussonCombat)
END_SEND_TABLE()


void CNPC_MagnussonCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SetModelName(AllocPooledString("models/magnusson.mdl"));

	CNPC_Citizen::Precache();
}

void CNPC_MagnussonCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_magnusson_a_health.GetInt();
}

Class_T	CNPC_MagnussonCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_MagnussonCombatEnenmy : public CNPC_MagnussonCombat
{
	DECLARE_CLASS(CNPC_MagnussonCombatEnenmy, CNPC_MagnussonCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_magnusson_enemy, CNPC_MagnussonCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_MagnussonCombatEnenmy, DT_NPC_MagnussonCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_MagnussonCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_MagnussonCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_magnusson_e_health.GetInt();
}

