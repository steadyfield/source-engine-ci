//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "grenade_hopwire.h"
#include "rope.h"
#include "rope_shared.h"
#include "beam_shared.h"
#include "physics.h"
#include "physics_saverestore.h"
#include "soundent.h"

// SET ME TO ZERO UPON RELEASE
ConVar	sv_hopwire_debug( "sv_hopwire_debug", "0", FCVAR_CHEAT, "Enables hopwire mine traceline debugging" );

BEGIN_DATADESC( CTetherHook )
	DEFINE_FIELD( m_hTetheredOwner, FIELD_EHANDLE ),
	DEFINE_PHYSPTR( m_pSpring ),
	DEFINE_FIELD( m_pRope, FIELD_CLASSPTR ),
	DEFINE_FIELD( m_pGlow, FIELD_CLASSPTR ),
	DEFINE_FIELD( m_pBeam, FIELD_CLASSPTR ),
	DEFINE_FIELD( m_bAttached, FIELD_BOOLEAN ),
	DEFINE_FUNCTION( HookThink ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( tetherhook, CTetherHook );

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CTetherHook::CreateVPhysics()
{
	// Create the object in the physics system
	IPhysicsObject *pPhysicsObject = VPhysicsInitNormal( SOLID_BBOX, 0, false );
	
	// Make sure I get touch called for static geometry
	if ( pPhysicsObject )
	{
		int flags = pPhysicsObject->GetCallbackFlags();
		flags |= CALLBACK_GLOBAL_TOUCH_STATIC;
		pPhysicsObject->SetCallbackFlags(flags);
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTetherHook::Spawn( void )
{
	m_bAttached = false;

	Precache();
	SetModel( TETHERHOOK_MODEL );

	UTIL_SetSize( this, vec3_origin, vec3_origin );

	CreateVPhysics();
}

void CTetherHook::Precache( void )
{
	PrecacheModel( "sprites/rollermine_shock.vmt" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTetherHook::CreateRope( void )
{
	//Make sure it's not already there
	if ( m_pRope == NULL )
	{
		if( !sv_hopwire_debug.GetBool() )
		{
			//Create it between ourselves and the owning grenade
			m_pRope = CRopeKeyframe::Create( this, m_hTetheredOwner, 0, 0 );
			
			if ( m_pRope != NULL )
			{
				m_pRope->m_Width		= 0.75;
				m_pRope->m_RopeLength	= 32;
				m_pRope->m_Slack		= 64;

				CPASAttenuationFilter filter( this,"TripwireGrenade.ShootRope" );
				EmitSound( filter, entindex(),"TripwireGrenade.ShootRope" );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CTetherHook::StartTouch( CBaseEntity *pOther )
{
	if ( m_bAttached == false )
	{
		m_bAttached = true;

		SetVelocity( vec3_origin, vec3_origin );
		SetMoveType( MOVETYPE_NONE );
		VPhysicsDestroyObject();

		EmitSound( "TripwireGrenade.Hook" );

		StopSound( entindex(),"TripwireGrenade.ShootRope" );

		//Make a physics constraint between us and the owner
		if ( m_pSpring == NULL )
		{
			springparams_t spring;

			//FIXME: Make these real
			spring.constant			= 150.0f;
			spring.damping			= 24.0f;
			spring.naturalLength	= 32;
			spring.relativeDamping	= 0.1f;
			spring.startPosition	= GetAbsOrigin();
			spring.endPosition		= m_hTetheredOwner->WorldSpaceCenter();
			spring.useLocalPositions= false;
			
			IPhysicsObject *pEnd	= m_hTetheredOwner->VPhysicsGetObject();

			m_pSpring = physenv->CreateSpring( g_PhysWorldObject, pEnd, &spring );
		}

		SetThink( &CTetherHook::HookThink );
		SetNextThink( gpGlobals->curtime + 0.1f );
		//UTIL_Remove(this);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &velocity - 
//			&angVelocity - 
//-----------------------------------------------------------------------------
void CTetherHook::SetVelocity( const Vector &velocity, const AngularImpulse &angVelocity )
{
	IPhysicsObject *pPhysicsObject = VPhysicsGetObject();

	if ( pPhysicsObject != NULL )
	{
		pPhysicsObject->AddVelocity( &velocity, &angVelocity );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTetherHook::HookThink( void )
{
	if ( m_pBeam == NULL )
	{
		if( !sv_hopwire_debug.GetBool() )
		{
			m_pBeam = CBeam::BeamCreate( "sprites/rollermine_shock.vmt", 1.0f );
			m_pBeam->EntsInit( this, m_hTetheredOwner );

			m_pBeam->SetNoise( 0.5f );
			m_pBeam->SetColor( 255, 255, 255 );
			m_pBeam->SetScrollRate( 25 );
			m_pBeam->SetBrightness( 128 );
			m_pBeam->SetWidth( 4.0f );
			m_pBeam->SetEndWidth( 1.0f );
		}
	}

	if ( m_pGlow == NULL )
	{
		m_pGlow = CSprite::SpriteCreate( "sprites/blueflare1.vmt", GetLocalOrigin(), false );

		if ( m_pGlow != NULL )
		{
			m_pGlow->SetParent( this );
			m_pGlow->SetTransparency( kRenderTransAdd, 255, 255, 255, 255, kRenderFxNone );
			m_pGlow->SetBrightness( 128, 0.1f );
			m_pGlow->SetScale( 0.5f, 0.1f );
		}
	}

	trace_t tr;
	UTIL_TraceLine( GetAbsOrigin(), m_hTetheredOwner->GetAbsOrigin(), MASK_ALL, this, COLLISION_GROUP_NONE, &tr );

	if( sv_hopwire_debug.GetBool() )
	{
		NDebugOverlay::Line( tr.startpos, tr.endpos, 255,0,0, true, 0.1f );
	}

	CBaseEntity *pHitEnt = tr.m_pEnt;
	if(pHitEnt)
	{
		if(pHitEnt->IsPlayer() || pHitEnt->IsNPC())
		{
			m_hTetheredOwner->TetherDetonate();
		}
	}

	SetNextThink( gpGlobals->curtime + 0.1f );
}

//-----------------------------------------------------------------------------
// Author: lucas93
// Purpose: 
//-----------------------------------------------------------------------------
void CTetherHook::RemoveThis()
{
	UTIL_Remove( m_pRope );
	UTIL_Remove( m_pGlow );
	UTIL_Remove( m_pBeam );
	UTIL_Remove( this );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &origin - 
//			&angles - 
//			*pOwner - 
//-----------------------------------------------------------------------------
CTetherHook	*CTetherHook::Create( const Vector &origin, const QAngle &angles, CGrenadeHopWire *pOwner )
{
	CTetherHook *pHook = CREATE_ENTITY( CTetherHook, "tetherhook" );
	
	if ( pHook != NULL )
	{
		pHook->m_hTetheredOwner = pOwner;
		pHook->SetAbsOrigin( origin );
		pHook->SetAbsAngles( angles );
		pHook->SetOwnerEntity( (CBaseEntity *) pOwner );
		pHook->Spawn();
	}

	return pHook;
}

//-----------------------------------------------------------------------------

#define GRENADE_MODEL "models/Weapons/w_hopwire.mdl"

BEGIN_DATADESC( CGrenadeHopWire )
	DEFINE_FIELD( m_nHooksShot, FIELD_INTEGER ),
	DEFINE_FIELD( m_pGlow, FIELD_CLASSPTR ),
	DEFINE_FUNCTION( TetherThink ),
	DEFINE_FUNCTION( CombatThink ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( npc_grenade_hopwire, CGrenadeHopWire );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeHopWire::Spawn( void )
{
	Precache();

	SetModel( GRENADE_MODEL );

	m_nHooksShot	= 0;
	m_pGlow			= NULL;

	CreateVPhysics();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CGrenadeHopWire::CreateVPhysics()
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, 0, false );
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeHopWire::Precache( void )
{
    PrecacheScriptSound( "TripmineGrenade.Charge" );
	PrecacheScriptSound( "TripmineGrenade.PowerUp" );
	PrecacheScriptSound( "TripmineGrenade.StopSound" );
	PrecacheScriptSound( "TripwireGrenade.Activate" );
	PrecacheScriptSound( "TripwireGrenade.ShootRope" );
	PrecacheScriptSound( "TripwireGrenade.Hook" );

	PrecacheModel( GRENADE_MODEL );
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : timer - 
//-----------------------------------------------------------------------------
void CGrenadeHopWire::SetTimer( float timer )
{
	// lucas93: trigger sound
	CPASAttenuationFilter filter( this,"TripwireGrenade.Activate" );
	EmitSound( filter, entindex(),"TripwireGrenade.Activate" );

	SetThink( &CGrenadeHopWire::Detonate );
	SetNextThink( gpGlobals->curtime + timer );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeHopWire::CombatThink( void )
{
	if ( m_pGlow == NULL )
	{
		m_pGlow = CSprite::SpriteCreate( "sprites/blueflare1.vmt", GetLocalOrigin(), false );

		if ( m_pGlow != NULL )
		{
			m_pGlow->SetParent( this );
			m_pGlow->SetTransparency( kRenderTransAdd, 255, 255, 255, 255, kRenderFxNone );
			m_pGlow->SetBrightness( 128, 0.1f );
			m_pGlow->SetScale( 1.0f, 0.1f );
		}
	}

	SetNextThink( gpGlobals->curtime + 0.1f );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeHopWire::TetherThink( void )
{
	CTetherHook *pHook = NULL;
	static Vector velocity = RandomVector( -1.0f, 1.0f );

	//Create a tether hook
	pHook = (CTetherHook *) CTetherHook::Create( GetLocalOrigin(), GetLocalAngles(), this );

	if ( pHook == NULL )
		return;

	pHook->CreateRope();

	if ( m_nHooksShot % 2 )
	{
		velocity.Negate();
	}
	else
	{
		velocity = RandomVector( -1.0f, 1.0f );
	}

	pHook->SetVelocity( velocity * 1500.0f, vec3_origin );

	pHooks[m_nHooksShot] = pHook;

	m_nHooksShot++;

	if ( m_nHooksShot == MAX_HOOKS )
	{
		// lucas93: activation sound
		CPASAttenuationFilter filter( this,"Weapon_Tripwire.Attach" );
		EmitSound( filter, entindex(),"Weapon_Tripwire.Attach" );

		SetNextThink( gpGlobals->curtime + 0.1f );
		SetThink( &CGrenadeHopWire::CombatThink );
	}
	else
	{
		SetThink( &CGrenadeHopWire::TetherThink );
		SetNextThink( gpGlobals->curtime + random->RandomFloat( 0.1f, 0.3f ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &velocity - 
//			&angVelocity - 
//-----------------------------------------------------------------------------
void CGrenadeHopWire::SetVelocity( const Vector &velocity, const AngularImpulse &angVelocity )
{
	IPhysicsObject *pPhysicsObject = VPhysicsGetObject();
	
	if ( pPhysicsObject != NULL )
	{
		pPhysicsObject->AddVelocity( &velocity, &angVelocity );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeHopWire::Detonate( void )
{
	Vector	hopVel = RandomVector( -8, 8 );
	hopVel[2] += 800.0f;

	AngularImpulse	hopAngle = RandomAngularImpulse( -300, 300 );

	//FIXME: We should find out how tall the ceiling is and always try to hop halfway

	//Add upwards velocity for the "hop"
	SetVelocity( hopVel, hopAngle );

	//Shoot 4-8 cords out to grasp the surroundings
	SetThink( &CGrenadeHopWire::TetherThink );
	SetNextThink( gpGlobals->curtime + 0.6f );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrenadeHopWire::TetherDetonate( void )
{
	// lucas93: kill our tethers
	for( int i = 0; i < MAX_HOOKS; i++ ) {
		pHooks[i]->RemoveThis();
	}

	trace_t		tr;
	Vector		vecSpot;// trace starts here!

	vecSpot = GetAbsOrigin() + Vector ( 0 , 0 , 8 );
	UTIL_TraceLine ( vecSpot, vecSpot + Vector ( 0, 0, -40 ),  MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, & tr);

	Explode( &tr, DMG_BLAST );

	if ( GetShakeAmplitude() )
	{
		UTIL_ScreenShake( GetAbsOrigin(), GetShakeAmplitude(), 150.0, 1.0, GetShakeRadius(), SHAKE_START );
	}

	// now that we're exploded, kill ourself
	UTIL_Remove( m_pGlow );
	UTIL_Remove( this );
}

//-----------------------------------------------------------------------------
// Purpose: Ripped straight from CBaseGrenade (and modified :P)
//-----------------------------------------------------------------------------
extern short g_sModelIndexFireball; 
extern short g_sModelIndexWExplosion;
void CGrenadeHopWire::Explode( trace_t *pTrace, int bitsDamageType )
{
#if !defined( CLIENT_DLL )
	float		flRndSound;// sound randomizer

	m_flDamage = HOPWIRE_DAMAGE;
	m_DmgRadius = HOPWIRE_DMG_RADIUS;

	SetModelName( NULL_STRING );//invisible
	AddSolidFlags( FSOLID_NOT_SOLID );

	m_takedamage = DAMAGE_NO;

	// Pull out of the wall a bit
	if ( pTrace->fraction != 1.0 )
	{
		SetLocalOrigin( pTrace->endpos + (pTrace->plane.normal * 0.6) );
	}

	Vector vecAbsOrigin = GetAbsOrigin();
	int contents = UTIL_PointContents ( vecAbsOrigin );

	if ( pTrace->fraction != 1.0 )
	{
		Vector vecNormal = pTrace->plane.normal;
		surfacedata_t *pdata = physprops->GetSurfaceData( pTrace->surface.surfaceProps );	
		CPASFilter filter( vecAbsOrigin );
		te->Explosion( filter, 0.0, 
			&vecAbsOrigin,
			!( contents & MASK_WATER ) ? g_sModelIndexFireball : g_sModelIndexWExplosion,
			m_DmgRadius * .03, 
			25,
			TE_EXPLFLAG_NONE,
			m_DmgRadius,
			m_flDamage,
			&vecNormal,
			NULL );
	}
	else
	{
		CPASFilter filter( vecAbsOrigin );
		te->Explosion( filter, 0.0,
			&vecAbsOrigin, 
			!( contents & MASK_WATER ) ? g_sModelIndexFireball : g_sModelIndexWExplosion,
			m_DmgRadius * .03, 
			25,
			TE_EXPLFLAG_NONE,
			m_DmgRadius,
			m_flDamage );
	}

#if !defined( CLIENT_DLL )
	CSoundEnt::InsertSound ( SOUND_COMBAT, GetAbsOrigin(), BASEGRENADE_EXPLOSION_VOLUME, 3.0 );
#endif

	// Use the owner's position as the reported position
	Vector vecReported = GetThrower() ? GetThrower()->GetAbsOrigin() : vec3_origin;
	
	CTakeDamageInfo info( this, GetThrower(), GetBlastForce(), GetAbsOrigin(), m_flDamage, bitsDamageType, 0, &vecReported );

	RadiusDamage( info, GetAbsOrigin(), m_DmgRadius, CLASS_NONE, NULL );

	UTIL_DecalTrace( pTrace, "Scorch" );

	flRndSound = random->RandomFloat( 0 , 1 );

	EmitSound( "BaseGrenade.Explode" );

	SetThink( &CBaseGrenade::SUB_Remove );
	SetTouch( NULL );
	
	AddEffects( EF_NODRAW );
	SetAbsVelocity( vec3_origin );
	SetNextThink( gpGlobals->curtime );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CBaseGrenade *HopWire_Create( const Vector &position, const QAngle &angles, const Vector &velocity, const AngularImpulse &angVelocity, CBaseEntity *pOwner, float timer )
{
	CGrenadeHopWire *pGrenade = (CGrenadeHopWire *) CBaseEntity::Create( "npc_grenade_hopwire", position, angles, pOwner );
	
	pGrenade->SetTimer( timer );
	pGrenade->SetVelocity( velocity, angVelocity );
	pGrenade->SetThrower( ToBaseCombatCharacter( pOwner ) );
	
	return pGrenade;
}
