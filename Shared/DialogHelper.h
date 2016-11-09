// EDialog.h: interface for the EDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_)
#define AFX_DIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

enum RT_CTRLSTATE // for SetCtrlState
{
	RTCS_ENABLED,
	RTCS_DISABLED,
	RTCS_READONLY,
};

class CDialogHelper
{
public:
	// safe versions if the window text is empty
	static void DDX_Text(CDataExchange* pDX, int nIDC, BYTE& value);
	static void DDX_Text(CDataExchange* pDX, int nIDC, short& value);
	static void DDX_Text(CDataExchange* pDX, int nIDC, int& value);
	static void DDX_Text(CDataExchange* pDX, int nIDC, UINT& value);
	static void DDX_Text(CDataExchange* pDX, int nIDC, long& value);
	static void DDX_Text(CDataExchange* pDX, int nIDC, DWORD& value);
	static void DDX_Text(CDataExchange* pDX, int nIDC, CString& value);
	static void DDX_Text(CDataExchange* pDX, int nIDC, float& value, int nDecimals = -1);
	static void DDX_Text(CDataExchange* pDX, int nIDC, double& value, int nDecimals = -1);

	static UINT MessageBoxEx(const CWnd* pWnd, UINT nIDText, UINT nIDCaption, UINT nType = MB_OK);
	static UINT MessageBoxEx(const CWnd* pWnd, LPCTSTR szText, UINT nIDCaption, UINT nType = MB_OK);

	static BOOL IsChildOrSame(HWND hWnd, HWND hwndChild);
	static BOOL IsChildOrSame(const CWnd* pWnd, const CWnd* pChild);
	static BOOL CtrlWantsEnter(HWND hwnd);
	static CString GetCtrlLabel(const CWnd* pWnd);
	static CString GetCtrlText(const CWnd* pWnd);

	static int GetChildCtrlIDs(const CWnd* pParent, CUIntArray& aCtrlIDs, LPCTSTR szClass = NULL);
	static void RemoveCtrlID(UINT nCtrlID, CUIntArray& aCtrlIDs);

	// font helper
	static void SetFont(CWnd* pWnd, HFONT hFont, BOOL bRedraw = TRUE);
    static HFONT GetFont(const CWnd* pWnd);
    static HFONT GetFont(HWND hWnd);

	// covers static text, radiobuttons, checkboxes
	static void ResizeButtonStaticTextFieldsToFit(CWnd* pParent);
	static void ResizeButtonStaticTextToFit(CWnd* pParent, UINT nCtrlID, CDC* pDCRef = NULL);
	static void ResizeButtonStaticTextToFit(CWnd* pParent, CWnd* pCtrl, CDC* pDCRef = NULL);

	static void ModifyChildCtrlsStyle(BOOL bExStyle, const CWnd* pParent, DWORD dwRemove, 
										DWORD dwAdd, UINT nFlags = 0, LPCTSTR szClass = NULL);
	static void ModifyEditCtrlsStyle(BOOL bExStyle, const CWnd* pParent, DWORD dwRemove, 
										DWORD dwAdd, UINT nFlags = 0);
	
	// comboboxes
	static int SetComboBoxItems(CComboBox& combo, const CStringArray& aItems);
	static int GetComboBoxItems(const CComboBox& combo, CStringArray& aItems);
	static int RefreshMaxDropWidth(CComboBox& combo, CDC* pDCRef = NULL, int nTabWidth = 0, int nExtra = 0);
	static int CalcMaxTextWidth(CComboBox& combo, int nMinWidth = 0, BOOL bDropped = FALSE, CDC* pDCRef = NULL, int nTabWidth = 0);
	static BOOL SelectItemByValue(CComboBox& combo, int nValue);
	static BOOL SelectItemByData(CComboBox& combo, DWORD dwItemData);
	static int GetSelectedItemAsValue(const CComboBox& combo);
	static CString GetSelectedItem(const CComboBox& combo);
	static CString GetItem(const CComboBox& combo, int nItem);
	static DWORD GetSelectedItemData(const CComboBox& combo);
	static BOOL IsDroppedComboBox(HWND hCtrl);
	static int FindItemByValue(const CComboBox& combo, int nValue);
	static int AddString(CComboBox& combo, LPCTSTR szItem, DWORD dwItemData);
	static int AddString(CComboBox& combo, UINT nIDItem, DWORD dwItemData);
	static BOOL SetComboReadOnly(CComboBox& combo, BOOL bVScroll, BOOL bReadonly = TRUE, int nDropHeight = 200);
	static void SetComboEditReadonly(CComboBox& combo, BOOL bReadonly = TRUE);
	static int FindItemByData(const CComboBox& combo, DWORD dwItemData);
	static BOOL ComboHasEdit(const CComboBox& combo);
	static void MoveCombo(CComboBox& combo, const CRect& rNew, int nDropHeight = 200);

