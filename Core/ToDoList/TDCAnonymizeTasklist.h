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
#include "TDCCustomAttributeDef.h"

#include "..\Shared\mapex.h"

//////////////////////////////////////////////////////////////////////

class CTDCAnonymizeTasklist  
{
public:
	static BOOL Anonymize(LPCTSTR szTaskfile);
	static BOOL Anonymize(LPCTSTR szTaskfile, CString& sAnonFilePath);

protected:
	CString m_sContent;
	CMapStringToStringMap m_mapSharedData;
	CTDCCustomAttribDefinitionArray m_aAttribDefs;

protected:
	CTDCAnonymizeTasklist();

	BOOL AnonymizeTasklist(CTaskFile& tasks);
	void AnonymizeAutoListData(CTaskFile& tasks);
	void AnonymizeCustomAttributeDefs(CTaskFile& tasks);
	void AnonymizeTask(CTaskFile& tasks, HTASKITEM hTask);
	void AnonymizeListItems(CStringArray& aItems, CMapStringToString& mapTLD);
	void AnonymizeListItem(CString& sItem, CMapStringToString& mapTLD);
	void AnonymizeCustomAttributeData(const CTDCCustomAttributeDataMap& mapCustomData,
										CTDCCustomAttributeDataMap& mapRndData);
	CString AnonymizeText(const CString& sItem);

	BOOL BuildContent(const CTaskFile& tasks);
	CString GetContent(const CTaskFile& tasks, HTASKITEM hTask) const;

	static BOOL IsWhiteSpaceOrNumberOrPunctuation(TCHAR c);

};

#endif // !defined(AFX_TDCANONYMIZETASKLIST_H__36BFC958_99EA_4B0B_8E44_04AB78F47CE4__INCLUDED_)
