//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef MDRIPPERMAIN_H
#define MDRIPPERMAIN_H
#ifdef _WIN32
#pragma once
#endif

#include "matsys_controls/QCGenerator.h"
#include <vgui_controls/Frame.h>
#include <FileSystem.h>
#include "vgui/IScheme.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Main dialog for media browser
//-----------------------------------------------------------------------------
class RootPanel : public Frame
{
	DECLARE_CLASS_SIMPLE(RootPanel, Frame );

public:

	RootPanel(Panel *parent, const char *name );
	virtual ~RootPanel();
		
protected:
	
	virtual void OnClose();
	virtual void OnCommand( const char *command );

private:

};


extern RootPanel* g_pRootPanel;


#endif // MDRIPPERMAIN_H
