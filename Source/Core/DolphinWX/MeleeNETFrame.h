// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <wx/dialog.h>

class MeleeNETFrame : public wxDialog
{
public:
	MeleeNETFrame(wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxString& title = _("About Dolphin"), const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	static wxStaticText* text;
	static wxStaticText* doot;

};
