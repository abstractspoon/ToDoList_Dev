// TDCTreeListCtrl.cpp: implementation of the CTDCTaskCtrlBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCTaskCtrlRenderer.h"
#include "todoctrldata.h"
#include "tdcstatic.h"
#include "tdcstruct.h"
#include "tdcimagelist.h"
#include "resource.h"

#include "..\shared\graphicsmisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\timehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\themed.h"
#include "..\shared\wndprompt.h"
#include "..\shared\osversion.h"
#include "..\shared\webmisc.h"
#include "..\shared\enbitmap.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\ScopedTimer.h"

#include "..\3rdparty\colordef.h"

#include "..\Interfaces\Preferences.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int LV_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int HD_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(6);
const int ICON_SIZE				= GraphicsMisc::ScaleByDPIFactor(16); 
const int MIN_RESIZE_WIDTH		= (ICON_SIZE + 3); 
const int COL_ICON_SIZE			= ICON_SIZE; 
const int COL_ICON_SPACING		= GraphicsMisc::ScaleByDPIFactor(2); 
const int MIN_COL_WIDTH			= GraphicsMisc::ScaleByDPIFactor(6);
const int MIN_TASKS_WIDTH		= GraphicsMisc::ScaleByDPIFactor(200);

const COLORREF COMMENTSCOLOR	= RGB(98, 98, 98);
const COLORREF ALTCOMMENTSCOLOR = RGB(164, 164, 164);

const UINT TIMER_BOUNDINGSEL	= 100;

const LPCTSTR APP_ICON			= _T("TDL_APP_ICON");

//////////////////////////////////////////////////////////////////////

enum
{
	IDC_TASKTREE = 100,		
	IDC_TASKTREECOLUMNS,		
	IDC_TASKTREEHEADER,		
};

//////////////////////////////////////////////////////////////////////

const CString EMPTY_STR(_T(""));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskCtrlRenderer::CTDCTaskCtrlRenderer(const CTDCImageList& ilIcons,
										   const CToDoCtrlData& data,
										   const CTDCColumnMap& mapColumns,
										   const CTDCColumnIDMap& mapVisibleCols,
										   const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
	: 
	m_data(data),
	m_ilTaskIcons(ilIcons),
	m_mapColumns(mapColumns),
	m_mapVisibleCols(mapVisibleCols),
	m_aCustomAttribDefs(aCustAttribDefs),
	m_crDone(CLR_NONE),
	m_crDue(CLR_NONE), 
	m_crDueToday(CLR_NONE),
	m_crFlagged(CLR_NONE),
	m_crStarted(CLR_NONE), 
	m_crStartedToday(CLR_NONE),
	m_crReference(CLR_NONE),
	m_crAltLine(CLR_NONE),
	m_crGridLine(CLR_NONE),
	m_dwTimeTrackTaskID(0), 
	m_nColorByAttrib(TDCA_NONE),
	m_comparer(data),
	m_calculator(data),
	m_formatter(data),
	m_imageIcons(16, 16)
{
}

CTDCTaskCtrlRenderer::~CTDCTaskCtrlRenderer()
{
}

///////////////////////////////////////////////////////////////////////////

void CTDCTaskCtrlRenderer::SetTimeTrackTaskID(DWORD dwTaskID)
{
	m_dwTimeTrackTaskID = dwTaskID;
}

BOOL CTDCTaskCtrlRenderer::GetTaskTextColors(DWORD dwTaskID, COLORREF& crText, COLORREF& crBack, BOOL bRef) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	
	if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		return FALSE;
	
	// else
	return GetTaskTextColors(pTDI, pTDS, crText, crBack, bRef, FALSE);
}

BOOL CTDCTaskCtrlRenderer::GetTaskTextColors(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS,
										COLORREF& crText, COLORREF& crBack, BOOL bRef) const
{
	return GetTaskTextColors(pTDI, pTDS, crText, crBack, bRef, FALSE);
}

BOOL CTDCTaskCtrlRenderer::GetTaskTextColors(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, COLORREF& crText, 
										COLORREF& crBack, BOOL bRef, BOOL bSelected) const
{
	ASSERT(pTDI && pTDS);

	if (!(pTDI && pTDS))
		return FALSE;

	if (bRef == -1)
		bRef = pTDI->IsReference();

	// all else
	crBack = CLR_NONE;
	crText = GetSysColor(COLOR_WINDOWTEXT);

	BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS);

	if (bDone)
	{
		if (HasColor(m_crDone))
		{
			crText = m_crDone; // parent and/or item is done
		}
		else if (bRef && HasColor(m_crReference))
		{
			crText = m_crReference;
		}
		else
		{
			crText = pTDI->color; 
		}
	}
	else // all incomplete tasks
	{
		while (true)
		{
			// if it's a ref task just return the ref colour
			if (bRef && HasColor(m_crReference))
			{
				crText = m_crReference;
				break;
			}

			// else
			BOOL bDueToday = m_calculator.IsTaskDueToday(pTDI, pTDS);
			BOOL bOverDue = m_calculator.IsTaskOverDue(pTDI, pTDS);

			// overdue takes priority
			if (HasColor(m_crDue) && bOverDue)
			{
				crText = m_crDue;
				break;
			}
			else if (HasColor(m_crDueToday) && bDueToday)
			{
				crText = m_crDueToday;
				break;
			}

			// started 'by now' takes priority
			if (HasColor(m_crStarted) && m_calculator.IsTaskStarted(pTDI, pTDS))
			{
				crText = m_crStarted;
				break;
			}
			else if (HasColor(m_crStartedToday) && m_calculator.IsTaskStarted(pTDI, pTDS, TRUE))
			{
				crText = m_crStartedToday;
				break;
			}

			// else
			if (HasColor(m_crFlagged) && pTDI->bFlagged)
			{
				crText = m_crFlagged;
				break;
			}

			if (HasStyle(TDCS_COLORTEXTBYPRIORITY))
			{
				int nPriority = FM_NOPRIORITY;

				if (bDueToday)
				{
					nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS, FALSE); // ignore due tasks
				}
				else if (bOverDue && HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY))
				{
					nPriority = 10;
				}
				else
				{
					nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS);
				}

				if (nPriority != FM_NOPRIORITY)
				{
					crText = GetPriorityColor(nPriority); 
					break;
				}
			}
			else if (HasStyle(TDCS_COLORTEXTBYATTRIBUTE))
			{
				switch (m_nColorByAttrib)
				{
				case TDCA_CATEGORY:
					GetAttributeColor(pTDI->GetCategory(0), crText);
					break;

				case TDCA_ALLOCBY:
					GetAttributeColor(pTDI->sAllocBy, crText);
					break;

				case TDCA_ALLOCTO:
					GetAttributeColor(pTDI->GetAllocTo(0), crText);
					break;

				case TDCA_STATUS:
					GetAttributeColor(pTDI->sStatus, crText);
					break;

				case TDCA_VERSION:
					GetAttributeColor(pTDI->sVersion, crText);
					break;

				case TDCA_EXTERNALID:
					GetAttributeColor(pTDI->sExternalID, crText);
					break;

				case TDCA_TAGS:
					GetAttributeColor(pTDI->GetTag(0), crText);
					break;

				default:
					ASSERT(0);
					break;
				}
			}
			else if (!HasStyle(TDCS_COLORTEXTBYNONE) && pTDI->color)
			{
				crText = pTDI->color; 
			}

			break; // always
		}
	}
	ASSERT(HasColor(crText));
	
	if (bSelected && !m_bSavingToImage)
	{
		crText = GraphicsMisc::GetExplorerItemTextColor(crText, GMIS_SELECTED, GMIB_THEMECLASSIC);
	}
	else
	{
		if (HasStyle(TDCS_TASKCOLORISBACKGROUND) && 
			(crText != GetSysColor(COLOR_WINDOWTEXT)) &&
			!m_calculator.IsTaskDone(pTDI, pTDS))
		{
			crBack = crText;
			crText = GraphicsMisc::GetBestTextColor(crBack);
		}
	}

	return TRUE;
}

