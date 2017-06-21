#pragma once
#include "SDK.h"

class CESP
{
public:

	void Run(CBaseEntity* pLocal);

private:

	void Player_ESP(CBaseEntity* pLocal, CBaseEntity* pEntity);

	//Other funcs

	void DrawBone(CBaseEntity* pEntity, int* iBones, int count, Color clrCol);
};

extern CESP gESP; 