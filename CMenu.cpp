#include "CMenu.h"
#include "SDK.h"
#include "CDrawManager.h"
#include "Util.h"

CCheatMenu gCheatMenu;

int ScreenH;
int ScreenW;

char* szKeyNames[] = {
	"None", "Mouse 1", "Mouse 2", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "F",
};

char* szHealthModes[] = {
	"OFF", "Text", "Bar", "Both",
};

char* szBoneModes[] = {
	"OFF", "White", "Health", "Team",
};

char* szHitboxes[] =
{
	"Auto", "Head", "Pelvis", "Lower Torse", "Lower Mid. Torse", "Upper Mid. Torse", "Upper Torse",
	"Left Upper Arm", "Left Lower Arm", "Left Hand", "Right Upper Arm",
	"Right Lower Arm", "Right Hand", "Left Hip", "Left Knee", "Left Foot",
	"Right Hip", "Right Knee", "Right Foot",
};

int CCheatMenu::AddItem(int nIndex, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isClassSwitch)
{
	strcpy(pMenu[nIndex].szTitle, szTitle);
	pMenu[nIndex].value = value;
	pMenu[nIndex].flMin = flMin;
	pMenu[nIndex].flMax = flMax;
	pMenu[nIndex].flStep = flStep;
	pMenu[nIndex].isClassSwitch = isClassSwitch;
	return (nIndex + 1);
}

void CCheatMenu::Render(void)
{
	gInts.Engine->GetScreenSize(ScreenW, ScreenH);

	int i = 0;

	i = AddItem(i, "Aimbot", &gCvars.aimbot_switch, 0, 1, 1, true);
	if (gCvars.aimbot_switch)
	{
		i = AddItem(i, " - Enabled", &gCvars.aimbot_active, 0, 1, 1, false);
		i = AddItem(i, " - Key", &gCvars.aimbot_key, 0, 8, 1, false);
		i = AddItem(i, " - Hitscan", &gCvars.aimbot_hitscan, 0, 1, 1, false);
		i = AddItem(i, " - Hitbox", &gCvars.aimbot_hitbox, 0, 18, 1, false);
		i = AddItem(i, " - Autoshoot", &gCvars.aimbot_autoshoot, 0, 1, 1, false);
	}

	i = AddItem(i, "Triggerbot", &gCvars.triggerbot_switch, 0, 1, 1, true);
	if (gCvars.triggerbot_switch)
	{
		i = AddItem(i, " - Enabled", &gCvars.triggerbot_active, 0, 1, 1, false);
		i = AddItem(i, " - Key", &gCvars.triggerbot_key, 0, 8, 1, false);
		i = AddItem(i, " - Head Only", &gCvars.triggerbot_headonly, 0, 1, 1, false);
	}

	i = AddItem(i, "Player List", &gCvars.playerlist_switch, 0, 1, 1, true);
	for (int p = 1; p <= gInts.Engine->GetMaxClients(); p++)
	{
		if (p == me)
			continue;

		CBaseEntity *pPlayer = GetBaseEntity(p);

		if (pPlayer == NULL)
			continue;

		player_info_t pInfo;
		gInts.Engine->GetPlayerInfo(p, &pInfo);

		char szString[256];
		sprintf(szString, " - %s", pInfo.name);

		if (gCvars.playerlist_switch)
			i = AddItem(i, szString, &gCvars.PlayerMode[pPlayer->GetIndex()], 0, 2, 1, false);
	}


	i = AddItem(i, "ESP", &gCvars.esp_switch, 0, 1, 1, true);
	if (gCvars.esp_switch)
	{
		i = AddItem(i, " - Enabled", &gCvars.esp_active, 0, 1, 1, false);
		i = AddItem(i, " - Enemies Only", &gCvars.esp_enemyonly, 0, 1, 1, false);
		i = AddItem(i, " - Box", &gCvars.esp_box, 0, 1, 1, false);
		i = AddItem(i, " - Name", &gCvars.esp_name, 0, 1, 1, false);
		i = AddItem(i, " - Class", &gCvars.esp_class, 0, 1, 1, false);
		i = AddItem(i, " - Health", &gCvars.esp_health, 0, 3, 1, false);
		i = AddItem(i, " - Bones", &gCvars.esp_bones, 0, 3, 1, false);
	}

	i = AddItem(i, "Settings", &gCvars.settings_switch, 0, 1, 1, true);
	if (gCvars.settings_switch)
	{
		i = AddItem(i, " - Menu Postion X", &gCvars.iMenu_Pos_X, 0, ScreenW, 25, false);
		i = AddItem(i, " - Menu Postion Y", &gCvars.iMenu_Pos_Y, 0, ScreenH, 25, false);
	}

	i = AddItem(i, "Misc", &gCvars.misc_switch, 0, 1, 1, true);
	if (gCvars.misc_switch)
	{
		i = AddItem(i, " - Bunnyhop", &gCvars.misc_bunnyhop, 0, 1, 1, false);
		i = AddItem(i, " - Autostrafe", &gCvars.misc_autostrafe, 0, 1, 1, false);
		i = AddItem(i, " - Noisemaker Spam", &gCvars.misc_noisemaker_spam, 0, 1, 1, false);
	}

	iMenuItems = i;
}

