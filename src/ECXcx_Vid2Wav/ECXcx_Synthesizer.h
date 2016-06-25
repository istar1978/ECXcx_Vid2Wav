
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_SYNTHESIZER_H
#define ECXcx_SYNTHESIZER_H

#include "ECXcx_Types.h"

#include <sapi.h>
#include <sphelper.h>

struct ECXcx_TVoiceEngine
{
	ISpObjectToken*	pVoiceEngine;
	wxString	strName;
	wxString	strLang;
	wxString	strDesc;
	wxString	strLdkw;	// lang dir key word

	ECXcx_TVoiceEngine()
	:pVoiceEngine(NULL)
	{}
};
typedef std::vector< ECXcx_TVoiceEngine >	ECXcx_TVecVoiceEngines;

class ECXcx_Synthesizer
{
public:
	enum ET_OutputFormat
	{
		E_OutputFormat_8K	=	0,
		E_OutputFormat_16K	=	1,
		E_OutputFormat_32K	=	2,
		E_OutputFormat_48K	=	3,
		E_OutputFormat_Invalid	=	99,
	};

public:
	ECXcx_Synthesizer();
	~ECXcx_Synthesizer();

	int32_t	Init( const wxString& wxstrConfFilePathVE = wxT("../data/VoiceEngine.ini") );
	int32_t	Fini();

	const ECXcx_TVecVoiceEngines&	GetVoiceEngines()	{ return this->m_vecVoiceEngines;}

	int32_t	Synthesize(
				const wxString&	wxstrWavFilePath,
				const wxString&	wxstrVoiceText,
				const int32_t&	nIdxVoiceEngine,
				const ET_OutputFormat&	eOutputFormat	=	E_OutputFormat_8K
				);

private:
	int32_t	InitVoiceEngines( const wxString& wxstrConfFilePathVE = wxT("../data/VoiceEngine.ini") );
	int32_t	FiniVoiceEngines();

	ISpObjectToken*	InitVoiceEngine( const wxString& wxstrVoiceEngineDesc );
	SPSTREAMFORMAT	GetStreamFormat( ET_OutputFormat eOutputFormat );

	int32_t	SetCurVoiceEngine( int32_t nIdxVoiceEngine );
	int32_t	SetCurOutputFormat( ET_OutputFormat eOutputFormat );

private:
	CComPtr< ISpVoice >	m_cpVoice;
	CComPtr< ISpAudio >	m_cpOutAudio;

	ECXcx_TVecVoiceEngines	m_vecVoiceEngines;

	int32_t			m_nIdxCurVoiceEngine;
	ET_OutputFormat	m_eCurOutputFormat;

public:
	static ECXcx_Synthesizer*	Instance();
	static void					Destroy();

private:
	static ECXcx_Synthesizer*	m_spInstance;
};

inline
ECXcx_Synthesizer* ECXcx_Synthesizer::Instance()
{
	if ( ECXcx_Synthesizer::m_spInstance == NULL )
	{
		ECXcx_Synthesizer::m_spInstance	=	new ECXcx_Synthesizer();
	}

	return ECXcx_Synthesizer::m_spInstance;
}

inline
void ECXcx_Synthesizer::Destroy()
{
	if ( ECXcx_Synthesizer::m_spInstance != NULL )
	{
		delete ECXcx_Synthesizer::m_spInstance;
		ECXcx_Synthesizer::m_spInstance	=	NULL;
	}
}

#endif //end ECXcx_SYNTHESIZER_H

