#include "AntiAntiAim.h"

CAntiAntiAim gAntiAntiAim;

void CAntiAntiAim::PostDataUpdateStart() {
	if (!gCvars.misc_anti_anti_aim) {
		return;
	}

	auto local = GetBaseEntity(me);

	if (!local) {
		return;
	}

	for (int i = 0; i <= gInts.Engine->GetMaxClients(); i++) {
		auto entity = GetBaseEntity(i);

		if (!entity ||
			entity->IsDormant() ||
			entity->GetLifeState() != LIFE_ALIVE ||
			i == me ||
			entity->GetTeamNum() == local->GetTeamNum()) {
			continue; // invalid entity.
		}

		Vector vEyeAngles = entity->GetEyeAngles();

		if (vEyeAngles.x >= +90.0f) {
			vEyeAngles.x = -90.0f; // fake down, set pitch angle to up.
		}
		else if (vEyeAngles.x <= -90.0f) {
			vEyeAngles.x = +90.0f; // fake up, set pitch angle to down.
		}
		else {
			if (gCvars.misc_anti_anti_aim_unsafe_x) {
				vEyeAngles.x = 0; // set it to zero if we're in an hvh server for example. i know it's a bad practice but i don't care lmao.
			}
		}

		entity->SetEyeAngles(vEyeAngles);
	}
}