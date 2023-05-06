//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Crossbow
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "NPCEvent.h"
#include "hl1mp_weapon_crossbow.h"
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
#endif

extern ConVar sk_plr_dmg_xbow_bolt_plr;
extern ConVar sk_plr_dmg_xbow_bolt_npc;

extern short	g_sModelIndexFireball;		// (in combatweapon.cpp) holds the index for the fireball 

#if !defined(CLIENT_DLL)

#define BOLT_MODEL			"models/crossbow_dart.mdl"

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
LINK_ENTITY_TO_CLASS( crossbow_dart, CHL1CrossbowBolt );

BEGIN_DATADESC( CHL1CrossbowBolt )
	// Function Pointers
	DEFINE_FUNCTION( BubbleThink ),
	DEFINE_FUNCTION( BoltTouch ),
    DEFINE_FUNCTION( ExplodeThink ),
END_DATADESC()

CHL1CrossbowBolt *CHL1CrossbowBolt::BoltCreate( const Vector &vecOrigin, const QAngle &angAngles, CBasePlayer *pentOwner )
{
	// Create a new entity with CHL1CrossbowBolt private data
	CHL1CrossbowBolt *pBolt = (CHL1CrossbowBolt *)CreateEntityByName( "crossbow_dart" );
	UTIL_SetOrigin( pBolt, vecOrigin );
	pBolt->SetAbsAngles( angAngles );
	pBolt->Spawn();
	pBolt->SetOwnerEntity( pentOwner );

	return pBolt;
}

void CHL1CrossbowBolt::Spawn( )
{
	Precache( );

	SetModel( BOLT_MODEL );
	UTIL_SetSize( this, -Vector(1, 1, 1), Vector(1, 1, 1) );

	SetSolid( SOLID_BBOX );
	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_BOUNCE );
	SetGravity( UTIL_ScaleForGravity( 40 ) );	// use a really low gravity (40 in/s^2)

	SetTouch( &CHL1CrossbowBolt::BoltTouch );

	SetThink( &CHL1CrossbowBolt::BubbleThink );
	SetNextThink( gpGlobals->curtime + 0.2 );

    m_bExplode = true;
}


void CHL1CrossbowBolt::Precache( )
{
	PrecacheModel( BOLT_MODEL );
	PrecacheScriptSound( "BaseGrenade.Explode" );    
}

void CHL1CrossbowBolt::BoltTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsSolid() || pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS) )
		return;

	SetTouch( NULL );
	SetThink( NULL );

	if ( pOther->m_takedamage != DAMAGE_NO )
	{
		trace_t tr, tr2;
		tr = BaseClass::GetTouchTrace( );
		Vector	vecNormalizedVel = GetAbsVelocity();

		ClearMultiDamage( );
		VectorNormalize( vecNormalizedVel );

		if ( pOther->IsPlayer() )
		{
            float m_fDamage = sk_plr_dmg_xbow_bolt_plr.GetFloat() * g_pGameRules->GetDamageMultiplier();

            // If multiplayer sniper bolt, multiply damage by 4
            if ( g_pGameRules->IsMultiplayer() && !m_bExplode )
                m_fDamage *= 4;
            
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), m_fDamage,DMG_NEVERGIB );
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
		}
		else
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), sk_plr_dmg_xbow_bolt_npc.GetFloat() * g_pGameRules->GetDamageMultiplier(), DMG_BULLET | DMG_NEVERGIB );
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
		}

		ApplyMultiDamage();

		SetAbsVelocity( Vector( 0, 0, 0 ) );

		// play body "thwack" sound
		EmitSound( "Weapon_Crossbow.BoltHitBody" );

		Vector vForward;

		AngleVectors( GetAbsAngles(), &vForward );
		VectorNormalize ( vForward );
		
		UTIL_TraceLine( GetAbsOrigin(),	GetAbsOrigin() + vForward * 128, MASK_OPAQUE, pOther, COLLISION_GROUP_NONE, &tr2 );

		if ( tr2.fraction != 1.0f )
		{
//			NDebugOverlay::Box( tr2.endpos, Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 255, 0, 0, 10 );
//			NDebugOverlay::Box( GetAbsOrigin(), Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 0, 255, 0, 10 );

			CEffectData	data;

			data.m_vOrigin = tr2.endpos;
			data.m_vNormal = vForward;
			data.m_nEntIndex = tr2.fraction != 1.0f;
		
			DispatchEffect( "BoltImpact", data );
		}

        //		if ( !g_pGameRules->IsMultiplayer() )
        if ( g_pGameRules->IsMultiplayer() && !m_bExplode )
		{
			UTIL_Remove( this );
		}
	}
	else
	{
		EmitSound( "Weapon_Crossbow.BoltHitWorld" );

		SetThink( &CHL1CrossbowBolt::SUB_Remove );
		SetNextThink( gpGlobals->curtime );// this will get changed below if the bolt is allowed to stick in what it hit.

		if ( m_bExplode == false )
		{
			Vector vForward;
			AngleVectors( GetAbsAngles(), &vForward );
			VectorNormalize ( vForward );

			CEffectData	data;

			data.m_vOrigin = GetAbsOrigin();
			data.m_vNormal = vForward;
			data.m_nEntIndex = 0;

			DispatchEffect( "BoltImpact", data );
		}

		if (  UTIL_PointContents( GetAbsOrigin() ) != CONTENTS_WATER)
		{
			g_pEffects->Sparks( GetAbsOrigin() );
		}
	}

    // Set up an explosion in one tenth of a second
	if ( g_pGameRules->IsMultiplayer() && m_bExplode )
	{
        SetThink( &CHL1CrossbowBolt::ExplodeThink );
        SetNextThink( gpGlobals->curtime + 0.1f );
	}
}

