//========= Made by Wineglass Stuidos, Some rights reserved. ============//
//
// Purpose:		Viewmodel skin handling
//
//=============================================================================//
#include "cbase.h"
#include "materialsystem/imaterialproxy.h"
#include "materialsystem/imaterialvar.h"
#include "materialsystem/itexture.h"
#include "c_basehlplayer.h"
#include "hl2_gamerules.h"
#include "proxyentity.h"
#include "iclientrenderable.h"
#include "toolframework_client.h"
#include "c_team.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================
//
// Team Material Proxy
//
// Handles changing team color (skins).
//
class CTeamMaterialProxy : public CEntityMaterialProxy
{
public:

	CTeamMaterialProxy();
	virtual ~CTeamMaterialProxy();
	virtual bool Init( IMaterial *pMaterial, KeyValues* pKeyValues );
	virtual void OnBind( C_BaseEntity *pEnt );
	virtual IMaterial *GetMaterial();

private:

	IMaterialVar* m_FrameVar;
};

//-----------------------------------------------------------------------------
// Purpose: Constructor.
//-----------------------------------------------------------------------------
CTeamMaterialProxy::CTeamMaterialProxy()
{
	m_FrameVar = 0;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor.
//-----------------------------------------------------------------------------
CTeamMaterialProxy::~CTeamMaterialProxy()
{
}

//-----------------------------------------------------------------------------
// Purpose: Initialization.
//-----------------------------------------------------------------------------
bool CTeamMaterialProxy::Init( IMaterial *pMaterial, KeyValues* pKeyValues )
{
	bool foundVar;
	m_FrameVar = pMaterial->FindVar( "$frame", &foundVar, false );
	if( !foundVar )
	{
		m_FrameVar = 0;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Set the appropriate texture (in the animated texture).
//-----------------------------------------------------------------------------
void CTeamMaterialProxy::OnBind( C_BaseEntity *pEnt )
{
	//broken, return for now
	return;

	if( !m_FrameVar )
		return;

	int team = pEnt->GetRenderTeamNumber();
	team -= 2;

	// Use that as an animated frame number
	m_FrameVar->SetIntValue( team );
}

IMaterial *CTeamMaterialProxy::GetMaterial()
{
	if ( !m_FrameVar )
		return NULL;

	return m_FrameVar->GetOwningMaterial();
}

EXPOSE_INTERFACE( CTeamMaterialProxy, IMaterialProxy, "HandsSkin" IMATERIAL_PROXY_INTERFACE_VERSION );
