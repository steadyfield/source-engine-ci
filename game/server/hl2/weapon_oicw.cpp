//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "basecombatweapon.h"
#include "npcevent.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "hl2_player.h"
#include "weapon_oicw.h"
#include "gamerules.h"
#include "game.h"
#include "in_buttons.h"
#include "ai_memory.h"
#include "shake.h"
#include "soundent.h"
#include "weapon_oicw.h"
#include "grenade_ar2.h"

extern ConVar sk_plr_dmg_smg1_grenade;

ConVar ar2_switch_mode_sounds("ar2_switch_mode_sounds", "1", FCVAR_ARCHIVE, "Switches AR2 mode sounds"); 

#define AR2_ZOOM_RATE	0.5f	// Interval between zoom levels in seconds.

//=========================================================
//=========================================================

BEGIN_DATADESC( CWeaponOICW )

	DEFINE_FIELD( m_nShotsFired,	FIELD_INTEGER ),
	DEFINE_FIELD( m_bZoomed,		FIELD_BOOLEAN ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CWeaponOICW, DT_WeaponOICW)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_oicw, CWeaponOICW );
PRECACHE_WEAPON_REGISTER(weapon_oicw);

acttable_t	CWeaponOICW::m_acttable[] = 
{
	{ ACT_RANGE_ATTACK1,	ACT_RANGE_ATTACK_AR2, true },
	{ ACT_WALK,				ACT_WALK_RIFLE,					false },
	{ ACT_WALK_AIM,			ACT_WALK_AIM_RIFLE,				false },
	{ ACT_WALK_CROUCH,		ACT_WALK_CROUCH_RIFLE,			false },
	{ ACT_WALK_CROUCH_AIM,	ACT_WALK_CROUCH_AIM_RIFLE,		false },
	{ ACT_RUN,				ACT_RUN_RIFLE,					false },
	{ ACT_RUN_AIM,			ACT_RUN_AIM_RIFLE,				false },
	{ ACT_RUN_CROUCH,		ACT_RUN_CROUCH_RIFLE,			false },
	{ ACT_RUN_CROUCH_AIM,	ACT_RUN_CROUCH_AIM_RIFLE,		false },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_AR2,	false },
//	{ ACT_RANGE_ATTACK2, ACT_RANGE_ATTACK_AR2_GRENADE, true },
};

IMPLEMENT_ACTTABLE(CWeaponOICW);

CWeaponOICW::CWeaponOICW( )
{
	m_fMinRange1	= 65;
	m_fMaxRange1	= 2048;

	m_fMinRange2	= 256;
	m_fMaxRange2	= 1024;

	m_nShotsFired	= 0;

	m_bUseGrenade = false;
}

void CWeaponOICW::Precache( void )
{
	UTIL_PrecacheOther("grenade_ar2");
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: Offset the autoreload
//-----------------------------------------------------------------------------
bool CWeaponOICW::Deploy( void )
{
	m_nShotsFired = 0;
	
//	SetThink( ScreenTextThink );
//	SetNextThink( gpGlobals->curtime + 0.1f );

	SendGrenageUsageState();

	return BaseClass::Deploy();
}

//-----------------------------------------------------------------------------
// Purpose: Handle grenade detonate in-air (even when no ammo is left)
//-----------------------------------------------------------------------------
void CWeaponOICW::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if (!pOwner)
		return;

	if ( ( pOwner->m_nButtons & IN_ATTACK ) == false )
	{
		m_nShotsFired = 0;
	}

	
//	if ( !m_bZoomed )
//	{
//		NDebugOverlay::ScreenText( 0.85, 0.9, (m_bUseGrenade ? "Grenade" : "Zoom"), 255, 127, 0, 255, 0.0 ); // VXP: Moved to Think
//	}
	
	//Zoom in
	if ( (pOwner->m_afButtonPressed & IN_ATTACK2) )
	{
		if ( !m_bUseGrenade )
		{
			Zoom();
			// VXP: Playing EMPTY sound because of "secondary_ammo" is not "None"
		}
		else
		{
			SecondaryAttack();
		}
	}

	if ( pOwner->m_afButtonPressed & IN_ATTACK3 )
	{
		UseGrenade( !m_bUseGrenade );
		
		if ( m_bZoomed )
		{
			Zoom();
		}
		
		//Msg( "AR2 secondary mode has changed (%s)\n", ( (m_bUseGrenade) ? "grenade" : "sight" ) );

		if ( ar2_switch_mode_sounds.GetBool() )
		{
		//	CPASAttenuationFilter filter( this );
		//	EmitSound( filter, entindex(), CHAN_VOICE, "weapons/ar2/ar2_mode.wav", 1, ATTN_NORM );

			EmitSound( m_bUseGrenade ? "Weapon_AR2.GrenadeMode" : "Weapon_AR2.ZoomMode" );
		}
	}
	
	//Don't kick the same when we're zoomed in
	if ( m_bZoomed )
	{
		m_fFireDuration = 0.05f;
	}
	
	// VXP: Fix for EMPTY sound while zooming
	if ( !m_bUseGrenade )
	{
	//	return; // VXP: But PrimaryAttack not working then...
	}

	BaseClass::ItemPostFrame();
}

