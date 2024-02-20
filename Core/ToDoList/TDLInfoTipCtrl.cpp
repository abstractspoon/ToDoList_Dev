// TDLInfoTipCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLInfoTipCtrl.h"
#include "TDCEnumContainers.h"
#include "ToDoCtrlDataDefines.h"
#include "ToDoCtrlData.h"
#include "ToDoCtrlDataUtils.h"

#include "..\Shared\Misc.h"
#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\Localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////

static const CString EMPTY_STR;

/////////////////////////////////////////////////////////////////////////////
// CTDLInfoTipCtrl

CTDLInfoTipCtrl::CTDLInfoTipCtrl(const CToDoCtrlData& data, 
								 const CTDCCustomAttribDefinitionArray& aCustAttribs, 
								 const CContentMgr& mgrContent)
	:
	m_data(data),
	m_aCustAttribs(aCustAttribs),
	m_formatter(data, mgrContent),
	m_calculator(data)
{
}

CTDLInfoTipCtrl::~CTDLInfoTipCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLInfoTipCtrl, CToolTipCtrlEx)
	//{{AFX_MSG_MAP(CTDLInfoTipCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLInfoTipCtrl message handlers

int CTDLInfoTipCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolTipCtrlEx::OnCreate(lpCreateStruct) != 0)
		return -1;

	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);
	return 0;
}

void CTDLInfoTipCtrl::OnTimer(UINT nIDEvent)
{
	// CImplicit check for mouse-down
	if ((m_tiLast.hwnd != NULL) && (::GetCapture() != NULL))
	{
		Activate(FALSE);
		return;
	}

	CToolTipCtrlEx::OnTimer(nIDEvent);
}

CString CTDLInfoTipCtrl::FormatTip(DWORD dwTaskID,
								   const CTDCAttributeMap& mapAttrib,
								   int nMaxCommentsLen,
								   const CString& sCompletionStatus) const
{
	// Build the attribute array
	CTDCInfoTipItemArray aItems;

	if (!BuildSortedAttributeArray(dwTaskID, mapAttrib, nMaxCommentsLen, sCompletionStatus, aItems))
		return EMPTY_STR;

	// Calculate offset to make item values line up
	CClientDC dc(const_cast<CTDLInfoTipCtrl*>(this));
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dc, GetSafeHwnd());

	// 1. Normalise the labels by adding a tab 
	// 2. Keep track of the widest label
	// 3. Keep track of the longest value string excluding comments
	int nMaxLabelWidth = 0;
	int nMaxValueLen = 0;

	int nItem = aItems.GetSize();
	CRect rItem(0, 0, 10000, 100);

	while (nItem--)
	{
		TDCINFOTIPITEM& iti = aItems[nItem];

		iti.sLabel += _T(":\t");
		dc.DrawText(iti.sLabel, rItem, DT_EXPANDTABS | DT_CALCRECT);
		
		iti.nLabelWidth = rItem.Width();
		nMaxLabelWidth = max(nMaxLabelWidth, iti.nLabelWidth);

		// omit various 'longer than normal' attributes from checks
		switch (iti.nAttribID)
		{
		case TDCA_COMMENTS:
		case TDCA_FILELINK:
		case TDCA_TASKNAME:
		case TDCA_PATH:
			break;

		default:
			nMaxValueLen = max(nMaxValueLen, iti.sValue.GetLength());
			break;
		}
	}

	// Now add tabs to each label until they all have the same length 
	// as the maximum
	dc.DrawText(_T("\t"), rItem, DT_EXPANDTABS | DT_CALCRECT);
	int nTabWidth = rItem.Width();

	nItem = aItems.GetSize();

	while (nItem--)
	{
		TDCINFOTIPITEM& iti = aItems[nItem];

		while (iti.nLabelWidth < nMaxLabelWidth)
		{
			iti.sLabel += '\t';
			iti.nLabelWidth += nTabWidth;
		}
	}
	
	// Build the final string
	CString sTip;

	for (nItem = 0; nItem < aItems.GetSize(); nItem++)
	{
		TDCINFOTIPITEM& iti = aItems[nItem];

		// Multi-line or long attributes are special case
		if (!iti.sValue.IsEmpty())
		{
			const int MAX_LINELEN = max(75, (nMaxValueLen + 10));

			// Note: Add the ellipsis at the end else it messes up the line splitting
			BOOL bWantEllipsis = ((iti.nAttribID == TDCA_COMMENTS) && (iti.sValue.GetLength() < m_data.GetTaskCommentsLength(dwTaskID)));

			if (((iti.sValue.Find('\n') != -1) || (iti.sValue.GetLength() > MAX_LINELEN)))
			{
				CStringArray aLines;

				int nNumLines = Misc::SplitLines(iti.sValue, aLines, MAX_LINELEN);
				nNumLines -= Misc::RemoveEmptyItems(aLines);

				if (nNumLines > 1)
				{
					for (int nLine = 0; nLine < nNumLines; nLine++)
					{
						if (nLine == 0)
							sTip += iti.sLabel;
						else
							sTip += CString('\t', (nMaxLabelWidth / nTabWidth));

						sTip += aLines[nLine];

						if ((nLine == (nNumLines - 1)) && bWantEllipsis)
							sTip += _T("...");

						sTip += _T("\n");
					}
					continue;
				}
			}

			if (bWantEllipsis)
				iti.sValue += _T("...");
		}

		// all the rest
		sTip += iti.sLabel;
		sTip += iti.sValue;
		sTip += _T("\n");
	}

	dc.SelectObject(pOldFont);

	return sTip;
}

