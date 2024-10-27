#include "cbase.h"
#include "../../tf_bot.h"
#include "tf_gamerules.h"
#include "tf_obj_dispenser.h"
#include "tf_bot_engineer_build.h"
#include "tf_bot_engineer_move_to_build.h"
#include "tf_bot_engineer_build_teleport_entrance.h"


ConVar tf_raid_engineer_infinte_metal( "tf_raid_engineer_infinte_metal", "1", FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY );


CTFBotEngineerBuild::CTFBotEngineerBuild()
{
}

CTFBotEngineerBuild::~CTFBotEngineerBuild()
{
}


const char *CTFBotEngineerBuild::GetName() const
{
	return "EngineerBuild";
}


ActionResult<CTFBot> CTFBotEngineerBuild::OnStart( CTFBot *me, Action<CTFBot> *priorAction )
{
	return Action<CTFBot>::Continue();
}

ActionResult<CTFBot> CTFBotEngineerBuild::Update( CTFBot *me, float dt )
{
	return Action<CTFBot>::Continue();
}

ActionResult<CTFBot> CTFBotEngineerBuild::OnResume( CTFBot *me, Action<CTFBot> *priorAction )
{
	return Action<CTFBot>::Continue();
}


Action<CTFBot> *CTFBotEngineerBuild::InitialContainedAction( CTFBot *me )
{
	return new CTFBotEngineerMoveToBuild;
}


EventDesiredResult<CTFBot> CTFBotEngineerBuild::OnTerritoryLost( CTFBot *me, int territoryID )
{
	return Action<CTFBot>::TryContinue();
}


QueryResultType CTFBotEngineerBuild::ShouldHurry( const INextBot *me ) const
{
	CTFBot *actor = static_cast<CTFBot *>( me->GetEntity() );
	CBaseObject *pSentry = actor->GetObjectOfType( OBJ_SENTRYGUN );
	CObjectDispenser *pDispenser = (CObjectDispenser *)actor->GetObjectOfType( OBJ_DISPENSER );

	if ( pSentry && pDispenser && !pSentry->IsBuilding() && !pDispenser->IsBuilding() )
	{
		if ( actor->GetActiveTFWeapon() && actor->GetActiveTFWeapon()->GetWeaponID() == TF_WEAPON_WRENCH )
		{
			if ( actor->IsAmmoLow() )
				return (QueryResultType)( pDispenser->GetAvailableMetal() > 0 );

			return ANSWER_YES;
		}
	}

	return ANSWER_UNDEFINED;
}

QueryResultType CTFBotEngineerBuild::ShouldAttack( const INextBot *me, const CKnownEntity *threat ) const
{
	CTFBot *actor = static_cast<CTFBot *>( me->GetEntity() );
	CBaseObject *pSentry = actor->GetObjectOfType( OBJ_SENTRYGUN);

	CTFPlayer *pPlayer = ToTFPlayer( threat->GetEntity() );
	if ( !pPlayer )
	{
		if ( pSentry && me->IsRangeLessThan( pSentry, 100.0f ) )
			return ANSWER_NO;

		return ANSWER_UNDEFINED;
	}

	if ( pPlayer->IsPlayerClass( TF_CLASS_SPY ) )
		return ANSWER_YES;

	return ANSWER_UNDEFINED;
}