// void CWeaponOICW::ScreenTextThink()
// {
//		NDebugOverlay::ScreenText( 0.85, 0.9, (m_bUseGrenade ? "Grenade" : "Zoom"), 255, 127, 0, 255, 0.0 );
// }

void CWeaponOICW::SendGrenageUsageState()
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( !pPlayer )
		return;

	CSingleUserRecipientFilter user( pPlayer );
	user.MakeReliable();
	UserMessageBegin( user, "AR2ModeChanged" );
		WRITE_BOOL( m_bUseGrenade );
	MessageEnd();
}

void CWeaponOICW::UseGrenade( bool use ) // Do this at Spawn too
{
	if ( m_bUseGrenade == use )
		return;

	m_bUseGrenade = use;
	SendGrenageUsageState();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponOICW::SecondaryAttack( void )
{
	if( !m_bUseGrenade )
	{
	//	BaseClass::SecondaryAttack();
		return;
	}

	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	//Must have ammo
	if ( pPlayer->GetAmmoCount( m_iSecondaryAmmoType ) <= 0 )
	{
		SendWeaponAnim( ACT_VM_DRYFIRE );
		BaseClass::WeaponSound( EMPTY );
		m_flNextSecondaryAttack = gpGlobals->curtime + 0.5f;
		return;
	}

	// MUST call sound before removing a round from the clip of a CMachineGun
	//BaseClass::WeaponSound( DOUBLE );

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecThrow = pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES ) * 1000.0f;
	
	//Create the grenade
	CGrenadeAR2 *pGrenade = (CGrenadeAR2*)Create( "grenade_ar2", vecSrc, vec3_angle, pPlayer );
	pGrenade->SetAbsVelocity( vecThrow );

	pGrenade->SetLocalAngularVelocity( RandomAngle( -400, 400 ) );
	pGrenade->SetMoveType( MOVETYPE_FLYGRAVITY ); 
	pGrenade->SetThrower( GetOwner() );
	pGrenade->SetDamage( sk_plr_dmg_smg1_grenade.GetFloat() );

	SendWeaponAnim( ACT_VM_SECONDARYATTACK );

	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	// Decrease ammo
	pPlayer->RemoveAmmo( 1, m_iSecondaryAmmoType );

	// Can shoot again immediately
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;

	// Can blow up after a short delay (so have time to release mouse button)
	m_flNextSecondaryAttack = gpGlobals->curtime + 1.0f;

	// Register a muzzleflash for the AI.
	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );	
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Activity
//-----------------------------------------------------------------------------
int CWeaponOICW::GetPrimaryAttackActivity_OICW( void )
{
	if ( m_nShotsFired < 2 )
		return ACT_VM_PRIMARYATTACK;

	if ( m_nShotsFired < 3 )
		return ACT_VM_HITLEFT;
	
	if ( m_nShotsFired < 4 )
		return ACT_VM_HITLEFT2;

	return ACT_VM_HITRIGHT;
}

