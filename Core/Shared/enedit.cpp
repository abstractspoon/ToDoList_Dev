// enedit.cpp : implementation file
//

#include "stdafx.h"
#include "enedit.h"
#include "themed.h"
#include "dlgunits.h"
#include "graphicsmisc.h"
#include "enbitmapex.h"
#include "AcceleratorString.h"
#include "icon.h"
#include "winclasses.h"
#include "wclassdefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

int MENUSIZE = -1;
const TCHAR MENU_DROPBTN = '6';

/////////////////////////////////////////////////////////////////////////////

CEnEdit::EDITBTN::EDITBTN()
	:
	nID(0),
	nWidth(0),
	bEnabled(TRUE),
	hFont(NULL),
	bChecked(FALSE),
	bDropMenu(FALSE),
	bSymbol(FALSE),
	iImage(-1),
	hIcon(NULL)
{
}

/////////////////////////////////////////////////////////////////////////////
// CEnEdit

IMPLEMENT_DYNAMIC(CEnEdit, CMaskEdit)

CEnEdit::CEnEdit(BOOL bComboStyle, LPCTSTR szMask, DWORD dwFlags) : 
					CMaskEdit(szMask, dwFlags),
					m_bComboStyle(bComboStyle),
					m_bFirstShow(TRUE), 
					m_nButtonDown(-1),
					m_bParentIsCombo(-1),
					m_nBottomBorder(0),
					m_nTopBorder(0)
{
}

CEnEdit::~CEnEdit()
{
	FreeButtonResources();
}


BEGIN_MESSAGE_MAP(CEnEdit, CMaskEdit)
	//{{AFX_MSG_MAP(CEnEdit)
	ON_WM_NCCALCSIZE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_HTHOTCHANGE, OnHotChange)
	ON_REGISTERED_MESSAGE(WM_TTC_TOOLHITTEST, OnToolHitTest)
	ON_WM_ENABLE()
	ON_MESSAGE(EM_SETREADONLY, OnSetReadOnly)
	ON_WM_STYLECHANGED()
	ON_WM_NCPAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnEdit message handlers

BOOL CEnEdit::AddButton(UINT nID, LPCTSTR szCaption, LPCTSTR szTip, int nWidth, LPCTSTR szFont)
{
	return InsertButton(GetButtonCount(), nID, szCaption, szTip, nWidth, szFont, FALSE);
}

BOOL CEnEdit::AddButton(UINT nID, UINT nChar, LPCTSTR szTip, int nWidth, LPCTSTR szFont)
{
	return InsertButton(GetButtonCount(), nID, CString(static_cast<TCHAR>(nChar)), szTip, nWidth, szFont, TRUE);
}

BOOL CEnEdit::AddButton(UINT nID, HICON hIcon, LPCTSTR szTip, int nWidth)
{
	return InsertButton(GetButtonCount(), nID, hIcon, szTip, nWidth);
}

BOOL CEnEdit::InsertButton(int nPos, UINT nID, LPCTSTR szCaption, LPCTSTR szTip, int nWidth, LPCTSTR szFont)
{
	return InsertButton(nPos, nID, szCaption, szTip, nWidth, szFont, FALSE);
}

BOOL CEnEdit::InsertButton(int nPos, UINT nID, UINT nChar, LPCTSTR szTip, int nWidth, LPCTSTR szFont)
{
	return InsertButton(nPos, nID, CString(static_cast<TCHAR>(nChar)), szTip, nWidth, szFont, TRUE);
}

BOOL CEnEdit::InsertButton(int nPos, UINT nID, LPCTSTR szCaption, LPCTSTR szTip, int nWidth, LPCTSTR szFont, BOOL bSymbolFont)
{
	if (nWidth < CALC_BTNWIDTH || !nID)
		return FALSE;

	nPos = max(nPos, 0);
	nPos = min(nPos, GetButtonCount());

	EDITBTN eb;

	eb.nID = nID;
	eb.sCaption = szCaption;
	eb.sTip = szTip;
	eb.nWidth = nWidth;

	if (szFont)
	{
		eb.hFont = GraphicsMisc::CreateFont(szFont, -1, bSymbolFont ? GMFS_SYMBOL : 0);
		eb.bSymbol = bSymbolFont;
	
		if (!eb.hFont)
			return FALSE;
	}

	m_aButtons.InsertAt(nPos, eb);

	// add rect to hot tracker
	m_hotTrack.AddRect(CRect(0, 0, 0, 0));

	if (GetSafeHwnd())
	{
		RecalcBtnHotRects();

		// force WM_NCCALCSIZE
		if (!m_bFirstShow)
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 
	}

	return TRUE;
}

