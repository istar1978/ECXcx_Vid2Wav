
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_TRANSCODER_H
#define ECXcx_TRANSCODER_H

#include "ECXcx_Types.h"

namespace ECXcx_Transcoder
{
	// wxstrCmdFormat, for lame example:
	//				.1:	xxx %s.wav %s.mp3
	//				.2: xxx %s.mp3 %s.wav
	// wxstrFilePath, without extension
	// and, 1st input file will be deleted
	int32_t	StereoToMono( const wxString& wxstrCmdFormat, const wxString& wxstrFilePath );
};

#endif //end ECXcx_TRANSCODER_H

