//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Hornetgun
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "NPCEvent.h"
#include "weapon_hornetgun.h"
//#include "basecombatcharacter.h"
//#include "AI_BaseNPC.h"
#include "gamerules.h"
#include "in_buttons.h"
#ifdef CLIENT_DLL
#include "c_basehlplayer.h"
#else
#include "hl2_player.h"
#include "soundent.h"
#include "game.h"
#endif
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#if !defined(CLIENT_DLL)
#include "hl1_npc_hornet.h"
#endif


IMPLEMENT_NETWORKCLASS_ALIASED( WeaponHgunHD, DT_WeaponHgunHD );

BEGIN_NETWORK_TABLE( CWeaponHgunHD, DT_WeaponHgunHD )
#ifdef CLIENT_DLL
	RecvPropFloat( RECVINFO( m_flRechargeTime ) ),
	RecvPropInt( RECVINFO( m_iFirePhase ) ),
#else
	SendPropFloat( SENDINFO( m_flRechargeTime ) ),
	SendPropInt( SENDINFO( m_iFirePhase ) ),
#endif
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponHgunHD )
#ifdef CLIENT_DLL
	DEFINE_PRED_FIELD( m_flRechargeTime, FIELD_FLOAT, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_iFirePhase, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),
#endif
END_PREDICTION_DATA()

//LINK_ENTITY_TO_CLASS( weapon_hornetgun, CWeaponHgunHD );
//PRECACHE_WEAPON_REGISTER( weapon_hornetgun);

//IMPLEMENT_SERVERCLASS_ST( CWeaponHgunHD, DT_WeaponHgunHD )
//END_SEND_TABLE()

