//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: gordon
//
//=============================================================================//

#include "cbase.h"
#include "npc_citizen17.h"

ConVar	sk_gordon_a_health("sk_gordon_a_health", "0");
ConVar	sk_gordon_e_health("sk_gordon_e_health", "0");

class CNPC_Gordon : public CNPC_Citizen
{
public:
	DECLARE_CLASS( CNPC_Gordon, CNPC_Citizen );
	DECLARE_SERVERCLASS();

	virtual void Precache();

	void	Spawn( void );
	void	SelectModel();
	Class_T Classify( void );

	void DeathSound( const CTakeDamageInfo &info );
};


LINK_ENTITY_TO_CLASS( npc_gordon_ally, CNPC_Gordon );

IMPLEMENT_SERVERCLASS_ST(CNPC_Gordon, DT_NPC_Gordon)
END_SEND_TABLE()

void CNPC_Gordon::Precache()
{
	m_Type = CT_UNIQUE;

	SelectModel();

	CNPC_Citizen::Precache();

	PrecacheScriptSound("SMODPlayer.VoiceDeath");
}

void CNPC_Gordon::Spawn( void )
{
	Precache();

	CNPC_Citizen::Spawn();

	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION);

	m_iHealth = sk_gordon_a_health.GetInt();
}

Class_T	CNPC_Gordon::Classify( void )
{
	return	CLASS_PLAYER_ALLY;
}

void CNPC_Gordon::SelectModel()
{
	//gordon has a citizen and hev versions
	if (CBaseEntity::GetModelName() == NULL_STRING)
		SetModelName(AllocPooledString("models/humans/gordon.mdl"));	//If we got here it failed so just fallback on the hev model
	else
		SetModelName(CBaseEntity::GetModelName());
}

void CNPC_Gordon::DeathSound( const CTakeDamageInfo &info )
{
	// Sentences don't play on dead NPCs
	SentenceStop();

	EmitSound( "SMODPlayer.VoiceDeath" );

}

class CNPC_GordonEnenmy : public CNPC_Gordon
{
	DECLARE_CLASS(CNPC_GordonEnenmy, CNPC_Gordon);
	Class_T Classify(void);
	void Spawn();
};

LINK_ENTITY_TO_CLASS(npc_gordon_enemy, CNPC_GordonEnenmy);

//-----------------------------------------------------------------------------
// Purpose: 
// Output : 
//-----------------------------------------------------------------------------
Class_T	CNPC_GordonEnenmy::Classify(void)
{
	return	CLASS_COMBINE;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_GordonEnenmy::Spawn(void)
{
	Precache();

	CNPC_Citizen::Spawn(); 

	m_iHealth = sk_gordon_e_health.GetInt();
}
