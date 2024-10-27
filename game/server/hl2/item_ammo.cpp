//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: The various ammo types for HL2
//
//=============================================================================//

#include "cbase.h"
#include "player.h"
#include "gamerules.h"
#include "items.h"
#include "ammodef.h"
#include "eventlist.h"
#include "npcevent.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar item_custom1_model("item_customammo1_model", "");
ConVar item_custom2_model("item_customammo2_model", "");
ConVar item_custom3_model("item_customammo3_model", "");
ConVar item_custom4_model("item_customammo4_model", "");
ConVar item_custom5_model("item_customammo5_model", "");
ConVar item_custom6_model("item_customammo6_model", "");
ConVar item_custom7_model("item_customammo7_model", "");
ConVar item_custom8_model("item_customammo8_model", "");
ConVar item_custom9_model("item_customammo9_model", "");
ConVar item_custom10_model("item_customammo10_model", "");

ConVar item_custom1_quantity("item_customammo1_quantity", "0");
ConVar item_custom2_quantity("item_customammo2_quantity", "0");
ConVar item_custom3_quantity("item_customammo3_quantity", "0");
ConVar item_custom4_quantity("item_customammo4_quantity", "0");
ConVar item_custom5_quantity("item_customammo5_quantity", "0");
ConVar item_custom6_quantity("item_customammo6_quantity", "0");
ConVar item_custom7_quantity("item_customammo7_quantity", "0");
ConVar item_custom8_quantity("item_customammo8_quantity", "0");
ConVar item_custom9_quantity("item_customammo9_quantity", "0");
ConVar item_custom10_quantity("item_customammo10_quantity", "0");

//---------------------------------------------------------
// Applies ammo quantity scale.
//---------------------------------------------------------
int ITEM_GiveAmmo(CBasePlayer *pPlayer, float flCount, const char *pszAmmoName, bool bSuppressSound = false)
{
	int iAmmoType = GetAmmoDef()->Index(pszAmmoName);
	if (iAmmoType == -1)
	{
		Msg("ERROR: Attempting to give unknown ammo type (%s)\n", pszAmmoName);
		return 0;
	}

	flCount *= g_pGameRules->GetAmmoQuantityScale(iAmmoType);

	// Don't give out less than 1 of anything.
	flCount = MAX(1.0f, flCount);

	return pPlayer->GiveAmmo(flCount, iAmmoType, bSuppressSound);
}

// ========================================================================
//	>> BoxSRounds
// ========================================================================
class CItem_BoxSRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxSRounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxsrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_PISTOL, "Pistol"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}

			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_box_srounds, CItem_BoxSRounds);
LINK_ENTITY_TO_CLASS(item_ammo_pistol, CItem_BoxSRounds);

// ========================================================================
//	>> LargeBoxSRounds
// ========================================================================
class CItem_LargeBoxSRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_LargeBoxSRounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsrounds.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxsrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_PISTOL_LARGE, "Pistol"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_large_box_srounds, CItem_LargeBoxSRounds);
LINK_ENTITY_TO_CLASS(item_ammo_pistol_large, CItem_LargeBoxSRounds);

// ========================================================================
//	>> BoxMRounds
// ========================================================================
class CItem_BoxMRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxMRounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxmrounds.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxmrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_SMG1, "SMG1"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_box_mrounds, CItem_BoxMRounds);
LINK_ENTITY_TO_CLASS(item_ammo_smg1, CItem_BoxMRounds);

// ========================================================================
//	>> LargeBoxMRounds
// ========================================================================
class CItem_LargeBoxMRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_LargeBoxMRounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxmrounds.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxmrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_SMG1_LARGE, "SMG1"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_large_box_mrounds, CItem_LargeBoxMRounds);
LINK_ENTITY_TO_CLASS(item_ammo_smg1_large, CItem_LargeBoxMRounds);

