//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Crowbar - an old favorite
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "weapon_hl2mpbase.h"

#ifdef CLIENT_DLL
#include "c_baseplayer.h"
#include "fx_impact.h"
#include "fx.h"
#else
#include "player.h"
#include "soundent.h"
#endif

#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"

#include "vstdlib/random.h"

extern ConVar sk_plr_dmg_crowbar;

#define	CROWBAR_RANGE		64.0f
#define	CROWBAR_REFIRE_MISS	0.5f
#define	CROWBAR_REFIRE_HIT	0.25f


#ifdef CLIENT_DLL
#define CHL1MPWeaponCrowbar C_HL1MPWeaponCrowbar
#endif

//-----------------------------------------------------------------------------
// CHL1MPWeaponCrowbar
//-----------------------------------------------------------------------------

class CHL1MPWeaponCrowbar : public CWeaponHL2MPBase
{
	DECLARE_CLASS( CHL1MPWeaponCrowbar, CWeaponHL2MPBase );
public:
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CHL1MPWeaponCrowbar();

	void			Precache( void );
	virtual void	ItemPostFrame( void );
	void			PrimaryAttack( void );

public:
	trace_t		m_traceHit;
	Activity	m_nHitActivity;

private:
	virtual void		Swing( void );
	virtual	void		Hit( void );
	virtual	void		ImpactEffect( void );
	void		ImpactSound( CBaseEntity *pHitEntity );
	virtual Activity	ChooseIntersectionPointAndActivity( trace_t &hitTrace, const Vector &mins, const Vector &maxs, CBasePlayer *pOwner );

public:

};

IMPLEMENT_NETWORKCLASS_ALIASED( HL1MPWeaponCrowbar, DT_HL1MPWeaponCrowbar );

BEGIN_NETWORK_TABLE( CHL1MPWeaponCrowbar, DT_HL1MPWeaponCrowbar )
/// what
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CHL1MPWeaponCrowbar )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_hl1mp_crowbar, CHL1MPWeaponCrowbar );
PRECACHE_WEAPON_REGISTER( weapon_hl1mp_crowbar );

#ifndef CLIENT_DLL
BEGIN_DATADESC( CHL1MPWeaponCrowbar )

	// DEFINE_FIELD( m_trLineHit, trace_t ),
	// DEFINE_FIELD( m_trHullHit, trace_t ),
	// DEFINE_FIELD( m_nHitActivity, FIELD_INTEGER ),
	// DEFINE_FIELD( m_traceHit, trace_t ),

	// Class CHL1MPWeaponCrowbar:
	// DEFINE_FIELD( m_nHitActivity, FIELD_INTEGER ),

	// Function Pointers
	DEFINE_FUNCTION( Hit ),

END_DATADESC()
#endif

#define BLUDGEON_HULL_DIM		16

static const Vector g_bludgeonMins(-BLUDGEON_HULL_DIM,-BLUDGEON_HULL_DIM,-BLUDGEON_HULL_DIM);
static const Vector g_bludgeonMaxs(BLUDGEON_HULL_DIM,BLUDGEON_HULL_DIM,BLUDGEON_HULL_DIM);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CHL1MPWeaponCrowbar::CHL1MPWeaponCrowbar()
{
	m_bFiresUnderwater = true;
}

//-----------------------------------------------------------------------------
// Purpose: Precache the weapon
//-----------------------------------------------------------------------------
void CHL1MPWeaponCrowbar::Precache( void )
{
	//Call base class first
	BaseClass::Precache();
}

//------------------------------------------------------------------------------
// Purpose : Update weapon
//------------------------------------------------------------------------------
void CHL1MPWeaponCrowbar::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	if ( (pOwner->m_nButtons & IN_ATTACK) && (m_flNextPrimaryAttack <= gpGlobals->curtime) )
	{
		PrimaryAttack();
	} 
	else 
	{
		WeaponIdle();
		return;
	}
}

