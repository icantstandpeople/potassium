#include "Aimbot.h"
#include "Util.h"

CAimbot gAim;

void CAimbot::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	old_movement_t old_mov = old_movement_t();

	if (gCvars.aimbot_silent) { // only backup the c_usercmd data when it's needed.
		old_mov.angle = pCommand->viewangles;
		old_mov.fwd = pCommand->forwardmove;
		old_mov.sdm = pCommand->sidemove;
	}

	gCvars.iAimbotIndex = -1;

	if (!gCvars.aimbot_active)
		return;

	if (!Util->IsKeyPressed(gCvars.aimbot_key))
		return;

	if (!pLocal->GetActiveWeapon())
		return;

	CBaseEntity* pEntity = GetBaseEntity(GetBestTarget(pLocal));

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity = pEntity->GetHitboxPosition(iBestHitbox);

	Vector vLocal = pLocal->GetEyePosition();

	Vector vAngs;
	VectorAngles((vEntity - vLocal), vAngs);

	ClampAngle(vAngs);
	gCvars.iAimbotIndex = pEntity->GetIndex();

	pCommand->viewangles = vAngs; // always set this cuz otherwise the viewangles will desync.

	if (!gCvars.aimbot_silent) {
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}

	if (gCvars.aimbot_silent) { // apply our movement fix if silent aim is enabled.
		Util->FixMovementForUserCmd(pCommand, old_mov);
	}

	if (gCvars.aimbot_autoshoot)
		pCommand->buttons |= IN_ATTACK;
}

int CAimbot::GetBestTarget(CBaseEntity* pLocal)
{
	int iBestTarget = -1;
						 //this num could be smaller 
	float flDistToBest = 99999.f;

	Vector vLocal = pLocal->GetEyePosition();

	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (pEntity->GetLifeState() != LIFE_ALIVE ||
			pEntity->GetTeamNum() == pLocal->GetTeamNum())
			continue;

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity = pEntity->GetHitboxPosition(iBestHitbox); //pEntity->GetWorldSpaceCenter(vEntity);

		if (!gCvars.PlayerMode[i])
			continue;

		if (pEntity->GetCond() & TFCond_Ubercharged ||
			pEntity->GetCond() & TFCond_UberchargeFading ||
			pEntity->GetCond() & TFCond_Bonked)
			continue;

		float flDistToTarget = (vLocal - vEntity).Length();

		if (flDistToTarget < flDistToBest)
		{
			flDistToBest = flDistToTarget;
			iBestTarget = i;
		}

		if (gCvars.PlayerMode[i] == 2) //always aim at rage targets first
			return i;
	}

	return iBestTarget;
}

int CAimbot::GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	int iBestHitbox = -1;

	if (!gCvars.aimbot_hitbox)
	{
		if (Util->IsHeadshotWeapon(pLocal, pLocal->GetActiveWeapon()))
			iBestHitbox = 0;
		else
			iBestHitbox = 4;
	}
	else
	{
		iBestHitbox = gCvars.aimbot_hitbox - 1;
	}

	if (gCvars.aimbot_hitscan)
	{
		for (int i = 0; i < 17; i++)
		{
			if (Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(i)))
				return i;
		}
	}

	if (pEntity->GetHitboxPosition(iBestHitbox).IsZero())
		return -1;

	if (!Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(iBestHitbox)))
		return - 1;
	
	return iBestHitbox;
}