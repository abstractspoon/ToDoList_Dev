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

#define TDCA_PARENT			TDCA_POSITION
#define TDCA_FIXEDCOLUMNS	((TDC_ATTRIBUTE)-2)

/////////////////////////////////////////////////////////////////////////////

enum // options
{
	KBCF_HIDEPARENTTASKS					= 0x00000001,
	KBCF_HIDEEMPTYCOLUMNS					= 0x00000002,
	KBCF_SORTSUBTASTASKSBELOWPARENTS		= 0x00000004,
	KBCF_TASKTEXTCOLORISBKGND				= 0x00000008,
	KBCF_STRIKETHRUDONETASKS				= 0x00000010,
	KBCF_ALWAYSSHOWBACKLOG					= 0x00000020,
	KBCF_SHOWTASKCOLORASBAR					= 0x00000040,
	KBCF_COLORBARBYPRIORITY					= 0x00000080,
	KBCF_SHOWCOMPLETIONCHECKBOXES			= 0x00000100,
	KBCF_INDENTSUBTASKS						= 0x00000200,
	KBCF_HIDEEMPTYATTRIBUTES				= 0x00000400,
	KBCF_SHOWLABELTIPS						= 0x00000800,
	KBCF_DUEHAVEHIGHESTPRIORITYRISK			= 0x00001000,
	KBCF_DONEHAVELOWESTPRIORITYRISK			= 0x00002000,
	KBCF_PARENTSSHOWHIGHESTPRIORITYRISK		= 0x00004000,
	KBCF_HIDESUBTASKS						= 0x00008000,
	KBCF_COLUMNHEADERSORTING                = 0x00010000,
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

enum KBC_IMAGETYPE
{
	KBCI_NONE = -1,
	KBCI_ICON,
	KBCI_FLAG,
	KBCI_PIN
};


////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_KANBANENUM_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
