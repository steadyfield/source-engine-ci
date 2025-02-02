//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "CreateMultiplayerGameDialog.h"
#include "CreateMultiplayerGameServerPage.h"
#include "CreateMultiplayerGameGameplayPage.h"
#include "CreateMultiplayerGameBotPage.h"

#include "EngineInterface.h"
#include "ModInfo.h"
#include "GameUI_Interface.h"

#include <stdio.h>

using namespace vgui;

#include "vgui_controls/ComboBox.h"
#include <vgui/ILocalize.h>

#include "filesystem.h"
#include <KeyValues.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CCreateMultiplayerGameDialog::CCreateMultiplayerGameDialog(vgui::Panel *parent) : PropertyDialog(parent, "CreateMultiplayerGameDialog")
{
	SetDeleteSelfOnClose(true);

	int w = 348;
	int h = 460;
	if (IsProportional())
	{
		w = scheme()->GetProportionalScaledValueEx(GetScheme(), w);
		h = scheme()->GetProportionalScaledValueEx(GetScheme(), h);
	}

	SetSize(w, h);
	
	SetTitle("#GameUI_CreateServer", true);
	SetOKButtonText("#GameUI_Start");

	m_pServerPage = new CCreateMultiplayerGameServerPage(this, "ServerPage");
	//m_pGameplayPage = new CCreateMultiplayerGameGameplayPage(this, "GameplayPage");

	AddPage(m_pServerPage, "#GameUI_Game");
	//AddPage(m_pGameplayPage, "#GameUI_Game");
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CCreateMultiplayerGameDialog::~CCreateMultiplayerGameDialog()
{
}

//-----------------------------------------------------------------------------
// Purpose: runs the server when the OK button is pressed
//-----------------------------------------------------------------------------
bool CCreateMultiplayerGameDialog::OnOK(bool applyOnly)
{
	BaseClass::OnOK(applyOnly);

	// get these values from m_pServerPage and store them temporarily
	char szMapName[64];
	strncpy(szMapName, m_pServerPage->GetMapName(), sizeof( szMapName ));

	char szMapCommand[1024];

	// create the command to execute
	Q_snprintf(szMapCommand, sizeof( szMapCommand ), "disconnect\nwait\nwait\nprogress_enable\nmap %s\n",
		szMapName
	);

	// exec
	engine->ClientCmd_Unrestricted(szMapCommand);

	return true;
}

void CCreateMultiplayerGameDialog::OnKeyCodePressed( vgui::KeyCode code )
{
	// Handle close here, CBasePanel parent doesn't support "DialogClosing" command
	ButtonCode_t nButtonCode = GetBaseButtonCode( code );

	if ( nButtonCode == KEY_XBUTTON_B || nButtonCode == STEAMCONTROLLER_B )
	{
		OnCommand( "Close" );
	}
	else if ( nButtonCode == KEY_XBUTTON_A || nButtonCode == STEAMCONTROLLER_A )
	{
		OnOK( false );
	}
	else if ( nButtonCode == KEY_XBUTTON_UP || 
			  nButtonCode == KEY_XSTICK1_UP ||
			  nButtonCode == KEY_XSTICK2_UP ||
			  nButtonCode == STEAMCONTROLLER_DPAD_UP ||
			  nButtonCode == KEY_UP )
	{
		int nItem = m_pServerPage->GetMapList()->GetActiveItem() - 1;
		if ( nItem < 0 )
		{
			nItem = m_pServerPage->GetMapList()->GetItemCount() - 1;
		}
		m_pServerPage->GetMapList()->ActivateItem( nItem );
	}
	else if ( nButtonCode == KEY_XBUTTON_DOWN || 
			  nButtonCode == KEY_XSTICK1_DOWN ||
			  nButtonCode == KEY_XSTICK2_DOWN || 
			  nButtonCode == STEAMCONTROLLER_DPAD_DOWN ||
			  nButtonCode == KEY_DOWN )
	{
		int nItem = m_pServerPage->GetMapList()->GetActiveItem() + 1;
		if ( nItem >= m_pServerPage->GetMapList()->GetItemCount() )
		{
			nItem = 0;
		}
		m_pServerPage->GetMapList()->ActivateItem( nItem );
	}
	else
	{
		BaseClass::OnKeyCodePressed( code );
	}
}