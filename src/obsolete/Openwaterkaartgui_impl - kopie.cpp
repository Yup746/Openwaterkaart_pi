/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Openwaterkaart Plugin
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

#include "Openwaterkaartgui_impl.h"
#include "Openwaterkaartgui.h"
#include "Openwaterkaart_pi.h"
#include "icons.h"

#include <wx/progdlg.h>
#include <wx/wx.h>
#include "wx/dir.h"
#include <list>
#include <cmath>
#include <wx/listctrl.h>
#include "ocpn_plugin.h"
#include <fstream>
#include "tinyxml2.h"
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <wx/filename.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>

#define FAIL(X) do { error = X; goto failed; } while(0)
#define CHART_DIR "Charts"

Dlg::Dlg(wxWindow *parent, Openwaterkaart_pi *ppi)
	: m_Dialog(parent)
{
	this->Fit();
	dbg = false; //for debug output set to true

	pPlugIn = ppi;
	pParent = parent;

	wxString blank_name = *GetpSharedDataLocation()
		+ _T("plugins/Openwaterkaart_pi/data/blank.ico");

	wxIcon icon(blank_name, wxBITMAP_TYPE_ICO);
	SetIcon(icon);
}

Dlg::~Dlg()
{

}

/*void Dlg::Addpoint(TiXmlElement* Route, wxString ptlat, wxString ptlon, wxString ptname, wxString ptsym, wxString pttype){
//add point
	TiXmlElement * RoutePoint = new TiXmlElement( "rtept" );
    RoutePoint->SetAttribute("lat", ptlat.mb_str());
    RoutePoint->SetAttribute("lon", ptlon.mb_str());


    TiXmlElement * Name = new TiXmlElement( "name" );
    TiXmlText * text = new TiXmlText( ptname.mb_str() );
    RoutePoint->LinkEndChild( Name );
    Name->LinkEndChild( text );

    TiXmlElement * Symbol = new TiXmlElement( "sym" );
    TiXmlText * text1 = new TiXmlText( ptsym.mb_str() );
    RoutePoint->LinkEndChild( Symbol );
    Symbol->LinkEndChild( text1 );

    TiXmlElement * Type = new TiXmlElement( "type" );
    TiXmlText * text2 = new TiXmlText( pttype.mb_str() );
    RoutePoint->LinkEndChild( Type );
    Type->LinkEndChild( text2 );
    Route->LinkEndChild( RoutePoint );
//done adding point
}

void Dlg::OnPSGPX( wxCommandEvent& event )
{
   //Calculate(event, true, 1);
}*/

void Dlg::OnClose(wxCloseEvent& event)
{
	pPlugIn->OnOpenwaterkaartDialogClose();
}

