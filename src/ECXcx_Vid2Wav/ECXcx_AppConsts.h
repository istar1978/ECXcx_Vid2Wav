
///*********************************************************************************
//*
//* ECXcx
//* Author: Zhu Fei
//* Email:  zhufei_13967158519@hotmail.com
//*
//*********************************************************************************/

#ifndef ECXcx_APP_CONSTS_H
#define ECXcx_APP_CONSTS_H

#include "ECXcx_Types.h"

namespace ECXcx
{
	enum ET_CtrlId
	{
		E_CtrlId_End	=	ECXcx::E_Consts_DFT_CtrlId_End,

		// { General
		E_CtrlId_General_End		=	E_CtrlId_End,
		// } end General

		// { PanelMain
		E_CtrlId_PanelMain_End		=	E_CtrlId_End - 32,
		// } end PanelMain

		// { PanelVidInfo
		E_CtrlId_PanelVidInfo_End	=	E_CtrlId_End - 64,
		E_CtrlId_PanelVidInfo_TreeCtrl	=	E_CtrlId_PanelVidInfo_End - 1,
		// } end PanelAppInfo
	};

	enum ET_CmdId
	{
		E_CmdId_Begin	=	ECXcx::E_Consts_DFT_CmdId_Begin,

		// { menuBar
		// File
		E_CmdId_Exit			=	wxID_EXIT,

		// Help
		E_CmdId_About			=	wxID_ABOUT,
		// } end menuBar

		E_CmdId_OnLogItemPrepared	=	E_CmdId_Begin + 64,

		E_CmdId_CMenu_LoadVidFile	=	E_CmdId_Begin + 128,
		E_CmdId_CMenu_ReloadVidFile	=	E_CmdId_Begin + 129,

		E_GroupCmdId_Begin_OnConfigExtCmds		=	E_CmdId_Begin + 1024,
		E_GroupCmdId_Begin_OnCMenuSynthesize	=	E_CmdId_Begin + 1088,
	};
};

#endif //end ECXcx_APP_CONSTS_H

