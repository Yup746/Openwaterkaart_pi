/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Openwaterkaart.nl Plugin
 * Author:   Pim Spithost
 *
 ***************************************************************************
 *   Copyright (C) 2020 by Pim  Spithost                                   *
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

#ifndef _CALCULATORGUI_IMPL_H_
#define _CALCULATORGUI_IMPL_H_

#ifdef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Openwaterkaartgui.h"
#include "Openwaterkaart_pi.h"
#include "ocpn_plugin.h"

#include <list>
#include <vector>
#include <string>

using namespace std;

class Openwaterkaart_pi;
class OpenwaterkaartConfig;

class Dlg : public m_Dialog
{
public:
	Dlg(wxWindow *parent, Openwaterkaart_pi *ppi);

	wxWindow *pParent;
	Openwaterkaart_pi *pPlugIn;

        void DownloadNoDialog(wxString url, wxString path, wxEvtHandler* handler, long* handle);
		void onDLEvent(OCPN_downloadEvent &ev);
        void OnDownloadCharts( wxCommandEvent& event );
		void OnDownloadLayers( wxCommandEvent& event );

		bool downloadfinished = false;

		wxString m_base_chart_dir;
		wxFileName fn;
		wxFileName configPath;
		wxFileName chartPath;
		vector<string> ccp; //configcountryprefix
		OpenwaterkaartConfig * config;
		vector<string> xmlurls;
		vector<string> landstrings;
		wxFileName XMLPath;
		wxFileName workdir;
		wxFileName icondir;
		wxFileName layerdir;

private:
	    void OnClose( wxCloseEvent& event );
};

class OpenwaterkaartConfig{
	public:
		string configLocation;
		string GetLicense();
		string GetMailadress();
		string GetLastUpdated(string ccp);

		void SetConfigPath(wxString configloc);

		void SetLicense(wxString newlicense);
		void SetMailadress(wxString newmailadress);
		void SetLastUpdated(string ccp, string newdate);

	private:
		vector<string> configvector;
		void WriteToFile();
};

#endif
