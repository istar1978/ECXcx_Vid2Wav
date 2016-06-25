
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_TreeCtrl.h"
#include "ECXcx_ResMgr.h"

#include "wx/wfstream.h"
#include "wx/datetime.h"

ECXcx_TreeCtrl::ECXcx_TreeCtrl()
:wxTreeCtrl()
{
}

ECXcx_TreeCtrl::ECXcx_TreeCtrl(wxWindow *parent, wxWindowID id,
				const wxPoint& pos,
				const wxSize& size,
				long style,
				const wxValidator& validator,
				const wxString& name)
:wxTreeCtrl(parent, id, pos, size, style, validator, name)
{
	const ECXcx_TVecVoiceEngines&	vecVoiceEngines	=
		ECXcx_Synthesizer::Instance()->GetVoiceEngines();

	if ( ! vecVoiceEngines.empty() )
	{
		wxEvtHandler::Connect(ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize, ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize + 5*vecVoiceEngines.size(), wxEVT_MENU, wxCommandEventHandler(ECXcx_TreeCtrl::OnCMenuSynthesize));
	}
}

ECXcx_TreeCtrl::~ECXcx_TreeCtrl()
{
	const ECXcx_TVecVoiceEngines&	vecVoiceEngines	=
		ECXcx_Synthesizer::Instance()->GetVoiceEngines();

	if ( ! vecVoiceEngines.empty() )
	{
		wxEvtHandler::Disconnect(ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize, ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize + 5*vecVoiceEngines.size(), wxEVT_MENU);
	}
}

wxTreeItemId ECXcx_TreeCtrl::FindChild( const wxTreeItemId& itemIdParent, const wxString& strItemText )
{
	return ECXcx_Tools::FindChild( *this, itemIdParent, strItemText );
}

ECXcx::ET_NodeType ECXcx_TreeCtrl::GetNodeType( const wxTreeItemId& itemId ) const
{
	if ( ! itemId.IsOk() )
	{
		return ECXcx::E_NodeType_Invalid;
	}

	ECXcx_TreeItemData*	pTreeItemData	=	(ECXcx_TreeItemData*)( this->GetItemData(itemId) );
	if ( pTreeItemData == NULL )
	{
		return ECXcx::E_NodeType_Invalid;
	}

	return pTreeItemData->eNodeType;
}

int32_t ECXcx_TreeCtrl::ParseVidItem( const wxString& strSectionName, const wxString& strKeyName, const wxString& strKeyValue, ECXcx_TVidItem& vidItem )
{
	static char	cSepVoiceText	=	0x7F;

	// strEntry: KeyName=VidName;//VoiceText^

	wxString::size_type	posVidEnd	=	strKeyValue.find(wxT(";//"));
	if ( posVidEnd == wxString::npos )
	{
		ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "no \";//\" found in entry[%s/%s], value[%s]!!!\n", strSectionName.ToUTF8().data(), strKeyName.ToUTF8().data(), strKeyValue.ToUTF8().data() );
		return ECX::E_Errno_ERR_GENERAL;
	}
	wxString::size_type	posVoiceTextEnd	=	strKeyValue.find(cSepVoiceText, posVidEnd+3);
	if ( posVoiceTextEnd == wxString::npos )
	{
		ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "no VoiceText Separator [0x7F] found in entry[%s/%s], value[%s]!!!\n", strSectionName.ToUTF8().data(), strKeyName.ToUTF8().data(), strKeyValue.ToUTF8().data() );
		return ECX::E_Errno_ERR_GENERAL;
	}

	vidItem.strSectionName	=	strSectionName;
	vidItem.strKeyName		=	strKeyName;
	vidItem.strVidName		=	strKeyValue.substr(0, posVidEnd);
	vidItem.strVoiceText	=	strKeyValue.substr(posVidEnd+3, posVoiceTextEnd-posVidEnd-3);

	ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Debug, "Successfully parsed VidItem[%s/%s/%s].\n", vidItem.strSectionName.ToUTF8().data(), vidItem.strVidName.ToUTF8().data(), vidItem.strVoiceText.ToUTF8().data() );

	return ECX::E_Errno_SUCCESS;
}

int32_t ECXcx_TreeCtrl::OnRespImportSection( const wxString& strSectionName )
{
	this->AppendItem( this->GetRootItem(), strSectionName, -1, -1, new ECXcx_TreeItemData(ECXcx::E_NodeType_Section) );

	return ECX::E_Errno_SUCCESS;
}

