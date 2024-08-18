//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"

#include "weapon_alyxgun.h"
#include "npcevent.h"
#include "ai_basenpc.h"
#include "globalstate.h"
#include "gamestats.h"
#include "bullet_9mm.h"
#include "hl2_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar bullettimesim;

extern ConVar disable_bullettime;

IMPLEMENT_SERVERCLASS_ST(CWeaponAlyxGun, DT_WeaponAlyxGun)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_alyxgun, CWeaponAlyxGun );
PRECACHE_WEAPON_REGISTER(weapon_alyxgun);

BEGIN_DATADESC( CWeaponAlyxGun )
END_DATADESC()

acttable_t	CWeaponAlyxGun::m_acttable[] = 
{
	{ ACT_IDLE,						ACT_IDLE_PISTOL,				true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_PISTOL,			true },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_PISTOL,		true },
	{ ACT_RELOAD,					ACT_RELOAD_PISTOL,				true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_PISTOL,			true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_PISTOL,				true },
	{ ACT_COVER_LOW,				ACT_COVER_PISTOL_LOW,			true },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_PISTOL_LOW,		true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_PISTOL,true },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_PISTOL_LOW,			true },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_PISTOL_LOW,	true },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_PISTOL,		true },

	// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_PISTOL,				false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_STIMULATED,			false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_STEALTH,				ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_RELAXED,				ACT_WALK,						false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_STIMULATED,			false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_STEALTH,				ACT_WALK_STEALTH_PISTOL,		false },

	{ ACT_RUN_RELAXED,				ACT_RUN,						false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_STIMULATED,				false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_STEALTH,				ACT_RUN_STEALTH_PISTOL,			false },

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_PISTOL,				false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_ANGRY_PISTOL,			false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_AIM_STEALTH,			ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK,						false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_PISTOL,			false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_AIM_STEALTH,			ACT_WALK_AIM_STEALTH_PISTOL,	false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN,						false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_PISTOL,				false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_AIM_STEALTH,			ACT_RUN_AIM_STEALTH_PISTOL,		false },//always aims
	//End readiness activities

	// Crouch activities
	{ ACT_CROUCHIDLE_STIMULATED,	ACT_CROUCHIDLE_STIMULATED,		false },
	{ ACT_CROUCHIDLE_AIM_STIMULATED,ACT_RANGE_AIM_PISTOL_LOW,		false },//always aims
	{ ACT_CROUCHIDLE_AGITATED,		ACT_RANGE_AIM_PISTOL_LOW,		false },//always aims

	// Readiness translations
	{ ACT_READINESS_RELAXED_TO_STIMULATED, ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED, false },
	{ ACT_READINESS_RELAXED_TO_STIMULATED_WALK, ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED_WALK, false },
	{ ACT_READINESS_AGITATED_TO_STIMULATED, ACT_READINESS_PISTOL_AGITATED_TO_STIMULATED, false },
	{ ACT_READINESS_STIMULATED_TO_RELAXED, ACT_READINESS_PISTOL_STIMULATED_TO_RELAXED, false },

	{ ACT_HL2MP_IDLE, ACT_HL2MP_IDLE_PISTOL, false },
	{ ACT_HL2MP_RUN, ACT_HL2MP_RUN_PISTOL, false },
	{ ACT_HL2MP_IDLE_CROUCH, ACT_HL2MP_IDLE_CROUCH_PISTOL, false },
	{ ACT_HL2MP_WALK_CROUCH, ACT_HL2MP_WALK_CROUCH_PISTOL, false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK, ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL, false },
	{ ACT_HL2MP_GESTURE_RELOAD, ACT_HL2MP_GESTURE_RELOAD_PISTOL, false },
	{ ACT_HL2MP_JUMP, ACT_HL2MP_JUMP_PISTOL, false },
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_PISTOL, false },

//	{ ACT_ARM,				ACT_ARM_PISTOL,					true },
//	{ ACT_DISARM,			ACT_DISARM_PISTOL,				true },
};

IMPLEMENT_ACTTABLE(CWeaponAlyxGun);

#define TOOCLOSETIMER_OFF	0.0f
#define ALYX_TOOCLOSETIMER	1.0f		// Time an enemy must be tooclose before Alyx is allowed to shoot it.

//=========================================================
CWeaponAlyxGun::CWeaponAlyxGun( )
{
	m_fMinRange1		= 1;
	m_fMaxRange1		= 5000;

	m_flTooCloseTimer	= TOOCLOSETIMER_OFF;

#ifdef HL2_EPISODIC
	m_fMinRange1		= 60;
	m_fMaxRange1		= 2048;
#endif//HL2_EPISODIC
}

