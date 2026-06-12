// EisenhowerUIExtensionControls.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "EisenhowerFilterSetupListCtrl.h"

#include <Shared\WndPrompt.h>
#include <Shared\EnString.h>
#include <Shared\Localizer.h>
#include <Shared\Themed.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace EisenhowerUIExtension;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

enum // Variable ValueType
{
	VAR_UNKNOWN = -1,
	VAR_INTEGER,
	VAR_DECIMAL,
	VAR_BOOLEAN,
	VAR_TIMEPERIOD,
	VAR_DATE,
};

////////////////////////////////////////////////////////////////////////////////////////////////

FILTER::FILTER() : nXVarIndex(-1), nYVarIndex(-1)
{
}

BOOL FILTER::IsValid() const
{
	if ((nXVarIndex == -1) || (nYVarIndex == -1))
		return FALSE;

	if (nXVarIndex == nYVarIndex)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
	XVAR_COL,
	XCUTOFF_COL,
	YVAR_COL,
	YCUTOFF_COL,
};

const int IDC_VARIABLES = 1001;

const UINT WM_ESLCN_EDITCHANGE = RegisterWindowMessage(L"WM_ESLCN_EDITCHANGE");

// --------------------------------

CEisenhowerSetupListCtrl::CEisenhowerSetupListCtrl()
{
}

BEGIN_MESSAGE_MAP(CEisenhowerSetupListCtrl, CInputListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY(NM_CUSTOMDRAW, 0, OnHeaderCustomDraw)
	ON_CBN_CLOSEUP(IDC_VARIABLES, OnVariableComboCloseUp)
END_MESSAGE_MAP()

int CEisenhowerSetupListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CEisenhowerSetupListCtrl::Initialise(const CArray<VARIABLE, VARIABLE&>& aVars,
										  const CArray<FILTER, FILTER&>& aFilters)
{
	CRect rClient;
	GetClientRect(rClient);

	AddCol(CEnString(IDS_VARIABLE_HEADER), ((rClient.Width() * 3) / 10), ILCT_COMBO);
	AddCol(CEnString(IDS_CUTOFF_HEADER), ((rClient.Width() * 2) / 10), ILCT_TEXT);
	AddCol(CEnString(IDS_VARIABLE_HEADER), ((rClient.Width() * 3) / 10), ILCT_COMBO);
	AddCol(CEnString(IDS_CUTOFF_HEADER), ((rClient.Width() * 2) / 10), ILCT_TEXT);

	ShowGrid(TRUE, TRUE);
	AutoAdd(TRUE, FALSE);
	SetAutoRowPrompt(CEnString(IDS_FILTERROW_PROMPT));

	// Save these for populating the combobox
	m_aVariables.Copy(aVars);

	// Populate the rows
	m_aFilters.Copy(aFilters);

	for (int nFilter = 0; nFilter < aFilters.GetSize(); nFilter++)
	{
		const FILTER& filter = aFilters[nFilter];

		int nRow = AddRow(GetVarLabel(filter.nXVarIndex));
		ASSERT(nRow == nFilter);

		if (CanEditCutOff(filter.nXVarIndex))
			SetItemText(nRow, 1, filter.sXCutoff);

		SetItemText(nRow, 2, GetVarLabel(filter.nYVarIndex));

		if (CanEditCutOff(filter.nYVarIndex))
			SetItemText(nRow, 3, filter.sYCutoff);
	}

	GetHeader()->SetRowCount(2);
}

int CEisenhowerSetupListCtrl::GetFilters(CArray<FILTER, FILTER&>& aFilters) const
{
	aFilters.Copy(m_aFilters);
	return aFilters.GetSize();
}

void CEisenhowerSetupListCtrl::DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags)
{
	if (!IsPrompt(nItem))
	{
		CString sCellPrompt = GetCellPrompt(nItem, nCol, sText);

		if (!sCellPrompt.IsEmpty())
		{
			CInputListCtrl::DrawCellText(pDC, nItem, nCol, rText, sCellPrompt, CWndPrompt::GetTextColor(), nDrawTextFlags | DT_CENTER);
			return;
		}
	}

	// All else
	CInputListCtrl::DrawCellText(pDC, nItem, nCol, rText, sText, crText, nDrawTextFlags | DT_LEFT);
}

