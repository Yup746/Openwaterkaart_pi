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

#include "Openwaterkaartgui_impl.h"
#include "Openwaterkaartgui.h"
#include "Openwaterkaart_pi.h"
#include "icons.h"

#include <wx/progdlg.h>
#include "ocpn_plugin.h"
#include <fstream>
#include "tinyxml2.h"
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/stdpaths.h>

Dlg::Dlg(wxWindow *parent, Openwaterkaart_pi *ppi)
	: m_Dialog(parent)
{
	this->Fit();

	pPlugIn = ppi;
	pParent = parent;

	wxString blank_name = *GetpSharedDataLocation()
		+ _T("plugins/Openwaterkaart_pi/data/blank.ico");

	wxIcon icon(blank_name, wxBITMAP_TYPE_ICO);
	SetIcon(icon);

    //Deze code is gekopieerd, dit haalt de basechartdir op, maar ik weet niet helemaal hoe
    fn.Assign(GetWritableDocumentsDir(), wxEmptyString);
    fn.AppendDir(_("Charts"));
    GetOCPNConfigObject()->Read ( _T ( "BaseChartDir" ), &m_base_chart_dir, fn.GetPath());
    if(!wxFileName::DirExists(m_base_chart_dir)) wxFileName::Mkdir(m_base_chart_dir); //Maak de basechartdir aan als deze nog niet bestaat

    //Dit initieert wat variabelen die later terugkomen
    configPath.Assign(m_base_chart_dir, "Openwaterkaart", "conf");
    chartPath.Assign(m_base_chart_dir, "Openwaterkaart");
    ccp = {"NL:","BE:","DE:","DK:"}; //configcountryprefix
    xmlurls = {"https://charts.openwaterkaart.nl/OSM-raster-nl/OSM-raster-nl-catalog.xml", "https://charts.openwaterkaart.nl/OSM-raster-be/OSM-raster-be-catalog.xml", "https://charts.openwaterkaart.nl/OSM-raster-de/OSM-raster-de-catalog.xml", "http://charts.openwaterkaart.nl/OSM-raster-dk/OSM-raster-dk-catalog.xml"};
    landstrings = {"Nederlandse", "Belgische", "Duitse", "Deense"};

    //Als de config nog niet bestaat moet dit het eerste gebruik zijn van de plugin en dan moeten er nog wat voorbereidingen plaatsvinden
    if(!configPath.FileExists()){
        //Hier wordt de configfile gemaakt
        ofstream configcreatestream(configPath.GetFullPath().mb_str());
        for(int cc = 0; cc < (int)ccp.size(); cc++){
            configcreatestream << ccp[cc] + "00000000\n";
        }
        configcreatestream << "mailadres:\nlicentie:\n";
        configcreatestream.close();

        //Hier maak ik de kaartenmap aan en voeg deze toe aan de database van OpenCPN
        wxFileName::Mkdir(chartPath.GetFullPath());
        wxArrayString myChartArray = GetChartDBDirArrayString();
        myChartArray.Add(chartPath.GetFullPath());
        UpdateChartDBInplace(myChartArray, true, true);
    }

	//Hier wordt het configobject geïnitialiseerd
    config = new OpenwaterkaartConfig;
    config->SetConfigPath(configPath.GetFullPath());
    XMLPath.Assign(m_base_chart_dir, "XML", "xml");


	//Dit haalt het al bekende mailadres en licentiecode op uit de config en laadt het in de tekstvakken
	m_mailadress->SetValue(config->GetMailadress());
	m_licensekey->SetValue(config->GetLicense());

	//Voorwerk voor downloaden van lagen
    if(wxPlatformInfo::Get().GetOperatingSystemFamilyName() == "Unix"){
        workdir = wxFileName(GetWritableDocumentsDir(), wxEmptyString);
        workdir.AppendDir(".opencpn");
    }else if(wxPlatformInfo::Get().GetOperatingSystemFamilyName() == "Windows"){
        workdir = wxFileName("C:\\ProgramData\\opencpn\\");
    }else if(wxPlatformInfo::Get().GetOperatingSystemFamilyName() == "Macintosh"){
        workdir.Assign(wxStandardPaths::Get().GetUserConfigDir(),"opencpn");
    }
    icondir.Assign(workdir.GetFullPath(),"UserIcons");
    layerdir.Assign(workdir.GetFullPath(),"layers");
    if(!wxFileName::DirExists(icondir.GetFullPath())) wxFileName::Mkdir(icondir.GetFullPath());
    if(!wxFileName::DirExists(layerdir.GetFullPath())) wxFileName::Mkdir(layerdir.GetFullPath());
}

