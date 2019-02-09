#pragma once
#include "SDK.h"

class CUtil
{
public:
	void VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut);

	bool IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd);

	bool IsKeyPressed(int i);

	bool IsHeadshotWeapon(CBaseEntity* pLocal, CBaseCombatWeapon* pWep);

	void FixMovementForUserCmd(CUserCmd* cmd, old_movement_t mov);

	PVOID InitKeyValue();
};

extern CUtil* Util;