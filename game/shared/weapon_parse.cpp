//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Weapon data file parsing, shared by game & client dlls.
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include <KeyValues.h>
#include <tier0/mem.h>
#include "filesystem.h"
#include "utldict.h"
#include "ammodef.h"
#if defined(CLIENT_DLL)
	#include "hl2/c_basehlcombatweapon.h"
	#include "SMMOD/c_weapon_custom.h"
	#include "c_baseentity.h"
#else
	#include "SMMOD/weapon_custom.h"
#endif

#include <string>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// The sound categories found in the weapon classname.txt files
// This needs to match the WeaponSound_t enum in weapon_parse.h
#if !defined(_STATIC_LINKED) || defined(CLIENT_DLL)
const char *pWeaponSoundCategories[ NUM_SHOOT_SOUND_TYPES ] = 
{
	"empty",
	"single_shot",
	"single_shot_npc",
	"double_shot",
	"double_shot_npc",
	"burst",
	"reload",
	"reload_npc",
	"melee_miss",
	"melee_hit",
	"melee_hit_world",
	"special1",
	"special2",
	"special3",
	"taunt",
	"deploy",

	"pump"	//SMOD
};
#else
extern const char *pWeaponSoundCategories[ NUM_SHOOT_SOUND_TYPES ];
#endif

int GetWeaponSoundFromString( const char *pszString )
{
	for ( int i = EMPTY; i < NUM_SHOOT_SOUND_TYPES; i++ )
	{
		if ( !Q_stricmp(pszString,pWeaponSoundCategories[i]) )
			return (WeaponSound_t)i;
	}
	return -1;
}


// Item flags that we parse out of the file.
typedef struct
{
	const char *m_pFlagName;
	int m_iFlagValue;
} itemFlags_t;
#if !defined(_STATIC_LINKED) || defined(CLIENT_DLL)
itemFlags_t g_ItemFlags[8] =
{
	{ "ITEM_FLAG_SELECTONEMPTY",	ITEM_FLAG_SELECTONEMPTY },
	{ "ITEM_FLAG_NOAUTORELOAD",		ITEM_FLAG_NOAUTORELOAD },
	{ "ITEM_FLAG_NOAUTOSWITCHEMPTY", ITEM_FLAG_NOAUTOSWITCHEMPTY },
	{ "ITEM_FLAG_LIMITINWORLD",		ITEM_FLAG_LIMITINWORLD },
	{ "ITEM_FLAG_EXHAUSTIBLE",		ITEM_FLAG_EXHAUSTIBLE },
	{ "ITEM_FLAG_DOHITLOCATIONDMG", ITEM_FLAG_DOHITLOCATIONDMG },
	{ "ITEM_FLAG_NOAMMOPICKUPS",	ITEM_FLAG_NOAMMOPICKUPS },
	{ "ITEM_FLAG_NOITEMPICKUP",		ITEM_FLAG_NOITEMPICKUP }
};
#else
extern itemFlags_t g_ItemFlags[8];
#endif


static CUtlDict< FileWeaponInfo_t*, unsigned short > m_WeaponInfoDatabase;

#if 1 //#ifdef _DEBUG
// used to track whether or not two weapons have been mistakenly assigned the wrong slot
bool g_bUsedWeaponSlots[MAX_WEAPON_SLOTS][MAX_WEAPON_POSITIONS] = { 0 };

#endif

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *name - 
// Output : FileWeaponInfo_t
//-----------------------------------------------------------------------------
static WEAPON_FILE_INFO_HANDLE FindWeaponInfoSlot( const char *name )
{
	// Complain about duplicately defined metaclass names...
	unsigned short lookup = m_WeaponInfoDatabase.Find( name );
	if ( lookup != m_WeaponInfoDatabase.InvalidIndex() )
	{
		return lookup;
	}

	FileWeaponInfo_t *insert = CreateWeaponInfo();

	lookup = m_WeaponInfoDatabase.Insert( name, insert );
	Assert( lookup != m_WeaponInfoDatabase.InvalidIndex() );
	return lookup;
}

