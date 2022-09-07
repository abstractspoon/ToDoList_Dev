// EDialog.h: interface for the EDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_)
#define AFX_DIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////////////////////////

#include "EnString.h"

//////////////////////////////////////////////////////////////////////

enum RT_CTRLSTATE // for SetCtrlState
{
	RTCS_ENABLED,
	RTCS_DISABLED,
	RTCS_READONLY,
};

//////////////////////////////////////////////////////////////////////

#ifndef WM_NCMOUSELEAVE
#	define WM_NCMOUSELEAVE 0x000002A2
#endif

//////////////////////////////////////////////////////////////////////

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

	static void DDX_CBValue(CDataExchange* pDX, int nIDC, int& value, int fallbackValue);
	static void DDX_CBValue(CDataExchange* pDX, CComboBox& combo, int& value, int fallbackValue);

	// Allows for out of order radio buttons
	static void DDX_Radio(CDataExchange* pDX, const UINT* pIDC, int nNumIDC, int& value);
	static void DDX_Radio(CDataExchange* pDX, const CUIntArray& aIDC, int& value);
	static void DDX_Radio(CDataExchange* pDX, int nIDC, int& value);

	template <class T>
	static void DDX_CBData(CDataExchange* pDX, CComboBox& combo, T& value, T fallbackValue)
	{
		if (!combo.GetCount())
			return;

		if (pDX->m_bSaveAndValidate)
		{
			value = (T)GetSelectedItemData(combo);
		}
		else
		{
			if (CB_ERR == SelectItemByData(combo, (DWORD)value))
			{
				value = fallbackValue;
				VERIFY(SelectItemByData(combo, (DWORD)value) != CB_ERR);
			}
		}
	}

	static UINT MessageBoxEx(const CWnd* pWnd, UINT nIDText, UINT nIDCaption, UINT nType = MB_OK);
	static UINT MessageBoxEx(const CWnd* pWnd, LPCTSTR szText, UINT nIDCaption, UINT nType = MB_OK);

	static BOOL IsChildOrSame(HWND hWnd, HWND hwndChild);
	static BOOL IsChildOrSame(const CWnd* pWnd, const CWnd* pChild);
	static BOOL CtrlWantsEnter(HWND hwnd);
	static CString GetCtrlLabel(const CWnd* pWnd, BOOL bStripAccelerator = TRUE);
	static CString GetCtrlText(const CWnd* pWnd, BOOL bStripAccelerator = TRUE);
	static HWND GetWindowFromPoint(HWND hwndParent, POINT ptScreen);

	static int GetCtrlsCount(const CWnd* pParent, LPCTSTR szClass = NULL);
	static int GetCtrlIDs(const CWnd* pParent, CUIntArray& aCtrlIDs, LPCTSTR szClass = NULL);
	static void RemoveCtrlID(UINT nCtrlID, CUIntArray& aCtrlIDs);
	static int GetCtrlsText(const CWnd* pParent, CStringArray& aItems, BOOL bStripAccelerator = TRUE);
	static int GetCtrlsText(const CWnd* pParent, CStringArray& aItems, LPCTSTR szClass, BOOL bStripAccelerator = TRUE);
	static BOOL SetCtrlsText(CWnd* pParent, const CStringArray& aItems);
	static BOOL SetCtrlsText(CWnd* pParent, const CStringArray& aItems, LPCTSTR szClass);
	static int GetCtrlsText(const CWnd* pParent, CStringArray& aItems, const LPCTSTR szClasses[], int nNumClasses, BOOL bStripAccelerator = TRUE);
	static BOOL SetCtrlsText(CWnd* pParent, const CStringArray& aItems, const LPCTSTR szClasses[], int nNumClasses);

	static int GetCtrlAccelerators(const CWnd* pParent, CString& sAccelerators);
	static TCHAR GetChildAccelerator(const CWnd* pCtrl);
	static TCHAR EnsureUniqueAccelerator(CString& sText, HWND hWndRef);
	static BOOL EnsureUniqueAccelerators(CWnd* pParent);
	
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
	static void SetStyle(CWnd* pWnd, DWORD dwStyle, BOOL bSet = TRUE);
	
	// CEdit and CRichEditCtrl
	static BOOL SelectText(const CWnd* pEdit, LPCTSTR szText, int nSearchStart, int nSearchLen);
	static BOOL SelectText(const CWnd* pParent, UINT nCtrlID, LPCTSTR szText, int nSearchStart, int nSearchLen);

	// Comboboxes and Listboxes
	template <class T, class S>
	static int BuildItemDataMap(const S& ctrl, T& mapItems)
	{
		mapItems.RemoveAll();
		int nNumItem = ctrl.GetCount();

		for (int nItem = 0; nItem < nNumItem; nItem++)
		{
			mapItems[ctrl.GetItemData(nItem)] = nItem;
		}

		return mapItems.GetCount();
	}
	
	template <class T, class S>
	static int FindItemByData(const S& ctrl, T itemData)
	{
		int nNumItem = ctrl.GetCount();

		for (int nItem = 0; nItem < nNumItem; nItem++)
		{
			if (ctrl.GetItemData(nItem) == (DWORD)itemData)
				return nItem;
		}

		return -1;
	}

	template <class T, class S>
	static int SelectItemByData(S& ctrl, T itemData)
	{
		int nItem = FindItemByData(ctrl, itemData);
		ctrl.SetCurSel(nItem);

		return nItem;
	}

	template <class T, class S>
	static T GetSelectedItemData(const S& ctrl, T fallbackValue)
	{
		int nSel = ctrl.GetCurSel();

		if (nSel != -1)
			return (T)ctrl.GetItemData(nSel);

		// else
		return fallbackValue;
	}

	template <class T, class S>
	static int AddString(S& ctrl, LPCTSTR szItem, T itemData)
	{
		int nIndex = ctrl.AddString(szItem);

		if (nIndex != -1)
			ctrl.SetItemData(nIndex, itemData);

		return nIndex;
	}

	template <class T, class S>
	static int AddString(S& ctrl, UINT nIDItem, T itemData)
	{
		return AddString(ctrl, CEnString(nIDItem), itemData);
	}

	template <class T, class S>
	static int InsertString(S& ctrl, int nPos, LPCTSTR szItem, T itemData)
	{
		int nIndex = ctrl.InsertString(nPos, szItem);

		if (nIndex != -1)
			ctrl.SetItemData(nIndex, itemData);

		return nIndex;
	}

	template <class T, class S>
	static int InsertString(S& ctrl, int nPos, UINT nIDItem, T itemData)
	{
		return InsertString(ctrl, nPos, CEnString(nIDItem), itemData);
	}


	
	// comboboxes
	static int SetComboBoxItems(CComboBox& combo, const CStringArray& aItems);
	static int GetComboBoxItems(const CComboBox& combo, CStringArray& aItems);
	static int RefreshMaxDropWidth(CComboBox& combo, CDC* pDCRef = NULL, int nTabWidth = -1, int nExtra = 0);
	static int CalcMaxTextWidth(CComboBox& combo, int nMinWidth = 0, BOOL bDropped = FALSE, CDC* pDCRef = NULL, int nTabWidth = 0);
	static int SelectItemByValue(CComboBox& combo, int nValue);
	static DWORD GetSelectedItemData(const CComboBox& combo);
	static int GetSelectedItemAsValue(const CComboBox& combo);
	static CString GetSelectedItem(const CComboBox& combo);
	static CString GetItem(const CComboBox& combo, int nItem);
	static CString GetEditText(const CComboBox& combo);

	static BOOL IsDroppedComboBox(HWND hCtrl);
	static int FindItemByValue(const CComboBox& combo, int nValue);
	static BOOL SetAutoComboReadOnly(CComboBox& combo, BOOL bVScroll, BOOL bReadonly = TRUE, int nDropHeight = 200);
	static void SetComboEditReadonly(CComboBox& combo, BOOL bReadonly = TRUE);
	static BOOL ComboHasEdit(const CComboBox& combo);
	static void MoveCombo(CComboBox& combo, const CRect& rNew, int nDropHeight = 200);

	// listboxes
	static int SetListBoxItems(CListBox& list, const CStringArray& aItems);
	static int GetListBoxItems(const CListBox& list, CStringArray& aItems);
	static int RefreshMaxColumnWidth(CListBox& list, CDC* pDCRef = NULL);
	static int CalcMaxTextWidth(CListBox& list, int nMinWidth = 0, CDC* pDCRef = NULL);
	static CString GetSelectedItem(const CListBox& list);
	static CString GetItem(const CListBox& list, int nItem);

	// better dialog control shortcut handling
	static BOOL ProcessDialogCtrlAccelerator(const MSG* pMsg);

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
	static int GetChildHeight(const CWnd* pChild);

	static void SetCtrlState(const CWnd* pParent, UINT nCtrlID, RT_CTRLSTATE nState);
	static void SetCtrlState(HWND hCtrl, RT_CTRLSTATE nState);
	static void SetCtrlsState(const CWnd* pParent, UINT nCtrlIDFrom, UINT nCtrlIDTo, RT_CTRLSTATE nState);

	static void ShowCtrls(const CWnd* pParent, UINT nCtrlIDFrom, UINT nCtrlIDTo, BOOL bShow = TRUE);
	static void ShowCtrls(const CWnd* pParent, const CUIntArray& aCtrlIDs, BOOL bShow = TRUE);
	static void ShowCtrl(const CWnd* pParent, UINT nCtrlID, BOOL bShow = TRUE);
	static void ShowCtrl(CWnd* pCtrl, BOOL bShow = TRUE);

	static void ExcludeCtrls(const CWnd* pParent, CDC* pDC, UINT nCtrlIDFrom, UINT nCtrlIDTo);
	static void ExcludeCtrls(const CWnd* pParent, CDC* pDC);
	static void ExcludeCtrl(const CWnd* pParent, UINT nCtrlID, CDC* pDC);
	static void ExcludeChild(const CWnd* pChild, CDC* pDC);

	static void EnableAllCtrls(const CWnd* pParent, BOOL bEnable = TRUE);
	static void InvalidateAllCtrls(const CWnd* pParent, BOOL bErase = TRUE);

	static BOOL TrackMouseLeave(HWND hWnd, BOOL bEnable = TRUE, BOOL bIncludeNonClient = TRUE);
	
	// helpers for updating only a single item
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, BYTE& value, BOOL bSaveAndValidate = TRUE);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, short& value, BOOL bSaveAndValidate = TRUE);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, int& value, BOOL bSaveAndValidate = TRUE);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, UINT& value, BOOL bSaveAndValidate = TRUE);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, long& value, BOOL bSaveAndValidate = TRUE);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, DWORD& value, BOOL bSaveAndValidate = TRUE);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, CString& value, BOOL bSaveAndValidate = TRUE);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, float& value, BOOL bSaveAndValidate = TRUE, int nDecimals = -1);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, double& value, BOOL bSaveAndValidate = TRUE, int nDecimals = -1);
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, CWnd& ctrl, BOOL bSaveAndValidate = TRUE);

	// for combo and listboxes only
	static BOOL UpdateDataExact(CWnd* pWnd, int nIDC, CString& value, BOOL bSaveAndValidate = TRUE);

	static CWnd* FindNextMatch(CWnd* pCurrent, UINT nShortcut);
	static TCHAR EnsureUniqueAccelerator(CWnd* pCtrl, const CString& sExclude);
	static BOOL CtrlMatchesClassFilter(const CWnd* pCtrl, LPCTSTR szClassFilter);
	static BOOL CtrlMatchesClassFilters(const CWnd* pCtrl, const LPCTSTR szClassFilters[], int nNumFilters);

protected: // very low-level helper
	static void TextFloatFormat(BOOL bSaveAndValidate, void* pData, double value, int nSizeGcvt, int nDecimals, LPTSTR szBuffer, int nBufSize);

protected: // Derived use only
	class CLockNotify
	{
	public:
		CLockNotify(HWND hWnd);
		~CLockNotify();

	protected:
		HWND m_hwndOldLockNotify;
	};

};

#endif // !defined(AFX_DIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_)
