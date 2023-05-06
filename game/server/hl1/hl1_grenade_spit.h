//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Projectile shot by bullsquid 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef	HL1GrenadeSpit_H
#define	HL1GrenadeSpit_H

#include "hl1_basegrenade.h"

enum SpitSize_e
{
	SPIT_SMALL,
	SPIT_MEDIUM,
	SPIT_LARGE,
};

#define SPIT_GRAVITY 0.9

class CHL1GrenadeSpit : public CHL1BaseGrenade
{
public:
	DECLARE_CLASS( CHL1GrenadeSpit, CHL1BaseGrenade );

	void		Spawn( void );
	void		Precache( void );
	void		SpitThink( void );
	void 		HL1GrenadeSpitTouch( CBaseEntity *pOther );
	void		Event_Killed( const CTakeDamageInfo &info );
	void		SetSpitSize(int nSize);

	int			m_nSquidSpitSprite;
	float		m_fSpitDeathTime;		// If non-zero won't detonate

	void EXPORT				Detonate(void);
	CHL1GrenadeSpit(void);

	DECLARE_DATADESC();
};

#endif	//HL1GrenadeSpit_H