// ========================================================================
//	>> BoxLRounds
// ========================================================================
class CItem_BoxLRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxLRounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/combine_rifle_cartridge01.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/combine_rifle_cartridge01.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_AR2, "AR2"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_box_lrounds, CItem_BoxLRounds);
LINK_ENTITY_TO_CLASS(item_ammo_ar2, CItem_BoxLRounds);

// ========================================================================
//	>> LargeBoxLRounds
// ========================================================================
class CItem_LargeBoxLRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_LargeBoxLRounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/combine_rifle_cartridge01.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/combine_rifle_cartridge01.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_AR2_LARGE, "AR2"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_large_box_lrounds, CItem_LargeBoxLRounds);
LINK_ENTITY_TO_CLASS(item_ammo_ar2_large, CItem_LargeBoxLRounds);


// ========================================================================
//	>> CItem_Box357Rounds
// ========================================================================
class CItem_Box357Rounds : public CItem
{
public:
	DECLARE_CLASS(CItem_Box357Rounds, CItem);

	void Precache(void)
	{
		PrecacheModel("models/items/357ammo.mdl");
	}
	void Spawn(void)
	{
		Precache();
		SetModel("models/items/357ammo.mdl");
		BaseClass::Spawn();
	}

	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_357, "357"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_ammo_357, CItem_Box357Rounds);


// ========================================================================
//	>> CItem_LargeBox357Rounds
// ========================================================================
class CItem_LargeBox357Rounds : public CItem
{
public:
	DECLARE_CLASS(CItem_LargeBox357Rounds, CItem);

	void Precache(void)
	{
		PrecacheModel("models/items/357ammobox.mdl");
	}
	void Spawn(void)
	{
		Precache();
		SetModel("models/items/357ammobox.mdl");
		BaseClass::Spawn();
	}

	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_357_LARGE, "357"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_ammo_357_large, CItem_LargeBox357Rounds);


// ========================================================================
//	>> CItem_BoxXBowRounds
// ========================================================================
class CItem_BoxXBowRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxXBowRounds, CItem);

	void Precache(void)
	{
		PrecacheModel("models/items/crossbowrounds.mdl");
	}

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/crossbowrounds.mdl");
		BaseClass::Spawn();
	}

	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_CROSSBOW, "XBowBolt"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_ammo_crossbow, CItem_BoxXBowRounds);


// ========================================================================
//	>> FlareRound
// ========================================================================
class CItem_FlareRound : public CItem
{
public:
	DECLARE_CLASS(CItem_FlareRound, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/flare.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/flare.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, 1, "FlareRound"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_flare_round, CItem_FlareRound);

// ========================================================================
//	>> BoxFlareRounds
// ========================================================================
#define SIZE_BOX_FLARE_ROUNDS 5

class CItem_BoxFlareRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxFlareRounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxflares.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxflares.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_BOX_FLARE_ROUNDS, "FlareRound"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_box_flare_rounds, CItem_BoxFlareRounds);

// ========================================================================
// RPG Round
// ========================================================================
class CItem_RPG_Round : public CItem
{
public:
	DECLARE_CLASS(CItem_RPG_Round, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/weapons/w_missile_closed.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/weapons/w_missile_closed.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_RPG_ROUND, "RPG_Round"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_ml_grenade, CItem_RPG_Round);
LINK_ENTITY_TO_CLASS(item_rpg_round, CItem_RPG_Round);

// ========================================================================
//	>> AR2_Grenade
// ========================================================================
class CItem_AR2_Grenade : public CItem
{
public:
	DECLARE_CLASS(CItem_AR2_Grenade, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/ar2_grenade.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/ar2_grenade.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_SMG1_GRENADE, "SMG1_Grenade"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_ar2_grenade, CItem_AR2_Grenade);
LINK_ENTITY_TO_CLASS(item_ammo_smg1_grenade, CItem_AR2_Grenade);

// ========================================================================
//	>> BoxSniperRounds
// ========================================================================
#define SIZE_BOX_SNIPER_ROUNDS 10

class CItem_BoxSniperRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxSniperRounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxsniperrounds.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxsniperrounds.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_BOX_SNIPER_ROUNDS, "SniperRound"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_box_sniper_rounds, CItem_BoxSniperRounds);


// ========================================================================
//	>> BoxBuckshot
// ========================================================================
class CItem_BoxBuckshot : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxBuckshot, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxbuckshot.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxbuckshot.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_BUCKSHOT, "Buckshot"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_box_buckshot, CItem_BoxBuckshot);

