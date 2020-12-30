///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version v3.8.0)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Openwaterkaartgui.h"

///////////////////////////////////////////////////////////////////////////

m_Dialog::m_Dialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer9111;
	bSizer9111 = new wxBoxSizer( wxVERTICAL );

	m_staticText1511 = new wxStaticText( this, wxID_ANY, wxT("Openwaterkaart.nl"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1511->Wrap( -1 );
	m_staticText1511->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer9111->Add( m_staticText1511, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticline111 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9111->Add( m_staticline111, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer1311;
	bSizer1311 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer14111112;
	bSizer14111112 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText32111112 = new wxStaticText( this, wxID_ANY, wxT("Welke kaarten wil je         \ndownloaden/bijwerken?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32111112->Wrap( -1 );
	bSizer14111112->Add( m_staticText32111112, 1, wxALL|wxEXPAND, 5 );

	wxString m_chartchecklistChoices[] = { wxT("Nederland"), wxT("België"), wxT("Duitsland"), wxT("Denemarken") };
	int m_chartchecklistNChoices = sizeof( m_chartchecklistChoices ) / sizeof( wxString );
	m_chartchecklist = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chartchecklistNChoices, m_chartchecklistChoices, 0 );
	bSizer14111112->Add( m_chartchecklist, 1, wxALL|wxEXPAND, 5 );


	bSizer1311->Add( bSizer14111112, 1, wxEXPAND, 5 );

	m_chartdownloadbutton = new wxButton( this, wxID_ANY, wxT("Download kaarten"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1311->Add( m_chartdownloadbutton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticline112 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1311->Add( m_staticline112, 0, wxEXPAND | wxALL, 5 );


	bSizer9111->Add( bSizer1311, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );

	m_staticText71 = new wxStaticText( this, wxID_ANY, wxT("Vul hieronder je inloggegevens in om de lagen toe\nte voegen. Let op, de licentiecode moet je eerst\naangeschaft hebben op www.openwaterkaart.nl."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	bSizer71->Add( m_staticText71, 0, wxALL, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("E-mailadres:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer9->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_mailadress = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer9->Add( m_mailadress, 1, wxALL|wxEXPAND, 5 );


	bSizer71->Add( bSizer9, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Licentiecode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer8->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_licensekey = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer8->Add( m_licensekey, 1, wxALL|wxEXPAND, 5 );


	bSizer71->Add( bSizer8, 1, wxEXPAND, 5 );

	m_layerdownloadbutton = new wxButton( this, wxID_ANY, wxT("Download lagen"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_layerdownloadbutton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


	bSizer9111->Add( bSizer71, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer9111 );
	this->Layout();
	bSizer9111->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( m_Dialog::OnClose ) );
	m_chartdownloadbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnDownloadCharts ), NULL, this );
	m_mailadress->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( m_Dialog::OnDownloadLayers ), NULL, this );
	m_licensekey->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( m_Dialog::OnDownloadLayers ), NULL, this );
	m_layerdownloadbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnDownloadLayers ), NULL, this );
}

m_Dialog::~m_Dialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( m_Dialog::OnClose ) );
	m_chartdownloadbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnDownloadCharts ), NULL, this );
	m_mailadress->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( m_Dialog::OnDownloadLayers ), NULL, this );
	m_licensekey->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( m_Dialog::OnDownloadLayers ), NULL, this );
	m_layerdownloadbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_Dialog::OnDownloadLayers ), NULL, this );

}