//------------------------------------------------------------------------------
// Purpose :
// Input   :
// Output  :
//------------------------------------------------------------------------------
void CHL1MPWeaponCrowbar::PrimaryAttack()
{
	Swing();
}


//------------------------------------------------------------------------------
// Purpose: Implement impact function
//------------------------------------------------------------------------------
void CHL1MPWeaponCrowbar::Hit( void )
{
	//Make sound for the AI
#ifndef CLIENT_DLL

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	CSoundEnt::InsertSound( SOUND_BULLET_IMPACT, m_traceHit.endpos, 400, 0.2f, pPlayer );

	CBaseEntity	*pHitEntity = m_traceHit.m_pEnt;

	//Apply damage to a hit target
	if ( pHitEntity != NULL )
	{
		Vector hitDirection;
		pPlayer->EyeVectors( &hitDirection, NULL, NULL );
		VectorNormalize( hitDirection );

		ClearMultiDamage();
		CTakeDamageInfo info( GetOwner(), GetOwner(), sk_plr_dmg_crowbar.GetFloat(), DMG_CLUB );
		CalculateMeleeDamageForce( &info, hitDirection, m_traceHit.endpos );
		pHitEntity->DispatchTraceAttack( info, hitDirection, &m_traceHit ); 
		ApplyMultiDamage();

		// Now hit all triggers along the ray that... 
		TraceAttackToTriggers( CTakeDamageInfo( GetOwner(), GetOwner(), sk_plr_dmg_crowbar.GetFloat(), DMG_CLUB ), m_traceHit.startpos, m_traceHit.endpos, hitDirection );

		//Play an impact sound	
		ImpactSound( pHitEntity );
	}
#endif

	//Apply an impact effect
	ImpactEffect();
}

//-----------------------------------------------------------------------------
// Purpose: Play the impact sound
// Input  : pHitEntity - entity that we hit
// assumes pHitEntity is not null
//-----------------------------------------------------------------------------
void CHL1MPWeaponCrowbar::ImpactSound( CBaseEntity *pHitEntity )
{
	bool bIsWorld = ( pHitEntity->entindex() == 0 );
#ifndef CLIENT_DLL
	if ( !bIsWorld )
	{
		bIsWorld |=	pHitEntity->Classify() == CLASS_NONE ||  pHitEntity->Classify() == CLASS_MACHINE;
	}
#endif

	if( bIsWorld )
	{
		WeaponSound( MELEE_HIT_WORLD );
	}
	else 
	{
		WeaponSound( MELEE_HIT );
	}
}

Activity CHL1MPWeaponCrowbar::ChooseIntersectionPointAndActivity( trace_t &hitTrace, const Vector &mins, const Vector &maxs, CBasePlayer *pOwner )
{
	int			i, j, k;
	float		distance;
	const float	*minmaxs[2] = {mins.Base(), maxs.Base()};
	trace_t		tmpTrace;
	Vector		vecHullEnd = hitTrace.endpos;
	Vector		vecEnd;

	distance = 1e6f;
	Vector vecSrc = hitTrace.startpos;

	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc)*2);
	UTIL_TraceLine( vecSrc, vecHullEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &tmpTrace );
	if ( tmpTrace.fraction == 1.0 )
	{
		for ( i = 0; i < 2; i++ )
		{
			for ( j = 0; j < 2; j++ )
			{
				for ( k = 0; k < 2; k++ )
				{
					vecEnd.x = vecHullEnd.x + minmaxs[i][0];
					vecEnd.y = vecHullEnd.y + minmaxs[j][1];
					vecEnd.z = vecHullEnd.z + minmaxs[k][2];

					UTIL_TraceLine( vecSrc, vecEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &tmpTrace );
					if ( tmpTrace.fraction < 1.0 )
					{
						float thisDistance = (tmpTrace.endpos - vecSrc).Length();
						if ( thisDistance < distance )
						{
							hitTrace = tmpTrace;
							distance = thisDistance;
						}
					}
				}
			}
		}
	}
	else
	{
		hitTrace = tmpTrace;
	}


	return ACT_VM_HITCENTER;
}

