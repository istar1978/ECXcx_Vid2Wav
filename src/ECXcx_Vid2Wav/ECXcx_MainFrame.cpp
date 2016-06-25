
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_MainFrame.h"
#include "ECXcx_PanelMain.h"

ECXcx_MainFrame::ECXcx_MainFrame( const wxString& title, const wxPoint& pos, const wxSize& size )
:wxFrame(NULL, wxID_ANY, title, pos, size)
{
#ifdef WIN32
	this->SetIcon( wxICON(ECXcx_AppIcon) );
#endif //end WIN32

	this->CreateMenuBar();
	this->CreateMainPanel();
	this->CreateStatusBar();

	wxSize	sizeClientMin	=	this->m_pPanelMain->GetMinSize();
	this->SetClientSize(sizeClientMin);
	this->SetMinClientSize(sizeClientMin);

	this->InitCtrlStatus();

	ECXcx_LogMgr::Instance()->RegisterLogger( this );
}

void ECXcx_MainFrame::CreateMenuBar()
{
	std::vector< wxString >	vecExtCmdNames;
	ECXcx_ResMgr::Instance()->GetExtCmds( vecExtCmdNames );
	size_t	uCnt	=	vecExtCmdNames.size();

	wxMenu*	pMenuFile		=	new wxMenu;
	this->m_pMenuConfigExtCmds	=	new wxMenu;
	for ( size_t i=0; i<uCnt; ++i )
	{
		this->m_pMenuConfigExtCmds->Append( ECXcx::E_GroupCmdId_Begin_OnConfigExtCmds + i, vecExtCmdNames[i] );
	}
	wxEvtHandler::Connect( ECXcx::E_GroupCmdId_Begin_OnConfigExtCmds, ECXcx::E_GroupCmdId_Begin_OnConfigExtCmds + uCnt, wxEVT_MENU, wxCommandEventHandler(ECXcx_MainFrame::OnConfigExtCmds) );
	//wxEvtHandler::Disconnect( ECXcx::E_GroupCmdId_Begin_OnConfigExtCmds, ECXcx::E_GroupCmdId_Begin_OnConfigExtCmds + uCnt, wxEVT_MENU );
	pMenuFile->AppendSubMenu( this->m_pMenuConfigExtCmds, wxT("Config ExtCmds") );
	pMenuFile->AppendSeparator();
	this->m_pMenuItemExit	=	pMenuFile->Append( ECXcx::E_CmdId_Exit, mT("E&xit") + wxT("\tAlt-F4") );
	wxMenu*	pMenuHelp		=	new wxMenu;
	this->m_pMenuItemAbout	=	pMenuHelp->Append( ECXcx::E_CmdId_About, mT("&About") );

	wxMenuBar*	pMenuBar	=	new wxMenuBar;
	pMenuBar->Append( pMenuFile, mT("&File") );
	pMenuBar->Append( pMenuHelp, mT("&Help") );

	this->SetMenuBar( pMenuBar );
}

void ECXcx_MainFrame::CreateMainPanel()
{
	this->m_pPanelMain	=
		new ECXcx_PanelMain( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN );
}

void ECXcx_MainFrame::CreateStatusBar()
{
	wxFrame::CreateStatusBar();
	this->SetStatusText( wxString(mT("Welcome to use")) + wxT(" ") + mT("ECXcx_Vid2Wav") );
}

void ECXcx_MainFrame::InitCtrlStatus()
{
	// { menuBar
	this->m_pMenuItemExit->Enable( true );

	this->m_pMenuItemAbout->Enable( true );
	// } end menuBar

	this->m_pPanelMain->InitCtrlStatus();
}

int32_t ECXcx_MainFrame::Log( const ECXcx::ET_LogLevel& eLogLevel, const char* pcLog, int32_t lenLog )
{
	char*	pcLogData	=	new char[sizeof(int32_t)+lenLog+1];
	if ( pcLogData == NULL )
	{
		return ECX::E_Errno_ERR_GENERAL;
	}
	*((int32_t*)pcLogData)	=	lenLog;
	::memcpy( pcLogData+sizeof(int32_t), pcLog, lenLog );
	pcLogData[sizeof(int32_t)+lenLog]	=	'\0';

	wxCommandEvent*	pEvtLog	=	new wxCommandEvent(wxEVT_COMMAND_TEXT_UPDATED, ECXcx::E_CmdId_OnLogItemPrepared);
	if ( pEvtLog != NULL )
	{
		pEvtLog->SetClientData( pcLogData );
		pcLogData	=	NULL;
	}
	this->QueueEvent( pEvtLog );

	return ECX::E_Errno_SUCCESS;
}