void CCheatMenu::DrawMenu(void)
{
	int x = gCvars.iMenu_Pos_X,
		xx = x + 150,
		y = gCvars.iMenu_Pos_Y,
		w = 200,
		h = 14;

	CBaseEntity *pLocal = GetBaseEntity(me);

	Color clrColor = gDrawManager.GetPlayerColor(pLocal);

	gDrawManager.DrawRect(x, y - (h + 4), w, iMenuItems * h + 21, Color(20, 20, 20, 128));
	gDrawManager.OutlineRect(x, y - (h + 4), w, (h + 4), clrColor);

	gDrawManager.DrawRect(x + 2, y - (h + 4), w - 4, (h + 4), clrColor);
	gDrawManager.OutlineRect(x - 1, y - (h + 4) - 1, w + 2, (h + 4), Color(0, 0, 0, 255)); // test
	gDrawManager.OutlineRect(x + 1, y - (h + 4) + 1, w - 2, (h + 4), Color(0, 0, 0, 255)); // test

	gDrawManager.OutlineRect(x, y - (h + 4), w, iMenuItems * h + 21, clrColor);

	gDrawManager.OutlineRect(x - 1, (y - (h + 4)) - 1, w + 2, (iMenuItems * h + 21) + 2, Color(0, 0, 0, 255));
	gDrawManager.OutlineRect(x + 1, (y - (h + 4)) + 1, w - 2, (iMenuItems * h + 21) - 2, Color(0, 0, 0, 255));

	gDrawManager.DrawString(x + 4, y - 16, clrColor, "potassium menu");

	for (int i = 0; i < iMenuItems; i++)
	{
		if (i != iMenuIndex)
		{
			if (pMenu[i].isClassSwitch)
			{
				if (!(pMenu[i].value[0]))
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[+] %s", pMenu[i].szTitle);
				}
				else if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[-] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				gDrawManager.DrawString(x + 4, y + (h * i), Color::White(), pMenu[i].szTitle);

				if (!strcmp(pMenu[i].szTitle, " - Health"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::White() : Color(105, 105, 105, 255), "%s", szHealthModes[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Bones"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::White() : Color(105, 105, 105, 255), "%s", szBoneModes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 18)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::White(), "%s", szHitboxes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 8)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::White(), "%s", szKeyNames[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 2)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::White(), !pMenu[i].value[0] ? "Ignore" : pMenu[i].value[0] == 1 ? "Normal" : "Rage");
				}

				else if (pMenu[i].flMax == 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::White() : Color(105, 105, 105, 255), pMenu[i].value[0] ? "ON" : "OFF");
				}

				else if (pMenu[i].value[0] >= 1 && pMenu[i].flMax > 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] >= 1 ? Color::White() : Color(105, 105, 105, 255), "%0.0f", pMenu[i].value[0]);
				}
			}
		}
		else
		{
			gDrawManager.DrawRect(x + 1, y + (h * i), w - 2, h, Color(255, 255, 255, 80));

			if (pMenu[i].isClassSwitch)
			{
				if (!(pMenu[i].value[0]))
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[+] %s", pMenu[i].szTitle);
				}
				else if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[-] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				gDrawManager.DrawString(x + 4, y + (h * i), clrColor, pMenu[i].szTitle);

				if (!strcmp(pMenu[i].szTitle, " - Health"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szHealthModes[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Bones"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szBoneModes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 18)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szHitboxes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 8)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szKeyNames[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 2)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, !pMenu[i].value[0] ? "Ignore" : pMenu[i].value[0] == 1 ? "Normal" : "Rage");
				}

				else if (pMenu[i].flMax == 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, pMenu[i].value[0] ? "ON" : "OFF");
				}

				else
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%0.0f", pMenu[i].value[0]);
				}
			}
		}
	}
}