BEGIN_DATADESC( CWeaponHgunHD )
	DEFINE_FIELD( m_flRechargeTime,	FIELD_TIME ),
	DEFINE_FIELD( m_iFirePhase,		FIELD_INTEGER ),
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponHgunHD::CWeaponHgunHD( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= true;

	m_flRechargeTime = 0.0;
	m_iFirePhase = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponHgunHD::Precache( void )
{
#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "hornet" );
#endif

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponHgunHD::PrimaryAttack( void )
{
#ifdef CLIENT_DLL
	C_BaseHLPlayer *pPlayer;
#else
	CHL2_Player *pPlayer;
#endif

	pPlayer = ToHL2Player( GetOwner() );

	if ( !pPlayer )
	{
		return;
	}

	if ( pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		return;
	}

	WeaponSound( SINGLE );
#if !defined(CLIENT_DLL)
	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 200, 0.2 );
#endif
	pPlayer->DoMuzzleFlash();

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector	vForward, vRight, vUp;
	QAngle	vecAngles;

	pPlayer->EyeVectors( &vForward, &vRight, &vUp );
	VectorAngles( vForward, vecAngles );

#if !defined(CLIENT_DLL)
	CBaseEntity *pHornet = CBaseEntity::Create( "hornet", pPlayer->Weapon_ShootPosition() + vForward * 16 + vRight * 8 + vUp * -12, vecAngles, pPlayer );
	pHornet->SetAbsVelocity( vForward * 300 );
#endif

	m_flRechargeTime = gpGlobals->curtime + 0.5;
	
	pPlayer->RemoveAmmo( 1, m_iPrimaryAmmoType );

	pPlayer->ViewPunch( QAngle( -2, 0, 0 ) );

	m_flNextPrimaryAttack = m_flNextPrimaryAttack + 0.25;

	if (m_flNextPrimaryAttack < gpGlobals->curtime )
	{
		m_flNextPrimaryAttack = gpGlobals->curtime + 0.25;
	}

	SetWeaponIdleTime( random->RandomFloat( 10, 15 ) );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponHgunHD::SecondaryAttack( void )
{
#ifdef CLIENT_DLL
	C_BaseHLPlayer *pPlayer;
#else
	CHL2_Player *pPlayer;
#endif

	pPlayer = ToHL2Player( GetOwner() );

	if ( !pPlayer )
	{
		return;
	}

	if ( pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		return;
	}

	WeaponSound( SINGLE );
#if !defined(CLIENT_DLL)
	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 200, 0.2 );
#endif
	pPlayer->DoMuzzleFlash();

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	CBaseEntity *pHornet;
	Vector vecSrc;

	Vector	vForward, vRight, vUp;
	QAngle	vecAngles;

	pPlayer->EyeVectors( &vForward, &vRight, &vUp );
	VectorAngles( vForward, vecAngles );

	vecSrc = pPlayer->Weapon_ShootPosition() + vForward * 16 + vRight * 8 + vUp * -12;

	m_iFirePhase++;
	switch ( m_iFirePhase )
	{
	case 1:
		vecSrc = vecSrc + vUp * 8;
		break;
	case 2:
		vecSrc = vecSrc + vUp * 8;
		vecSrc = vecSrc + vRight * 8;
		break;
	case 3:
		vecSrc = vecSrc + vRight * 8;
		break;
	case 4:
		vecSrc = vecSrc + vUp * -8;
		vecSrc = vecSrc + vRight * 8;
		break;
	case 5:
		vecSrc = vecSrc + vUp * -8;
		break;
	case 6:
		vecSrc = vecSrc + vUp * -8;
		vecSrc = vecSrc + vRight * -8;
		break;
	case 7:
		vecSrc = vecSrc + vRight * -8;
		break;
	case 8:
		vecSrc = vecSrc + vUp * 8;
		vecSrc = vecSrc + vRight * -8;
		m_iFirePhase = 0;
		break;
	}

#ifdef CLIENT_DLL
	pHornet = NULL;
#else
	pHornet = CBaseEntity::Create( "hornet", vecSrc, vecAngles, pPlayer );
	pHornet->SetAbsVelocity( vForward * 1200 );
	pHornet->SetThink( &CNPC_Hornet::StartDart );
#endif

	m_flRechargeTime = gpGlobals->curtime + 0.5;

	pPlayer->ViewPunch( QAngle( -2, 0, 0 ) );
	pPlayer->RemoveAmmo( 1, m_iPrimaryAmmoType );

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->curtime + 0.1;
	SetWeaponIdleTime( random->RandomFloat( 10, 15 ) );
}

void CWeaponHgunHD::WeaponIdle( void )
{
	//SMOD: Ironsight fix
	if (m_bIsIronsighted)
		return;

	if ( !HasWeaponIdleTimeElapsed() )
		return;

	int iAnim;
	float flRand = random->RandomFloat( 0, 1 );
	if ( flRand <= 0.75 )
	{
		iAnim = ACT_VM_IDLE;
	}
	else
	{
		iAnim = ACT_VM_FIDGET;
	}

	SendWeaponAnim( iAnim );
}

bool CWeaponHgunHD::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	bool bRet;

	bRet = BaseClass::Holster( pSwitchingTo );

	if ( bRet )
	{
#ifdef CLIENT_DLL
		C_BaseHLPlayer *pPlayer;
#else
		CHL2_Player *pPlayer;
#endif

		pPlayer = ToHL2Player( GetOwner() );

		if ( pPlayer )
		{
#if !defined(CLIENT_DLL)            
			//!!!HACKHACK - can't select hornetgun if it's empty! no way to get ammo for it, either.
            int iCount = pPlayer->GetAmmoCount( m_iPrimaryAmmoType );
            if ( iCount <= 0 )
            {
                pPlayer->GiveAmmo( iCount+1, m_iPrimaryAmmoType, true );
            }
#endif            
		}
	}

	return bRet;
}

bool CWeaponHgunHD::Reload( void )
{
	if ( m_flRechargeTime >= gpGlobals->curtime )
	{
		return true;
	}
	
#ifdef CLIENT_DLL
	C_BaseHLPlayer *pPlayer;
#else
	CHL2_Player *pPlayer;
#endif

	pPlayer = ToHL2Player( GetOwner() );

	if ( !pPlayer )
	{
		return true;
	}

#ifdef CLIENT_DLL
#else
	if ( !g_pGameRules->CanHaveAmmo( pPlayer, m_iPrimaryAmmoType ) )
		return true;

	while ( ( m_flRechargeTime < gpGlobals->curtime ) && g_pGameRules->CanHaveAmmo( pPlayer, m_iPrimaryAmmoType ) )
	{
		pPlayer->GiveAmmo( 1, m_iPrimaryAmmoType, true );
		m_flRechargeTime += 0.5;
	}
#endif

	return true;
}

void CWeaponHgunHD::ItemPostFrame( void )
{
	Reload();

	BaseClass::ItemPostFrame();
}
