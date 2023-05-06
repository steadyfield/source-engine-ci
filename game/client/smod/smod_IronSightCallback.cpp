//========= Made by The SMOD13 Team, some rights reserved. ============//
//
// Purpose: Clientside Callback for the ironsights
//
//=============================================================================//
#include "cbase.h"
#include "c_basehlplayer.h"

void CC_ToggleIronSights(void)
{
	CBasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer == NULL)
		return;

	CBaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
	if (pWeapon == NULL)
		return;

	pWeapon->ToggleIronsights();

	engine->ServerCmd("toggle_ironsight"); //forward to server
}

static ConCommand toggle_ironsight("toggle_ironsight", CC_ToggleIronSights);
