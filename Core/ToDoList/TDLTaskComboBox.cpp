// RecurrenceEdit.cpp: implementation of the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTaskComboBox.h"
#include "TaskFile.h"
#include "TDCImageList.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\Misc.h"
#include "..\Shared\GraphicsMisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int MAX_DROPWIDTH = GraphicsMisc::ScaleByDPIFactor(400);
const int ICON_SIZE		= GraphicsMisc::ScaleByDPIFactor(16);

const int ICON_INDENT	= (ICON_SIZE + 2);
const int LEVEL_INDENT	= ICON_INDENT;
const int REF_INDENT	= ((ICON_INDENT / 2) + 1);

//////////////////////////////////////////////////////////////////////

CTDLTaskComboBox::CTDLTaskComboBox()
	:
	m_pIlTasks(NULL),
	m_bShowParentsAsFolders(FALSE),
	m_bEnableParents(TRUE)
{
}

BEGIN_MESSAGE_MAP(CTDLTaskComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CTDLTaskComboBox)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

DWORD CTDLTaskComboBox::GetSelectedTaskID(BOOL bTrueTask) const
{
	int nSel = GetCurSel();
	
	if (nSel == CB_ERR)
		return 0;

	if (CDialogHelper::ComboHasEdit(*this) && CDialogHelper::GetEditText(*this).IsEmpty())
		return 0;

	if (bTrueTask)
	{
		DWORD dwRefTaskID = GetItemRefTaskID(nSel);

		if (dwRefTaskID)
			return dwRefTaskID;
	}

	// else
	return GetItemData(nSel);
}

BOOL CTDLTaskComboBox::SetSelectedTaskID(DWORD dwTaskID)
{
	if (dwTaskID == 0)
	{
		SetCurSel(CB_ERR);
		return FALSE;
	}

	return (CDialogHelper::SelectItemByDataT(*this, dwTaskID) != CB_ERR);
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

BOOL CTDLTaskComboBox::InsertTask(int nPos, const CString& sTask, DWORD dwTaskID, BOOL bParent, int nDepth, int nImage, DWORD dwRefTaskID)
{
	int nTask = CDialogHelper::InsertStringT(*this, nPos, sTask, dwTaskID);

	if (nTask == CB_ERR)
		return FALSE;
	else
		ASSERT(nTask == nPos);

	if ((nDepth > 0) || (nImage != -1) || bParent)
	{
		TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetAddExtItemData(nTask);
		ASSERT(pItemData);

		if (pItemData)
		{
			pItemData->nDepth = nDepth;
			pItemData->nImage = nImage;
			pItemData->bParent = bParent;
			pItemData->dwRefTaskID = dwRefTaskID;
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
							pItemData->nDepth, 
							nImage,
							pItemData->dwRefTaskID))
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
	if (!m_pIlTasks || !m_pIlTasks->GetSafeHandle())
		return -1;

	const TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	if (!pItemData)
		return -1;

	if ((pItemData->nImage == -1) && pItemData->bParent && m_bShowParentsAsFolders)
		return 0;

	return pItemData->nImage;
}

int CTDLTaskComboBox::GetItemDepth(int nItem) const
{
	const TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	return (pItemData ? pItemData->nDepth : 0);
}

DWORD CTDLTaskComboBox::GetItemRefTaskID(int nItem) const
{
	const TCB_ITEMDATA* pItemData = (TCB_ITEMDATA*)GetExtItemData(nItem);

	return (pItemData ? pItemData->dwRefTaskID : FALSE);
}

void CTDLTaskComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
									DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (IsHeadingItem(nItem))
	{
		COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
		return;
	}

	// else
	CRect rText(rect);

	int nImage = GetItemImage(nItem);
	BOOL bReference = (0 != GetItemRefTaskID(nItem));

	if (bList)
	{
		// ALWAYS indent the text to make room for the image, unless we have
		// headings in which case the base class will do that for us
		rText.left += (GetItemDepth(nItem) * LEVEL_INDENT);

		if (m_nNumHeadings == 0)
			rText.left += ICON_INDENT;
	}
	else if (nImage != -1)
	{
		rText.left += ICON_INDENT;
	}
	else if (bReference)
	{
		rText.left += REF_INDENT;
	}

	COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);

	// Draw icon and/or shortcut overlay
	if ((nImage != -1) || bReference)
	{
		CRect rImage(rText);
		rImage.OffsetRect(-ICON_INDENT, 0);

		if (!bList)
			rImage.top++;

		if (nImage != -1)
			m_pIlTasks->Draw(&dc, nImage, rImage.TopLeft());

		if (bReference)
			GraphicsMisc::DrawShortcutOverlay(&dc, rImage);
	}
}

