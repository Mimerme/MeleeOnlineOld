//Base code for Melee.net

//Parse arguments
#include "MeleeNET.h"
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <thread>  
#include "wx/event.h"       // for the base class
#include "wx/eventfilter.h" // (and another one)
#include "wx/build.h"
#include "wx/cmdargs.h"     // for wxCmdLineArgsArray used by wxApp::argv
#include "wx/init.h"        // we must declare wxEntry()
#include "wx/intl.h"        // for wxLayoutDirection
#include "wx/log.h"         // for wxDISABLE_DEBUG_LOGGING_IN_RELEASE_BUILD()

bool MeleeNET::m_netplay = false;
wxString MeleeNET::m_netplay_code = "";
bool MeleeNET::is_host = false;
DebugInterface* MeleeNET::debugInterface = NULL;
MeleeNETFrame* MeleeNET::meleeFrame = NULL;
int MeleeNET::gameWindowWidth = 800;
int MeleeNET::gameWindowHeight = 600;
bool MeleeNET::uiActive = false;

void MeleeNET::HookArguments(wxCmdLineArgsArray* argv) {
	
	std::cout << "test" << std::endl;
	LogToVSDebug("arg \n");
	MeleeNET::LogToVSDebug((*argv)[0].mb_str(wxConvUTF8).data());
	LogToVSDebug("\n ");
	MeleeNET::LogToVSDebug((*argv)[1].mb_str(wxConvUTF8).data());
	LogToVSDebug("\n ");
	MeleeNET::LogToVSDebug((*argv)[2].mb_str(wxConvUTF8).data());

}

wxString MeleeNET::getNetplayCode() {
	return m_netplay_code;
}

bool MeleeNET::getNetplay() {
	return m_netplay;
}

void MeleeNET::setNetplay(bool value) {
	m_netplay = value;
}

void MeleeNET::setNetplayCode(wxString value) {
	if (value.IsSameAs("host", false)) {
		is_host = true;
	}
	else {
		m_netplay_code = value;
	}
}

void MeleeNET::memoryPolling() {
	
}

void MeleeNET::createNewMemoryListener() {
	new CheckMemory();
}

void MeleeNET::LogToVSDebug(const char* str) {
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, str, -1, wString, 4096);
	OutputDebugString(wString);
}
