
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_Transcoder.h"

	// wxstrCmdFormat, for lame example:
	//				.1:	xxx "%s.wav" "%s.mp3"
	//				.2: xxx "%s.mp3" "%s.wav"
	// wxstrFilePath, without extension
	// and, 1st input file will be deleted
int32_t ECXcx_Transcoder::StereoToMono( const wxString& wxstrCmdFormat, const wxString& wxstrFilePath )
{
	wxString	wxstrCmd	=	wxString::Format( wxstrCmdFormat, wxstrFilePath, wxstrFilePath );
	::wxExecute( wxstrCmd, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE );

	size_t	posFirstBegin	=	wxstrCmdFormat.find( wxT("\"%s") );
	if ( posFirstBegin != wxString::npos )
	{
		size_t	posFirstEnd	=	wxstrCmdFormat.find( wxT("\""), posFirstBegin + 1 );
		if ( posFirstEnd != wxString::npos )
		{
			wxString	wxstrInFileFormat	=	wxstrCmdFormat.substr( posFirstBegin+1, posFirstEnd - posFirstBegin-1 );
			::wxRemoveFile( wxString::Format(wxstrInFileFormat, wxstrFilePath) );
		}
	}

	return ECX::E_Errno_SUCCESS;
}

