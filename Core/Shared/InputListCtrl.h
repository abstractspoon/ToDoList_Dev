#if !defined(AFX_INPUTLISTCTRL_H__2E5810B0_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_)
#define AFX_INPUTLISTCTRL_H__2E5810B0_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InputListCtrl.h : header file
//

#include "enlistctrl.h"
#include "popupeditctrl.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

enum { NOTVALID, ADDITEM, EDITITEM };

/////////////////////////////////////////////////////////////////////////////

enum IL_COLUMNTYPE 
{ 
	ILCT_TEXT, 
	ILCT_COMBO, 
	ILCT_BROWSE, 
	ILCT_DATE,
	ILCT_CHECK,
	ILCT_POPUPMENU, 
	ILCT_ICON,
}; 

/////////////////////////////////////////////////////////////////////////////
// CInputListCtrl window

class CInputListCtrl : public CEnListCtrl
{
	DECLARE_DYNAMIC(CInputListCtrl)

// Construction
public:
	CInputListCtrl();
	virtual ~CInputListCtrl();

	void AutoAdd(BOOL bRows, BOOL bCols);
	void SetAutoRowPrompt(const CString& sPrompt);
	BOOL CanEditSelectedCell() const;
	void EditSelectedCell();
	BOOL SetCellText(int nRow, int nCol, const CString& sText);
	BOOL DeleteAllItems(BOOL bIncludeCols = FALSE);
	BOOL DeleteItem(int nItem);
	void SetCurSel(int nRow, int nCol, BOOL bNotifyParent = FALSE);
	BOOL GetCurSel(int& nRow, int& nCol) const;
	int SetCurSel(int nIndex, bool bNotifyParent = FALSE); // single selection
	int GetCurSel() const;
	int GetLastEdit(int* pRow = NULL, int* pCol = NULL);
	void AllowDuplicates(BOOL bAllow, BOOL bNotify = FALSE);
	int AddRow(const CString& sRowText, int nImage = -1);
	int AddCol(const CString& sColText, int nWidth = -1, IL_COLUMNTYPE nColType = ILCT_TEXT);
	void SetView(int nView);
	void SetEditMask(LPCTSTR szMask, DWORD dwFlags = 0);
	void SetReadOnly(BOOL bReadOnly);
	void EndEdit();
	void RedrawCell(int nRow, int nCol, BOOL bErase = TRUE);

	virtual BOOL CanDeleteSelectedCell() const;
	virtual BOOL DeleteSelectedCell();
	virtual int GetItemIndent(int /*nItem*/) const { return 0; }

	// column methods
	void EnableColumnEditing(int nCol, BOOL bEnable);
	BOOL IsColumnEditingEnabled(int nCol) const;
	void SetAutoColumnWidth(int nWidth);
	void SetAutoColPrompt(const CString& sPrompt);
	void SetColumnType(int nCol, IL_COLUMNTYPE nType);
	IL_COLUMNTYPE GetColumnType(int nCol) const;
	
protected:
	class CColumnData2 : public CEnListCtrl::CColumnData
	{
	public:
		CColumnData2();
		BOOL bEditEnabled;
		IL_COLUMNTYPE nType;
	};

protected:
	int m_nItemLastSelected;
	int m_nColLastSelected;
	int m_nCurCol;
	int m_nEditItem;
	int m_nEditCol;
	CPopupEditCtrl m_editBox;
	BOOL m_bAutoAddRows;
	BOOL m_bAutoAddCols;
	int m_nAutoColWidth;
	CString m_sAutoRowPrompt;
	CString m_sAutoColPrompt;
	int m_nLastEditRow, m_nLastEditCol, m_nLastEditResult;
	BOOL m_bAllowDuplication;
	BOOL m_bNotifyDuplicates;
	CPoint m_ptPopupPos;

private:
	CHotTracker m_hotTrack;

protected:
	virtual void OnEndEdit(UINT uIDCtrl, int* pResult);
	virtual void OnCancelEdit();
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	//{{AFX_MSG(CInputListCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnEditEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHotChange(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnSelItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNotifyKillFocus(UINT nCtrlID, NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
		
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual BOOL IsEditing() const;
	virtual BOOL IsButtonEnabled(int nRow, int nCol) const;
	virtual CColumnData* GetNewColumnData() const { return new CColumnData2; }
	virtual int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const;
	virtual void GetCellEditRect(int nRow, int nCol, CRect& rCell) const;
	virtual void PrepareControl(CWnd& /*ctrl*/, int /*nRow*/, int /*nCol*/) {}
	virtual BOOL GetButtonRect(int nRow, int nCol, CRect& rButton) const;
	virtual BOOL DrawButton(CDC* pDC, int nRow, int nCol, const CString& sText, BOOL bSelected, CRect& rButton);
	virtual IL_COLUMNTYPE GetCellType(int nRow, int nCol) const;
	virtual void InitState();
	virtual void HideAllControls(const CWnd* pWndIgnore = NULL);

