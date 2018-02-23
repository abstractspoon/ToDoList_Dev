// KanbanEnum.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANBANENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
#define AFX_KANBANENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IUIExtension.h" // for IUI_POSITION

/////////////////////////////////////////////////////////////////////////////

// Pseudo Attributes

const int IUI_PARENT		= IUI_POSITION;
const int IUI_FIXEDCOLUMNS	= 0xffffffff;

/////////////////////////////////////////////////////////////////////////////

enum // options
{
	KBCF_SHOWPARENTTASKS				= 0x0001,
	KBCF_SHOWEMPTYCOLUMNS				= 0x0002,
	KBCF_SORTSUBTASTASKSBELOWPARENTS	= 0x0004,
	KBCF_TASKTEXTCOLORISBKGND			= 0x0008,
	KBCF_STRIKETHRUDONETASKS			= 0x0010,
	KBCF_ALWAYSSHOWBACKLOG				= 0x0020,
	KBCF_SHOWTASKCOLORASBAR				= 0x0040,
	KBCF_COLORBARBYPRIORITY				= 0x0080,
	KBCF_SHOWCOMPLETIONCHECKBOXES		= 0x0100,
	// 	KBCF_		= 0x0010,
	// 	KBCF_		= 0x0020,
	// 	KBCF_		= 0x0040,
	// 	KBCF_		= 0x0080,
};

/////////////////////////////////////////////////////////////////////////////

enum KBC_HITTEST
{
	KBCHT_NOWHERE = -1,
	KBCHT_BEGIN,
	KBCHT_MIDDLE,
	KBCHT_END,
};

/////////////////////////////////////////////////////////////////////////////

enum KBC_ATTRIBLABELS
{
	KBCAL_LONG,
	KBCAL_SHORT,
	KBCAL_NONE,
};

////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_KANBANENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
