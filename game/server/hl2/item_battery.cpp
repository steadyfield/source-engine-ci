//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Handling for the suit batteries.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "hl2_player.h"
#include "basecombatweapon.h"
#include "hl2_gamerules.h"
#include "items.h"
#include "engine/IEngineSound.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar sk_battery;

class CItemBattery : public CItem
{
public:
	DECLARE_CLASS( CItemBattery, CItem );
	DECLARE_DATADESC();

	void Spawn( void )
	{ 
		Precache( );

		SetModel(STRING(GetModelName()));

		BaseClass::Spawn( );
	}
	void Precache( void )
	{

		CHalfLife2 *pHL2Rules = HL2GameRules();
		if (pHL2Rules->IsInHL1Map())
			SetModelName(AllocPooledString("models/w_battery.mdl"));	//If we're in HL1
		else if (CBaseEntity::GetModelName() == NULL_STRING)
			SetModelName(AllocPooledString("models/items/battery.mdl"));	//If we got here, we are not in HL1 and we do not have a custom model
		else
			SetModelName(CBaseEntity::GetModelName());


		PrecacheModel(STRING(GetModelName()));

		PrecacheScriptSound( "ItemBattery.Touch" );

	}
	bool MyTouch( CBasePlayer *pPlayer )
	{
		if ((pPlayer->ArmorValue() < MAX_NORMAL_BATTERY) && pPlayer->IsSuitEquipped())
		{
			int pct;
			char szcharge[64];

			if (iBatteryCharge)
				pPlayer->IncrementArmorValue( iBatteryCharge, MAX_NORMAL_BATTERY );
			else
				pPlayer->IncrementArmorValue( sk_battery.GetFloat(), MAX_NORMAL_BATTERY );

			CPASAttenuationFilter filter( this, "ItemBattery.Touch" );
			EmitSound( filter, entindex(), "ItemBattery.Touch" );

			CSingleUserRecipientFilter user( pPlayer );
			user.MakeReliable();

			UserMessageBegin( user, "ItemPickup" );
				WRITE_STRING( GetClassname() );
			MessageEnd();

			
			// Suit reports new power level
			// For some reason this wasn't working in release build -- round it.
			pct = (intp)( (float)(pPlayer->ArmorValue() * 100.0) * (1.0/MAX_NORMAL_BATTERY) + 0.5);
			pct = (pct / 5);
			if (pct > 0)
				pct--;
		
			Q_snprintf( szcharge,sizeof(szcharge),"!HEV_%1dP", pct );
			
			//UTIL_EmitSoundSuit(edict(), szcharge);
			pPlayer->SetSuitUpdate(szcharge, FALSE, SUIT_NEXT_IN_30SEC);
			return true;		
		}
		return false;
	}

	int iBatteryCharge;
};

BEGIN_DATADESC(CItemBattery)
DEFINE_KEYFIELD(iBatteryCharge, FIELD_INTEGER, "ChargeAmount"),
END_DATADESC()

LINK_ENTITY_TO_CLASS(item_battery, CItemBattery);
PRECACHE_REGISTER(item_battery);