BOOL CTDLTaskComboBox::IsSelectableItem(int nItem) const
{
	if (!COwnerdrawComboBoxBase::IsSelectableItem(nItem))
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

	return COwnerdrawComboBoxBase::PreTranslateMessage(pMsg);
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

int CTDLTaskComboBox::GetExtraListboxWidth() const
{
	return (COwnerdrawComboBoxBase::GetExtraListboxWidth() + ICON_INDENT);
}

int CTDLTaskComboBox::GetMaxDropWidth() const 
{ 
	return MAX_DROPWIDTH; 
}

int CTDLTaskComboBox::CalcMinItemHeight(BOOL bList) const
{
	int nMinHeight = COwnerdrawComboBoxBase::CalcMinItemHeight(bList);

	if (bList)
		nMinHeight = max(nMinHeight, (ICON_SIZE + 2));

	return nMinHeight;
}

int CTDLTaskComboBox::Populate(const CTaskFile& tasks, const CTDCImageList& ilTasks, BOOL bIncDoneTasks)
{
	return Populate(tasks, ilTasks, CDWordArray(), bIncDoneTasks);
}

int CTDLTaskComboBox::Populate(const CTaskFile& tasks, const CTDCImageList& ilTasks, const CDWordArray& aRecentSel, BOOL bIncDoneTasks)
{
	m_pIlTasks = &ilTasks;

	CWaitCursor cursor;
	ResetContent();

	// Recent tasks first
	if (aRecentSel.GetSize())
	{
		int nPos = 0;

		for (int nSel = 0; nSel < aRecentSel.GetSize(); nSel++)
		{
			HTASKITEM hTask = tasks.FindTask(aRecentSel[nSel]);
			ASSERT(hTask);

			if (hTask && (bIncDoneTasks || !tasks.IsTaskGoodAsDone(hTask)))
			{
				int nImage = (m_pIlTasks ? m_pIlTasks->GetImageIndex(tasks.GetTaskIcon(hTask)) : -1);

				InsertTask(nPos++,
						   tasks.GetTaskTitle(hTask),
						   tasks.GetTaskID(hTask),
						   tasks.IsTaskParent(hTask),
						   0,
						   nImage,
						   tasks.GetTaskReferenceID(hTask));
			}
		}

		if (nPos != 0)
		{
			SetHeadingItem(InsertString(0, _T("Recently Selected")));
			nPos++;
			
			SetHeadingItem(InsertString(nPos, _T("All Tasks")));
		}
	}

	// Then 'all' tasks
	Populate(tasks, NULL, 0, bIncDoneTasks);

	return GetCount();
}

void CTDLTaskComboBox::Populate(const CTaskFile& tasks, HTASKITEM hTask, int nDepth, BOOL bIncDoneTasks)
{
	if (hTask)
	{
		if (!bIncDoneTasks && tasks.IsTaskGoodAsDone(hTask))
			return;

		int nImage = (m_pIlTasks ? m_pIlTasks->GetImageIndex(tasks.GetTaskIcon(hTask)) : -1);

		InsertTask(GetCount(),
				   tasks.GetTaskTitle(hTask),
				   tasks.GetTaskID(hTask),
				   tasks.IsTaskParent(hTask),
				   nDepth++,
				   nImage,
				   tasks.GetTaskReferenceID(hTask));
	}

	HTASKITEM hSubtask = tasks.GetFirstTask(hTask);

	while (hSubtask)
	{
		Populate(tasks, hSubtask, nDepth, bIncDoneTasks); // RECURSIVE CALL
		hSubtask = tasks.GetNextTask(hSubtask);
	}
}
