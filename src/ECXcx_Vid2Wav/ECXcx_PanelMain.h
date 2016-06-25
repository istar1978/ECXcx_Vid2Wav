
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_PANEL_MAIN_H
#define ECXcx_PANEL_MAIN_H

#include "ECXcx_AppTypes.h"

class ECXcx_PanelVidInfo;
class ECXcx_PanelMain: public wxWindow
{
public:
	ECXcx_PanelMain( wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0 );

	wxSize	GetMinSize();

	void	InitCtrlStatus();

	void	Log( int32_t nDebugLevel, const wxString& strLog );

private:
	int	m_nWidthClientMin;
	int	m_nHeightClientMin;

	wxNotebook*	m_pTabCtrl;
	ECXcx_PanelVidInfo*	m_pPanelVidInfo;

	wxTextCtrl*		m_pTextCtrlLog;

	enum ET_TabIndex
	{
		E_TabIndex_VidInfo	=	0,
		E_TabIndex_VixInfo	=	1,
	};

	wxDECLARE_EVENT_TABLE();
private:
	void	OnSize( wxSizeEvent& evt );
};

#endif //end ECXcx_PANEL_MAIN_H