BOOL CEnEdit::InsertButton(int nPos, UINT nID, HICON hIcon, LPCTSTR szTip, int nWidth)
{
	if (!nID || !hIcon)
		return FALSE;
	
	nPos = max(nPos, 0);
	nPos = min(nPos, GetButtonCount());
	
	EDITBTN eb;
	
	eb.nID = nID;
	eb.sTip = szTip;

	if (nWidth != DEF_BTNWIDTH)
		eb.nWidth = GraphicsMisc::ScaleByDPIFactor(nWidth);
	else
		eb.nWidth = (GraphicsMisc::ScaleByDPIFactor(16) + 4); // 2 px padding

	if (m_ilBtns.GetSafeHandle())
	{
		eb.iImage = m_ilBtns.Add(hIcon);

		CIcon iconDisabled(CEnBitmapEx::CreateDisabledIcon(hIcon));
		VERIFY(m_ilDisabledBtns.Add(iconDisabled) == eb.iImage);
	
		ASSERT(m_ilBtns.GetImageCount() == m_ilDisabledBtns.GetImageCount());
	}
	else
	{
		eb.hIcon = ::CopyIcon(hIcon); // for later imagelist creation
	}

	m_aButtons.InsertAt(nPos, eb);
	
	// add rect to hot tracker
	m_hotTrack.AddRect(CRect(0, 0, 0, 0));

	if (GetSafeHwnd())
	{
		RecalcBtnHotRects();
		
		// force WM_NCCALCSIZE
		if (!m_bFirstShow)
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 
	}
	
	return TRUE;
}

BOOL CEnEdit::InitializeImageLists()
{
	if (!m_ilBtns.GetSafeHandle() && !m_ilDisabledBtns.GetSafeHandle())
	{
		int nImageSize = GraphicsMisc::ScaleByDPIFactor(16);

		for (int nBtn = 0; nBtn < m_aButtons.GetSize(); nBtn++)
		{
			EDITBTN& eb = m_aButtons[nBtn];

			if (eb.hIcon)
			{
				if (!m_ilBtns.GetSafeHandle())
				{
					ASSERT(!m_ilDisabledBtns.GetSafeHandle());

					if (!m_ilBtns.Create(nImageSize, nImageSize, ILC_COLOR32 | ILC_MASK, 0, 1) ||
						!m_ilDisabledBtns.Create(nImageSize, nImageSize, ILC_COLOR32 | ILC_MASK, 0, 1))
					{
						return FALSE;
					}
				}

				eb.iImage = m_ilBtns.Add(eb.hIcon);

				CIcon iconDisabled(CEnBitmapEx::CreateDisabledIcon(eb.hIcon));

				VERIFY(m_ilDisabledBtns.Add(iconDisabled) == eb.iImage);
				ASSERT(m_ilBtns.GetImageCount() == m_ilDisabledBtns.GetImageCount());

				// Note: We cleanup icons in the destructor
			}
		}
	}

	return TRUE;
}

BOOL CEnEdit::DeleteButton(UINT nID)
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn == -1)
		return FALSE;

	// Cleanup hIcons
	FreeButtonResources(nBtn);

	m_aButtons.RemoveAt(nBtn);
	m_hotTrack.DeleteRect(nBtn);

	if (GetSafeHwnd() && !m_bFirstShow)
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 

	return TRUE;
}

void CEnEdit::DeleteAllButtons()
{
	FreeButtonResources();

	m_aButtons.RemoveAll();
	m_hotTrack.Reset();

	if (GetSafeHwnd() && !m_bFirstShow)
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 
}

