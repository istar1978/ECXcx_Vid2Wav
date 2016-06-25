
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_PanelMain.h"
#include "ECXcx_MainFrame.h"
#include "ECXcx_PanelVidInfo.h"

ECXcx_PanelMain::ECXcx_PanelMain( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
:wxWindow(parent, id, pos, size, style)
{
	this->m_nWidthClientMin	=	ECXcx::E_Consts_DFT_BORDER_WH
							+	ECXcx::E_Consts_DFT_PANEL_CTRL_WIDTH + ECXcx::E_Consts_DFT_BORDER_WH;
	this->m_nHeightClientMin=	ECXcx::E_Consts_DFT_BORDER_WH
							+	ECXcx::E_Consts_DFT_TREECTRL_HEIGHT + ECXcx::E_Consts_DFT_BORDER_WH
							+	ECXcx::E_Consts_DFT_LOG_CTRL_HEIGHT + ECXcx::E_Consts_DFT_BORDER_WH;

	this->SetMinSize(wxSize(this->m_nWidthClientMin, this->m_nHeightClientMin));

	wxPoint	posCtrl;
	wxSize	sizeCtrl;

	// { Tab Pages
		posCtrl.x	=	ECXcx::E_Consts_DFT_BORDER_WH;
		posCtrl.y	=	ECXcx::E_Consts_DFT_BORDER_WH;
		sizeCtrl.SetWidth( ECXcx::E_Consts_DFT_PANEL_CTRL_WIDTH );
		sizeCtrl.SetHeight( this->m_nHeightClientMin - 2*ECXcx::E_Consts_DFT_BORDER_WH );
		this->m_pTabCtrl	=	new wxNotebook(this, wxID_ANY, posCtrl, sizeCtrl);

		posCtrl.x	=	0;
		posCtrl.y	=	0;
		sizeCtrl.SetWidth( sizeCtrl.GetWidth() - ECXcx::E_Consts_DFT_BORDER_WH );
		sizeCtrl.SetHeight( sizeCtrl.GetHeight() - ECXcx::E_Consts_DFT_BORDER_WH );
		this->m_pPanelVidInfo	=	new ECXcx_PanelVidInfo(this->m_pTabCtrl, wxID_ANY, posCtrl, sizeCtrl);

		this->m_pTabCtrl->InsertPage(E_TabIndex_VidInfo, this->m_pPanelVidInfo, mT("Vid Info"));
	// } end Tab Pages

	// { Log
	posCtrl.x	=	ECXcx::E_Consts_DFT_BORDER_WH;
	posCtrl.y	=	this->m_nHeightClientMin - ECXcx::E_Consts_DFT_BORDER_WH - ECXcx::E_Consts_DFT_LOG_CTRL_HEIGHT;
	sizeCtrl.SetWidth( this->m_nWidthClientMin - 2*ECXcx::E_Consts_DFT_BORDER_WH );
	sizeCtrl.SetHeight( ECXcx::E_Consts_DFT_LOG_CTRL_HEIGHT );
	this->m_pTextCtrlLog	=	new wxTextCtrl(this, wxID_ANY, wxT(""), posCtrl, sizeCtrl, wxTE_READONLY | wxTE_MULTILINE);
	// } end Log

	this->InitCtrlStatus();
}

wxSize ECXcx_PanelMain::GetMinSize()
{
	return wxSize(this->m_nWidthClientMin, this->m_nHeightClientMin);
}

void ECXcx_PanelMain::InitCtrlStatus()
{
	this->m_pTextCtrlLog->Clear();
}

void ECXcx_PanelMain::Log( int32_t nDebugLevel, const wxString& strLog )
{
	this->m_pTextCtrlLog->AppendText(strLog);
}

wxBEGIN_EVENT_TABLE(ECXcx_PanelMain, wxWindow)
	EVT_SIZE(ECXcx_PanelMain::OnSize)
wxEND_EVENT_TABLE()

void ECXcx_PanelMain::OnSize( wxSizeEvent& evt )
{
	wxSize	sizeClient	=	this->GetClientSize();

	wxSize	sizeTabCtrl;
	sizeTabCtrl.SetWidth( sizeClient.GetWidth() - 2*ECXcx::E_Consts_DFT_BORDER_WH );
	sizeTabCtrl.SetHeight( sizeClient.GetHeight() - ECXcx::E_Consts_DFT_LOG_CTRL_HEIGHT - 3*ECXcx::E_Consts_DFT_BORDER_WH);
	this->m_pTabCtrl->SetSize( sizeTabCtrl );

	wxSize	sizeTabPage;
	sizeTabPage.SetWidth( sizeTabCtrl.GetWidth() - ECXcx::E_Consts_DFT_BORDER_WH );
	sizeTabPage.SetHeight( sizeTabCtrl.GetHeight() - 2*ECXcx::E_Consts_DFT_BORDER_WH );
	this->m_pTabCtrl->SetPageSize( sizeTabPage );

	wxSize	sizeLogCtrl	=	this->m_pTextCtrlLog->GetSize();
	sizeLogCtrl.SetWidth( sizeClient.GetWidth() - 2*ECXcx::E_Consts_DFT_BORDER_WH );
	//sizeLogCtrl.SetHeight( sizeClient.GetHeight() - ECXcx::E_Consts_DFT_LOG_CTRL_HEIGHT );
	this->m_pTextCtrlLog->SetSize(sizeLogCtrl);
	this->m_pTextCtrlLog->Move( ECXcx::E_Consts_DFT_BORDER_WH, sizeClient.GetHeight() - ECXcx::E_Consts_DFT_BORDER_WH - ECXcx::E_Consts_DFT_LOG_CTRL_HEIGHT );
}

