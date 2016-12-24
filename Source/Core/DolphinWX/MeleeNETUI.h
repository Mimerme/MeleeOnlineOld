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
	static std::string MeleeNETUI::menuOptions[5];
	const static int MAX_OPTIONS = 5;
	static void MeleeNETUI::drawMenu();
	static int MeleeNETUI::activeOption;
	static int MeleeNETUI::activeColor;

	//Lifecycle
	//->Init Hook
		//-> Add objects to the options
	//-> Begin render cycle
	//->On Input waits for hotkeys

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

		if (!MeleeNETUI::uiActive)
			return;

		switch (hotKEY) {
		case HK_MELEE_DOWN:
			if ((MeleeNETUI::activeOption + 1) < MAX_OPTIONS)
				MeleeNETUI::activeOption++;
			break;
		case HK_MELEE_UP:
			if((MeleeNETUI::activeOption - 1) > 0)
				MeleeNETUI::activeOption--;
			break;
		case HK_MELEE_SELECT:
			break;
		default:
			break;
		}

	}

	static void MeleeNETUI::initalizeUIHook() {
		menuOptions[0] = ("Melee.NET Hook!");
		menuOptions[1] = ("By Mimerme :D");
		menuOptions[2] = ("Test Element 1");
		menuOptions[3] = ("Test Element 2");
		menuOptions[4] = ("Look for netplay game");

	}

	static void MeleeNETUI::renderUI(std::unique_ptr<Renderer> &rend) {
		for (int i = 0; i < MAX_OPTIONS; i++) {
			std::string append = "";
			if (i == MeleeNETUI::activeOption)
				append = ">";
			g_renderer->RenderText(append + menuOptions[i], CalculateAlignRight(menuOptions[i], gameWindowWidth), i * 20, MeleeNETUI::activeColor);
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