CWeaponAlyxGun::~CWeaponAlyxGun( )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponAlyxGun::Precache( void )
{
	BaseClass::Precache();

	UTIL_PrecacheOther("bullet_9mm");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CWeaponAlyxGun::Equip( CBaseCombatCharacter *pOwner )
{
	BaseClass::Equip( pOwner );
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CWeaponAlyxGun::PrimaryAttack( void )
{
	CHL2_Player *pPlayer = dynamic_cast < CHL2_Player* >(UTIL_PlayerByIndex(1));

	if (!pPlayer->m_HL2Local.m_bInSlowMo && disable_bullettime.GetInt() == 0)
	{
		// Only the player fires this way so we can cast
		CBasePlayer *pPlayer = ToBasePlayer(GetOwner());
		if (!pPlayer)
			return;

		// Abort here to handle burst and auto fire modes
		if ((UsesClipsForAmmo1() && m_iClip1 == 0) || (!UsesClipsForAmmo1() && !pPlayer->GetAmmoCount(m_iPrimaryAmmoType)))
			return;

		m_nShotsFired++;

		pPlayer->DoMuzzleFlash();

		// To make the firing framerate independent, we may have to fire more than one bullet here on low-framerate systems, 
		// especially if the weapon we're firing has a really fast rate of fire.
		int iBulletsToFire = 0;
		float fireRate = GetFireRate();

		// MUST call sound before removing a round from the clip of a CHLMachineGun
		while (m_flNextPrimaryAttack <= gpGlobals->curtime)
		{
			WeaponSound(SINGLE, m_flNextPrimaryAttack);
			m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
			iBulletsToFire++;
		}

		// Make sure we don't fire more than the amount in the clip, if this weapon uses clips
		if (UsesClipsForAmmo1())
		{
			if (iBulletsToFire > m_iClip1)
				iBulletsToFire = m_iClip1;
			m_iClip1 -= iBulletsToFire;
		}

		m_iPrimaryAttacks++;
		gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

		// Fire the bullets
		FireBulletsInfo_t info;
		info.m_iShots = iBulletsToFire;
		info.m_vecSrc = pPlayer->Weapon_ShootPosition();
		info.m_vecDirShooting = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);
		info.m_vecSpread = pPlayer->GetAttackSpread(this);
		info.m_flDistance = MAX_TRACE_LENGTH;
		info.m_iAmmoType = m_iPrimaryAmmoType;
		info.m_iTracerFreq = 2;
		FireBullets(info);

		//Factor in the view kick
		AddViewKick();

		CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pPlayer);

		if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
		{
			// HEV suit - indicate out of ammo condition
			pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
		}

		SendWeaponAnim(GetPrimaryAttackActivity());
		pPlayer->SetAnimation(PLAYER_ATTACK1);

		// Register a muzzleflash for the AI
		pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);
	}
	else if (pPlayer->m_HL2Local.m_bInSlowMo && disable_bullettime.GetInt() == 0)
	{
		Fire9MMBullet();
	}

	if (bullettimesim.GetInt() == 1 && disable_bullettime.GetInt() == 1)
	{
		Fire9MMBullet();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CWeaponAlyxGun::Fire9MMBullet( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if (!pPlayer)
		return;
	
	// Abort here to handle burst and auto fire modes
	if ( (UsesClipsForAmmo1() && m_iClip1 == 0) || ( !UsesClipsForAmmo1() && !pPlayer->GetAmmoCount(m_iPrimaryAmmoType) ) )
		return;

	m_nShotsFired++;

	pPlayer->DoMuzzleFlash();

	// To make the firing framerate independent, we may have to fire more than one bullet here on low-framerate systems, 
	// especially if the weapon we're firing has a really fast rate of fire.
	int iBulletsToFire = 0;
	float fireRate = GetFireRate();

	// MUST call sound before removing a round from the clip of a CHLMachineGun
	while ( m_flNextPrimaryAttack <= gpGlobals->curtime )
	{
		WeaponSound(SINGLE, m_flNextPrimaryAttack);
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
		iBulletsToFire++;
	}

	// Make sure we don't fire more than the amount in the clip, if this weapon uses clips
	if ( UsesClipsForAmmo1() )
	{
		if ( iBulletsToFire > m_iClip1 )
			iBulletsToFire = m_iClip1;
		m_iClip1 -= iBulletsToFire;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired( pPlayer, true, GetClassname() );

	Vector vecAiming;
	vecAiming = pPlayer->GetAutoaimVector(0);
	Vector vecSrc = pPlayer->Weapon_ShootPosition();

	QAngle angAiming;
	VectorAngles(vecAiming, angAiming);

	CBullet9MM *pBullet = CBullet9MM::BulletCreate(vecSrc, angAiming, pPlayer);

	if (pPlayer->GetWaterLevel() == 3)
	{
		pBullet->SetAbsVelocity(vecAiming * 1100);
	}
	else
	{
		pBullet->SetAbsVelocity(vecAiming * 1400);
	}

	m_flNextPrimaryAttack = gpGlobals->curtime + fireRate;

	/*
	// Fire the bullets
	FireBulletsInfo_t info;
	info.m_iShots = iBulletsToFire;
	info.m_vecSrc = pPlayer->Weapon_ShootPosition( );
	info.m_vecDirShooting = pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );
	info.m_vecSpread = pPlayer->GetAttackSpread( this );
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_iTracerFreq = 2;
	FireBullets( info );
	*/

	//Factor in the view kick
	AddViewKick();

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pPlayer );
	
	if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0); 
	}

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	// Register a muzzleflash for the AI
	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );
}

