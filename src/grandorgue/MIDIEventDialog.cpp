/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "MIDIEventDialog.h"

#include "MIDIEventKeyDialog.h"
#include "MIDIEventRecvDialog.h"
#include "MIDIEventSendDialog.h"
#include <wx/bookctrl.h>

BEGIN_EVENT_TABLE(MIDIEventDialog, wxPropertySheetDialog)
	EVT_BUTTON(wxID_APPLY, MIDIEventDialog::OnApply)
	EVT_BUTTON(wxID_OK, MIDIEventDialog::OnOK)
	EVT_BUTTON(wxID_CANCEL, MIDIEventDialog::OnCancel)
END_EVENT_TABLE()

MIDIEventDialog::MIDIEventDialog (GOrgueDocumentBase* doc, wxWindow* parent, wxString title, GOrgueSettings& settings, GOrgueMidiReceiverBase* event, GOrgueMidiSender* sender, GOrgueKeyReceiver* key, GOrgueMidiSender* division):
	wxPropertySheetDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
	GOrgueView(doc, this),
	m_recvPage(NULL),
	m_sendPage(NULL),
	m_sendDivisionPage(NULL),
	m_keyPage(NULL)
{
	CreateButtons(wxOK | wxCANCEL);

	wxBookCtrlBase* notebook = GetBookCtrl();

	if (event)
	{
		m_recvPage = new MIDIEventRecvDialog(notebook, event, settings);
		notebook->AddPage(m_recvPage,  _("Receive"));
	}
	if (sender)
	{
		m_sendPage = new MIDIEventSendDialog(notebook, sender, m_recvPage, settings);
		notebook->AddPage(m_sendPage,  _("Send"));
	}
	if (key)
	{
		m_keyPage = new MIDIEventKeyDialog(notebook, key);
		notebook->AddPage(m_keyPage,  _("Shortcut"));
	}
	if (division)
	{
		m_sendDivisionPage = new MIDIEventSendDialog(notebook, division, m_recvPage, settings);
		notebook->AddPage(m_sendDivisionPage,  _("Send Division Output"));
	}

	LayoutDialog();
}

MIDIEventDialog::~MIDIEventDialog()
{
}

void MIDIEventDialog::RegisterMIDIListener(GOrgueMidi* midi)
{
	if (m_recvPage)
		m_recvPage->RegisterMIDIListener(midi);
}

void MIDIEventDialog::OnApply(wxCommandEvent& event)
{
	DoApply();
}

void MIDIEventDialog::OnOK(wxCommandEvent& event)
{
	DoApply();
	Destroy();
}

void MIDIEventDialog::OnCancel(wxCommandEvent& event)
{
	Destroy();
}

void MIDIEventDialog::DoApply()
{
	if (m_recvPage)
		m_recvPage->DoApply();
	if (m_sendPage)
		m_sendPage->DoApply();
	if (m_sendDivisionPage)
		m_sendDivisionPage->DoApply();
	if (m_keyPage)
		m_keyPage->DoApply();
}
