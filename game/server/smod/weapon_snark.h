#ifndef WEAPON_SNARKHD_H
#define WEAPON_SNARKHD_H
#ifdef _WIN32
#pragma once
#endif

#include "hl1_basecombatweapon_shared.h"

class CWeaponSnarkHD : public CBaseHL1CombatWeapon
{
	DECLARE_CLASS( CWeaponSnarkHD, CBaseHL1CombatWeapon );
public:

	CWeaponSnarkHD( void );

	void	Precache( void );
	void	PrimaryAttack( void );
	void	WeaponIdle( void );
	bool	Deploy( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

protected:
	bool	m_bJustThrown;
};

#endif