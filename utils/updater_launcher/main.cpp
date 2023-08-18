#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <direct.h>
#endif
#include "basetypes.h"
#include "platform.h"


typedef int (*UpdaterMain_t)(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow);

static char* GetBaseDir(const char* pszBuffer)
{
	static char	basedir[MAX_PATH];
	char szBuffer[MAX_PATH];
	size_t j;
	char* pBuffer = NULL;

	strcpy(szBuffer, pszBuffer);

	pBuffer = strrchr(szBuffer, '\\');
	if (pBuffer)
	{
		*(pBuffer + 1) = '\0';
	}

	strcpy(basedir, szBuffer);

	j = strlen(basedir);
	if (j > 0)
	{
		if ((basedir[j - 1] == '\\') ||
			(basedir[j - 1] == '/'))
		{
			basedir[j - 1] = 0;
		}
	}

	return basedir;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Must add 'bin' to the path....
	char* pPath = getenv("PATH");

	// Use the .EXE name to determine the root directory
	char moduleName[MAX_PATH];
	char szBuffer[4096];
	if (!GetModuleFileName(hInstance, moduleName, MAX_PATH))
	{
		MessageBox(0, "Failed calling GetModuleFileName", "Updater Error", MB_OK);
		return 0;
	}

	// Get the root directory the .exe is in
	char* pRootDir = GetBaseDir(moduleName);

#ifdef _DEBUG
	int len =
#endif
		_snprintf(szBuffer, sizeof(szBuffer) - 1, "PATH=%s\\updater_" DEST_OS "\\;%s", pRootDir, pPath);
	szBuffer[ARRAYSIZE(szBuffer) - 1] = 0;
	assert(len < 4096);
	_putenv(szBuffer);

	HINSTANCE launcher = LoadLibrary("updater_" DEST_OS "\\libupdater.dll");
	if (!launcher)
	{
		char* pszError;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pszError, 0, NULL);

		char szBuf[1024];
		_snprintf(szBuf, sizeof(szBuf) - 1, "Failed to load the updater DLL:\n\n%s", pszError);
		szBuf[ARRAYSIZE(szBuf) - 1] = 0;
		MessageBox(0, szBuf, "Updater Error", MB_OK);

		LocalFree(pszError);
		return 0;
	}

	UpdaterMain_t main = (UpdaterMain_t)GetProcAddress(launcher, "UpdaterMain");
	return main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}