// RecurrenceEdit.cpp: implementation of the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTaskComboBox.h"
#include "ToDoCtrlData.h"

#include "..\shared\DialogHelper.h"
#include "..\Shared\Misc.h"

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

void CTDLTaskComboBox::BuildCombo(const CToDoCtrlData& data)
{
	ResetContent();

	BuildCombo(data, data.GetStructure(), 0);
}

void CTDLTaskComboBox::BuildCombo(const CToDoCtrlData& data, const TODOSTRUCTURE* pTDS, int nLevel)
{
	if (!pTDS->IsRoot())
	{
		DWORD dwTaskID = pTDS->GetTaskID();

		if (data.IsTaskDone(dwTaskID))
			return;

		CString sTaskName(TAB, nLevel);
		sTaskName += data.GetTaskTitle(dwTaskID);

		CDialogHelper::AddString(*this, sTaskName, dwTaskID);
		nLevel++;
	}

	for (int nPos = 0; nPos < pTDS->GetSubTaskCount(); nPos++)
		BuildCombo(data, pTDS->GetSubTask(nPos), nLevel); // RECURSIVE CALL
}

