//========= Copyleft Wineglass Studios, some rights reserved. ============//
//
// Purpose: HL1 crossbow bolt
//
//=============================================================================//
#ifndef HL1Weapon_Crossbow_H
#define HL1Weapon_Crossbow_H

#include "hl1mp_basecombatweapon_shared.h"

#ifdef GAME_DLL
#define BOLT_AIR_VELOCITY	2000
#define BOLT_WATER_VELOCITY	1000

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
class CHL1CrossbowBolt : public CBaseCombatCharacter
{
	DECLARE_CLASS(CHL1CrossbowBolt, CBaseCombatCharacter);

public:
	CHL1CrossbowBolt()
	{
		m_bExplode = true;
	}

	Class_T Classify(void) { return CLASS_NONE; }

public:
	void Spawn(void);
	void Precache(void);
	void BubbleThink(void);
	void BoltTouch(CBaseEntity *pOther);
	void ExplodeThink(void);
	void SetExplode(bool bVal) { m_bExplode = bVal; }

	static CHL1CrossbowBolt *BoltCreate(const Vector &vecOrigin, const QAngle &angAngles, CBasePlayer *pentOwner = NULL);

	DECLARE_DATADESC();

private:
	bool m_bExplode;
};
#endif

//-----------------------------------------------------------------------------
// CHL1WeaponCrossbow
//-----------------------------------------------------------------------------

#ifdef CLIENT_DLL
#define CHL1WeaponCrossbow C_HL1WeaponCrossbow
#endif

class CHL1WeaponCrossbow : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CHL1WeaponCrossbow, CBaseHL1MPCombatWeapon );
public:

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CHL1WeaponCrossbow( void );

	void	Precache( void );
	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	bool	Reload( void );
	void	WeaponIdle( void );
	bool	Deploy( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

//	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

#ifndef CLIENT_DLL
//	DECLARE_ACTTABLE();
#endif

private:
	void	FireBolt( void );
	void	ToggleZoom( void );

private:
//	bool	m_fInZoom;
	CNetworkVar( bool, m_fInZoom );
};

#endif