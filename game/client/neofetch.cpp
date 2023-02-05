#include "cbase.h" 

//From tier0/cpu.cpp
const tchar* GetProcessorArchName()
{
#if defined( __x86_64__) || defined(_M_X64)
	return "amd64";
#elif defined(__i386__) || defined(_X86_) || defined(_M_IX86)
	return "i386";
#elif defined __aarch64__
        return "aarch64";
#elif defined __arm__ || defined _M_ARM
        return "arm";
#else
		return "Unknown architecture";
#endif
}

const tchar* GetPlatformName()
{
#ifdef LINUX	
	return "Linux";
#elif ANDROID
	 return "Android";
#elif PLATFORM_BSD
	return "*BSD";
#elif WIN32
	return "Windows";
#elif OSX
	return "OSX";
#endif
}

const tchar* GetGame()
{
#if defined(HL2_CLIENT_DLL) && !defined(CLIENT_GUNMOD_DLL)
	return "Half-Life 2 / Half-Life 2 Based Mod";
#elif defined(PORTAL_CLIENT_DLL) && !defined(CLIENT_GUNMOD_DLL)
	return "Portal / Portal Based Mod";
#elif defined(HL1_CLIENT_DLL) && !defined(CLIENT_GUNMOD_DLL)
	return "Half-Life 1 / Half-Life 1 Based Mod";
#elif defined(HL2MP) && !defined(CLIENT_GUNMOD_DLL)
	return "Half-Life 2 Deatmatch / Half-Life 2 Deathmatch Based Mod";
#elif defined ( CLIENT_GUNMOD_DLL )
	return "Gunmod";
#else
	return "Unknown Version";
#endif
}

const tchar* GetGunmodVersion()
{
#ifndef HL2MP
	return "Singeplayer, Version 1.16";
#else
	return "Multiplayer, Version 1.16";
#endif
}

void CC_Neofetch(void)
{
	/*Msg("@@@@@@&#BBGGGGGGGGGGGGGGGGGGGGGGGGG#&@@@\n"
		"@&BPY?77!!!!!!!!!!!!!!!!!!!!!!!!!!!7?5#@\n"
		"Y7!!!7?JY55555YJ?77!!!77777777777777!!7G\n"
		"GJJ5B&@@@@@@@@@@@&#PY7!!77777777777777!?\n"
		"@@@@@@@@&&&&&&&@@@@@@&GJ!!777777777777!Y\n"
		"@@@@#GP555555555PG#@@@@@B?!7777777777!J&\n"
		"@@&PYYY555PPPP5YYYY5#@@@@@5!77777777!?&@\n"
		"@@PY5555B@@@@@&P55555#@@@@@5!777777!7#@@\n"
		"@@PY5555PB&&@@@@&&&&&&@@@@@@J!7777!7B@@@\n"
		"@@&P5YY5YY555PPGB#&@@@@@@@@@G!7777!P@@@@\n"
		"@@@@#BGP555YYYYYYY55G&@@@@@@#7777!5@@@@@\n"
		"@@@@@@@@@&&#BBP55555YP@@@@@@B!77!Y@@@@@@\n"
		"@#PPPPPB@@@@@@@&555555&@@@@@Y!7!J@@@@@@@\n"
		"@@PYYYY5G#&&&&#G5555YG@@@@@B77!?&@@@@@@@\n"
		"@@@B55YYYY5555YYYY5PB@@@@@B7!!7#@@@@@@@@\n"
		"@@@@@#BGPPPPPPPPGB#@@@@@&57!!?#@@@@@@@@@\n"
		"@@@@@@@@@@@@@@@@@@@@@@&P?!!!Y&@@@@@@@@@@\n"
		"@@@@@@@@@@@@@@@@@@@@@@P!!7JB@@@@@@@@@@@@\n"
		"@@@@@@@@@@@@@@@@@@@@@@@BGB@@@@@@@@@@@@@@\n");*/
	Msg("Engine Version: Source Engine 1.15\n");
	Msg("Platform: %s\n", GetPlatformName());
	Msg("Arch: %s\n", GetProcessorArchName());
	Msg("Game: %s\n", GetGame());
#ifdef CLIENT_GUNMOD_DLL
	Msg("Gunmod Version: %s\n", GetGunmodVersion());
#endif
}

ConCommand neofetch("neofetch", CC_Neofetch, "");
ConCommand buildinfo("buildinfo", CC_Neofetch, "");