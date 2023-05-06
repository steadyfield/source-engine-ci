//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: kleiner
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_kleiner_a_health("sk_kleiner_a_health", "0");
ConVar	sk_kleiner_e_health("sk_kleiner_e_health", "0");

class CNPC_KleinerCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_KleinerCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_kleiner_ally, CNPC_KleinerCombat);
LINK_ENTITY_TO_CLASS(npc_f_kleiner, CNPC_KleinerCombat); //Original Smod ClassName

IMPLEMENT_SERVERCLASS_ST(CNPC_KleinerCombat, DT_NPC_KleinerCombat)
END_SEND_TABLE()


void CNPC_KleinerCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SetModelName(AllocPooledString("models/kleiner.mdl"));

	CNPC_Citizen::Precache();
}

void CNPC_KleinerCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_kleiner_a_health.GetInt();
}

Class_T	CNPC_KleinerCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_KleinerCombatEnenmy : public CNPC_KleinerCombat
{
	DECLARE_CLASS(CNPC_KleinerCombatEnenmy, CNPC_KleinerCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_kleiner_enemy, CNPC_KleinerCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_KleinerCombatEnenmy, DT_NPC_KleinerCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_KleinerCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_KleinerCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_kleiner_e_health.GetInt();
}