//---------------------------------------------------------
//---------------------------------------------------------
void CWeaponOICW::PrimaryAttack( void )
{
	m_nShotsFired++;

	// Call the baseclass's primary attack
	BaseClass::PrimaryAttack();

	// Stop the animation from looping repeatedly. :/
	// This is because we're inheriting from CHLMachineGun.
	SendWeaponAnim( GetPrimaryAttackActivity_OICW() );
}

void CWeaponOICW::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{ 
		case EVENT_WEAPON_AR2:
		{
			Vector vecShootOrigin, vecShootDir;
			vecShootOrigin = pOperator->Weapon_ShootPosition();

			CAI_BaseNPC *npc = pOperator->MyNPCPointer();
			ASSERT( npc != NULL );
			vecShootDir = npc->GetActualShootTrajectory( vecShootOrigin );
			WeaponSound(SINGLE_NPC);
			pOperator->FireBullets( 1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2 );;
			// pOperator->DoMuzzleFlash();	
			m_iClip1 = m_iClip1 - 1;
		}
		break;
		default:
			CBaseCombatWeapon::Operator_HandleAnimEvent( pEvent, pOperator );
			break;
	}
}

/*
==================================================
AddViewKick
==================================================
*/

void CWeaponOICW::AddViewKick( void )
{
	#define	EASY_DAMPEN			0.5f
	#define	MAX_VERTICAL_KICK	24.0f	//Degrees
	#define	SLIDE_LIMIT			3.0f	//Seconds
	
	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if (!pPlayer)
		return;

	DoMachineGunKick( pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, m_fFireDuration, SLIDE_LIMIT );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponOICW::Zoom( void )
{
	// LUCAS HACK: Since the AR2 is an HL2 weapon, cast the owner of this weapon
	// to CHL2_Player. This is done because CHL2_Player handles zooming.
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	color32 lightGreen = { 50, 255, 170, 32 };

	if ( m_bZoomed )
	{
		pPlayer->ShowViewModel( true );

		// Zoom out to the default zoom level
		WeaponSound(SPECIAL2);
		pPlayer->SetFOV( this, 0, 0.1f );
		m_bZoomed = false;

		UTIL_ScreenFade( pPlayer, lightGreen, 0.2f, 0, (FFADE_IN|FFADE_PURGE) );
	}
	else
	{
		pPlayer->ShowViewModel( false );

		WeaponSound(SPECIAL1);
		pPlayer->SetFOV( this, 35, 0.1f );
		m_bZoomed = true;

		UTIL_ScreenFade( pPlayer, lightGreen, 0.2f, 0, (FFADE_OUT|FFADE_PURGE|FFADE_STAYOUT) );	
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : float
//-----------------------------------------------------------------------------
float CWeaponOICW::GetFireRate( void )
{ 
	if ( m_bZoomed )
		return 0.3f;

	return 0.1f;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : NULL - 
//-----------------------------------------------------------------------------
bool CWeaponOICW::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	if ( m_bZoomed )
	{
		Zoom();
	}

	return BaseClass::Holster( pSwitchingTo );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponOICW::Reload( void )
{
	if ( m_bZoomed )
	{
		Zoom();
	}

	bool fRet;
	float fCacheTime = m_flNextSecondaryAttack;

	fRet = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD );
	if ( fRet )
	{
		// Undo whatever the reload process has done to our secondary
		// attack timer. We allow you to interrupt reloading to fire
		// a grenade.
		m_flNextSecondaryAttack = GetOwner()->m_flNextAttack = fCacheTime;

		WeaponSound( RELOAD );
	}

	return fRet;

//	return BaseClass::Reload();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponOICW::Drop( const Vector &velocity )
{
	if ( m_bZoomed )
	{
		Zoom();
	}

	BaseClass::Drop( velocity );
}
