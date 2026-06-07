// EisenhowerUIExtensionControls.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EisenhowerFilterSetupListCtrl.h"

#include <Shared\WndPrompt.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace EisenhowerUIExtension;
using namespace Abstractspoon::Tdl::PluginHelpers;

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

const int IDC_COMBO = 1001;

// --------------------------------

CEisenhowerSetupListCtrl::CEisenhowerSetupListCtrl()
{
}

BEGIN_MESSAGE_MAP(CEisenhowerSetupListCtrl, CInputListCtrl)
	ON_WM_CREATE()
	ON_CBN_SELENDOK(IDC_COMBO, OnComboSelChange)
END_MESSAGE_MAP()

int CEisenhowerSetupListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CEisenhowerSetupListCtrl::Initialise(LPCWSTR szXVarColName,
										  LPCWSTR szXCutoffColName,
										  LPCWSTR szYVarColName,
										  LPCWSTR szYCutoffColName,
										  LPCWSTR szNewRowPrompt,
										  LPCWSTR szCutoffPrompt,
										  const CArray<VARIABLE, VARIABLE&>& aVars,
										  const CArray<FILTER, FILTER&>& aFilters)
{
	m_sCutoffPrompt = szCutoffPrompt;

	CRect rClient;
	GetClientRect(rClient);

	AddCol(szXVarColName, (rClient.Width() * 3) / 10, ILCT_COMBO);
	AddCol(szXCutoffColName, (rClient.Width() * 2) / 10, ILCT_TEXT);
	AddCol(szYVarColName, (rClient.Width() * 3) / 10, ILCT_COMBO);
	AddCol(szYCutoffColName, (rClient.Width() * 2) / 10, ILCT_TEXT);

	SetAutoRowPrompt(szNewRowPrompt);
	AutoAdd(TRUE, FALSE);

	// Save these for populating the combobox
	m_aVariables.Copy(aVars);

	// Populate the rows
	m_aFilters.Copy(aFilters);

	for (int nFilter = 0; nFilter < aFilters.GetSize(); nFilter++)
	{
		const FILTER& filter = aFilters[nFilter];

		int nRow = AddRow(m_aVariables[filter.nXVarIndex].sLabel);

		SetItemText(nRow, 1, filter.sXCutoff);
		SetItemText(nRow, 2, m_aVariables[filter.nYVarIndex].sLabel);
		SetItemText(nRow, 3, filter.sYCutoff);

		SetItemData(nRow, nFilter);
	}
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
		switch (nCol)
		{
		case XCUTOFF_COL:
		case YCUTOFF_COL:
			if (sText.IsEmpty())
				CInputListCtrl::DrawCellText(pDC, nItem, nCol, rText, m_sCutoffPrompt, CWndPrompt::GetTextColor(), nDrawTextFlags | DT_RIGHT);
			else
				CInputListCtrl::DrawCellText(pDC, nItem, nCol, rText, sText, crText, nDrawTextFlags | DT_RIGHT);
			return;
		}
	}

	// Variable columns
	CInputListCtrl::DrawCellText(pDC, nItem, nCol, rText, sText, crText, nDrawTextFlags | DT_LEFT);
}