void Dlg::OnClose(wxCloseEvent& event)
{
    config->SetMailadress(m_mailadress->GetValue());
    config->SetLicense(m_licensekey->GetValue());
	pPlugIn->OnOpenwaterkaartDialogClose();
}

void Dlg::DownloadNoDialog(wxString url, wxString path, wxEvtHandler* handler, long* handle){
    downloadfinished = false;
    OCPN_downloadFileBackground( url, path, handler, handle);
    while(!downloadfinished){
        wxYield();
        wxMilliSleep(10);
    }
    downloadfinished = false;
}

void Dlg::onDLEvent(OCPN_downloadEvent &ev){
    if(ev.getDLEventCondition() == OCPN_DL_EVENT_TYPE_END) downloadfinished = true;
}

void Dlg::OnDownloadCharts( wxCommandEvent& event ){ //Functie voor het downloaden van de kaarten
    if(OCPN_isOnline()){
        Connect(wxEVT_DOWNLOAD_EVENT, (wxObjectEventFunction)(wxEventFunction)&Dlg::onDLEvent);
        bool ietsgevinkt = false; //Flag om te kijken of er iets is gebeurd

        for(int c = 0; c < ccp.size(); c++){ //Loop door alle landcodes
            if(m_chartchecklist->IsChecked(c)){ //Als ze aangevinkt zijn om te downloaden
                ietsgevinkt = true;

                //Hier wordt de XML van de landen gedownload en geparsed
                long handle;
                DownloadNoDialog( xmlurls[c], XMLPath.GetFullPath(), this, &handle);
                tinyxml2::XMLDocument newXML;
                tinyxml2::XMLError errorCheck = newXML.LoadFile(XMLPath.GetFullPath());
                tinyxml2::XMLNode * docRoot = newXML.FirstChildElement();
                tinyxml2::XMLElement * latestUpdateElement = docRoot->FirstChildElement("chart")->FirstChildElement("zipfile_datetime");
                string latestUpdate = latestUpdateElement->GetText();
                latestUpdate = latestUpdate.substr(0,8);
                string olddate = config->GetLastUpdated(ccp[c]);

                if(latestUpdate>olddate){ //Als de kaarten van dit land een update vereisen
                    config->SetLastUpdated(ccp[c],latestUpdate);
                    int chartAmount = 0;
                    int progress = 0;
                    for(tinyxml2::XMLElement* e = docRoot->FirstChildElement("chart"); e != NULL; e = e->NextSiblingElement("chart")){ //Tellen van aantal kaarten
                        chartAmount++;
                    }

                    //Het downloaden van de kaarten
                    wxProgressDialog pdlg("Openwaterkaart.nl", landstrings[c] + " kaarten gedownload: " + to_string(progress) + " van de " + to_string(chartAmount) + ".", chartAmount, this);
                    for(tinyxml2::XMLElement* e = docRoot->FirstChildElement("chart"); e != NULL; e = e->NextSiblingElement("chart")){
                        wxFileName tempfilename(chartPath.GetFullPath(), e->FirstChildElement("title")->GetText(), "zip");
                        DownloadNoDialog(e->FirstChildElement("zipfile_location")->GetText(), tempfilename.GetFullPath(), this, &handle);

                        //Uitpakken
                        unique_ptr<wxZipEntry> entry;
                        wxFileInputStream in(tempfilename.GetFullPath());
                        wxZipInputStream zip(in);
                        while (entry.reset(zip.GetNextEntry()), entry.get() != NULL)
                        {
                            wxString name = entry->GetName();
                            wxFileName singlechartpath(chartPath.GetFullPath(),name);
                            wxFileOutputStream fos(singlechartpath.GetFullPath());
                            zip.Read(fos);
                        }
                        wxRemoveFile(tempfilename.GetFullPath());
                        progress++;
                        pdlg.Update(progress, landstrings[c] + " kaarten gedownload: " + to_string(progress) + " van de " + to_string(chartAmount) + ".");
                    }
                }
            }
        }

        if(ietsgevinkt){ //Als de checkbox niet per ongeluk leeg gelaten is
            //Afsluitende functies
            UpdateChartDBInplace(GetChartDBDirArrayString(), true, false);
            ForceChartDBUpdate();
            wxMessageBox("De meest recente kaarten zijn gedownload.\nStart OpenCPN opnieuw op als de\nkaarten niet zichtbaar worden.");
        }else wxMessageBox("Er zijn geen landen gekozen,\nmaak minimaal één keuze.");
        Disconnect(wxEVT_DOWNLOAD_EVENT, (wxObjectEventFunction)(wxEventFunction)&Dlg::onDLEvent);
    }else wxMessageBox("Geen internetverbinding,\nupdaten is niet mogelijk.");
}

