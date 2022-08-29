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

BOOL CTDLTaskComboBox::AddTask(const CString& sTask, DWORD dwTaskID, int nLevel)
{
	return (CB_ERR != CDialogHelper::AddString(*this, (CString(TAB, nLevel) + sTask), dwTaskID));
}
