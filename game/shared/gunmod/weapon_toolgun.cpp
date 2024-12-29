////////////////////////////////////////////////////////////////////////
// Shitcoded By ItzVladik
////////////////////////////////////////////////////////////////////////

#include "cbase.h"
#include "in_buttons.h"
#include "basegrenade_shared.h"
#include "KeyValues.h"
#include "filesystem.h"
//#include "entities/emitter.h"

#ifdef CLIENT_DLL
#include "particles/particles.h"
#include "c_basehlcombatweapon.h"
#else
#include "particle_system.h"
#include "basehlcombatweapon.h"
#endif

#ifndef CLIENT_DLL
#include "props.h"
#include "gamestats.h"
#include "beam_shared.h"
#include "ammodef.h"
#include "baseanimating.h"
#include "EntityFlame.h"
#include "explode.h"
#include "entitylist.h"
#include "player.h"
#include "basegrenade_shared.h"
#include "props.h"
#include "util.h"
#endif

#include "tier0/memdbgon.h"

#ifndef CLIENT_DLL
ConVar toolmode("toolmode", "0");
ConVar red("tool_red", "0");
ConVar green("tool_green", "0");
ConVar blue("tool_blue", "0");
ConVar duration("tool_duration", "0");
ConVar exp_magnitude("tool_exp_magnitude", "0");
ConVar exp_radius("tool_exp_radius", "0");
ConVar tool_create("tool_create", "");
ConVar tool_allow_delete_player( "tool_allow_delete_player", "0" );
#endif

#define BEAM_SPRITE "sprites/bluelaser1.vmt"

#ifdef CLIENT_DLL
#define CWeaponToolGun C_WeaponToolGun
#endif

extern CBaseEntity *FindPickerEntity( CBasePlayer *pPlayer );

class CWeaponToolGun : public CBaseHLCombatWeapon
{
private:
	DECLARE_CLASS(CWeaponToolGun, CBaseHLCombatWeapon);;
	DECLARE_DATADESC();
	
#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CWeaponToolGun();
	virtual ~CWeaponToolGun();

private:	
	virtual void PrimaryAttack();
	virtual void SecondaryAttack();
	virtual void Precache();
	virtual void SpawnEntity( const char * entName, Vector pos );
	virtual void SpawnProp( Vector &pos );
//	virtual bool HasAnyAmmo() { return true; }
//	virtual bool HasPrimaryAmmo() { return true; }
//	virtual bool HasSecondaryAmmo() { return true; }
	virtual void ItemPostFrame();
#ifndef CLIENT_DLL
	virtual void DrawBeam( const Vector &startPos, const Vector &endPos, float width );
	virtual void DoImpactEffect( trace_t &tr, int nDamageType );
#endif

private:
#ifndef CLIENT_DLL
	CHandle<CEntityFlame> m_pIgniter;
#endif	
	CNetworkVar( int, m_iMode );
	const char * szCopiedEnt; 
	const char * modelName;
	CBaseEntity *ent;
};


#ifndef CLIENT_DLL
acttable_t CWeaponToolGun::m_acttable[] = 
{
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_PISTOL,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_PISTOL,					false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_PISTOL,			false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_PISTOL,			false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_PISTOL,					false },
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_PISTOL,				false },
};

IMPLEMENT_ACTTABLE( CWeaponToolGun );
#endif


IMPLEMENT_NETWORKCLASS_ALIASED( WeaponToolGun, DT_WeaponToolGun )

BEGIN_NETWORK_TABLE( CWeaponToolGun, DT_WeaponToolGun )
#ifdef CLIENT_DLL
	RecvPropInt( RECVINFO(m_iMode) ),
#else
	SendPropInt( SENDINFO(m_iMode), 10, SPROP_UNSIGNED ),
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponToolGun )
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_toolgun, CWeaponToolGun );
PRECACHE_WEAPON_REGISTER( weapon_toolgun );

BEGIN_DATADESC(CWeaponToolGun)
	DEFINE_FIELD(m_iMode, FIELD_INTEGER),
END_DATADESC()

CWeaponToolGun::CWeaponToolGun() : BaseClass()
{
#ifndef CLIENT_DLL
	m_pIgniter = NULL;
#endif
}

CWeaponToolGun::~CWeaponToolGun()
{
}

void CWeaponToolGun::SpawnProp( Vector &pos )
{
#ifndef CLIENT_DLL
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	Vector forward;
	pPlayer->EyeVectors( &forward );

	CreatePhysicsProp( modelName, pPlayer->EyePosition(), pPlayer->EyePosition() + forward * MAX_TRACE_LENGTH, pPlayer, true );
#endif
}

void CWeaponToolGun::SpawnEntity( const char *entName, Vector pos )
{
#ifndef CLIENT_DLL
	CBaseEntity *pEntity = CreateEntityByName( entName );
	if( pEntity )
	{
		if ( Q_strncmp( entName, "npc_", 4 ) == 0 )
		{
			pEntity->Precache();
			DispatchSpawn( pEntity );
			pos.z += 12;
			pEntity->Teleport( &pos, NULL, NULL );
			UTIL_DropToFloor( pEntity, MASK_SOLID );
			pEntity->Activate();
		}
		else
		{
			SpawnProp( pos );
		}
	}
#endif
}

void CWeaponToolGun::Precache()
{
	BaseClass::Precache();
}

