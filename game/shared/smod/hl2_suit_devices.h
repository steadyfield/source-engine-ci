//========= Copyleft Wineglass Studios, Some rights reserved. =================//
//
// Purpose: Defines for suit functions, moving them out of SHAREDDEFS.H, 
//			thus letting us add or remove any suit functions as we need without
//			having to do a full recompile
//
//=============================================================================//


#define MAX_SUIT_DEVICES			8			//3 by default, up by one for each extra suit device
#define bits_SUIT_DEVICE_SPRINT		0x00000001	//HL2 Sprint
#define bits_SUIT_DEVICE_FLASHLIGHT	0x00000002	//HL2 Flashlight
#define bits_SUIT_DEVICE_BREATHER	0x00000004	//HL2 Underwater air guage
#define bits_SUIT_DEVICE_CLOAK		0x00000008	//SMOD Cloak
#define bits_SUIT_DEVICE_LONGJUMP	0x00000016	//SMOD Long Jump
#define bits_SUIT_DEVICE_freeslot1	0x00000032	//free slot
#define bits_SUIT_DEVICE_freeslot2	0x00000064	//free slot
#define bits_SUIT_DEVICE_freeslot3	0x00000128	//free slot
