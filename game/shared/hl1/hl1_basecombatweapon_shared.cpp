//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hl1_basecombatweapon_shared.h"

#ifdef CLIENT_DLL
#include "c_basehlplayer.h"
#else
#include "hl2_player.h"
#endif

LINK_ENTITY_TO_CLASS( basehl1combatweapon, CBaseHL1CombatWeapon );

IMPLEMENT_NETWORKCLASS_ALIASED( BaseHL1CombatWeapon , DT_BaseHL1CombatWeapon )

BEGIN_NETWORK_TABLE( CBaseHL1CombatWeapon , DT_BaseHL1CombatWeapon )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CBaseHL1CombatWeapon )
END_PREDICTION_DATA()


void CBaseHL1CombatWeapon::Spawn( void )
{
	Precache();

	SetSolid( SOLID_BBOX );
	m_flNextEmptySoundTime = 0.0f;

	// Weapons won't show up in trace calls if they are being carried...
	RemoveEFlags( EFL_USE_PARTITION_WHEN_NOT_SOLID );

	m_iState = WEAPON_NOT_CARRIED;
	// Assume 
	m_nViewModelIndex = 0;

	// If I use clips, set my clips to the default
	if ( UsesClipsForAmmo1() )
	{
		m_iClip1 = GetDefaultClip1();
	}
	else
	{
		SetPrimaryAmmoCount( GetDefaultClip1() );
		m_iClip1 = WEAPON_NOCLIP;
	}
	if ( UsesClipsForAmmo2() )
	{
		m_iClip2 = GetDefaultClip2();
	}
	else
	{
		SetSecondaryAmmoCount( GetDefaultClip2() );
		m_iClip2 = WEAPON_NOCLIP;
	}

	SetModel( GetWorldModel() );

#if !defined( CLIENT_DLL )
	FallInit();
	SetCollisionGroup( COLLISION_GROUP_WEAPON );

	m_takedamage = DAMAGE_EVENTS_ONLY;

	SetBlocksLOS( false );

	// Default to non-removeable, because we don't want the
	// game_weapon_manager entity to remove weapons that have
	// been hand-placed by level designers. We only want to remove
	// weapons that have been dropped by NPC's.
	SetRemoveable( false );
#endif

	//Make weapons easier to pick up in MP.
	if ( g_pGameRules->IsMultiplayer() )
	{
		CollisionProp()->UseTriggerBounds( true, 36 );
	}
	else
	{
		CollisionProp()->UseTriggerBounds( true, 24 );
	}

	// Use more efficient bbox culling on the client. Otherwise, it'll setup bones for most
	// characters even when they're not in the frustum.
	AddEffects( EF_BONEMERGE_FASTCULL );
}

#if !defined( CLIENT_DLL )

Vector CBaseHL1CombatWeapon::GetSoundEmissionOrigin() const
{
	if ( gpGlobals->maxClients == 1 || !GetOwner() )
		return CBaseCombatWeapon::GetSoundEmissionOrigin();

//	Vector vecOwner = GetOwner()->GetSoundEmissionOrigin();
//	Vector vecThis = WorldSpaceCenter();
//	DevMsg("SoundEmissionOrigin: Owner: %4.1f,%4.1f,%4.1f Default:%4.1f,%4.1f,%4.1f\n",
//			vecOwner.x, vecOwner.y, vecOwner.z,
//			vecThis.x, vecThis.y, vecThis.z );

	// TEMP fix for HL1MP... sounds are sometimes beeing emitted underneath the ground
	return GetOwner()->GetSoundEmissionOrigin();
}

#endif