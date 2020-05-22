// RuntimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RuntimeDlg.h"
#include "dlgunits.h"
#include "winstyles.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "richedithelper.h"
#include "dialoghelper.h"
#include "filemisc.h"
#include "misc.h"
#include "graphicsmisc.h"

#include "..\Interfaces\IPreferences.h"

#include <afxpriv.h> // for CDialogTemplate

/////////////////////////////////////////////////////////////////////////////
// private mfc functions we need to access
extern void AFXAPI AfxHookWindowCreate(CWnd* pWnd);
extern BOOL AFXAPI AfxUnhookWindowCreate();

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuntimeDlg

class RTDLGTEMPLATE : public DLGTEMPLATE
{
public:
	RTDLGTEMPLATE(DWORD dwStyle, DWORD dwExStyle, const CRect& rectDLU = CRuntimeDlg::rectAuto)
	{
		style = dwStyle;
		dwExtendedStyle = dwExStyle;
		x = (short)rectDLU.left;
		y = (short)rectDLU.top;
		cx = (short)rectDLU.Width();
		cy = (short)rectDLU.Height();
		
		cdit = 0; // always 0
		wMenu = 0; // always 0
		wClass = 0; // always 0
		nCaption = 0; // always 0
	}
	
	virtual ~RTDLGTEMPLATE()
	{
	}
	
	operator LPCDLGTEMPLATE() { return (LPCDLGTEMPLATE)this; }
	
private:
	WORD wMenu;
	WORD wClass;
	short nCaption;
};

/////////////////////////////////////////////////////////////////////////////

const CRect CRuntimeDlg::rectAuto = CRect(0, 0, 0, 0);
CMapStringToString CRuntimeDlg::s_mapClasses;

CRuntimeDlg::CRuntimeDlg(CWnd* pParentWnd, IPreferences* pPrefs, LPCTSTR szSettingsKey)
: 
	CDialog((LPCTSTR)NULL, pParentWnd),
	m_pPrefs(pPrefs),
	m_sSettingsKey(szSettingsKey), 
	m_hILarge(NULL), 
	m_hISmall(NULL), 
	m_rBordersDLU(0), 
	m_rBorders(0),
	m_hFont(NULL)
{
	ASSERT((!m_pPrefs && m_sSettingsKey.IsEmpty()) || (!m_pPrefs && m_sSettingsKey.IsEmpty()));

	if (!s_mapClasses.GetCount())
		BuildClassMap();
	
	SetBordersDLU(7);
}

CRuntimeDlg::~CRuntimeDlg()
{
}

BEGIN_MESSAGE_MAP(CRuntimeDlg, CDialog)
//{{AFX_MSG_MAP(CRuntimeDlg)
ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRuntimeDlg message handlers

IMPLEMENT_DYNAMIC(CRuntimeDlg, CDialog)

void CRuntimeDlg::OnCancel()
{
	// filter out if we're a child
	if (GetStyle() & WS_CHILD)
		return;
	
	CDialog::OnCancel();
}

void CRuntimeDlg::OnOK()
{
	// filter out if we're a child
	if (GetStyle() & WS_CHILD)
		return;
	
	CDialog::OnOK();
}

int CRuntimeDlg::DoModal(LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, const CRect& rect, CWnd* pParentWnd, UINT nID)
{
	// fail immediately if no controls have been added
	if (!m_lstControls.GetCount())
		return IDCANCEL;
	
	// setup for modal loop and creation
	m_nModalResult = -1;
	m_nFlags |= WF_CONTINUEMODAL;
	
	// disable parent (before creating dialog)
	HWND hWndParent = PreModal();
	AfxUnhookWindowCreate();
	
	BOOL bEnableParent = FALSE;
	if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}
	
	try
	{
		// create modeless dialog
		if (Create(szCaption, dwStyle, dwExStyle, rect, pParentWnd ? pParentWnd : CWnd::FromHandle(hWndParent), nID))
		{
			if (m_nFlags & WF_CONTINUEMODAL)
			{
				// enter modal loop
				DWORD dwFlags = MLF_SHOWONIDLE;
				if (GetStyle() & DS_NOIDLEMSG)
					dwFlags |= MLF_NOIDLEMSG;
				VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
			}
			
			// hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}
	catch(...)
	{
		m_nModalResult = -1;
	}
	
	if (bEnableParent)
		::EnableWindow(hWndParent, TRUE);
	
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);
	
	SavePosition();

	// destroy modal window
	DestroyWindow();
	PostModal();

	// cleanup resources
	GraphicsMisc::VerifyDeleteObject(m_hFont);
	::DestroyIcon(m_hILarge);
	::DestroyIcon(m_hISmall);
	
	return m_nModalResult;
}

