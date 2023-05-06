//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef HL2MP_WEAPON_hands_H
#define HL2MP_WEAPON_hands_H
#pragma once

#ifdef CLIENT_DLL
#define CWeaponhands C_Weaponhands
#include "basehlcombatweapon_shared.h"
#else
#include "basebludgeonweapon.h"
#endif

//-----------------------------------------------------------------------------
// CWeaponhands
//-----------------------------------------------------------------------------
#ifdef CLIENT_DLL
class CWeaponhands : public CBaseHLCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponhands, CBaseHLCombatWeapon );
#else
class CWeaponhands : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS(CWeaponhands, CBaseHLBludgeonWeapon);
#endif

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	CWeaponhands();

	float		GetRange( void );
	float		GetFireRate( void );

	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );
	void		SecondaryAttack( void )	{	return;	}

	void		Drop( const Vector &vecVelocity );

	CWeaponhands( const CWeaponhands & ); 

	// Animation event
#ifndef CLIENT_DLL
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	virtual int WeaponMeleeAttack1Condition( float flDot, float flDist ); 
private: 
	// Animation event handlers 
	void HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
#endif

};


#endif // HL2MP_WEAPON_hands_H

