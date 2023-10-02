//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Configuration utility
//
//===========================================================================//

#include <windows.h>
#include <io.h>
#include <stdio.h>


#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>

#include "appframework/tier3app.h"
#include "tier0/icommandline.h"
#include "inputsystem/iinputsystem.h"
#include "filesystem_init.h"
#include "RootPanel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

#define QCGENERATOR_MAIN_PATH_ID	"MAIN"
#define QCGENERATOR_WRITE_PATH     "DEFAULT_WRITE_PATH"

RootPanel* g_pMainFrame = 0;
CHttpDownloader* g_pAPIDownloader = 0;
UpdaterDownloadHandler* g_pUpdateDownloadHandler = 0;
CHttpDownloader* g_pFileDownloader = 0;
GameDownloadHandler* g_pGameDownloadHandler = 0;

// Dummy window
static WNDCLASS staticWndclass = { NULL };
static ATOM staticWndclassAtom = 0;
static HWND staticHwnd = 0;

// List of our game configs, as read from the gameconfig.txt file
//HANDLE g_dwChangeHandle = NULL;

char pszPath[MAX_PATH];
char pszScene[MAX_PATH];

//-----------------------------------------------------------------------------
// Purpose: Copy a string into a CUtlVector of characters
//-----------------------------------------------------------------------------
void UtlStrcpy(CUtlVector<char>& dest, const char* pSrc)
{
	dest.EnsureCount((int)(strlen(pSrc) + 1));
	Q_strncpy(dest.Base(), pSrc, dest.Count());
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : const char
//-----------------------------------------------------------------------------
const char* GetBaseDirectory(void)
{
	static char path[MAX_PATH] = { 0 };
	if (path[0] == 0)
	{
		GetModuleFileName((HMODULE)GetAppInstance(), path, sizeof(path));
		Q_StripLastDir(path, sizeof(path));	// Get rid of the filename.
		Q_StripTrailingSlash(path);
	}
	return path;
}


void InitalizeDownloader(void)
{
	g_pUpdateDownloadHandler = new UpdaterDownloadHandler();
	g_pAPIDownloader = new CHttpDownloader(g_pUpdateDownloadHandler);
	g_pGameDownloadHandler = new GameDownloadHandler();
	g_pFileDownloader = new CHttpDownloader(g_pGameDownloadHandler);
}

//-----------------------------------------------------------------------------
// Purpose: Setup all our VGUI info
//-----------------------------------------------------------------------------
void InitializeVGUI(void)
{
	vgui::ivgui()->SetSleep(false);
	vgui::scheme()->SetTheme("themes/defaulttheme.thm");
	// Init the surface
	vgui::Panel* pPanel = new vgui::Panel(NULL, "TopPanel");
	pPanel->SetVisible(true);

	vgui::surface()->SetEmbeddedPanel(pPanel->GetVPanel());

	// load the scheme
	vgui::scheme()->LoadSchemeFromFile("resource/sourcescheme.res", NULL);

	// localization
	g_pVGuiLocalize->AddFile("resource/platform_%language%.txt");
	g_pVGuiLocalize->AddFile("resource/vgui_%language%.txt");
	//g_pVGuiLocalize->AddFile("QCGenerator_english.txt");

	// Start vgui
	vgui::ivgui()->Start();

	// add our main window
	g_pMainFrame = new RootPanel(pPanel, "RootPanel");

	// show main window
	g_pMainFrame->MoveToCenterOfScreen();
	g_pMainFrame->Activate();
	g_pMainFrame->SetSizeable(true);
	g_pMainFrame->SetMenuButtonVisible(true);
}

//-----------------------------------------------------------------------------
// Purpose: Stop VGUI
//-----------------------------------------------------------------------------
void ShutdownVGUI(void)
{
	delete g_pMainFrame;
}


//-----------------------------------------------------------------------------
// The application object
//-----------------------------------------------------------------------------
class ExampleApp : public CVguiSteamApp
{
	typedef CVguiSteamApp BaseClass;

public:
	// Methods of IApplication
	virtual bool Create();
	virtual bool PreInit();
	virtual int Main();
	virtual void PostShutdown();
	virtual void Destroy() {}
};

//int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
static ExampleApp __s_ApplicationObject; 
static CSteamApplication __s_SteamApplicationObject(&__s_ApplicationObject); 
DLL_EXPORT int UpdaterMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return AppMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow, &__s_SteamApplicationObject);
};


//-----------------------------------------------------------------------------
// The application object
//-----------------------------------------------------------------------------
bool ExampleApp::Create()
{
	AppSystemInfo_t appSystems[] =
	{
		{ "inputsystem.dll",		INPUTSYSTEM_INTERFACE_VERSION },
		{ "vgui2.dll",				VGUI_IVGUI_INTERFACE_VERSION },
		{ "", "" }	// Required to terminate the list
	};

	return AddSystems(appSystems);
}


//-----------------------------------------------------------------------------
// Purpose: Entry point
//-----------------------------------------------------------------------------
bool ExampleApp::PreInit()
{
	if (!BaseClass::PreInit())
		return false;

	FileSystem_SetErrorMode(FS_ERRORMODE_AUTO);

	// We only want to use the gameinfo.txt that is in the bin\vconfig directory.
	/*
	char dirName[MAX_PATH];
	Q_strncpy(dirName, GetBaseDirectory(), sizeof(dirName));
	Q_AppendSlash( dirName, sizeof( dirName ) );
	Q_strncat(dirName, "updater_win64", sizeof(dirName), COPY_ALL_CHARACTERS);

	if (!BaseClass::SetupSearchPaths(dirName, true, true))
	{
		::MessageBox(NULL, "Error", "Unable to initialize file system\n", MB_OK);
		return false;
	}
	*/
	// the "base dir" so we can scan mod name
	g_pFullFileSystem->AddSearchPath(GetBaseDirectory(), QCGENERATOR_MAIN_PATH_ID);

	// the main platform dir
	g_pFullFileSystem->AddSearchPath("platform", "PLATFORM", PATH_ADD_TO_HEAD);
	g_pFullFileSystem->AddSearchPath(".\\updater_" DEST_OS, "GAME", PATH_ADD_TO_HEAD);
	//g_pFullFileSystem->AddSearchPath( ".\\QCGenerator\\", QCGENERATOR_WRITE_PATH, PATH_ADD_TO_HEAD );

	return true;
}


void ExampleApp::PostShutdown()
{
	BaseClass::PostShutdown();
}


//-----------------------------------------------------------------------------
// Purpose: Entry point
//-----------------------------------------------------------------------------
int ExampleApp::Main()
{
	InitalizeDownloader();

	// Run app frame loop
	InitializeVGUI();

	// Run the app
	while (vgui::ivgui()->IsRunning())
	{
		Sleep(10);
		g_pAPIDownloader->Think();
		g_pFileDownloader->Think();
		vgui::ivgui()->RunFrame();
	}

	ShutdownVGUI();
	g_pAPIDownloader->AbortDownloadAndCleanup();
	delete g_pAPIDownloader;
	return 1;
}
