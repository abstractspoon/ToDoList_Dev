// ToDoCtrlSplitting.cpp: implementation of the CToDoCtrlSplitting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ToDoCtrlLayout.h"

#include "..\shared\Misc.h"
#include "..\shared\GraphicsMisc.h"
#include "..\shared\AutoFlag.h"

#include "..\interfaces\uitheme.h"
#include "..\interfaces\preferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int SPLITSIZE = GraphicsMisc::ScaleByDPIFactor(6);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlLayout::CToDoCtrlLayout(CWnd* pParent, CWnd* pAttributes, CWnd* pComments)
	:
	m_pParent(pParent),
	m_pAttributes(pAttributes),
	m_pComments(pComments),
	m_nAttribsPos(TDCUIL_RIGHT),
	m_nCommentsPos(TDCUIL_RIGHT),
	m_bAllowStacking(TRUE),
	m_bStackCommentsAbove(TRUE),
	m_bRebuildingLayout(FALSE),
	m_nMaxState(TDCMS_NORMAL),
	m_splitterHorz(0, SSP_HORZ, 30, SPLITSIZE),
	m_splitterVert(0, SSP_VERT, 30, SPLITSIZE),
	m_bFirstLayout(TRUE)
{
	ASSERT(m_pParent);
	ASSERT(m_pAttributes);
	ASSERT(m_pComments);
}

CToDoCtrlLayout::~CToDoCtrlLayout()
{

}

void CToDoCtrlLayout::ExcludeSplitBars(CDC* pDC) const
{
	ExcludeSplitBar(m_splitterHorz, pDC);
	ExcludeSplitBar(m_splitterVert, pDC);
}

void CToDoCtrlLayout::ExcludeSplitBar(const CSimpleSplitter& splitter, CDC* pDC) const
{
	if (splitter.GetSafeHwnd() && splitter.GetPaneCount())
	{
		int nPane = splitter.GetPaneCount() - 1;

		while (nPane--)
		{
			CRect rBar;
			splitter.GetBarRect(nPane, rBar, m_pParent);

			pDC->ExcludeClipRect(rBar);
		}
	}
}

BOOL CToDoCtrlLayout::ModifyLayout(TDC_UILOCATION nAttribsPos,
								   TDC_UILOCATION nCommentsPos,
								   BOOL bAllowStacking,
								   BOOL bStackCommentAbove)
{
	ASSERT(::IsWindow(*m_pParent));
	ASSERT(::IsWindow(*m_pAttributes));
	ASSERT(::IsWindow(*m_pComments));

	// Check for change
	BOOL bRebuild = (m_bFirstLayout || (m_nAttribsPos != nAttribsPos) || (m_nCommentsPos != nCommentsPos));

	if (!bRebuild && (m_nAttribsPos == m_nCommentsPos))
	{
		bRebuild = (Misc::StatesDiffer(m_bAllowStacking, bAllowStacking) ||
					 Misc::StatesDiffer(m_bStackCommentsAbove, bStackCommentAbove));
	}

	m_nAttribsPos = nAttribsPos;
	m_nCommentsPos = nCommentsPos;
	m_bAllowStacking = bAllowStacking;
	m_bStackCommentsAbove = bStackCommentAbove;

	if (bRebuild)
	{
		RebuildLayout(TRUE);

		if (m_bFirstLayout)
		{
			SetDefaultPaneSizes();
			m_bFirstLayout = FALSE;
		}
	}

	return bRebuild;
}

void CToDoCtrlLayout::SetSplitBarColor(COLORREF color)
{
	m_splitterHorz.SetBarColor(color);
	m_splitterVert.SetBarColor(color);
}

void CToDoCtrlLayout::Resize(int cx, int cy)
{
	CRect rect(0, 0, cx, cy);

	if (HasSplitters())
	{
		// Resize the primary splitter only
		if (!ResizeIfRoot(m_splitterHorz, rect))
			ResizeIfRoot(m_splitterVert, rect);
	}
	else
	{
		switch (m_nMaxState)
		{
		case TDCMS_MAXTASKLIST:
			m_pParent->SendMessage(WM_SS_NOTIFYSPLITCHANGE, 0, (LPARAM)(LPCRECT)rect);
			break;

		case TDCMS_NORMAL:
			break;

		case TDCMS_MAXCOMMENTS:
			m_pComments->MoveWindow(rect);
			break;
		}
	}
}

