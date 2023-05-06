#ifndef WEAPON_GAUSSHD_H
#define WEAPON_GAUSSHD_H
#ifdef _WIN32
#pragma once
#endif

#include "hl1mp_basecombatweapon_shared.h"

#ifdef CLIENT_DLL
#define CWeaponGaussHD C_WeaponGaussHD
#endif

//-----------------------------------------------------------------------------
// CWeaponGaussHD
//-----------------------------------------------------------------------------


class CWeaponGaussHD : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CWeaponGaussHD, CBaseHL1MPCombatWeapon );
public:

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponGaussHD( void );

	void	Precache( void );
	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	void	WeaponIdle( void );
	void	AddViewKick( void );
	bool	Deploy( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

//	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

private:
	void	StopSpinSound( void );
	float	GetFullChargeTime( void );
	void	StartFire( void );
	void	Fire( Vector vecOrigSrc, Vector vecDir, float flDamage );

private:
//	int			m_nAttackState;
//	bool		m_bPrimaryFire;
	CNetworkVar( int, m_nAttackState);
	CNetworkVar( bool, m_bPrimaryFire);

	CSoundPatch	*m_sndCharge;
};

#endif