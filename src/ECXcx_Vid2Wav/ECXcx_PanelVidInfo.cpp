
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_PanelVidInfo.h"
#include "ECXcx_App.h"
#include "ECXcx_MainFrame.h"

ECXcx_PanelVidInfo::ECXcx_PanelVidInfo( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
:wxWindow(parent, id, pos, size, style)
,m_treeCtrl(this, ECXcx::E_CtrlId_PanelVidInfo_TreeCtrl, wxPoint(0, 0), wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT )
{
	wxSize	sizeClient	=	this->GetClientSize();
	this->m_treeCtrl.SetSize( sizeClient );

	this->InitCtrlStatus();
}

void ECXcx_PanelVidInfo::InitCtrlStatus()
{
}


wxBEGIN_EVENT_TABLE(ECXcx_PanelVidInfo, wxWindow)
	EVT_SIZE(ECXcx_PanelVidInfo::OnSize)
wxEND_EVENT_TABLE()

void ECXcx_PanelVidInfo::OnSize( wxSizeEvent& evt )
{
	wxSize	sizeClient	=	this->GetClientSize();

	this->m_treeCtrl.SetSize( 0, 0, sizeClient.GetWidth(), sizeClient.GetHeight() );
}

