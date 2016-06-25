
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_TOOLS_H
#define ECXcx_TOOLS_H

#include "ECXcx_AppTypes.h"

namespace ECXcx_Tools
{
	wxTreeItemId	FindChild( const wxTreeCtrl& treeCtrl, const wxTreeItemId& itemIdParent, const wxString& strItemText );

	wxFileConfig*	OpenIniFile( const wxString& strFilePath, bool bOpenForWrite = false, const wxFontEncoding& encoding = wxFONTENCODING_UTF8 );
	int32_t			CloseIniFile( wxFileConfig* & pFileConfig );
	int32_t			SaveIniFile( wxFileConfig* pFileConfig, const wxString& strFilePath, const wxFontEncoding& encoding = wxFONTENCODING_UTF8 );
};

#endif //end ECXcx_TOOLS_H