int32_t ECXcx_TreeCtrl::OnRespImportVidItem( const ECXcx_TVidItem& vidItem )
{
	wxTreeItemId	itemIdSection	=
		this->FindChild( this->GetRootItem(), vidItem.strSectionName );
	if ( ! itemIdSection.IsOk() )
	{
		ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "Section[%s] not found!!!\n", vidItem.strSectionName.ToUTF8().data() );
		return ECX::E_Errno_ERR_GENERAL;
	}

	wxTreeItemId	itemIdVidItem	=
		this->AppendItem( itemIdSection, vidItem.strVidName, -1, -1, new ECXcx_TVidItem(vidItem) );

	this->AppendItem( itemIdVidItem, vidItem.strVoiceText, -1, -1, new ECXcx_TreeItemData(ECXcx::E_NodeType_Other) );

	return ECX::E_Errno_SUCCESS;
}

void ECXcx_TreeCtrl::OnCMenuSynthesizeFile( const wxTreeItemId& itemId, const int32_t& nIdxVoiceEngine, const ECXcx_Synthesizer::ET_OutputFormat& eOutputFormat, const std::vector< wxString >& vecCmdsStereoToMono )
{
	wxTreeItemIdValue	cookie;
	wxTreeItemId		iterItemId	=	this->GetFirstChild( itemId, cookie );
	while ( iterItemId.IsOk() )
	{
		if ( this->GetNodeType(iterItemId) == ECXcx::E_NodeType_Section )
		{
			this->OnCMenuSynthesizeSection( iterItemId, nIdxVoiceEngine, eOutputFormat, vecCmdsStereoToMono );
		}

		iterItemId	=	this->GetNextSibling( iterItemId );
	}

	ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Info, "OnCMenuSynthesizeFile finished.\n" );
}

void ECXcx_TreeCtrl::OnCMenuSynthesizeSection( const wxTreeItemId& itemId, const int32_t& nIdxVoiceEngine, const ECXcx_Synthesizer::ET_OutputFormat& eOutputFormat, const std::vector< wxString >& vecCmdsStereoToMono )
{
	wxTreeItemIdValue	cookie;
	wxTreeItemId		iterItemId	=	this->GetFirstChild( itemId, cookie );
	while ( iterItemId.IsOk() )
	{
		if ( this->GetNodeType(iterItemId) == ECXcx::E_NodeType_VidItem )
		{
			this->OnCMenuSynthesizeVidItem( iterItemId, nIdxVoiceEngine, eOutputFormat, vecCmdsStereoToMono );
		}

		iterItemId	=	this->GetNextSibling( iterItemId );
	}

	ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Info, "OnCMenuSynthesizeSection finished.\n" );
}

void ECXcx_TreeCtrl::OnCMenuSynthesizeVidItem( const wxTreeItemId& itemId, const int32_t& nIdxVoiceEngine, const ECXcx_Synthesizer::ET_OutputFormat& eOutputFormat, const std::vector< wxString >& vecCmdsStereoToMono )
{
	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		if ( this->GetNodeType(itemId) != ECXcx::E_NodeType_VidItem )
		{
			break;
		}
		ECXcx_TVidItem*	pVidItem	=
			(ECXcx_TVidItem*)( this->GetItemData(itemId) );
		if ( pVidItem == NULL )
		{
			break;
		}

		wxString	strWavFileDir	=
			this->PrepareWavFileDir( *pVidItem, nIdxVoiceEngine, eOutputFormat );
		if ( strWavFileDir.empty() )
		{
			ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "PrepareWavFileDir[%s] failed!!!\n", strWavFileDir.ToUTF8().data() );
			break;
		}

		wxString	strWavFilePath	=
			strWavFileDir + wxFileName::GetPathSeparator() + pVidItem->strVidName;

		if ( ECXcx_Synthesizer::Instance()->Synthesize(strWavFilePath + wxT(".wav"), pVidItem->strVoiceText, nIdxVoiceEngine, eOutputFormat) != ECX::E_Errno_SUCCESS )
		{
			ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "OnCMenuSynthesizeVidItem[%s] failed!!!\n", strWavFilePath.ToUTF8().data() );
			break;
		}

		std::vector< wxString >::const_iterator	iter;
		for ( iter = vecCmdsStereoToMono.begin(); iter != vecCmdsStereoToMono.end(); ++ iter )
		{
			wxString	strCmdFormat	=	ECXcx_ResMgr::Instance()->GetExtCmdFormat( *iter );
			if ( strCmdFormat.IsEmpty() )
			{
				break;
			}

			ECXcx_Transcoder::StereoToMono( strCmdFormat, strWavFilePath );
		}

		ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Info, "OnCMenuSynthesizeVidItem[%s] finished.\n", strWavFilePath.ToUTF8().data() );
	}
}

