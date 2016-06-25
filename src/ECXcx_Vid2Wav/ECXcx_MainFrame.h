
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_MAIN_FRAME_H
#define ECXcx_MAIN_FRAME_H

#include "ECXcx_ResMgr.h"

class ECXcx_PanelMain;
class ECXcx_MainFrame: public wxFrame, public ECXcx_Logger
{
public:
	ECXcx_MainFrame( const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

public:
	void	CreateMenuBar();
	void	CreateMainPanel();
	void	CreateStatusBar();
	void	InitCtrlStatus();

	virtual int32_t	Log( const ECXcx::ET_LogLevel& eLogLevel, const char* pcLog, int32_t lenLog );
	void	OnLogHandle( wxCommandEvent& evt );

private:
	wxMenu*		m_pMenuConfigExtCmds;
	wxMenuItem*	m_pMenuItemExit;

	wxMenuItem*	m_pMenuItemAbout;

	ECXcx_PanelMain*	m_pPanelMain;

	wxDECLARE_EVENT_TABLE();
private:
	void	OnClose( wxCloseEvent& evt );

	// { menuBar
	void	OnConfigExtCmds( wxCommandEvent& evt );
	void	OnExit( wxCommandEvent& evt );

	void	OnAbout( wxCommandEvent& evt );
	// } end menuBar
};

#endif //end ECXcx_MAIN_FRAME_H

