//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Crossbow
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "hl1mp_basecombatweapon_shared.h"
//#include "basecombatcharacter.h"
//#include "AI_BaseNPC.h"
#ifdef CLIENT_DLL
#include "c_baseplayer.h"
#else
#include "player.h"
#endif
//#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#ifdef CLIENT_DLL
#else
#include "soundent.h"
#include "entitylist.h"
#include "game.h"
#endif
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "IEffects.h"
#ifdef CLIENT_DLL
#include "c_te_effect_dispatch.h"
#else
#include "te_effect_dispatch.h"
#include "hl1mp_weapon_crossbow.h"
#endif

//-----------------------------------------------------------------------------
// CWeaponDartbow
//-----------------------------------------------------------------------------

#ifdef CLIENT_DLL
#define CWeaponDartbow C_WeaponDartbow
#endif

class CWeaponDartbow : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CWeaponDartbow, CBaseHL1MPCombatWeapon );
public:

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponDartbow( void );

	void	Precache( void );
	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	bool	Reload( void );
	void	WeaponIdle( void );
	bool	Deploy( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

//	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

#ifndef CLIENT_DLL
//	DECLARE_ACTTABLE();
#endif

private:
	void	FireBolt( void );
	void	ToggleZoom( void );

private:
//	bool	m_fInZoom;
	CNetworkVar( bool, m_fInZoom );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponDartbow, DT_WeaponDartbow );

BEGIN_NETWORK_TABLE( CWeaponDartbow, DT_WeaponDartbow )
#ifdef CLIENT_DLL
	RecvPropBool( RECVINFO( m_fInZoom ) ),
#else
	SendPropBool( SENDINFO( m_fInZoom ) ),
#endif
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponDartbow )
#ifdef CLIENT_DLL
	DEFINE_PRED_FIELD( m_fInZoom, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
#endif
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_dartbow, CWeaponDartbow );
PRECACHE_WEAPON_REGISTER( weapon_dartbow );

//IMPLEMENT_SERVERCLASS_ST( CWeaponDartbow, DT_WeaponDartbow )
//END_SEND_TABLE()

BEGIN_DATADESC( CWeaponDartbow )
END_DATADESC()

#if 0
#ifndef CLIENT_DLL

acttable_t	CWeaponDartbow::m_acttable[] = 
{
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_CROSSBOW,					false },
//	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_CROSSBOW,						false },
//	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_CROSSBOW,				false },
//	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_CROSSBOW,				false },
//	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW,	false },
//	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_CROSSBOW,			false },
//	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_CROSSBOW,					false },
};

IMPLEMENT_ACTTABLE(CWeaponDartbow);

