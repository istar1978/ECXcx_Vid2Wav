
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_PANEL_VID_INFO_H
#define ECXcx_PANEL_VID_INFO_H

#include "ECXcx_ResMgr.h"
#include "ECXcx_TreeCtrl.h"

class ECXcx_PanelVidInfo: public wxWindow
{
public:
	ECXcx_PanelVidInfo( wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0 );

	void	InitCtrlStatus();

private:
	ECXcx_TreeCtrl	m_treeCtrl;

	wxDECLARE_EVENT_TABLE();
private:
	void	OnSize( wxSizeEvent& evt );
};

#endif //end ECXcx_PANEL_VID_INFO_H