// ========================================================================
//	>> CItem_AR2AltFireRound
// ========================================================================
class CItem_AR2AltFireRound : public CItem
{
public:
	DECLARE_CLASS(CItem_AR2AltFireRound, CItem);

	void Precache(void)
	{
		PrecacheParticleSystem("combineball");
		PrecacheModel("models/items/combine_rifle_ammo01.mdl");
	}

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/combine_rifle_ammo01.mdl");
		BaseClass::Spawn();
	}

	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, SIZE_AMMO_AR2_ALTFIRE, "AR2AltFire"))
		{
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}
		return false;
	}
};

LINK_ENTITY_TO_CLASS(item_ammo_ar2_altfire, CItem_AR2AltFireRound);

// ========================================================================
//	>> CItem_custom
// ========================================================================
class CItem_custom : public CItem
{
public:
	DECLARE_CLASS(CItem_custom, CItem);

	void Precache(void)
	{
		if (Q_stricmp(GetClassname(), "custom1"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom1_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom2"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom2_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom3"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom3_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom4"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom4_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom5"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom5_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom6"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom6_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom7"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom7_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom8"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom8_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom9"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom9_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom10"))
			//PrecacheParticleSystem("combineball");
			PrecacheModel(item_custom10_model.GetString());
	}

	void Spawn(void)
	{
		Precache();
		if (Q_stricmp(GetClassname(), "custom1"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom1_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom2"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom2_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom3"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom3_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom4"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom4_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom5"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom5_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom6"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom6_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom7"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom7_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom8"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom8_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom9"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom9_model.GetString());
		else if (Q_stricmp(GetClassname(), "custom10"))
			//PrecacheParticleSystem("combineball");
			SetModel(item_custom10_model.GetString());
		//SetModel("models/items/combine_rifle_ammo01.mdl");
		BaseClass::Spawn();
	}

	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (Q_stricmp(GetClassname(), "custom1")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom1_quantity.GetInt(), "CustomAmmo1"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom2")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom2_quantity.GetInt(), "CustomAmmo2"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom3")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom3_quantity.GetInt(), "CustomAmmo3"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom4")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom4_quantity.GetInt(), "CustomAmmo4"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom5")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom5_quantity.GetInt(), "CustomAmmo5"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom6")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom6_quantity.GetInt(), "CustomAmmo6"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom7")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom7_quantity.GetInt(), "CustomAmmo7"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom8")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom8_quantity.GetInt(), "CustomAmmo8"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom9")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom9_quantity.GetInt(), "CustomAmmo9"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		else if (Q_stricmp(GetClassname(), "custom10")) {
			if (ITEM_GiveAmmo(pPlayer, item_custom10_quantity.GetInt(), "CustomAmmo10"))
			{
				if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
				{
					UTIL_Remove(this);
				}
				return true;
			}
		}
		return false;
	}
};

LINK_ENTITY_TO_CLASS(item_ammo_custom1, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom2, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom3, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom4, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom5, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom6, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom7, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom8, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom9, CItem_custom);
LINK_ENTITY_TO_CLASS(item_ammo_custom10, CItem_custom);

// ==================================================================
// Ammo crate which will supply infinite ammo of the specified type
// ==================================================================

