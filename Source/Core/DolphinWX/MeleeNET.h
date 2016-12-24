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
class MeleeNET {
	public:

		static void HookArguments(wxCmdLineArgsArray* argv);
		static void LogToVSDebug(const char *string);
		static DebugInterface* debugInterface;
		static MeleeNETFrame* meleeFrame;

		static void setNetplay(bool value);
		static void setNetplayCode(wxString value);
		static bool getNetplay();
		static wxString getNetplayCode();
		static void setCallback(void (CFrame::*OnNetPlay)(wxCommandEvent&));

		static void setDebug(DebugInterface *debugFace) {
			debugInterface = debugFace;
		};
		static void setFrame(MeleeNETFrame *frame) {
			MeleeNET::meleeFrame = frame;
		};

		static void createNewMemoryListener();
		static void memoryPolling();

		//Why the fuck do these addresses change with every version of Dolphin
		//static const int player1DamageAddress = 0x004a10d0;
		//static const int player2DamageAddress = 0x004a10d0;
		static const int player1Stocks = 0x804a13cc;
		static const int player2Stocks = 0x804a141c;

		//Check if Dolphin boots directly into netplay
		static bool m_netplay;
		static wxString m_netplay_code;
		static bool is_host;
};

class CheckMemory
	: public wxThread
{
public:
	CheckMemory() : wxThread(wxTHREAD_DETACHED)
	{
		if (wxTHREAD_NO_ERROR == Create()) {
			Run();
		}
	}
protected:
	virtual ExitCode Entry()
	{
		wxString meme;
		//Infinite loop checking for memory change
		while (true) {
			//meme = wxString::Format(wxT("%i"), PowerPC::Read_U32(0x804a13cc));
			//meme = PowerPC::Read_U16(MeleeNET::player1Rank);
			MeleeNETFrame::text->SetLabelText(meme);
		}
		return static_cast<ExitCode>(NULL);
	}
};