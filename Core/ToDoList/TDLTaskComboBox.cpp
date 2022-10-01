// RecurrenceEdit.cpp: implementation of the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTaskComboBox.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\Misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const TCHAR TAB = '\t';

const UINT WM_RESELECTTASKID = (CB_MSGMAX + 1);

//////////////////////////////////////////////////////////////////////

CTDLTaskComboBox::CTDLTaskComboBox() 
	: 
	CTabbedComboBox(IMAGESIZE),
	m_bShowParentsAsFolders(FALSE),
	m_bEnableParents(TRUE)
{
}

BEGIN_MESSAGE_MAP(CTDLTaskComboBox, CTabbedComboBox)
	//{{AFX_MSG_MAP(CTDLTaskComboBox)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditChange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
	ON_MESSAGE(WM_RESELECTTASKID, OnReselectTaskID)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

DWORD CTDLTaskComboBox::GetSelectedTaskID() const
{
	return CDialogHelper::GetSelectedItemData(*this);
}

BOOL CTDLTaskComboBox::SetSelectedTaskID(DWORD dwTaskID)
{
	return (CDialogHelper::SelectItemByData(*this, dwTaskID) != CB_ERR);
}

CString CTDLTaskComboBox::GetSelectedTaskName() const
{
	CString sTask = CDialogHelper::GetSelectedItem(*this);
	sTask.TrimLeft();

	return sTask;
}

int CTDLTaskComboBox::GetSelectedTaskImage() const
{
	return GetItemImage(GetCurSel());
}

BOOL CTDLTaskComboBox::AddTask(const CString& sTask, DWORD dwTaskID, BOOL bParent, int nIndent, int nImage)
{
	return InsertTask(GetCount(), sTask, dwTaskID, bParent, nIndent, nImage);
}

BOOL CTDLTaskComboBox::InsertTask(int nPos, const CString& sTask, DWORD dwTaskID, BOOL bParent, int nIndent, int nImage)
{
	int nTask = CDialogHelper::InsertString(*this, nPos, (CString(TAB, nIndent) + sTask), dwTaskID);

	if (nTask == CB_ERR)
		return FALSE;
	else
		ASSERT(nTask == nPos);

	if ((nIndent > 0) || (nImage != -1) || bParent)
	{
		TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetAddExtItemData(nTask);
		ASSERT(pItemData);

		if (pItemData)
		{
			pItemData->nIndent = nIndent;
			pItemData->nImage = nImage;
			pItemData->bParent = bParent;
		}
	}

	return TRUE;
}

