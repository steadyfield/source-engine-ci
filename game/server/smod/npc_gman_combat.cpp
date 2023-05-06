//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: gman
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_gman_a_health("sk_gman_a_health", "0");
ConVar	sk_gman_e_health("sk_gman_e_health", "0");

class CNPC_GManCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_GManCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_gman_ally, CNPC_GManCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_GManCombat, DT_NPC_GManCombat)
END_SEND_TABLE()


void CNPC_GManCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SetModelName(AllocPooledString("models/gman.mdl"));

	CNPC_Citizen::Precache();
}

void CNPC_GManCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_gman_a_health.GetInt();
}

Class_T	CNPC_GManCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_GManCombatEnenmy : public CNPC_GManCombat
{
	DECLARE_CLASS(CNPC_GManCombatEnenmy, CNPC_GManCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_gman_enemy, CNPC_GManCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_GManCombatEnenmy, DT_NPC_GManCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_GManCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_GManCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_gman_e_health.GetInt();
}

