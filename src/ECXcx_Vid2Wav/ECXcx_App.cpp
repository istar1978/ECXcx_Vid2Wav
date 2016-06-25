
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_App.h"
#include "ECXcx_MainFrame.h"
#include "ECXcx_ResMgr.h"

wxIMPLEMENT_APP(ECXcx_App);

bool ECXcx_App::OnInit()
{
	/*
	if ( ! wxApp::OnInit() )
	{
		return false;
	}
	*/
	if ( ECXcx_ResMgr::Instance()->Init() != ECX::E_Errno_SUCCESS )
	{
		wxMessageBox(
			wxString(wxT("ECXcx_ResMgr::Init() failed!!!")),
			wxT("ECXcx_Vid2Wav")
			);

		return false;
	}

	ECXcx_MainFrame*	pMainFrame	=
		new ECXcx_MainFrame( mT("ECXcx_Vid2Wav") );
	this->SetTopWindow( pMainFrame );

	pMainFrame->CenterOnScreen();
	pMainFrame->Show( true );

	return true;
}

int ECXcx_App::OnExit()
{
	ECXcx_ResMgr::Destroy();

	return wxApp::OnExit();
}