BOOL CRuntimeDlg::OnInitDialog() 
{
	// first init borders if set in DLU
	if (!m_rBordersDLU.IsRectNull())
		SetBordersDLU(m_rBordersDLU.left, m_rBordersDLU.top, m_rBordersDLU.right, m_rBordersDLU.bottom);
	
	// make sure we create the controls before calling the base class
	CreateControls();
	
	CDialog::OnInitDialog();
	
	// add icons only if we have no parent
	CWnd* pParent = GetParent();

	if (!pParent)
	{
		CString sAppPath = FileMisc::GetAppFilePath();
		
		if (::ExtractIconEx(sAppPath, 0, &m_hILarge, &m_hISmall, 1))
		{
			SetIcon(m_hILarge ? m_hILarge : m_hISmall, TRUE);
			SetIcon(m_hISmall ? m_hISmall : m_hILarge, FALSE);
		}
	}

	// set font to parent's if poss
	SetFont(CDialogHelper::GetFont(pParent), FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRuntimeDlg::Create(LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, const CRect& rect, CWnd* pParentWnd, UINT nID)
{
	RTDLGTEMPLATE rtDlgTemp(dwStyle, dwExStyle);
	CDialogTemplate dlgTemp(&rtDlgTemp);

	// cache and remove visibility
	BOOL bVisible = (dwStyle & WS_VISIBLE);
	dwStyle &= ~WS_VISIBLE;
	
	// Handle DS_SETFONT
	if (pParentWnd && (dwStyle & DS_SETFONT))
	{
		HFONT hFont = GraphicsMisc::GetFont(*pParentWnd, FALSE);

		if (hFont)
		{
			CString sFont;
			int nSize = GraphicsMisc::GetFontNameAndPointSize(hFont, sFont);

			dlgTemp.SetFont(sFont, (WORD)nSize);
		}
	}
	
	if (dwStyle & WS_CHILD)
	{
		dwStyle |= DS_CONTROL;
		dwExStyle |= WS_EX_CONTROLPARENT;
	}
	
	// create modeless dialog
	AfxHookWindowCreate(this);

	// Save size before WM_INITDIALOG gets sent
	m_sizeInitial = rect.Size();
	
	HGLOBAL hTemplate = dlgTemp.Detach();
	LPCDLGTEMPLATE hDlgTemplate = (DLGTEMPLATE*)GlobalLock(hTemplate);

	BOOL bCreated = CreateDlgIndirect(hDlgTemplate, pParentWnd, NULL);

	GlobalUnlock(hTemplate);
	GlobalFree(hTemplate);

	if (bCreated)
	{
		// notify parent if we're a child window
		if (pParentWnd && (dwStyle & WS_CHILD) && !(dwExStyle & WS_EX_NOPARENTNOTIFY))
			pParentWnd->SendMessage(WM_PARENTNOTIFY, MAKEWPARAM(WM_CREATE, nID), (LPARAM)GetSafeHwnd());

		// restore position
		SetInitialPos(rect, dwStyle);
		
		// set window text
		SetWindowText(szCaption);
		
		// set control id
		SetDlgCtrlID(nID);
		
		PostCreate(); // for derived classes
		
		// reshow?
		if (bVisible)
			ShowWindow(SW_SHOW);
	}
	
	return bCreated;
}

void CRuntimeDlg::SetInitialPos(LPCRECT pRect, DWORD dwStyle)
{
	// size to fit?
	if ((dwStyle & WS_POPUP) && (!pRect || rectAuto.EqualRect(pRect)))
	{
		RestorePosition(m_pPrefs, m_sSettingsKey, (dwStyle & DS_CENTER));
	}
	else if (pRect && !rectAuto.EqualRect(pRect))
	{
		MoveWindow(pRect);
	}

	UpdateWindow();
}

void CRuntimeDlg::SavePosition()
{
	SavePosition(m_pPrefs, m_sSettingsKey);
}

void CRuntimeDlg::SavePosition(IPreferences* pPrefs, LPCTSTR szSettingsKey) const
{
	if (!GetSafeHwnd() || !(GetStyle() && WS_POPUP))
		return;
	
	if (pPrefs && !Misc::IsEmpty(szSettingsKey))
	{
		CRect rect;
		GetWindowRect(rect);
			
		pPrefs->WriteProfileInt(szSettingsKey, _T("left"), rect.left);
		pPrefs->WriteProfileInt(szSettingsKey, _T("right"), rect.right);
		pPrefs->WriteProfileInt(szSettingsKey, _T("top"), rect.top);
		pPrefs->WriteProfileInt(szSettingsKey, _T("bottom"), rect.bottom);
	}
}

void CRuntimeDlg::RestorePosition(const IPreferences* pPrefs, LPCTSTR szSettingsKey, BOOL bCentreFirstTime)
{
	if (!GetSafeHwnd() || !(GetStyle() && WS_POPUP))
		return;

	if (pPrefs && !Misc::IsEmpty(szSettingsKey))
	{
		CRect rect;
		
		rect.left = pPrefs->GetProfileInt(szSettingsKey, _T("left"), rectAuto.left);
		rect.right = pPrefs->GetProfileInt(szSettingsKey, _T("right"), rectAuto.right);
		rect.top = pPrefs->GetProfileInt(szSettingsKey, _T("top"), rectAuto.top);
		rect.bottom = pPrefs->GetProfileInt(szSettingsKey, _T("bottom"), rectAuto.bottom);
		
		if (rect != rectAuto)
		{
			MoveWindow(rect);
			return;
		}
	}

	// all else
	AutoFit();

	if (bCentreFirstTime)
		CenterWindow();
}

void CRuntimeDlg::OnDestroy()
{
	// save position for popup dialogs
	if (GetStyle() & WS_POPUP)
		SavePosition();

	CDialog::OnDestroy();
}

BOOL CRuntimeDlg::DestroyWindow()
{
	// save position for popup dialogs
	if (m_hWnd && (GetStyle() & WS_POPUP))
		SavePosition();

	return CDialog::DestroyWindow();
}

BOOL CRuntimeDlg::ShowWindow(int nCmdShow)
{
	// save/restore position for popup dialogs
	if (GetStyle() & WS_POPUP)
	{
		if (IsWindowVisible() && (nCmdShow == SW_HIDE))
		{
			SavePosition();
		}
		else if (!IsWindowVisible() && ((nCmdShow == SW_SHOW) || (nCmdShow == SW_SHOWDEFAULT)))
		{
			SetInitialPos(NULL, 0);
		}
	}

	return CDialog::ShowWindow(nCmdShow);
}

void CRuntimeDlg::AutoFit()
{
	// iterate all the child controls accumulating the largest bottom right coord
	CRect rClient(0, 0, 0, 0);
	
	CWnd* pCtrl = GetWindow(GW_CHILD);
	
	while (pCtrl)
	{
		CRect rCtrl;
		pCtrl->GetWindowRect(rCtrl);
		ScreenToClient(rCtrl);
		
		rClient.right = max(rClient.right, rCtrl.right);
		rClient.bottom = max(rClient.bottom, rCtrl.bottom);
		
		pCtrl = pCtrl->GetNextWindow();
	}
	
	// add border
	rClient.right += m_rBorders.right;
	rClient.bottom += m_rBorders.bottom;
	
	// calc new window rect
	CRect rWindow;
	GetWindowRect(rWindow);
	CalcWindowRect(rClient);
	
	rClient = CRect(rWindow.TopLeft(), rClient.Size());

	CPoint ptTopLeft = GetInitialPos();

	if (ptTopLeft.x != -1 || ptTopLeft.y != -1)
		rClient.OffsetRect(ptTopLeft - rClient.TopLeft());

	else // match centerpoints of old and new
		rClient.OffsetRect(rWindow.CenterPoint() - rClient.CenterPoint());
	
	MoveWindow(rClient);
}

BOOL CRuntimeDlg::AddControl(LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
							 const CRect& rect, UINT nID)
{
	return AddControl(szClass, szCaption, dwStyle, dwExStyle, rect.left, rect.top, rect.Width(), rect.Height(), nID);
}

BOOL CRuntimeDlg::AddControl(LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
							 int x, int y, int cx, int cy, UINT nID)
{
	if (GetSafeHwnd())
		return (NULL != CreateControl(szClass, szCaption, dwStyle, dwExStyle, x, y, cx, cy, nID, FALSE));
	else
	{
		RTCONTROL rtc(NULL, szClass, szCaption, dwStyle, dwExStyle, CRect(x, y, x + cx, y + cy), nID, FALSE);
		m_lstControls.AddTail(rtc);
	}
	
	return TRUE;
}

BOOL CRuntimeDlg::AddControl(CWnd* pWnd, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
							 const CRect& rect, UINT nID)
{
	return AddControl(pWnd, szCaption, dwStyle, dwExStyle, rect.left, rect.top, rect.Width(), rect.Height(), nID);
}

BOOL CRuntimeDlg::AddControl(CWnd* pWnd, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle,
							 int x, int y, int cx, int cy, UINT nID)
{
	if (GetSafeHwnd())
		return CreateControl(pWnd, szCaption, dwStyle, dwExStyle, x, y, cx, cy, nID, FALSE);
	else
	{
		RTCONTROL rtc(pWnd, NULL, szCaption, dwStyle, dwExStyle, CRect(x, y, x + cx, y + cy), nID, FALSE);
		m_lstControls.AddTail(rtc);
	}
	
	return TRUE;
}

BOOL CRuntimeDlg::AddRCControl(LPCTSTR szRCType, LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, 
							   DWORD dwExStyle, int x, int y, int cx, int cy, UINT nID, UINT nIconID)
{
	CString sClass(szClass);
	
	// get the win32 class name
	if (sClass.IsEmpty() || _tcsicmp(szRCType, _T("CONTROL")) != 0)
	{
		if (!CRCCtrlParser::GetClassName(szRCType, sClass) || sClass.IsEmpty())
			return FALSE;
	}
	
	if (!dwStyle)
		dwStyle = CRCCtrlParser::GetDefaultStyles(sClass);
	
	if (!(dwStyle & WS_NOTVISIBLE))
		dwStyle |= WS_VISIBLE;
	else
		dwStyle &= ~WS_NOTVISIBLE;
	
	if (CRCCtrlParser::CtrlWantsClientEdge(sClass))
		dwExStyle |= WS_EX_CLIENTEDGE;
	
	if (GetSafeHwnd())
		return (NULL != CreateControl(sClass, szCaption, dwStyle, dwExStyle, x, y, cx, cy, nID, TRUE, nIconID));
	else
	{
		RTCONTROL rtc(NULL, sClass, szCaption, dwStyle, dwExStyle, CRect(x, y, x + cx, y + cy), nID, TRUE, nIconID);
		m_lstControls.AddTail(rtc);
	}
	
	return TRUE;
}

BOOL CRuntimeDlg::AddRCControl(CWnd* pWnd, LPCTSTR szCaption, DWORD dwStyle, 
							   DWORD dwExStyle, int x, int y, int cx, int cy, UINT nID, UINT nIconID)
{
	ASSERT_VALID(pWnd);
	CString sClass = CWinClasses::GetClass(*pWnd);
	
	if (!dwStyle)
		dwStyle = CRCCtrlParser::GetDefaultStyles(sClass);
	
	if (!(dwStyle & WS_NOTVISIBLE))
		dwStyle |= WS_VISIBLE;
	else
		dwStyle &= ~WS_NOTVISIBLE;
	
	if (CRCCtrlParser::CtrlWantsClientEdge(sClass))
		dwExStyle |= WS_EX_CLIENTEDGE;
	
	if (GetSafeHwnd())
		return (NULL != CreateControl(pWnd, szCaption, dwStyle, dwExStyle, x, y, cx, cy, nID, TRUE, nIconID));
	else
	{
		RTCONTROL rtc(pWnd, sClass, szCaption, dwStyle, dwExStyle, CRect(x, y, x + cx, y + cy), nID, TRUE, nIconID);
		m_lstControls.AddTail(rtc);
	}
	
	return TRUE;
}

HWND CRuntimeDlg::CreateControl(LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
								int x, int y, int cx, int cy, UINT nID, BOOL bDLU, UINT nIconID)
{
	if (bDLU)
	{
		CDlgUnits dlu(*this);
		
		dlu.ToPixels(x, y);
		dlu.ToPixels(cx, cy);
	}

	// override caption
	CString sCaption = OverrideItemText(nID);

	if (sCaption.IsEmpty())
		sCaption = szCaption;
	
	// validate the topleft position against the borders
	x = max(x, m_rBorders.left);
	y = max(y, m_rBorders.top);
	
	// make reasonably sure control is child
	dwStyle |= WS_CHILD;
	dwStyle &= ~WS_POPUP;
	
	// we don't need to be notified by our own children
	dwExStyle |= WS_EX_NOPARENTNOTIFY;
	
	// if its a richedit then make sure richedit dll is initialized
	HWND hwnd = NULL;
	
	if (CWinClasses::IsRichEditControl(szClass))
		hwnd = CreateRichEdit(szClass, sCaption, dwStyle, dwExStyle, x, y, cx, cy, *this, nID);
	else
		hwnd = ::CreateWindowEx(dwExStyle, szClass, sCaption, dwStyle, x, y, cx, cy, *this, (HMENU)nID, NULL, NULL);
	
	if (hwnd)
	{
		::SendMessage(hwnd, WM_SETFONT, (WPARAM)CDialogHelper::GetFont(this), 0); 

		if (nIconID && _tcsicmp(szClass, _T("static")) == 0)
		{
			HICON hIcon = AfxGetApp()->LoadIcon(nIconID);
			
			if (hIcon)
				::SendMessage(hwnd, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
		}
		else if (!sCaption.IsEmpty())
			::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)sCaption);
	}
	else
		TRACE(_T("CreateWindowEx(%s) failed. GetLastError returned %08X\n"), szClass, GetLastError());
	
	return hwnd;
}

HWND CRuntimeDlg::CreateRichEdit(LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
								 int x, int y, int cx, int cy, HWND hwndParent, UINT nID)
{
	CRichEditHelper::InitRichEdit();
	
	ASSERT (CWinClasses::IsRichEditControl(szClass));
	
	HWND hwnd = ::CreateWindowEx(dwExStyle, szClass, szCaption, dwStyle, x, y, cx, cy, hwndParent, (HMENU)nID, NULL, NULL);
	
	// if it failed and its not richedit10 then try richedit10
	if (!hwnd && !CWinClasses::IsClass(szClass, WC_RICHEDIT))
		hwnd = ::CreateWindowEx(dwExStyle, WC_RICHEDIT, szCaption, dwStyle, x, y, cx, cy, hwndParent, (HMENU)nID, NULL, NULL);
	
	return hwnd;
}

BOOL CRuntimeDlg::CreateControl(CWnd* pWnd, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
								int x, int y, int cx, int cy, UINT nID, BOOL bDLU, UINT nIconID)
{
	ASSERT (!pWnd->GetSafeHwnd());
	
	if (pWnd->GetSafeHwnd())
		return FALSE;
	
	CString sClass = GetControlClassName(pWnd);
	
	if (sClass.IsEmpty())
		return FALSE;
	
	HWND hwnd = CreateControl(sClass, szCaption, dwStyle, dwExStyle, x, y, cx, cy, nID, bDLU, nIconID);
	
	if (hwnd)
		return pWnd->SubclassWindow(hwnd);
	
	// else
	return FALSE;
}


BOOL CRuntimeDlg::CreateControl(const RTCONTROL& rtc)
{
	if (rtc.m_pWnd)
		return CreateControl(rtc.m_pWnd, 
							 rtc.m_sCaption, 
							 rtc.m_dwStyle, 
							 rtc.m_dwExStyle, 
							 rtc.m_rect.left, 
							 rtc.m_rect.top, 
							 rtc.m_rect.Width(), 
							 rtc.m_rect.Height(), 
							 rtc.m_nID, 
							 rtc.m_bDLU, 
							 rtc.m_nIconID);
	
	if (!rtc.m_sClass.IsEmpty())
		return (CreateControl(rtc.m_sClass, 
							  rtc.m_sCaption, 
							  rtc.m_dwStyle, 
							  rtc.m_dwExStyle, 
							  rtc.m_rect.left, 
							  rtc.m_rect.top, 
							  rtc.m_rect.Width(), 
							  rtc.m_rect.Height(), 
							  rtc.m_nID, 
							  rtc.m_bDLU, 
							  rtc.m_nIconID) != NULL);
	
	// else
	return FALSE;
}

void CRuntimeDlg::CreateControls()
{
	ASSERT (GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return;
	
	POSITION pos = m_lstControls.GetHeadPosition();
	
	while (pos)
		CreateControl(m_lstControls.GetNext(pos));
}

void CRuntimeDlg::BuildClassMap()
{
	if (s_mapClasses.GetCount())
		return; // already done
	
	s_mapClasses[_T("CStatic")]         = WC_STATIC;
	s_mapClasses[_T("CButton")]         = WC_BUTTON;
	s_mapClasses[_T("CListBox")]        = WC_LISTBOX;
	s_mapClasses[_T("CComboBox")]       = WC_COMBOBOX;
	s_mapClasses[_T("CEdit")]           = WC_EDIT;
	s_mapClasses[_T("CScrollBar")]      = WC_SCROLLBAR;
	s_mapClasses[_T("CListCtrl")]       = WC_LISTVIEW;
	s_mapClasses[_T("CTreeCtrl")]       = WC_TREEVIEW;
	s_mapClasses[_T("CSpinButtonCtrl")] = WC_SPIN;
	s_mapClasses[_T("CHeaderCtrl")]     = WC_HEADER;
	s_mapClasses[_T("CSliderCtrl")]     = WC_SLIDER;
	s_mapClasses[_T("CProgressCtrl")]   = WC_PROGRESS;
	s_mapClasses[_T("CHotKeyCtrl")]     = WC_HOTKEY;
	s_mapClasses[_T("CToolTipCtrl")]    = WC_TOOLTIPS;
	s_mapClasses[_T("CTabCtrl")]        = WC_TABCONTROL;
	s_mapClasses[_T("CAnimateCtrl")]    = WC_ANIMATE;
	s_mapClasses[_T("CToolBarCtrl")]    = WC_TOOLBAR;
	s_mapClasses[_T("CStatusBarCtrl")]  = WC_STATUSBAR;
	s_mapClasses[_T("CRichEditCtrl")]   = WC_RICHEDIT20;
	s_mapClasses[_T("CIPAddressCtrl")]  = WC_IPADDRESS;
	s_mapClasses[_T("CDateTimeCtrl")]   = WC_DATETIMEPICK;
	s_mapClasses[_T("CMonthCalCtrl")]   = WC_MONTHCAL;
	s_mapClasses[_T("CReBar")]          = WC_REBAR;
	//	s_mapClasses[""] = _T("");
}

void CRuntimeDlg::SetBorders(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rBordersDLU.SetRectEmpty();
	m_rBorders.SetRect(max(0, nLeft), max(0, nTop), max(0, nRight), max(0, nBottom));
}

void CRuntimeDlg::SetBordersDLU(int nLeft, int nTop, int nRight, int nBottom)
{
	if (GetSafeHwnd())
	{
		CDlgUnits dlu(*this);
		
		dlu.ToPixels(nLeft, nTop);
		dlu.ToPixels(nRight, nBottom);
		
		SetBorders(nLeft, nTop, nRight, nBottom);
	}
	else
	{
		m_rBorders.SetRectEmpty();
		m_rBordersDLU.SetRect(max(0, nLeft), max(0, nTop), max(0, nRight), max(0, nBottom));
	}
}

CRect CRuntimeDlg::OffsetCtrl(UINT nCtrlID, int x, int y)
{
	return CDialogHelper::OffsetCtrl(this, nCtrlID, x, y);
}

CRect CRuntimeDlg::GetCtrlRect(UINT nCtrlID) const
{
	return CDialogHelper::GetCtrlRect(this, nCtrlID);
}

CRect CRuntimeDlg::MoveCtrl(UINT nCtrlID, int x, int y)
{
	return CDialogHelper::MoveCtrl(this, nCtrlID, x, y);
}

CRect CRuntimeDlg::ResizeCtrl(UINT nCtrlID, int cx, int cy)
{
	return CDialogHelper::ResizeCtrl(this, nCtrlID, cx, cy);
}

int CRuntimeDlg::AddRCControls(const CString& sRCControls)
{
	CRCCtrlParser rccp(sRCControls);
	
	return rccp.GetRCControls(m_lstControls);
}

BOOL CRuntimeDlg::PreTranslateMessage(MSG* pMsg)
{
	BOOL bChild = (GetStyle() & WS_CHILD);
	BOOL bTab = (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB);
	
	// also filter out what look like accelerator keys 
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam != VK_CONTROL && Misc::IsKeyPressed(VK_CONTROL))
			return FALSE;
	}
	
	return (bChild && !bTab) ? CWnd::PreTranslateMessage(pMsg) : CDialog::PreTranslateMessage(pMsg);
}