	virtual void DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);
	virtual void DrawCell(CDC* pDC, int nItem, int nCol, const CRect& rCell, const CString& sText, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);
	virtual void DrawCellBackground(CDC* pDC, int nItem, int nCol, const CRect& rCell, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);
	virtual void DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual UINT GetTextDrawFlags(int nCol) const;

	BOOL CreateControl(CComboBox& ctrl, UINT nID, DWORD dwComboStyles = CBS_DROPDOWNLIST | CBS_SORT);
	BOOL CreateControl(CEdit& ctrl, UINT nID, DWORD dwEditStyles = ES_AUTOHSCROLL);
	BOOL CreateControl(CDateTimeCtrl& ctrl, UINT nID, DWORD dwDateTimeStyles = DTS_SHORTDATEFORMAT);
	void PostCreateControl(CWnd& ctrl);
	void HideControl(CWnd& ctrl, const CWnd* pWndIgnore = NULL);
	void ShowControl(CWnd& ctrl, int nRow, int nCol, BOOL bBtnClick = FALSE);
	CPopupEditCtrl* GetEditControl();

	BOOL IsDuplicateRow(CString sRow, int nRowToIgnore) const;
	BOOL IsDuplicateCol(CString sCol, int nColToIgnore) const;
	CRect ScrollCellIntoView(int nRow, int nCol); // returns the final position of the cell 
	BOOL IsPrompt(int nItem, int nCol = -1) const;
	const CColumnData2* GetColumnData(int nCol) const;
	int InsertRow(const CString& sRowText, int nItem, int nImage = -1);
	BOOL CanDeleteCell(int nRow, int nCol) const;
	void NotifyParentEditCell(const CString& sText, int nRow = -1, int nCol = -1) const;
	BOOL HasNonTextCells() const;
	DWORD GetButtonState(int nRow, int nCol, BOOL bSelected) const;
	BOOL CellHasButton(int nRow, int nCol) const;
	BOOL IsCellSelected(int nRow, int nCol, BOOL bVisually = FALSE) const;
	void CheckApplyPushedState(const CRect& rBtn, DWORD& dwState) const;

	void DrawBlankButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const;
	void DrawIconButton(CDC* pDC, const CRect& rBtn, HICON hIcon, DWORD dwState) const;
	void DrawDateButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const;
	void DrawMenuButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const;
	void DrawBrowseButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const;
	void DrawComboButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const;
	void DrawCheckBoxButton(CDC* pDC, const CRect& rBtn, DWORD dwState) const;
	BOOL DrawButton(CDC* pDC, const CRect& rBtn, IL_COLUMNTYPE nType, DWORD dwState) const;

private:
	void RecalcHotButtonRects();
	BOOL IsButtonHot(int nRow, int nCol) const;
	void InvalidateHotButton(int nBtn);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTLISTCTRL_H__2E5810B0_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_)
