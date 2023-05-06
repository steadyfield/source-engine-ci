//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_ALYXGUN_H
#define WEAPON_ALYXGUN_H

#include "basehlcombatweapon.h"

#if defined( _WIN32 )
#pragma once
#endif

class CWeaponAlyxGun : public CHLSelectFireMachineGun
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CWeaponAlyxGun, CHLSelectFireMachineGun );

	CWeaponAlyxGun();
	~CWeaponAlyxGun();

	DECLARE_SERVERCLASS();
	
	void	Precache( void );

	virtual int		GetMinBurst( void ) { return 4; }
	virtual int		GetBurstSize( void ) { return 3; };
	virtual int		GetMaxBurst( void ) { return 7; }
	virtual float	GetMinRestTime( void );
	virtual float	GetMaxRestTime( void );
	virtual bool	Reload( void );
	void			PrimaryAttack( void );
	void			BasePrimaryAttack( void );
	void			SecondaryAttack( void );
	void			BurstThink( void );
	virtual void			WeaponIdle( void );						// called when no buttons pressed
	Activity	GetPrimaryAttackActivity( void );
	Activity	GetSecondaryAttackActivity( void );
	virtual Activity		GetDrawActivity( void );
	virtual bool	Deploy( void );

	virtual void Equip( CBaseCombatCharacter *pOwner );

	float	GetFireRate( void ) { return 0.1f; }
	int		CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	int		WeaponRangeAttack1Condition( float flDot, float flDist );
	int		WeaponRangeAttack2Condition( float flDot, float flDist );

	virtual const Vector& GetBulletSpread( void );

	void FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, bool bUseWeaponAngles );

	void Operator_ForceNPCFire( CBaseCombatCharacter  *pOperator, bool bSecondary );
	void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	/*virtual void SetPickupTouch( void )
	{
		// Alyx gun cannot be picked up
		SetTouch(NULL);
	}*/

	float m_flTooCloseTimer;
	bool m_bJustDrawn;

	DECLARE_ACTTABLE();
	
	bool m_bBurstFirstSound;

private:
	float	m_flSoonestPrimaryAttack;
	float	m_flLastAttackTime;
	float	m_flAccuracyPenalty;
	int		m_nNumShotsFired;

};

#endif // WEAPON_ALYXGUN_H
