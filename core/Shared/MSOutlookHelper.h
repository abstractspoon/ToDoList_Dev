// OutlookHelper.h: interface for the COutlookHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTLOOKHELPER_H__C57DEEC1_2B2B_490A_9F2B_1A7B9127313F__INCLUDED_)
#define AFX_OUTLOOKHELPER_H__C57DEEC1_2B2B_490A_9F2B_1A7B9127313F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MSOutlookHelperEnum.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CMSOutlookItemDataMap : public CMap<OUTLOOK_FIELDTYPE, OUTLOOK_FIELDTYPE, CString, CString&>
{
public:
	CMSOutlookItemDataMap();
	CMSOutlookItemDataMap(const CMSOutlookItemDataMap& map);

	CString GetFieldData(OUTLOOK_FIELDTYPE nField) const;
	BOOL HasFieldData(OUTLOOK_FIELDTYPE nField) const;
};

//////////////////////////////////////////////////////////////////////

namespace OutlookAPI
{
	class _Application;
	class Selection;
	class _Item;
}

//////////////////////////////////////////////////////////////////////

class CMSOutlookHelper  
{
public:
	CMSOutlookHelper();
	virtual ~CMSOutlookHelper();

	BOOL IsValid() const { return (s_pOutlook != NULL); }

	static BOOL IsOutlookInstalled();
// 	static BOOL IsUrlHandlerInstalled();
// 	static BOOL InstallUrlHandler();
// 	static BOOL QueryInstallUrlHandler(UINT nIDQuery, UINT nMBOptions = MB_YESNO, int nMBSuccess = IDYES);
	static BOOL HandleUrl(HWND hWnd, LPCTSTR szURL);

	static BOOL IsOutlookObject(LPDATAOBJECT lpObject);
	static BOOL IsOutlookObject(COleDataObject* pObject);
	static BOOL IsOutlookObject(LPCTSTR szFilePath);
	static BOOL IsOutlookUrl(LPCTSTR szURL);

	// caller deletes before COutlookHelper object goes out of scope
	OutlookAPI::_Item* GetFirstFileObject(const CStringArray& aFiles);
	OutlookAPI::_Item* GetFileObject(LPCTSTR szFilePath);
	OutlookAPI::Selection* GetSelection();
	int GetSelectionCount();
	OutlookAPI::_Item* GetFirstSelectedObject();

	static OutlookAPI::_Item* GetFirstObject(OutlookAPI::Selection* pSelection);
	static int GetItemData(OutlookAPI::_Item& obj, CMSOutlookItemDataMap& mapData, BOOL bIncludeConfidential = TRUE);
	static CString GetItemData(OutlookAPI::_Item& obj, OUTLOOK_FIELDTYPE nField);
	static CString GetItemID(OutlookAPI::_Item& obj);
	static CString GetItemClass(OutlookAPI::_Item& obj);
	static BOOL IsConfidential(OUTLOOK_FIELDTYPE nField);
	static CString FormatItemAsUrl(OutlookAPI::_Item& obj, DWORD dwFlags = (OAFMT_NICE | OAFMT_REPLACESPACES)); 

	static BOOL HasDenyConfidential() { return s_bDenyConfidential; }
	static void ResetDenyConfidential() { s_bDenyConfidential = FALSE; }

	static CString GetFullPath(OutlookAPI::_Item& obj);
	static BOOL PathsMatch(OutlookAPI::_Item& obj1, OutlookAPI::_Item& obj2);
	
	static const CLIPFORMAT CF_OUTLOOK;
	static const DATE NULL_DATE;

protected:
	static OutlookAPI::_Application* s_pOutlook;
	static int s_nRefCount;
	static BOOL s_bDenyConfidential;

protected:
	static CString MapDate(DATE date);
	static BOOL EscapeText(CString& sText);
	static BOOL CanEscapeText(const CString& sText);

};

#endif // !defined(AFX_OUTLOOKHELPER_H__C57DEEC1_2B2B_490A_9F2B_1A7B9127313F__INCLUDED_)