void CWeaponAlyxGun::FireNPC9MMBullet( void )
{
	// m_vecEnemyLKP should be center of enemy body
	Vector vecArmPos;
	QAngle angArmDir;
	Vector vecDirToEnemy;
	QAngle angDir;

	if ( GetEnemy() )
	{
		Vector vecEnemyLKP = GetEnemy()->GetAbsOrigin();

		vecDirToEnemy = ( ( vecEnemyLKP ) - GetAbsOrigin() );
		VectorAngles( vecDirToEnemy, angDir );
		VectorNormalize( vecDirToEnemy );
	}
	else
	{
		angDir = GetAbsAngles();
		angDir.x = -angDir.x;

		Vector vForward;
		AngleVectors( angDir, &vForward );
		vecDirToEnemy = vForward;
	}

	DoMuzzleFlash();

	// make angles +-180
	if (angDir.x > 180)
	{
		angDir.x = angDir.x - 360;
	}

	VectorAngles( vecDirToEnemy, angDir );

	float RandomAngle = (rand() % 55960);
	float RandMagnitudeX = ((rand() % 70375) / 3800.0);
	float RandMagnitudeY = ((rand() % 70375) / 3800.0);
	angDir.x += (RandMagnitudeX)*cos(RandomAngle);
	angDir.y += (RandMagnitudeY)*sin(RandomAngle);

	AngleVectors(angDir, &vecDirToEnemy);

	GetAttachment( "muzzle", vecArmPos, angArmDir );

	vecArmPos = vecArmPos + vecDirToEnemy * 32;

	CBaseEntity *pBullet = CBaseEntity::Create( "bullet_9mm", vecArmPos, QAngle( 0, 0, 0 ), this );

	Vector vForward;
	AngleVectors( angDir, &vForward );
	
	pBullet->SetAbsVelocity( vForward * 475 );
	pBullet->SetOwnerEntity( this );
			
	// CSoundEnt::InsertSound( SOUND_COMBAT|SOUND_CONTEXT_GUNFIRE, GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, this, SOUNDENT_CHANNEL_WEAPON, GetEnemy() );

	WeaponSound( SINGLE_NPC );

	m_iClip1 = m_iClip1 - 1;
}