BOOL CTDLTaskComboBox::ModifyItem(int nItem, const CString& sName, int nImage)
{
	if (!IsValidIndex(nItem))
	{
		ASSERT(0);
		return FALSE;
	}

	CString sCurName = CDialogHelper::GetItem(*this, nItem);
	sCurName.TrimLeft();

	BOOL bNameChange = (sName != sCurName);

	TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	if (pItemData)
	{
		if (bNameChange)
		{
			if (!InsertTask(nItem, 
							sName, 
							pItemData->dwItemData, 
							pItemData->bParent, 
							pItemData->nIndent, 
							nImage))
			{
				return FALSE;
			}

			DeleteString(nItem + 1);
		}
		else if (nImage != pItemData->nImage)
		{
			pItemData->nImage = nImage;
		}
		else
		{
			return FALSE;
		}
	}
	else if (bNameChange)
	{
		if (!InsertTask(nItem,
						sName,
						GetItemData(nItem),
						FALSE,
						0,
						nImage))
		{
			return FALSE;
		}

		DeleteString(nItem + 1);
	}
	else if (nImage != -1)
	{
		TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetAddExtItemData(nItem);
		ASSERT(pItemData);

		if (pItemData)
			pItemData->nImage = nImage;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

int CTDLTaskComboBox::GetItemImage(int nItem) const
{
	if (!m_hilTasks)
		return -1;

	const TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	if (!pItemData)
		return -1;

	if ((pItemData->nImage == -1) && pItemData->bParent && m_bShowParentsAsFolders)
		return 0;

	return pItemData->nImage;
}

int CTDLTaskComboBox::GetItemIndent(int nItem) const
{
	const TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	return pItemData ? pItemData->nIndent : 0;
}

void CTDLTaskComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
									DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{

	if (IsHeadingItem(nItem))
	{
		CTabbedComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
		return;
	}

	// else
	CRect rText(rect), rIcon(rect);
	CString sText(sItem);
	int nImage = GetItemImage(nItem);

	if (bList)
	{
		// Always indent the text to make room for the image, unless we have
		// headings in which case the base class will do that for us
		if (m_nNumHeadings == 0)
			rText.left += IMAGESIZE;

		// Draw the actual image
		if (nImage != -1)
		{
			rIcon.left += (IMAGESIZE * GetItemIndent(nItem));

			if (m_nNumHeadings)
				rIcon.left -= IMAGESIZE;
		}
	}
	else
	{
		sText.TrimLeft(TAB);

		if (nImage != -1)
			rText.left += IMAGESIZE;

		rIcon.top--;
	}

	CTabbedComboBox::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sText, bList, crText);

	if (nImage != -1)
		ImageList_Draw(m_hilTasks, nImage, dc, rIcon.left, rIcon.top, ILD_TRANSPARENT);
}

BOOL CTDLTaskComboBox::IsSelectableItem(int nItem) const
{
	if (!CTabbedComboBox::IsSelectableItem(nItem))
		return FALSE;

	if (!m_bEnableParents)
	{
		const TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);
		return (!pItemData || !pItemData->bParent);
	}

	// else
	return TRUE;
}

void CTDLTaskComboBox::OnEditChange()
{
	SelectNextFind(TRUE);
}

void CTDLTaskComboBox::OnDropDown()
{
	if (CDialogHelper::ComboHasEdit(*this))
	{
		// Trim leading tabs
		CString sText;
		GetWindowText(sText);

		if (!sText.IsEmpty() && (Misc::First(sText) == TAB))
		{
			// Modifying the window text will cause the selection 
			// to be cleared/changed after this function has returned, 
			// so we'll need to post ourselves a message to fix things up
			DWORD dwSelTaskID = GetSelectedTaskID();
			sText.TrimLeft(TAB);

			SetWindowText(sText);

			PostMessage(WM_RESELECTTASKID, 0, dwSelTaskID);
		}
	}
}

LRESULT CTDLTaskComboBox::OnReselectTaskID(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(lp);
	ASSERT(GetCurSel() == CB_ERR);
	ASSERT(CDialogHelper::ComboHasEdit(*this));
	ASSERT(GetDroppedState());

	if (SetSelectedTaskID(lp))
	{
		SetWindowText(GetSelectedTaskName());
		SetEditSel(0, -1); // select all
	}

	return 0L;
}

BOOL CTDLTaskComboBox::PreTranslateMessage(MSG* pMsg)
{
	if (CDialogHelper::ComboHasEdit(*this) && GetDroppedState())
	{
		switch (pMsg->message)
		{
		case WM_KEYDOWN:
			if (pMsg->wParam == VK_F3)
				SelectNextFind(!Misc::ModKeysArePressed(MKS_SHIFT));
			break;
		}
	}

	return CTabbedComboBox::PreTranslateMessage(pMsg);
}

void CTDLTaskComboBox::SelectNextFind(BOOL bForward)
{
	ASSERT(CDialogHelper::ComboHasEdit(*this));

	CString sText;
	GetWindowText(sText);
	
	DWORD dwSel = GetEditSel();

	if (SelectNextItem(sText, bForward))
	{
		// Restore the text and selection because the
		// selection change will have overwritten it
		SetWindowText(sText);
		SetEditSel(LOWORD(dwSel), HIWORD(dwSel));
	}
}
