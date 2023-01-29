#include "cbase.h" 
#include "../common/proto_version.h"

extern const tchar* GetProcessorArchName();

void CC_Neofetch(void)
{
#ifdef PLATFORM_LINUX	
	char platform = 'Linux';
#endif
	Msg("Engine Version: Source Engine %i\n
		 Platform: %s\n
		 ", PROTOCOL_VERSION, platform);
}

ConCommand neofetch("neofetch", CC_Neofetch, ""); 