void ECXcx_TreeCtrl::LoadVidFile( const wxString& strFilePathVid )
{
	this->DeleteAllItems();
	this->AddRoot( strFilePathVid, -1, -1, new ECXcx_TreeItemData(ECXcx::E_NodeType_File) );

	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	wxFileConfig*	pFileConfig	=	NULL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		pFileConfig	=	ECXcx_Tools::OpenIniFile( strFilePathVid );
		if ( pFileConfig == NULL )
		{
			ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "open vid file[%s] failed!!!\n", strFilePathVid.ToUTF8().data() );
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		ECXcx_TVidItem	vidItem;

		long		lIndexSection	=	0;
		wxString	strSectionName;

		if ( ! pFileConfig->GetFirstGroup(strSectionName, lIndexSection) )
		{
			ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "no section found in vid file[%s] failed!!!\n", strFilePathVid.ToUTF8().data() );
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		nRet	=	ECX::E_Errno_SUCCESS;
		do
		{
			pFileConfig->SetPath( wxString(wxT("/") + strSectionName) );
			this->OnRespImportSection( strSectionName );

			long		lIndexEntry		=	0;
			wxString	strKeyName;
			wxString	strKeyValue;

			if ( pFileConfig->GetFirstEntry(strKeyName, lIndexEntry) )
			{
				do
				{
					if ( ! pFileConfig->Read(strKeyName, &strKeyValue) )
					{
						ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "read key value[%s/%s] failed!!!\n", strSectionName.ToUTF8().data(), strKeyName.ToUTF8().data() );
						break;
					}

					if ( this->ParseVidItem(strSectionName, strKeyName, strKeyValue, vidItem) != ECX::E_Errno_SUCCESS )
					{
						ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "parse entry[%s/%s] failed!!!\n", strSectionName.ToUTF8().data(), strKeyName.ToUTF8().data() );
						break;
					}

					this->OnRespImportVidItem( vidItem );

					strKeyName.Clear();
					strKeyValue.Clear();
				} while ( pFileConfig->GetNextEntry(strKeyName, lIndexEntry) );
				if ( nRet != ECX::E_Errno_SUCCESS )
				{
					break;
				}
			}

			strSectionName.Clear();
			pFileConfig->SetPath( wxT("/") );
		} while ( pFileConfig->GetNextGroup(strSectionName, lIndexSection) );

		this->Expand( this->GetRootItem() );

		ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Info, "load vid file[%s] successfully.\n", strFilePathVid.ToUTF8().data() );
		nRet	=	ECX::E_Errno_SUCCESS;
	}
	if ( nRet != ECX::E_Errno_SUCCESS )
	{
		ECXcx_LogMgr::Instance()->Log( ECXcx::E_LogLevel_Error, "load vid file[%s] failed!!!\n", strFilePathVid.ToUTF8().data() );
	}

	ECXcx_Tools::CloseIniFile( pFileConfig );
}

wxString ECXcx_TreeCtrl::PrepareWavFileDir(
					const ECXcx_TVidItem&	vidItem,
					const int32_t&			nIdxVoiceEngine,
					const ECXcx_Synthesizer::ET_OutputFormat&	eOutputFormat
					)
{
	wxString	strWavFileDir;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		wxString	strVidFilePath	=	this->GetItemText( this->GetRootItem() );

		wxString	strVoiceEngineDirName	=	this->GetVoiceEngineDirName( nIdxVoiceEngine );
		wxString	strOutputFormatDirName	=	this->GetOutputFormatDirName( eOutputFormat );

		if ( strVidFilePath.empty() || strVoiceEngineDirName.empty() || strOutputFormatDirName.empty() )
		{
			break;
		}

		wxFileName	fnVidFileDir( strVidFilePath );
		strWavFileDir	=
				fnVidFileDir.GetPath() + wxFileName::GetPathSeparator()
			+	strVoiceEngineDirName + wxFileName::GetPathSeparator()
			+	vidItem.strSectionName + wxFileName::GetPathSeparator()
			+	strOutputFormatDirName;
		if ( ! ::wxDirExists(strWavFileDir) )
		{
			//if ( ! ::wxMkdir(strWavFileDir) )
			if ( ! wxFileName::Mkdir(strWavFileDir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL) )
			{
				strWavFileDir.Clear();
				break;
			}
		}
	}

	return strWavFileDir;
}

