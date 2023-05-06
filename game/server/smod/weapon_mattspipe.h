//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_MattsPipe_H
#define WEAPON_MattsPipe_H

#include "basebludgeonweapon.h"

#if defined( _WIN32 )
#pragma once
#endif

#define	MattsPipe_RANGE		100.0f
#define	MattsPipe_REFIRE	0.8f

//-----------------------------------------------------------------------------
// CMattsPipe
//-----------------------------------------------------------------------------

class CMattsPipe : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS( CMattsPipe, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	CMattsPipe();

	float		GetRange( void )		{	return	MattsPipe_RANGE;	}
	float		GetFireRate( void )		{	return	MattsPipe_REFIRE;	}

	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );

	virtual int WeaponMeleeAttack1Condition( float flDot, float flDist );
	void		SecondaryAttack( void )	{	return;	}

	// Animation event
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

private:
	// Animation event handlers
	void HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
};

#endif // WEAPON_MattsPipe_H