CString CEisenhowerSetupListCtrl::GetCellPrompt(int nItem, int nCol, const CString& sText) const
{
	if (!IsPrompt(nItem))
	{
		switch (nCol)
		{
		case XVAR_COL:
		case YVAR_COL:
			if (sText.IsEmpty())
				return CEnString(IDS_UNDEFINEDVAR_PROMPT);
			break;

		case XCUTOFF_COL:
			return GetCellPrompt(nItem, nCol, sText, m_aFilters[nItem].nXVarIndex);

		case YCUTOFF_COL:
			return GetCellPrompt(nItem, nCol, sText, m_aFilters[nItem].nYVarIndex);
		}
	}

	return L"";
}

CString CEisenhowerSetupListCtrl::GetCellPrompt(int nItem, int nCol, const CString& sText, int nVar) const
{
	switch (GetVarType(nVar))
	{
	case VAR_BOOLEAN:
		return CEnString(IDS_BOOLCUTOFF_PROMPT);

	default:
		if (sText.IsEmpty())
			return CEnString(IDS_DEFCUTOFF_PROMPT);
		break;
	}

	return L"";
}

int CEisenhowerSetupListCtrl::GetVarType(int nVar) const
{
	if (nVar < 0 || nVar >= m_aVariables.GetSize())
		return VAR_UNKNOWN;

	return m_aVariables[nVar].nType;
}

CString CEisenhowerSetupListCtrl::GetVarLabel(int nVar) const
{
	if (nVar < 0 || nVar >= m_aVariables.GetSize())
		return L"";

	return m_aVariables[nVar].sLabel;
}

BOOL CEisenhowerSetupListCtrl::CanEditCutOff(int nVar) const
{
	switch (GetVarType(nVar))
	{
	case VAR_UNKNOWN:
	case VAR_BOOLEAN:
		return FALSE;
	}

	return TRUE;
}

void CEisenhowerSetupListCtrl::PrepareControl(CWnd& ctrl, int nRow, int nCol)
{
	switch (nCol)
	{
	case XVAR_COL:
	case YVAR_COL:
		PrepareCombo(nRow, nCol);
		break;

	case XCUTOFF_COL:
		{
			ASSERT(CanEditCutOff(m_aFilters[nRow].nXVarIndex));

			if (GetVarType(m_aFilters[nRow].nXVarIndex) == VAR_INTEGER)
				GetEditControl()->SetMask(L"0123456789");
			else
				GetEditControl()->SetMask(L"0123456789.", ME_LOCALIZEDECIMAL);
		}
		break;

	case YCUTOFF_COL:
		{
			ASSERT(CanEditCutOff(m_aFilters[nRow].nXVarIndex));

			if (GetVarType(m_aFilters[nRow].nYVarIndex) == VAR_INTEGER)
				GetEditControl()->SetMask(L"0123456789");
			else
				GetEditControl()->SetMask(L"0123456789.", ME_LOCALIZEDECIMAL);
		}
		break;
	}
}

BOOL CEisenhowerSetupListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (!IsPrompt(nRow))
	{
		const FILTER& filter = m_aFilters[nRow];

		switch (nCol)
		{
		case XVAR_COL:
		case YVAR_COL:
			return TRUE;

		case XCUTOFF_COL:
			return CanEditCutOff(filter.nXVarIndex);

		case YCUTOFF_COL:
			return CanEditCutOff(filter.nYVarIndex);
		}
	}

	// All else
	return CInputListCtrl::CanEditCell(nRow, nCol);
}

void CEisenhowerSetupListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	switch (nCol)
	{
	case XVAR_COL:
	case YVAR_COL:
		ShowControl(m_cbVariables, nItem, nCol, bBtnClick);
		return;
	}

	// default
	return CInputListCtrl::EditCell(nItem, nCol, bBtnClick);
}

BOOL CEisenhowerSetupListCtrl::DeleteSelectedCell()
{
	int nRow = GetCurSel(), nCol = m_nCurCol;

	if (!CInputListCtrl::DeleteSelectedCell())
		return FALSE;

	// Synchronise underlying filter array
	FILTER& filter = m_aFilters[nRow];

	switch (nCol)
	{
		case XVAR_COL:
			m_aFilters.RemoveAt(nRow);
			NotifyEditChange();
			break;

		case YVAR_COL:
			if (filter.nYVarIndex != -1)
			{
				filter.nYVarIndex = -1;
				NotifyEditChange();
			}
			break;

		case XCUTOFF_COL:
			if (!filter.sXCutoff.IsEmpty())
			{
				filter.sXCutoff.Empty();
				NotifyEditChange();
			}
			break;

		case YCUTOFF_COL:
			if (!filter.sYCutoff.IsEmpty())
			{
				filter.sYCutoff.Empty();
				NotifyEditChange();
			}
			break;
	}

	return TRUE;
}

