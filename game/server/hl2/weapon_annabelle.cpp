//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Grigori's personal shotgun (npc_monk)
//
//=============================================================================//

#include	"cbase.h"
#include	"npcevent.h"
#include	"basehlcombatweapon_shared.h"
#include	"basecombatcharacter.h"
#include	"ai_basenpc.h"
#include	"player.h"
#include	"gamerules.h"		// For g_pGameRules
#include	"in_buttons.h"
#include	"soundent.h"
#include	"vstdlib/random.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar sk_auto_reload_time;

class CWeaponAnnabelle : public CBaseHLCombatWeapon
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CWeaponAnnabelle, CBaseHLCombatWeapon );

	DECLARE_SERVERCLASS();

	int GetMaxClip1() const; //SMOD

private:
	bool	m_bNeedPump;		// When emptied completely
	bool	m_bDelayedFire1;	// Fire primary when finished reloading
	bool	m_bDelayedFire2;	// Fire secondary when finished reloading

public:
	void	Precache( void );

	int CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	virtual const Vector& GetBulletSpread( void )
	{
		static Vector cone = vec3_origin;
		return cone;
	}

	virtual int				GetMinBurst() { return 1; }
	virtual int				GetMaxBurst() { return 3; }

	void ItemHolsterFrame( void );
	void CheckHolsterReload( void );
	void Pump( void );
	void DryFire( void );
	virtual float GetFireRate( void ) { return 1.5; };
	virtual float			GetMinRestTime() { return 1.0; }
	virtual float			GetMaxRestTime() { return 1.5; }

	void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	DECLARE_ACTTABLE();

	CWeaponAnnabelle(void);
};

IMPLEMENT_SERVERCLASS_ST(CWeaponAnnabelle, DT_WeaponAnnabelle)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_annabelle, CWeaponAnnabelle );
#ifndef HL2MP
PRECACHE_WEAPON_REGISTER(weapon_annabelle);
#endif

BEGIN_DATADESC( CWeaponAnnabelle )
	DEFINE_FIELD( m_bNeedPump, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bDelayedFire1, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bDelayedFire2, FIELD_BOOLEAN ),
END_DATADESC()

acttable_t	CWeaponAnnabelle::m_acttable[] = 
{
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_SMG1,				true },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_SHOTGUN,			true },
	{ ACT_RELOAD,					ACT_RELOAD_SMG1,					true },
	{ ACT_WALK,						ACT_WALK_RIFLE,						true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_RIFLE,					true },
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_RIFLE,				false },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_RIFLE,			false },
	{ ACT_RUN,						ACT_RUN_RIFLE,						true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_RIFLE,					true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_RIFLE,				false },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_RIFLE,			false },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_SHOTGUN,	true },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_SMG1_LOW,				false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_SMG1,			false },
};

IMPLEMENT_ACTTABLE(CWeaponAnnabelle);

void CWeaponAnnabelle::Precache( void )
{
	CBaseCombatWeapon::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CWeaponAnnabelle::GetMaxClip1(void) const
{
	CBasePlayer *player = ToBasePlayer( GetOwner() );
	if (player)
		return 6;
	else
		return 2;
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CWeaponAnnabelle::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
		case EVENT_WEAPON_SHOTGUN_FIRE:
		{
			Vector vecShootOrigin, vecShootDir;
			vecShootOrigin = pOperator->Weapon_ShootPosition();
			CAI_BaseNPC *npc = pOperator->MyNPCPointer();
			ASSERT( npc != NULL );
			WeaponSound( SINGLE_NPC );
			pOperator->DoMuzzleFlash();
			m_iClip1 = m_iClip1 - 1;

			vecShootDir = npc->GetActualShootTrajectory( vecShootOrigin );
			pOperator->FireBullets( 1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0 );
		}
		break;

		default:
			CBaseCombatWeapon::Operator_HandleAnimEvent( pEvent, pOperator );
			break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Play weapon pump anim
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CWeaponAnnabelle::Pump( void )
{
	CBaseCombatCharacter *pOwner  = GetOwner();

	if ( pOwner == NULL )
		return;
	
	m_bNeedPump = false;
	
	WeaponSound( SPECIAL1 );

	// Finish reload animation
	SendWeaponAnim( ACT_SHOTGUN_PUMP );

	pOwner->m_flNextAttack	= gpGlobals->curtime + SequenceDuration();
	m_flNextPrimaryAttack	= gpGlobals->curtime + SequenceDuration();
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CWeaponAnnabelle::DryFire( void )
{
	WeaponSound(EMPTY);
	SendWeaponAnim( ACT_VM_DRYFIRE );
	
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponAnnabelle::ItemHolsterFrame( void )
{
	// Must be player held
	if ( GetOwner() && GetOwner()->IsPlayer() == false )
		return;

	// We can't be active
	if ( GetOwner()->GetActiveWeapon() == this )
		return;

	// If it's been longer than three seconds, reload
	//SMOD: Die.
	/*if ( ( gpGlobals->curtime - m_flHolsterTime ) > sk_auto_reload_time.GetFloat() )
	{
		// Reset the timer
		m_flHolsterTime = gpGlobals->curtime;
	
		if ( GetOwner() == NULL )
			return;

		if ( m_iClip1 == GetMaxClip1() )
			return;

		// Just load the clip with no animations
		int ammoFill = MIN( (GetMaxClip1() - m_iClip1), GetOwner()->GetAmmoCount( GetPrimaryAmmoType() ) );
		
		GetOwner()->RemoveAmmo( ammoFill, GetPrimaryAmmoType() );
		m_iClip1 += ammoFill;
	}*/
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponAnnabelle::CWeaponAnnabelle( void )
{
	m_bReloadsSingly = false;

	m_bNeedPump		= false;
	m_bDelayedFire1 = false;
	m_bDelayedFire2 = false;

	m_fMinRange1		= 0.0;
	m_fMaxRange1		= 500;
	m_fMinRange2		= 0.0;
	m_fMaxRange2		= 200;
}
