//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: eli
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_eli_a_health("sk_eli_a_health", "0");
ConVar	sk_eli_e_health("sk_eli_e_health", "0");

class CNPC_EliCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_EliCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_eli_ally, CNPC_EliCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_EliCombat, DT_NPC_EliCombat)
END_SEND_TABLE()


void CNPC_EliCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SetModelName(AllocPooledString("models/eli.mdl"));

	CNPC_Citizen::Precache();
}

void CNPC_EliCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_eli_a_health.GetInt();
}

Class_T	CNPC_EliCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_EliCombatEnenmy : public CNPC_EliCombat
{
	DECLARE_CLASS(CNPC_EliCombatEnenmy, CNPC_EliCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_eli_enemy, CNPC_EliCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_EliCombatEnenmy, DT_NPC_EliCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_EliCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_EliCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_eli_e_health.GetInt();
}