BOOL CToDoCtrlLayout::ResizeIfRoot(CSimpleSplitter& splitter, const CRect& rect) const
{
	if (!m_pParent->GetSafeHwnd() || !splitter.GetSafeHwnd() || (splitter.GetParent() != m_pParent))
		return FALSE;

	CRect rSplitter;
	splitter.GetClientRect(rSplitter);

	if (rect.Size() != rSplitter.Size())
		splitter.MoveWindow(rect);
	else
		splitter.RecalcLayout();

	return TRUE;
}

BOOL CToDoCtrlLayout::IsVisible(TDC_SETFOCUSTO nLocation) const
{
	switch (nLocation)
	{
	case TDCSF_COMMENTS:	return ((m_nMaxState == TDCMS_MAXTASKLIST) ? m_bShowCommentsAlways : TRUE);
	case TDCSF_TASKVIEW:	return (m_nMaxState != TDCMS_MAXCOMMENTS);
	case TDCSF_PROJECTNAME:	return (m_nMaxState == TDCMS_NORMAL);
	case TDCSF_ATTRIBUTES:	return (m_nMaxState == TDCMS_NORMAL);
		break;

	default:
		ASSERT(0);
		break;
	}

	return FALSE;
}

BOOL CToDoCtrlLayout::SetMaximised(TDC_MAXSTATE nState, BOOL bShowCommentsAlways, BOOL bRecalcLayout)
{
	if (m_nMaxState == nState)
	{
		if (!Misc::StatesDiffer(m_bShowCommentsAlways, bShowCommentsAlways))
			return FALSE;

		switch (nState)
		{
		case TDCMS_MAXCOMMENTS:
		case TDCMS_NORMAL:
			// Save state but requires no change to splitters
			m_bShowCommentsAlways = bShowCommentsAlways;
			return FALSE;
		}
	}

	m_bShowCommentsAlways = bShowCommentsAlways;
	m_nMaxState = nState;

	RebuildLayout(bRecalcLayout);

	return TRUE;
}

void CToDoCtrlLayout::RecalcLayout()
{
	if (m_splitterHorz.GetSafeHwnd())
		m_splitterHorz.RecalcLayout();

	if (m_splitterVert.GetSafeHwnd())
		m_splitterVert.RecalcLayout();
}