COLORREF CTDCTaskCtrlRenderer::GetTaskCommentsTextColor(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	COLORREF crText = COMMENTSCOLOR;

	if (HasColor(m_crDone) && m_calculator.IsTaskDone(pTDI, pTDS, TDCCHECKALL))
		crText = m_crDone;

	return crText;
}

BOOL CTDCTaskCtrlRenderer::SetPriorityColors(const CDWordArray& aColors)
{
	ASSERT (aColors.GetSize() == 11);
	
	if ((aColors.GetSize() == 11) && !Misc::MatchAllT(aColors, m_aPriorityColors, FALSE))
	{
		m_aPriorityColors.Copy(aColors);
		return TRUE;
	}
	
	// else
	return FALSE; // invalid combination or no change
}

BOOL CTDCTaskCtrlRenderer::SetStartedTaskColors(COLORREF crStarted, COLORREF crStartedToday)
{
	BOOL bChange = SetColor(m_crStarted, crStarted);
	bChange |= SetColor(m_crStartedToday, crStartedToday);

	return bChange;
}

BOOL CTDCTaskCtrlRenderer::CheckUpdateDueBrushColor(COLORREF crNew, COLORREF& crCur, CBrush& brCur)
{
	if (crCur != crNew)
	{
		GraphicsMisc::VerifyDeleteObject(brCur);

		if (HasColor(crNew))
			brCur.CreateSolidBrush(crNew);

		crCur = crNew;
		return TRUE;
	}

	return FALSE;
}

BOOL CTDCTaskCtrlRenderer::SetDueTaskColors(COLORREF crDue, COLORREF crDueToday)
{
	BOOL bChange = CheckUpdateDueBrushColor(crDue, m_crDue, m_brDue);
	bChange |= CheckUpdateDueBrushColor(crDueToday, m_crDueToday, m_brDueToday);

	return bChange;
}

BOOL CTDCTaskCtrlRenderer::SetColor(COLORREF& color, COLORREF crNew)
{
	if (color != crNew)
	{
		color = crNew;
		return TRUE;
	}

	return FALSE; // no change
}

BOOL CTDCTaskCtrlRenderer::SetCompletedTaskColor(COLORREF color)
{
	return SetColor(m_crDone, color);
}

BOOL CTDCTaskCtrlRenderer::SetFlaggedTaskColor(COLORREF color)
{
	return SetColor(m_crFlagged, color);
}

BOOL CTDCTaskCtrlRenderer::SetReferenceTaskColor(COLORREF color)
{
	return SetColor(m_crReference, color);
}

BOOL CTDCTaskCtrlRenderer::SetAttributeColors(TDC_ATTRIBUTE nAttrib, const CTDCColorMap& colors)
{
	// see if there is any change
	if ((m_nColorByAttrib == nAttrib) && m_mapAttribColors.MatchAll(colors))
	{
		return FALSE; // no change
	}

	m_nColorByAttrib = nAttrib;
	m_mapAttribColors.Copy(colors);
	
	return TRUE;
}

BOOL CTDCTaskCtrlRenderer::GetAttributeColor(const CString& sAttrib, COLORREF& color) const
{
	return m_mapAttribColors.GetColor(sAttrib, color);
}

COLORREF CTDCTaskCtrlRenderer::GetPriorityColor(int nPriority) const
{
	if (nPriority < 0 || nPriority >= m_aPriorityColors.GetSize())
		return 0;
	
	return (COLORREF)m_aPriorityColors[nPriority];
}

void CTDCTaskCtrlRenderer::DrawCommentsText(CDC* pDC, const CRect& rRow, const CRect& rComments, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS)
{
	// Draw the minimum necessary
	COLORREF crText = GetTaskCommentsTextColor(pTDI, pTDS);

	if (HasStyle(TDCS_SHOWCOMMENTSINLIST))
	{
		int nFind = pTDI->sComments.FindOneOf(_T("\n\r")); 

		if (HasStyle(TDCS_SHOWFIRSTCOMMENTLINEINLIST))
		{
			if (nFind == 0) 
				return; // comments start with a newline -> show nothing

			// else
			DrawColumnText(pDC, pTDI->sComments, rComments, DT_LEFT, crText, TRUE, nFind);
		}
		else
		{
			// Calculate the max length of comments we are likely to show
			int nShow = ((int)(rComments.Width() / GraphicsMisc::GetAverageCharWidth(pDC)) * 2);
			nShow = min(nShow, pTDI->sComments.GetLength());

			CString sShow;
			LPTSTR szBuffer = sShow.GetBuffer(nShow);
			
			for (int nChar = 0; nChar < nShow; nChar++)
			{
				TCHAR cChar = pTDI->sComments[nChar];

				switch (cChar)
				{
				case '\r':
				case '\n':
				case '\t':
					cChar = ' ';
					// fall thru
				}

				szBuffer[nChar] = cChar;
			}
			sShow.ReleaseBuffer(nShow);
			sShow.TrimRight();

			DrawColumnText(pDC, sShow, rComments, DT_LEFT, crText, TRUE, nShow);
		}
	}
	else
	{
		DrawColumnText(pDC, _T("[...]"), rComments, DT_LEFT, crText, FALSE, 5);
	}
}

void CTDCTaskCtrlRenderer::DrawGridlines(CDC* pDC, const CRect& rect, BOOL bSelected, BOOL bHorz, BOOL bVert)
{
	if (HasColor(m_crGridLine))
	{
		if (bHorz)
			GraphicsMisc::DrawHorzLine(pDC, rect.left, rect.right, (rect.bottom - 1), m_crGridLine);

		if (bVert)
		{
			CRect rGridline(rect);

			// don't overdraw selection
			if (bSelected)
				rGridline.DeflateRect(0, 1);

			GraphicsMisc::DrawVertLine(pDC, rGridline.top, rGridline.bottom, rect.right - 1, m_crGridLine);
		}
	}
}

// BOOL CTDCTaskCtrlRenderer::HasThemedState(GM_ITEMSTATE nState) const
// {
// 	return ((nState != GMIS_NONE) && CThemed::AreControlsThemed());
// }

