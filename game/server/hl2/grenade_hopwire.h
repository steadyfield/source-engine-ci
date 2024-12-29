//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
//=============================================================================

#ifndef GRENADE_HOPWIRE_H
#define GRENADE_HOPWIRE_H
#ifdef _WIN32
#pragma once
#endif

#include "basegrenade_shared.h"
#include "beam_shared.h"
#include "rope.h"
#include "Sprite.h"

#define MAX_HOOKS			8
#define HOPWIRE_DAMAGE		100
#define HOPWIRE_DMG_RADIUS	128

#define TETHERHOOK_MODEL	"models/Weapons/w_hopwire.mdl"

class CGrenadeHopWire;

//-----------------------------------------------------------------------------
// Tether hook
//-----------------------------------------------------------------------------

class CTetherHook : public CBaseAnimating
{
	DECLARE_CLASS( CTetherHook, CBaseAnimating );
public:
	bool	CreateVPhysics( void );
	void 	Precache( void );
	void	Spawn( void );

	void	SetVelocity( const Vector &velocity, const AngularImpulse &angVelocity );
	void	StartTouch( CBaseEntity *pOther );

	static CTetherHook	*Create( const Vector &origin, const QAngle &angles, CGrenadeHopWire *pOwner );

	void	CreateRope( void );
	void	HookThink( void );

	void	RemoveThis( void );

	DECLARE_DATADESC();

private:
	CHandle<CGrenadeHopWire>	m_hTetheredOwner;
	IPhysicsSpring				*m_pSpring;
	CRopeKeyframe				*m_pRope;
	CSprite						*m_pGlow;
	CBeam						*m_pBeam;
	bool						m_bAttached;
};

class CGrenadeHopWire : public CBaseGrenade
{
	DECLARE_CLASS( CGrenadeHopWire, CBaseGrenade );
	DECLARE_DATADESC();

public:
	void	Spawn( void );
	void	Precache( void );
	bool	CreateVPhysics( void );
	void	SetTimer( float timer );
	void	SetVelocity( const Vector &velocity, const AngularImpulse &angVelocity );
	void	Detonate( void );

	void	TetherDetonate( void );
	void	Explode( trace_t *pTrace, int bitsDamageType );
	
	void	CombatThink( void );
	void	TetherThink( void );

protected:

	friend class CTetherHook;

	int			m_nHooksShot;
	CSprite		*m_pGlow;

	CTetherHook *pHooks[MAX_HOOKS];
};

extern CBaseGrenade *HopWire_Create( const Vector &position, const QAngle &angles, const Vector &velocity, const AngularImpulse &angVelocity, CBaseEntity *pOwner, float timer );

#endif // GRENADE_HOPWIRE_H
