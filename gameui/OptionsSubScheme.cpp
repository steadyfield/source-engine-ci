//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "OptionsSubScheme.h"

#include <vgui/IScheme.h>
#include <stdio.h>
#include "vgui_schemeeditor.h"
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

static CSchemeEditor* g_pSchemeEditor = NULL;

COptionsSubScheme::COptionsSubScheme(vgui::Panel *parent) : PropertyPage(parent, NULL)
{
	g_pSchemeEditor = vgui::SETUP_PANEL(new CSchemeEditor(this));
	g_pSchemeEditor->InvalidateLayout(false, true);
	g_pSchemeEditor->SetCloseButtonVisible(false);

	LoadControlSettings("Resource\\OptionsSubScheme.res");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
COptionsSubScheme::~COptionsSubScheme()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubScheme::OnResetData()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubScheme::OnApplyChanges()
{
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void COptionsSubScheme::ApplySchemeSettings(IScheme *pScheme)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubScheme::OnPageShow()
{
	g_pSchemeEditor->SetVisible(true);
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubScheme::OnPageHide()
{
	g_pSchemeEditor->SetVisible(false);
}