COLORREF CEisenhowerSetupListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (!bSelected && !bDropHighlighted && !IsPrompt(nItem) && !CanEditCell(nItem, nCol))
		return GetSysColor(COLOR_3DFACE);

	// else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

void CEisenhowerSetupListCtrl::PrepareCombo(int nRow, int nCol)
{
	int nFilter = (IsPrompt(nRow) ? -1 : nRow);
	int nVarExclude = -1, nSelVar = -1;

	if ((nFilter != -1) && (nFilter < m_aFilters.GetSize()))
	{
		const FILTER& filter = m_aFilters[nFilter];

		switch (nCol)
		{
		case XVAR_COL:
			nSelVar = filter.nXVarIndex;
			nVarExclude = filter.nYVarIndex;
			break;

		case YVAR_COL:
			nSelVar = filter.nYVarIndex;
			nVarExclude = filter.nXVarIndex;
			break;

		default:
			return;
		}
	}

	if (!m_cbVariables.GetSafeHwnd())
		CreateControl(m_cbVariables, 1001, CBS_DROPDOWNLIST | CBS_SORT);
	else
		m_cbVariables.ResetContent();

	for (int nVar = 0; nVar < m_aVariables.GetSize(); nVar++)
	{
		if (nVar != nVarExclude)
		{
			int nItem = m_cbVariables.AddString(m_aVariables[nVar].sLabel);
			m_cbVariables.SetItemData(nItem, nVar);
		}
	}

	m_cbVariables.SelectString(-1, GetVarLabel(nSelVar));
}

void CEisenhowerSetupListCtrl::OnVariableComboCloseUp()
{
	int nSelItem = GetCurSel();

	int nSelVar = m_cbVariables.GetCurSel();
	int nVar = (int)m_cbVariables.GetItemData(nSelVar);

	if (nSelItem >= m_aFilters.GetSize())
	{
		FILTER filter;
		filter.nXVarIndex = nVar;

		m_aFilters.Add(filter);
		AddRow(GetVarLabel(filter.nXVarIndex));
	}
	else // existing filter
	{
		int nFilter = nSelItem;
		FILTER& filter = m_aFilters[nFilter];

		switch (m_nCurCol)
		{
		case XVAR_COL:
			if (nVar != filter.nXVarIndex)
			{
				filter.nXVarIndex = nVar;
				SetItemText(nSelItem, m_nCurCol, GetVarLabel(nVar));
			}
			break;

		case YVAR_COL:
			if (nVar != filter.nYVarIndex)
			{
				filter.nYVarIndex = nVar;
				SetItemText(nSelItem, m_nCurCol, GetVarLabel(nVar));
			}
			break;

		default:
			return;
		}
	}

	NotifyEditChange();
	HideControl(m_cbVariables);
}

void CEisenhowerSetupListCtrl::NotifyEditChange()
{
	GetParent()->SendMessage(WM_ESLCN_EDITCHANGE);
}

void CEisenhowerSetupListCtrl::OnEndEdit(UINT uIDCtrl, int* pResult)
{
	switch (m_nEditCol)
	{
	case XCUTOFF_COL:
		{
			CString sText;
			GetEditControl()->GetWindowText(sText);

			if (sText != GetItemText(m_nEditItem, m_nEditCol))
			{
				m_aFilters[m_nEditItem].sXCutoff = sText;
				SetItemText(m_nEditItem, m_nEditCol, sText);
				NotifyEditChange();
			}
		}
		break;

	case YCUTOFF_COL:
		{
			CString sText;
			GetEditControl()->GetWindowText(sText);

			if (sText != GetItemText(m_nEditItem, m_nEditCol))
			{
				m_aFilters[m_nEditItem].sYCutoff = sText;
				SetItemText(m_nEditItem, m_nEditCol, sText);
				NotifyEditChange();
			}
			break;
		}
		break;

	default:
		ASSERT(0);
	}
}