wxString ECXcx_TreeCtrl::GetVoiceEngineDirName( const int32_t& nIdxVoiceEngine )
{
	const ECXcx_TVecVoiceEngines&	vecVoiceEngines	=
		ECXcx_Synthesizer::Instance()->GetVoiceEngines();

	if ( (nIdxVoiceEngine < 0) || (nIdxVoiceEngine >= vecVoiceEngines.size()) )
	{
		return wxEmptyString;
	}

	return vecVoiceEngines[nIdxVoiceEngine].strName;
}

wxString ECXcx_TreeCtrl::GetOutputFormatDirName( const ECXcx_Synthesizer::ET_OutputFormat& eOutputFormat )
{
	wxString	strOutputFormatDirName;
	switch ( eOutputFormat )
	{
	case ECXcx_Synthesizer::E_OutputFormat_8K:
		{
			strOutputFormatDirName	=	wxT("8000");
			break;
		}
	case ECXcx_Synthesizer::E_OutputFormat_16K:
		{
			strOutputFormatDirName	=	wxT("16000");
			break;
		}
	case ECXcx_Synthesizer::E_OutputFormat_32K:
		{
			strOutputFormatDirName	=	wxT("32000");
			break;
		}
	case ECXcx_Synthesizer::E_OutputFormat_48K:
		{
			strOutputFormatDirName	=	wxT("48000");
			break;
		}
	default:
		{
			break;
		}
	}

	return strOutputFormatDirName;
}


wxBEGIN_EVENT_TABLE(ECXcx_TreeCtrl, wxTreeCtrl)
	EVT_RIGHT_DOWN(ECXcx_TreeCtrl::OnRightMouseDown)

	EVT_MENU(ECXcx::E_CmdId_CMenu_LoadVidFile, ECXcx_TreeCtrl::OnCMenuLoadVidFile)
	EVT_MENU(ECXcx::E_CmdId_CMenu_ReloadVidFile, ECXcx_TreeCtrl::OnCMenuReloadVidFile)
wxEND_EVENT_TABLE()

void ECXcx_TreeCtrl::OnRightMouseDown( wxMouseEvent& evt )
{
	wxTreeItemId	itemId	=	this->HitTest( evt.GetPosition() );
	if ( itemId.IsOk() )
	{
		this->SelectItem( itemId );
	}
	else
	{
		this->UnselectAll();
	}

	ECXcx::ET_NodeType	eNodeType	=	this->GetNodeType( itemId );

	wxMenu	contextMenu;

	switch ( eNodeType )
	{
	case ECXcx::E_NodeType_File:
		{
			contextMenu.Append( ECXcx::E_CmdId_CMenu_ReloadVidFile, mT("Reload VID File") );

			contextMenu.AppendSeparator();
		}
	case ECXcx::E_NodeType_Section:
	case ECXcx::E_NodeType_VidItem:
		{
			wxString	strFilePathVid	=	this->GetItemText( this->GetRootItem() );

			const ECXcx_TVecVoiceEngines&	vecVoiceEngines	=
				ECXcx_Synthesizer::Instance()->GetVoiceEngines();
			if ( vecVoiceEngines.empty() )
			{
				break;
			}

			wxMenu*	pMenuSynthesize	=	new wxMenu;

			wxString	strCurMenuLang;
			wxMenu*		pCurMenuLang	=	NULL;

			for ( size_t i=0; i<vecVoiceEngines.size(); ++i )
			{
				if ( strFilePathVid.find(vecVoiceEngines[i].strLdkw) == wxString::npos )
				{
					continue;
				}

				if ( strCurMenuLang != vecVoiceEngines[i].strLang )
				{
					pCurMenuLang	=	new wxMenu;
				}

				wxMenu*	pMenuName	=	new wxMenu;
				pMenuName->Append( ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize + 5*i, mT("All") );
				pMenuName->Append( ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize + 5*i+1, mT("8kHz 16 Bit Stereo") );
				pMenuName->Append( ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize + 5*i+2, mT("16kHz 16 Bit Stereo") );
				pMenuName->Append( ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize + 5*i+3, mT("32kHz 16 Bit Stereo") );
				pMenuName->Append( ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize + 5*i+4, mT("48kHz 16 Bit Stereo") );

				pCurMenuLang->AppendSubMenu( pMenuName, vecVoiceEngines[i].strName );

				if ( strCurMenuLang != vecVoiceEngines[i].strLang )
				{
					strCurMenuLang	=	vecVoiceEngines[i].strLang;
					pMenuSynthesize->AppendSubMenu( pCurMenuLang, strCurMenuLang );
				}
			}

			contextMenu.AppendSubMenu( pMenuSynthesize, mT("Synthesize") );

			break;
		}
	case ECXcx::E_NodeType_Other:
		{
			break;
		}
	default:
		{
			contextMenu.Append( ECXcx::E_CmdId_CMenu_LoadVidFile, mT("Load VID File") );
			break;
		}
	}

	this->PopupMenu( &contextMenu );
}