void Dlg::OnDownloadLayers( wxCommandEvent& event ){ //Functie voor het downloaden van de betaalde lagen
    Connect(wxEVT_DOWNLOAD_EVENT, (wxObjectEventFunction)(wxEventFunction)&Dlg::onDLEvent);
    if(!OCPN_isOnline()){
        wxMessageBox("Geen internetverbinding,\nupdaten is niet mogelijk.");
    }else{
        //Hier wordt een XML opgehaald op basis van het opgegeven mailadres en licentiecode
        long handle;
        DownloadNoDialog("https://www.openwaterkaart.nl/charts/process.php?command=license&email=" + m_mailadress->GetValue() + "&code=" + m_licensekey->GetValue() + "&want=xml" , XMLPath.GetFullPath(), this, &handle);
        tinyxml2::XMLDocument newXML;
        tinyxml2::XMLError errorCheck = newXML.LoadFile(XMLPath.GetFullPath());
        tinyxml2::XMLNode * docRoot = newXML.FirstChildElement();
        tinyxml2::XMLElement * licenseokelement = docRoot->FirstChildElement("license");
        string licenseok = licenseokelement->GetText();
        /*Hier wordt de door de server geleverde waarde geparsed:
            ok = Mailadres en licentiecode werken
            nok = Mailadres en licentiecode zijn onbekend
            mok = Het mailadres is bekend, maar de licentiecode is waarschijnlijk verkeerd gespeld
            bok = De combinatie is wel bekend, maar is nog niet geactiveerd door de beheerder
            vok = Er is te vaak geprobeerd in te loggen (wordt beoordeeld door de server), er wordt verzocht contact op te nemen met de beheerder
            */
        if(licenseok == "ok"){
            { //Deze extra scope dient ervoor om ervoor te zorgen dat de progressdialog op tijd verdwijnt
                int steps = 6;
                int progress = 0;
                //Hier worden de icons gedownload
                wxProgressDialog pdlg("Openwaterkaart.nl", "Lagen aan het downloaden...", steps, this);
                wxFileName iconzippath(icondir.GetFullPath(), "usericons", "zip");
                DownloadNoDialog("https://www.openwaterkaart.nl/charts/process.php?naam=usericons.zip&email=" + m_mailadress->GetValue() + "&code=" + m_licensekey->GetValue() + "&command=file" , iconzippath.GetFullPath(), this, &handle);
                progress++;
                pdlg.Update(progress);

                //Hier worden de lagen gedownload
                wxFileName layerzippath(layerdir.GetFullPath(), "layers", "zip");
                DownloadNoDialog("https://www.openwaterkaart.nl/charts/process.php?naam=layers.zip&email=" + m_mailadress->GetValue() + "&code=" + m_licensekey->GetValue() + "&command=file" , layerzippath.GetFullPath(), this, &handle);
                progress++;
                pdlg.Update(progress);

                //Hier worden de icons uitgepakt
                unique_ptr<wxZipEntry> iconentry;
                wxFileInputStream iconin(iconzippath.GetFullPath());
                wxZipInputStream iconzip(iconin);
                while (iconentry.reset(iconzip.GetNextEntry()), iconentry.get() != NULL)
                {
                    wxString name = iconentry->GetName();
                    wxFileName singleiconpath(icondir.GetFullPath(),name);
                    wxFileOutputStream fos(singleiconpath.GetFullPath());
                    iconzip.Read(fos);
                }
                progress++;
                pdlg.Update(progress);

                //Hier worden de lagen uitgepakt
                unique_ptr<wxZipEntry> layerentry;
                wxFileInputStream layerin(layerzippath.GetFullPath());
                wxZipInputStream layerzip(layerin);
                while (layerentry.reset(layerzip.GetNextEntry()), layerentry.get() != NULL)
                {
                    wxString name = layerentry->GetName();
                    wxFileName singlelayerpath(layerdir.GetFullPath(),name);
                    wxFileOutputStream fos(singlelayerpath.GetFullPath());
                    layerzip.Read(fos);
                }
                progress++;
                pdlg.Update(progress);

                wxRemoveFile(iconzippath.GetFullPath());
                progress++;
                pdlg.Update(progress);
                wxRemoveFile(layerzippath.GetFullPath());
                progress++;
                pdlg.Update(progress);
            }

            wxMessageBox("Lagen zijn toegevoegd/bijgewerkt.\nStart OpenCPN opnieuw op\nom de lagen zichtbaar te maken.");

        }else if(licenseok == "nok"){
            wxMessageBox("Gegevens onbekend,\nprobeer het nogmaals.");
        }else if(licenseok == "mok"){
            wxMessageBox("E-mailadres is bekend,\nmaar de licentiecode is onjuist.");
        }else if(licenseok == "bok"){
            wxMessageBox("Gegevens zijn juist, maar de\nlicentiecode is nog niet actief\n(Dit gebeurt meestal binnen 24 uur).");
        }else if(licenseok == "vok"){
            wxMessageBox("Er is iets misgegaan,\nneem contact op met\nklaus@openwaterkaart.nl");
        }
    }
    Disconnect(wxEVT_DOWNLOAD_EVENT, (wxObjectEventFunction)(wxEventFunction)&Dlg::onDLEvent);
}

