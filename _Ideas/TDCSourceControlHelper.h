// TDCSourceControlHelper.h: interface for the CTDCCustomAttributeHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
#define AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Shared\mapex.h"

//////////////////////////////////////////////////////////////////////

class CToDoCtrl;
class CToDoCtrlData;

struct TODOITEM;

//////////////////////////////////////////////////////////////////////

class CTDCSourceControlHelper  
{
public:
	CString GetTasklistSourceControlPath(LPCTSTR szTasklistPath);
	CString GetTaskSourceControlFolder(LPCTSTR szTasklistPath);
	CString GetTaskSourceControlPath(LPCTSTR szTasklistPath, DWORD dwTaskID);

	CString GetSourceControlID(BOOL bIncludeUser, BOOL bAlternate);
	BOOL MatchesSourceControlID(const CString& sID);

	BOOL CheckOutTask(LPCTSTR szTasklistPath, DWORD dwTaskID, const TODOITEM& tdi, LPCTSTR szXmlHeader, BOOL bIncludeUserInID);
	BOOL CheckInTask(LPCTSTR szTasklistPath, DWORD dwTaskID, TODOITEM& tdi);

	BOOL LoadCheckedOutTask(LPCTSTR szPath, DWORD dwTaskID, TODOITEM& tdi);
	BOOL LoadCheckedOutTask(LPCTSTR szPath, DWORD& dwTaskID, TODOITEM& tdi);

protected:
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDCSOURCECONTROLHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
