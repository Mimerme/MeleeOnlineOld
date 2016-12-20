// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <wx/bitmap.h>
#include <wx/dialog.h>
#include <wx/generic/statbmpg.h>
#include <wx/hyperlink.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "Common/Common.h"
#include "MeleeNETFrame.h"
#include "DolphinWX/WxUtils.h"

wxStaticText* MeleeNETFrame::text = NULL;
wxStaticText* MeleeNETFrame::doot = NULL;

MeleeNETFrame::MeleeNETFrame(wxWindow* parent, wxWindowID id, const wxString& title,
	const wxPoint& position, const wxSize& size, long style)
	: wxDialog(parent, id, title, position, size, style)
{

	text = new wxStaticText(this, wxID_ANY, "[Game Not Started...]");
	doot = new wxStaticText(this, wxID_ANY, "doot");

	wxSizerFlags center_flag;
	center_flag.Center();
	const int space5 = FromDIP(5);
	const int space10 = FromDIP(20);
	const int space15 = FromDIP(25);
	const int space30 = FromDIP(30);
	const int space40 = FromDIP(40);
	const int space75 = FromDIP(75);

	wxBoxSizer* const memoryDisplay = new wxBoxSizer(wxHORIZONTAL);
	memoryDisplay->Add(text, center_flag);
	memoryDisplay->Add(doot, center_flag);


	SetSizerAndFit(memoryDisplay);

	Center();
	SetFocus();

}
