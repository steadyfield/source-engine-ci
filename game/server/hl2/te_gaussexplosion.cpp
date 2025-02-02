//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "te_particlesystem.h"
#include "weapon_gauss.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CTEGaussExplosion::CTEGaussExplosion( const char *name ) : BaseClass( name )
{
	m_nType = 0;
	m_vecDirection.Init();
}

CTEGaussExplosion::~CTEGaussExplosion( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : msg_dest - 
//			delay - 
//			*origin - 
//			*recipient - 
//-----------------------------------------------------------------------------
void CTEGaussExplosion::Create( IRecipientFilter& filter, float delay )
{
	engine->PlaybackTempEntity( filter, delay, (void *)this, GetServerClass()->m_pTable, GetServerClass()->m_ClassID );
}

IMPLEMENT_SERVERCLASS_ST( CTEGaussExplosion, DT_TEGaussExplosion )
	SendPropInt( SENDINFO(m_nType), 2, SPROP_UNSIGNED ),
	SendPropVector( SENDINFO(m_vecDirection), -1, SPROP_COORD ),
END_SEND_TABLE()

static CTEGaussExplosion g_TEGaussExplosion( "GaussExplosion" );

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &pos - 
//			&angles - 
//-----------------------------------------------------------------------------
void TE_GaussExplosion( IRecipientFilter& filter, float delay,
	const Vector &pos, const Vector &dir, int type )
{
	g_TEGaussExplosion.m_vecOrigin		= pos;
	g_TEGaussExplosion.m_vecDirection	= dir;
	g_TEGaussExplosion.m_nType			= type;

	//Send it
	g_TEGaussExplosion.Create( filter, delay );
}