void CRuntimeDlg::SetFont(CFont* pFont, BOOL bRedraw)
{
	SetFont(pFont ? (HFONT)pFont->GetSafeHandle() : NULL, bRedraw);
}

void CRuntimeDlg::SetFont(HFONT hFont, BOOL bRedraw)
{
	CDialogHelper::SetFont(this, hFont, bRedraw);
}

int CRuntimeDlg::CalcLinesRequired(const CString& sText, int nWidthDLU)
{
	// iterate the text by segment (\n) and add up the number of lines
	// each segment takes up
	int nStart = 0;
	int nCR = sText.Find('\n', nStart);
	int nLines = 0;

	while (nCR != -1)
	{
		int nSegLenDLU = (nCR - nStart) * 4;

		if (nSegLenDLU)
			nLines += (nSegLenDLU / nWidthDLU) + ((nSegLenDLU % nWidthDLU) ? 1 : 0);
		else
			nLines++; // empty line

		nStart = nCR + 1;
		nCR = sText.Find('\n', nStart);
	}

	// handle last segment
	int nSegLenDLU = (sText.GetLength() - nStart) * 4;
	nLines += (nSegLenDLU / nWidthDLU) + ((nSegLenDLU % nWidthDLU) ? 1 : 0);

	return nLines;
}

