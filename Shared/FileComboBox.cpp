// FileComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "FileComboBox.h"
#include "FileMisc.h"
#include "enbitmap.h"
#include "enfiledialog.h"

// CMultiFileEdit //////////////////////////////////////////////////////////

CFileComboBox::CMultiFileEdit::CMultiFileEdit(int nEditStyle) : CFileEdit(nEditStyle)
{

}

void CFileComboBox::CMultiFileEdit::HandleBrowseForFile(CEnFileDialog& dlg)
{
	dlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT;

	if (dlg.DoModal() == IDOK)
	{
		SetFocus();
		
		CStringArray aPaths;
		int nFile = dlg.GetPathNames(aPaths);

		if (nFile)
		{
			if (HasStyle(FES_RELATIVEPATHS))
			{
				while (nFile--)
					FileMisc::MakeRelativePath(aPaths[nFile], m_sCurFolder, FALSE);
			}
			
			SetWindowText(aPaths[0]);
			
			// send parent a change notification
			GetParent()->SendMessage(WM_COMMAND, MAKELPARAM(EN_CHANGE, GetDlgCtrlID()), (LPARAM)GetSafeHwnd());
			
			// and a browse notification
			CString sPaths = Misc::FormatArray(aPaths, '|');
			GetParent()->SendMessage(WM_FEN_BROWSECHANGE, GetDlgCtrlID(), (LPARAM)(LPCTSTR)sPaths);
		}
	}
}

// CFileComboBox //////////////////////////////////////////////////////////

CFileComboBox::CFileComboBox(int nEditStyle) 
	: 
	CAutoComboBox(ACBS_ALLOWDELETE | ACBS_ADDTOSTART),
	m_fileEdit(nEditStyle),
	m_imageIcons(16, 16),
	m_nLastTooltipBtnID(0),
	m_bReadOnly(FALSE)
{

}

CFileComboBox::~CFileComboBox()
{
}


BEGIN_MESSAGE_MAP(CFileComboBox, CAutoComboBox)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(WM_FEN_BROWSECHANGE, OnFileEditBrowseChange)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnFileEditGetFileIcon)
	ON_REGISTERED_MESSAGE(WM_FE_DISPLAYFILE, OnFileEditDisplayFile)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xffff, OnNeedTooltipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xffff, OnNeedTooltipText)
END_MESSAGE_MAP()

// CFileComboBox message handlers
void CFileComboBox::PreSubclassWindow()
{
	CAutoComboBox::PreSubclassWindow();

	VERIFY (InitFileEdit());

	EnableToolTips(TRUE);
}

BOOL CFileComboBox::PreCreateWindow(CREATESTRUCT& cs)
{
	// only support combos with edit boxes
	if (cs.style & CBS_DROPDOWNLIST)
		return FALSE;

	if (!InitFileEdit())
		return FALSE;

	EnableToolTips(TRUE);

	cs.style |= (CBS_OWNERDRAWFIXED | CBS_HASSTRINGS);

	return CAutoComboBox::PreCreateWindow(cs);
}

BOOL CFileComboBox::OnNeedTooltipText(UINT /*id*/, NMHDR* pNMHDR, LRESULT* pResult)
{
	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
	*pResult = 0;

	if (m_nLastTooltipBtnID == -1)
	{
		CString sFilePath;
		m_fileEdit.GetWindowText(sFilePath);
		ASSERT(!sFilePath.IsEmpty());

		pTTT->lpszText = _tcsdup((LPCTSTR)sFilePath);

		return TRUE;
	}
	else if (m_nLastTooltipBtnID != 0)
	{
		CString sTip = m_fileEdit.GetButtonTip(m_nLastTooltipBtnID);

		if (!sTip.IsEmpty())
			pTTT->lpszText = _tcsdup((LPCTSTR)sTip);

		return TRUE;
	}

	return FALSE;
}

int CFileComboBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// CFileEdit's buttons will not work when embedded inside
	// a combo box. I've spent long hours trying to work out
	// why but to no avail. Therefore we duplicate the tooltips
	// ourselves
	m_nLastTooltipBtnID = 0;

	CWnd::ClientToScreen(&point);

	int nBtn = m_fileEdit.GetButtonCount();

	while (nBtn--)
	{
		CRect rButton = m_fileEdit.GetButtonRectByIndex(nBtn);

		if (rButton.PtInRect(point))
		{
			m_nLastTooltipBtnID = m_fileEdit.GetButtonID(nBtn);

			pTI->hwnd = m_hWnd;
			pTI->uId = m_nLastTooltipBtnID;
			pTI->uFlags = (TTF_ALWAYSTIP | TTF_NOTBUTTON);
			pTI->lpszText = LPSTR_TEXTCALLBACK;

			CWnd::ScreenToClient(rButton);
			pTI->rect = rButton;

			return m_nLastTooltipBtnID;
		}
	}

	// Now test edit field
	if (m_fileEdit.m_bTipNeeded)
	{
		CRect rEdit, rCombo;
		m_fileEdit.GetWindowRect(rEdit);

		// Expand to combo borders
		GetWindowRect(rCombo);
		rEdit.top = rCombo.top;
		rEdit.bottom = rCombo.bottom;

		if (rEdit.PtInRect(point))
		{
			m_nLastTooltipBtnID = -1;

			pTI->hwnd = m_hWnd;
			pTI->uId = MAKELONG(point.x, point.y);
			pTI->uFlags = (TTF_ALWAYSTIP | TTF_NOTBUTTON);
			pTI->lpszText = LPSTR_TEXTCALLBACK;
			
			CWnd::ScreenToClient(rEdit);
			pTI->rect = rEdit;

			return 1;
		}
	}

	// else
	return -1;
}