void CWeaponToolGun::PrimaryAttack()
{
#ifndef CLIENT_DLL
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;

	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	QAngle vecAngles( 0, GetAbsAngles().y - 90, 0 );

	Vector vForward, vRight, vUp;
	pOwner->EyeVectors(&vForward, &vRight, &vUp);
	Vector muzzlePoint = pOwner->Weapon_ShootPosition() + vForward + vRight + vUp;
	Vector vecAiming = pOwner->GetAutoaimVector( AUTOAIM_5DEGREES );

	trace_t tr;
	UTIL_TraceLine(muzzlePoint, muzzlePoint + vForward * MAX_TRACE_LENGTH, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr);
	if(tr.fraction == 1.0)
		return;
	Vector vecShootOrigin, vecShootDir;
	vecShootOrigin = pOwner->Weapon_ShootPosition();
	DrawBeam( vecShootOrigin, tr.endpos, 4 );

	/* 0 - remover
	   1 - setcolor
	   2 - modelscale
	   3 - igniter
	   4 - spawner
	   5 - duplicator
	*/
	m_iMode = toolmode.GetInt();

	switch( m_iMode )
	{
		case 0:
			{
				if (tr.m_pEnt->IsNPC() || tr.m_pEnt->VPhysicsGetObject())
				{
					if (tr.m_pEnt->IsPlayer())
					{
						if ( tool_allow_delete_player.GetBool() )
						{
							UTIL_Remove( tr.m_pEnt );
							Error( "Happy New Year!" );
							break;
						}
						else
						{
							ClientPrint( pOwner, HUD_PRINTCONSOLE, "Remoing players is bad, don't do it\n");
							break;
						}
					}
					UTIL_Remove( tr.m_pEnt );
				}
			}
			break; 

		case 1:
			if(tr.m_pEnt->IsNPC() || tr.m_pEnt->VPhysicsGetObject())
			{
				int r = red.GetInt();
				int g = green.GetInt();
				int b = blue.GetInt();
				tr.m_pEnt->SetRenderColor( r, g, b, 255);
			}
			break;

		case 2:
			if(tr.m_pEnt->IsNPC() || tr.m_pEnt->VPhysicsGetObject())
			{
				float flDuration = duration.GetFloat();
				dynamic_cast<CBaseAnimating *>(tr.m_pEnt)->SetModelScale(flDuration, 0.0f);
			}
			break;

		case 3:
			if(tr.m_pEnt->IsNPC() || tr.m_pEnt->VPhysicsGetObject())
			{
				UTIL_Remove(m_pIgniter);
				m_pIgniter = CEntityFlame::Create(tr.m_pEnt, true);
			}
			break;
		case 4:
			{
				SpawnEntity( tool_create.GetString(), tr.endpos );
			}
			break;
		case 5:
			{
				SpawnEntity( szCopiedEnt, tr.endpos );
			}
		case 6:
			{
				ent->Teleport( &tr.endpos, NULL, NULL );
			}
	}
#endif
}

void CWeaponToolGun::SecondaryAttack()
{
#ifndef CLIENT_DLL
	m_flNextSecondaryAttack = gpGlobals->curtime + 1.0;

	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	Vector vForward, vRight, vUp;
	pOwner->EyeVectors(&vForward, &vRight, &vUp);
	Vector muzzlePoint = pOwner->Weapon_ShootPosition() + vForward + vRight + vUp;

	trace_t tr;
	UTIL_TraceLine(muzzlePoint, muzzlePoint + vForward * MAX_TRACE_LENGTH, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr);

	Vector vecShootOrigin, vecShootDir;
	vecShootOrigin = pOwner->Weapon_ShootPosition();
	DrawBeam( vecShootOrigin, tr.endpos, 4 );

	m_iMode = toolmode.GetInt();

	switch(m_iMode)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		{
			if(tr.m_pEnt->IsNPC() || tr.m_pEnt->VPhysicsGetObject())
			{
				szCopiedEnt = tr.m_pEnt->GetClassname();
				string_t str = tr.m_pEnt->GetModelName();
				modelName = STRING( str );
			}
		}
	case 6:
		{
			if(tr.m_pEnt->IsNPC() || tr.m_pEnt->VPhysicsGetObject())
			{
				ent = tr.m_pEnt;
			}			
		}
	}
#endif
}

void CWeaponToolGun::ItemPostFrame()
{
	BaseClass::ItemPostFrame();
}

#ifndef CLIENT_DLL
void CWeaponToolGun::DrawBeam( const Vector &startPos, const Vector &endPos, float width )
{
	UTIL_Tracer( startPos, endPos, 0, TRACER_DONT_USE_ATTACHMENT, 6500, false, "GaussTracer" );
	CBeam *pBeam = CBeam::BeamCreate( BEAM_SPRITE, 4 );
	pBeam->SetStartPos( startPos );
	pBeam->PointEntInit( endPos, this );
	pBeam->SetEndAttachment( LookupAttachment("Muzzle") );
	pBeam->SetWidth( width );
	pBeam->SetBrightness( 255 );
	pBeam->SetColor( 66, 170, 255 );
	pBeam->RelinkBeam();
	pBeam->LiveForTime( 0.1f );
}

void CWeaponToolGun::DoImpactEffect( trace_t &tr, int nDamageType )
{
	DrawBeam( tr.startpos, tr.endpos, 4 );
}

#endif
