
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_TREECTRL_H
#define ECXcx_TREECTRL_H

#include "ECXcx_AppTypes.h"
#include "ECXcx_Synthesizer.h"
#include "ECXcx_Transcoder.h"

namespace ECXcx
{
	enum ET_NodeType
	{
		E_NodeType_File		=	0,
		E_NodeType_Section	=	1,
		E_NodeType_VidItem	=	2,
		E_NodeType_Other	=	3,

		E_NodeType_Invalid	=	99
	};
};

struct ECXcx_TreeItemData: public wxTreeItemData
{
	ECXcx::ET_NodeType	eNodeType;

	ECXcx_TreeItemData( const ECXcx::ET_NodeType& eNodeTypeParam )
	:eNodeType(eNodeTypeParam)
	{}

private:
	ECXcx_TreeItemData()
	:eNodeType(ECXcx::E_NodeType_Invalid)
	{}
};

struct ECXcx_TVidItem: public ECXcx_TreeItemData
{
	wxString	strSectionName;
	wxString	strKeyName;
	wxString	strVidName;
	wxString	strVoiceText;

	ECXcx_TVidItem()
	:ECXcx_TreeItemData(ECXcx::E_NodeType_VidItem)
	{}
	ECXcx_TVidItem( const ECXcx_TVidItem& rhs )
	:ECXcx_TreeItemData(ECXcx::E_NodeType_VidItem)
	,strSectionName(rhs.strSectionName), strKeyName(rhs.strKeyName), strVidName(rhs.strVidName), strVoiceText(rhs.strVoiceText)
	{}
	ECXcx_TVidItem( const wxString& strSectionNameParam, const wxString& strKeyNameParam, const wxString& strVidNameParam, const wxString& strVoiceTextParam )
	:ECXcx_TreeItemData(ECXcx::E_NodeType_VidItem)
	,strSectionName(strSectionNameParam), strKeyName(strKeyNameParam), strVidName(strVidNameParam), strVoiceText(strVoiceTextParam)
	{}

	void	Clear()
	{
		strSectionName.Clear();
		strKeyName.Clear();
		strVidName.Clear();
		strVoiceText.Clear();
	}
};

class ECXcx_TreeCtrl: public wxTreeCtrl
{
public:
	ECXcx_TreeCtrl();
	ECXcx_TreeCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
				long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT,
				const wxValidator& validator = wxDefaultValidator,
				const wxString& name = wxTreeCtrlNameStr);
	virtual ~ECXcx_TreeCtrl();

	wxTreeItemId	FindChild( const wxTreeItemId& itemIdParent, const wxString& strItemText );

	ECXcx::ET_NodeType	GetNodeType( const wxTreeItemId& itemId ) const;

private:
	int32_t	ParseVidItem( const wxString& strSectionName, const wxString& strKeyName, const wxString& strKeyValue, ECXcx_TVidItem& vidItem );

	int32_t	OnRespImportSection( const wxString& strSectionName );
	int32_t	OnRespImportVidItem( const ECXcx_TVidItem& vidItem );

	void	OnCMenuSynthesizeFile( const wxTreeItemId& itemId, const int32_t& nIdxVoiceEngine, const ECXcx_Synthesizer::ET_OutputFormat& eOutputFormat, const std::vector< wxString >& vecCmdsStereoToMono );
	void	OnCMenuSynthesizeSection( const wxTreeItemId& itemId, const int32_t& nIdxVoiceEngine, const ECXcx_Synthesizer::ET_OutputFormat& eOutputFormat, const std::vector< wxString >& vecCmdsStereoToMono );
	void	OnCMenuSynthesizeVidItem( const wxTreeItemId& itemId, const int32_t& nIdxVoiceEngine, const ECXcx_Synthesizer::ET_OutputFormat& eOutputFormat, const std::vector< wxString >& vecCmdsStereoToMono );

	void		LoadVidFile( const wxString& strFilePathVid );
	wxString	PrepareWavFileDir(
					const ECXcx_TVidItem&	vidItem,
					const int32_t&			nIdxVoiceEngine,
					const ECXcx_Synthesizer::ET_OutputFormat&	eOutputFormat
					);
	wxString	GetVoiceEngineDirName( const int32_t& nIdxVoiceEngine );
	wxString	GetOutputFormatDirName( const ECXcx_Synthesizer::ET_OutputFormat& eOutputFormat );

	wxDECLARE_EVENT_TABLE();
private:
	void	OnRightMouseDown( wxMouseEvent& evt );

	void	OnCMenuLoadVidFile( wxCommandEvent& evt );
	void	OnCMenuReloadVidFile( wxCommandEvent& evt );
	void	OnCMenuSynthesize( wxCommandEvent& evt );
};

#endif //end ECXcx_TREECTRL_H

