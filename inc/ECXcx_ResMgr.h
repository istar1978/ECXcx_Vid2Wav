
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_RES_MGR_H
#define ECXcx_RES_MGR_H

#include "ECXcx_AppTypes.h"
#include "ECXcx_Tools.h"
#include "ECXcx_Log.h"

class ECXcx_ResMgr
{
public:
	ECXcx_ResMgr();
	~ECXcx_ResMgr();

	int32_t	Init();
	int32_t	Fini();

	wxString	GetVersion() const;
	wxString	GetReleaseDate() const;

	void		GetExtCmds( std::vector< wxString >& vecCmdNames ) const;
	int32_t		GetExtCmdGroup( const wxString& wxstrCmdName, std::vector< wxString >& vecCmdNames ) const;
	wxString	GetExtCmdFormat( const wxString& wxstrCmdName ) const;
	int32_t		SetExtCmdFormat( const wxString& wxstrCmdName, const wxString& wxstrCmdValue );

private:
	wxString		m_wxstrAppDir;
	wxFileConfig*	m_pFileConfigApp;

public:
	static ECXcx_ResMgr*	Instance();
	static void				Destroy();

private:
	static ECXcx_ResMgr*	m_spInstance;
};
#define mT(pstr_en_us) wxString::FromUTF8(pstr_en_us)

inline
ECXcx_ResMgr* ECXcx_ResMgr::Instance()
{
	if ( ECXcx_ResMgr::m_spInstance == NULL )
	{
		ECXcx_ResMgr::m_spInstance	=	new ECXcx_ResMgr;
	}

	return ECXcx_ResMgr::m_spInstance;
}

inline
void ECXcx_ResMgr::Destroy()
{
	if ( ECXcx_ResMgr::m_spInstance != NULL )
	{
		delete ECXcx_ResMgr::m_spInstance;
		ECXcx_ResMgr::m_spInstance	=	NULL;
	}
}

#endif //end ECXcx_RES_MGR_H

