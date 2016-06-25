
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_Tools.h"

#include "wx/wfstream.h"

wxTreeItemId ECXcx_Tools::FindChild( const wxTreeCtrl& treeCtrl, const wxTreeItemId& itemIdParent, const wxString& strItemText )
{
	wxTreeItemIdValue	cookie;
	wxTreeItemId		iterItemId	=	treeCtrl.GetFirstChild( itemIdParent, cookie );
	while ( iterItemId.IsOk() )
	{
		if ( treeCtrl.GetItemText(iterItemId) == strItemText )
		{
			break;
		}

		iterItemId	=	treeCtrl.GetNextSibling( iterItemId );
	}

	return iterItemId;
}

wxFileConfig* ECXcx_Tools::OpenIniFile( const wxString& strFilePath, bool bOpenForWrite, const wxFontEncoding& encoding )
{
	wxFileConfig*	pFileConfig	=	NULL;

	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		pFileConfig	=	new wxFileConfig( wxEmptyString, wxEmptyString, strFilePath, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto(encoding) );
		if ( pFileConfig == NULL )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( bOpenForWrite )
		{
			pFileConfig->DeleteAll();
		}

		nRet	=	ECX::E_Errno_SUCCESS;
	}
	if ( nRet != ECX::E_Errno_SUCCESS )
	{
		if ( pFileConfig != NULL )
		{
			delete pFileConfig;
			pFileConfig	=	NULL;
		}
	}

	return pFileConfig;
}

int32_t ECXcx_Tools::CloseIniFile( wxFileConfig* & pFileConfig )
{
	if ( pFileConfig == NULL )
	{
		return ECX::E_Errno_SUCCESS;
	}

	delete pFileConfig;
	pFileConfig	=	NULL;

	return ECX::E_Errno_SUCCESS;
}

int32_t ECXcx_Tools::SaveIniFile( wxFileConfig* pFileConfig, const wxString& strFilePath, const wxFontEncoding& encoding )
{
	if ( (pFileConfig == NULL) || strFilePath.IsEmpty() )
	{
		return ECX::E_Errno_ERR_GENERAL;
	}

	wxFileOutputStream	fos( strFilePath );

	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		if ( ! pFileConfig->Save(fos, wxCSConv(encoding)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		nRet	=	ECX::E_Errno_SUCCESS;
	}

	return nRet;
}