void CRuntimeDlg::ShowCtrls(UINT nCtrlIDFrom, UINT nCtrlIDTo, BOOL bShow)
{
	CDialogHelper::ShowCtrls(this, nCtrlIDFrom, nCtrlIDTo, bShow);
}

CString CRuntimeDlg::GetControlClassName(CWnd* pWnd)
{
	ASSERT (pWnd);
	
	if (!pWnd)
		return "";
	
	// if there is no permanent mapping to this CWnd just return GetClassName()
	// but only if it has a window handle attached else it must be a real CWnd
	if (pWnd->GetSafeHwnd() && CWnd::FromHandlePermanent(*pWnd) != pWnd)
		return CWinClasses::GetClass(*pWnd);
	
	CRuntimeClass* pRTClass = pWnd->GetRuntimeClass();
	
	// work our way up the derivation chain till we find a match
	while (pRTClass)
	{
		CString sWinClass, sRTClass = pRTClass->m_lpszClassName;
		
		if (s_mapClasses.Lookup(sRTClass, sWinClass))
			return sWinClass;
		
		// try ancestor
#ifdef _AFXDLL
		pRTClass = (*pRTClass->m_pfnGetBaseClass)();
#else
		pRTClass = pRTClass->m_pBaseClass;
#endif
	}
	
	// means we did not find anything
	ASSERT (0);
	return "";
}