	// listboxes
	static int RefreshMaxColumnWidth(CListBox& list, CDC* pDCRef = NULL);
	static int CalcMaxTextWidth(CListBox& list, int nMinWidth = 0, CDC* pDCRef = NULL);
	static int FindItemByData(const CListBox& list, DWORD dwItemData);
	static int AddString(CListBox& list, LPCTSTR szItem, DWORD dwItemData);
	static int AddString(CListBox& list, UINT nIDItem, DWORD dwItemData);

	// better dialog control shortcut handling
	static BOOL ProcessDialogCtrlShortcut(const MSG* pMsg);
	static UINT GetShortcut(const CString& sText);

	static CRect OffsetCtrl(const CWnd* pParent, UINT nCtrlID, int dx, int dy); 
	static CRect ResizeCtrl(const CWnd* pParent, UINT nCtrlID, int cx, int cy);
	static CRect MoveCtrl(const CWnd* pParent, UINT nCtrlID, int x, int y);
	
	static CRect OffsetChild(CWnd* pChild, int dx, int dy);
	static CRect ResizeChild(CWnd* pChild, int cx, int cy);
	static CRect MoveChild(CWnd* pChild, int x, int y);

	static void OffsetCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, int x, int y); 
	static void ResizeCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, int cx, int cy);
	static void MoveCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, int x, int y);

	static CRect GetCtrlRect(const CWnd* pParent, UINT nCtrlID);
	static CRect GetChildRect(const CWnd* pChild);

	static void SetCtrlState(const CWnd* pParent, UINT nCtrlID, RT_CTRLSTATE nState);
	static void SetCtrlState(HWND hCtrl, RT_CTRLSTATE nState);
	static void SetCtrlsState(const CWnd* pParent, UINT nCtrlIDFrom, UINT nCtrlIDTo, RT_CTRLSTATE nState);

	static void ShowCtrls(const CWnd* pParent, UINT nCtrlIDFrom, UINT nCtrlIDTo, BOOL bShow = TRUE);
	static void ShowCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, BOOL bShow = TRUE);
	static void ShowCtrl(const CWnd* pParent, UINT nCtrlID, BOOL bShow = TRUE);
	static void ShowCtrl(CWnd* pCtrl, BOOL bShow = TRUE);

	static void ExcludeCtrls(const CWnd* pParent, CDC* pDC, UINT nCtrlIDFrom, UINT nCtrlIDTo, BOOL bIgnoreCorners = FALSE);
	static void ExcludeCtrls(const CWnd* pParent, CDC* pDC, BOOL bIgnoreCorners = FALSE);
	static void ExcludeCtrl(const CWnd* pParent, UINT nCtrlID, CDC* pDC, BOOL bIgnoreCorners = FALSE);
	static void ExcludeChild(const CWnd* pChild, CDC* pDC, BOOL bIgnoreCorners = FALSE);

	static int ShowMessageBox(HWND hwndParent, LPCTSTR szCaption, LPCTSTR szInstruction, LPCTSTR szText, UINT nFlags);

protected:
	CDialogHelper() : m_bInUpdateEx(FALSE) {}

	// helpers for updating only a single item
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, BYTE& value, BOOL bSaveAndValidate = TRUE);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, short& value, BOOL bSaveAndValidate = TRUE);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, int& value, BOOL bSaveAndValidate = TRUE);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, UINT& value, BOOL bSaveAndValidate = TRUE);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, long& value, BOOL bSaveAndValidate = TRUE);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, DWORD& value, BOOL bSaveAndValidate = TRUE);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, CString& value, BOOL bSaveAndValidate = TRUE);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, float& value, BOOL bSaveAndValidate = TRUE, int nDecimals = -1);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, double& value, BOOL bSaveAndValidate = TRUE, int nDecimals = -1);
	BOOL UpdateDataEx(CWnd* pWnd, int nIDC, CWnd& ctrl, BOOL bSaveAndValidate = TRUE);
	BOOL InUpdateEx() { return m_bInUpdateEx; }

	// for combo and listboxes only
	BOOL UpdateDataExact(CWnd* pWnd, int nIDC, CString& value, BOOL bSaveAndValidate = TRUE);

	static CWnd* FindNextMatch(CWnd* pCurrent, UINT nShortcut);

private:
	BOOL m_bInUpdateEx;
};

#endif // !defined(AFX_DIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_)