void CToDoCtrlLayout::RebuildLayout(BOOL bRecalcLayout)
{
	m_splitterHorz.DestroyWindow();
	m_splitterHorz.ClearPanes();

	m_splitterVert.DestroyWindow();
	m_splitterVert.ClearPanes();

	// Scoped to exclude the updates right at the end
	{
		CAutoFlag af(m_bRebuildingLayout, TRUE);

		switch (m_nMaxState)
		{
		case TDCMS_NORMAL:
			{
				switch (m_nCommentsPos)
				{
				case TDCUIL_LEFT: // Comments
					{
						switch (m_nAttribsPos)
						{
						case TDCUIL_LEFT: // Attributes
							if (m_bAllowStacking)
							{
								// .----..----------.      .----..----------.
								// | C  || T        |      | A  || T        |
								// |    ||          |      |    ||          |
								// ·----·|          |  OR  ·----·|          |
								// .----.|          |      .----.|          |
								// | A  ||          |      | C  ||          |
								// |    ||          |      |    ||          |
								// ·----··----------·      ·----··----------·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);
								m_splitterVert.Create(SSP_VERT, &m_splitterHorz, IDC_VERTSPLITTER);

								m_splitterHorz.SetPaneCount(2);
								m_splitterHorz.SetPane(0, &m_splitterVert);
								m_splitterHorz.SetPane(1, NULL); // Tasks

								m_splitterVert.SetPaneCount(2);

								if (m_bStackCommentsAbove)
								{
									m_splitterVert.SetPane(0, m_pComments);
									m_splitterVert.SetPane(1, m_pAttributes);
								}
								else
								{
									m_splitterVert.SetPane(0, m_pAttributes);
									m_splitterVert.SetPane(1, m_pComments);
								}
							}
							else
							{
								// .----..----..----.
								// | C  || A  || T  |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// ·----··----··----·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);

								m_splitterHorz.SetPaneCount(3);
								m_splitterHorz.SetPane(0, m_pComments);
								m_splitterHorz.SetPane(1, m_pAttributes);
								m_splitterHorz.SetPane(2, NULL); // Tasks
							}
							break;

						case TDCUIL_RIGHT: // Attributes
							{
								// .----..----..----.
								// | C  || T  || A  |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// ·----··----··----·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);

								m_splitterHorz.SetPaneCount(3);
								m_splitterHorz.SetPane(0, m_pComments);
								m_splitterHorz.SetPane(1, NULL); // Tasks
								m_splitterHorz.SetPane(2, m_pAttributes);
							}
							break;

						case TDCUIL_BOTTOM: // Attributes
							{
								// .----..---------.
								// | C  || T       |
								// |    ||         |
								// |    |·---------·
								// |    |.---------.
								// |    || A       |
								// |    ||         |
								// ·----··---------·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);
								m_splitterVert.Create(SSP_VERT, &m_splitterHorz, IDC_VERTSPLITTER);

								m_splitterHorz.SetPaneCount(2);
								m_splitterHorz.SetPane(0, m_pComments);
								m_splitterHorz.SetPane(1, &m_splitterVert);

								m_splitterVert.SetPaneCount(2);
								m_splitterVert.SetPane(0, NULL); // Tasks
								m_splitterVert.SetPane(1, m_pAttributes);
							}
							break;
						}
					}
					break;

				case TDCUIL_RIGHT: // Comments
					{
						switch (m_nAttribsPos)
						{
						case TDCUIL_LEFT: // Attributes
							{
								// .----..----..----.
								// | A  || T  || C  |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// ·----··----··----·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);

								m_splitterHorz.SetPaneCount(3);
								m_splitterHorz.SetPane(0, m_pAttributes);
								m_splitterHorz.SetPane(1, NULL); // Tasks
								m_splitterHorz.SetPane(2, m_pComments);
							}
							break;

						case TDCUIL_RIGHT: // Attributes
							if (m_bAllowStacking)
							{
								// .----------..----.      .----------..----.
								// | T        || C  |      | T        || A  |
								// |          ||    |      |          ||    |
								// |          |·----·  OR  |          |·----·
								// |          |.----.      |          |.----.
								// |          || A  |      |          || C  |
								// |          ||    |      |          ||    |
								// ·----------··----·      ·----------··----·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);
								m_splitterVert.Create(SSP_VERT, &m_splitterHorz, IDC_VERTSPLITTER);

								m_splitterHorz.SetPaneCount(2);
								m_splitterHorz.SetPane(0, NULL); // Tasks
								m_splitterHorz.SetPane(1, &m_splitterVert);

								m_splitterVert.SetPaneCount(2);

								if (m_bStackCommentsAbove)
								{
									m_splitterVert.SetPane(0, m_pComments);
									m_splitterVert.SetPane(1, m_pAttributes);
								}
								else
								{
									m_splitterVert.SetPane(0, m_pAttributes);
									m_splitterVert.SetPane(1, m_pComments);
								}
							}
							else
							{
								// .----..----..----.
								// | T  || A  || C  |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// |    ||    ||    |
								// ·----··----··----·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);

								m_splitterHorz.SetPaneCount(3);
								m_splitterHorz.SetPane(0, NULL); // Tasks
								m_splitterHorz.SetPane(1, m_pAttributes);
								m_splitterHorz.SetPane(2, m_pComments);
							}
							break;

						case TDCUIL_BOTTOM: // Attributes
							{
								// .---------..----.
								// | T       || C  |
								// |         ||    |
								// ·---------·|    |
								// .---------.|    |
								// | A       ||    |
								// |         ||    |
								// ·---------··----·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);
								m_splitterVert.Create(SSP_VERT, &m_splitterHorz, IDC_VERTSPLITTER);

								m_splitterHorz.SetPaneCount(2);
								m_splitterHorz.SetPane(0, &m_splitterVert);
								m_splitterHorz.SetPane(1, m_pComments);

								m_splitterVert.SetPaneCount(2);
								m_splitterVert.SetPane(0, NULL); // Tasks
								m_splitterVert.SetPane(1, m_pAttributes);
							}
							break;
						}
					}
					break;

				case TDCUIL_BOTTOM: // Comments
					{
						switch (m_nAttribsPos)
						{
						case TDCUIL_LEFT:
							{
								// .----..---------.
								// | A  || T       |
								// |    ||         |
								// |    |·---------·
								// |    |.---------.
								// |    || C       |
								// |    ||         |
								// ·----··---------·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);
								m_splitterVert.Create(SSP_VERT, &m_splitterHorz, IDC_VERTSPLITTER);

								m_splitterHorz.SetPaneCount(2);
								m_splitterHorz.SetPane(0, m_pAttributes);
								m_splitterHorz.SetPane(1, &m_splitterVert);

								m_splitterVert.SetPaneCount(2);
								m_splitterVert.SetPane(0, NULL); // Tasks
								m_splitterVert.SetPane(1, m_pComments);
							}
							break;

						case TDCUIL_RIGHT:
							{
								// .---------..----.
								// | T       || A  |
								// |         ||    |
								// ·---------·|    |
								// .---------.|    |
								// | C       ||    |
								// |         ||    |
								// ·---------··----·
								m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);
								m_splitterVert.Create(SSP_VERT, &m_splitterHorz, IDC_VERTSPLITTER);

								m_splitterHorz.SetPaneCount(2);
								m_splitterHorz.SetPane(0, &m_splitterVert);
								m_splitterHorz.SetPane(1, m_pAttributes);

								m_splitterVert.SetPaneCount(2);
								m_splitterVert.SetPane(0, NULL); // Tasks
								m_splitterVert.SetPane(1, m_pComments);
							}
							break;

						case TDCUIL_BOTTOM:
							{
								// .-------------.      .-------------.
								// |      T      |      |      T      |
								// |             |      |             |
								// ·-------------·  OR  ·-------------·
								// .-----. .-----.      .-----. .-----.
								// |  C  | |  A  |      |  A  | |  C  |
								// |     | |     |      |     | |     |
								// ·-----· ·-----·      ·-----· ·-----·
								if (m_bAllowStacking)
								{

									m_splitterVert.Create(SSP_VERT, m_pParent, IDC_VERTSPLITTER);
									m_splitterHorz.Create(SSP_HORZ, &m_splitterVert, IDC_HORZSPLITTER);

									m_splitterVert.SetPaneCount(2);
									m_splitterVert.SetPane(0, NULL); // Tasks
									m_splitterVert.SetPane(1, &m_splitterHorz);

									m_splitterHorz.SetPaneCount(2);

									if (m_bStackCommentsAbove)
									{
										m_splitterHorz.SetPane(0, m_pComments);
										m_splitterHorz.SetPane(1, m_pAttributes);
									}
									else
									{
										m_splitterHorz.SetPane(0, m_pAttributes);
										m_splitterHorz.SetPane(1, m_pComments);
									}
								}
								else
								{
									// .-------------.
									// |      T      |
									// ·-------------·
									// .-------------.
									// |      A      |
									// ·-------------·
									// .-------------.
									// |      C      |
									// ·-------------·
									m_splitterVert.Create(SSP_VERT, m_pParent, IDC_VERTSPLITTER);

									m_splitterVert.SetPaneCount(3);
									m_splitterVert.SetPane(0, NULL); // Tasks
									m_splitterVert.SetPane(1, m_pAttributes);
									m_splitterVert.SetPane(2, m_pComments);
								}
							}
							break;
						}
					}
					break;
				}
			}
			break;

		case TDCMS_MAXCOMMENTS:
			{
				// No splitter required
			}
			break;

		case TDCMS_MAXTASKLIST:
			if (m_bShowCommentsAlways)
			{
				switch (m_nCommentsPos)
				{
				case TDCUIL_LEFT:
					{
						// .----..----.
						// | C  || T  |
						// |    ||    |
						// |    ||    |
						// |    ||    |
						// |    ||    |
						// |    ||    |
						// ·----··----·
						m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);
						m_splitterHorz.SetPaneCount(2);
						m_splitterHorz.SetPane(0, m_pComments); // Tasks
						m_splitterHorz.SetPane(1, NULL); // Tasks
					}
					break;

				case TDCUIL_RIGHT:
					{
						// .----..----.
						// | T  || C  |
						// |    ||    |
						// |    ||    |
						// |    ||    |
						// |    ||    |
						// |    ||    |
						// ·----··----·
						m_splitterHorz.Create(SSP_HORZ, m_pParent, IDC_HORZSPLITTER);
						m_splitterHorz.SetPaneCount(2);
						m_splitterHorz.SetPane(0, NULL); // Tasks
						m_splitterHorz.SetPane(1, m_pComments);
					}
					break;

				case TDCUIL_BOTTOM:
					{
						// .-------------.
						// |      T      |
						// ·-------------·
						// .-------------.
						// |      C      |
						// ·-------------·
						m_splitterVert.Create(SSP_VERT, m_pParent, IDC_VERTSPLITTER);
						m_splitterVert.SetPaneCount(2);
						m_splitterVert.SetPane(0, NULL); // Tasks
						m_splitterVert.SetPane(1, m_pComments);
					}
					break;
				}

			}
			else
			{
				// No splitter required
			}
			break;
		}
	}

	if (bRecalcLayout)
		RecalcLayout();
}