void CEnEdit::SetBorders(int nTop, int nBottom)
{
	nTop = max(nTop, 0);
	nBottom = max(nBottom, 0);

	if (m_nTopBorder != nTop || m_nBottomBorder != nBottom)
	{
		m_nBottomBorder = nBottom;
		m_nTopBorder = nTop;

		// force WM_NCCALCSIZE
		if (GetSafeHwnd())
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 
	}
}

void CEnEdit::FilterToolTipMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd() && CToolTipCtrlEx::WantMessage(pMsg->message))
	{
		m_tooltip.FilterToolTipMessage(pMsg);
	}
}

BOOL CEnEdit::PreTranslateMessage(MSG* pMsg)
{
	FilterToolTipMessage(pMsg);

	// Treat 'return' as a button click for single button controls
	if ((pMsg->message == WM_KEYDOWN) && 
		(pMsg->wParam == VK_RETURN) && 
		IsWindowEnabled() && 
		(GetButtonCount() == 1))
	{ 
		const EDITBTN& btn = m_aButtons[0];
		
		if (btn.bEnabled)
		{
			// call derived class first
			OnBtnClick(btn.nID);
			
			// then parent
			GetParent()->SendMessage(WM_EE_BTNCLICK, GetDlgCtrlID(), btn.nID);
			
			return TRUE;
		}
	}
		
	return CMaskEdit::PreTranslateMessage(pMsg);
}

void CEnEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (!(GetStyle() & ES_MULTILINE))
	{
		if (bCalcValidRects)
		{
			m_bFirstShow = FALSE; // in case we get here before OnNcPaint()
		
			lpncsp->rgrc[0].right -= GetButtonsWidth();
			lpncsp->rgrc[0].top += m_nTopBorder;
			lpncsp->rgrc[0].bottom -= m_nBottomBorder;
		}
	}
	
	CMaskEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CEnEdit::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CMaskEdit::OnNcLButtonDown(nHitTest, point);

	if (IsWindowEnabled())
	{
		int nBtn = ButtonHitTest(point);

		if (nBtn >= 0)
		{
			if (m_aButtons[nBtn].bEnabled)
			{
				SetCapture();
				m_nButtonDown = nBtn;
				
				RedrawButtonByIndex(nBtn);
			}
		}
		else
		{
			SetFocus();
		}
	}
}

void CEnEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CMaskEdit::OnLButtonUp(nFlags, point);

	if (m_nButtonDown == -1)
		return;

	ClientToScreen(&point);
	int nBtnDown = m_nButtonDown;
	int nBtnUp = ButtonHitTest(point);

	// update UI
	ReleaseCapture();
	m_nButtonDown = -1;

	RedrawButtonByIndex(nBtnDown);

	// process
	if (nBtnDown == nBtnUp)
	{
		// call derived class first
		OnBtnClick(m_aButtons[nBtnUp].nID);

		// then parent
		GetParent()->SendMessage(WM_EE_BTNCLICK, GetDlgCtrlID(), m_aButtons[nBtnUp].nID);
		
		RedrawButtonByIndex(nBtnDown);
	}
}

void CEnEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	CMaskEdit::OnMouseMove(nFlags, point);

	if (m_nButtonDown != -1)
	{
		ClientToScreen(&point);

		if (ButtonHitTest(point) != m_nButtonDown)
		{
			ReleaseCapture();

			int nBtnDown = m_nButtonDown;
			m_nButtonDown = -1;

			RedrawButtonByIndex(nBtnDown);
		}
	}
}

void CEnEdit::OnSize(UINT nType, int cx, int cy) 
{
	CMaskEdit::OnSize(nType, cx, cy);

	if (m_bParentIsCombo == -1) // first time init
		m_bParentIsCombo = CWinClasses::IsClass(*GetParent(), WC_COMBOBOX);

	// The only reliable place to initialise tooltips
	InitializeTooltips();
	InitializeImageLists();

	// update tool rects
	RecalcBtnHotRects();
}

