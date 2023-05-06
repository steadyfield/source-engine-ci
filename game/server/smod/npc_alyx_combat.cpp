//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: alyx
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_alyx_a_health("sk_alyx_a_health", "0");
ConVar	sk_alyx_e_health("sk_alyx_e_health", "0");

class CNPC_AlyxCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_AlyxCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_alyx_ally, CNPC_AlyxCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_AlyxCombat, DT_NPC_AlyxCombat)
END_SEND_TABLE()


void CNPC_AlyxCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SetModelName(AllocPooledString("models/alyx.mdl"));

	CNPC_Citizen::Precache();
}

void CNPC_AlyxCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_alyx_a_health.GetInt();
}

Class_T	CNPC_AlyxCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_AlyxCombatEnenmy : public CNPC_AlyxCombat
{
	DECLARE_CLASS(CNPC_AlyxCombatEnenmy, CNPC_AlyxCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_alyx_enemy, CNPC_AlyxCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_AlyxCombatEnenmy, DT_NPC_AlyxCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_AlyxCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_AlyxCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_alyx_e_health.GetInt();
}

