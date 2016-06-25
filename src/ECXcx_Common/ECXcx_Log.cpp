
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#include "ECXcx_Log.h"
#include <time.h>

ECXcx_LogMgr*	ECXcx_LogMgr::m_spInstance	=	NULL;

ECXcx_LogMgr::ECXcx_LogMgr()
:m_eLogLevelMax(ECXcx::E_LogLevel_Info)
{
}

ECXcx_LogMgr::~ECXcx_LogMgr()
{
	this->Fini();
}

int32_t ECXcx_LogMgr::Init( const ECXcx::ET_LogLevel& eLogLevelMax )
{
	this->m_eLogLevelMax	=	eLogLevelMax;

	return ECX::E_Errno_SUCCESS;
}

int32_t ECXcx_LogMgr::Fini()
{
	this->m_setLoggers.clear();

	this->m_eLogLevelMax	=	ECXcx::E_LogLevel_Info;

	return ECX::E_Errno_SUCCESS;
}

int32_t ECXcx_LogMgr::RegisterLogger( ECXcx_Logger* pLogger )
{
	if ( pLogger == NULL )
	{
		return ECX::E_Errno_ERR_GENERAL;
	}

	this->m_setLoggers.insert( pLogger );

	return ECX::E_Errno_SUCCESS;
}

int32_t ECXcx_LogMgr::UnregisterLogger( ECXcx_Logger* pLogger )
{
	if ( pLogger == NULL )
	{
		return ECX::E_Errno_ERR_GENERAL;
	}

	this->m_setLoggers.erase( pLogger );

	return ECX::E_Errno_SUCCESS;
}

int32_t ECXcx_LogMgr::Log( const ECXcx::ET_LogLevel& eLogLevel, const char* pcFormat, ... )
{
	if ( eLogLevel > this->m_eLogLevelMax )
	{
		return ECX::E_Errno_SUCCESS;
	}

	time_t		tNow	=	time(NULL);
	struct tm*	tmNow	=	localtime(&tNow);
	char		bufDateTime[32];
	memset( bufDateTime, 0, sizeof(bufDateTime) );
	int32_t		lenDateTime	=	strftime( bufDateTime, sizeof(bufDateTime), "%Y-%m-%d %H-%M-%S", tmNow);

	int32_t	nRet	=	ECX::E_Errno_ERR_GENERAL;

	char*	pcLog	=	NULL;
	int32_t	lenLog	=	0;

	for ( int32_t iOnce=0; iOnce<1; ++iOnce )
	{
		va_list	args;
		va_start( args, pcFormat );
		lenLog	=	vsnprintf( NULL, 0, pcFormat, args );
		va_end( args );

		pcLog	=	new char[lenDateTime+1 + lenLog+1];
		if ( pcLog == NULL )
		{
			nRet	=	ECX::E_Errno_ERR_GENERAL;
			break;
		}
		memset( pcLog, 0, lenDateTime+1 + lenLog+1 );
		memcpy( pcLog, bufDateTime, lenDateTime );
		pcLog[lenDateTime]	=	' ';
		pcLog[lenDateTime+1]=	'\0';

		va_start( args, pcFormat );
		lenLog	=	vsnprintf( pcLog + lenDateTime+1, lenLog+1, pcFormat, args );
		va_end( args );

		lenLog	=	lenDateTime+1 + lenLog;

		std::set< ECXcx_Logger* >::iterator	iter;
		for (
			iter = this->m_setLoggers.begin();
			iter != this->m_setLoggers.end();
			++ iter
			)
		{
			(*iter)->Log( eLogLevel, pcLog, lenLog );
		}

		nRet	=	ECX::E_Errno_SUCCESS;
	}

	if ( pcLog != NULL )
	{
		delete [] pcLog;
		pcLog	=	NULL;
	}
	lenLog	=	0;

	return nRet;
}

