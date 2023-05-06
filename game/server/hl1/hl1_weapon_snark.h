#ifndef HL1Weapon_Snark_H
#define HL1Weapon_Snark_H

#include "hl1mp_basecombatweapon_shared.h"

class CWeaponSnark : public CBaseHL1CombatWeapon
{
	DECLARE_CLASS( CWeaponSnark, CBaseHL1CombatWeapon );
public:

	CWeaponSnark( void );

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