// TDCAnonymizeTasklist.h: interface for the CTDCAnonymizeTasklist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCANONYMIZETASKLIST_H__36BFC958_99EA_4B0B_8E44_04AB78F47CE4__INCLUDED_)
#define AFX_TDCANONYMIZETASKLIST_H__36BFC958_99EA_4B0B_8E44_04AB78F47CE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "TaskFile.h"

#include "..\Shared\mapex.h"

//////////////////////////////////////////////////////////////////////

class CTDCAnonymizeTasklist  
{
public:
	static BOOL Anonymize(LPCTSTR szTaskfile);

protected:
	static void Anonymize(CTaskFile& tasks, HTASKITEM hTask, CMapStringToStringMap& mapTLD);
	static void Anonymize(CStringArray& aItems, CMapStringToString& mapTLD);
	static void Anonymize(CString& sItem, CMapStringToString& mapTLD);
};

#endif // !defined(AFX_TDCANONYMIZETASKLIST_H__36BFC958_99EA_4B0B_8E44_04AB78F47CE4__INCLUDED_)