void CHL1CrossbowBolt::ExplodeThink( void )
{
    //    int iContents = UTIL_PointContents( pev->origin );
    CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), sk_plr_dmg_xbow_bolt_npc.GetFloat() * g_pGameRules->GetDamageMultiplier(), DMG_BLAST );

    ::RadiusDamage( dmgInfo, GetAbsOrigin(), 128, CLASS_NONE, NULL );

#if !defined( CLIENT_DLL )
    CPASFilter filter( GetAbsOrigin() );

    te->Explosion( filter,                /* filter */
                   0.0,                   /* delay  */
                   &GetAbsOrigin(),       /* pos    */
                   g_sModelIndexFireball, /* modelindex */
                   0.2,                   /* scale  */
                   25,                    /* framerate */
                   TE_EXPLFLAG_NONE,      /* flags */
                   128,                   /* radius */
                   64,                    /* magnitude */
                   NULL,                  /* normal */
                   'C' );                 /* materialType */

	//CSoundEnt::InsertSound ( SOUND_COMBAT, GetAbsOrigin(), BASEGRENADE_EXPLOSION_VOLUME, 3.0 );
	EmitSound( "BaseGrenade.Explode" );    
#endif

    
    SetThink( &CHL1CrossbowBolt::SUB_Remove );
    SetNextThink( gpGlobals->curtime );
    
    AddEffects( EF_NODRAW );
}

void CHL1CrossbowBolt::BubbleThink( void )
{
	QAngle angNewAngles;

	VectorAngles( GetAbsVelocity(), angNewAngles );
	SetAbsAngles( angNewAngles );

	SetNextThink( gpGlobals->curtime + 0.1f );

	if ( GetWaterLevel()  == 0 )
		return;

	UTIL_BubbleTrail( GetAbsOrigin() - GetAbsVelocity() * 0.1, GetAbsOrigin(), 1 );
}

#endif	// HL1CrossbowBolt

IMPLEMENT_NETWORKCLASS_ALIASED( HL1WeaponCrossbow, DT_HL1WeaponCrossbow );

BEGIN_NETWORK_TABLE( CHL1WeaponCrossbow, DT_HL1WeaponCrossbow )
#ifdef CLIENT_DLL
	RecvPropBool( RECVINFO( m_fInZoom ) ),
#else
	SendPropBool( SENDINFO( m_fInZoom ) ),
#endif
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CHL1WeaponCrossbow )
#ifdef CLIENT_DLL
	DEFINE_PRED_FIELD( m_fInZoom, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
#endif
END_PREDICTION_DATA()

//INK_ENTITY_TO_CLASS( hl1_crossbow, CHL1WeaponCrossbow );
//PRECACHE_WEAPON_REGISTER( hl1_crossbow );

//IMPLEMENT_SERVERCLASS_ST( CHL1WeaponCrossbow, DT_HL1WeaponCrossbow )
//END_SEND_TABLE()

BEGIN_DATADESC( CHL1WeaponCrossbow )
END_DATADESC()

#if 0
#ifndef CLIENT_DLL

acttable_t	CHL1WeaponCrossbow::m_acttable[] = 
{
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_CROSSBOW,					false },
//	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_CROSSBOW,						false },
//	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_CROSSBOW,				false },
//	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_CROSSBOW,				false },
//	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW,	false },
//	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_CROSSBOW,			false },
//	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_CROSSBOW,					false },
};

IMPLEMENT_ACTTABLE(CHL1WeaponCrossbow);

#endif
#endif

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHL1WeaponCrossbow::CHL1WeaponCrossbow( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= true;
	m_fInZoom			= false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHL1WeaponCrossbow::Precache( void )
{
#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "crossbow_dart" );
#endif

	PrecacheScriptSound( "Weapon_Crossbow.BoltHitBody" );
	PrecacheScriptSound( "Weapon_Crossbow.BoltHitWorld" );

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CHL1WeaponCrossbow::PrimaryAttack( void )
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
void CHL1WeaponCrossbow::SecondaryAttack( void )
{
	ToggleZoom();
	m_flNextSecondaryAttack = gpGlobals->curtime + 1.0;
}


void CHL1WeaponCrossbow::FireBolt( void )
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


bool CHL1WeaponCrossbow::Reload( void )
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


void CHL1WeaponCrossbow::WeaponIdle( void )
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


bool CHL1WeaponCrossbow::Deploy( void )
{
	if ( m_iClip1 <= 0 )
	{
		return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), ACT_CROSSBOW_DRAW_UNLOADED, (char*)GetAnimPrefix() );
	}

	return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), ACT_VM_DRAW, (char*)GetAnimPrefix() );
}


bool CHL1WeaponCrossbow::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	if ( m_fInZoom )
	{
		ToggleZoom();
	}

	return BaseClass::Holster( pSwitchingTo );
}


void CHL1WeaponCrossbow::ToggleZoom( void )
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