int CTDLInfoTipCtrl::BuildSortedAttributeArray(DWORD dwTaskID, 
											   const CTDCAttributeMap& mapAttrib,
											   int nMaxCommentsLen,
											   const CString& sCompletionStatus,
											   CTDCInfoTipItemArray& aItems) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		return 0;

	// Always add title
	aItems.Add(TDCINFOTIPITEM(TDCA_TASKNAME, IDS_TDLBC_TASK, pTDI->sTitle));

	// A few attributes require special handling
	if (mapAttrib.Has(TDCA_COMMENTS) && !pTDI->sComments.IsEmpty())
	{
		CString sComments = pTDI->sComments;

		if (nMaxCommentsLen > 0)
		{
			sComments = Misc::Left(pTDI->sComments, nMaxCommentsLen, TRUE);

			// Make 'nMaxCommentsLen' a hard limit
			if (sComments.GetLength() > nMaxCommentsLen)
				sComments = sComments.Left(nMaxCommentsLen);
		}

		aItems.Add(TDCINFOTIPITEM(TDCA_COMMENTS, IDS_TDLBC_COMMENTS, sComments));
	}

	// Rest are simple
#define ADDINFOITEM(a, s, e)						\
	if (mapAttrib.Has(a))							\
	{												\
		CString sVal = e;							\
		if (!sVal.IsEmpty())						\
			aItems.Add(TDCINFOTIPITEM(a, s, sVal));	\
	}

	ADDINFOITEM(TDCA_ALLOCBY, IDS_TDLBC_ALLOCBY, pTDI->sAllocBy);
	ADDINFOITEM(TDCA_ALLOCTO, IDS_TDLBC_ALLOCTO, m_formatter.GetTaskAllocTo(pTDI));
	ADDINFOITEM(TDCA_CATEGORY, IDS_TDLBC_CATEGORY, m_formatter.GetTaskCategories(pTDI));
	ADDINFOITEM(TDCA_COST, IDS_TDLBC_COST, m_formatter.GetTaskCost(pTDI, pTDS));
	ADDINFOITEM(TDCA_CREATEDBY, IDS_TDLBC_CREATEDBY, pTDI->sCreatedBy);
	ADDINFOITEM(TDCA_CREATIONDATE, IDS_TDLBC_CREATEDATE, m_formatter.GetTaskCreationDate(pTDI));
	ADDINFOITEM(TDCA_DEPENDENCY, IDS_TDLBC_DEPENDS, pTDI->aDependencies.Format());
	ADDINFOITEM(TDCA_DONEDATE, IDS_TDLBC_DONEDATE, m_formatter.GetTaskDoneDate(pTDI));
	ADDINFOITEM(TDCA_FILELINK, IDS_TDLBC_FILELINK, Misc::FormatArray(pTDI->aFileLinks, '\n')); // separate lines
	ADDINFOITEM(TDCA_FLAG, IDS_TDLBC_FLAG, CEnString(pTDI->bFlagged ? IDS_YES : 0));
	ADDINFOITEM(TDCA_ID, IDS_TDLBC_ID, Misc::Format(dwTaskID));
	ADDINFOITEM(TDCA_LASTMODBY, IDS_TDLBC_LASTMODBY, pTDI->sLastModifiedBy);
	ADDINFOITEM(TDCA_LASTMODDATE, IDS_TDLBC_LASTMODDATE, m_formatter.GetTaskLastModDate(pTDI));
	ADDINFOITEM(TDCA_LOCK, IDS_TDLBC_LOCK, CEnString(pTDI->bLocked ? IDS_YES : 0));
	ADDINFOITEM(TDCA_PATH, IDS_TDC_COLUMN_PATH, m_formatter.GetTaskPath(pTDI, pTDS));
	ADDINFOITEM(TDCA_PERCENT, IDS_TDLBC_PERCENT, m_formatter.GetTaskPercentDone(pTDI, pTDS));
	ADDINFOITEM(TDCA_POSITION, IDS_TDLBC_POS, m_formatter.GetTaskPosition(pTDS));
	ADDINFOITEM(TDCA_PRIORITY, IDS_TDLBC_PRIORITY, m_formatter.GetTaskPriority(pTDI, pTDS, FALSE));
	ADDINFOITEM(TDCA_RECURRENCE, IDS_TDLBC_RECURRENCE, m_formatter.GetTaskRecurrence(pTDI));
	ADDINFOITEM(TDCA_RISK, IDS_TDLBC_RISK, m_formatter.GetTaskRisk(pTDI, pTDS));
	ADDINFOITEM(TDCA_STATUS, IDS_TDLBC_STATUS, m_formatter.GetTaskStatus(pTDI, pTDS, sCompletionStatus));
	ADDINFOITEM(TDCA_SUBTASKDONE, IDS_TDLBC_SUBTASKDONE, m_formatter.GetTaskSubtaskCompletion(pTDI, pTDS));
	ADDINFOITEM(TDCA_TAGS, IDS_TDLBC_TAGS, m_formatter.GetTaskTags(pTDI));
	ADDINFOITEM(TDCA_TIMEESTIMATE, IDS_TDLBC_TIMEEST, m_formatter.GetTaskTimeEstimate(pTDI, pTDS));
	ADDINFOITEM(TDCA_TIMEREMAINING, IDS_TDLBC_REMAINING, m_formatter.GetTaskTimeRemaining(pTDI, pTDS));
	ADDINFOITEM(TDCA_TIMESPENT, IDS_TDLBC_TIMESPENT, m_formatter.GetTaskTimeSpent(pTDI, pTDS));
	ADDINFOITEM(TDCA_VERSION, IDS_TDLBC_VERSION, pTDI->sVersion);
	
	if (!pTDI->IsDone() || !m_data.HasStyle(TDCS_HIDESTARTDUEFORDONETASKS))
	{
		ADDINFOITEM(TDCA_STARTDATE, IDS_TDLBC_STARTDATE, m_formatter.GetTaskStartDate(pTDI, pTDS));
		ADDINFOITEM(TDCA_DUEDATE, IDS_TDLBC_DUEDATE, m_formatter.GetTaskDueDate(pTDI, pTDS));
	}

	if (mapAttrib.Has(TDCA_DEPENDENCY))
	{
		CDWordArray aDependents;

		if (m_data.GetTaskLocalDependents(dwTaskID, aDependents, FALSE))
			ADDINFOITEM(TDCA_DEPENDENCY, IDS_TDLBC_DEPENDENTS, Misc::FormatArray(aDependents));
	}

	// Custom attributes
	int nCust = m_aCustAttribs.GetSize();

	while (nCust--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustAttribs[nCust];

		CString sValue = m_formatter.GetTaskCustomAttributeData(pTDI, pTDS, attribDef);
		
		if (!sValue.IsEmpty())
			aItems.Add(TDCINFOTIPITEM(attribDef.GetAttributeID(), attribDef.sLabel, sValue));
	}

	// Alphabetic Sort
	Misc::SortArrayT<TDCINFOTIPITEM>(aItems, InfoTipSortProc);

	return aItems.GetSize();
}

int CTDLInfoTipCtrl::InfoTipSortProc(const void* pV1, const void* pV2)
{
	const TDCINFOTIPITEM* pITI1 = (const TDCINFOTIPITEM*)pV1;
	const TDCINFOTIPITEM* pITI2 = (const TDCINFOTIPITEM*)pV2;

	// Task title always sorts at top
	if (pITI1->nAttribID == TDCA_TASKNAME)
		return -1;

	if (pITI2->nAttribID == TDCA_TASKNAME)
		return 1;

	// Comments always sort at bottom
	if (pITI1->nAttribID == TDCA_COMMENTS)
		return 1;

	if (pITI2->nAttribID == TDCA_COMMENTS)
		return -1;

	// Rest sort by label
	return Misc::NaturalCompare(pITI1->sLabel, pITI2->sLabel);
}

