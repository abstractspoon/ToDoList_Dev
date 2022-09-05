// RecurrenceEdit.cpp: implementation of the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTaskComboBox.h"

#include "..\Shared\DialogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const TCHAR TAB = '\t';

//////////////////////////////////////////////////////////////////////

CTDLTaskComboBox::CTDLTaskComboBox() 
	: 
	CTabbedComboBox(IMAGESIZE),
	m_bShowParentsAsFolders(FALSE),
	m_bEnableParents(TRUE)
{
}

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

	return sTask.TrimLeft();
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

	CString sCurName = CDialogHelper::GetItem(*this, nItem).TrimLeft();
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

	if (pItemData->bParent && m_bShowParentsAsFolders)
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
		if (nImage != -1)
		{
			rIcon.left += (IMAGESIZE * GetItemIndent(nItem));

			if (m_nNumHeadings)
				rIcon.left -= IMAGESIZE;
		}

		// Indent the text to make room for the image, unless we have
		// headings in which case the base class will do that for us
		if (m_nNumHeadings == 0)
			rText.left += IMAGESIZE;
	}
	else
	{
		sText.TrimLeft(TAB);

		if (nImage != -1)
			rText.left += IMAGESIZE;
	}

	if (nImage != -1)
		ImageList_Draw(m_hilTasks, nImage, dc, rIcon.left, rIcon.top, ILD_TRANSPARENT);

	CTabbedComboBox::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sText, bList, crText);
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