void CTDCTaskCtrlRenderer::DrawTasksRowBackground(CDC* pDC, const CRect& rRow, const CRect& rLabel, GM_ITEMSTATE nState, COLORREF crBack)
{
	ASSERT(!m_bSavingToImage || (nState == GMIS_NONE));

	BOOL bSelected = ((nState != GMIS_NONE) && !m_bSavingToImage);

	if (!bSelected)
	{
		ASSERT(HasColor(crBack));

		CRect rBack(rLabel);
		rBack.right = rRow.right; // else overwriting with comments produces artifacts

								  // if we have gridlines we don't fill the bottom line so 
								  // as to avoid overwriting gridlines previously drawn
		if (HasColor(m_crGridLine))
			rBack.bottom--;

		pDC->FillSolidRect(rBack, crBack);
	}

	// draw horz gridline before selection
	DrawGridlines(pDC, rRow, FALSE, TRUE, FALSE);

	if (bSelected) // selection of some sort
	{
		DWORD dwFlags = (GMIB_THEMECLASSIC | GMIB_EXTENDRIGHT);

		// if the columns are on the right then
		// we don't want to draw the rounded end 
		// on the right so it looks continuous with the columns
		if (HasStyle(TDCS_RIGHTSIDECOLUMNS))
			dwFlags |= GMIB_CLIPRIGHT;

		GraphicsMisc::DrawExplorerItemBkgnd(pDC, NULL, nState, rLabel, dwFlags);
	}
}

void CTDCTaskCtrlRenderer::DrawColumn(CDC* pDC, DWORD dwTaskID, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS,
								  const TDCCOLUMN& col, const CRect& rSubItem, COLORREF crText)
{
	TDC_COLUMN nColID = col.nColID;

	// Note: we pass dwTaskID NOT dwTrueID here so that references 
	// can be handled correctly
	CString sTaskColText = GetTaskColumnText(dwTaskID, pTDI, pTDS, nColID, TRUE);

	switch (nColID)
	{
	case TDCC_POSITION:
	case TDCC_RISK:
	case TDCC_RECURRENCE:
	case TDCC_ID:
	case TDCC_PARENTID:
	case TDCC_RECENTEDIT:
	case TDCC_COST:
	case TDCC_EXTERNALID:
	case TDCC_VERSION:
	case TDCC_ALLOCTO:
	case TDCC_ALLOCBY:
	case TDCC_STATUS:
	case TDCC_CATEGORY:
	case TDCC_TAGS:
	case TDCC_CREATEDBY:
	case TDCC_PATH:
	case TDCC_REMAINING:
	case TDCC_SUBTASKDONE:
	case TDCC_TIMEEST:
	case TDCC_LASTMODBY:
	case TDCC_COMMENTSSIZE:
		DrawColumnText(pDC, sTaskColText, rSubItem, col.nTextAlignment, crText);
		break;

	case TDCC_TIMESPENT:
		if (!sTaskColText.IsEmpty())
		{
			// show text in red if we're currently tracking
			DWORD dwTrueID = pTDS->GetTaskID();
			COLORREF crTemp = ((m_dwTimeTrackTaskID == dwTrueID) ? 255 : crText);

			DrawColumnText(pDC, sTaskColText, rSubItem, col.nTextAlignment, crTemp);
		}
		break;

	case TDCC_PRIORITY:
		// priority color
		if (!HasStyle(TDCS_DONEHAVELOWESTPRIORITY) || !m_calculator.IsTaskDone(pTDI, pTDS))
		{
			CRect rPriority(rSubItem);
			rPriority.DeflateRect(2, 1, 3, 2);

			// first draw the priority colour
			int nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS, FALSE);
			BOOL bHasPriority = (nPriority != FM_NOPRIORITY);

			if (bHasPriority)
			{
				COLORREF crFill = GetPriorityColor(nPriority);
				COLORREF crBorder = GraphicsMisc::Darker(crFill, 0.5);

				GraphicsMisc::DrawRect(pDC, rPriority, crFill, crBorder);//, nCornerRadius);
			}

			// then, if the task is also due, draw a small tag
			// unless it's due today and the user doesn't want today's tasks marked as due
			// or there's no due color 
			HBRUSH brTag = NULL;

			if (HasColor(m_crDue) && m_calculator.IsTaskOverDue(pTDI, pTDS))
			{
				brTag = m_brDue;
			}
			else if (HasColor(m_crDueToday) && m_calculator.IsTaskDueToday(pTDI, pTDS))
			{
				brTag = m_brDueToday;
			}

			if (brTag != NULL)
			{
				POINT pt[3] =
				{
					{ rSubItem.left, rSubItem.top + 7 },
					{ rSubItem.left, rSubItem.top },
					{ rSubItem.left + 7, rSubItem.top }
				};

				HGDIOBJ hOldBr = pDC->SelectObject(brTag);
				pDC->SelectStockObject(NULL_PEN);

				pDC->Polygon(pt, 3);
				pDC->SelectObject(hOldBr);

				// a black line between the two
				pDC->SelectStockObject(BLACK_PEN);
				pDC->MoveTo(rSubItem.left, rSubItem.top + 6);
				pDC->LineTo(rSubItem.left + 7, rSubItem.top - 1);
			}

			// draw priority number over the top
			if (bHasPriority && !HasStyle(TDCS_HIDEPRIORITYNUMBER))
			{
				COLORREF crTemp = GraphicsMisc::GetBestTextColor(GetPriorityColor(nPriority));
				DrawColumnText(pDC, sTaskColText, rSubItem, col.nTextAlignment, crTemp);
			}
		}
		break;

	case TDCC_PERCENT:
		if (!sTaskColText.IsEmpty())
		{
			CRect rPercent(rSubItem);
			rPercent.DeflateRect(2, 1, 3, 2);

			// draw default text first
			DrawColumnText(pDC, sTaskColText, rPercent, col.nTextAlignment, crText);

			if (HasStyle(TDCS_SHOWPERCENTASPROGRESSBAR))
			{
				// if the task is done then draw in 'done' colour else priority colour
				BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS);

				COLORREF crBar(m_crDone);

				if (!bDone || !HasStyle(TDCS_DONEHAVELOWESTPRIORITY)) // determine appropriate priority
				{
					int nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS, FALSE);
					crBar = GetPriorityColor(nPriority);

					// check for due
					if (m_calculator.IsTaskOverDue(pTDI, pTDS))
					{
						if (HasColor(m_crDueToday) && m_calculator.IsTaskDueToday(pTDI, pTDS))
						{
							crBar = m_crDueToday;
						}
						else if (HasColor(m_crDue))
						{
							crBar = m_crDue;
						}
					}
				}

				if (HasColor(crBar))
				{
					CRect rProgress(rPercent);

					// draw border
					COLORREF crBorder = GraphicsMisc::Darker(crBar, 0.5);
					GraphicsMisc::DrawRect(pDC, rProgress, CLR_NONE, crBorder);

					// draw fill
					int nPercent = m_calculator.GetTaskPercentDone(pTDI, pTDS);

					rProgress.DeflateRect(1, 1);
					rProgress.right = rProgress.left + MulDiv(rProgress.Width(), nPercent, 100);

					if (rProgress.Width() > 0)
					{
						pDC->FillSolidRect(rProgress, crBar);

						// Exclude the 'unfilled' part so that we do not
						// overwrite the text
						CRect rUnfilled(rSubItem);
						rUnfilled.left = rProgress.right;

						pDC->ExcludeClipRect(rUnfilled);

						// draw text in colour to suit progress bar
						COLORREF crTemp = GraphicsMisc::GetBestTextColor(crBar);
						DrawColumnText(pDC, sTaskColText, rSubItem, col.nTextAlignment, crTemp);
					}
				}
			}

		}
		break;

	case TDCC_TRACKTIME:
		{
			DWORD dwTrueID = pTDS->GetTaskID();

			if (m_dwTimeTrackTaskID == dwTrueID)
				DrawColumnImage(pDC, nColID, rSubItem);
		}
		break;

	case TDCC_FLAG:
		if (pTDI->bFlagged)
		{
			DrawColumnImage(pDC, nColID, rSubItem);
		}
		else if (m_calculator.IsTaskFlagged(pTDI, pTDS))
		{
			DrawColumnImage(pDC, nColID, rSubItem, TRUE);
		}
		break;

	case TDCC_LOCK:
		if (pTDI->bLocked)
		{
			DrawColumnImage(pDC, nColID, rSubItem);
		}
		else if (m_calculator.IsTaskLocked(pTDI, pTDS))
		{
			DrawColumnImage(pDC, nColID, rSubItem, TRUE);
		}
		break;

	case TDCC_REMINDER:
		{
/*
			DWORD dwTrueID = pTDS->GetTaskID();
			time_t tRem = GetTaskReminder(dwTrueID);

			// Reminder must be set and start/due date must be set
			if (tRem != 0)
			{
				if (tRem != -1)
				{
					if (HasStyle(TDCS_SHOWREMINDERSASDATEANDTIME))
					{
						COleDateTime dtRem(tRem);

						if (CDateHelper::IsDateSet(dtRem))
						{
							DrawColumnDate(pDC, dtRem, TDCD_REMINDER, rSubItem, crText);
						}
					}
					else
					{
						DrawColumnImage(pDC, nColID, rSubItem);
					}
				}
				else
				{
					DrawColumnImage(pDC, nColID, rSubItem, TRUE);
				}
			}
*/
		}
		break;

	case TDCC_STARTDATE:
		{
			COleDateTime date;
			BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS);
			BOOL bCalculated = FALSE;

			if (bDone && !HasStyle(TDCS_HIDESTARTDUEFORDONETASKS))
			{
				date = pTDI->dateStart;
			}
			else if (!bDone)
			{
				date = m_calculator.GetTaskStartDate(pTDI, pTDS);
				bCalculated = (date != pTDI->dateStart);
			}

			DrawColumnDate(pDC, date, TDCD_START, rSubItem, crText, bCalculated);
		}
		break;

	case TDCC_DUEDATE:
		{
			COleDateTime date;
			BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS);
			BOOL bCalculated = FALSE;

			if (bDone && !HasStyle(TDCS_HIDESTARTDUEFORDONETASKS))
			{
				date = pTDI->dateDue;
			}
			else if (!bDone)
			{
				date = m_calculator.GetTaskDueDate(pTDI, pTDS);
				bCalculated = (date != pTDI->dateDue);
			}

			DrawColumnDate(pDC, date, TDCD_DUE, rSubItem, crText, bCalculated);
		}
		break;


	case TDCC_DONEDATE:
		DrawColumnDate(pDC, pTDI->dateDone, TDCD_DONE, rSubItem, crText);
		break;

	case TDCC_CREATIONDATE:
		DrawColumnDate(pDC, pTDI->dateCreated, TDCD_CREATE, rSubItem, crText);
		break;

	case TDCC_LASTMODDATE:
		DrawColumnDate(pDC, m_calculator.GetTaskLastModifiedDate(pTDI, pTDS), TDCD_LASTMOD, rSubItem, crText);
		break;

	case TDCC_ICON:
		{
			int nIcon = GetTaskIconIndex(pTDI, pTDS);

			if (nIcon >= 0)
			{
				int nImageSize = m_ilTaskIcons.GetImageSize();
				CPoint pt(CalcColumnIconTopLeft(rSubItem, nImageSize));

				m_ilTaskIcons.Draw(pDC, nIcon, pt, ILD_TRANSPARENT);
			}
		}
		break;

	case TDCC_DEPENDENCY:
		if (pTDI->aDependencies.GetSize())
			DrawColumnImage(pDC, nColID, rSubItem);
		break;

	case TDCC_FILEREF:
		DrawColumnFileLinks(pDC, pTDI->aFileLinks, rSubItem, crText);
		break;

	case TDCC_DONE:
		{
			TTCB_CHECK nCheck = (pTDI->IsDone() ? TTCBC_CHECKED : TTCNC_UNCHECKED);

			if ((nCheck == TTCNC_UNCHECKED) && m_data.TaskHasCompletedSubtasks(pTDS))
				nCheck = TTCBC_MIXED;

			DrawColumnCheckBox(pDC, rSubItem, nCheck);
		}
		break;

	default:
		// custom attribute columns
		VERIFY(DrawItemCustomColumn(pTDI, pTDS, nColID, pDC, rSubItem, crText));
		break;
	}
}

