#if !defined(AFX_TDLFINDRESULTSLISTCTRL_H__E3FBC372_D7CC_457E_B7BB_1036256A64E9__INCLUDED_)
#define AFX_TDLFINDRESULTSLISTCTRL_H__E3FBC372_D7CC_457E_B7BB_1036256A64E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdlfindresultslistctrl.h : header file
//

#include "tdcenum.h"

#include "..\shared\enlistctrl.h"
#include "..\shared\fontcache.h"

/////////////////////////////////////////////////////////////////////////////////////////////

struct SEARCHRESULT;

class CFilteredToDoCtrl;

/////////////////////////////////////////////////////////////////////////////////////////////

struct FTDRESULT
{
	FTDRESULT();
	FTDRESULT(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTaskList);

	BOOL IsReference() const { return (dwFlags & RF_REFERENCE); }
	BOOL IsDone() const { return (dwFlags & RF_DONE); }
	BOOL IsGoodAsDone() const { return (dwFlags & RF_GOODASDONE); }
	BOOL IsParent() const { return (dwFlags & RF_PARENT); }
	BOOL IsTopmost() const { return (dwFlags & RF_TOPMOST); }

	BOOL HasIcon() const;
	void DrawIcon(CDC* pDC, const CRect& rIcon) const;

	DWORD dwTaskID, dwFlags;
	const CFilteredToDoCtrl* pTDC;
};

typedef CArray<FTDRESULT, FTDRESULT&> CFTDResultsArray;

/////////////////////////////////////////////////////////////////////////////
// CTDLFindResultsListCtrl window

class CTDLFindResultsListCtrl : public CEnListCtrl
{
// Construction
public:
	CTDLFindResultsListCtrl();

// Attributes
public:
	BOOL AddHeaderRow(LPCTSTR szText);
	int AddResult(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTDC, BOOL bShowValueOnly);

	int GetResultCount() const; // all tasklists
	int GetResultCount(const CFilteredToDoCtrl* pTDC) const;
	int GetAllResults(CFTDResultsArray& aResults) const;
	int GetResults(const CFilteredToDoCtrl* pTDC, CFTDResultsArray& aResults) const;
	int GetResultIDs(const CFilteredToDoCtrl* pTDC, CDWordArray& aTaskIDs) const;

	void DeleteResults(const CFilteredToDoCtrl* pTDC);
	void DeleteAllResults();
	
	FTDRESULT* GetResult(int nItem) const { return (FTDRESULT*)GetItemData(nItem); }

	void SetStrikeThroughCompletedTasks(BOOL bStrikeThru);
	void SetGroupHeaderBackColor(COLORREF crBack);
	int GetColumnWidths(CIntArray& aWidths) const;
	BOOL SetColumnWidths(const CIntArray& aWidths);

protected:
	int m_nCurGroupID;
	int m_nHotItem;
	int m_nTextOffset;
	BOOL m_bStrikeThruDone;

	mutable CFontCache m_fonts;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFindResultsListCtrl)
	//}}AFX_VIRTUAL
	virtual void PreSubclassWindow();

// Implementation
public:
	virtual ~CTDLFindResultsListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLFindResultsListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnVScroll(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

protected:
	virtual int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const;
	virtual COLORREF GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual CFont* GetItemFont(int nItem, int nSubItem) const;
	virtual void DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual void DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);

	CString FormatWhatMatched(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTDC, BOOL bShowValueOnly) const;
	CString GetAttributeName(TDC_ATTRIBUTE nAttribID, const CFilteredToDoCtrl* pTDC) const;
	void UpdateHotItem(CPoint point);
	void RecalcTextOffset();
	int GetTextOffset(const FTDRESULT* pRes) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFINDRESULTSLISTCTRL_H__E3FBC372_D7CC_457E_B7BB_1036256A64E9__INCLUDED_)
