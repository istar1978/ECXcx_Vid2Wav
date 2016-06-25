
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_Synthesizer.h"
#include "ECXcx_Tools.h"

ECXcx_Synthesizer*	ECXcx_Synthesizer::m_spInstance	=	NULL;

ECXcx_Synthesizer::ECXcx_Synthesizer()
:m_nIdxCurVoiceEngine(-1)
,m_eCurOutputFormat(ECXcx_Synthesizer::E_OutputFormat_Invalid)
{
}

ECXcx_Synthesizer::~ECXcx_Synthesizer()
{
	this->Fini();
}

int32_t ECXcx_Synthesizer::Init( const wxString& wxstrConfFilePathVE )
{
	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	CoInitialize( NULL );

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		if ( ! SUCCEEDED(this->m_cpVoice.CoCreateInstance(CLSID_SpVoice)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOOUT, & this->m_cpOutAudio)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(this->m_cpVoice->SetInterest(SPFEI_ALL_TTS_EVENTS, SPFEI_ALL_TTS_EVENTS)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		nRet	=	this->InitVoiceEngines( wxstrConfFilePathVE );
		if ( nRet != ECX::E_Errno_SUCCESS )
		{
			break;
		}

		if ( ! this->m_vecVoiceEngines.empty() )
		{
			nRet	=	this->SetCurVoiceEngine(0);
			if ( nRet != ECX::E_Errno_SUCCESS )
			{
				break;
			}
		}

		nRet	=	this->SetCurOutputFormat( ECXcx_Synthesizer::E_OutputFormat_8K );
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

int32_t ECXcx_Synthesizer::Fini()
{
	this->FiniVoiceEngines();
	this->m_nIdxCurVoiceEngine	=	-1;
	this->m_eCurOutputFormat	=	ECXcx_Synthesizer::E_OutputFormat_Invalid;

	CoUninitialize();

	return ECX::E_Errno_SUCCESS;
}

int32_t ECXcx_Synthesizer::Synthesize(
				const wxString&	wxstrWavFilePath,
				const wxString&	wxstrVoiceText,
				const int32_t&	nIdxVoiceEngine,
				const ET_OutputFormat&	eOutputFormat
				)
{
	if (	wxstrWavFilePath.IsEmpty()
		||	wxstrVoiceText.IsEmpty()
		||	(nIdxVoiceEngine < 0) || (nIdxVoiceEngine >= this->m_vecVoiceEngines.size())
		||	(eOutputFormat < ECXcx_Synthesizer::E_OutputFormat_8K) || (eOutputFormat > ECXcx_Synthesizer::E_OutputFormat_48K)
		)
	{
		return ECX::E_Errno_ERR_GENERAL;
	}

	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	SPSTREAMFORMAT	eStreamFormat	=	SPSF_NoAssignedFormat;
	CComPtr< ISpStream >	cpWavStream;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		nRet	=	this->SetCurVoiceEngine( nIdxVoiceEngine );
		if ( nRet != ECX::E_Errno_SUCCESS )
		{
			break;
		}

		nRet	=	this->SetCurOutputFormat( eOutputFormat );
		if ( nRet != ECX::E_Errno_SUCCESS )
		{
			break;
		}

		CComPtr< ISpStreamFormat >	cpOldStream;
		CSpStreamFormat	originalFmt;
		if ( this->m_cpVoice->GetOutputStream(&cpOldStream) != S_OK )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}
		if ( ! SUCCEEDED(originalFmt.AssignFormat(cpOldStream)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(SPBindToFile(wxstrWavFilePath.wc_str(), SPFM_CREATE_ALWAYS, &cpWavStream, &originalFmt.FormatId(), originalFmt.WaveFormatExPtr())) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}
		if ( ! SUCCEEDED(this->m_cpVoice->SetOutput(cpWavStream, TRUE)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(this->m_cpVoice->Speak(wxstrVoiceText.wc_str(), SPF_ASYNC | SPF_IS_NOT_XML, 0)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(this->m_cpVoice->Resume()) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		this->m_cpVoice->WaitUntilDone( INFINITE );

		this->m_cpVoice->SetOutput( cpOldStream, FALSE );

		nRet	=	ECX::E_Errno_SUCCESS;
	}

	cpWavStream.Release();

	return nRet;
}

int32_t ECXcx_Synthesizer::InitVoiceEngines( const wxString& wxstrConfFilePathVE )
{
	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	wxFileConfig*	pFileConfig	=	NULL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		pFileConfig	=	ECXcx_Tools::OpenIniFile( wxstrConfFilePathVE );
		if ( pFileConfig == NULL )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		ECXcx_TVoiceEngine	voiceEngine;

		long		lIndexSection	=	0;
		wxString	strSectionName;

		if ( ! pFileConfig->GetFirstGroup(strSectionName, lIndexSection) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}
		do
		{
			pFileConfig->SetPath( wxString(wxT("/")) + strSectionName );
			pFileConfig->Read( wxT("name"), & voiceEngine.strName );
			pFileConfig->Read( wxT("lang"), & voiceEngine.strLang );
			pFileConfig->Read( wxT("desc"), & voiceEngine.strDesc );
			pFileConfig->Read( wxT("ldkw"), & voiceEngine.strLdkw );
			if ( !voiceEngine.strName.IsEmpty() && !voiceEngine.strLang.IsEmpty() && !voiceEngine.strDesc.IsEmpty() )
			{
				voiceEngine.pVoiceEngine	=	this->InitVoiceEngine( voiceEngine.strDesc );
				if ( voiceEngine.pVoiceEngine != NULL )
				{
					this->m_vecVoiceEngines.push_back( voiceEngine );
				}
			}

			voiceEngine.strName.Clear();
			voiceEngine.strLang.Clear();
			voiceEngine.strDesc.Clear();
			voiceEngine.strLdkw.Clear();
			voiceEngine.pVoiceEngine	=	NULL;

			strSectionName.Clear();
			pFileConfig->SetPath( wxT("/") );
		} while ( pFileConfig->GetNextGroup(strSectionName, lIndexSection) );

		nRet	=	ECX::E_Errno_SUCCESS;
	}

	if ( pFileConfig != NULL )
	{
		ECXcx_Tools::CloseIniFile( pFileConfig );
		pFileConfig	=	NULL;
	}

	return nRet;
}

int32_t ECXcx_Synthesizer::FiniVoiceEngines()
{
	ECXcx_TVecVoiceEngines::iterator	iter;
	for (
		iter = this->m_vecVoiceEngines.begin();
		iter != this->m_vecVoiceEngines.end();
		++ iter
		)
	{
		if ( iter->pVoiceEngine != NULL )
		{
			iter->pVoiceEngine->Release();
			iter->pVoiceEngine	=	NULL;
		}
	}

	this->m_vecVoiceEngines.clear();

	return ECX::E_Errno_SUCCESS;
}

ISpObjectToken* ECXcx_Synthesizer::InitVoiceEngine( const wxString& wxstrVoiceEngineDesc )
{
	ISpObjectToken*	pVoiceEngine	=	NULL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		CComPtr< IEnumSpObjectTokens >	cpEnum;
		if ( ! SUCCEEDED(SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum)) )
		{
			break;
		}

		ISpObjectToken*	piterVoiceEngine	=	NULL;
		while ( cpEnum->Next(1, &piterVoiceEngine, NULL) == S_OK )
		{
			CSpDynamicString	dstrDesc;
			if ( SUCCEEDED(SpGetDescription(piterVoiceEngine, &dstrDesc)) )
			{
				if ( wxstrVoiceEngineDesc == dstrDesc.m_psz )
				{
					pVoiceEngine	=	piterVoiceEngine;
					break;
				}
			}

			piterVoiceEngine->Release();
			piterVoiceEngine	=	NULL;
		}
	}

	return pVoiceEngine;
}

	/*
	 * SPSF_8kHz16BitStereo
	 * SPSF_16kHz16BitStereo
	 * SPSF_32kHz16BitStereo
	 * SPSF_48kHz16BitStereo
	 */
SPSTREAMFORMAT ECXcx_Synthesizer::GetStreamFormat( ET_OutputFormat eOutputFormat )
{
	SPSTREAMFORMAT	eStreamFormat	=	SPSF_NoAssignedFormat;

	switch ( eOutputFormat )
	{
	case E_OutputFormat_8K:
		{
			eStreamFormat	=	SPSF_8kHz16BitStereo;
			break;
		}
	case E_OutputFormat_16K:
		{
			eStreamFormat	=	SPSF_16kHz16BitStereo;
			break;
		}
	case E_OutputFormat_32K:
		{
			eStreamFormat	=	SPSF_32kHz16BitStereo;
			break;
		}
	case E_OutputFormat_48K:
		{
			eStreamFormat	=	SPSF_48kHz16BitStereo;
			break;
		}
	default:
		{
			eStreamFormat	=	SPSF_NoAssignedFormat;
			break;
		}
	}
	if ( eStreamFormat == SPSF_NoAssignedFormat )
	{
		return SPSF_NoAssignedFormat;
	}

	return eStreamFormat;
}

int32_t ECXcx_Synthesizer::SetCurVoiceEngine( int32_t nIdxVoiceEngine )
{
	if ( (nIdxVoiceEngine < 0) || (nIdxVoiceEngine >= this->m_vecVoiceEngines.size()) )
	{
		return ECX::E_Errno_ERR_GENERAL;
	}

	if ( nIdxVoiceEngine == this->m_nIdxCurVoiceEngine )
	{
		return ECX::E_Errno_SUCCESS;
	}

	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		CComPtr< ISpObjectToken >	pOldVoiceEngine;
		if ( ! SUCCEEDED(this->m_cpVoice->GetVoice(&pOldVoiceEngine)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(this->m_cpVoice->Speak(NULL, SPF_PURGEBEFORESPEAK, 0)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(this->m_cpVoice->SetVoice(this->m_vecVoiceEngines[nIdxVoiceEngine].pVoiceEngine)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		this->m_nIdxCurVoiceEngine	=	nIdxVoiceEngine;

		nRet	=	ECX::E_Errno_SUCCESS;
	}

	return nRet;
}

int32_t ECXcx_Synthesizer::SetCurOutputFormat( ECXcx_Synthesizer::ET_OutputFormat eOutputFormat )
{
	if ( eOutputFormat == ECXcx_Synthesizer::E_OutputFormat_Invalid )
	{
		return ECX::E_Errno_ERR_GENERAL;
	}

	if ( eOutputFormat == this->m_eCurOutputFormat )
	{
		return ECX::E_Errno_SUCCESS;
	}

	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		SPSTREAMFORMAT	eStreamFormat	=	this->GetStreamFormat( eOutputFormat );
		if ( eStreamFormat == SPSF_NoAssignedFormat )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		CSpStreamFormat	fmt;
		if ( ! SUCCEEDED(fmt.AssignFormat(eStreamFormat)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(this->m_cpOutAudio->SetFormat(fmt.FormatId(), fmt.WaveFormatExPtr())) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}

		if ( ! SUCCEEDED(this->m_cpVoice->SetOutput(this->m_cpOutAudio, FALSE)) )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}


		this->m_eCurOutputFormat	=	eOutputFormat;

		nRet	=	ECX::E_Errno_SUCCESS;
	}

	return nRet;
}