#endif
#endif

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponDartbow::CWeaponDartbow( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= true;
	m_fInZoom			= false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponDartbow::Precache( void )
{
#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "crossbow_dart" );
#endif

	PrecacheScriptSound( "Weapon_HL1Crossbow.BoltHitBody" );
	PrecacheScriptSound( "Weapon_HL1Crossbow.BoltHitWorld" );

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponDartbow::PrimaryAttack( void )
{
	if ( m_fInZoom && g_pGameRules->IsMultiplayer() )
	{
//		FireSniperBolt();
		FireBolt();
	}
	else
	{
		FireBolt();
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponDartbow::SecondaryAttack( void )
{
	ToggleZoom();
	m_flNextSecondaryAttack = gpGlobals->curtime + 1.0;
}


void CWeaponDartbow::FireBolt( void )
{
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

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	Vector vecAiming	= pOwner->GetAutoaimVector( AUTOAIM_2DEGREES );	
	Vector vecSrc		= pOwner->Weapon_ShootPosition();

	QAngle angAiming;
	VectorAngles( vecAiming, angAiming );

#ifndef CLIENT_DLL
	CHL1CrossbowBolt *pBolt = CHL1CrossbowBolt::BoltCreate( vecSrc, angAiming, pOwner );

    // In multiplayer, secondary fire is instantaneous.
    if ( g_pGameRules->IsMultiplayer() && m_fInZoom )
    {
        Vector vecEnd = vecSrc + ( vecAiming * MAX_TRACE_LENGTH );
        
        trace_t trace;
        UTIL_TraceLine( vecSrc, vecEnd, MASK_SHOT, GetOwner(), COLLISION_GROUP_NONE, &trace );
        pBolt->SetAbsOrigin( trace.endpos );

        // We hit someone
        if ( trace.m_pEnt && trace.m_pEnt->m_takedamage )
        {
            pBolt->SetExplode( false );            
            pBolt->BoltTouch( trace.m_pEnt );
            return;
        }
    }
    else
    {
        if ( pOwner->GetWaterLevel() == 3 )
        {
            pBolt->SetAbsVelocity( vecAiming * BOLT_WATER_VELOCITY );
        }
        else
        {
            pBolt->SetAbsVelocity( vecAiming * BOLT_AIR_VELOCITY );
        }
    }

	pBolt->SetLocalAngularVelocity( QAngle( 0, 0, 10 ) );

    if ( m_fInZoom )
    {
        pBolt->SetExplode( false );
        
    }
#endif

	m_iClip1--;

	pOwner->ViewPunch( QAngle( -2, 0, 0 ) );

	WeaponSound( SINGLE );
	WeaponSound( RELOAD );

#ifdef CLIENT_DLL
#else
	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 200, 0.2 );
#endif

	if ( m_iClip1 > 0 )
	{
		SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	}
	else if ( pOwner->GetAmmoCount(m_iPrimaryAmmoType) > 0 )
	{
		SendWeaponAnim( ACT_VM_SECONDARYATTACK );
	}

	if ( !m_iClip1 && pOwner->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		// HEV suit - indicate out of ammo condition
		pOwner->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	m_flNextPrimaryAttack	= gpGlobals->curtime + 0.75;
	m_flNextSecondaryAttack	= gpGlobals->curtime + 0.75;

	if ( m_iClip1 > 0 )
	{
		SetWeaponIdleTime( gpGlobals->curtime + 5.0 );
	}
	else
	{
		SetWeaponIdleTime( gpGlobals->curtime + 0.75 );
	}
}


bool CWeaponDartbow::Reload( void )
{
	bool fRet;

	fRet = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD );
	if ( fRet )
	{
		if ( m_fInZoom )
		{
			ToggleZoom();
		}

		WeaponSound( RELOAD );
	}

	return fRet;
}


void CWeaponDartbow::WeaponIdle( void )
{
	//SMOD: Ironsight fix
	if (m_bIsIronsighted)
		return;

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( pPlayer )
	{
		pPlayer->GetAutoaimVector( AUTOAIM_2DEGREES );
	}

	if ( !HasWeaponIdleTimeElapsed() )
		return;

	int iAnim;
	float flRand = random->RandomFloat( 0, 1 );
	if ( flRand <= 0.75 )
	{
		if ( m_iClip1 <= 0 )
			iAnim = ACT_CROSSBOW_IDLE_UNLOADED;
		else
			iAnim = ACT_VM_IDLE;
	}
	else
	{
		if ( m_iClip1 <= 0 )
			iAnim = ACT_CROSSBOW_FIDGET_UNLOADED;
		else
			iAnim = ACT_VM_FIDGET;
	}

	SendWeaponAnim( iAnim );
}


bool CWeaponDartbow::Deploy( void )
{
	if ( m_iClip1 <= 0 )
	{
		return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), ACT_CROSSBOW_DRAW_UNLOADED, (char*)GetAnimPrefix() );
	}

	return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), ACT_VM_DRAW, (char*)GetAnimPrefix() );
}


bool CWeaponDartbow::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	if ( m_fInZoom )
	{
		ToggleZoom();
	}

	return BaseClass::Holster( pSwitchingTo );
}


void CWeaponDartbow::ToggleZoom( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( !pPlayer )
	{
		return;
	}

#if !defined(CLIENT_DLL)
	if ( m_fInZoom )
	{
		if ( pPlayer->SetFOV( this, 0 ) )
		{
			m_fInZoom = false;
		}
	}
	else
	{
		if ( pPlayer->SetFOV( this, 20 ) )
		{
			m_fInZoom = true;
		}
	}
#endif
}