void CEisenhowerSetupListCtrl::OnHeaderCustomDraw(NMHDR* pNMHDR, LPARAM* lResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	*lResult = CDRF_DODEFAULT;

	if (m_header.GetItemCount())
	{
		switch (pNMCD->dwDrawStage)
		{
		case CDDS_PREPAINT:
			*lResult = CDRF_NOTIFYITEMDRAW;
			break;

		case CDDS_ITEMPREPAINT:
			{
				// For each column draw its own 'lower' text and 
				// its portion of the double-width 'upper' text
				// because I can find no way around the clipping
				// rectangle assigned to each notification
				int nItem = (int)pNMCD->dwItemSpec;
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);

				// Lower text
				CRect rDraw(pNMCD->rc);
				rDraw.top += (rDraw.Height() / 2);

				DrawHeaderRect(pDC, rDraw, m_header.GetItemText(nItem));

				// Upper text
				switch (nItem)
				{
				case XVAR_COL:
				case XCUTOFF_COL:
					{
						m_header.GetItemRect(XVAR_COL, rDraw);

						rDraw.right += m_header.GetItemWidth(XCUTOFF_COL);
						rDraw.bottom -= (rDraw.Height() / 2);

						DrawHeaderRect(pDC, rDraw, CEnString(IDS_URGENCY_HEADER));
					}
					break;

				case YVAR_COL:
				case YCUTOFF_COL:
					{
						m_header.GetItemRect(YVAR_COL, rDraw);

						rDraw.right += m_header.GetItemWidth(YCUTOFF_COL);
						rDraw.bottom -= (rDraw.Height() / 2);

						DrawHeaderRect(pDC, rDraw, CEnString(IDS_IMPORTANCE_HEADER));
					}
					break;

				default:
					ASSERT(0);
				}
			}
			*lResult = CDRF_SKIPDEFAULT;
			break;
		}
	}
}

void CEisenhowerSetupListCtrl::DrawHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem)
{
	if (CThemed::AreControlsThemed())
	{
		CThemed th;
		VERIFY(th.Open(&m_header, _T("HEADER")));

		th.DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rItem);
	}
	else
	{
		pDC->FillSolidRect(rItem, ::GetSysColor(COLOR_3DFACE));
		pDC->Draw3dRect(rItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
	}

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));

	UINT nFlags = (DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CENTER);
	pDC->DrawText(sItem, (LPRECT)(LPCRECT)rItem, nFlags);
}

////////////////////////////////////////////////////////////////////////////////////////////////

HWND GetHwnd(IntPtr hWnd)
{
	return static_cast<HWND>(hWnd.ToPointer());
}

HFONT GetHfont(IntPtr hFont)
{
	return static_cast<HFONT>(hFont.ToPointer());
}

// ---------------------------------------------

HostedEisenhowerSetupListCtrl::HostedEisenhowerSetupListCtrl(HWND hwndParent)
{
	VERIFY(m_WndOfManagedHandle.Attach(hwndParent));
}

HostedEisenhowerSetupListCtrl* HostedEisenhowerSetupListCtrl::Attach(HWND hwndParent, HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedEisenhowerSetupListCtrl* pCtrl = new HostedEisenhowerSetupListCtrl(hwndParent);

	// Create control to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	DWORD dwFlags = WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_NOSORTHEADER;

	if (!pCtrl->m_WndOfManagedHandle.IsWindowEnabled())
		dwFlags |= WS_DISABLED;

	pCtrl->m_ListCtrl.Create(dwFlags, rClient, &(pCtrl->m_WndOfManagedHandle), 1001);
	pCtrl->m_ListCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	pCtrl->m_ListCtrl.SendMessage(WM_SETFONT, (WPARAM)hFont, 0);

	return pCtrl;
}

void HostedEisenhowerSetupListCtrl::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_ListCtrl.DestroyWindow();

	delete this;
}

void HostedEisenhowerSetupListCtrl::DrawItem(WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ListCtrl.SendMessage(WM_DRAWITEM, wp, lp);
}

void HostedEisenhowerSetupListCtrl::UpdateSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRect rSlider;
	m_WndOfManagedHandle.GetClientRect(rSlider);

	m_ListCtrl.MoveWindow(rSlider);
}

void HostedEisenhowerSetupListCtrl::Initialise(ITransText* pTrans,
											   const CArray<VARIABLE, VARIABLE&>& aVars,
											   const CArray<FILTER, FILTER&>& aFilters)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pTrans && !CLocalizer::IsInitialized())
		CLocalizer::Initialize(pTrans);

	m_ListCtrl.Initialise(aVars, aFilters);
}

int HostedEisenhowerSetupListCtrl::GetFilters(CArray<FILTER, FILTER&>& aFilters)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_ListCtrl.GetFilters(aFilters);
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedEisenhowerSetupListCtrl* ListCtrl(IntPtr ptr)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return (HostedEisenhowerSetupListCtrl*)ptr.ToPointer();
}

// ------------------------------------------------