void CEnEdit::OnDestroy()
{
	CMaskEdit::OnDestroy();

	m_tooltip.DestroyWindow();
	m_hotTrack.Reset();
	m_ilBtns.DeleteImageList();
	m_ilDisabledBtns.DeleteImageList();
}

BOOL CEnEdit::InitializeTooltips()
{
	// Tooltips don't work when we're part of a combobox
	if (!m_bParentIsCombo)
	{
		if (m_tooltip.GetSafeHwnd())
		{
			return TRUE;
		}
		else if (m_tooltip.Create(this))
		{
			m_tooltip.ModifyStyleEx(0, WS_EX_TRANSPARENT);

			// hot tracking
			if (CThemed().AreControlsThemed())
				m_hotTrack.Initialize(this);

			return TRUE;
		}
	}

	return FALSE;
}

void CEnEdit::RecalcBtnHotRects()
{
	if (!m_hotTrack.IsInitialized())
		return;
	
	int nBtn = GetButtonCount();
	
	while (nBtn--)
	{
		CRect rBtn = GetButtonRectByIndex(nBtn);
		ScreenToClient(rBtn);

		m_hotTrack.UpdateRect(nBtn, rBtn);
	}
}

CRect CEnEdit::GetButtonRectByIndex(int nBtn) const
{
	CRect rButton(0, 0, 0, 0);

	if (nBtn < GetButtonCount())
	{
		int nOffset = 0;
		
		for (int nIndex = 0; nIndex < nBtn; nIndex++)
			nOffset += (GetButtonWidthByIndex(nIndex) + 1);
		
		GetClientRect(rButton);
		
		rButton.left = rButton.right + nOffset;
		rButton.right += nOffset + GetButtonWidthByIndex(nBtn);
		rButton.top -= m_nTopBorder;
		rButton.bottom += m_nBottomBorder;
		
		ClientToScreen(rButton);
	}

	return rButton;
}

void CEnEdit::OnNcPaint() 
{
	if (m_bFirstShow)
	{
		m_bFirstShow = FALSE;
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW); 
	}
	
	// our custom drawing
	{
		CWindowDC dc(this);
		CRect rWindow;
		
		GetWindowRect(rWindow);
		NcPaint(&dc, rWindow);
	}
	
	Default();
}

void CEnEdit::NcPaint(CDC* pDC, const CRect& rWindow)
{
	// Flicker free painting
	CDC dcTemp;
	
	if (dcTemp.CreateCompatibleDC(pDC))
	{
		CRect rButtons = GetButtonsRect();
		CBitmap bmTemp;
		
		if (bmTemp.CreateCompatibleBitmap(pDC, rButtons.Width(), rButtons.Height()))
		{
			// Background
			CBitmap* pOld = dcTemp.SelectObject(&bmTemp);
			dcTemp.FillSolidRect(0, 0, rButtons.Width(), rButtons.Height(), GetSysColor(COLOR_WINDOW));
			
			// Buttons
			CPoint ptCursor(::GetMessagePos());
			int nBtn = GetButtonCount();
			
			while (nBtn--)
				DrawButton(&dcTemp, rButtons, nBtn, ptCursor);

			// blit to screen
			rButtons.OffsetRect(-rWindow.TopLeft());
			pDC->BitBlt(rButtons.left, rButtons.top, rButtons.Width(), rButtons.Height(), &dcTemp, 0, 0, SRCCOPY);
			
			// cleanup
			dcTemp.SelectObject(pOld);
		}
	}
}

int CEnEdit::ButtonHitTest(CPoint ptScreen) const
{
	int nBtn = GetButtonCount();
	
	while (nBtn--)
	{
		if (GetButtonRectByIndex(nBtn).PtInRect(ptScreen))
			return nBtn;
	}

	return -1;
}

int CEnEdit::ButtonHitTest(UINT nID) const
{
	int nBtn = GetButtonCount();
	
	while (nBtn--)
	{
		if (m_aButtons[nBtn].nID == nID)
			return nBtn;
	}

	return -1;
}