int CTDCTaskCtrlRenderer::GetTaskIconIndex(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		return -1;

	return GetTaskIconIndex(pTDI, pTDS);
}

int CTDCTaskCtrlRenderer::GetTaskIconIndex(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	int nIcon = m_ilTaskIcons.GetImageIndex(pTDI->sIcon);

	if ((nIcon == -1) && pTDS->HasSubTasks() && HasStyle(TDCS_SHOWPARENTSASFOLDERS))
		nIcon = 0;

	return nIcon;
}

void CTDCTaskCtrlRenderer::DrawColumnFileLinks(CDC* pDC, const CStringArray& aFileLinks, const CRect& rect, COLORREF crText)
{
	int nNumFiles = aFileLinks.GetSize();

	switch (nNumFiles)
	{
	case 0:
		break;

	case 1:
		// TDCS_SHOWNONFILEREFSASTEXT only works for one file
		if (HasStyle(TDCS_SHOWNONFILEREFSASTEXT))
		{
			CString sFileRef = aFileLinks[0];
			int nImage = m_ilFileRef.GetFileImageIndex(sFileRef, TRUE);
			
			if (nImage == -1)
			{
				DrawColumnText(pDC, sFileRef, rect, DT_LEFT, crText);
				break;
			}
		}
		// else fall thru
		
	default:
		{
			// Everything else
			for (int nFile = 0; nFile < nNumFiles; nFile++)
			{
				CRect rIcon;
				
				if (!CalcFileIconRect(rect, rIcon, nFile, nNumFiles))
					break; // out of bounds
				
				// first check for a tdl://
				CString sFileRef = aFileLinks[nFile];
				
				if (sFileRef.Find(TDL_PROTOCOL) != -1)
				{
					// draw our app icon 
					if (m_imageIcons.HasIcon(APP_ICON) || 
						m_imageIcons.Add(APP_ICON, GraphicsMisc::GetAppWindowIcon(FALSE)))
					{
						m_imageIcons.Draw(pDC, APP_ICON, rIcon.TopLeft());
					}
				}
				else
				{
					// get the associated image, failing if necessary
					sFileRef.Remove('\"'); // remove quotes
					FileMisc::MakeFullPath(sFileRef, m_sTasklistFolder);
					
					if (m_imageIcons.HasIcon(sFileRef) || 
						(CEnBitmap::IsSupportedImageFile(sFileRef) && 
						FileMisc::PathExists(sFileRef) &&
						m_imageIcons.Add(sFileRef, sFileRef)))
					{
						m_imageIcons.Draw(pDC, sFileRef, rIcon.TopLeft());
					}
					else
					{
						m_ilFileRef.Draw(pDC, sFileRef, rIcon.TopLeft());
					}
				}
			}
		}
		break;
	}
}