// Find a weapon slot, assuming the weapon's data has already been loaded.
WEAPON_FILE_INFO_HANDLE LookupWeaponInfoSlot( const char *name )
{
	return m_WeaponInfoDatabase.Find( name );
}



// FIXME, handle differently?
static FileWeaponInfo_t gNullWeaponInfo;


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : handle - 
// Output : FileWeaponInfo_t
//-----------------------------------------------------------------------------
FileWeaponInfo_t *GetFileWeaponInfoFromHandle( WEAPON_FILE_INFO_HANDLE handle )
{
	if ( handle < 0 || handle >= m_WeaponInfoDatabase.Count() )
	{
		return &gNullWeaponInfo;
	}

	if ( handle == m_WeaponInfoDatabase.InvalidIndex() )
	{
		return &gNullWeaponInfo;
	}

	return m_WeaponInfoDatabase[ handle ];
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : WEAPON_FILE_INFO_HANDLE
//-----------------------------------------------------------------------------
WEAPON_FILE_INFO_HANDLE GetInvalidWeaponInfoHandle( void )
{
	return (WEAPON_FILE_INFO_HANDLE)m_WeaponInfoDatabase.InvalidIndex();
}

void PrecacheFileWeaponInfoDatabase( IFileSystem *filesystem, const unsigned char *pICEKey )
{
	if ( m_WeaponInfoDatabase.Count() )
		return;

	KeyValues *manifest = new KeyValues( "weaponscripts" );
	if ( manifest->LoadFromFile( filesystem, "scripts/weapon_manifest.txt", "GAME" ) )
	{
		for ( KeyValues *sub = manifest->GetFirstSubKey(); sub != NULL ; sub = sub->GetNextKey() )
		{
			if ( !Q_stricmp( sub->GetName(), "file" ) )
			{
				char fileBase[512];
				Q_FileBase( sub->GetString(), fileBase, sizeof(fileBase) );
				WEAPON_FILE_INFO_HANDLE tmp;
#ifdef CLIENT_DLL
				if ( ReadWeaponDataFromFileForSlot( filesystem, fileBase, &tmp, pICEKey ) )
				{
					gWR.LoadWeaponSprites( tmp );
				}
#else
				ReadWeaponDataFromFileForSlot( filesystem, fileBase, &tmp, pICEKey );
#endif
			}
			else
			{
				Error( "Expecting 'file', got %s\n", sub->GetName() );
			}
		}
	}
	manifest->deleteThis();
}

KeyValues* ReadEncryptedKVFile( IFileSystem *filesystem, const char *szFilenameWithoutExtension, const unsigned char *pICEKey, bool bForceReadEncryptedFile /*= false*/ )
{
	Assert( strchr( szFilenameWithoutExtension, '.' ) == NULL );
	char szFullName[512];

	const char *pSearchPath = "MOD";

	if ( pICEKey == NULL )
	{
		pSearchPath = "GAME";
	}

	// Open the weapon data file, and abort if we can't
	KeyValues *pKV = new KeyValues( "WeaponDatafile" );

	Q_snprintf(szFullName,sizeof(szFullName), "%s.txt", szFilenameWithoutExtension);

	if ( bForceReadEncryptedFile || !pKV->LoadFromFile( filesystem, szFullName, pSearchPath ) ) // try to load the normal .txt file first
	{
#ifndef _XBOX
		if ( pICEKey )
		{
			Q_snprintf(szFullName,sizeof(szFullName), "%s.ctx", szFilenameWithoutExtension); // fall back to the .ctx file

			FileHandle_t f = filesystem->Open( szFullName, "rb", pSearchPath );

			if (!f)
			{
				pKV->deleteThis();
				return NULL;
			}
			// load file into a null-terminated buffer
			int fileSize = filesystem->Size(f);
			char *buffer = (char*)MemAllocScratch(fileSize + 1);
		
			Assert(buffer);
		
			filesystem->Read(buffer, fileSize, f); // read into local buffer
			buffer[fileSize] = 0; // null terminate file as EOF
			filesystem->Close( f );	// close file after reading

			UTIL_DecodeICE( (unsigned char*)buffer, fileSize, pICEKey );

			bool retOK = pKV->LoadFromBuffer( szFullName, buffer, filesystem );

			MemFreeScratch();

			if ( !retOK )
			{
				pKV->deleteThis();
				return NULL;
			}
		}
		else
		{
			pKV->deleteThis();
			return NULL;
		}
#else
		pKV->deleteThis();
		return NULL;
#endif
	}

	return pKV;
}


//-----------------------------------------------------------------------------
// Purpose: Read data on weapon from script file
// Output:  true  - if data2 successfully read
//			false - if data load fails
//-----------------------------------------------------------------------------

bool ReadWeaponDataFromFileForSlot( IFileSystem* filesystem, const char *szWeaponName, WEAPON_FILE_INFO_HANDLE *phandle, const unsigned char *pICEKey )
{
	if ( !phandle )
	{
		Assert( 0 );
		return false;
	}
	
	*phandle = FindWeaponInfoSlot( szWeaponName );
	FileWeaponInfo_t *pFileInfo = GetFileWeaponInfoFromHandle( *phandle );
	Assert( pFileInfo );

	if ( pFileInfo->bParsedScript )
		return true;

	char sz[128];
	Q_snprintf( sz, sizeof( sz ), "scripts/%s", szWeaponName );

	KeyValues *pKV = ReadEncryptedKVFile( filesystem, sz, pICEKey,
#if defined( DOD_DLL )
		true			// Only read .ctx files!
#else
		false
#endif
		);
	
	if ( !pKV ) //If it failed even after the custom weapon check, then don't read it
		return false;

	pFileInfo->Parse( pKV, szWeaponName );

	pKV->deleteThis();

	return true;
}


//-----------------------------------------------------------------------------
// FileWeaponInfo_t implementation.
//-----------------------------------------------------------------------------

FileWeaponInfo_t::FileWeaponInfo_t()
{
	bParsedScript = false;
	bLoadedHudElements = false;
	szClassName[0] = 0;
	szPrintName[0] = 0;

	szViewModel[0] = 0;
	szWorldModel[0] = 0;
	szAnimationPrefix[0] = 0;
	iSlot = 17; //SMOD: To not cover up the crowbar...
	iPosition = 0;
	iMaxClip1 = 0;
	iMaxClip2 = 0;
	iDefaultClip1 = 0;
	iDefaultClip2 = 0;
	iWeight = 0;
	iRumbleEffect = -1;
	bAutoSwitchTo = false;
	bAutoSwitchFrom = false;
	iFlags = 0;
	szAmmo1[0] = 0;
	szAmmo2[0] = 0;
	memset( aShootSounds, 0, sizeof( aShootSounds ) );
	iAmmoType = 0;
	iAmmo2Type = 0;
	m_bMeleeWeapon = false;
	iSpriteCount = 0;
	iconActive = 0;
	iconInactive = 0;
	iconAmmo = 0;
	iconAmmo2 = 0;
	iconCrosshair = 0;
	iconAutoaim = 0;
	iconZoomedCrosshair = 0;
	iconZoomedAutoaim = 0;
	bShowUsageHint = false;
	m_bAllowFlipping = true;
	m_bBuiltRightHanded = true;
	m_bUsesCSMuzzleFlash = false;
}

#ifdef CLIENT_DLL
extern ConVar hud_fastswitch;
#endif

void FileWeaponInfo_t::Parse( KeyValues *pKeyValuesData, const char *szWeaponName )
{
	// Okay, we tried at least once to look this up...
	bParsedScript = true;

	// Classname
	Q_strncpy( szClassName, szWeaponName, MAX_WEAPON_STRING );
	// Printable name
	Q_strncpy( szPrintName, pKeyValuesData->GetString( "printname", WEAPON_PRINTNAME_MISSING ), MAX_WEAPON_STRING );
	// View model & world model
	Q_strncpy( szViewModel, pKeyValuesData->GetString( "viewmodel" ), MAX_WEAPON_STRING );
	Q_strncpy( szWorldModel, pKeyValuesData->GetString( "playermodel" ), MAX_WEAPON_STRING );
	Q_strncpy( szAnimationPrefix, pKeyValuesData->GetString( "anim_prefix" ), MAX_WEAPON_PREFIX );
	iPosition = pKeyValuesData->GetInt( "bucket_position", 0 );
	
	const char *pBucket = pKeyValuesData->GetString( "bucket", NULL );
	if ( pBucket )
	{
		if ( Q_stricmp( pBucket, "melee" ) == 0 )
		{
			iSlot = 0;
		}
		else if ( Q_stricmp( pBucket, "pistol" ) == 0 )
		{
			iSlot = 1;
		}
		else if ( Q_stricmp( pBucket, "light" ) == 0 )
		{
			iSlot = 2;
		}
		else if ( Q_stricmp( pBucket, "heavy" ) == 0 )
		{
			iSlot = 3;
		}
		else if ( Q_stricmp( pBucket, "grenades" ) == 0 )
		{
			iSlot = 4;
		}
		else if ( Q_stricmp( pBucket, "special" ) == 0 )
		{
			iSlot = 5;
		}
		else if ( Q_stricmp( pBucket, "tool" ) == 0 )
		{
			iSlot = 6;
		}
		else if ( Q_stricmp( pBucket, "disabled" ) == 0 )
		{
			iSlot = 25;
		}
		else
		{
			iSlot = pKeyValuesData->GetInt("bucket", 0);
			Msg("Weapon %s (%s) is using old-style buckets, should really be using new ones.\n", szPrintName, szClassName);
		}
	}
	else
	{
		iSlot = pKeyValuesData->GetInt( "bucket", 17 );  //Default this to out-of-bounds so they don't cover up the crowbar...
		Warning("WARNING: Weapon %s (%s) is missing bucket info!!\n", szPrintName, szClassName);
	}

	// Use the console (X360) buckets if hud_fastswitch is set to 2.
	//SMOD: Our new hud does not support X360-style hudbuckets
//#ifdef CLIENT_DLL
//	if ( hud_fastswitch.GetInt() == 2 )
//#else
//	if ( IsX360() )
//#endif
//	{
//		iSlot = pKeyValuesData->GetInt( "bucket_360", iSlot );
//		iPosition = pKeyValuesData->GetInt( "bucket_position_360", iPosition );
//	}

	iMaxClip1 = pKeyValuesData->GetInt( "clip_size", WEAPON_NOCLIP );					// Max primary clips gun can hold (assume they don't use clips by default)
	iMaxClip2 = pKeyValuesData->GetInt( "clip2_size", WEAPON_NOCLIP );					// Max secondary clips gun can hold (assume they don't use clips by default)
	iDefaultClip1 = pKeyValuesData->GetInt( "default_clip", iMaxClip1 );		// amount of primary ammo placed in the primary clip when it's picked up
	iDefaultClip2 = pKeyValuesData->GetInt( "default_clip2", iMaxClip2 );		// amount of secondary ammo placed in the secondary clip when it's picked up
	//iWeight = pKeyValuesData->GetInt( "weight", 0 );
	iWeight = 0; //SMOD: The autoselect for other guns when the current one is empty can get a bit fucky with 60+ weapons, better to have it always zero

	iRumbleEffect = pKeyValuesData->GetInt( "rumble", -1 );
	
	// LAME old way to specify item flags.
	// Weapon scripts should use the flag names.
	iFlags = pKeyValuesData->GetInt( "item_flags", ITEM_FLAG_LIMITINWORLD );

	for ( int i=0; i < ARRAYSIZE( g_ItemFlags ); i++ )
	{
		int iVal = pKeyValuesData->GetInt( g_ItemFlags[i].m_pFlagName, -1 );
		if ( iVal == 0 )
		{
			iFlags &= ~g_ItemFlags[i].m_iFlagValue;
		}
		else if ( iVal == 1 )
		{
			iFlags |= g_ItemFlags[i].m_iFlagValue;
		}
	}


	bShowUsageHint = ( pKeyValuesData->GetInt( "showusagehint", 0 ) != 0 ) ? true : false;
	bAutoSwitchTo = ( pKeyValuesData->GetInt( "autoswitchto", 1 ) != 0 ) ? true : false;
	bAutoSwitchFrom = ( pKeyValuesData->GetInt( "autoswitchfrom", 1 ) != 0 ) ? true : false;
	m_bBuiltRightHanded = ( pKeyValuesData->GetInt( "BuiltRightHanded", 1 ) != 0 ) ? true : false;
	m_bAllowFlipping = ( pKeyValuesData->GetInt( "AllowFlipping", 1 ) != 0 ) ? true : false;
	m_bMeleeWeapon = ( pKeyValuesData->GetInt( "MeleeWeapon", 0 ) != 0 ) ? true : false;

#if 1 //#if defined(_DEBUG) && defined(HL2_CLIENT_DLL)
	// make sure two weapons aren't in the same slot & position
	//SMOD: Always enable these but suppress it if it's in 25th (aka disabled) slot
	if (!(iSlot == 25))
	{
		if (iSlot >= MAX_WEAPON_SLOTS ||
			iPosition >= MAX_WEAPON_POSITIONS)
		{
			Warning("Invalid weapon slot or position [slot %d/%d max], pos[%d/%d max]\n",
				iSlot, MAX_WEAPON_SLOTS - 1, iPosition, MAX_WEAPON_POSITIONS - 1);
		}
		else
		{
			if (g_bUsedWeaponSlots[iSlot][iPosition])
			{
				Warning("Duplicately assigned weapon slots in selection hud:  %s (%s, %d, %d)\n", szPrintName, szClassName, iSlot, iPosition);
			}
			g_bUsedWeaponSlots[iSlot][iPosition] = true;
		}
	}
#endif

	// Primary ammo used
	const char *pAmmo = pKeyValuesData->GetString( "primary_ammo", "None" );
	if ( strcmp("None", pAmmo) == 0 )
		Q_strncpy( szAmmo1, "", sizeof( szAmmo1 ) );
	else
		Q_strncpy( szAmmo1, pAmmo, sizeof( szAmmo1 )  );
	iAmmoType = GetAmmoDef()->Index( szAmmo1 );
	
	// Secondary ammo used
	pAmmo = pKeyValuesData->GetString( "secondary_ammo", "None" );
	if ( strcmp("None", pAmmo) == 0)
		Q_strncpy( szAmmo2, "", sizeof( szAmmo2 ) );
	else
		Q_strncpy( szAmmo2, pAmmo, sizeof( szAmmo2 )  );
	iAmmo2Type = GetAmmoDef()->Index( szAmmo2 );

	// Now read the weapon sounds
	memset( aShootSounds, 0, sizeof( aShootSounds ) );
	KeyValues *pSoundData = pKeyValuesData->FindKey( "SoundData" );
	if ( pSoundData )
	{
		for ( int i = EMPTY; i < NUM_SHOOT_SOUND_TYPES; i++ )
		{
			const char *soundname = pSoundData->GetString( pWeaponSoundCategories[i] );
			if ( soundname && soundname[0] )
			{
				Q_strncpy( aShootSounds[i], soundname, MAX_WEAPON_STRING );
			}
		}
	}
	
	//SMOD custom parse stuff

	m_flFovAdd = pKeyValuesData->GetFloat("addfov", 0.0f);
	Q_strncpy( szMuzzleAttachement, pKeyValuesData->GetString( "MuzzleFlashAttachement", "muzzle" ), MAX_WEAPON_STRING );
	m_bUsesCSMuzzleFlash = ( pKeyValuesData->GetInt( "UsesCSMuzzleFlash", 0 ) != 0 ) ? true : false;
	Q_strncpy( szCSMuzzleFlashName, pKeyValuesData->GetString( "CSMuzzleFlashType", "CS_MUZZLEFLASH" ), MAX_WEAPON_STRING );
	
	m_bCanIronSight = ( pKeyValuesData->GetInt( "CanIronsight", 0 ) != 0 ) ? true : false;
	KeyValues *pSights = pKeyValuesData->FindKey( "IronSight" );
	if (pSights)
	{
		vecIronsightPosOffset.x		= pSights->GetFloat( "forward", 0.0f );
		vecIronsightPosOffset.y		= pSights->GetFloat( "right", 0.0f );
		vecIronsightPosOffset.z		= pSights->GetFloat( "up", 0.0f );
 
		angIronsightAngOffset[PITCH]	= pSights->GetFloat( "pitch", 0.0f );
		angIronsightAngOffset[YAW]		= pSights->GetFloat( "yaw", 0.0f );
		angIronsightAngOffset[ROLL]		= pSights->GetFloat( "roll", 0.0f );
 
		flIronsightFOVOffset		= pSights->GetFloat( "fov", 0.0f );
	}
	else
	{
		vecIronsightPosOffset = vec3_origin;
		angIronsightAngOffset.Init();
		flIronsightFOVOffset = 0.0f;
	}

	m_bIsScripted = (pKeyValuesData->GetInt("IsScriptedWeapon", 0) != 0) ? true : false;

	KeyValues *pWeaponSettings = pKeyValuesData->FindKey( "WeaponSettings" );
	if ( m_bIsScripted && pWeaponSettings )
	{
		Q_strncpy( szPrecacheEnt1, pWeaponSettings->GetString( "precache_entity" ), MAX_WEAPON_STRING );
		Q_strncpy( szPrecacheEnt2, pWeaponSettings->GetString( "precache_entity2" ), MAX_WEAPON_STRING );
		Q_strncpy( szGiveCheatName, pWeaponSettings->GetString( "name" ), MAX_WEAPON_STRING );

		m_bReloadsSingly = ( pWeaponSettings->GetInt( "ReloadsSingly", 0 ) != 0 ) ? true : false;
		m_bUsesShotgunReload = ( pWeaponSettings->GetInt( "UsesShotgunReload", 0 ) != 0 ) ? true : false;

		KeyValues *pAnimations = pWeaponSettings->FindKey("Animations");
		if ( pAnimations )
		{
			Q_strncpy(szAnimDeploy, pAnimations->GetString("deploy",	"ACT_VM_DRAW"),				MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimReload, pAnimations->GetString("reload",	"ACT_VM_RELOAD"),			MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimIdle,	pAnimations->GetString("idle",		"ACT_VM_IDLE"),				MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimPriAtk, pAnimations->GetString("primary",	"ACT_VM_PRIMARYATTACK"),	MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimPriMiss,pAnimations->GetString("primiss",	"ACT_VM_MISSCENTER"),		MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimSecAtk, pAnimations->GetString("secondary",	"ACT_VM_SECONDARYATTACK"),	MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimSecMiss,pAnimations->GetString("secmiss",	"ACT_VM_MISSCENTER2"),		MAX_WEAPON_PREFIX);
		}
		else
		{
			Warning("WARNING: Custom weapon %s (%s) is missing Animations info!\n", szPrintName, szClassName);
			Q_strncpy(szAnimDeploy,		"ACT_VM_DRAW",				MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimReload,		"ACT_VM_RELOAD",			MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimIdle,		"ACT_VM_IDLE",				MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimPriAtk,		"ACT_VM_PRIMARYATTACK",		MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimPriMiss,	"ACT_VM_MISSCENTER",		MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimSecAtk,		"ACT_VM_SECONDARYATTACK",	MAX_WEAPON_PREFIX);
			Q_strncpy(szAnimSecMiss,	"ACT_VM_MISSCENTER2",		MAX_WEAPON_PREFIX);
		}

		KeyValues *pPrimaryAttack = pWeaponSettings->FindKey("PrimaryAttack");
		if (pPrimaryAttack)
		{
			Q_strncpy(szAttackTypePri, pPrimaryAttack->GetString("type", "bullet"), 6);
			m_iDamagePri = pPrimaryAttack->GetInt("Damage", 0);
			m_bUnderWtaerPri = (pPrimaryAttack->GetInt("UnderWater", 0) != 0) ? true : false;
			m_bUsesShotgunPumpPri = (pPrimaryAttack->GetInt("Pump", 0) != 0) ? true : false;
			m_fFireRatePri = pPrimaryAttack->GetFloat("FireRate", 0.0f);

			KeyValues *pBulletPri = pPrimaryAttack->FindKey("Bullet");
			KeyValues *pLaunchPri = pPrimaryAttack->FindKey("Launch");
			KeyValues *pMeleePri = pPrimaryAttack->FindKey("Melee");
			KeyValues *pGrenadePri = pPrimaryAttack->FindKey("Grenade");

			if ((!Q_strnicmp(szAttackTypePri, "Bullet", 6)) && pBulletPri)
			{
				Q_strncpy(szFireModePri, pBulletPri->GetString("Mode", "Semi"), 5);

				m_iAmountPri = pBulletPri->GetInt("Amount", 0);
				m_iRangePri = pBulletPri->GetInt("Range", 4096);
				Q_strncpy(szTracerPri, pBulletPri->GetString("Tracer", "0"), MAX_WEAPON_PREFIX);

				KeyValues *pSpreadPri = pBulletPri->FindKey("Spread");
				if (pSpreadPri)
				{
					m_vSpreadPri.x = sin((pSpreadPri->GetFloat("x", 0.0f) / 2.0f));
					m_vSpreadPri.y = sin((pSpreadPri->GetFloat("y", 0.0f) / 2.0f));
					m_vSpreadPri.z = sin((pSpreadPri->GetFloat("z", 0.0f) / 2.0f));

					KeyValues *pSpreadPriIron = pSpreadPri->FindKey("Ironsight");
					if (pSpreadPriIron && m_bCanIronSight)
					{
						m_vSpreadPri.x = sin((pSpreadPriIron->GetFloat("x", 0.0f) / 2.0f));
						m_vSpreadPri.y = sin((pSpreadPriIron->GetFloat("y", 0.0f) / 2.0f));
						m_vSpreadPri.z = sin((pSpreadPriIron->GetFloat("z", 0.0f) / 2.0f));
					}
					else
					{
						m_vSpreadPriIron.x = m_vSpreadPri.x;
						m_vSpreadPriIron.y = m_vSpreadPri.y;
						m_vSpreadPriIron.z = m_vSpreadPri.z;
					}
				}
				else
				{
					Warning("WARNING: Custom weapon %s (%s) is using bullets (primary) but has no spread set!\n", szPrintName, szClassName);
					m_vSpreadPri.x = 0.0f;
					m_vSpreadPri.y = 0.0f;
					m_vSpreadPri.z = 0.0f;
				}	
			}
			else if ((!Q_strnicmp(szAttackTypePri, "Launch", 6)) && pLaunchPri)
			{	
				Q_strncpy(szLnchEntNamePri, pLaunchPri->GetString("Entity"), MAX_WEAPON_PREFIX);
				Q_strncpy(szLnchTypePri, pLaunchPri->GetString("Launch_Type", "smg1"), 4);
				m_iVelocityPri = pLaunchPri->GetInt("Velocity", 4096);
			}
			else if ((!V_strnicmp(szAttackTypePri, "Melee", 5)) && pMeleePri)
			{

			}
			else if ((!Q_strnicmp(szAttackTypePri, "Grenade", 7)) && pGrenadePri)
			{

			}
			else
			{
				Warning("WARNING: Custom weapon %s (%s) has a primary attack but it's set up incorrectly!\n", szPrintName, szClassName);
			}
		}
		else
		{
			Warning("WARNING: Custom weapon %s (%s) doesn't have a primary attack!\n", szPrintName, szClassName);
		}
	}
	else if (m_bIsScripted && !pWeaponSettings)
	{
		Warning("WARNING: Custom weapon %s (%s) is missing WeaponSettings info!!!\n", szPrintName, szClassName);
	}
}

