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

CTDLTaskComboBox::CTDLTaskComboBox() : CTabbedComboBox(IMAGESIZE)
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

BOOL CTDLTaskComboBox::AddTask(const CString& sTask, DWORD dwTaskID, int nIndent, int nImage)
{
	return InsertTask(GetCount(), sTask, dwTaskID, nIndent, nImage);
}

BOOL CTDLTaskComboBox::InsertTask(int nPos, const CString& sTask, DWORD dwTaskID, int nIndent, int nImage)
{
	int nTask = CDialogHelper::InsertString(*this, nPos, (CString(TAB, nIndent) + sTask), dwTaskID);

	if (nTask == CB_ERR)
		return FALSE;

	if ((nIndent > 0) || (nImage != -1))
	{
		TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetAddExtItemData(nTask);
		ASSERT(pItemData);

		if (pItemData)
		{
			pItemData->nIndent = nIndent;
			pItemData->nImage = nImage;
		}
	}

	return TRUE;
}

BOOL CTDLTaskComboBox::SetTaskImage(DWORD dwTaskID, int nImage)
{
	int nItem = CDialogHelper::FindItemByData(*this, dwTaskID);

	if (nItem == CB_ERR)
	{
		ASSERT(0);
		return FALSE;
	}

	TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetAddExtItemData(nItem);

	if (pItemData == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	pItemData->nImage = nImage;
	return TRUE;
}

int CTDLTaskComboBox::GetItemImage(int nItem) const
{
	const TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	return (pItemData ? pItemData->nImage : -1);
}

void CTDLTaskComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
									DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (IsHeadingItem(nItem) || !m_hilTasks)
	{
		CTabbedComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
		return;
	}

	// If we have no headings then we need to indent the text to account for the image
	// Otherwise the base class will indent the text of its own accord
	CRect rText(rect);

	if (m_nNumHeadings == 0)
		rText.left += IMAGESIZE;
	
	CTabbedComboBox::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);

	const TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	if (pItemData && (pItemData->nImage >= 0))
	{
		CRect rIcon(rect);
		rIcon.left += (IMAGESIZE * pItemData->nIndent);

		if (m_nNumHeadings)
			rIcon.left -= IMAGESIZE;
		
		ImageList_Draw(m_hilTasks, pItemData->nImage, dc, rIcon.left, rIcon.top, ILD_TRANSPARENT);
	}
}