void CToDoCtrlLayout::SaveState(CPreferences& prefs, LPCTSTR szKey) const
{
	if (HasSplitters())
	{
		CString sKey = Misc::MakeKey(_T("%s\\SplitState"), szKey);

		switch (m_nMaxState)
		{
		case TDCMS_MAXTASKLIST:
			sKey += _T("\\MaxTasklist");
			break;

		case TDCMS_NORMAL:
			sKey += _T("\\Normal");
			break;
		}

		SaveState(prefs, sKey, _T("Horz"), m_splitterHorz);
		SaveState(prefs, sKey, _T("Vert"), m_splitterVert);
	}
}

void CToDoCtrlLayout::SaveState(CPreferences& prefs, LPCTSTR szKey, LPCTSTR szEntry, const CSimpleSplitter& splitter)
{
	if (!splitter.GetSafeHwnd() || !splitter.GetPaneCount())
	{
		prefs.DeleteProfileEntry(szKey, szEntry);
		return;
	}

	CArray<int, int&> aSizes;
	splitter.GetRelativePaneSizes(aSizes);

	CString sState = Misc::FormatArrayT(aSizes, _T("%d"), ':');
	prefs.WriteProfileString(szKey, szEntry, sState);
}

void CToDoCtrlLayout::LoadState(const CPreferences& prefs, LPCTSTR szKey, BOOL bRecalcLayout)
{
	if (HasSplitters())
	{
		CString sKey = Misc::MakeKey(_T("%s\\SplitState"), szKey);

		switch (m_nMaxState)
		{
		case TDCMS_MAXTASKLIST:
			sKey += _T("\\MaxTasklist");
			break;

		case TDCMS_NORMAL:
			sKey += _T("\\Normal");
			break;
		}

		LoadState(prefs, sKey, _T("Horz"), m_splitterHorz, bRecalcLayout);
		LoadState(prefs, sKey, _T("Vert"), m_splitterVert, bRecalcLayout);
	}
}

