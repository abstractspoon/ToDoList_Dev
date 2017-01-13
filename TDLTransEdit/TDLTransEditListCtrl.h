#pragma once

/////////////////////////////////////////////////////////////////////////////

#include "..\shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

class CTransDictionary;
struct DICTITEM;

/////////////////////////////////////////////////////////////////////////////

enum { ENG_COL = 0, TRANS_COL, HINT_COL, NUM_COLS };

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditListCtrl

class CTDLTransEditListCtrl : public CInputListCtrl
{
	DECLARE_DYNAMIC(CTDLTransEditListCtrl)

public:
	CTDLTransEditListCtrl();
	virtual ~CTDLTransEditListCtrl();

	void Initialise();
	BOOL RebuildList(const CTransDictionary& dict, BOOL bShowAlternatives = TRUE, const CString& sFilter = _T(""));
	BOOL SetColumnWidths(const int nWidths[NUM_COLS]);
	BOOL GetColumnWidths(int nWidths[NUM_COLS]) const;
	void RecalcColumnWidths();
	void SetSortUntranslatedAtTop(BOOL bEnable);
	void Sort();
	CString GetEnglishText(int nItem) const;

protected:
	BOOL m_bSortUntranslatedAtTop;
	CMap<DWORD, DWORD, CString, CString&> m_mapSortTransCol;
	
protected:
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	// Virtual overrides
	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn);
	COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;

	CString FormatInfoTip(int nItem) const;

	static BOOL MatchesFilter(const DICTITEM* pDI, const CString& sFilter);
};


