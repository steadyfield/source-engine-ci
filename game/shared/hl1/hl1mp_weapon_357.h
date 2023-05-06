#ifndef HL1Weapon_357_H
#define HL1Weapon_357_H

#include "hl1mp_basecombatweapon_shared.h"

#ifdef CLIENT_DLL
#define CHL1Weapon357 C_HL1Weapon357
#endif

//-----------------------------------------------------------------------------
// CHL1Weapon357
//-----------------------------------------------------------------------------

class CHL1Weapon357 : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CHL1Weapon357, CBaseHL1CombatWeapon );
public:
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CHL1Weapon357( void );

	void	Precache( void );
	bool	Deploy( void );
	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	bool	Reload( void );
	void	WeaponIdle( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

//	DECLARE_SERVERCLASS();
//	DECLARE_DATADESC();

private:
	void	ToggleZoom( void );

private:
	CNetworkVar( float, m_fInZoom );
};

#endif