void ECXcx_MainFrame::OnLogHandle( wxCommandEvent& evt )
{
	char*	pcLogData	=	(char*) evt.GetClientData();
	ECXcx::ET_LogLevel	eLogLevel	=	(ECXcx::ET_LogLevel)( *((int32_t*)pcLogData) );
	this->m_pPanelMain->Log( eLogLevel, wxString::FromUTF8(pcLogData+sizeof(uint32_t)) );
	delete [] pcLogData;
	pcLogData	=	NULL;
	evt.SetClientData( NULL );
}

wxBEGIN_EVENT_TABLE(ECXcx_MainFrame, wxFrame)
	EVT_CLOSE(ECXcx_MainFrame::OnClose)

	// { menuBar
	EVT_MENU(ECXcx::E_CmdId_Exit, ECXcx_MainFrame::OnExit)

	EVT_MENU(ECXcx::E_CmdId_About, ECXcx_MainFrame::OnAbout)
	// } end menuBar

	EVT_COMMAND( ECXcx::E_CmdId_OnLogItemPrepared, wxEVT_COMMAND_TEXT_UPDATED, ECXcx_MainFrame::OnLogHandle)
wxEND_EVENT_TABLE()

void ECXcx_MainFrame::OnClose( wxCloseEvent& evt )
{
	if ( evt.CanVeto() )
	{
		if ( wxMessageBox(
				mT("Are you sure to QUIT?"),
				mT("ECXcx_Vid2Wav"),
				wxICON_QUESTION | wxYES_NO
				) != wxYES
			)
		{
			evt.Veto();
			return;
		}
	}

	this->Destroy();
}

// { menuBar

void ECXcx_MainFrame::OnConfigExtCmds( wxCommandEvent& evt )
{
	wxMenuItem*	pMenuItemConfigExtCmd	=
		this->m_pMenuConfigExtCmds->FindItem( evt.GetId() );
	if ( pMenuItemConfigExtCmd == NULL )
	{
		wxMessageBox( wxT("Not Found!") );
		return;
	}

	wxString	wxstrExtCmdName		=	pMenuItemConfigExtCmd->GetItemLabelText();
	wxString	wxstrExtCmdFormatOld=	ECXcx_ResMgr::Instance()->GetExtCmdFormat( wxstrExtCmdName );

	wxTextEntryDialog	dlg( NULL, wxstrExtCmdName, wxT("Config ExtCmd"), wxstrExtCmdFormatOld );
	if ( dlg.ShowModal() == wxID_OK )
	{
		wxString	wxstrExtCmdFormat	=	dlg.GetValue();
		if ( wxstrExtCmdFormat != wxstrExtCmdFormatOld )
		{
			ECXcx_ResMgr::Instance()->SetExtCmdFormat( wxstrExtCmdName, wxstrExtCmdFormat );
		}
	}
}

void ECXcx_MainFrame::OnExit( wxCommandEvent& evt )
{
	this->Close();
}

void ECXcx_MainFrame::OnAbout( wxCommandEvent& evt )
{
	wxMessageBox(
		mT("ECXcx_Vid2Wav") + wxT("\n\n")
			+	mT("Version") + wxT(": ") + ECXcx_ResMgr::Instance()->GetVersion() + wxT("\n")
			+	mT("Release Date") + wxT(": ") + ECXcx_ResMgr::Instance()->GetReleaseDate() + wxT("\n")
			+	mT("Author") + wxT(": ") + mT("Zhu Fei") + wxT("\n")
			+	mT("Email") + wxT(": ") + wxT("zhufei_13967158519@hotmail.com"),
		mT("About") + wxT(" ") + mT("ECXcx_Vid2Wav"),
		wxOK | wxICON_INFORMATION
		);
}
	// } end menuBar

