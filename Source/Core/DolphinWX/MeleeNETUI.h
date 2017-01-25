#pragma once
#include "wx/event.h"       // for the base class
#include "wx/eventfilter.h" // (and another one)
#include "wx/build.h"
#include "wx/cmdargs.h"     // for wxCmdLineArgsArray used by wxApp::argv
#include "wx/init.h"        // we must declare wxEntry()
#include "wx/intl.h"        // for wxLayoutDirection
#include "wx/log.h"  
#include "DolphinWX/Frame.h"
#include "Core/Debugger/Debugger_SymbolMap.h"
#include "Core/Debugger/PPCDebugInterface.h"
#include "Core/HW/CPU.h"
#include "Core/HW/Memmap.h"
#include "Core/HW/SystemTimers.h"
#include "Core/Host.h"
#include "Core/PowerPC/Gekko.h"
#include "Core/PowerPC/JitInterface.h"
#include "Core/PowerPC/PPCSymbolDB.h"
#include "Core/PowerPC/PPCTables.h"
#include "Core/PowerPC/PowerPC.h"
#include "DolphinWX/Debugger/BreakpointWindow.h"
#include "DolphinWX/Debugger/CodeView.h"
#include "DolphinWX/Debugger/CodeWindow.h"
#include "DolphinWX/Debugger/DebuggerUIUtil.h"
#include "DolphinWX/Debugger/JitWindow.h"
#include "DolphinWX/Debugger/MemoryWindow.h"
#include "DolphinWX/Debugger/RegisterWindow.h"
#include "DolphinWX/Debugger/WatchWindow.h"
#include <wx/msgdlg.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wxprec.h>
#include "MeleeNETFrame.h"
#include "VideoCommon/RenderBase.h"
#include "VideoCommon/VideoBackendBase.h"
#include "VideoCommon/VideoConfig.h"
#include "MeleeNET.h"
#include "Core/HotKeyManager.h"

class MeleeNETUI {
public:
	//UI Variables
	static int MeleeNETUI::gameWindowWidth, MeleeNETUI::gameWindowHeight;
	static bool MeleeNETUI::uiActive;

	//0 - menu
	//1 - match
	//2 - matchconfig
	//3 - options
	//4 - about
	static int MeleeNETUI::SCENE_STATE;

	static std::string MeleeNETUI::menuOptions[4];
	static std::string MeleeNETUI::matchOptions[3];
	static std::string MeleeNETUI::about[3];
	static std::string MeleeNETUI::matchConfig[4];
	static std::string MeleeNETUI::options[3];
	static std::string MeleeNETUI::lookingForMatch[3];

	static int MeleeNETUI::MAX_OPTIONS;
	static void MeleeNETUI::drawMenu();
	static int MeleeNETUI::activeOption;
	static int MeleeNETUI::activeColor;

	//Lifecycle
	//->Init Hook
		//-> Add objects to the options
	//-> Begin render cycle
	//->On Input waits for hotkeys

	static void MeleeNETUI::back() {
		//Exit 'Find Match'
		if (SCENE_STATE == 1) {
			MAX_OPTIONS = 4;
			SCENE_STATE = 0;
		}
		else if (SCENE_STATE == 2) {
			MAX_OPTIONS = 3;
			SCENE_STATE = 1;
		}
		else if (SCENE_STATE == 3) {
			MAX_OPTIONS = 4;
			SCENE_STATE = 0;
		}
		else if (SCENE_STATE == 4) {
			//Go to match config, remember friendlies
			MAX_OPTIONS = 4;
			SCENE_STATE = 0;
		}
		else if (SCENE_STATE == 5) {
			MAX_OPTIONS = 4;
			SCENE_STATE = 2;
		}
	}

	static void MeleeNETUI::select() {
		//Enter 'Find Match'
		if (activeOption == 1 && SCENE_STATE == 0) {
			MAX_OPTIONS = 3;
			SCENE_STATE = 1;
		}
		else if (activeOption == 2 && SCENE_STATE == 0) {
			MAX_OPTIONS = 3;
			SCENE_STATE = 4;
		}
		else if (activeOption == 3 && SCENE_STATE == 0) {
			MAX_OPTIONS = 3;
			SCENE_STATE = 3;
		}
		else if (activeOption == 1 && SCENE_STATE == 1) {
			//Go to match config, remember friendlies
			MAX_OPTIONS = 4;
			SCENE_STATE = 2;
		}
		//Not implemeneted
		else if (activeOption == 2 && SCENE_STATE == 1) {
			//Go to match config, remember for glory
			//MAX_OPTIONS = 3;
			//SCENE_STATE = 2;
		}
		else if (activeOption == 1 && SCENE_STATE == 3) {
			//Performance Test
		}
		else if (activeOption == 2 && SCENE_STATE == 3) {
			//Configure Dolphin
		}

		//Lookking for match
		else if (SCENE_STATE == 2) {
			//Go to match config, remember for glory
			MAX_OPTIONS = 3;
			SCENE_STATE = 5;
		}
	}

