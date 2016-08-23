#if !defined(AFX_TDLFINDRESULTSLISTCTRL_H__E3FBC372_D7CC_457E_B7BB_1036256A64E9__INCLUDED_)
#define AFX_TDLFINDRESULTSLISTCTRL_H__E3FBC372_D7CC_457E_B7BB_1036256A64E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdlfindresultslistctrl.h : header file
//

#include "tdcstruct.h"

#include "..\shared\enlistctrl.h"
#include "..\shared\fontcache.h"

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
	int AddResult(const SEARCHRESULT& result, LPCTSTR szTask, LPCTSTR szPath, const CFilteredToDoCtrl* pTDC);

	int GetResultCount() const; // all tasklists
	int GetResultCount(const CFilteredToDoCtrl* pTDC) const;
	int GetAllResults(CFTDResultsArray& aResults) const;
	int GetResults(const CFilteredToDoCtrl* pTDC, CFTDResultsArray& aResults) const;
	int GetResultIDs(const CFilteredToDoCtrl* pTDC, CDWordArray& aTaskIDs) const;

	void DeleteResults(const CFilteredToDoCtrl* pTDC);
	void DeleteAllResults();
	
	FTDRESULT* GetResult(int nItem) const { return (FTDRESULT*)GetItemData(nItem); }

	void RefreshUserPreferences();

	int GetColumnWidths(CIntArray& aWidths) const;
	BOOL SetColumnWidths(const CIntArray& aWidths);

protected:
	COLORREF m_crDone, m_crRef;
	int m_nCurGroupID;
	BOOL m_bStrikeThruDone;
	CFontCache m_fonts;

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
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

protected:
  	COLORREF GetResultTextColor(const FTDRESULT* pRes, BOOL bSelected, BOOL bHot) const;
	CFont* GetResultFont(const FTDRESULT* pRes, BOOL bHot);
	BOOL IsResultHot(const RECT& rResult) const;

	static BOOL OsIsXP();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFINDRESULTSLISTCTRL_H__E3FBC372_D7CC_457E_B7BB_1036256A64E9__INCLUDED_)