BOOL CEnEdit::EnableButton(UINT nID, BOOL bEnable)
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn < 0)
		return FALSE;

	if (m_aButtons[nBtn].bEnabled != bEnable)
	{
		m_aButtons[nBtn].bEnabled = bEnable;
		RedrawButtonByIndex(nBtn);
	}

	return TRUE;
}

void CEnEdit::RedrawButtonByIndex(int nBtn)
{
	if (GetSafeHwnd())
	{
		CWindowDC dc(this);
		CPoint ptCursor(::GetMessagePos());
		
		CRect rWindow;
		GetWindowRect(rWindow);
		
		DrawButton(&dc, rWindow, nBtn, ptCursor);
	}
}

BOOL CEnEdit::SetDropMenuButton(UINT nID, BOOL bDropMenu)
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn < 0)
		return FALSE;

	if (m_aButtons[nBtn].bDropMenu != bDropMenu)
	{
		m_aButtons[nBtn].bDropMenu = bDropMenu;
		RedrawButtonByIndex(nBtn);
	}

	return TRUE;
}

UINT CEnEdit::TrackPopupMenu(UINT nID, CMenu* pMenu, DWORD dwFlags)
{
	if (ButtonHitTest(nID) == -1)
	{
		ASSERT(0);
		return 0;
	}

	ASSERT_VALID(pMenu);

	CRect rButton = GetButtonRect(nID);
	
	TPMPARAMS tpmp;
	tpmp.cbSize = sizeof(TPMPARAMS);
	tpmp.rcExclude = rButton;

	int nX = ((dwFlags & EETPM_RIGHT) ? rButton.right : rButton.left);
	int nY = ((dwFlags & EETPM_RIGHT) ? rButton.top : rButton.bottom);

	UINT nMenuFlags = (TPM_LEFTALIGN | TPM_LEFTBUTTON | ((dwFlags & EETPM_RETURNCMD) ? TPM_RETURNCMD : 0));
	
	return ::TrackPopupMenuEx(pMenu->GetSafeHmenu(), nMenuFlags, nX, nY, ::GetParent(*this), &tpmp);
}

BOOL CEnEdit::CheckButton(UINT nID, BOOL bChecked)
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn < 0)
		return FALSE;

	if (m_aButtons[nBtn].bChecked != bChecked)
	{
		m_aButtons[nBtn].bChecked = bChecked;
		RedrawButtonByIndex(nBtn);
	}

	return TRUE;
}

BOOL CEnEdit::SetButtonTip(UINT nID, LPCTSTR szTip)
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn < 0)
		return FALSE;

	if (m_aButtons[nBtn].sTip != szTip)
	{
		m_aButtons[nBtn].sTip = szTip;
		
		if (m_tooltip.GetSafeHwnd())
			m_tooltip.Activate(FALSE);
	}

	return TRUE;
}

BOOL CEnEdit::SetButtonCaption(UINT nID, LPCTSTR szCaption)
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn < 0)
		return FALSE;

	EDITBTN& eb = m_aButtons[nBtn];
	CAcceleratorString sCaption(szCaption, TRUE); // Remove accelerator

	if (eb.sCaption.Compare(sCaption) != 0)
	{
		eb.sCaption = sCaption;

		// recalc width?
		if (eb.nWidth == CALC_BTNWIDTH)
			RecalcBtnHotRects();

		if (GetSafeHwnd())
		{
			// force WM_NCCALCSIZE
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 
			SendMessage(WM_NCPAINT);
		}
	}

	return TRUE;
}

CRect CEnEdit::GetButtonRect(UINT nID) const
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn >= 0)
		return GetButtonRectByIndex(nBtn);

	return CRect(0, 0, 0, 0);
}

CRect CEnEdit::GetButtonsRect() const
{
	CRect rBtns(0, 0, 0, 0);

	if (GetButtonCount())
	{
		int nWidth = GetButtonsWidth();

		if (nWidth)
		{
			rBtns = GetButtonRectByIndex(0);
			rBtns.right = (rBtns.left + nWidth);
		}
	}

	return rBtns;
}

