#ifndef WEAPON_EGONHD_H
#define WEAPON_EGONHD_H
#ifdef _WIN32
#pragma once
#endif

#include "hl1mp_basecombatweapon_shared.h"
#include "Sprite.h"
#include "beam_shared.h"

//-----------------------------------------------------------------------------
// CWeaponEgonHD
//-----------------------------------------------------------------------------

#ifdef CLIENT_DLL
#define CWeaponEgonHD C_WeaponEgonHD
#endif

class CWeaponEgonHD : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CWeaponEgonHD, CBaseHL1MPCombatWeapon );
public:

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

    CWeaponEgonHD(void);

	virtual bool	Deploy( void );
	void	PrimaryAttack( void );
    virtual void    Precache( void );
    
	void	SecondaryAttack( void )
	{
		PrimaryAttack();
	}

	void	WeaponIdle( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

    //	DECLARE_SERVERCLASS();
    //	DECLARE_DATADESC();

private:
	bool	HasAmmo( void );
	void	UseAmmo( int count );
	void	Attack( void );
	void	EndAttack( void );
	void	Fire( const Vector &vecOrigSrc, const Vector &vecDir );
	void	UpdateEffect( const Vector &startPoint, const Vector &endPoint );
	void	CreateEffect( void );
	void	DestroyEffect( void );

	enum EGON_FIRESTATE { FIRE_OFF, FIRE_STARTUP, FIRE_CHARGE };
	EGON_FIRESTATE		m_fireState;
	float				m_flAmmoUseTime;	// since we use < 1 point of ammo per update, we subtract ammo on a timer.
	float				m_flShakeTime;
	float				m_flStartFireTime;
	float				m_flDmgTime;
	CHandle<CSprite>	m_hSprite;
	CHandle<CBeam>		m_hBeam;
	CHandle<CBeam>		m_hNoise;
};

#endif