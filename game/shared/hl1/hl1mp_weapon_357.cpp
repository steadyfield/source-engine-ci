//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		357 - hand gun
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "weapon_hl2mpbase.h"
#ifdef CLIENT_DLL
#include "c_baseplayer.h"
#else
#include "player.h"
#endif

#include "gamerules.h"
#include "in_buttons.h"
#ifndef CLIENT_DLL
#include "soundent.h"
#include "game.h"
#endif
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"

#ifdef CLIENT_DLL
#define CHL1MPWeapon357 C_HL1MPWeapon357
#endif

//-----------------------------------------------------------------------------
// CHL1MPWeapon357
//-----------------------------------------------------------------------------

class CHL1MPWeapon357 : public CWeaponHL2MPBase
{
	DECLARE_CLASS( CHL1MPWeapon357, CWeaponHL2MPBase );
public:
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CHL1MPWeapon357( void );

	void	Precache( void );
	bool	Deploy( void );
	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	bool	Reload( void );
	void	WeaponIdle( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

//	DECLARE_SERVERCLASS();
//	DECLARE_DATADESC();

private:
	void	ToggleZoom( void );

private:
	CNetworkVar( float, m_fInZoom );
};

IMPLEMENT_NETWORKCLASS_ALIASED( HL1MPWeapon357, DT_HL1MPWeapon357 );

BEGIN_NETWORK_TABLE( CHL1MPWeapon357, DT_HL1MPWeapon357 )
#ifdef CLIENT_DLL
	RecvPropFloat( RECVINFO( m_fInZoom ) ),
#else
	SendPropFloat( SENDINFO( m_fInZoom ) ),
#endif
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CHL1MPWeapon357 )
#ifdef CLIENT_DLL
	DEFINE_PRED_FIELD( m_fInZoom, FIELD_FLOAT, FTYPEDESC_INSENDTABLE ),
#endif
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_hl1mp_357, CHL1MPWeapon357 );

PRECACHE_WEAPON_REGISTER( weapon_hl1mp_357 );

//IMPLEMENT_SERVERCLASS_ST( CHL1MPWeapon357, DT_Weapon357 )
//END_SEND_TABLE()

//BEGIN_DATADESC( CHL1MPWeapon357 )
//END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHL1MPWeapon357::CHL1MPWeapon357( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;
	m_fInZoom			= false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHL1MPWeapon357::Precache( void )
{
	BaseClass::Precache();

#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "ammo_357" );
#endif
}

bool CHL1MPWeapon357::Deploy( void )
{
// Bodygroup stuff not currently working correctly
//	if ( g_pGameRules->IsMultiplayer() )
//	{
		// enable laser sight geometry.
//		SetBodygroup( 4, 1 );
//	}
//	else
//	{
//		SetBodygroup( 4, 0 );
//	}

	return BaseClass::Deploy();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CHL1MPWeapon357::PrimaryAttack( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( !pPlayer )
	{
		return;
	}

	if ( m_iClip1 <= 0 )
	{
		if ( !m_bFireOnEmpty )
		{
			Reload();
		}
		else
		{
			WeaponSound( EMPTY );
			m_flNextPrimaryAttack = 0.15;
		}

		return;
	}

	WeaponSound( SINGLE );
	pPlayer->DoMuzzleFlash();

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.75;
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;

	m_iClip1--;

	Vector vecSrc		= pPlayer->Weapon_ShootPosition();
	Vector vecAiming	= pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );	

//	pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0 );

	FireBulletsInfo_t info( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, MAX_TRACE_LENGTH, m_iPrimaryAmmoType );
	info.m_pAttacker = pPlayer;

	pPlayer->FireBullets( info );

#ifndef CLIENT_DLL
	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );
#endif

	pPlayer->ViewPunch( QAngle( -10, 0, 0 ) );

#ifndef CLIENT_DLL
	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 600, 0.2 );
#endif

	if ( !m_iClip1 && pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate( "!HEV_AMO0", FALSE, 0 ); 
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CHL1MPWeapon357::SecondaryAttack( void )
{
	// only in multiplayer
	if ( !g_pGameRules->IsMultiplayer() )
	{
#ifndef CLIENT_DLL
		// unless we have cheats on
		if ( !sv_cheats->GetBool() )
		{
			return;
		}
#endif
	}

	ToggleZoom();

	m_flNextSecondaryAttack = gpGlobals->curtime + 0.5;
}


bool CHL1MPWeapon357::Reload( void )
{
	bool fRet;

	fRet = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD );
	if ( fRet )
	{
		if ( m_fInZoom )
		{
			ToggleZoom();
		}
	}

	return fRet;
}


void CHL1MPWeapon357::WeaponIdle( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( pPlayer )
	{
		pPlayer->GetAutoaimVector( AUTOAIM_10DEGREES );
	}

	if ( !HasWeaponIdleTimeElapsed() )
		return;

	if ( random->RandomFloat( 0, 1 ) <= 0.9 )
	{
		SendWeaponAnim( ACT_VM_IDLE );
	}
	else
	{
		SendWeaponAnim( ACT_VM_FIDGET );
	}
}


bool CHL1MPWeapon357::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	if ( m_fInZoom )
	{
		ToggleZoom();
	}

	return BaseClass::Holster( pSwitchingTo );
}


void CHL1MPWeapon357::ToggleZoom( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( !pPlayer )
	{
		return;
	}

#ifndef CLIENT_DLL
	if ( m_fInZoom )
	{
		if ( pPlayer->SetFOV( this, 0 ) )
		{
			m_fInZoom = false;
			pPlayer->ShowViewModel( true );
		}
	}
	else
	{
		if ( pPlayer->SetFOV( this, 40 ) )
		{
			m_fInZoom = true;
			pPlayer->ShowViewModel( false );
		}
	}
#endif
}
