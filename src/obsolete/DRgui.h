///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version v3.8.0)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/checklst.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class m_Dialog
///////////////////////////////////////////////////////////////////////////////
class m_Dialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText1511;
		wxStaticLine* m_staticline111;
		wxStaticText* m_staticText32111112;
		wxCheckListBox* m_checkList1;
		wxButton* m_button2;
		wxStaticLine* m_staticline11;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_textCtrl4;
		wxStaticText* m_staticText9;
		wxTextCtrl* m_textCtrl3;
		wxButton* m_button3111;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnDownloadCharts( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDownloadLayers( wxCommandEvent& event ) { event.Skip(); }


	public:

		m_Dialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("DR"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~m_Dialog();

};

