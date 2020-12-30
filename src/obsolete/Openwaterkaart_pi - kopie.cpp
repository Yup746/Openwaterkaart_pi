/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Openwaterkaart Plugin
 * Author:   Pim Spithost
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Pim Spithost                                *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "Openwaterkaart_pi.h"
#include "Openwaterkaartgui_impl.h"
#include "Openwaterkaartgui.h"

#include "version.h"
#include "wxWTranslateCatalog.h"


// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new Openwaterkaart_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    Openwaterkaart PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

Openwaterkaart_pi::Openwaterkaart_pi(void *ppimgr)
      :opencpn_plugin_116 (ppimgr)
{
      // Create the PlugIn icons
      initialize_images();

	  wxFileName fn;
	  wxString tmp_path;

	  tmp_path = GetPluginDataDir("Openwaterkaart_pi");
	  fn.SetPath(tmp_path);
	  fn.AppendDir(_T("data"));
	  fn.SetFullName("Openwaterkaart_pi_panel_icon.png");

	  wxString shareLocn = fn.GetFullPath();
	  wxImage panelIcon(shareLocn);

	  if (panelIcon.IsOk())
		  m_panelBitmap = wxBitmap(panelIcon);
	  else
		  wxLogMessage(_T("    Openwaterkaart_pi panel icon NOT loaded"));
	  m_bShowOpenwaterkaart = false;
}

Openwaterkaart_pi::~Openwaterkaart_pi(void)
{
     delete _img_Openwaterkaart_pi;
     delete _img_Openwaterkaart;

}

int Openwaterkaart_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-Openwaterkaart_pi") );

      // Set some default private member parameters
      m_route_dialog_x = 0;
      m_route_dialog_y = 0;
      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      //    This PlugIn needs a toolbar icon, so request its insertion
	if(m_bOpenwaterkaartShowIcon)

    m_leftclick_tool_id = InsertPlugInTool(_T(""), _img_Openwaterkaart, _img_Openwaterkaart, wxITEM_CHECK,
        _("Openwaterkaart"), _T(""), NULL,
        CALCULATOR_TOOL_POSITION, 0, this);


      m_pDialog = NULL;

      return (WANTS_OVERLAY_CALLBACK |
              WANTS_OPENGL_OVERLAY_CALLBACK |

		      WANTS_CURSOR_LATLON      |
              WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |
              WANTS_CONFIG             |
			  WANTS_PLUGIN_MESSAGING

           );
}

bool Openwaterkaart_pi::DeInit(void)
{
      //    Record the dialog position
      if (NULL != m_pDialog)
      {
            //Capture dialog position
            wxPoint p = m_pDialog->GetPosition();
            SetCalculatorDialogX(p.x);
            SetCalculatorDialogY(p.y);
            m_pDialog->Close();
            delete m_pDialog;
            m_pDialog = NULL;

			m_bShowOpenwaterkaart = false;
			SetToolbarItemState( m_leftclick_tool_id, m_bShowOpenwaterkaart );

      }

    SaveConfig();

    RequestRefresh(m_parent_window); // refresh mainn window

    return true;
}

int Openwaterkaart_pi::GetAPIVersionMajor()
{
      return OCPN_API_VERSION_MAJOR;
}

int Openwaterkaart_pi::GetAPIVersionMinor()
{
      return OCPN_API_VERSION_MINOR;
}

int Openwaterkaart_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int Openwaterkaart_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *Openwaterkaart_pi::GetPlugInBitmap()
{
      return &m_panelBitmap;
}

wxString Openwaterkaart_pi::GetCommonName()
{
      return _("Openwaterkaart");
}


wxString Openwaterkaart_pi::GetShortDescription()
{
      return _("Kaarten en lagen downloaden van Openwaterkaart");
}

wxString Openwaterkaart_pi::GetLongDescription()
{
      return _("Met deze plugin kunnen de kaarten en lagen met nuttige informatie van Openwaterkaart geïmplementeerd worden");
}

int Openwaterkaart_pi::GetToolbarToolCount(void)
{
      return 1;
}

void Openwaterkaart_pi::SetColorScheme(PI_ColorScheme cs)
{
      if (NULL == m_pDialog)
            return;

      DimeWindow(m_pDialog);
}

void Openwaterkaart_pi::OnToolbarToolCallback(int id)
{

	if(NULL == m_pDialog)
      {
            m_pDialog = new Dlg(m_parent_window, this);
            m_pDialog->Move(wxPoint(m_route_dialog_x, m_route_dialog_y));
      }

	  m_pDialog->Fit();
	  //Toggle
	  m_bShowOpenwaterkaart = !m_bShowOpenwaterkaart;

      //    Toggle dialog?
      if(m_bShowOpenwaterkaart) {
          m_pDialog->Show();
      } else
          m_pDialog->Hide();

      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowOpenwaterkaart );

      RequestRefresh(m_parent_window); // refresh main window
}

bool Openwaterkaart_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/Openwaterkaart_pi" ) );
			 pConf->Read ( _T( "ShowOpenwaterkaartIcon" ), &m_bOpenwaterkaartShowIcon, 1 );

            m_route_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_route_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

            if((m_route_dialog_x < 0) || (m_route_dialog_x > m_display_width))
                  m_route_dialog_x = 5;
            if((m_route_dialog_y < 0) || (m_route_dialog_y > m_display_height))
                  m_route_dialog_y = 5;
            return true;
      }
      else
            return false;
}

bool Openwaterkaart_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/Openwaterkaart_pi" ) );
			pConf->Write ( _T ( "ShowOpenwaterkaartIcon" ), m_bOpenwaterkaartShowIcon );

            pConf->Write ( _T ( "DialogPosX" ),   m_route_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_route_dialog_y );

            return true;
      }
      else
            return false;
}

void Openwaterkaart_pi::OnOpenwaterkaartDialogClose()
{
    m_bShowOpenwaterkaart = false;
    SetToolbarItemState( m_leftclick_tool_id, m_bShowOpenwaterkaart );
    m_pDialog->Hide();
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

}


