#include "cbase.h"
#include "hl2_gamerules.h"

#ifdef GAME_DLL
#include "hl1_weapon_crowbar.h"
#include "hl1_weapon_snark.h"
#endif
#include "hl1mp_weapon_357.h"
#include "hl1mp_weapon_crossbow.h"
#include "hl1mp_weapon_rpg.h"
#include "hl1mp_weapon_shotgun.h"
#include "hl1mp_weapon_egon.h"
#include "hl1mp_weapon_hornetgun.h"
#include "hl1mp_weapon_gauss.h"

#ifdef GAME_DLL
#include "weapon_crowbar.h"
#include "weapon_357.h"
#include "weapon_crossbow.h"
#include "weapon_rpg.h"
#include "weapon_shotgun.h"
#endif

#include "weapon_egon.h"
#include "weapon_gauss.h"
#include "weapon_hornetgun.h"
#ifdef GAME_DLL
#include "weapon_snark.h"
#endif

#include "tier0/memdbgon.h"

#ifdef GAME_DLL
template <class HL1, class HL2>
class CHL1SharedEntity : public IEntityFactory
{
	public:
		CHL1SharedEntity(const char *HL2Classname, const char *HL1Classname)
		{
			EntityFactoryDictionary()->InstallFactory(this, HL2Classname);
			m_pHL1Classname = HL1Classname;
		}

	private:
		virtual IServerNetworkable *Create(const char *pClassName)
		{
			CBaseEntity *pEnt = NULL;
			if(HL2GameRules()->IsInHL1Map())
				pEnt = _CreateEntityTemplate<HL1>((HL1 *)NULL, m_pHL1Classname);
			else
				pEnt = _CreateEntityTemplate<HL2>((HL2 *)NULL, pClassName);
			return pEnt->NetworkProp();
		}

		virtual void Destroy(IServerNetworkable *pNetworkable)
		{
			if(pNetworkable)
				pNetworkable->Release();
		}

		virtual size_t GetEntitySize()
		{
			if(HL2GameRules()->IsInHL1Map())
				return sizeof(HL1);
			else
				return sizeof(HL2);
		}

	private:
		const char *m_pHL1Classname;
};

static CHL1SharedEntity<CHL1Weapon357, CWeapon357> weapon_357("weapon_357", "hl1_357");
static CHL1SharedEntity<CHL1WeaponShotgun, CWeaponShotgun> weapon_shotgun("weapon_shotgun", "hl1_shotgun");
static CHL1SharedEntity<CHL1WeaponCrowbar, CWeaponCrowbar> weapon_crowbar("weapon_crowbar", "hl1_crowbar");
static CHL1SharedEntity<CHL1WeaponCrossbow, CWeaponCrossbow> weapon_crossbow("weapon_crossbow", "hl1_crossbow");
static CHL1SharedEntity<CHL1WeaponRPG, CWeaponRPG> weapon_rpg("weapon_rpg", "hl1_rpg");

static CHL1SharedEntity<CWeaponSnark, CWeaponSnarkHD> weapon_snark("weapon_snark", "hl1_snark");
static CHL1SharedEntity<CWeaponEgon, CWeaponEgonHD> weapon_egon("weapon_egon", "hl1_egon");
static CHL1SharedEntity<CWeaponGauss, CWeaponGaussHD> weapon_gauss("weapon_gauss", "hl1_gauss");
static CHL1SharedEntity<CWeaponHgun, CWeaponHgunHD> weapon_hornetgun("weapon_hornetgun", "hl1_hornetgun");
#endif

#ifdef CLIENT_DLL
LINK_ENTITY_TO_CLASS(hl1_357, CHL1Weapon357);
LINK_ENTITY_TO_CLASS(hl1_shotgun, CHL1WeaponShotgun);
//LINK_ENTITY_TO_CLASS(hl1_crowbar, CHL1WeaponCrowbar);
LINK_ENTITY_TO_CLASS(hl1_crossbow, CHL1WeaponCrossbow);
LINK_ENTITY_TO_CLASS(hl1_rpg, CHL1WeaponRPG);

//LINK_ENTITY_TO_CLASS(hl1_snark, CWeaponSnark);
LINK_ENTITY_TO_CLASS(hl1_egon, CWeaponEgon);
LINK_ENTITY_TO_CLASS(hl1_gauss, CWeaponGauss);
LINK_ENTITY_TO_CLASS(hl1_hornetgun, CWeaponHgun);
#endif

PRECACHE_WEAPON_REGISTER(weapon_357);
PRECACHE_WEAPON_REGISTER(weapon_shotgun);
PRECACHE_WEAPON_REGISTER(weapon_crowbar);
PRECACHE_WEAPON_REGISTER(weapon_crossbow);
PRECACHE_WEAPON_REGISTER(weapon_rpg);

PRECACHE_WEAPON_REGISTER(weapon_snark);
PRECACHE_WEAPON_REGISTER(weapon_egon);
PRECACHE_WEAPON_REGISTER(weapon_gauss);
PRECACHE_WEAPON_REGISTER(weapon_hornetgun);