void CEisenhowerSetupListCtrl::PrepareControl(CWnd& ctrl, int nRow, int nCol)
{
	PrepareCombo(nRow, nCol);
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

void CEisenhowerSetupListCtrl::PrepareCombo(int nRow, int nCol)
{
	int nFilter = (IsPrompt(nRow) ? -1 : (int)GetItemData(nRow));
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

	if (nSelVar != -1)
		m_cbVariables.SelectString(-1, m_aVariables[nSelVar].sLabel);
}

void CEisenhowerSetupListCtrl::OnComboSelChange()
{
	int nSelItem = GetCurSel();
	int nSelVar = m_cbVariables.GetCurSel();

	if (nSelItem >= m_aFilters.GetSize())
	{
		FILTER filter;
		filter.nXVarIndex = (int)m_cbVariables.GetItemData(nSelVar);

		m_aFilters.Add(filter);

		SetItemText(nSelItem, m_nCurCol, m_aVariables[filter.nXVarIndex].sLabel);
	}
	else // existing filter
	{
		int nFilter = (int)GetItemData(nSelItem);
		FILTER& filter = m_aFilters[nFilter];

		switch (m_nCurCol)
		{
		case XVAR_COL:
			filter.nXVarIndex = (int)m_cbVariables.GetItemData(nSelVar);
			SetItemText(nSelItem, m_nCurCol, m_aVariables[filter.nXVarIndex].sLabel);
			break;

		case YVAR_COL:
			filter.nYVarIndex = (int)m_cbVariables.GetItemData(nSelVar);
			SetItemText(nSelItem, m_nCurCol, m_aVariables[filter.nYVarIndex].sLabel);
			break;

		default:
			return;
		}
	}

	HideControl(m_cbVariables);
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

void HostedEisenhowerSetupListCtrl::Initialise(LPCWSTR szXVarColName,
											   LPCWSTR szXCutoffColName,
											   LPCWSTR szYVarColName,
											   LPCWSTR szYCutoffColName,
											   LPCWSTR szNewRowPrompt,
											   LPCWSTR szCutoffPrompt,
											   const CArray<VARIABLE, VARIABLE&>& aVars,
											   const CArray<FILTER, FILTER&>& aFilters)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ListCtrl.Initialise(szXVarColName,
						  szXCutoffColName,
						  szYVarColName,
						  szYCutoffColName,
						  szNewRowPrompt,
						  szCutoffPrompt,
						  aVars, 
						  aFilters);
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
	if (m_pMFCInfo == IntPtr::Zero)
		return m_Filters;

	// else
	auto filters = gcnew EisenhowerFilters();

	CArray<FILTER, FILTER&> aFilters;
	int numFilters = ListCtrl(m_pMFCInfo)->GetFilters(aFilters);

	for (int f = 0; f < numFilters; f++)
	{
		const FILTER& filter = aFilters[f];

		if (filter.IsValid())
		{
			auto ef = gcnew EisenhowerFilter();

			ef->XVar = m_Vars[filter.nXVarIndex];
			ef->YVar = m_Vars[filter.nYVarIndex];

			ef->XCutoff = gcnew String(filter.sXCutoff);
			ef->YCutoff = gcnew String(filter.sYCutoff);

			filters->Add(ef);
		}
	}

	return filters;
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
	Control::OnHandleDestroyed(e);

	if (m_pMFCInfo != IntPtr::Zero)
	{
		ListCtrl(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}
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

			if (ef->XVar != nullptr)
				filter.nXVarIndex = m_Vars->IndexOf(ef->XVar);

			if (ef->YVar != nullptr)
				filter.nYVarIndex = m_Vars->IndexOf(ef->YVar);

			filter.sXCutoff = MarshalledString(ef->XCutoff);
			filter.sYCutoff = MarshalledString(ef->YCutoff);

			if (filter.IsValid())
				aFilters.Add(filter);
		}

		ListCtrl(m_pMFCInfo)->Initialise(MarshalledString(m_Trans->Translate(L"'Urgent' Variable", Translator::Type::Header)),
										 MarshalledString(m_Trans->Translate(L"'Urgent' Cutoff", Translator::Type::Header)),
										 MarshalledString(m_Trans->Translate(L"'Important' Variable", Translator::Type::Header)),
										 MarshalledString(m_Trans->Translate(L"'Important' Cutoff", Translator::Type::Header)),
										 MarshalledString(m_Trans->Translate(L"<new filter>", Translator::Type::Text)),
										 MarshalledString(m_Trans->Translate(L"<data midpoint>", Translator::Type::Text)),
										 aVars, 
										 aFilters);
	}
}
