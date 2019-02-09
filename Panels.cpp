#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "Util.h"
#include "ESP.h"

CScreenSize gScreenSize;
//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{
		VMTManager& hook = VMTManager::GetHook(pPanels); 
		hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int vguiFocusOverlayPanel;

		if (vguiFocusOverlayPanel == NULL)
		{											//FocusOverlayPanel
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'F' && szName[5] == 'O' &&  szName[12] == 'P' )
			{
				vguiFocusOverlayPanel = vguiPanel;
				Intro();
			}
		}
		
		if (vguiFocusOverlayPanel == vguiPanel )
		{
			int iWidth, iHeight; //Resolution fix, so this can work in Fullscreen
			gInts.Engine->GetScreenSize(iWidth, iHeight);
			if (gScreenSize.iScreenWidth != iWidth || gScreenSize.iScreenHeight != iHeight)
				gInts.Engine->GetScreenSize(gScreenSize.iScreenWidth, gScreenSize.iScreenHeight);

			if( gInts.Engine->IsDrawingLoadingImage() || !gInts.Engine->IsInGame( ) || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible( ) )
			{
				gDrawManager.DrawString(5, 5, Color::White(), "Polly.xyz");
				return; //No need to draw the rest.
			}

			CBaseEntity* pLocal = gInts.EntList->GetClientEntity(me); 

			if (!pLocal)
				return;

			gESP.Run(pLocal);

			if (gCheatMenu.bMenuActive)
			{
				gCheatMenu.DrawMenu();
				gCheatMenu.Render();
			}
		}
	}
	catch(...)
	{
		Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.

		gNetVars.Initialize();
	}
	catch(...)
	{
		Log::Fatal("Failed Intro");
	}
}