int CEnEdit::GetButtonsWidth() const
{
	int nWidth = 0;

	if (GetButtonCount())
	{
		for (int nBtn = 0; nBtn < GetButtonCount(); nBtn++)
			nWidth += (GetButtonWidthByIndex(nBtn) + 1);

		// trim extra final pixel
		nWidth--;
	}

	return nWidth;
}

CString CEnEdit::GetButtonTip(UINT nID) const
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn >= 0)
		return m_aButtons[nBtn].sTip;

	return _T("");
}

int CEnEdit::GetButtonCount() const
{
	return m_aButtons.GetSize();
}

UINT CEnEdit::GetButtonID(int nPos) const
{
	if ((nPos < 0) || (nPos >= GetButtonCount()))
	{
		ASSERT(0);
		return 0;
	}

	return m_aButtons[nPos].nID;
}

void CEnEdit::DrawButton(CDC* pDC, const CRect& rWindow, int nBtn, const CPoint& ptCursor)
{
	const EDITBTN& eb = m_aButtons[nBtn];

	CRect rBtn = GetButtonRectByIndex(nBtn);

	if (rBtn.IsRectEmpty())
		return;

	BOOL bThemed = CThemed().AreControlsThemed();
	BOOL bHot = rBtn.PtInRect(ptCursor);
	BOOL bEnabled = (IsWindowEnabled() && eb.bEnabled);
	BOOL bDown = ((m_nButtonDown == nBtn) || eb.bChecked);

	rBtn.OffsetRect(-rWindow.TopLeft());

	// nasty business here because the API function DrawThemeEdge() is not theme aware!
	// and drawing a themed combostyle button will also draw the arrow which we don't want
	if (!m_bComboStyle || bThemed)	// draw as button type (for now)
	{
		UINT nFlags = DFCS_ADJUSTRECT | DFCS_BUTTONPUSH;
		
		// note: we do not take account of ES_READONLY as the effect of this
		// is not deterministic at this level so we assume derived classes or 
		// parents have handled it
		if (!bEnabled)
		{
			nFlags |= DFCS_INACTIVE;
		}
		else if (bDown)
		{
			nFlags |= DFCS_PUSHED;
		}
		else if (bHot)
		{
			nFlags |= DFCS_HOT;
		}

		// clip the drawing rect to prevent window getting the parent bkgnd color wrong
		CRect rClip(rBtn);

		if (bThemed)
			rBtn.InflateRect(1, 1);
		
		// for now
		CThemed::DrawFrameControl(this, pDC, rBtn, DFC_BUTTON, nFlags, rClip);
	}
	else // unthemed combo style
	{
		if (bEnabled && bDown)
		{
			pDC->DrawEdge(rBtn, BDR_RAISEDOUTER, BF_RECT | BF_MIDDLE | BF_FLAT);
		}
		else
		{
			pDC->DrawEdge(rBtn, BDR_RAISEDOUTER, BF_ADJUST | BF_RECT | BF_MIDDLE);
			pDC->DrawEdge(rBtn, BDR_RAISEDINNER, BF_RECT | BF_MIDDLE);
		}
	}

	// drop menu arrow
	if (eb.bDropMenu)
	{
		CRect rArrow(rBtn);
		UINT nFlags = DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP;

		if (eb.sCaption.IsEmpty())
		{
			nFlags |= DT_CENTER;
		}
		else
		{
			nFlags |= DT_RIGHT;
			rArrow.right -= 2;
		}

		pDC->SetTextColor(GetSysColor(bEnabled ? COLOR_BTNTEXT : COLOR_3DSHADOW));
		GraphicsMisc::DrawAnsiSymbol(pDC, MENU_DROPBTN, rArrow, nFlags, &GraphicsMisc::Marlett());
	}
	
	// draw caption/image
	if (eb.iImage != -1)
	{
		int nImageSize = m_ilBtns.GetImageSize();

		CRect rDraw(0, 0, nImageSize, nImageSize);
		GraphicsMisc::CentreRect(rDraw, rBtn, TRUE, TRUE);

		if (bEnabled)
			m_ilBtns.Draw(pDC, eb.iImage, rDraw.TopLeft(), ILD_TRANSPARENT);
		else
			m_ilDisabledBtns.Draw(pDC, eb.iImage, rDraw.TopLeft(), ILD_TRANSPARENT);
	}
	else if (!eb.sCaption.IsEmpty())
	{
		CFont* pOld = NULL;
		
		if (eb.hFont)
		{
			pOld = pDC->SelectObject(CFont::FromHandle(eb.hFont));
		}
		else
		{
			CFont* pFont = GetFont();
			pOld = (CFont*)pDC->SelectObject(pFont);
		}
		
		pDC->SetTextAlign((eb.bSymbol ? TA_LEFT : TA_CENTER) | TA_TOP);
		pDC->SetBkMode(TRANSPARENT);
		
		int nCharHeight = pDC->GetTextExtent("A").cy;
		int nVOffset = ((rBtn.Height() - nCharHeight + 1) / 2) - (bThemed ? 0 : 1);
		int nHOffset = eb.bSymbol ? -2 : (rBtn.Width() + 1) / 2;

		// if the button has a drop menu position the text in the 
		// center of what remains
		if (eb.bDropMenu)
			nHOffset = (rBtn.Width() - MENUSIZE + 4) / 2; 

		CPoint ptText(rBtn.left + nHOffset, rBtn.top + nVOffset);
		DrawEnabledText(pDC, ptText, eb.sCaption, rBtn, bEnabled, eb.bSymbol);

		// cleanup
		if (pOld)
			pDC->SelectObject(pOld);
	}

	pDC->ExcludeClipRect(rBtn);
}