void CFileComboBox::OnSize(UINT nType, int cx, int cy)
{
	CAutoComboBox::OnSize(nType, cx, cy);

	ResizeEdit();
}

void CFileComboBox::ResizeEdit()
{
	// resize the edit to better fit the combo
	if (m_fileEdit.GetSafeHwnd())
	{
		CRect rCombo;
		GetClientRect(rCombo);

		CRect rEdit(rCombo);
		rEdit.DeflateRect(2, 3, 2, 3);
		rEdit.right -= GetSystemMetrics(SM_CXVSCROLL);
		m_fileEdit.MoveWindow(rEdit, FALSE);
	}
}

BOOL CFileComboBox::InitFileEdit()
{
	ASSERT (!m_fileEdit.GetSafeHwnd());

	if (!m_fileEdit.SubclassDlgItem(1001, this))
		return FALSE;

	ResizeEdit();

	return TRUE;
}

LRESULT CFileComboBox::OnFileEditBrowseChange(WPARAM wp, LPARAM lp)
{
	ASSERT(wp == 1001);
	ASSERT(lp);

	CString sPaths((LPCTSTR)lp);
	CStringArray aPaths;
	
	if (Misc::Split(sPaths, aPaths, '|') > 1)
	{
		AddUniqueItems(aPaths);
		SelectString(-1, aPaths[0]);
	}

	HandleReturnKey();
	ResizeEdit();

	return 0L;
}

BOOL CFileComboBox::OnSelChange()
{
	// this constitutes an edit
	m_bEditChange = TRUE;
	
	return CAutoComboBox::OnSelChange();
}

LRESULT CFileComboBox::OnFileEditGetFileIcon(WPARAM wp, LPARAM lp)
{
	ASSERT(wp == 1001);
	ASSERT(lp);

	// pass to parent
	return GetParent()->SendMessage(WM_FE_GETFILEICON, GetDlgCtrlID(), lp);
}

LRESULT CFileComboBox::OnFileEditDisplayFile(WPARAM wp, LPARAM lp)
{
	ASSERT(wp == 1001);
	ASSERT(lp);

	// pass to parent
	return GetParent()->SendMessage(WM_FE_DISPLAYFILE, GetDlgCtrlID(), lp);
}

int CFileComboBox::GetFileList(CStringArray& aFiles) 
{ 
	return GetItems(aFiles); 
}

int CFileComboBox::SetFileList(const CStringArray& aFiles) 
{ 
	int nNumFiles = SetStrings(aFiles);
	
	if (nNumFiles)
		SetCurSel(0);
	else
		m_fileEdit.Invalidate();
	
	return nNumFiles;
}

int CFileComboBox::AddFiles(const CStringArray& aFiles)
{
	// add files in reverse order so that the first one ends up at the top
	int nFile = aFiles.GetSize();
	int nNumAdded = 0;

	while (nFile--)
	{
		if (AddFile(Misc::GetItem(aFiles, nFile)) != -1)
			nNumAdded++;
	}

	return nNumAdded;
}

void CFileComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList)
{
	CRect rText(rect);

	if (bList && !sItem.IsEmpty())
	{
		BOOL bDrawn = FALSE;

		if (m_fileEdit.HasStyle(FES_DISPLAYSIMAGES) && CEnBitmap::IsSupportedImageFile(sItem))
		{
			CString sFullPath(sItem);
			FileMisc::MakeFullPath(sFullPath, m_fileEdit.GetCurrentFolder());
			
			if (m_imageIcons.HasIcon(sFullPath) || m_imageIcons.Add(sFullPath, sFullPath))
			{
				bDrawn = m_imageIcons.Draw(&dc, sFullPath, rect.TopLeft());
			}
		}

		// Fallback/default
		if (!bDrawn)
			m_fileEdit.DrawFileIcon(&dc, sItem, rText);

		rText.left += 20;
	}

	CAutoComboBox::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList);
}

BOOL CFileComboBox::DeleteLBItem(int nItem)
{
	// Cache item text for later image removal
	CString sFilename;
	GetLBText(nItem, sFilename);

	if (CAutoComboBox::DeleteLBItem(nItem))
	{
		m_bEditChange = TRUE;

		// update the edit box with the current list selection
		int nLBSel = ::SendMessage(GetListbox(), LB_GETCURSEL, 0, 0);

		if (nLBSel != -1)
		{
			SetCurSel(nLBSel);
		}
		else if (GetCount())
		{
			if (nItem < GetCount())
				SetCurSel(nItem);
			else
				SetCurSel(nItem - 1);
		}

		// delete any image associated with this item
		if (m_imageIcons.HasIcon(sFilename))
			m_imageIcons.Remove(sFilename);

		return TRUE;
	}

	// else
	return FALSE;
}

LRESULT CFileComboBox::OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	// handle 'escape' to cancel deletions
	switch (msg)
	{
	case WM_CHAR:
		if (wp == VK_ESCAPE)
		{
			m_bEditChange = FALSE;
			ShowDropDown(FALSE);

			return 0L;
		}
		break;
	}

	// default handling
	return CAutoComboBox::OnEditboxMessage(msg, wp, lp);
}

void CFileComboBox::SetReadOnly(BOOL bReadOnly)
{
	m_bReadOnly = bReadOnly;

	ModifyFlags((bReadOnly ? ACBS_ALLOWDELETE : 0), (bReadOnly ? 0 : ACBS_ALLOWDELETE));
	m_fileEdit.SendMessage(EM_SETREADONLY, bReadOnly);
}

void CFileComboBox::HandleReturnKey()
{
	if (m_bReadOnly)
		m_bEditChange = FALSE;
	else
		CAutoComboBox::HandleReturnKey();
}
