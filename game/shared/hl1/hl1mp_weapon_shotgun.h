#ifndef HL1Weapon_Shotgun_H
#define HL1Weapon_Shotgun_H

#include "hl1mp_basecombatweapon_shared.h"

#ifdef CLIENT_DLL
#define CHL1WeaponShotgun C_HL1WeaponShotgun
#endif

class CHL1WeaponShotgun : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CHL1WeaponShotgun, CBaseHL1MPCombatWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

private:
//	float	m_flPumpTime;
//	int		m_fInSpecialReload;

	CNetworkVar( float, m_flPumpTime);
	CNetworkVar( int, m_fInSpecialReload );

public:
	void	Precache( void );

	bool Reload( void );
	void FillClip( void );
	void WeaponIdle( void );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	void DryFire( void );

//	DECLARE_SERVERCLASS();
//	DECLARE_DATADESC();

	CHL1WeaponShotgun(void);

//#ifndef CLIENT_DLL
//	DECLARE_ACTTABLE();
//#endif
};

#endif