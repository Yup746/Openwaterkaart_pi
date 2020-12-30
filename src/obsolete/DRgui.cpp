///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version v3.8.0)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "DRgui.h"

///////////////////////////////////////////////////////////////////////////

m_Dialog::m_Dialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer9111;
	bSizer9111 = new wxBoxSizer( wxVERTICAL );

	m_staticText1511 = new wxStaticText( this, wxID_ANY, wxT("Openwaterkaart"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1511->Wrap( -1 );
	m_staticText1511->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer9111->Add( m_staticText1511, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticline111 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9111->Add( m_staticline111, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer1311;
	bSizer1311 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer14111112;
	bSizer14111112 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText32111112 = new wxStaticText( this, wxID_ANY, wxT("Welke kaarten moeten         \ngedownload/bijgewerkt \nworden?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32111112->Wrap( -1 );
	bSizer14111112->Add( m_staticText32111112, 1, wxALL|wxEXPAND, 5 );

	wxString m_checkList1Choices[] = { wxT("Nederland"), wxT("België"), wxT("Duitsland"), wxT("Denemarken") };
	int m_checkList1NChoices = sizeof( m_checkList1Choices ) / sizeof( wxString );
	m_checkList1 = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkList1NChoices, m_checkList1Choices, 0 );
	bSizer14111112->Add( m_checkList1, 1, wxALL|wxEXPAND, 5 );


	bSizer1311->Add( bSizer14111112, 1, wxEXPAND, 5 );

	m_button2 = new wxButton( this, wxID_ANY, wxT("Download kaarten"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_button2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticline11 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1311->Add( m_staticline11, 0, wxEXPAND | wxALL, 5 );


	bSizer9111->Add( bSizer1311, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );

	m_staticText71 = new wxStaticText( this, wxID_ANY, wxT("Als je een licentie voor Openwaterkaart hebt gekocht \nkun je hier de inloggegevens invullen om de lagen met\nnuttige informatie ook te downloaden"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	bSizer71->Add( m_staticText71, 0, wxALL, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Emailadres:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer9->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrl4 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_textCtrl4, 1, wxALL|wxEXPAND, 5 );


	bSizer71->Add( bSizer9, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Licentiecode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer8->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrl3 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_textCtrl3, 1, wxALL|wxEXPAND, 5 );


	bSizer71->Add( bSizer8, 1, wxEXPAND, 5 );

	m_button3111 = new wxButton( this, wxID_ANY, wxT("Download lagen"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_button3111, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


	bSizer9111->Add( bSizer71, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer9111 );
	this->Layout();
	bSizer9111->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( m_Dialog::OnClose ) );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnDownloadCharts ), NULL, this );
	m_button3111->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnDownloadLayers ), NULL, this );
}

m_Dialog::~m_Dialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( m_Dialog::OnClose ) );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnDownloadCharts ), NULL, this );
	m_button3111->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnDownloadLayers ), NULL, this );

}