void CTDCTaskCtrlRenderer::DrawColumnImage(CDC* pDC, TDC_COLUMN nColID, const CRect& rect, BOOL bAlternate)
{
	const TDCCOLUMN* pCol = GetColumn(nColID);
	ASSERT(pCol);

	if (pCol)
	{
		TDCC_IMAGE iImage = (bAlternate ? pCol->iAlternateImage : pCol->iImage);
		ASSERT(iImage != TDCC_NONE);
	
		if (iImage != TDCC_NONE)
		{
			int nImageSize = m_ilColSymbols.GetImageSize();
			CPoint ptDraw(CalcColumnIconTopLeft(rect, nImageSize, iImage));

			m_ilColSymbols.Draw(pDC, iImage, ptDraw, ILD_TRANSPARENT);
		}
	}
}

void CTDCTaskCtrlRenderer::DrawColumnCheckBox(CDC* pDC, const CRect& rSubItem, TTCB_CHECK nCheck)
{
	int nImageSize = m_ilCheckboxes.GetImageSize();
	CPoint pt(CalcColumnIconTopLeft(rSubItem, nImageSize));
				
	// if the line height is odd, move one pixel down
	// to avoid collision with selection rect
	pt.y += (rSubItem.Height() % 2);

	int nImage = (nCheck + 1); // first image is blank
	m_ilCheckboxes.Draw(pDC, nImage, pt, ILD_TRANSPARENT);
}

CPoint CTDCTaskCtrlRenderer::CalcColumnIconTopLeft(const CRect& rSubItem, int nImageSize, int nImage, int nCount) const
{
	CRect rImage(rSubItem.TopLeft(), CSize(nImageSize, nImageSize));
	GraphicsMisc::CentreRect(rImage, rSubItem, (nCount == 1), TRUE);
	
	if (nCount > 1)
		rImage.OffsetRect((nImage * (nImageSize + 1)), 0);

	return rImage.TopLeft();
}

BOOL CTDCTaskCtrlRenderer::CalcFileIconRect(const CRect& rSubItem, CRect& rIcon, int nImage, int nCount) const
{
	int nImageSize = m_ilFileRef.GetImageSize();

	rIcon = CRect(CalcColumnIconTopLeft(rSubItem, nImageSize, nImage, nCount), CSize(nImageSize, nImageSize));

	// we always draw the first icon
	if ((nImage == 0) || (rIcon.right <= rSubItem.right))
		return TRUE;

	// else
	rIcon.SetRectEmpty();
	return FALSE;
}

BOOL CTDCTaskCtrlRenderer::DrawItemCustomColumn(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID, 
											CDC* pDC, const CRect& rSubItem, COLORREF crText)
{
	if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		return FALSE;

	TDCCUSTOMATTRIBUTEDEFINITION attribDef;
	
	if (!m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef) || !attribDef.bEnabled)
		return TRUE;

	TDCCADATA data;
	pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data);

	CRect rCol(rSubItem);
	DWORD dwDataType = attribDef.GetDataType();
	
	switch (dwDataType)
	{
	case TDCCA_DATE:
		{
			double dDate = 0.0;
			m_calculator.GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dDate);

			DrawColumnDate(pDC, dDate, TDCD_CUSTOM, rCol, crText, FALSE, 
							attribDef.HasFeature(TDCCAF_SHOWTIME), attribDef.nTextAlignment);
		}
		break;
		
	case TDCCA_ICON:
		if (!data.IsEmpty() && (rCol.Width() >= CalcRequiredIconColumnWidth(1)))
		{
			CStringArray aImages;
			int nNumImage = data.AsArray(aImages);

			int nReqWidth = CalcRequiredIconColumnWidth(nNumImage);
			int nAvailWidth = rCol.Width();

			if (nAvailWidth < nReqWidth)
			{
				nNumImage = min(nNumImage, ((nAvailWidth + COL_ICON_SPACING - (LV_COLPADDING * 2)) / (COL_ICON_SIZE + COL_ICON_SPACING)));
				nReqWidth = CalcRequiredIconColumnWidth(nNumImage);
			}

			CString sName;
			
			if (nNumImage == 1)
				sName = attribDef.GetImageName(data.AsString());

			rCol.bottom = (rCol.top + COL_ICON_SIZE);
			GraphicsMisc::CentreRect(rCol, rSubItem, FALSE, TRUE); // centre vertically

			int nTextAlign = attribDef.nTextAlignment;
			
			switch (nTextAlign)
			{
			case DT_RIGHT:
				// We still draw from the left just like text
				rCol.left = (rCol.right - nReqWidth);
				break;
				
			case DT_CENTER:
				// if there is associated text then we align left
				if (sName.IsEmpty())
				{
					rCol.right = (rCol.left + nReqWidth);
					GraphicsMisc::CentreRect(rCol, rSubItem, TRUE, FALSE);
				}
				else 
				{
					nTextAlign = DT_LEFT;
				}
				break;
				
			case DT_LEFT:
			default:
				break;
			}

			BOOL bOverrun = FALSE;
			rCol.left += LV_COLPADDING;

			for (int nImg = 0; ((nImg < nNumImage) && !bOverrun); nImg++)
			{
				CString sImage, sDummy;

				if (TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(aImages[nImg], sImage, sDummy))
				{
					m_ilTaskIcons.Draw(pDC, sImage, rCol.TopLeft(), ILD_TRANSPARENT);
					rCol.left += (COL_ICON_SIZE + COL_ICON_SPACING);

					bOverrun = ((rCol.left + COL_ICON_SIZE) > rCol.right);
				}
			}
			
			// optional text for single list images
			if (!bOverrun && (nNumImage == 1) && attribDef.IsList() && !sName.IsEmpty())
			{
				DrawColumnText(pDC, sName, rCol, nTextAlign, crText);
			}
		}
		break;
		
	case TDCCA_BOOL:
		DrawColumnCheckBox(pDC, rSubItem, (data.AsBool() ? TTCBC_CHECKED : TTCNC_UNCHECKED));
		break;

	case TDCCA_FILELINK:
		{
			CStringArray aItems;
			
			if (data.AsArray(aItems))
				DrawColumnFileLinks(pDC, aItems, rSubItem, crText);
		}
		break;

	default:
		{
			CString sData = m_formatter.GetTaskCustomAttributeData(pTDI, pTDS, attribDef);
			DrawColumnText(pDC, sData, rCol, attribDef.nTextAlignment, crText);
		}
		break;
	}

	return TRUE; // we handled it
}