void ECXcx_TreeCtrl::OnCMenuLoadVidFile( wxCommandEvent& evt )
{
	wxString	strFilePathVid	=
		wxFileSelector( wxT("Please select vid file:"), wxT("../data"), wxEmptyString, wxT("vid"), wxT("VID files (*.vid)|*.vid") );
	if ( strFilePathVid.empty() )
	{
		return;
	}

	this->LoadVidFile( strFilePathVid );
}

void ECXcx_TreeCtrl::OnCMenuReloadVidFile( wxCommandEvent& evt )
{
	wxTreeItemId	itemId	=	this->GetRootItem();
	if ( itemId.IsOk() )
	{
		this->LoadVidFile( this->GetItemText(itemId) );
	}
}

void ECXcx_TreeCtrl::OnCMenuSynthesize( wxCommandEvent& evt )
{
	int32_t	nId	=	evt.GetId() - ECXcx::E_GroupCmdId_Begin_OnCMenuSynthesize;

	int32_t	nIdxVoiceEngine	=	nId / 5;
	int32_t	nIdxOutputFormat=	nId % 5;

	std::vector< wxString >	vecCmdGroupStereo2Mono;
	ECXcx_ResMgr::Instance()->GetExtCmdGroup( wxT("Transcoder.Stereo2Mono"), vecCmdGroupStereo2Mono );

	wxTreeItemId	itemId	=	this->GetSelection();
	switch ( this->GetNodeType(itemId) )
	{
	case ECXcx::E_NodeType_File:
		{
			if ( nIdxOutputFormat == 0 )
			{
				this->OnCMenuSynthesizeFile( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_8K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeFile( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_16K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeFile( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_32K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeFile( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_48K, vecCmdGroupStereo2Mono );
			}
			else
			{
				this->OnCMenuSynthesizeFile( itemId, nIdxVoiceEngine, (ECXcx_Synthesizer::ET_OutputFormat)(nIdxOutputFormat - 1), vecCmdGroupStereo2Mono );
			}
			break;
		}
	case ECXcx::E_NodeType_Section:
		{
			if ( nIdxOutputFormat == 0 )
			{
				this->OnCMenuSynthesizeSection( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_8K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeSection( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_16K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeSection( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_32K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeSection( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_48K, vecCmdGroupStereo2Mono );
			}
			else
			{
				this->OnCMenuSynthesizeSection( itemId, nIdxVoiceEngine, (ECXcx_Synthesizer::ET_OutputFormat)(nIdxOutputFormat - 1), vecCmdGroupStereo2Mono );
			}
			break;
		}
	case ECXcx::E_NodeType_VidItem:
		{
			if ( nIdxOutputFormat == 0 )
			{
				this->OnCMenuSynthesizeVidItem( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_8K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeVidItem( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_16K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeVidItem( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_32K, vecCmdGroupStereo2Mono );
				this->OnCMenuSynthesizeVidItem( itemId, nIdxVoiceEngine, ECXcx_Synthesizer::E_OutputFormat_48K, vecCmdGroupStereo2Mono );
			}
			else
			{
				this->OnCMenuSynthesizeVidItem( itemId, nIdxVoiceEngine, (ECXcx_Synthesizer::ET_OutputFormat)(nIdxOutputFormat - 1), vecCmdGroupStereo2Mono );
			}
			break;
		}
	default:
		{
			break;
		}
	}
}

