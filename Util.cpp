#include "Util.h"

CUtil* Util;

void CUtil::VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut)
{
	for (auto i = 0; i < 3; i++)
		vOut[i] = vSome.Dot((Vector&)vMatrix[i]) + vMatrix[i][3];
}

bool CUtil::IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd)
{
	trace_t Trace;
	Ray_t Ray;			 // the future of variable naming
	CTraceFilter Filter;

	Filter.pSkip = pLocal;

	Ray.Init(vStart, vEnd);

	gInts.EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	return (Trace.m_pEnt == pEntity);
}

bool CUtil::IsKeyPressed(int i)
{
	switch (i)
	{
	case 0: //None
		return true;
	case 1: //Mouse 1
		return GetAsyncKeyState(VK_LBUTTON);
	case 2:
		return GetAsyncKeyState(VK_RBUTTON);
	case 3:										  //Mouses 1-5
		return GetAsyncKeyState(VK_MBUTTON);
	case 4:
		return GetAsyncKeyState(VK_XBUTTON1);
	case 5:
		return GetAsyncKeyState(VK_XBUTTON2);
	case 6: //Shift
		return GetAsyncKeyState(VK_SHIFT);
	case 7: //Alt
		return GetAsyncKeyState(VK_MENU);
	case 8: //F
		return GetAsyncKeyState(0x46);
	default:
		return false;
	}

	return false;
}

bool CUtil::IsHeadshotWeapon(CBaseEntity* pLocal, CBaseCombatWeapon* pWep)
{
	if (pWep->GetSlot() == 0 && pLocal->GetClassNum() == TF2_Sniper)
		if (pWep->GetItemDefinitionIndex() != WPN_Huntsman && pWep->GetItemDefinitionIndex() != WPN_CompoundBow)
			return true;

	if (pWep->GetItemDefinitionIndex() == WPN_Ambassador || pWep->GetItemDefinitionIndex() == WPN_FestiveAmbassador)
		return true;

	return false;
}

PVOID CUtil::InitKeyValue() //Credits f1ssion
{
	typedef PDWORD(__cdecl* Init_t)(int);
	static DWORD dwInitLocation = gSignatures.GetClientSignature("E8 ? ? ? ? 83 C4 14 85 C0 74 10 68") + 0x1;
	static DWORD dwInit = ((*(PDWORD)(dwInitLocation)) + dwInitLocation + 4);
	static Init_t InitKeyValues = (Init_t)dwInit;
	return InitKeyValues(32);
}

void CUtil::FixMovementForUserCmd(CUserCmd* cmd, old_movement_t mov) {
	float deltaView = cmd->viewangles.x - mov.angle.y;
	float f1;
	float f2;

	if (mov.angle.y < 0.f)
		f1 = 360.0f + mov.angle.y;
	else
		f1 = mov.angle.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	cmd->forwardmove = cos(DEG2RAD(deltaView)) * mov.fwd + cos(DEG2RAD(deltaView + 90.f)) * mov.sdm;
	cmd->sidemove = sin(DEG2RAD(deltaView)) * mov.fwd + sin(DEG2RAD(deltaView + 90.f)) * mov.sdm;
}