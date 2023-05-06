//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: breen
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_breen_a_health("sk_breen_a_health", "0");
ConVar	sk_breen_e_health("sk_breen_e_health", "0");

class CNPC_BreenCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_BreenCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_breen_ally, CNPC_BreenCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_BreenCombat, DT_NPC_BreenCombat)
END_SEND_TABLE()


void CNPC_BreenCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SetModelName(AllocPooledString("models/breen.mdl"));

	CNPC_Citizen::Precache();
}

void CNPC_BreenCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_breen_a_health.GetInt();
}

Class_T	CNPC_BreenCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_BreenCombatEnenmy : public CNPC_BreenCombat
{
	DECLARE_CLASS(CNPC_BreenCombatEnenmy, CNPC_BreenCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_breen_enemy, CNPC_BreenCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_BreenCombatEnenmy, DT_NPC_BreenCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_BreenCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_BreenCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_breen_e_health.GetInt();
}