EisenhowerFilterSetupListCtrl::EisenhowerFilterSetupListCtrl()
	: 
	m_Trans(nullptr),
	m_Filters(nullptr),
	m_ModifiedFilters(nullptr),
	m_Vars(nullptr)
{
} 

void EisenhowerFilterSetupListCtrl::Initialise(Translator^ trans,
											   EisenhowerVariables^ supportedVars,
											   EisenhowerFilters^ filters)
{
	m_Trans = trans;
	m_Vars = supportedVars;
	m_Filters = filters;

	CheckInitListCtrl();
}

EisenhowerFilters^ EisenhowerFilterSetupListCtrl::Filters::get()
{
	// Build on demand only
	if (m_ModifiedFilters == nullptr)
	{
		// Get modified filters
		m_ModifiedFilters = gcnew EisenhowerFilters();

		CArray<FILTER, FILTER&> aFilters;
		int numFilters = ListCtrl(m_pMFCInfo)->GetFilters(aFilters);

		for (int f = 0; f < numFilters; f++)
		{
			const FILTER& filter = aFilters[f];

			auto ef = gcnew EisenhowerFilter();

			if (filter.nXVarIndex != -1)
				ef->XVar = m_Vars[filter.nXVarIndex];

			if (filter.nYVarIndex != -1)
				ef->YVar = m_Vars[filter.nYVarIndex];

			ef->XCutoff = gcnew String(filter.sXCutoff);
			ef->YCutoff = gcnew String(filter.sYCutoff);

			m_ModifiedFilters->Add(ef);
		}
	}

	return m_ModifiedFilters;
}

// ------------------------------------------------

void EisenhowerFilterSetupListCtrl::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedEisenhowerSetupListCtrl::Attach(GetHwnd(Handle), GetHfont(Font->ToHfont())));
	CheckInitListCtrl();
}

void EisenhowerFilterSetupListCtrl::OnHandleDestroyed(EventArgs^ e)
{
	if (m_pMFCInfo != IntPtr::Zero)
	{
		// Clean up
		ListCtrl(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}

	Control::OnHandleDestroyed(e);
}

void EisenhowerFilterSetupListCtrl::OnSizeChanged(EventArgs^ e)
{
	Control::OnSizeChanged(e);

	if (m_pMFCInfo != IntPtr::Zero)
		ListCtrl(m_pMFCInfo)->UpdateSize();
}

void EisenhowerFilterSetupListCtrl::WndProc(Message% m)
{
	Control::WndProc(m);

	if (m_pMFCInfo == IntPtr::Zero)
		return;

	// C# case statements must be const
	if (m.Msg == WM_ESLCN_EDITCHANGE)
	{
		m_ModifiedFilters = nullptr;
		ChangeEvent(this, gcnew EventArgs());

		return;
	}

	switch (m.Msg)
	{
	case WM_DRAWITEM:
		ListCtrl(m_pMFCInfo)->DrawItem(m.WParam.ToInt32(), m.LParam.ToInt32());
		break;
	}
}

void EisenhowerFilterSetupListCtrl::CheckInitListCtrl()
{
	if ((m_pMFCInfo != IntPtr::Zero) &&
		(m_Vars != nullptr)	&&
		(m_Filters != nullptr))
	{
		// Convert managed arrays to MFC equivalents
		CArray<VARIABLE, VARIABLE&> aVars;

		for each (EisenhowerVariable^ ev in m_Vars)
		{
			VARIABLE var;

			var.sLabel = MarshalledString(ev->Attribute->Label);
			var.nType = (int)ev->Type;

			if (ev->Attribute->IsCustom())
				var.sAttribID = MarshalledString(ev->Attribute->CustomAttributeId);
			else
				var.sAttribID = MarshalledString(ev->Attribute->AttributeId.ToString());

			aVars.Add(var);
		}

		CArray<FILTER, FILTER&> aFilters;

		for each (EisenhowerFilter^ ef in m_Filters)
		{
			FILTER filter;

			if (ef->XVar == nullptr)
				filter.nXVarIndex = -1;
			else
				filter.nXVarIndex = m_Vars->IndexOf(ef->XVar);

			if (ef->YVar == nullptr)
				filter.nYVarIndex = -1;
			else
				filter.nYVarIndex = m_Vars->IndexOf(ef->YVar);

			filter.sXCutoff = MarshalledString(ef->XCutoff);
			filter.sYCutoff = MarshalledString(ef->YCutoff);

			aFilters.Add(filter);
		}

		ListCtrl(m_pMFCInfo)->Initialise(m_Trans->GetITransText(),
										 aVars, 
										 aFilters);
	}
}