	static void MeleeNETUI::onInput(int hotKEY) {
		if (hotKEY == HK_MELEE_TEST) {
			MeleeNETUI::uiActive = !MeleeNETUI::uiActive;
			if (MeleeNETUI::uiActive) {
				MeleeNETUI::activeColor = 0xffffffff;
			}
			else {
				MeleeNETUI::activeColor = 0xffffff66;
			}
		}


		if (MeleeNETUI::uiActive == TRUE) {
			switch (hotKEY) {
			case HK_MELEE_DOWN:
				if ((MeleeNETUI::activeOption + 1) < MAX_OPTIONS)
					MeleeNETUI::activeOption++;
				break;
			case HK_MELEE_UP:
				if ((MeleeNETUI::activeOption - 1) > 0)
					MeleeNETUI::activeOption--;
				break;
			case HK_MELEE_SELECT:
				MeleeNETUI::select();
				break;
			case HK_MELEE_BACK:
				MeleeNETUI::back();
				break;
			default:
				break;
			}
		}

	}

	static void MeleeNETUI::initalizeUIHook() {
		menuOptions[0] = ("--Melee.NET---");
		menuOptions[1] = ("Find Match");
		menuOptions[2] = ("About");
		menuOptions[3] = ("Options");

		matchOptions[0] = ("--Choose Match Type--");
		matchOptions[1] = ("Friendlies");
		matchOptions[2] = ("For Glory (TBD)");

		about[0] = ("--About--");
		about[1] = ("Emulator written by the Dolphin Emulator Project");
		about[2] = ("Matchmaking written by Mimerme and sprd");

		matchConfig[0] = ("--Choose Match Type--");
		matchConfig[1] = ("Infinite");
		matchConfig[2] = ("Best of 3");
		matchConfig[3] = ("Best of 5");

		options[0] = ("--Options--");
		options[1] = ("Performance Test");
		options[2] = ("Configure Dolphin");

		lookingForMatch[0] = "--Finding [GAMEMODE] [GAMELENGTH]--";
		lookingForMatch[1] = "[PLACEHOLDER STATUS]";
		lookingForMatch[2] = "Press B to Cancel";

	}

	static void MeleeNETUI::renderUI(std::unique_ptr<Renderer> &rend) {
		for (int i = 0; i < MAX_OPTIONS; i++) {
			std::string append = "";
			if (i == MeleeNETUI::activeOption)
				append = "<";
			switch (SCENE_STATE) {
			case 0:
				g_renderer->RenderText(menuOptions[i] + append, 20, (i + 1) * 20, MeleeNETUI::activeColor);
				break;
			case 1:
				g_renderer->RenderText(matchOptions[i] + append, 20, (i + 1) * 20, MeleeNETUI::activeColor);
				break;
			case 2:
				g_renderer->RenderText(matchConfig[i] + append, 20, (i + 1) * 20, MeleeNETUI::activeColor);
				break;
			case 3:
				g_renderer->RenderText(options[i] + append, 20, (i + 1) * 20, MeleeNETUI::activeColor);
				break;
			case 4:
				g_renderer->RenderText(about[i] + append, 20, (i + 1) * 20, MeleeNETUI::activeColor);
				break;
			case 5:
				g_renderer->RenderText(lookingForMatch[i] + append, 20, (i + 1) * 20, MeleeNETUI::activeColor);
				break;
			}
		}
	}

	static int MeleeNETUI::CalculateAlignRight(std::string input, int windowWidth) {
		const int characterSize = 10;
		return windowWidth - (input.length() * characterSize);
	}

	//Why the fuck do these addresses change with every version of Dolphin
	//static const int player1DamageAddress = 0x004a10d0;
	//static const int player2DamageAddress = 0x004a10d0;
	static const int MeleeNETUI::player1Stocks = 0x804a13cc;
	static const int MeleeNETUI::player2Stocks = 0x804a141c;
};