string OpenwaterkaartConfig::GetLicense(){
    string license;
    for(int i = 0; i < (int)configvector.size(); i++){
        if(configvector[i].find("licentie:") != string::npos) license = configvector[i].substr(9);
    }
    return license;
}

string OpenwaterkaartConfig::GetMailadress(){
    string mail;
    for(int i = 0; i < (int)configvector.size(); i++){
        if(configvector[i].find("mailadres:") != string::npos) mail = configvector[i].substr(10);;
    }
    return mail;
}

string OpenwaterkaartConfig::GetLastUpdated(string ccp){
    string lastupdated;
    for(int i = 0; i < (int)configvector.size(); i++){
        if(configvector[i].find(ccp) != string::npos) lastupdated = configvector[i].substr(3,8);
    }
    return lastupdated;
}

void OpenwaterkaartConfig::SetConfigPath(wxString configloc){
    configLocation = configloc.ToStdString();
    ifstream configloadstream(configLocation);
    string line;
    while ( getline (configloadstream,line) ) {
        configvector.push_back(line);
    }
}

void OpenwaterkaartConfig::SetLicense(wxString license){
    for(int i = 0; i < (int)configvector.size(); i++){
        if(configvector[i].find("licentie:") != string::npos) configvector[i] = "licentie:" + license;
    }
    WriteToFile();
}

void OpenwaterkaartConfig::SetMailadress(wxString mail){
    for(int i = 0; i < (int)configvector.size(); i++){
        if(configvector[i].find("mailadres:") != string::npos) configvector[i] = "mailadres:" + mail;
    }
    WriteToFile();
}

void OpenwaterkaartConfig::SetLastUpdated(string ccp, string newdate){
    for(int i = 0; i < (int)configvector.size(); i++){
        if(configvector[i].find(ccp) != string::npos) configvector[i] = ccp + newdate;
    }
    WriteToFile();
}

void OpenwaterkaartConfig::WriteToFile(){
    ofstream configwrite(configLocation);
		for(int i = 0; i < (int)configvector.size(); i++){
			configwrite << configvector[i] + "\n";
		}
    configwrite.close();
}
