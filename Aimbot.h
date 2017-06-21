#pragma once
#include "SDK.h"

class CAimbot
{
public:

	void Run(CBaseEntity* pLocal, CUserCmd* pCommand);

private:

	int GetBestTarget(CBaseEntity* pLocal);

	int GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity);
};

extern CAimbot gAim; //dsajkhfdlksahf