void CToDoCtrlLayout::LoadState(const CPreferences& prefs, LPCTSTR szKey, LPCTSTR szEntry, CSimpleSplitter& splitter, BOOL bRecalcLayout)
{
	if (!splitter.GetSafeHwnd() || !splitter.GetPaneCount())
		return;

	CString sState = prefs.GetProfileString(szKey, szEntry);
	
	if (!sState.IsEmpty())
	{
		CStringArray aState;
		int nState = Misc::Split(sState, aState, ':');

		if (nState == splitter.GetPaneCount())
		{
			CArray<int, int&> aSizes;

			while (nState--)
			{
				int nSize = _ttoi(aState[nState]);
				aSizes.InsertAt(0, nSize);
			}

			splitter.SetRelativePaneSizes(aSizes, bRecalcLayout);
		}
	}
}

void CToDoCtrlLayout::SetDefaultPaneSizes()
{
	SetDefaultPaneSizes(m_splitterHorz);
	SetDefaultPaneSizes(m_splitterVert);
}

void CToDoCtrlLayout::SetDefaultPaneSizes(CSimpleSplitter& splitter)
{
	int nNumPanes = splitter.GetPaneCount();

	if (!nNumPanes)
		return;

	int nTasksPane = splitter.FindPane(NULL);

	CArray<int, int&> aSizes;
	aSizes.SetSize(nNumPanes);

	if (nTasksPane == -1)
		ASSERT(nNumPanes == 2);
	else
		aSizes[nTasksPane] = 3; // Tasks

	// The other panes
	switch (nTasksPane)
	{
	case -1:
	case 2:
		aSizes[0] = aSizes[1] = 1;
		break;

	case 0:
	case 1:
		aSizes[1 - nTasksPane] = 1; // item 0 or 1

		if (nNumPanes == 3)
			aSizes[2] = 1;
		break;

	default:
		ASSERT(0);
	}

	splitter.SetRelativePaneSizes(aSizes);
}