// Ammo types
enum
{
	AMMOCRATE_SMALL_ROUNDS,
	AMMOCRATE_MEDIUM_ROUNDS,
	AMMOCRATE_LARGE_ROUNDS,
	AMMOCRATE_RPG_ROUNDS,
	AMMOCRATE_BUCKSHOT,
	AMMOCRATE_GRENADES,
	AMMOCRATE_357,
	AMMOCRATE_CROSSBOW,
	AMMOCRATE_AR2_ALTFIRE,
	AMMOCRATE_SMG_ALTFIRE,
	NUM_AMMO_CRATE_TYPES,
};

// Ammo crate

class CItem_AmmoCrate : public CBaseAnimating
{
public:
	DECLARE_CLASS(CItem_AmmoCrate, CBaseAnimating);

	void	Spawn(void);
	void	Precache(void);
	bool	CreateVPhysics(void);

	virtual void HandleAnimEvent(animevent_t *pEvent);

	void	SetupCrate(void);
	void	OnRestore(void);

	//FIXME: May not want to have this used in a radius
	int		ObjectCaps(void) { return (BaseClass::ObjectCaps() | (FCAP_IMPULSE_USE | FCAP_USE_IN_RADIUS)); };
	void	Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

	void	InputKill(inputdata_t &data);
	void	CrateThink(void);

	virtual int OnTakeDamage(const CTakeDamageInfo &info);

protected:

	int		m_nAmmoType;
	int		m_nAmmoIndex;

	static const char *m_lpzModelNames[NUM_AMMO_CRATE_TYPES];
	static const char *m_lpzAmmoNames[NUM_AMMO_CRATE_TYPES];
	static int m_nAmmoAmounts[NUM_AMMO_CRATE_TYPES];
	static const char *m_pGiveWeapon[NUM_AMMO_CRATE_TYPES];

	float	m_flCloseTime;
	COutputEvent	m_OnUsed;
	CHandle< CBasePlayer > m_hActivator;

	DECLARE_DATADESC();
};

LINK_ENTITY_TO_CLASS(item_ammo_crate, CItem_AmmoCrate);

BEGIN_DATADESC(CItem_AmmoCrate)

DEFINE_KEYFIELD(m_nAmmoType, FIELD_INTEGER, "AmmoType"),

DEFINE_FIELD(m_flCloseTime, FIELD_FLOAT),
DEFINE_FIELD(m_hActivator, FIELD_EHANDLE),

// These can be recreated
//DEFINE_FIELD( m_nAmmoIndex,		FIELD_INTEGER ),
//DEFINE_FIELD( m_lpzModelNames,	FIELD_ ),
//DEFINE_FIELD( m_lpzAmmoNames,	FIELD_ ),
//DEFINE_FIELD( m_nAmmoAmounts,	FIELD_INTEGER ),

DEFINE_OUTPUT(m_OnUsed, "OnUsed"),

DEFINE_INPUTFUNC(FIELD_VOID, "Kill", InputKill),

DEFINE_THINKFUNC(CrateThink),

END_DATADESC()

//-----------------------------------------------------------------------------
// Animation events.
//-----------------------------------------------------------------------------

// Models names
const char *CItem_AmmoCrate::m_lpzModelNames[NUM_AMMO_CRATE_TYPES] =
{
	"models/items/ammocrate_pistol.mdl",	// Small rounds
	"models/items/ammocrate_smg1.mdl",		// Medium rounds
	"models/items/ammocrate_ar2.mdl",		// Large rounds
	"models/items/ammocrate_rockets.mdl",	// RPG rounds
	"models/items/ammocrate_buckshot.mdl",	// Buckshot
	"models/items/ammocrate_grenade.mdl",	// Grenades
	"models/items/ammocrate_smg1.mdl",		// 357
	"models/items/ammocrate_smg1.mdl",	// Crossbow

										//FIXME: This model is incorrect!
										"models/items/ammocrate_ar2.mdl",		// Combine Ball 
										"models/items/ammocrate_smg2.mdl",	    // smg grenade
};

