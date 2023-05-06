#ifndef HL1Weapon_Hornetgun_H
#define HL1Weapon_Hornetgun_H

#include "hl1mp_basecombatweapon_shared.h"

//-----------------------------------------------------------------------------
// CWeaponHgun
//-----------------------------------------------------------------------------

#ifdef CLIENT_DLL
#define CWeaponHgun C_WeaponHgun
#endif

class CWeaponHgun : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CWeaponHgun, CBaseHL1MPCombatWeapon );
public:

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponHgun( void );

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