
#ifndef	WEAPONCUSTOM_H
#define	WEAPONCUSTOM_H

#include "basehlcombatweapon.h"
#include "weapon_rpg.h"

class CWeaponCustom : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponCustom, CBaseHLCombatWeapon);
public:

	CWeaponCustom(void);

	void	PrimaryAttack(void);
	void	Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator);

	float	WeaponAutoAimScale()	{ return 0.6f; }

	virtual const Vector& GetBulletSpread(void)
	{
		// Handle NPCs first
		static Vector npcCone = VECTOR_CONE_5DEGREES;
		if (GetOwner() && GetOwner()->IsNPC())
			return npcCone;
		else
			return CBaseCombatWeapon::GetBulletSpread();
	}

	int		CapabilitiesGet(void) { return bits_CAP_WEAPON_RANGE_ATTACK1; }


	virtual float GetFireRate(void)
	{
		if (GetOwner() && GetOwner()->IsNPC())
			return 1.0f;
		else
			return BaseClass::GetFireRate();
	}

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();
};

#define CustomWeaponAdd( num )										\
class CWeaponCustom##num : public CWeaponCustom						\
{																	\
	DECLARE_DATADESC();												\
	public:															\
	DECLARE_CLASS( CWeaponCustom##num, CWeaponCustom );				\
	CWeaponCustom##num() {};										\
	DECLARE_SERVERCLASS();											\
};																	\
IMPLEMENT_SERVERCLASS_ST(CWeaponCustom##num, DT_WeaponCustom##num)	\
END_SEND_TABLE()													\
BEGIN_DATADESC( CWeaponCustom##num )										\
END_DATADESC()														\
LINK_ENTITY_TO_CLASS( weapon_custom##num, CWeaponCustom##num );		\
PRECACHE_WEAPON_REGISTER(weapon_custom##num);

#define CustomWeaponNamedAdd( customname )										\
class CWeaponCustomNamed##customname : public CWeaponCustom						\
{																	\
	DECLARE_DATADESC();												\
	public:															\
	DECLARE_CLASS( CWeaponCustomNamed##customname, CWeaponCustom );				\
	CWeaponCustomNamed##customname() {};										\
	DECLARE_SERVERCLASS();											\
};																	\
IMPLEMENT_SERVERCLASS_ST(CWeaponCustomNamed##customname, DT_WeaponCustomNamed##customname)	\
END_SEND_TABLE()													\
BEGIN_DATADESC( CWeaponCustomNamed##customname )										\
END_DATADESC()														\
LINK_ENTITY_TO_CLASS( weapon_##customname, CWeaponCustomNamed##customname );		\
PRECACHE_WEAPON_REGISTER(weapon_##customname);
#endif	//WEAPONCUSTOM_H