int CTDCTaskCtrlRenderer::CalcRequiredIconColumnWidth(int nNumImage)
{
	return ((nNumImage * (COL_ICON_SIZE + COL_ICON_SPACING)) - COL_ICON_SPACING + (LV_COLPADDING * 2));
}

BOOL CTDCTaskCtrlRenderer::FormatDate(const COleDateTime& date, TDC_DATE nDate, CString& sDate, CString& sTime, CString& sDow, BOOL bCustomWantsTime) const
{
	sDate = m_formatter.GetDateOnly(date, TRUE);

	if (sDate.IsEmpty())
		return FALSE;

	if (WantDrawColumnTime(nDate, bCustomWantsTime))
		sTime = m_formatter.GetTimeOnly(date, nDate);
	else
		sTime.Empty();

	if (HasStyle(TDCS_SHOWWEEKDAYINDATES))
		sDow = CDateHelper::GetDayOfWeekName(CDateHelper::GetDayOfWeek(date), TRUE);
	else
		sDow.Empty();

	return TRUE;
}

void CTDCTaskCtrlRenderer::DrawColumnDate(CDC* pDC, const COleDateTime& date, TDC_DATE nDate, const CRect& rect,
									  COLORREF crText, BOOL bCalculated, BOOL bCustomWantsTime, int nAlign)
{
	CString sDate, sTime, sDow;

	if (!FormatDate(date, nDate, sDate, sTime, sDow, bCustomWantsTime))
		return; // nothing to do

	BOOL bHasTime = !sTime.IsEmpty();
	BOOL bHasDow = !sDow.IsEmpty();
	BOOL bTimeIsReal = CDateHelper::DateHasTime(date);

  	ASSERT((nDate != TDCD_REMINDER) || HasStyle(TDCS_SHOWREMINDERSASDATEANDTIME));
	ASSERT((nDate != TDCD_REMINDER) || bHasTime);

	// Because the column width can be changed by the user we need
	// a strategy for deciding what to cull as the width reduces
	// +--------+-----------------------+-----------------------+
	// |   DoW  |        Date           |        Time           |
	// +--------+-----------------------+-----------------------+

	m_dateTimeWidths.Initialise(pDC);

	int nSpace = pDC->GetTextExtent(_T(" ")).cx;

	int nMaxDateWidth = (m_dateTimeWidths.nMaxDateWidth + nSpace);
	int nMinDateWidth = (m_dateTimeWidths.nMinDateWidth + nSpace);
	int nMaxTimeWidth = (bHasTime ? (m_dateTimeWidths.nMaxTimeWidth + nSpace) : 0);
	int nMaxDayWidth = (bHasDow ? (m_dateTimeWidths.nMaxDowNameWidth + nSpace) : 0);

	// Work out what we can draw
	BOOL bDrawDate = FALSE;
	BOOL bDrawTime = FALSE;
	BOOL bDrawDow = FALSE;

	const int nAvailWidth = (rect.Width() - LV_COLPADDING);
	int nReqWidth = 0;

	do // easy exit
	{
		// Enough room for everything 
		nReqWidth = (nMaxDayWidth + nMaxDateWidth + nMaxTimeWidth);

		if (nAvailWidth >= nReqWidth)
		{
			bDrawDow = bHasDow;
			bDrawDate = TRUE;
			bDrawTime = bHasTime;

			break;
		}

		// Sacrifice wide date for narrow date
		nReqWidth = (nMaxDayWidth + nMinDateWidth + nMaxTimeWidth);

		if (nAvailWidth >= nReqWidth)
		{
			bDrawDow = bHasDow;
			bDrawDate = TRUE;
			bDrawTime = bHasTime;

			nMaxDateWidth = nMinDateWidth;
			sDate = m_formatter.GetDateOnly(date, FALSE);

			break;
		}

		// Sacrifice time component if it's not 'real'
		nReqWidth = (nMaxDayWidth + nMinDateWidth);

		if (bHasDow && !bTimeIsReal && (nAvailWidth >= nReqWidth))
		{
			bDrawDow = TRUE;
			bDrawDate = TRUE;
			bDrawTime = FALSE;

			nMaxDateWidth = nMinDateWidth;
			sDate = m_formatter.GetDateOnly(date, FALSE);

			break;
		}

		// Sacrifice the date if it falls within 7 days
		BOOL bWithin7Days = IsDateWithin7DaysOfToday(date, nDate);

		nReqWidth = (nMaxDayWidth + nMaxTimeWidth);

		if (bHasDow && bWithin7Days && (nAvailWidth >= nReqWidth))
		{
			bDrawDow = TRUE;
			bDrawDate = FALSE;
			bDrawTime = bHasTime;

			break;
		}

		// Sacrifice day of week
		nReqWidth = (nMinDateWidth + nMaxTimeWidth);

		if (nAvailWidth >= nReqWidth)
		{
			bDrawDow = FALSE;
			bDrawTime = bHasTime;
			bDrawDate = TRUE;

			nMaxDateWidth = nMinDateWidth;
			sDate = m_formatter.GetDateOnly(date, FALSE);

			break;
		}

		// Sacrifice date and day of week if date is today and time is 'real'
		if (bTimeIsReal && CDateHelper::IsToday(date))
		{
			bDrawDow = FALSE;
			bDrawTime = TRUE;
			bDrawDate = FALSE;

			nReqWidth = nMaxTimeWidth;
			break;
		}

		// Sacrifice date and time if date is within 7 days
		if (bHasDow && bWithin7Days)
		{
			bDrawDow = TRUE;
			bDrawTime = FALSE;
			bDrawDate = FALSE;

			nReqWidth = nMaxDayWidth;
			break;
		}

		// else display narrow date
		bDrawDow = FALSE;
		bDrawTime = FALSE;
		bDrawDate = TRUE;

		nMaxDateWidth = nMinDateWidth;
		sDate = m_formatter.GetDateOnly(date, FALSE);

		nReqWidth = nMinDateWidth;
	}
	while (false); // always end

	ASSERT((nReqWidth > 0) && (bDrawDow || bDrawDate || bDrawTime));

	// Draw calculated dates in a lighter colour
	if (bCalculated && !Misc::IsHighContrastActive())
	{
		crText = (HasColor(crText) ? crText : pDC->GetTextColor());
		crText = GraphicsMisc::Lighter(crText, 0.5);
	}

	// We always draw FROM THE RIGHT and with each component 
	// aligned to the right
	CRect rDraw(rect);

	switch (nAlign)
	{
	case DT_LEFT:
		ASSERT(nDate == TDCD_CUSTOM);
		rDraw.right = min(rDraw.right, (rDraw.left + nReqWidth));
		break;

	case DT_RIGHT:
		break;

	case DT_CENTER:
		ASSERT(nDate == TDCD_CUSTOM);
		rDraw.right = min(rDraw.right, (rDraw.CenterPoint().x + (nReqWidth / 2)));
		break;
	}

	// draw time first
	if (bDrawTime)
	{
		ASSERT(!sTime.IsEmpty());

		// if NO time component, render 'default' start and due time 
		// in a lighter colour to indicate it wasn't user-set
		COLORREF crTime(crText);

		if (!bTimeIsReal && !bCalculated && !Misc::IsHighContrastActive())
		{
			// Note: If we've already calculated it above we need not again
			if (!HasColor(crTime))
				crTime = pDC->GetTextColor();

			crTime = GraphicsMisc::Lighter(crTime, 0.5);
		}

		// draw and adjust rect
		DrawColumnText(pDC, sTime, rDraw, DT_RIGHT, crTime);
		rDraw.right -= nMaxTimeWidth;
	}

	if (bDrawDate)
	{
		DrawColumnText(pDC, sDate, rDraw, DT_RIGHT, crText);
		rDraw.right -= nMaxDateWidth;
	}
	
	// Finally day of week
	if (bDrawDow)
	{
		ASSERT(!sDow.IsEmpty());

		DrawColumnText(pDC, sDow, rDraw, DT_RIGHT, crText);
	}
}

