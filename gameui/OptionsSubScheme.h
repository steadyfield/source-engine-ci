//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONS_SUB_SCHEME_H
#define OPTIONS_SUB_SCHEME_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyPage.h>


namespace vgui
{
	class Label;
	class Panel;
}

//-----------------------------------------------------------------------------
// Purpose: Mouse Details, Part of OptionsDialog
//-----------------------------------------------------------------------------
class COptionsSubScheme : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE( COptionsSubScheme, vgui::PropertyPage );

public:
	COptionsSubScheme(vgui::Panel *parent);
	~COptionsSubScheme();

	virtual void OnResetData();
	virtual void OnApplyChanges();

protected:
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	virtual void OnPageShow();
	virtual void OnPageHide();
};



#endif // OPTIONS_SUB_MOUSE_H