void CEnEdit::DrawEnabledText(CDC* pDC, const CPoint& ptTopLeft, const CString& sText, 
						const CRect& rect, BOOL bEnabled, BOOL bSymbol)
{
	if (bEnabled)
	{
		pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
		DrawText(pDC, ptTopLeft, sText, rect, bSymbol);
	}
	else // disabled
	{
		// draw embossed: dark over pale
		CPoint ptText(ptTopLeft);

		ptText.Offset(1, 1);
		pDC->SetTextColor(GetSysColor(COLOR_3DHIGHLIGHT));
		DrawText(pDC, ptText, sText, rect, bSymbol);
		
		ptText.Offset(-1, -1);
		pDC->SetTextColor(GetSysColor(COLOR_3DSHADOW));
		DrawText(pDC, ptText, sText, rect, bSymbol);
	}
}

void CEnEdit::DrawText(CDC* pDC, const CPoint& ptTopLeft, const CString& sText, const CRect& rect, BOOL bSymbol)
{
	if (bSymbol)
	{
		CRect rSymbol(rect);
		rSymbol.left = ptTopLeft.x + 3;
		rSymbol.top = ptTopLeft.y;

		GraphicsMisc::DrawAnsiSymbol(pDC, (char)sText[0], rSymbol, DT_CENTER | DT_TOP | DT_NOCLIP);
	}
	else
	{
		pDC->ExtTextOut(ptTopLeft.x, ptTopLeft.y, 0, rect, sText, NULL);
	}
}

#if _MSC_VER >= 1400
LRESULT CEnEdit::OnNcHitTest(CPoint point)
#else
UINT CEnEdit::OnNcHitTest(CPoint point)
#endif
{
	int nBtn = ButtonHitTest(point);

	if (nBtn >= 0)
		return HTBORDER;

	return (LRESULT)CMaskEdit::OnNcHitTest(point);
}

LRESULT CEnEdit::OnHotChange(WPARAM wp, LPARAM lp)
{
	LRESULT lr = Default();

	// wp has prev hot rect index
	// lp has new hot rect index
	ASSERT (((int)wp != -1 || (int)lp != -1) && (int)wp != (int)lp);

	if (((int)wp != -1) && m_aButtons[wp].bEnabled)
		RedrawButtonByIndex(wp);

	if (((int)lp != -1) && m_aButtons[lp].bEnabled)
		RedrawButtonByIndex(lp);

	return lr;
}

