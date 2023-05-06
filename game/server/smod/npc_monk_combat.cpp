//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: monk
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_monk_a_health("sk_monk_a_health", "0");
ConVar	sk_monk_e_health("sk_monk_e_health", "0");

class CNPC_MonkCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_MonkCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_monk_ally, CNPC_MonkCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_MonkCombat, DT_NPC_MonkCombat)
END_SEND_TABLE()


void CNPC_MonkCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SetModelName(AllocPooledString("models/monk.mdl"));

	CNPC_Citizen::Precache();
}

void CNPC_MonkCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_monk_a_health.GetInt();
}

Class_T	CNPC_MonkCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_MonkCombatEnenmy : public CNPC_MonkCombat
{
	DECLARE_CLASS(CNPC_MonkCombatEnenmy, CNPC_MonkCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_monk_enemy, CNPC_MonkCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_MonkCombatEnenmy, DT_NPC_MonkCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_MonkCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_MonkCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_monk_e_health.GetInt();
}

