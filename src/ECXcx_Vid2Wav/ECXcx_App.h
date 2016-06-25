
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_APP_H
#define ECXcx_APP_H

#include "ECXcx_AppTypes.h"

class ECXcx_App: public wxApp
{
public:
	virtual bool	OnInit();
	virtual int		OnExit();
};

DECLARE_APP(ECXcx_App)
//ECXcx_MainFrame*	pMainFrame	=	(ECXcx_MainFrame*)( wxGetApp().GetTopWindow() );

#endif //end ECXcx_APP_H

