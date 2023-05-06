//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: mossman
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_mossman_a_health("sk_mossman_a_health", "0");
ConVar	sk_mossman_e_health("sk_mossman_e_health", "0");

class CNPC_MossmanCombat : public CNPC_Citizen
{
public:
	DECLARE_CLASS(CNPC_MossmanCombat, CNPC_Citizen);
	DECLARE_SERVERCLASS();

	virtual void Precache();
	void	Spawn(void);
	Class_T Classify(void);
	void SelectModel();
};


LINK_ENTITY_TO_CLASS(npc_mossman_ally, CNPC_MossmanCombat);

IMPLEMENT_SERVERCLASS_ST(CNPC_MossmanCombat, DT_NPC_MossmanCombat)
END_SEND_TABLE()


void CNPC_MossmanCombat::Precache(void)
{
	m_Type = CT_UNIQUE;

	SelectModel();

	CNPC_Citizen::Precache();
}

void CNPC_MossmanCombat::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_mossman_a_health.GetInt();
}

Class_T	CNPC_MossmanCombat::Classify(void)
{
	return	CLASS_PLAYER_ALLY;
}

void CNPC_MossmanCombat::SelectModel()
{
	//mossman has 2 models, one for default and 1 for episodic, so actually respect this value here
	if (CBaseEntity::GetModelName() == NULL_STRING)
		SetModelName(AllocPooledString("models/mossman.mdl"));	//If we got here it failed so just fallback on the HL2 model
	else
		SetModelName(CBaseEntity::GetModelName());
}

class CNPC_MossmanCombatEnenmy : public CNPC_MossmanCombat
{
	DECLARE_CLASS(CNPC_MossmanCombatEnenmy, CNPC_MossmanCombat);
	DECLARE_SERVERCLASS();

	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_mossman_enemy, CNPC_MossmanCombatEnenmy);

IMPLEMENT_SERVERCLASS_ST(CNPC_MossmanCombatEnenmy, DT_NPC_MossmanCombatEnenmy)
END_SEND_TABLE()

Class_T	CNPC_MossmanCombatEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

void CNPC_MossmanCombatEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn();

	m_iHealth = sk_mossman_e_health.GetInt();
}

