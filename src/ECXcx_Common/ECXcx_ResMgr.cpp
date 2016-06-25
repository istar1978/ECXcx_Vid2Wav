
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_ResMgr.h"
#include "ECXcx_Synthesizer.h"

ECXcx_ResMgr*	ECXcx_ResMgr::m_spInstance	=	NULL;

ECXcx_ResMgr::ECXcx_ResMgr()
:m_pFileConfigApp(NULL)
{
}

ECXcx_ResMgr::~ECXcx_ResMgr()
{
	this->Fini();
}

int32_t ECXcx_ResMgr::Init()
{
	wxStandardPaths&	stdPaths	=	wxStandardPaths::Get();
	wxString	wxstrExecutablePath	=	stdPaths.GetExecutablePath();
	wxString	wxstrExecutableDir;
	size_t		posLastPathSep		=	wxstrExecutablePath.find_last_of(wxFileName::GetPathSeparator());
	if ( posLastPathSep != wxString::npos )
	{
		wxstrExecutableDir	=	wxstrExecutablePath.substr(0, posLastPathSep);
		wxSetWorkingDirectory( wxstrExecutableDir );
	}
	posLastPathSep	=	wxstrExecutableDir.find_last_of(wxFileName::GetPathSeparator());
	if ( posLastPathSep != wxString::npos )
	{
		this->m_wxstrAppDir	=	wxstrExecutableDir.substr(0, posLastPathSep);
	}

	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		nRet	=	ECXcx_LogMgr::Instance()->Init( ECXcx::E_LogLevel_Info );
		if ( nRet != ECX::E_Errno_SUCCESS )
		{
			break;
		}

		this->m_pFileConfigApp	=	ECXcx_Tools::OpenIniFile( this->m_wxstrAppDir + wxFileName::GetPathSeparator() + wxT("data") + wxFileName::GetPathSeparator() + wxT("ECXcx_Vid2Wav.ini") );
		if ( this->m_pFileConfigApp == NULL )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		nRet	=	ECXcx_Synthesizer::Instance()->Init( this->m_wxstrAppDir + wxFileName::GetPathSeparator() + wxT("data") + wxFileName::GetPathSeparator() + wxT("VoiceEngine.ini") );
		if ( nRet != ECX::E_Errno_SUCCESS )
		{
			break;
		}

		nRet	=	ECX::E_Errno_SUCCESS;
	}

	if ( nRet != ECX::E_Errno_SUCCESS )
	{
		this->Fini();
	}

	return nRet;
}

int32_t ECXcx_ResMgr::Fini()
{
	ECXcx_Synthesizer::Instance()->Fini();

	ECXcx_Tools::CloseIniFile( this->m_pFileConfigApp );

	ECXcx_LogMgr::Instance()->Fini();

	return ECX::E_Errno_SUCCESS;
}

wxString ECXcx_ResMgr::GetVersion() const
{
	wxString	wxstrVersion	=	wxT("1.0");
	if ( this->m_pFileConfigApp == NULL )
	{
		return wxstrVersion;
	}

	this->m_pFileConfigApp->Read( wxT("/AppInfo/Version"), & wxstrVersion );

	return wxstrVersion;
}

wxString ECXcx_ResMgr::GetReleaseDate() const
{
	wxString	wxstrReleaseDate	=	wxT("2016-05-19");
	if ( this->m_pFileConfigApp == NULL )
	{
		return wxstrReleaseDate;
	}

	this->m_pFileConfigApp->Read( wxT("/AppInfo/ReleaseDate"), & wxstrReleaseDate );

	return wxstrReleaseDate;
}

void ECXcx_ResMgr::GetExtCmds( std::vector< wxString >& vecCmdNames ) const
{
	vecCmdNames.clear();

	if ( this->m_pFileConfigApp == NULL )
	{
		return;
	}

	this->m_pFileConfigApp->SetPath( wxT("/ExtCmds") );

	long		lIndexEntry		=	0;
	wxString	strKeyName;
	wxString	strKeyValue;

	if ( this->m_pFileConfigApp->GetFirstEntry(strKeyName, lIndexEntry) )
	{
		do
		{
			if ( ! this->m_pFileConfigApp->Read(strKeyName, &strKeyValue) )
			{
				break;
			}

			vecCmdNames.push_back( strKeyName );

			strKeyName.Clear();
		} while ( this->m_pFileConfigApp->GetNextEntry(strKeyName, lIndexEntry) );
	}
}

int32_t ECXcx_ResMgr::GetExtCmdGroup( const wxString& wxstrCmdName, std::vector< wxString >& vecCmdNames ) const
{
	vecCmdNames.clear();

	if ( this->m_pFileConfigApp == NULL )
	{
		return ECX::E_Errno_ERR_GENERAL;
	}

	int	nIdx	=	1;
	wxString	wxstrKey;
	wxString	wxstrCmdValue;
	while ( true )
	{
		wxstrKey		=	wxString::Format( wxT("%s.%d"), wxstrCmdName, nIdx );
		wxstrCmdValue	=	this->GetExtCmdFormat( wxstrKey );
		if ( wxstrCmdValue.IsEmpty() )
		{
			break;
		}

		vecCmdNames.push_back( wxstrKey );
		++ nIdx;
		wxstrKey.Clear();
		wxstrCmdValue.Clear();
	}

	return ECX::E_Errno_SUCCESS;
}

wxString ECXcx_ResMgr::GetExtCmdFormat( const wxString& strCmdName ) const
{
	wxString	wxstrCmdValue;
	if ( this->m_pFileConfigApp == NULL )
	{
		return wxEmptyString;
	}

	wxString	wxstrKey	=	wxString(wxT("/ExtCmds/")) + strCmdName;
	if ( ! this->m_pFileConfigApp->Read(wxstrKey, & wxstrCmdValue) )
	{
		return wxEmptyString;
	}

	return wxstrCmdValue;
}

int32_t ECXcx_ResMgr::SetExtCmdFormat( const wxString& wxstrCmdName, const wxString& wxstrCmdValue )
{
	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		if ( this->m_pFileConfigApp == NULL )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		wxString	wxstrKey	=	wxString(wxT("/ExtCmds/")) + wxstrCmdName;
		if ( ! this->m_pFileConfigApp->Write(wxstrKey, wxstrCmdValue) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		nRet	=	ECXcx_Tools::SaveIniFile( this->m_pFileConfigApp, this->m_wxstrAppDir + wxFileName::GetPathSeparator() + wxT("data") + wxFileName::GetPathSeparator() + wxT("ECXcx_Vid2Wav.ini") );
		if ( nRet != ECX::E_Errno_SUCCESS )
		{
			break;
		}

		nRet	=	ECX::E_Errno_SUCCESS;
	}

	return nRet;
}