BOOL CTDCTaskCtrlRenderer::IsDateWithin7DaysOfToday(const COleDateTime& date, TDC_DATE nDate)
{
	ASSERT(CDateHelper::IsDateSet(date));

	COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);

	switch (nDate)
	{
	case TDCD_CREATE:
	case TDCD_DONE:
	case TDCD_LASTMOD:
		// Previous 6 days
		dtToday.m_dt++;
		return ((date < dtToday) && ((dtToday.m_dt - date.m_dt) < 7));

	case TDCD_START:
	case TDCD_DUE:
	case TDCD_REMINDER:
	case TDCD_CUSTOM:
		// Next 6 days
		return ((date >= dtToday) && ((date.m_dt - dtToday.m_dt) < 7));
	}

	ASSERT(0);
	return FALSE;
}

void CTDCTaskCtrlRenderer::DrawColumnText(CDC* pDC, const CString& sText, const CRect& rect, int nAlign, 
										COLORREF crText, BOOL bTaskTitle, int nTextLen)
{
	ASSERT(crText != CLR_NONE);

	if (sText.IsEmpty())
		return;

	if (nTextLen == -1)
		nTextLen = sText.GetLength();
	
	CRect rText(rect);
	CPoint ptText(0, rText.top);
	
	if (!bTaskTitle)
	{
		switch (nAlign)
		{
		case DT_LEFT:
			rText.left += LV_COLPADDING;
			break;
			
		case DT_RIGHT:
			rText.right -= LV_COLPADDING;
			break;
			
		case DT_CENTER:
			break;
		}
	}
	
	UINT nFlags = (nAlign | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(pDC));

	if (!m_bSavingToImage && bTaskTitle)
		nFlags |= DT_END_ELLIPSIS;

	COLORREF crOld = pDC->SetTextColor(crText);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(sText, nTextLen, rText, nFlags);
	pDC->SetTextColor(crOld);
}

/*
LRESULT CTDCTaskCtrlRenderer::OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD)
{
	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
		
	case CDDS_ITEMPREPAINT:
		{
			// don't draw columns having min width
			CRect rItem(pNMCD->rc);

			if (rItem.Width() > MIN_COL_WIDTH)
				return CDRF_NOTIFYPOSTPAINT;
		}
		break;

	case CDDS_ITEMPOSTPAINT:
		{
			// don't draw columns having min width
			CRect rItem(pNMCD->rc);

			if (rItem.Width() > MIN_COL_WIDTH)
			{
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);
				CFont* pFont = NULL;

				// draw sort direction
				int nCol = (int)pNMCD->dwItemSpec;
				TDC_COLUMN nColID = (TDC_COLUMN)pNMCD->lItemlParam;

				if (nColID == m_nSortColID)
				{
					BOOL bUp = (m_nSortDir == TDC_SORTUP);
					GetColumnHeaderCtrl(nColID).DrawItemSortArrow(pDC, nCol, bUp);
				}

				const TDCCOLUMN* pTDCC = GetColumn(nColID);
				int nAlignment = DT_LEFT;
				
				if (pTDCC)
				{
					nAlignment = pTDCC->GetColumnHeaderAlignment();

					// handle symbol images
					if (pTDCC->iImage != -1)
					{
						CRect rImage(0, 0, COL_ICON_SIZE, COL_ICON_SIZE);
						GraphicsMisc::CentreRect(rImage, rItem, TRUE, TRUE);

 						m_ilColSymbols.Draw(pDC, pTDCC->iImage, rImage.TopLeft(), ILD_TRANSPARENT);
						return CDRF_SKIPDEFAULT;
					}
				}

				// Handle RTL text column headers
				if (GraphicsMisc::GetRTLDrawTextFlags(pNMCD->hdr.hwndFrom) == DT_RTLREADING)
				{
					CString sColumn(GetColumnHeaderCtrl(nColID).GetItemText(nCol));
					DrawColumnText(pDC, sColumn, pNMCD->rc, nAlignment, GetSysColor(COLOR_WINDOWTEXT));
					
					return CDRF_SKIPDEFAULT;
				}
			}
		}
		break;
	}
	
	return CDRF_DODEFAULT;
}
*/

const TDCCOLUMN* CTDCTaskCtrlRenderer::GetColumn(TDC_COLUMN nColID) const
{
	if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		return NULL;

	ASSERT(!m_mapColumns.IsEmpty());

	const TDCCOLUMN* pCol = NULL;
	VERIFY(m_mapColumns.Lookup(nColID, pCol));

	ASSERT(pCol);
	return pCol;
}

BOOL CTDCTaskCtrlRenderer::SetCompletionStatus(const CString& sStatus) 
{ 
	if (sStatus != m_sCompletionStatus)
	{
		m_sCompletionStatus = sStatus; 
		return TRUE;
	}

	return FALSE; // no change
}

CString CTDCTaskCtrlRenderer::FormatTaskDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_DATE nDate) const
{
	COleDateTime date;

	switch (nDate)
	{
	case TDCD_CREATE:	return FormatDate(pTDI->GetDate(nDate), nDate);
	case TDCD_DONE:		return FormatDate(pTDI->GetDate(nDate), nDate);
	case TDCD_START:	return FormatDate(m_calculator.GetTaskStartDate(pTDI, pTDS), nDate);
	case TDCD_DUE:		return FormatDate(m_calculator.GetTaskDueDate(pTDI, pTDS), nDate);
	case TDCD_LASTMOD:	return FormatDate(m_calculator.GetTaskLastModifiedDate(pTDI, pTDS), nDate);

	default:
		ASSERT(0);
		break;
	}

	return EMPTY_STR;
}

