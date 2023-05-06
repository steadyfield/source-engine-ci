//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Manhack Wpn
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "NPCEvent.h"
#include "hl1_basecombatweapon_shared.h"
#include "basecombatcharacter.h"
#include "AI_BaseNPC.h"
#include "hl2_player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "hl1_items.h"
#include "npc_manhack.h"
#include "beam_shared.h"
#include "prop_combine_ball.h"


//-----------------------------------------------------------------------------
// CWeaponManhack
//-----------------------------------------------------------------------------
class CWeaponManhack : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponManhack, CBaseHLCombatWeapon );
public:

	CWeaponManhack( void );

	void	Precache( void );
	void	PrimaryAttack( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	void	ItemPostFrame();
	void	DelayedAttack();
	void	WeaponIdle();

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

protected:
	bool					m_bJustThrown;
	float					m_flDelayedFire;
	bool					m_bShotDelayed;
	float					m_flThrownTimer;
};

LINK_ENTITY_TO_CLASS( weapon_manhack, CWeaponManhack );

PRECACHE_WEAPON_REGISTER( weapon_manhack );

IMPLEMENT_SERVERCLASS_ST( CWeaponManhack, DT_WeaponManhack )
END_SEND_TABLE()

BEGIN_DATADESC( CWeaponManhack )
	DEFINE_FIELD( m_bJustThrown, FIELD_BOOLEAN ),
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponManhack::CWeaponManhack( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;
	m_bJustThrown		= false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponManhack::Precache( void )
{
	BaseClass::Precache();

	UTIL_PrecacheOther("npc_manhack_ally");
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponManhack::PrimaryAttack( void )
{
	if ( m_bShotDelayed )
		return;

	m_bShotDelayed = true;
	m_flNextPrimaryAttack = m_flNextSecondaryAttack = m_flDelayedFire = gpGlobals->curtime + 0.5f;

	SendWeaponAnim( ACT_VM_THROW );
}

bool CWeaponManhack::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( !pPlayer )
	{
		return false;
	}

	if ( !BaseClass::Holster( pSwitchingTo ) )
	{
		return false;
	}

	if ( pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		SetThink( &CWeaponManhack::DestroyItem );
		SetNextThink( gpGlobals->curtime + 0.1 );
	}

	pPlayer->SetNextAttack( gpGlobals->curtime + 0.5 );

	return true;
}

void CWeaponManhack::WeaponIdle()
{
	if (m_bJustThrown && m_flThrownTimer == gpGlobals->curtime)
	{
		SendWeaponAnim(ACT_VM_DRAW);
		SetWeaponIdleTime(SequenceDuration());
	}

	BaseClass::WeaponIdle();
}

void CWeaponManhack::ItemPostFrame(void)
{
	// See if we need to fire off our secondary round
	if (m_bShotDelayed && gpGlobals->curtime > m_flDelayedFire)
	{
		DelayedAttack();
	}

	BaseClass::ItemPostFrame();
}

//=========================================================
void CWeaponManhack::DelayedAttack(void)
{
	m_bShotDelayed = false;
	
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());
	
	if ( pOwner == NULL )
		return;

	// Deplete the clip completely
	m_flNextSecondaryAttack = pOwner->m_flNextAttack = gpGlobals->curtime + SequenceDuration();

	Vector vecForward;
	pOwner->EyeVectors(&vecForward);

	// find place to toss monster
	// Does this need to consider a crouched player?
	Vector vecStart = pOwner->WorldSpaceCenter() + (vecForward * 20);
	Vector vecEnd	= vecStart + (vecForward * 44);
	trace_t tr;
	UTIL_TraceLine( vecStart, vecEnd, MASK_SOLID, this, COLLISION_GROUP_NONE, &tr );
	if ( tr.allsolid || tr.startsolid || tr.fraction <= 0.25 )
		return;

	Vector vecSrc = pOwner->Weapon_ShootPosition();

	// player "shoot" animation
	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pOwner->SetAnimation(PLAYER_ATTACK1);

	CNPC_Manhack *pSnark = (CNPC_Manhack*)Create("npc_manhack_ally", vecSrc, pOwner->EyeAngles(), GetOwner());
	if ( pSnark )
	{
		pSnark->SetAbsVelocity(vecForward * 200 + pOwner->GetAbsVelocity());
	}

	// Decrease ammo
	pOwner->RemoveAmmo( 1, m_iPrimaryAmmoType );

	// Can shoot again immediately
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;

	// Can blow up after a short delay (so have time to release mouse button)
	m_flNextSecondaryAttack = gpGlobals->curtime + 1.0f;

	m_bJustThrown = true;
	m_flThrownTimer = gpGlobals->curtime + 2.5f; //Set until the animation can play
}
