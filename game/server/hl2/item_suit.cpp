//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
/*

===== item_suit.cpp ========================================================

  handling for the player's suit.
*/

#include "cbase.h"
#include "hl2_player.h"
#include "hl2_gamerules.h"
#include "items.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define SF_SUIT_SHORTLOGON		0x0001

extern ConVar smod_playerdialog;
ConVar smod_alwaysplayshortsuitdialog("smod_alwaysplayshortsuitdialog", "0", FCVAR_ARCHIVE);

class CItemSuit : public CItem
{
public:
	DECLARE_CLASS( CItemSuit, CItem );
	DECLARE_DATADESC();

	float timeSinceSpawn;

	CItemSuit()
	{
		iSoundType = 0;
	}

	void Spawn( void )
	{ 
		Precache( );

		SetModel(STRING(GetModelName()));

		BaseClass::Spawn( );
		
		CollisionProp()->UseTriggerBounds( false, 0 );
	}
	void Precache( void )
	{
		CHalfLife2 *pHL2Rules = HL2GameRules();
		if (pHL2Rules->IsInHL1Map())
			SetModelName(AllocPooledString("models/w_suit.mdl"));	//If we're in HL1
		else if (CBaseEntity::GetModelName() == NULL_STRING)
			SetModelName(AllocPooledString("models/items/hevsuit.mdl"));	//If we got here, we are not in HL1 and we do not have a custom model
		else
			SetModelName(CBaseEntity::GetModelName());

		PrecacheModel(STRING(GetModelName()));

		if (iSoundTipe)
			iSoundType = iSoundTipe;
	}
	bool MyTouch( CBasePlayer *pPlayer )
	{
		//SMOD: May have more then 1 suit
		//if ( pPlayer->IsSuitEquipped() )
		//	return FALSE;

		//SMOD: Use old behavor if the spawnflag is set, otherwise use the new ones.
		CHalfLife2 *pHL2Rules = HL2GameRules();
		
		if ( m_spawnflags & SF_SUIT_SHORTLOGON )
		{
			UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_A0");	// Only beep
		}
		else if (!Q_strnicmp(STRING(gpGlobals->mapname), "t0a0", 4))
		{
			//Play the beep and "Hev MK4" dialog if in HL1's training course
			UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_AAw");	// short version of suit logon (HL1)
		}
		else if (pHL2Rules->IsInHL1Map())
		{
			//Play the full thing with the Hev Mk4 dialog
			UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_AAv");	// long version of suit logon (HL1)
		}
		else if (!Q_strnicmp(STRING(gpGlobals->mapname), "d1_transation_05", 16))
		{
			//Also play the "Hev MK5" dialog if in d1_transation_05
			UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_AAx");	// short version of suit logon
		}
		else
		{
			if (iSoundType == 2)
				UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_A0");	// Only beep,
			else if (iSoundType == 3)
				UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_AAx");	// short version of suit logon,
			else if (iSoundType == 4)
				UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_AAz");	// long version of suit logon,
			else if (iSoundType == 7)
				UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_AAy");	// OpFor armor equip noise,
			else if (iSoundType == 5)
				UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_AAw");	// short version of suit logon (HL1),
			else if (iSoundType == 6)
				UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_AAv");	// long version of suit logon (HL1),
			else if (iSoundType == 0) //Old type suit...
				UTIL_EmitSoundSuit(pPlayer->edict(), "!HEV_A0");	// only beep (again)
		}

		pPlayer->EquipSuit();
				
		return true;
	}
	int iSoundType;
	int iSoundTipe;
};

BEGIN_DATADESC( CItemSuit )
DEFINE_KEYFIELD(iSoundTipe, FIELD_INTEGER, "SuitStartUpSound"),
END_DATADESC()

LINK_ENTITY_TO_CLASS(item_suit, CItemSuit);
