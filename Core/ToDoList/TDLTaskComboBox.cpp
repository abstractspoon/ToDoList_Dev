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

CTDLTaskComboBox::CTDLTaskComboBox() : CTabbedComboBox(16)
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

BOOL CTDLTaskComboBox::AddTask(const CString& sTask, DWORD dwTaskID, int nLevel, int nImage)
{
	int nTask = CDialogHelper::AddString(*this, (CString(TAB, nLevel) + sTask), dwTaskID);

	if (nTask == CB_ERR)
		return FALSE;

	SetItemImage(nTask, nImage);
	return TRUE;
}

BOOL CTDLTaskComboBox::SetTaskImage(DWORD dwTaskID, int nImage)
{
	return SetItemImage(CDialogHelper::FindItemByData(*this, dwTaskID), nImage);
}

BOOL CTDLTaskComboBox::SetItemImage(int nItem, int nImage)
{
	if (!IsValidIndex(nItem))
	{
		ASSERT(0);
		return FALSE;
	}

	TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	if (!pItemData && (nImage != -1))
	{
		pItemData = (TCB_ITEMDATA*)GetAddExtItemData(nItem);
		ASSERT(pItemData);

		if (pItemData == NULL)
			return FALSE;
	}

	pItemData->nImage = nImage;
	return TRUE;
}
