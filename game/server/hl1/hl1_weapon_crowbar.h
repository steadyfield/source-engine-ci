#ifndef HL1Weapon_Crowbar_H
#define HL1Weapon_Crowbar_H

#include "hl1mp_basecombatweapon_shared.h"

#ifdef CLIENT_DLL
#define CHL1WeaponCrowbar C_HL1WeaponCrowbar
#endif

//-----------------------------------------------------------------------------
// CHL1WeaponCrowbar
//-----------------------------------------------------------------------------

class CHL1WeaponCrowbar : public CBaseHL1MPCombatWeapon
{
	DECLARE_CLASS( CHL1WeaponCrowbar, CBaseHL1MPCombatWeapon );
public:
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CHL1WeaponCrowbar();

	void			Precache( void );
	virtual void	ItemPostFrame( void );
	void			PrimaryAttack( void );

public:
	trace_t		m_traceHit;
	Activity	m_nHitActivity;

private:
	virtual void		Swing( void );
	virtual	void		Hit( void );
	virtual	void		ImpactEffect( void );
	void		ImpactSound( CBaseEntity *pHitEntity );
	virtual Activity	ChooseIntersectionPointAndActivity( trace_t &hitTrace, const Vector &mins, const Vector &maxs, CBasePlayer *pOwner );

public:

};

#endif