// Ammo type names
const char *CItem_AmmoCrate::m_lpzAmmoNames[NUM_AMMO_CRATE_TYPES] =
{
	"Pistol",
	"SMG1",
	"AR2",
	"RPG_Round",
	"Buckshot",
	"Grenade",
	"357",
	"XBowBolt",
	"AR2AltFire",
	"SMG1_Grenade",
};

// Ammo amount given per +use
int CItem_AmmoCrate::m_nAmmoAmounts[NUM_AMMO_CRATE_TYPES] =
{
	300,	// Pistol
	300,	// SMG1
	300,	// AR2
	3,		// RPG rounds
	120,	// Buckshot
	5,		// Grenades
	50,		// 357
	50,		// Crossbow
	3,		// AR2 alt-fire
	5,
};

const char *CItem_AmmoCrate::m_pGiveWeapon[NUM_AMMO_CRATE_TYPES] =
{
	NULL,	// Pistol
	NULL,	// SMG1
	NULL,	// AR2
	NULL,		// RPG rounds
	NULL,	// Buckshot
	"weapon_frag",		// Grenades
	NULL,		// 357
	NULL,		// Crossbow
	NULL,		// AR2 alt-fire
	NULL,		// SMG alt-fire
};

#define	AMMO_CRATE_CLOSE_DELAY	1.5f

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::Spawn(void)
{
	Precache();

	BaseClass::Spawn();

	SetModel(STRING(GetModelName()));
	SetMoveType(MOVETYPE_NONE);
	SetSolid(SOLID_VPHYSICS);
	CreateVPhysics();

	ResetSequence(LookupSequence("Idle"));
	SetBodygroup(1, true);

	m_flCloseTime = gpGlobals->curtime;
	m_flAnimTime = gpGlobals->curtime;
	m_flPlaybackRate = 0.0;
	SetCycle(0);

	m_takedamage = DAMAGE_EVENTS_ONLY;

}

