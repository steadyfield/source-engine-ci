//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Dialog for selecting game configurations
//
//=====================================================================================//

#include <windows.h>

#include <vgui/IVGui.h>
#include <vgui/IInput.h>
#include <vgui/ISystem.h>
#include <vgui_controls/MessageBox.h>


#include "RootPanel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

RootPanel *g_pCQCGenMain = NULL;

class CModalPreserveMessageBox : public vgui::MessageBox
{
public:
	CModalPreserveMessageBox(const char *title, const char *text, vgui::Panel *parent)
		: vgui::MessageBox( title, text, parent )
	{
		m_PrevAppFocusPanel = vgui::input()->GetAppModalSurface();
	}

	~CModalPreserveMessageBox()
	{
		vgui::input()->SetAppModalSurface( m_PrevAppFocusPanel );
	}

public:
	vgui::VPANEL	m_PrevAppFocusPanel;
};
		
//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
RootPanel::RootPanel( Panel *parent, const char *name ) : BaseClass( parent, name )
{
	Assert( !g_pCQCGenMain );
	g_pCQCGenMain = this;
	
	SetMinimizeButtonVisible( true );	

	SetSize( 300, 300 );
	SetMinimumSize(100, 100);	
	SetTitle( "Updater" , true);
	
}

//-----------------------------------------------------------------------------
// Destructor 
//-----------------------------------------------------------------------------
RootPanel::~RootPanel()
{
	g_pCQCGenMain = NULL;
}



//-----------------------------------------------------------------------------
// Purpose: Kills the whole app on close
//-----------------------------------------------------------------------------
void RootPanel::OnClose( void )
{
	BaseClass::OnClose();
	ivgui()->Stop();	
}

//-----------------------------------------------------------------------------
// Purpose: Parse commands coming in from the VGUI dialog
//-----------------------------------------------------------------------------
void RootPanel::OnCommand( const char *command )
{		
	BaseClass::OnCommand( command );
}