bool Dlg::OpenXML()
{
    Position my_position;

    my_positions.clear();

	int response = wxID_CANCEL;
	int my_count = 0;

	wxArrayString file_array;
    wxString filename;
	wxFileDialog openDialog( this, _( "Import GPX Route file" ), m_gpx_path, wxT ( "" ),
                wxT ( "GPX files (*.gpx)|*.gpx|All files (*.*)|*.*" ),
                wxFD_OPEN | wxFD_MULTIPLE );
        response = openDialog.ShowModal();
        if( response == wxID_OK ) {
            openDialog.GetPaths( file_array );

            //    Record the currently selected directory for later use
            if( file_array.GetCount() ) {
                wxFileName fn( file_array[0] );
				filename = file_array[0];
                m_gpx_path = fn.GetPath();
            }
        }
		else if(response == wxID_CANCEL){
		return false;
		}

    TiXmlDocument doc;
    wxString error;
    wxProgressDialog *progressdialog = NULL;


	if(!doc.LoadFile(filename.mb_str())){
        FAIL(_("Failed to load file: ") + filename);
	}
    else {
        TiXmlElement *root = doc.RootElement();
        if(!strcmp(root->Value(), "rte"))
            FAIL(_("rte Invalid xml file"));

        int count = 0;
        for(TiXmlElement* e = root->FirstChildElement(); e; e = e->NextSiblingElement())
            count++;

        int i=0;
        for(TiXmlElement* e = root->FirstChildElement(); e; e = e->NextSiblingElement(), i++) {
            if(progressdialog) {
                if(!progressdialog->Update(i))
                    return true;
            } else {
               if(1) {
                    progressdialog = new wxProgressDialog(
                        _("Route"), _("Loading"), count, this,
                        wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
                }
            }

                for(TiXmlElement* f = e->FirstChildElement(); f; f = f->NextSiblingElement()) {
                    if(!strcmp(f->Value(), "rtept")) {
                        wxString rte_lat = wxString::FromUTF8(f->Attribute("lat"));
						wxString rte_lon = wxString::FromUTF8(f->Attribute("lon"));

						my_position.lat = rte_lat;
						my_position.lon = rte_lon;
						my_positions.push_back(my_position);
					}  //else if(!strcmp(f->Value(), "extensions")) {
                        //rte_start = wxString::FromUTF8(f->Attribute("opencpn:start"));
						//rte_end = wxString::FromUTF8(f->Attribute("opencpn:end"));

                    //}
                }

        }
    }

    delete progressdialog;
    return true;

failed:
    delete progressdialog;

    wxMessageDialog mdlg(this, error, _("Openwaterkaart"), wxOK | wxICON_ERROR);
    mdlg.ShowModal();

    return false;
}

void Dlg::onDLEvent(OCPN_downloadEvent &ev){
    if(ev.getDLEventCondition() == OCPN_DL_EVENT_TYPE_END) downloadfinished = true;
}

void Dlg::OnDownloadCharts( wxCommandEvent& event ){
    if(OCPN_isOnline()){
        //Deze code is gekopieerd, dit haalt de basechartdir op, maar ik weet niet helemaal hoe
        wxString m_base_chart_dir;
        wxFileName fn(GetWritableDocumentsDir(), wxEmptyString);
        fn.AppendDir(CHART_DIR);
        GetOCPNConfigObject()->Read ( _T ( "BaseChartDir" ), &m_base_chart_dir, fn.GetPath());

        //Hier regel ik de chartPath en de configPath voor Openwaterkaart
        wxString filename = "Openwaterkaart";
        wxString ext = "conf";
        wxFileName configPath(m_base_chart_dir, filename, ext);
        wxFileName chartPath(m_base_chart_dir, filename);
        std::vector<string> xmlurls = {"https://charts.openwaterkaart.nl/OSM-raster-nl/OSM-raster-nl-catalog.xml", "https://charts.openwaterkaart.nl/OSM-raster-be/OSM-raster-be-catalog.xml", "https://charts.openwaterkaart.nl/OSM-raster-de/OSM-raster-de-catalog.xml", "http://charts.openwaterkaart.nl/OSM-raster-dk/OSM-raster-dk-catalog.xml"};
        std::vector<string> landstrings = {"Nederlandse", "Belgische", "Duitse", "Deense"};
        std::vector<string> ccp = {"NL:","BE:","DE:","DK:"}; //configcountryprefix
        //Als de config nog niet bestaat moet dit het eerste gebruik zijn van de plugin en dan moeten er nog wat voorbereidingen plaatsvinden
        bool firstTime = !configPath.FileExists();
        if(firstTime){
            ofstream configcreatestream(configPath.GetFullPath());
            for(int cc = 0; cc < (int)ccp.size(); cc++){
                configcreatestream << ccp[cc] + "00000000\n";
            }
            configcreatestream.close();
            wxFileName tmp(m_base_chart_dir);
            wxFileName::Mkdir(chartPath.GetFullPath());
            wxArrayString myChartArray = GetChartDBDirArrayString();
            myChartArray.Add(chartPath.GetFullPath());
            UpdateChartDBInplace(myChartArray, true, true);
        }
        ifstream configloadstream(configPath.GetFullPath());
        string line;
        std::vector<std::string> configvector;
        while ( getline (configloadstream,line) ){
            std::string x = line;                                                   //Hoeft maar 1 keer ofc
            configvector.push_back(x);
            //wxMessageBox(x.find("DE:") != std::string::npos ? "true" : "false");
        }
        configloadstream.close();
        wxFileName newXMLPath(m_base_chart_dir, "ChartXML", "xml");
        Connect(wxEVT_DOWNLOAD_EVENT, (wxObjectEventFunction)(wxEventFunction)&Dlg::onDLEvent);
        downloadfinished = false;




        //wxExtractZipFiles("/home/d/Charts/Openwaterkaart/ONC-eu12_00070008.zip","/home/d/Charts/ONC-eu12_00070008");
        bool ietsgevinkt = false;
        for(int c = 0; c < 4; c++){
            if(m_chartchecklist->IsChecked(c)){
                ietsgevinkt = true;
                long handle;
                _OCPN_DLStatus status = OCPN_downloadFileBackground( xmlurls[c], newXMLPath.GetFullPath(), this, &handle);
                while(!downloadfinished.load()){
                    wxYield();
                    wxMilliSleep(10);
                }
                downloadfinished = false;
                tinyxml2::XMLDocument newXML;
                tinyxml2::XMLError errorCheck = newXML.LoadFile(newXMLPath.GetFullPath());
                string olddate = "00000000";
                //if(errorCheck == tinyxml2::XML_SUCCESS) wxMessageBox("Xml geladen");
                tinyxml2::XMLNode * docRoot = newXML.FirstChildElement();
                //if(docRoot == nullptr) wxMessageBox("fout");
                tinyxml2::XMLElement * latestUpdateElement = docRoot->FirstChildElement("chart")->FirstChildElement("zipfile_datetime");
                string latestUpdate = latestUpdateElement->GetText();
                latestUpdate = latestUpdate.substr(0,8);

                for(int i = 0; i < (int)configvector.size(); i++){
                    if(configvector[i].find(ccp[c]) != std::string::npos) {
                        olddate = configvector[i].substr(3,8);
                        if(latestUpdate>olddate){
                            configvector[i] = ccp[c] + latestUpdate;
                        }
                    }
                }
                if(latestUpdate>olddate){
                    int chartAmount = 0;
                    int progress = 0;
                    for(tinyxml2::XMLElement* e = docRoot->FirstChildElement("chart"); e != NULL; e = e->NextSiblingElement("chart")){
                        chartAmount++;
                    }
                    wxProgressDialog pdlg("Openwaterkaart", landstrings[c] + " kaarten gedownload: " + std::to_string(progress) + " van de " + std::to_string(chartAmount) + ".", chartAmount, this);
                    for(tinyxml2::XMLElement* e = docRoot->FirstChildElement("chart"); e != NULL; e = e->NextSiblingElement("chart")){
                        wxFileName tempfilename(chartPath.GetFullPath(), e->FirstChildElement("title")->GetText(), "zip");
                        OCPN_downloadFileBackground(e->FirstChildElement("zipfile_location")->GetText(), tempfilename.GetFullPath(), this, &handle);
                        while(!downloadfinished.load()){
                            wxYield();
                            wxMilliSleep(10);
                        }
                        downloadfinished = false;
                        pdlg.Update(++progress, landstrings[c] + " kaarten gedownload: " + std::to_string(progress) + " van de " + std::to_string(chartAmount) + ".");

                    }




                }
            }

        }

        if(ietsgevinkt){
            int zipamount = 0;
            int progress = 0;
            wxString a = wxFindFirstFile( chartPath.GetFullPath() + wxFileName::GetPathSeparator() + "*.zip");
            while (!a.empty()){
                zipamount++;
                a = wxFindNextFile();
            }

            wxProgressDialog pdlg("Openwaterkaart", "Kaarten uitpakken: " + std::to_string(progress) + " van de " + std::to_string(zipamount) + ".", zipamount, this);

            wxString f = wxFindFirstFile( chartPath.GetFullPath() + wxFileName::GetPathSeparator() + "*.zip");
            while (!f.empty()){
                auto_ptr<wxZipEntry> entry;
                wxFileInputStream in(f);
                wxZipInputStream zip(in);
                while (entry.reset(zip.GetNextEntry()), entry.get() != NULL)
                {
                    // access meta-data
                    wxString name = entry->GetName();
                    wxFileName singlechartpath(chartPath.GetFullPath(),name);
                    wxFileOutputStream fos(singlechartpath.GetFullPath());
                    zip.Read(fos);
                    // read 'zip' to access the entry's data
                }
                wxRemoveFile(f);
                f = wxFindNextFile();
                pdlg.Update(++progress,"Kaarten uitpakken: " + std::to_string(progress) + " van de " + std::to_string(zipamount) + ".");
            }

            ofstream configwrite(configPath.GetFullPath());
            for(int i = 0; i < (int)configvector.size(); i++){
                configwrite << configvector[i] + "\n";
            }
            configwrite.close();
            Disconnect(wxEVT_DOWNLOAD_EVENT, (wxObjectEventFunction)(wxEventFunction)&Dlg::onDLEvent);
            UpdateChartDBInplace(GetChartDBDirArrayString(), true, false);
            ForceChartDBUpdate();
            //ForceChartDBRebuild();
            wxMessageBox("De meest recente kaarten zijn geïnstalleerd.");
        }else wxMessageBox("Er zijn geen landen gekozen, probeer het nog eens.");


    }else{
        wxMessageBox("Geen internetverbinding, geen update mogelijk voor nu.");
    }

	/*ofstream outfile (configPath.GetFullPath());
	if (outfile.is_open()){
        outfile << "This is a line.\n";
        outfile << "This is another line.\n";
        outfile.close();
        string line;
        ifstream myfile (configPath.GetFullPath());
        if (myfile.is_open()){
            while ( getline (myfile,line) ){
                wxMessageBox(_(line));
            }
            myfile.close();
        }
        else wxMessageBox(_("fout2"));
    }
    else wxMessageBox(_("fout1"));
    if(configPath.FileExists()) wxMessageBox(_("gelukt"));*/
}

void Dlg::OnDownloadLayers( wxCommandEvent& event ){
    /*wxString m_base_chart_dir;
    wxFileName fn(GetWritableDocumentsDir(), wxEmptyString);
    fn.AppendDir(_T(CHART_DIR));
    GetOCPNConfigObject()->Read ( _T ( "BaseChartDir" ), &m_base_chart_dir,  fn.GetPath() );
    wxArrayString myChartArray = GetChartDBDirArrayString();
    wxString newChartFolder = m_base_chart_dir;
    myChartArray.Add(newChartFolder);
    bool update = UpdateChartDBInplace(myChartArray, true, true);
    if (!update)wxMessageBox(_("Unable to update the chart database"));
	ForceChartDBUpdate();
	wxMessageBox(_(m_base_chart_dir));*/

}
