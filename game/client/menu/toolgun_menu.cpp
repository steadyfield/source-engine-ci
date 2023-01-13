#include "cbase.h"
#include "toolgun_menu.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>

class CToolMenu : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CToolMenu, vgui::Frame);

	CToolMenu(vgui::VPANEL parent);
	~CToolMenu(){};
protected:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);
};

CToolMenu::CToolMenu(vgui::VPANEL parent)
	: BaseClass(NULL, "ToolMenu")
{
	SetParent(parent);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	SetProportional(true);

	int w = 500;
	int h = 500;
	if (IsProportional())
	{
		w = scheme()->GetProportionalScaledValueEx(GetScheme(), w);
		h = scheme()->GetProportionalScaledValueEx(GetScheme(), h);
	}
	SetSize(w, h);

	SetTitleBarVisible(true);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(false);
	SetSizeable(false);
	SetMoveable(false);
	SetVisible(true);
	SetTitle("ToolGun Menu", this);

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	LoadControlSettings("resource/ui/toolmenu.res");

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
}

class CToolMenuInterface : public ToolMenu
{
private:
	CToolMenu *ToolMenu;
public:
	CToolMenuInterface()
	{
		ToolMenu = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		ToolMenu = new CToolMenu(parent);
	}
	void Destroy()
	{
		if (ToolMenu)
		{
			ToolMenu->SetParent((vgui::Panel *)NULL);
			delete ToolMenu;
		}
	}
	void Activate(void)
	{
		if (ToolMenu)
		{
			ToolMenu->Activate();
		}
	}
};
static CToolMenuInterface g_ToolMenu;
ToolMenu* toolmenu = (ToolMenu*)&g_ToolMenu;

ConVar cl_toolmenu("toolmenu", "0", FCVAR_CLIENTDLL, "ToolGun Menu");

void CToolMenu::OnTick()
{
	BaseClass::OnTick();
	SetVisible(cl_toolmenu.GetBool());
}

void CToolMenu::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);

	if (!Q_stricmp(pcCommand, "turnoff"))	
	{
		cl_toolmenu.SetValue(0);
	}
	else if(!Q_stricmp(pcCommand, "remover"))
	{
		engine->ClientCmd("toolmode 0");
	}
	else if(!Q_stricmp(pcCommand, "setcolor"))
	{
		engine->ClientCmd("toolmode 1");
	}
	else if(!Q_stricmp(pcCommand, "modelscale"))
	{
		engine->ClientCmd("toolmode 2");
	}
	else if(!Q_stricmp(pcCommand, "igniter"))
	{
		engine->ClientCmd("toolmode 3");
	}
}