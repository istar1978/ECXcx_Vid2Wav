
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_LOG_H
#define ECXcx_LOG_H

#include "ECXcx_Types.h"
#include <set>

namespace ECXcx
{
	enum ET_LogLevel
	{
		E_LogLevel_Error	=	1,
		E_LogLevel_Warn		=	2,
		E_LogLevel_Info		=	3,
		E_LogLevel_Debug	=	4,

		E_LogLevel_Invalid
	};
};

class ECXcx_Logger
{
public:
	virtual ~ECXcx_Logger()	{}

	virtual int32_t	Log( const ECXcx::ET_LogLevel& eLogLevel, const char* pcLog, int32_t lenLog )	=	0;
};

class ECXcx_LogMgr
{
public:
	ECXcx_LogMgr();
	~ECXcx_LogMgr();

	int32_t	Init( const ECXcx::ET_LogLevel& eLogLevelMax );
	int32_t	Fini();

	int32_t	RegisterLogger( ECXcx_Logger* pLogger );
	int32_t	UnregisterLogger( ECXcx_Logger* pLogger );

	int32_t	Log( const ECXcx::ET_LogLevel& eLogLevel, const char* pcFormat, ... );

private:
	ECXcx::ET_LogLevel	m_eLogLevelMax;

	std::set< ECXcx_Logger* >	m_setLoggers;

public:
	static ECXcx_LogMgr*	Instance();
	static void				Destroy();

private:
	static ECXcx_LogMgr*	m_spInstance;
};

inline
ECXcx_LogMgr* ECXcx_LogMgr::Instance()
{
	if ( ECXcx_LogMgr::m_spInstance == NULL )
	{
		ECXcx_LogMgr::m_spInstance	=	new ECXcx_LogMgr();
	}

	return ECXcx_LogMgr::m_spInstance;
}

inline
void ECXcx_LogMgr::Destroy()
{
	if ( ECXcx_LogMgr::m_spInstance != NULL )
	{
		delete ECXcx_LogMgr::m_spInstance;
		ECXcx_LogMgr::m_spInstance	=	NULL;
	}
}

#endif //end ECXcx_LOG_H