#ifdef HL1MP_CLIENT_DLL
//-----------------------------------------------------------------------------
// Purpose: Handle jeep impacts
//-----------------------------------------------------------------------------
void ImpactCrowbarCallback( const CEffectData &data )
{
	trace_t tr;
	Vector vecOrigin, vecStart, vecShotDir;
	int iMaterial, iDamageType, iHitbox;
	short nSurfaceProp;
	C_BaseEntity *pEntity = ParseImpactData( data, &vecOrigin, &vecStart, &vecShotDir, nSurfaceProp, iMaterial, iDamageType, iHitbox );

	bool bIsWorld = ( pEntity->entindex() == 0 );

	if ( !pEntity )
	{
		// This happens for impacts that occur on an object that's then destroyed.
		// Clear out the fraction so it uses the server's data
		tr.fraction = 1.0;
		GetActiveWeapon()->WeaponSound( bIsWorld ? MELEE_HIT_WORLD : MELEE_HIT );
		return;
	}

	// If we hit, perform our custom effects and play the sound
	if ( Impact( vecOrigin, vecStart, iMaterial, iDamageType, iHitbox, pEntity, tr ) )
	{
		// Check for custom effects based on the Decal index
		PerformCustomEffects( vecOrigin, tr, vecShotDir, iMaterial, 2 );
	}

	GetActiveWeapon()->WeaponSound( bIsWorld ? MELEE_HIT_WORLD : MELEE_HIT );
}

DECLARE_CLIENT_EFFECT( "ImpactCrowbar", ImpactCrowbarCallback );

#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHL1MPWeaponCrowbar::ImpactEffect( void )
{
	//FIXME: need new decals
#ifdef HL1MP_CLIENT_DLL
	// in hl1mp force the basic crowbar sound
	UTIL_ImpactTrace( &m_traceHit, DMG_CLUB, "ImpactCrowbar" );
#else
	UTIL_ImpactTrace( &m_traceHit, DMG_CLUB );
#endif
}

//------------------------------------------------------------------------------
// Purpose : Starts the swing of the weapon and determines the animation
//------------------------------------------------------------------------------
void CHL1MPWeaponCrowbar::Swing( void )
{
	// Try a ray
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	Vector swingStart = pOwner->Weapon_ShootPosition( );
	Vector forward;

	pOwner->EyeVectors( &forward, NULL, NULL );

	Vector swingEnd = swingStart + forward * CROWBAR_RANGE;

	UTIL_TraceLine( swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &m_traceHit );
	m_nHitActivity = ACT_VM_HITCENTER;

	if ( m_traceHit.fraction == 1.0 )
	{
		float bludgeonHullRadius = 1.732f * BLUDGEON_HULL_DIM;  // hull is +/- 16, so use cuberoot of 2 to determine how big the hull is from center to the corner point

		// Back off by hull "radius"
		swingEnd -= forward * bludgeonHullRadius;

		UTIL_TraceHull( swingStart, swingEnd, g_bludgeonMins, g_bludgeonMaxs, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &m_traceHit );
		if ( m_traceHit.fraction < 1.0 )
		{
			m_nHitActivity = ChooseIntersectionPointAndActivity( m_traceHit, g_bludgeonMins, g_bludgeonMaxs, pOwner );
		}
	}


	// -------------------------
	//	Miss
	// -------------------------
	if ( m_traceHit.fraction == 1.0f )
	{
		m_nHitActivity = ACT_VM_MISSCENTER;

		//Play swing sound
		WeaponSound( SINGLE );

		//Setup our next attack times
		m_flNextPrimaryAttack = gpGlobals->curtime + CROWBAR_REFIRE_MISS;
	}
	else
	{
		Hit();

		//Setup our next attack times
		m_flNextPrimaryAttack = gpGlobals->curtime + CROWBAR_REFIRE_HIT;
	}

	//Send the anim
	SendWeaponAnim( m_nHitActivity );
	pOwner->SetAnimation( PLAYER_ATTACK1 );
}
