//========= Made by The SMOD13 Team, Some rights reserved. ============//
//
// Purpose: New ammo defines, new ammo items and new ammo convars
//
//========================================================================//

#include "cbase.h"
#include "ammodef.h"
#include "hl2_shareddefs.h"

#ifndef CLIENT_DLL
#include "globalstate.h"
#include "hl1_items.h"
#endif

//New convars
ConVar	sk_plr_dmg_ak47("sk_plr_dmg_ak47", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_ak47("sk_npc_dmg_ak47", "0", FCVAR_REPLICATED);
ConVar	sk_max_ak47("sk_max_ak47", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_awp("sk_plr_dmg_awp", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_awp("sk_npc_dmg_awp", "0", FCVAR_REPLICATED);
ConVar	sk_max_awp("sk_max_awp", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_manhack("sk_plr_dmg_manhack", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_manhack("sk_npc_dmg_manhack", "0", FCVAR_REPLICATED);
ConVar	sk_max_manhack("sk_max_manhack", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_flare("sk_plr_dmg_flare", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_flare("sk_npc_dmg_flare", "0", FCVAR_REPLICATED);
ConVar	sk_max_flare("sk_max_flare", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_blackhole("sk_plr_dmg_blackhole", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_blackhole("sk_npc_dmg_blackhole", "0", FCVAR_REPLICATED);
ConVar	sk_max_blackhole("sk_max_blackhole", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_annabelle("sk_plr_dmg_annabelle", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_annabelle("sk_npc_dmg_annabelle", "0", FCVAR_REPLICATED);
ConVar	sk_max_annabelle("sk_max_annabelle", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_laser("sk_plr_dmg_laser", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_laser("sk_npc_dmg_laser", "0", FCVAR_REPLICATED);
ConVar	sk_max_laser("sk_max_laser", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_katana("sk_plr_dmg_katana", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_katana("sk_npc_dmg_katana", "0", FCVAR_REPLICATED);
ConVar	sk_max_katana("sk_max_katana", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_minigun("sk_plr_dmg_minigun", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_minigun("sk_npc_dmg_minigun", "0", FCVAR_REPLICATED);
ConVar	sk_max_minigun("sk_max_minigun", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_psp("sk_plr_dmg_psp", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_psp("sk_npc_dmg_psp", "0", FCVAR_REPLICATED);
ConVar	sk_max_psp("sk_max_psp", "0", FCVAR_REPLICATED);

//Ported HL1 ammo types
ConVar sk_plr_dmg_egon_narrow		( "sk_plr_dmg_egon_narrow",		"0", FCVAR_REPLICATED );
ConVar sk_plr_dmg_egon_wide			( "sk_plr_dmg_egon_wide",		"0", FCVAR_REPLICATED );
ConVar sk_max_uranium				( "sk_max_uranium",				"0", FCVAR_REPLICATED );

ConVar sk_plr_dmg_gauss				( "sk_plr_dmg_gauss",			"0", FCVAR_REPLICATED );
ConVar sk_npc_dmg_gauss				( "sk_npc_dmg_gauss",			"0", FCVAR_REPLICATED );

ConVar sk_plr_dmg_hornet			( "sk_plr_dmg_hornet",			"0", FCVAR_REPLICATED );
ConVar sk_npc_dmg_hornet			( "sk_npc_dmg_hornet",			"0", FCVAR_REPLICATED );
ConVar sk_max_hornet				( "sk_max_hornet",				"0", FCVAR_REPLICATED );

ConVar sk_max_snark					( "sk_max_snark",				"0", FCVAR_REPLICATED );
ConVar sk_plr_dmg_snark				( "sk_plr_dmg_snark",			"0", FCVAR_REPLICATED );
ConVar sk_npc_dmg_snark				( "sk_npc_dmg_snark",			"0", FCVAR_REPLICATED );

ConVar sk_plr_dmg_xbow_bolt_plr		( "sk_plr_dmg_xbow_bolt_plr",	"0", FCVAR_REPLICATED );
ConVar sk_plr_dmg_xbow_bolt_npc		( "sk_plr_dmg_xbow_bolt_npc",	"0", FCVAR_REPLICATED );
ConVar sk_max_xbow_bolt				( "sk_max_xbow_bolt",			"0", FCVAR_REPLICATED );

ConVar sk_plr_dmg_hl1tripmine		( "sk_plr_dmg_hl1tripmine",		"0", FCVAR_REPLICATED );
ConVar sk_npc_dmg_hl1tripmine		( "sk_npc_dmg_hl1tripmine",		"0", FCVAR_REPLICATED );
ConVar sk_max_tripmine				( "sk_max_tripmine",			"0", FCVAR_REPLICATED );

ConVar sk_plr_dmg_hl1satchel		( "sk_plr_dmg_hl1satchel",		"0", FCVAR_REPLICATED );
ConVar sk_npc_dmg_hl1satchel		( "sk_npc_dmg_hl1satchel",		"0", FCVAR_REPLICATED );
ConVar sk_max_satchel				( "sk_max_satchel",				"0", FCVAR_REPLICATED );

//Convars for existing ammo that didn't have them
ConVar	sk_plr_dmg_striderminigun("sk_plr_dmg_striderminigun", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_striderminigun("sk_npc_dmg_striderminigun", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_striderminigunep2("sk_npc_dmg_striderminigunep2", "0", FCVAR_REPLICATED);
ConVar	sk_max_striderminigun("sk_max_striderminigun", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_thumper("sk_plr_dmg_thumper", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_thumper("sk_npc_dmg_thumper", "0", FCVAR_REPLICATED);
ConVar	sk_max_thumper("sk_max_thumper", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_gravity("sk_plr_dmg_gravity", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_gravity("sk_npc_dmg_gravity", "0", FCVAR_REPLICATED);
ConVar	sk_max_gravity("sk_max_gravity", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_extinguisher("sk_plr_dmg_extinguisher", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_extinguisher("sk_npc_dmg_extinguisher", "0", FCVAR_REPLICATED);
ConVar	sk_max_extinguisher("sk_max_extinguisher", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_battery("sk_plr_dmg_battery", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_battery("sk_npc_dmg_battery", "0", FCVAR_REPLICATED);
ConVar	sk_max_battery("sk_max_battery", "0", FCVAR_REPLICATED);

ConVar	sk_max_gunship("sk_max_gunship", "0", FCVAR_REPLICATED);

ConVar	sk_max_airboat("sk_max_airboat", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_striderdirect("sk_plr_dmg_striderdirect", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_striderdirect("sk_npc_dmg_striderdirect", "0", FCVAR_REPLICATED);
ConVar	sk_max_striderdirect("sk_max_striderdirect", "0", FCVAR_REPLICATED);

ConVar	sk_max_helicopter("sk_max_helicopter", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_ar2_altfire("sk_plr_dmg_ar2_altfire", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_ar2_altfire("sk_npc_dmg_ar2_altfire", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_hopwire("sk_plr_dmg_hopwire", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_hopwire("sk_npc_dmg_hopwire", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_gunship_heavy("sk_plr_dmg_gunship_heavy", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_gunship_heavy("sk_npc_dmg_gunship_heavy", "0", FCVAR_REPLICATED);
ConVar	sk_max_gunship_heavy("sk_max_gunship_heavy", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_prototype("sk_plr_dmg_prototype", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_prototype("sk_npc_dmg_prototype", "0", FCVAR_REPLICATED);
ConVar	sk_max_prototype("sk_max_prototype", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_slam("sk_plr_dmg_slam", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_slam("sk_npc_dmg_slam", "0", FCVAR_REPLICATED);
ConVar	sk_max_slam("sk_max_slam", "0", FCVAR_REPLICATED);

// shared ammo definition
// JAY: Trying to make a more physical bullet response
#define BULLET_MASS_GRAINS_TO_LB(grains)	(0.002285*(grains)/16.0f)
#define BULLET_MASS_GRAINS_TO_KG(grains)	lbs2kg(BULLET_MASS_GRAINS_TO_LB(grains))

// exaggerate all of the forces, but use real numbers to keep them consistent
#define BULLET_IMPULSE_EXAGGERATION			3.5
// convert a velocity in ft/sec and a mass in grains to an impulse in kg in/s
#define BULLET_IMPULSE(grains, ftpersec)	((ftpersec)*12*BULLET_MASS_GRAINS_TO_KG(grains)*BULLET_IMPULSE_EXAGGERATION)


CAmmoDef *GetAmmoDef()
{
	static CAmmoDef def;
	static bool bInitted = false;
	
	if ( !bInitted )
	{
		bInitted = true;

		def.AddAmmoType("AR2",				DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_ar2",			"sk_npc_dmg_ar2",			"sk_max_ar2",			BULLET_IMPULSE(200, 1225), 0 );
		def.AddAmmoType("AlyxGun",			DMG_BULLET,					TRACER_LINE_AND_WHIZ,			"sk_plr_dmg_alyxgun",		"sk_npc_dmg_alyxgun",		"sk_max_alyxgun",		BULLET_IMPULSE(200, 1225), 0 );
		def.AddAmmoType("Pistol",			DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_pistol",		"sk_npc_dmg_pistol",		"sk_max_pistol",		BULLET_IMPULSE(200, 1225), 0 );
		def.AddAmmoType("SMG1",				DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_smg1",			"sk_npc_dmg_smg1",			"sk_max_smg1",			BULLET_IMPULSE(200, 1225), 0 );
		def.AddAmmoType("357",				DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_357",			"sk_npc_dmg_357",			"sk_max_357",			BULLET_IMPULSE(800, 5000), 0 );
		def.AddAmmoType("XBowBolt",			DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_crossbow",		"sk_npc_dmg_crossbow",		"sk_max_crossbow",		BULLET_IMPULSE(800, 8000), 0 );

		def.AddAmmoType("Buckshot",			DMG_BULLET | DMG_BUCKSHOT,	TRACER_LINE,			"sk_plr_dmg_buckshot",		"sk_npc_dmg_buckshot",		"sk_max_buckshot",		BULLET_IMPULSE(400, 1200), 0 );
		def.AddAmmoType("RPG_Round",		DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_rpg_round",		"sk_npc_dmg_rpg_round",		"sk_max_rpg_round",		0, 0 );
		def.AddAmmoType("SMG1_Grenade",		DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_smg1_grenade",	"sk_npc_dmg_smg1_grenade",	"sk_max_smg1_grenade",	0, 0 );
		def.AddAmmoType("SniperRound",		DMG_BULLET | DMG_SNIPER,	TRACER_LINE_AND_WHIZ,			"sk_plr_dmg_sniper_round",	"sk_npc_dmg_sniper_round",	"sk_max_sniper_round",	BULLET_IMPULSE(650, 6000), 0 );
		def.AddAmmoType("SniperPenetratedRound", DMG_BULLET | DMG_SNIPER, TRACER_NONE,			"sk_dmg_sniper_penetrate_plr", "sk_dmg_sniper_penetrate_npc", "sk_max_sniper_round", BULLET_IMPULSE(150, 6000), 0 );
		def.AddAmmoType("Grenade",			DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_grenade",		"sk_npc_dmg_grenade",		"sk_max_grenade",		0, 0);
		def.AddAmmoType("Thumper",			DMG_SONIC,					TRACER_NONE,			"sk_plr_dmg_thumper", "sk_npc_dmg_thumper", "sk_max_thumper", 0, 0 );
		def.AddAmmoType("Gravity",			DMG_CLUB,					TRACER_NONE,			"sk_plr_dmg_gravity",	"sk_npc_dmg_gravity", "sk_max_gravity", 0, 0 );
		def.AddAmmoType("Extinguisher",		DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_extinguisher",	"sk_npc_dmg_extinguisher", "sk_max_extinguisher", 0, 0 );
		def.AddAmmoType("Battery",			DMG_CLUB,					TRACER_NONE,			"sk_plr_dmg_battery", "sk_npc_dmg_battery", "sk_max_battery", 0, 0 );
		def.AddAmmoType("GaussEnergy",		DMG_SHOCK,					TRACER_NONE,			"sk_jeep_gauss_damage",		"sk_jeep_gauss_damage", "sk_max_gauss_round", BULLET_IMPULSE(650, 8000), 0 ); // hit like a 10kg weight at 400 in/s
		def.AddAmmoType("CombineCannon",	DMG_BULLET,					TRACER_LINE,			"sk_npc_dmg_gunship_to_plr", "sk_npc_dmg_gunship", "sk_max_gunship", 1.5 * 750 * 12, 0 ); // hit like a 1.5kg weight at 750 ft/s
		def.AddAmmoType("AirboatGun",		DMG_AIRBOAT,				TRACER_LINE,			"sk_plr_dmg_airboat",		"sk_npc_dmg_airboat",		"sk_max_airboat",					BULLET_IMPULSE(10, 600), 0 );

#ifndef CLIENT_DLL
		//Smarter way to do it since we're both HL2 AND Episodic.
		if (!Q_strnicmp(STRING(gpGlobals->mapname), "ep1", 3) || !Q_strnicmp(STRING(gpGlobals->mapname), "ep2", 3))
			def.AddAmmoType("StriderMinigun",	DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_striderminigun", "sk_npc_dmg_striderminigunep2", "sk_max_striderminigun", 1.0 * 750 * 12, 0 ); // hit like a 1.0kg weight at 750 ft/s
		else
			def.AddAmmoType("StriderMinigun",	DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_striderminigun", "sk_npc_dmg_striderminigun", "sk_max_striderminigun", 1.0 * 750 * 12, 0 ); // hit like a 1.0kg weight at 750 ft/s
#else
		//Client doesn't have gpGlobals->mapname so on the client just always use the HL2 value
		def.AddAmmoType("StriderMinigun",	DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_striderminigun", "sk_npc_dmg_striderminigun", "sk_max_striderminigun", 1.0 * 750 * 12, 0 ); // hit like a 1.0kg weight at 750 ft/s
#endif

		def.AddAmmoType("StriderMinigunDirect",	DMG_BULLET,				TRACER_LINE,			"sk_plr_dmg_striderdirect", "sk_npc_dmg_striderdirect", "sk_max_striderdirect", 1.0 * 750 * 12, 0 ); // hit like a 1.0kg weight at 750 ft/s
		def.AddAmmoType("HelicopterGun",	DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_npc_dmg_helicopter_to_plr", "sk_npc_dmg_helicopter",	"sk_max_helicopter",	BULLET_IMPULSE(400, 1225), AMMO_INTERPRET_PLRDAMAGE_AS_DAMAGE_TO_PLAYER );
		def.AddAmmoType("AR2AltFire",		DMG_DISSOLVE,				TRACER_NONE,			"sk_plr_dmg_ar2_altfire", "sk_npc_dmg_ar2_altfire", "sk_max_ar2_altfire", 0, 0 );
		
		def.AddAmmoType("slam",				DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_slam",			"sk_npc_dmg_slam",			"sk_max_slam",			0,							0 );
		
#ifdef HL2_EPISODIC
		def.AddAmmoType("Hopwire",			DMG_BLAST,					TRACER_NONE,			"sk_plr_dmg_hopwire",		"sk_npc_dmg_hopwire",		"sk_max_hopwire",		0, 0);
		def.AddAmmoType("CombineHeavyCannon",	DMG_BULLET,				TRACER_LINE,			"sk_plr_dmg_gunship_heavy",	"sk_npc_dmg_gunship_heavy", "sk_max_gunship_heavy", 10 * 750 * 12, 0 ); // hit like a 10 kg weight at 750 ft/s
		def.AddAmmoType("ammo_proto1",			DMG_BULLET,				TRACER_LINE,			"sk_plr_dmg_prototype", "sk_npc_dmg_prototype", "sk_max_prototype", 0, 0 );
#endif // HL2_EPISODIC

		//Ported HL1 ammo types
		def.AddAmmoType("Uranium",			DMG_ENERGYBEAM,				TRACER_NONE, "sk_plr_dmg_gauss",		"sk_npc_dmg_gauss",		"sk_max_uranium",		0, 0 );
		def.AddAmmoType("Hornet",			DMG_BULLET,					TRACER_NONE, "sk_plr_dmg_hornet",		"sk_npc_dmg_hornet",	"sk_max_hornet",		BULLET_IMPULSE(100, 1200), 0 );
		def.AddAmmoType("Snark",			DMG_SLASH,					TRACER_NONE, "sk_plr_dmg_snark",		"sk_npc_dmg_snark",		"sk_max_snark",			0, 0 );
		def.AddAmmoType("XBowDart",			DMG_BULLET | DMG_NEVERGIB,	TRACER_LINE, "sk_plr_dmg_xbow_bolt_plr","sk_plr_dmg_xbow_bolt_npc","sk_max_xbow_bolt",	BULLET_IMPULSE( 200, 1200), 0 ); //Renamed to XBowDart to not conflict with HL2's
		def.AddAmmoType("TripMine",			DMG_BURN | DMG_BLAST,		TRACER_NONE, "sk_plr_dmg_hl1tripmine",	"sk_npc_dmg_hl1tripmine","sk_max_tripmine",		0, 0 );
		def.AddAmmoType("Satchel",			DMG_BURN | DMG_BLAST,		TRACER_NONE, "sk_plr_dmg_hl1satchel",	"sk_npc_dmg_hl1satchel","sk_max_satchel",		0, 0 );

		//SMOD custom ammo
		def.AddAmmoType("AssaultRifle", DMG_BULLET, TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_ak47",		"sk_npc_dmg_ak47",		"sk_max_ak47",		BULLET_IMPULSE(400, 1225), 0);
		def.AddAmmoType("Manhack",		DMG_CLUB,	TRACER_NONE,			"sk_plr_dmg_manhack",	"sk_npc_dmg_manhack",	"sk_max_manhack",	0, 0);
		def.AddAmmoType("FlareRound",	DMG_BURN,	TRACER_NONE,			"sk_plr_dmg_flare",		"sk_npc_dmg_flare",		"sk_max_flare",		0, 0);
		def.AddAmmoType("AWP",			DMG_BULLET,	TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_awp",		"sk_npc_dmg_awp",		"sk_max_awp",		BULLET_IMPULSE(650, 6000), 0 );
		def.AddAmmoType("BlackHole",	DMG_BLAST,	TRACER_NONE,			"sk_plr_dmg_blackhole",	"sk_npc_dmg_blackhole",	"sk_max_blackhole",	0, 0);
		def.AddAmmoType("Annabelle",	DMG_BULLET,	TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_annabelle",	"sk_npc_dmg_annabelle",	"sk_max_annabelle",	BULLET_IMPULSE(800, 5000), 0 );
		def.AddAmmoType("Laser",		DMG_DISSOLVE,TRACER_NONE,			"sk_plr_dmg_laser",		"sk_npc_dmg_laser",		"sk_max_laser",		BULLET_IMPULSE(200, 1225), 0);
		def.AddAmmoType("Katana",		DMG_SLASH,	TRACER_NONE,			"sk_plr_dmg_katana",	"sk_npc_dmg_katana",	"sk_max_katana",	BULLET_IMPULSE(200, 1225), 0);
		def.AddAmmoType("Gatling",		DMG_BULLET,	TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_minigun",	"sk_npc_dmg_minigun",	"sk_max_minigun",	BULLET_IMPULSE(400, 1225), 0);
		def.AddAmmoType("PSPGame",		DMG_BULLET,	TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_psp",		"sk_npc_dmg_psp",		"sk_max_psp",		BULLET_IMPULSE(200, 1225), 0 );
		
		
	}

	return &def;
}

//Inside this ifndef is where we define new ammo types
#ifndef CLIENT_DLL

#define ITEM_AMMO_SNARK_GIVE		5
#define ITEM_AMMO_ALYXGUN_GIVE		30
#define ITEM_AMMO_AK47_GIVE			30
#define ITEM_AMMO_GAUSS_GIVE		20
#define ITEM_AMMO_CROSSBOW_GIVE		5
#define ITEM_AMMO_ANNABELLE_GIVE	12
extern int ITEM_GiveAmmo(CBasePlayer *pPlayer, float flCount, const char *pszAmmoName, bool bSuppressSound = false);

// ========================================================================
//	>> SNARK ammo
// ========================================================================

class CItem_SnarkAmmo : public CHL1Item
{
public:
	DECLARE_CLASS(CItem_SnarkAmmo, CHL1Item);

	void Spawn(void)
	{
		Precache();
		SetModel("models/aliens/snarknest.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/aliens/snarknest.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (!pPlayer->Weapon_OwnsThisType("weapon_snark"))
		{
			pPlayer->GiveNamedItem("weapon_snark");
			if (g_pGameRules->ItemShouldRespawn(this) == GR_ITEM_RESPAWN_NO)
			{
				UTIL_Remove(this);
			}
			return true;
		}

		if (pPlayer->GiveAmmo(ITEM_AMMO_SNARK_GIVE, "Snark"))
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
LINK_ENTITY_TO_CLASS(item_ammo_snark, CItem_SnarkAmmo);
PRECACHE_REGISTER(item_ammo_snark);

// ========================================================================
//	>> BoxAlyxRounds
// ========================================================================
class CItem_BoxAlyxRounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxAlyxRounds, CItem);

	void Spawn( void )
	{ 
		Precache( );
		SetModel( "models/items/boxalyxrounds.mdl" );
		BaseClass::Spawn( );
	}
	void Precache( void )
	{
		PrecacheModel ("models/items/boxalyxrounds.mdl");
	}
	bool MyTouch( CBasePlayer *pPlayer )
	{
		if (ITEM_GiveAmmo(pPlayer, ITEM_AMMO_ALYXGUN_GIVE, "AlyxGun"))
		{
			if ( g_pGameRules->ItemShouldRespawn( this ) == GR_ITEM_RESPAWN_NO )
			{
				UTIL_Remove(this);	
			}

			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_ammo_alyxgun, CItem_BoxAlyxRounds);
PRECACHE_REGISTER(item_ammo_alyxgun);

// ========================================================================
//	>> BoxAK47Rounds
// ========================================================================
class CItem_BoxAK47Rounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxAK47Rounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/m16_cartridge.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/m16_cartridge.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, ITEM_AMMO_ALYXGUN_GIVE, "AssaultRifle"))
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
LINK_ENTITY_TO_CLASS(item_ammo_ak47, CItem_BoxAK47Rounds);
PRECACHE_REGISTER(item_ammo_ak47);

// ========================================================================
//	>> Gauss ammo
// ========================================================================
class CItem_GaussAmmo : public CHL1Item
{
public:
	DECLARE_CLASS(CItem_GaussAmmo, CHL1Item);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/gaussammo.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/gaussammo.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (pPlayer->GiveAmmo(ITEM_AMMO_GAUSS_GIVE, "Uranium"))
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
LINK_ENTITY_TO_CLASS(item_ammo_uranium, CItem_GaussAmmo);
PRECACHE_REGISTER(item_ammo_uranium);

// ========================================================================
//	>> Crossbow DARTS
// ========================================================================
class CItem_DartAmmo : public CHL1Item
{
public:
	DECLARE_CLASS(CItem_DartAmmo, CHL1Item);

	void Spawn( void )
	{ 
		Precache();
		SetModel( "models/items/crossbow_clip.mdl" );
		BaseClass::Spawn();
	}
	void Precache( void )
	{
		PrecacheModel( "models/items/crossbow_clip.mdl" );
	}
	bool MyTouch( CBasePlayer *pPlayer )
	{
		if (pPlayer->GiveAmmo( ITEM_AMMO_CROSSBOW_GIVE, "XBowDart" ) )
		{
			if ( g_pGameRules->ItemShouldRespawn( this ) == GR_ITEM_RESPAWN_NO )
			{
				UTIL_Remove( this );
			}
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(item_ammo_darts, CItem_DartAmmo);
PRECACHE_REGISTER(item_ammo_darts);

// ========================================================================
//	>> BoxANNABELLERounds
// ========================================================================
class CItem_BoxANNABALLERounds : public CItem
{
public:
	DECLARE_CLASS(CItem_BoxANNABALLERounds, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/boxannabelle.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/boxannabelle.mdl");
	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		if (ITEM_GiveAmmo(pPlayer, ITEM_AMMO_ANNABELLE_GIVE, "Annabelle"))
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
LINK_ENTITY_TO_CLASS(item_ammo_rifle, CItem_BoxANNABALLERounds);
PRECACHE_REGISTER(item_ammo_rifle);

#endif
