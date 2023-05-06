//========= Copyright ?1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "basehlcombatweapon_shared.h"

#ifndef BASEHL1COMBATWEAPON_SHARED_H
#define BASEHL1COMBATWEAPON_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#if defined( CLIENT_DLL )
#define CBaseHL1CombatWeapon C_BaseHL1CombatWeapon
#endif

class CBaseHL1CombatWeapon : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CBaseHL1CombatWeapon, CBaseHLCombatWeapon );
public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

public:
	void Spawn( void );

public:
// Server Only Methods
#if !defined( CLIENT_DLL )
 //   DECLARE_DATADESC();
	virtual void Precache();

	void FallInit( void );						// prepare to fall to the ground
	void FallThink( void );						// make the weapon fall to the ground after spawning

	void EjectShell( CBaseEntity *pPlayer, int iType );

	Vector GetSoundEmissionOrigin() const;
//#else

//	virtual void	AddViewmodelBob( CBaseViewModel *viewmodel, Vector &origin, QAngle &angles );
//	virtual	float	CalcViewmodelBob( void );
	
#endif
};

#endif // BASEHL1COMBATWEAPON_SHARED_H
