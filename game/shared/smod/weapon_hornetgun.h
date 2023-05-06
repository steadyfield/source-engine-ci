#ifndef WEAPON_HORNETGUNHD_H
#define WEAPON_HORNETGUNHD_H
#ifdef _WIN32
#pragma once
#endif

#include "hl1mp_basecombatweapon_shared.h"

//-----------------------------------------------------------------------------
// CWeaponHgunHD
//-----------------------------------------------------------------------------

#ifdef CLIENT_DLL
#define CWeaponHgunHD C_WeaponHgunHD
#endif

class CWeaponHgunHD : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CWeaponHgunHD, CBaseHL1MPCombatWeapon );
public:

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponHgunHD( void );

	void	Precache( void );
	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	void	WeaponIdle( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	bool	Reload( void );

	virtual void ItemPostFrame( void );

//	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

private:

//	float	m_flRechargeTime;
//	int		m_iFirePhase;

	CNetworkVar( float,	m_flRechargeTime );
	CNetworkVar( int,	m_iFirePhase );
};

#endif