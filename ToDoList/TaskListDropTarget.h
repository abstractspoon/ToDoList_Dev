// TaskListDropTarget.h: interface for the CTaskListDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTDROPTARGET_H__56519FB1_2923_45BB_97A2_08B8B1DC7C97__INCLUDED_)
#define AFX_TASKLISTDROPTARGET_H__56519FB1_2923_45BB_97A2_08B8B1DC7C97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\TreeCtrlHelper.h"

#include <afxole.h>

//////////////////////////////////////////////////////////////////////

// predecs
namespace OutlookAPI{ class Selection; }
class CMSOutlookHelper;

//////////////////////////////////////////////////////////////////////

struct TLDT_DATA
{	
	TLDT_DATA(COleDataObject* pObj = NULL, CPoint pt = 0);

	CString GetFirstFile() const;
	int GetFileCount() const;
	CString GetFile(int nFile) const;

	DWORD dwTaskID;
	COleDataObject* pObject;
	CStringArray* pFilePaths;
	OutlookAPI::Selection* pOutlookSelection;
	CPoint ptClient;
};

//////////////////////////////////////////////////////////////////////

const UINT WM_TLDT_DROP		= ::RegisterWindowMessage(_T("WM_TLDT_DROP"));		// wparam = TLDT_DATA*, lParam == (CWnd*)target
const UINT WM_TLDT_CANDROP	= ::RegisterWindowMessage(_T("WM_TLDT_CANDROP"));	// wparam = TLDT_DATA*, lParam == (CWnd*)target

//////////////////////////////////////////////////////////////////////

class CTaskListDropTarget : public COleDropTarget  
{
public:
	CTaskListDropTarget();
	virtual ~CTaskListDropTarget();

	BOOL Register(CWnd* pTarget, CWnd* pOwner);

protected:
	CWnd* m_pOwner;
	DWORD m_dwPrevItem; 
	CMSOutlookHelper* m_pOutlook;

	DWORD m_dwTVHoverStart;
	CHTIMap m_mapTVItems;

protected:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pObject, DROPEFFECT dropEffect, CPoint point);

protected:

	enum TLDT_HITTEST
	{
		TLDTHT_NONE = -1,
		TLDTHT_TASKVIEW,
		TLDTHT_FILEEDIT,
		TLDTHT_CAPTION,
	};

	TLDT_HITTEST DoHitTest(CWnd* pWnd, CPoint point, DWORD& dwHitTaskID);
	BOOL InitializeOutlook();
	void ResetDrag(CWnd* pWnd);

	static int GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles, BOOL& bFromText);
	static DROPEFFECT GetDropEffect(TLDT_HITTEST nHitTest, const TLDT_DATA& drop, BOOL bFromText = FALSE);
	static DROPEFFECT GetDropEffect(TLDT_HITTEST nHitTest, BOOL bItemHit = FALSE, BOOL bFromText = FALSE);
};

#endif // !defined(AFX_TASKLISTDROPTARGET_H__56519FB1_2923_45BB_97A2_08B8B1DC7C97__INCLUDED_)