//------------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
bool CItem_AmmoCrate::CreateVPhysics(void)
{
	return (VPhysicsInitStatic() != NULL);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::Precache(void)
{
	SetupCrate();
	PrecacheModel(STRING(GetModelName()));

	PrecacheScriptSound("AmmoCrate.Open");
	PrecacheScriptSound("AmmoCrate.Close");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::SetupCrate(void)
{
	SetModelName(AllocPooledString(m_lpzModelNames[m_nAmmoType]));

	m_nAmmoIndex = GetAmmoDef()->Index(m_lpzAmmoNames[m_nAmmoType]);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::OnRestore(void)
{
	BaseClass::OnRestore();

	// Restore our internal state
	SetupCrate();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pActivator - 
//			*pCaller - 
//			useType - 
//			value - 
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	CBasePlayer *pPlayer = ToBasePlayer(pActivator);

	if (pPlayer == NULL)
		return;

	m_OnUsed.FireOutput(pActivator, this);

	int iSequence = LookupSequence("Open");

	// See if we're not opening already
	if (GetSequence() != iSequence)
	{
		Vector mins, maxs;
		trace_t tr;

		CollisionProp()->WorldSpaceAABB(&mins, &maxs);

		Vector vOrigin = GetAbsOrigin();
		vOrigin.z += (maxs.z - mins.z);
		mins = (mins - GetAbsOrigin()) * 0.2f;
		maxs = (maxs - GetAbsOrigin()) * 0.2f;
		mins.z = (GetAbsOrigin().z - vOrigin.z);

		UTIL_TraceHull(vOrigin, vOrigin, mins, maxs, MASK_SOLID, this, COLLISION_GROUP_NONE, &tr);

		if (tr.startsolid || tr.allsolid)
			return;

		m_hActivator = pPlayer;

		// Animate!
		ResetSequence(iSequence);

		// Make sound
		CPASAttenuationFilter sndFilter(this, "AmmoCrate.Open");
		EmitSound(sndFilter, entindex(), "AmmoCrate.Open");

		// Start thinking to make it return
		SetThink(&CItem_AmmoCrate::CrateThink);
		SetNextThink(gpGlobals->curtime + 0.1f);
	}

	// Don't close again for two seconds
	m_flCloseTime = gpGlobals->curtime + AMMO_CRATE_CLOSE_DELAY;
}

//-----------------------------------------------------------------------------
// Purpose: allows the crate to open up when hit by a crowbar
//-----------------------------------------------------------------------------
int CItem_AmmoCrate::OnTakeDamage(const CTakeDamageInfo &info)
{
	// if it's the player hitting us with a crowbar, open up
	CBasePlayer *player = ToBasePlayer(info.GetAttacker());
	if (player)
	{
		CBaseCombatWeapon *weapon = player->GetActiveWeapon();

		if (weapon && !stricmp(weapon->GetName(), "weapon_crowbar"))
		{
			// play the normal use sound
			player->EmitSound("HL2Player.Use");
			// open the crate
			Use(info.GetAttacker(), info.GetAttacker(), USE_TOGGLE, 0.0f);
		}
	}

	// don't actually take any damage
	return 0;
}


//-----------------------------------------------------------------------------
// Purpose: Catches the monster-specific messages that occur when tagged
//			animation frames are played.
// Input  : *pEvent - 
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::HandleAnimEvent(animevent_t *pEvent)
{
	if (pEvent->event == AE_AMMOCRATE_PICKUP_AMMO)
	{
		if (m_hActivator)
		{
			if (m_pGiveWeapon[m_nAmmoType] && !m_hActivator->Weapon_OwnsThisType(m_pGiveWeapon[m_nAmmoType]))
			{
				CBaseEntity *pEntity = CreateEntityByName(m_pGiveWeapon[m_nAmmoType]);
				CBaseCombatWeapon *pWeapon = dynamic_cast<CBaseCombatWeapon*>(pEntity);
				if (pWeapon)
				{
					pWeapon->SetAbsOrigin(m_hActivator->GetAbsOrigin());
					pWeapon->m_iPrimaryAmmoType = 0;
					pWeapon->m_iSecondaryAmmoType = 0;
					pWeapon->Spawn();
					if (!m_hActivator->BumpWeapon(pWeapon))
					{
						UTIL_Remove(pEntity);
					}
					else
					{
						SetBodygroup(1, false);
					}
				}
			}

			if (m_hActivator->GiveAmmo(m_nAmmoAmounts[m_nAmmoType], m_nAmmoIndex) != 0)
			{
				SetBodygroup(1, false);
			}
			m_hActivator = NULL;
		}
		return;
	}

	BaseClass::HandleAnimEvent(pEvent);
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::CrateThink(void)
{
	StudioFrameAdvance();
	DispatchAnimEvents(this);

	SetNextThink(gpGlobals->curtime + 0.1f);

	// Start closing if we're not already
	if (GetSequence() != LookupSequence("Close"))
	{
		// Not ready to close?
		if (m_flCloseTime <= gpGlobals->curtime)
		{
			m_hActivator = NULL;

			ResetSequence(LookupSequence("Close"));
		}
	}
	else
	{
		// See if we're fully closed
		if (IsSequenceFinished())
		{
			// Stop thinking
			SetThink(NULL);
			CPASAttenuationFilter sndFilter(this, "AmmoCrate.Close");
			EmitSound(sndFilter, entindex(), "AmmoCrate.Close");

			// FIXME: We're resetting the sequence here
			// but setting Think to NULL will cause this to never have
			// StudioFrameAdvance called. What are the consequences of that?
			ResetSequence(LookupSequence("Idle"));
			SetBodygroup(1, true);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &data - 
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::InputKill(inputdata_t &data)
{
	UTIL_Remove(this);
}

