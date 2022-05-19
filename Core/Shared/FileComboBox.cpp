// FileComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "FileComboBox.h"
#include "FileMisc.h"
#include "GraphicsMisc.h"
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
			GetParent()->SendMessage(WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)GetSafeHwnd());
			
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
	m_imageIcons(FALSE), // small icons
	m_bReadOnly(FALSE)
{

}

CFileComboBox::~CFileComboBox()
{
}

IMPLEMENT_DYNAMIC(CFileComboBox, CAutoComboBox)

BEGIN_MESSAGE_MAP(CFileComboBox, CAutoComboBox)
	ON_REGISTERED_MESSAGE(WM_FEN_BROWSECHANGE, OnFileEditBrowseChange)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnFileEditGetFileIcon)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILETOOLTIP, OnFileEditGetFileTooltip)
	ON_REGISTERED_MESSAGE(WM_FE_DISPLAYFILE, OnFileEditDisplayFile)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	END_MESSAGE_MAP()
	
// CFileComboBox message handlers
BOOL CFileComboBox::PreCreateWindow(CREATESTRUCT& cs)
{
	// only support combos with edit boxes
	if ((cs.style & 0xf) == CBS_DROPDOWNLIST)
		return FALSE;
	
	cs.style |= (CBS_OWNERDRAWFIXED | CBS_HASSTRINGS);
	
	return CAutoComboBox::PreCreateWindow(cs);
}

void CFileComboBox::OnPaint()
{
	CPaintDC dc(this);

	// default painting
	DefWindowProc(WM_PAINT, (WPARAM)(HDC)dc, 0);

	// If the edit field has an image and its icon rect 
	// is less than the height of the image, then we draw 
	// the extra bit that MIGHT have been clipped out
	if (!m_fileEdit.GetSafeHwnd() || m_fileEdit.GetWindowTextLength() == 0)
		return;

	CRect rIcon = m_fileEdit.GetIconScreenRect();

	if (rIcon.Height() >= m_imageIcons.GetIconSize())
		return;

	ScreenToClient(rIcon);

	// Because CFileEdit messes with its non-client rect
	// we can end up with a negative rectangle during startup
	if (rIcon.left < 0)
		return;

	// Check that the bit we need to draw is visible
	CRect rClip(rIcon);

	rClip.bottom = rClip.top + m_imageIcons.GetIconSize();
	rClip.top = rIcon.bottom;

	if (!dc.IntersectClipRect(rClip))
		return;

	CString sIcon;
	m_fileEdit.GetWindowText(sIcon);
	m_fileEdit.DrawFileIcon(&dc, sIcon, rIcon);
}

HBRUSH CFileComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CAutoComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	if ((nCtlColor == CTLCOLOR_EDIT) && (m_fileEdit.GetSafeHwnd() == NULL))
	{
		VERIFY (InitFileEdit());
	}

	return hbr;
}

int CFileComboBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT (m_fileEdit.GetSafeHwnd());

	ClientToScreen(&point);
	m_fileEdit.ScreenToClient(&point);

	int nTool = m_fileEdit.OnToolHitTest(point, pTI);

	if (nTool != -1)
	{
		pTI->hwnd = m_hWnd;

		m_fileEdit.ClientToScreen(&pTI->rect);
		ScreenToClient(&pTI->rect);

		return nTool;
	}

	// else
	return CAutoComboBox::OnToolHitTest(point, pTI);
}

BOOL CFileComboBox::InitFileEdit()
{
	if (!m_fileEdit.GetSafeHwnd())
	{
		if (!m_fileEdit.SubclassDlgItem(1001, this))
			return FALSE;

		m_fileEdit.SendMessage(EM_SETREADONLY, m_bReadOnly);

		// CFileEdit disables its tooltips when embedded in a combobox
		// simply because they don't seem to work
		ASSERT(!m_fileEdit.m_tooltip.GetSafeHwnd());

		// So we have to handle it ourselves
		EnableToolTips(TRUE);
	}

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

	return 0L;
}

BOOL CFileComboBox::OnSelChange()
{
	if (!m_bNotifyingParent)
	{
		// this constitutes an edit
		m_bEditChange = TRUE;
	}
	
	return CAutoComboBox::OnSelChange();
}

LRESULT CFileComboBox::OnFileEditGetFileIcon(WPARAM wp, LPARAM lp)
{
	ASSERT(wp == 1001);
	ASSERT(lp);

	// forward request to parent
	return GetParent()->SendMessage(WM_FE_GETFILEICON, GetDlgCtrlID(), lp);
}

LRESULT CFileComboBox::OnFileEditGetFileTooltip(WPARAM wp, LPARAM lp)
{
	ASSERT(wp == 1001);
	ASSERT(lp);

	// forward request to parent
	return GetParent()->SendMessage(WM_FE_GETFILETOOLTIP, GetDlgCtrlID(), lp);
}

LRESULT CFileComboBox::OnFileEditDisplayFile(WPARAM wp, LPARAM lp)
{
	ASSERT(wp == 1001);
	ASSERT(lp);

	// forward request to parent
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
	{
		SetCurSel(0);
	}
	else if (m_fileEdit.GetSafeHwnd())
	{
		m_fileEdit.Invalidate();
	}
	
	return nNumFiles;
}

void CFileComboBox::DDX(CDataExchange* pDX, CStringArray& value)
{
	if (pDX->m_bSaveAndValidate)
		GetFileList(value);
	else
		SetFileList(value);
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
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
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

		rText.left += m_imageIcons.GetIconSize() + 2;
	}

	CAutoComboBox::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);
}

BOOL CFileComboBox::DeleteLBItem(int nItem)
{
	// Cache item text for later image removal
	CString sFilename = GetItemText(nItem);

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
	switch (msg)
	{
	case WM_CHAR:
		// handle 'escape' to cancel deletions
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

LRESULT CFileComboBox::OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	// handle 'escape' to cancel deletions
	switch (msg)
	{
	case WM_LBUTTONUP:
		// Handle mouse selection changes when edit is readonly
		if (m_bReadOnly && GetDroppedState())
		{
			int nSel = m_scList.SendMessage(LB_ITEMFROMPOINT, 0, lp);

			SendMessage(CB_SHOWDROPDOWN, FALSE);

			if (nSel != LB_ERR)
			{
				CString sItem;
				GetLBText(nSel, sItem);

				m_fileEdit.SetWindowText(sItem);
			}

			return 0L; // we handled it
		}
	}

	// default handling
	return CAutoComboBox::OnListboxMessage(msg, wp, lp);
}

void CFileComboBox::SetReadOnly(BOOL bReadOnly)
{
	m_bReadOnly = bReadOnly;
	ModifyFlags((bReadOnly ? ACBS_ALLOWDELETE : 0), (bReadOnly ? 0 : ACBS_ALLOWDELETE));

	if (m_fileEdit.GetSafeHwnd())
		m_fileEdit.SendMessage(EM_SETREADONLY, bReadOnly);
}

void CFileComboBox::HandleReturnKey()
{
	if (m_bReadOnly)
		m_bEditChange = FALSE;
	else
		CAutoComboBox::HandleReturnKey();
}
