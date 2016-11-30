#pragma once
#include "wx/event.h"       // for the base class
#include "wx/eventfilter.h" // (and another one)
#include "wx/build.h"
#include "wx/cmdargs.h"     // for wxCmdLineArgsArray used by wxApp::argv
#include "wx/init.h"        // we must declare wxEntry()
#include "wx/intl.h"        // for wxLayoutDirection
#include "wx/log.h"  
#include "DolphinWX/Frame.h"

class MeleeNET {
	public:
		static void HookArguments(wxCmdLineArgsArray* argv);
		static void LogToVSDebug(const char *string);

		static void setNetplay(bool value);
		static void setNetplayCode(wxString value);
		static bool getNetplay();
		static wxString getNetplayCode();
		static void setCallback(void (CFrame::*OnNetPlay)(wxCommandEvent&));

		//Check if Dolphin boots directly into netplay
		static bool m_netplay;
		static wxString m_netplay_code;
		static bool is_host;
};