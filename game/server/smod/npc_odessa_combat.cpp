//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: odessa
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_odessa_a_health("sk_odessa_a_health", "0");
ConVar	sk_odessa_e_health("sk_odessa_e_health", "0");

class CNPC_OdessaCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_OdessaCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
};


LINK_ENTITY_TO_CLASS(npc_odessa_ally, CNPC_OdessaCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_OdessaCombat, DT_NPC_OdessaCombat)
END_SEND_TABLE()


void CNPC_OdessaCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SetModelName(AllocPooledString("models/odessa.mdl"));

	CNPC_Citizen::Precache();
}

void CNPC_OdessaCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_odessa_a_health.GetInt();
}

Class_T	CNPC_OdessaCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}


class CNPC_OdessaCombatEnenmy : public CNPC_OdessaCombat
{
	DECLARE_CLASS(CNPC_OdessaCombatEnenmy, CNPC_OdessaCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_odessa_enemy, CNPC_OdessaCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_OdessaCombatEnenmy, DT_NPC_OdessaCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_OdessaCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_OdessaCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_odessa_e_health.GetInt();
}