CString CTDCTaskCtrlRenderer::FormatDate(const COleDateTime& date, TDC_DATE nDate) const
{
	switch (nDate)
	{
	case TDCD_CREATE:
	case TDCD_DONE:
	case TDCD_START:
	case TDCD_DUE:
	case TDCD_LASTMOD:
	case TDCD_REMINDER:
		{
			CString sDate, sTime, sDow;

			if (FormatDate(date, nDate, sDate, sTime, sDow))
				return (sDow + ' ' + sDate + ' ' + sTime);
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	return EMPTY_STR;
}

CString CTDCTaskCtrlRenderer::GetTaskColumnText(DWORD dwTaskID, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID, BOOL bDrawing) const
{
	if (!pTDS || !pTDI || !dwTaskID || (nColID == TDCC_NONE))
	{
		ASSERT(0);
		return EMPTY_STR;
	}

	switch (nColID)
	{
	case TDCC_CLIENT:		return pTDI->sTitle;
	case TDCC_EXTERNALID:	return pTDI->sExternalID;
	case TDCC_VERSION:		return pTDI->sVersion;
	case TDCC_LASTMODBY:	return pTDI->sLastModifiedBy;
	case TDCC_ALLOCBY:		return pTDI->sAllocBy;
	case TDCC_CREATEDBY:	return pTDI->sCreatedBy;

	case TDCC_POSITION:		return m_formatter.GetTaskPosition(pTDS);
	case TDCC_RISK:			return m_formatter.GetTaskRisk(pTDI, pTDS);
	case TDCC_RECURRENCE:	return m_formatter.GetTaskRecurrence(pTDI);
	case TDCC_RECENTEDIT:	return m_formatter.GetTaskRecentlyModified(pTDI, pTDS);
	case TDCC_COST:			return m_formatter.GetTaskCost(pTDI, pTDS);
	case TDCC_ALLOCTO:		return m_formatter.GetTaskAllocTo(pTDI);
	case TDCC_STATUS:		return m_formatter.GetTaskStatus(pTDI, pTDS, m_sCompletionStatus);
	case TDCC_CATEGORY:		return m_formatter.GetTaskCategories(pTDI);
	case TDCC_TAGS:			return m_formatter.GetTaskTags(pTDI);
	case TDCC_PERCENT:		return m_formatter.GetTaskPercentDone(pTDI, pTDS);
	case TDCC_REMAINING:	return m_formatter.GetTaskTimeRemaining(pTDI, pTDS);
	case TDCC_TIMEEST:		return m_formatter.GetTaskTimeEstimate(pTDI, pTDS);
	case TDCC_TIMESPENT:	return m_formatter.GetTaskTimeSpent(pTDI, pTDS);
	case TDCC_PATH:			return m_formatter.GetTaskPath(pTDI, pTDS);
	case TDCC_SUBTASKDONE:	return m_formatter.GetTaskSubtaskCompletion(pTDI, pTDS);
	case TDCC_COMMENTSSIZE:	return m_formatter.GetTaskCommentSize(pTDI);

	case TDCC_ID:			return m_formatter.GetID(pTDS->GetTaskID(), dwTaskID);
	case TDCC_PARENTID:		return m_formatter.GetID(pTDS->GetParentTaskID());

		// items having no text
	case TDCC_ICON:
	case TDCC_DONE:
	case TDCC_FLAG:
	case TDCC_LOCK:
	case TDCC_TRACKTIME:
		return EMPTY_STR;

		// items rendered differently
	case TDCC_STARTDATE:
	case TDCC_DUEDATE:
	case TDCC_DONEDATE:
	case TDCC_CREATIONDATE:
	case TDCC_LASTMODDATE:
		if (!bDrawing)
			return FormatTaskDate(pTDI, pTDS, TDC::MapColumnToDate(nColID));
		break;

	case TDCC_DEPENDENCY:
		if (!bDrawing)
			return Misc::FormatArray(pTDI->aDependencies, '+');
		break;

	case TDCC_REMINDER:
		if (!bDrawing)
		{
/*
			time_t tRem = GetTaskReminder(m_data.GetTrueTaskID(dwTaskID));

			// Reminder must be set and start/due date must be set
			if ((tRem != 0) && (tRem != -1))
				return FormatDate(COleDateTime(tRem), TDCD_REMINDER);
*/
		}
		break;

	case TDCC_FILEREF:
		if (!bDrawing)
			return Misc::FormatArray(pTDI->aFileLinks, '+');
		break;

	case TDCC_PRIORITY:
		if (!bDrawing || !HasStyle(TDCS_HIDEPRIORITYNUMBER))
			return m_formatter.GetTaskPriority(pTDI, pTDS);
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		{
			if (!bDrawing)
			{
				TDCCUSTOMATTRIBUTEDEFINITION attribDef;

				if (m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef))
				{
					switch (attribDef.GetDataType())
					{
					case TDCCA_BOOL:
					case TDCCA_ICON:
						return EMPTY_STR;
					}

					TDCCADATA data;

					if (pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data))
						return attribDef.FormatData(data, HasStyle(TDCS_SHOWDATESINISO));
				}
			}
			return EMPTY_STR;
		}
		else
		{
			ASSERT(0);
		}
		break;
	}

	return EMPTY_STR;
}

BOOL CTDCTaskCtrlRenderer::SetAlternateLineColor(COLORREF crAltLine)
{
	return SetColor(m_crAltLine, crAltLine);
}

BOOL CTDCTaskCtrlRenderer::SetGridlineColor(COLORREF crGridLine)
{
	return SetColor(m_crGridLine, crGridLine);
}

BOOL CTDCTaskCtrlRenderer::IsColumnShowing(TDC_COLUMN nColID) const
{
	// Some columns are always visible
	if (nColID == TDCC_CLIENT)
	{
		return TRUE;
	}
	else if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
	{
		return m_aCustomAttribDefs.IsColumnEnabled(nColID);
	}

	return m_mapVisibleCols.Has(nColID);
}

BOOL CTDCTaskCtrlRenderer::WantDrawColumnTime(TDC_DATE nDate, BOOL bCustomWantsTime) const
{
	switch (nDate)
	{
	case TDCD_CREATE:
		return IsColumnShowing(TDCC_CREATIONTIME);

	case TDCD_START:		
	case TDCD_STARTDATE:	
	case TDCD_STARTTIME:
		return IsColumnShowing(TDCC_STARTTIME);
		
	case TDCD_DUE:		
	case TDCD_DUEDATE:	
	case TDCD_DUETIME:	
		return IsColumnShowing(TDCC_DUETIME);
		
	case TDCD_DONE:		
	case TDCD_DONEDATE:	
	case TDCD_DONETIME:	
		return IsColumnShowing(TDCC_DONETIME);
		
	case TDCD_CUSTOM:
		return bCustomWantsTime;
		
	case TDCD_LASTMOD:
	case TDCD_REMINDER:
		return TRUE; // always
	}
	
	// all else
	ASSERT(0);
	return FALSE;
}