//-----------------------------------------------------------------------------
// Purpose: Try to encourage Alyx not to use her weapon at point blank range,
//			but don't prevent her from defending herself if cornered.
// Input  : flDot - 
//			flDist - 
// Output : int
//-----------------------------------------------------------------------------
int CWeaponAlyxGun::WeaponRangeAttack1Condition( float flDot, float flDist )
{
#ifdef HL2_EPISODIC
	
	if( flDist < m_fMinRange1 )
	{
		// If Alyx is not able to fire because an enemy is too close, start a timer.
		// If the condition persists, allow her to ignore it and defend herself. The idea
		// is to stop Alyx being content to fire point blank at enemies if she's able to move
		// away, without making her defenseless if she's not able to move.
		float flTime;

		if( m_flTooCloseTimer == TOOCLOSETIMER_OFF )
		{
			m_flTooCloseTimer = gpGlobals->curtime;
		}

		flTime = gpGlobals->curtime - m_flTooCloseTimer;

		if( flTime > ALYX_TOOCLOSETIMER )
		{
			// Fake the range to allow Alyx to shoot.
			flDist = m_fMinRange1 + 1.0f;
		}
	}
	else
	{
		m_flTooCloseTimer = TOOCLOSETIMER_OFF;
	}

	int nBaseCondition = BaseClass::WeaponRangeAttack1Condition( flDot, flDist );

	// While in a vehicle, we extend our aiming cone (this relies on COND_NOT_FACING_ATTACK 
	// TODO: This needs to be rolled in at the animation level
	if ( GetOwner()->IsInAVehicle() )
	{
		Vector vecRoughDirection = ( GetOwner()->GetEnemy()->WorldSpaceCenter() - WorldSpaceCenter() );
		Vector vecRight;
		GetVectors( NULL, &vecRight, NULL );
		bool bRightSide = ( DotProduct( vecRoughDirection, vecRight ) > 0.0f );
		float flTargetDot = ( bRightSide ) ? -0.7f : 0.0f;
		
		if ( nBaseCondition == COND_NOT_FACING_ATTACK && flDot >= flTargetDot )
		{
			nBaseCondition = COND_CAN_RANGE_ATTACK1;
		}
	}

	return nBaseCondition;

#else 

	return BaseClass::WeaponRangeAttack1Condition( flDot, flDist );

#endif//HL2_EPISODIC
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : flDot - 
//			flDist - 
// Output : int
//-----------------------------------------------------------------------------
int CWeaponAlyxGun::WeaponRangeAttack2Condition( float flDot, float flDist )
{
	return COND_NONE;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOperator - 
//-----------------------------------------------------------------------------
void CWeaponAlyxGun::FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, bool bUseWeaponAngles )
{
  	Vector vecShootOrigin, vecShootDir;
	CAI_BaseNPC *npc = pOperator->MyNPCPointer();
	ASSERT( npc != NULL );

	if ( bUseWeaponAngles )
	{
		QAngle	angShootDir;
		GetAttachment( LookupAttachment( "muzzle" ), vecShootOrigin, angShootDir );
		AngleVectors( angShootDir, &vecShootDir );
	}
	else 
	{
		vecShootOrigin = pOperator->Weapon_ShootPosition();
 		vecShootDir = npc->GetActualShootTrajectory( vecShootOrigin );
	}

	WeaponSound( SINGLE_NPC );

	if( hl2_episodic.GetBool() )
	{
		pOperator->FireBullets( 1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 1 );
	}
	else
	{
		pOperator->FireBullets( 1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2 );
	}

	pOperator->DoMuzzleFlash();

	if( hl2_episodic.GetBool() )
	{
		// Never fire Alyx's last bullet just in case there's an emergency
		// and she needs to be able to shoot without reloading.
		if( m_iClip1 > 1 )
		{
			m_iClip1 = m_iClip1 - 1;
		}
	}
	else
	{
		m_iClip1 = m_iClip1 - 1;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponAlyxGun::Operator_ForceNPCFire( CBaseCombatCharacter *pOperator, bool bSecondary )
{
	// Ensure we have enough rounds in the clip
	m_iClip1++;

	// HACK: We need the gun to fire its muzzle flash
	if ( bSecondary == false )
	{
		SetActivity( ACT_RANGE_ATTACK_PISTOL, 0.0f );
	}

	FireNPCPrimaryAttack( pOperator, true );
}

//-----------------------------------------------------------------------------
void CWeaponAlyxGun::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	CHL2_Player *pPlayer = dynamic_cast < CHL2_Player* >(UTIL_PlayerByIndex(1));

	switch( pEvent->event )
	{
		case EVENT_WEAPON_PISTOL_FIRE:
		{
			if (!pPlayer->m_HL2Local.m_bInSlowMo && disable_bullettime.GetInt() == 0)
			{
		        FireNPCPrimaryAttack(pOperator, false);
			}
			else if (pPlayer->m_HL2Local.m_bInSlowMo && disable_bullettime.GetInt() == 0)
			{
				FireNPC9MMBullet();
			}
			if (bullettimesim.GetInt() == 1 && disable_bullettime.GetInt() == 1)
			{
				FireNPC9MMBullet();
			}
			break;
		}
		
		default:
			BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
			break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Whether or not Alyx is in the injured mode
//-----------------------------------------------------------------------------
bool IsAlyxInInjuredMode( void )
{
	if ( hl2_episodic.GetBool() == false )
		return false;

	return ( GlobalEntity_GetState("ep2_alyx_injured") == GLOBAL_ON );
}

//-----------------------------------------------------------------------------
const Vector& CWeaponAlyxGun::GetBulletSpread( void )
{
	static const Vector cone = VECTOR_CONE_2DEGREES;
	static const Vector injuredCone = VECTOR_CONE_6DEGREES;

	if ( IsAlyxInInjuredMode() )
		return injuredCone;

	return cone;
}

//-----------------------------------------------------------------------------
float CWeaponAlyxGun::GetMinRestTime( void )
{
	if ( IsAlyxInInjuredMode() )
		return 1.5f;

	return BaseClass::GetMinRestTime();
}

//-----------------------------------------------------------------------------
float CWeaponAlyxGun::GetMaxRestTime( void )
{
	if ( IsAlyxInInjuredMode() )
		return 3.0f;

	return BaseClass::GetMaxRestTime();
}