BOOL CEnEdit::SetButtonWidth(UINT nID, int nWidth)
{
	int nBtn = ButtonHitTest(nID);

	if (nBtn < 0)
		return FALSE;

	if (m_aButtons[nBtn].nWidth != nWidth)
	{
		m_aButtons[nBtn].nWidth = nWidth;

		if (GetSafeHwnd())
		{
			// force WM_NCCALCSIZE
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 

			RecalcBtnHotRects();
			SendMessage(WM_NCPAINT);
		}
	}

	return TRUE;
}

int CEnEdit::GetButtonWidthByIndex(int nBtn) const
{
	if (nBtn < 0 || nBtn >= GetButtonCount() || !GetSafeHwnd())
		return 0;

	const EDITBTN& eb = m_aButtons[nBtn];

	if (eb.nWidth > 0)
		return eb.nWidth;
	
	// else calc
	CWindowDC dc(GetParent());
	CFont* pFont = CFont::FromHandle(eb.hFont ? eb.hFont : (HFONT)GetStockObject(DEFAULT_GUI_FONT));
	CFont* pOldFont = dc.SelectObject(pFont);

	int nWidth = dc.GetTextExtent(eb.sCaption).cx + 6;
	dc.SelectObject(pOldFont);

	if (eb.bDropMenu)
	{
		if (MENUSIZE == -1)
		{
			pOldFont = dc.SelectObject(&GraphicsMisc::Marlett());
			MENUSIZE = dc.GetTextExtent("u").cx - 1;
			dc.SelectObject(pOldFont);
		}

		nWidth += MENUSIZE;
	}

	return nWidth;
}

BOOL CEnEdit::SetButtonWidthDLU(UINT nID, int nDLU)
{
	ASSERT (GetSafeHwnd());

	int nWidth = CDlgUnits(this, TRUE).ToPixelsX(nDLU);
	return SetButtonWidth(nID, nWidth);
}

LRESULT CEnEdit::OnSetReadOnly(WPARAM wp, LPARAM /*lp*/)
{
	LRESULT lr = Default();

	OnSetReadOnly((BOOL)wp);
	ForceRedraw();

	return lr;
}

void CEnEdit::OnEnable(BOOL bEnable) 
{
	CEdit::OnEnable(bEnable);
	
	ForceRedraw();
}

void CEnEdit::ForceRedraw()
{
	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

	if (m_bParentIsCombo)
		GetParent()->Invalidate(TRUE);
}

void CEnEdit::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	if ((nStyleType == GWL_STYLE) && 
		((lpStyleStruct->styleOld & ES_READONLY) != (lpStyleStruct->styleNew & ES_READONLY)))
	{
		Default();
	
		OnSetReadOnly((lpStyleStruct->styleNew & ES_READONLY));
		ForceRedraw();
	}
}

LRESULT CEnEdit::OnToolHitTest(WPARAM wp, LPARAM lp)
{
	CPoint pt(wp);
	TOOLINFO* pTI = (TOOLINFO*)lp;

	return OnToolHitTest(pt, pTI);
}

int CEnEdit::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ClientToScreen(&point);

	int nBtn = ButtonHitTest(point);

	if (nBtn != -1)
	{
		const EDITBTN& btn = m_aButtons[nBtn];

		if (!btn.sTip.IsEmpty())
		{
			CRect rBounds;
			GetWindowRect(rBounds);
			ScreenToClient(rBounds);

			return CToolTipCtrlEx::SetToolInfo(*pTI, this, btn.sTip, (nBtn + 1), rBounds);
		}
	}

	// else
	return CMaskEdit::OnToolHitTest(point, pTI);
}

void CEnEdit::FreeButtonResources()
{
	// cleanup fonts and icons
	int nBtn = GetButtonCount();

	while (nBtn--)
		FreeButtonResources(nBtn);
}

void CEnEdit::FreeButtonResources(int nBtn)
{
	ASSERT((nBtn >= 0) && (nBtn <= GetButtonCount()));

	EDITBTN& btn = m_aButtons[nBtn];

	GraphicsMisc::VerifyDeleteObject(btn.hFont);
	btn.hFont = NULL;

	::DestroyIcon(btn.hIcon);
	btn.hIcon = NULL;
}
