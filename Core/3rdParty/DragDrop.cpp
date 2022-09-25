////////////////////////////////////////////////////////////////
// MSDN Magazine -- October 2004
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET 2003 on Windows XP. Tab size=3.
//
// heavily modded by .dan.g.
//
#include "stdafx.h"
#include "DragDrop.h"

#include <windowsx.h> // for GET_X_LPARAM
#include <afxpriv.h> // for AfxLoadString

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Macro to get point from WM_ mouse messages
#define GETPOINT(lp) (CPoint(GET_X_LPARAM(lp), GET_Y_LPARAM(lp)))

CDragDropMgr::CDragDropMgr() : m_hInstOle32(NULL)
{
	m_pMainWnd = NULL;
	m_hwndTracking = NULL;
	m_hCursorSave = NULL;
	m_pDragImage = NULL;
	m_iState = NONE;

	memset(&m_ddi,0,sizeof(m_ddi));
}

CDragDropMgr::~CDragDropMgr()
{
}

BOOL CDragDropMgr::Install(CWnd *pMainWnd, DRAGDROPWND* ddwnds)
{
	m_pMainWnd = pMainWnd;

	for (int i=0; ddwnds[i].type; i++) 
    {
		HWND hwnd = ::GetDlgItem(pMainWnd->m_hWnd, ddwnds[i].id);
        ASSERT(hwnd && ::IsWindow(hwnd));

		m_mapHwnd[hwnd] = ddwnds[i].type;
	}

	return TRUE;
}

BOOL CDragDropMgr::Install(CWnd *pMainWnd, HWND hwnd, int type)
{
	m_pMainWnd = pMainWnd;
	m_mapHwnd[hwnd] = type;

	return TRUE;
}

void CDragDropMgr::AddWindow(HWND hwnd, int type)
{
    ASSERT(hwnd && ::IsWindow(hwnd));
    
	m_mapHwnd[hwnd] = type;
}

void CDragDropMgr::RemoveWindow(HWND hwnd)
{
    m_mapHwnd.RemoveKey(hwnd); 
}

UINT CDragDropMgr::GetWindowType(HWND hwnd)
{
    UINT nType = DDW_NONE;
    m_mapHwnd.Lookup(hwnd, nType);

    return nType;
}

//////////////////
// detail: Note that this works for input only
//
UINT CDragDropMgr::ProcessMessage(const MSG* pMsg, BOOL bAllowNcDrag)
{
    if (!m_pMainWnd)
        return FALSE;

	const MSG& msg = *pMsg;

	if (IsSourceWnd(msg.hwnd)) 
    {
		BOOL bAbort = FALSE;

		switch (msg.message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			return OnButtonDown(msg);
			
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			if (bAllowNcDrag) 
				return OnButtonDown(msg);
			break;

		case WM_MOUSEMOVE:
			return OnMouseMove(msg);

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			return OnButtonUp(msg);

		case WM_KEYDOWN:
			bAbort = ((m_iState != NONE) && (msg.wParam == VK_ESCAPE));
			break;

		case WM_CONTEXTMENU:
		case WM_KILLFOCUS:
			bAbort = (m_iState != NONE);
			break;

		case WM_ENABLE:
			bAbort = ((m_iState != NONE) && (pMsg->wParam == FALSE));
			break;
		}

		if (bAbort)
		{
			SendDragMessage(WM_DD_DRAGABORT);
			SetState(NONE);

			return TRUE;
		}
	}

	// not handled
	return FALSE;
}

//////////////////
// Handle button-down message: enter CAPTURED state.
//
BOOL CDragDropMgr::OnButtonDown(const MSG& msg)
{
	ASSERT (IsSourceWnd(msg.hwnd));

	// if we're already dragging then this must be a click with another mouse button
	// so we just abort
	if (IsDragging())
	{
		SendDragMessage(WM_DD_DRAGABORT);
		SetState(NONE);

		return TRUE;
	}
	
	// convert m_ptOrg to client coords
	if (msg.message == WM_NCLBUTTONDOWN || msg.message == WM_NCRBUTTONDOWN)
	{
		// check we're not on a scrollbar
		UINT nHitTest = SendMessage(msg.hwnd, WM_NCHITTEST, 0, msg.lParam);
		
		if (nHitTest == HTHSCROLL || nHitTest == HTVSCROLL)
			return FALSE;
		
		CPoint pt = GETPOINT(msg.lParam);
		::ScreenToClient(msg.hwnd, &pt);
		
		// or above or below the client area
		CRect rClient;
		::GetClientRect(msg.hwnd, rClient);
		
		if (pt.y < rClient.top || pt.y > rClient.bottom)
			return FALSE;
		
		m_ptOrg = pt;
	}
	else
	{
		m_ptOrg = GETPOINT(msg.lParam);
	}
	
	m_hwndTracking = msg.hwnd;
	SetState(CAPTURED);
	
	return FALSE;
}

//////////////////
// Handle mousemove: enter DRAGGING state.
//
BOOL CDragDropMgr::OnMouseMove(const MSG& msg)
{
	if (!IsCaptured())
		return FALSE;

	BOOL bLButtonDown = (GetKeyState(VK_LBUTTON) & 0x8000);
	BOOL bRButtonDown = (GetKeyState(VK_RBUTTON) & 0x8000);

	// check mouse button is still down
	if (!bLButtonDown && !bRButtonDown)
	{
		SetState(NONE); 
		return FALSE;
	}

	CWnd* pWnd = CWnd::FromHandle(m_hwndTracking);
	CPoint pt(msg.lParam);

	if (IsDragging()) 
    {
		// already dragging: move drag image
		pWnd->ClientToScreen(&pt); // convert to screen coords

		m_ddi.pt = pt;

		// figure out the deepest child window at this position
		// note: WindowFromPoint does not seem to work for a
		// 32-bit app running on 64-bit windows so instead we 
		// loop using ChildWindowFromPoint
		HWND hwndTarget = *m_pMainWnd;
		
		while (hwndTarget)
		{
			m_ddi.pt = pt;
			::ScreenToClient(hwndTarget, &m_ddi.pt);

			HWND hwndChild = ::ChildWindowFromPoint(hwndTarget, m_ddi.pt);

			if ((hwndChild == NULL) || (hwndChild == hwndTarget))
				break;

			hwndTarget = hwndChild; // keep going
		}
		ASSERT(hwndTarget && ::IsChild(*m_pMainWnd, hwndTarget));

		if (hwndTarget == NULL || !::IsChild(*m_pMainWnd, hwndTarget))
		{
			m_ddi.pt.x = m_ddi.pt.y = -10000; // outer space
		}
		
		m_ddi.hwndTarget = hwndTarget;

		// m_ddi.pt is current pos in client coords of current target window
		SendDragMessage(WM_DD_PREDRAGMOVE);

		// now move drag image
		m_pDragImage->DragMove(pt);

		// update ui
		UINT nDropEffect = SendDragMessage(WM_DD_DRAGOVER);

		// set the cursor
        if (!(m_ddi.hwndTarget && IsTargetWnd(m_ddi.hwndTarget)))
			nDropEffect = DROPEFFECT_NONE;

		HCURSOR hCur = NULL;

		if (!m_hInstOle32)
			m_hInstOle32 = LoadLibrary(_T("Ole32.dll"));

		switch (nDropEffect)
		{
		case DD_DROPEFFECT_COPY:
			hCur = LoadCursor(m_hInstOle32, MAKEINTRESOURCE(3));
			break;

		case DD_DROPEFFECT_MOVE:
			hCur = LoadCursor(m_hInstOle32, MAKEINTRESOURCE(2));
			break;

		case DD_DROPEFFECT_LINK:
			hCur = LoadCursor(m_hInstOle32, MAKEINTRESOURCE(4));
			break;

		case DD_DROPEFFECT_NONE:
		default:
			hCur = LoadCursor(m_hInstOle32, MAKEINTRESOURCE(1));
			break;
		}

		if (hCur)
			SetCursor(hCur);
	} 
    else // if (IsCaptured())
    {
		// Not dragging yet: enter drag mode if mouse moves beyond threshhold.
		CPoint delta = pt - m_ptOrg;
		static CPoint jog(GetSystemMetrics(SM_CXDRAG), GetSystemMetrics(SM_CYDRAG));

		if (abs(delta.x) > jog.x || abs(delta.y) > jog.y) 
        {
			m_ddi.hwndSource = m_hwndTracking;
			m_ddi.pt = m_ptOrg;	// start from ORIGINAL point, not where now
			m_ddi.hwndTarget = m_hwndTracking;
			m_ddi.pData = NULL;
			m_ddi.bLeftDrag = bLButtonDown;

			// Send main window a message: enter drag mode. 
			BOOL bDrag = SendDragMessage(WM_DD_DRAGENTER);

			if (bDrag && m_ddi.pData) 
            {
				SetState(bLButtonDown ? LDRAGGING : RDRAGGING);	// I am now dragging
				OnMouseMove(msg);

				CSize sizeImage;
				m_pDragImage = m_ddi.pData->CreateDragImage(pWnd, sizeImage);
				m_pDragImage->BeginDrag(0, CPoint(sizeImage.cx, sizeImage.cy));

				pWnd->ClientToScreen(&pt);
				m_pDragImage->DragEnter(NULL, pt);
			} 
            else
                SetState(NONE);
		}
	}
	return TRUE;
}

UINT CDragDropMgr::SendDragMessage(UINT nMessage)
{
	UINT nCtrlID = ::GetDlgCtrlID(m_ddi.hwndTarget);

	if (nMessage == WM_DD_DRAGABORT)
		return m_pMainWnd->SendMessage(nMessage, nCtrlID, 0);

	if (nMessage == WM_DD_DRAGOVER)
	{
		DragShowNolock(FALSE);

		UINT nRes = m_pMainWnd->SendMessage(nMessage, nCtrlID, (LPARAM)&m_ddi);

		DragShowNolock(TRUE);
		return nRes;
	}

	// all else
	return m_pMainWnd->SendMessage(nMessage, nCtrlID, (LPARAM)&m_ddi);
}

void CDragDropMgr::DragShowNolock(BOOL bShow)
{
	if (m_pDragImage)
		m_pDragImage->DragShowNolock(bShow);
}

//////////////////
// Handle button-up: drop the data and return to home state (NONE).
//
BOOL CDragDropMgr::OnButtonUp(const MSG& msg)
{
	if (!IsDragging()) 
    {
		SetState(NONE); 
		return FALSE;
	}

	if (IsTargetWnd(m_ddi.hwndTarget)) 
    {
		CPoint pt = GETPOINT(msg.lParam);

		CWnd* pWndSource = CWnd::FromHandle(m_ddi.hwndSource);
		CWnd* pWndTarget = CWnd::FromHandle(m_ddi.hwndTarget);

		pWndSource->ClientToScreen(&pt);
		pWndTarget->ScreenToClient(&pt);
		m_ddi.pt = pt;

		SendDragMessage(WM_DD_DRAGDROP);
	} 
    else 
    {
		SendDragMessage(WM_DD_DRAGABORT);
	}

	SetState(NONE);
    
	return TRUE;
}

//////////////////
// Change state for finite-state-machine.
//
void CDragDropMgr::SetState(UINT iState)
{
//	TRACE(_T("CDragDropMgr::SetState %d\n"),iState);
	
	if (iState != m_iState) 
    {
		switch (iState)
		{
		case CAPTURED:
			::SetCapture(m_hwndTracking);	 // capture mouse input
			break;
			
        case LDRAGGING:
        case RDRAGGING:
			m_hCursorSave = GetCursor();	 // save current cursor
			break;
			
		default: // NONE
			if (GetCapture() == m_hwndTracking)
				::ReleaseCapture();				 // release capture and..
			
			SetCursor(m_hCursorSave);		 // ..restore cursor
			
            if (m_pDragImage) 
            {
				m_pDragImage->DragLeave(CWnd::FromHandle(m_ddi.hwndTarget));
				m_pDragImage->EndDrag();	 // end drawing and..
				
				delete m_pDragImage;			 // ..destroy..
				m_pDragImage=NULL;			 // ..image list
			}

			if (m_hwndTracking)
				InvalidateRect(m_hwndTracking, NULL, FALSE);

			if (m_ddi.hwndTarget)
				InvalidateRect(m_ddi.hwndTarget, NULL, FALSE);

			delete m_ddi.pData;
			m_ddi.pData = NULL;
			m_hwndTracking = NULL;
			
			break;
		}
		
		m_iState = iState;
	}
}

//////////////////
// Create the drag image: create an image list and call virtual darw function
// to draw the data into the image list. Will then use this during dragging.
//
CImageList* CDragDropData::CreateDragImage(CWnd* pWnd, CSize& sizeImage)
{
	CImageList *pil = new CImageList();

	if (!CreateDragImage(pWnd, *pil, sizeImage))
	{
		delete pil;
		pil = NULL;
	}
    
	return pil;
}

BOOL CDragDropData::CreateDragImage(CWnd* pWnd, CImageList& il, CSize& sizeImage)
{
	il.DeleteImageList();

	// create memory dc compatible w/source window
	CWindowDC dcWin(pWnd);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dcWin);

	// use same font as source window
	CFont* pFont = pWnd->GetFont();
	CFont* pOldFont = dcMem.SelectObject(pFont);

	// get size of drag image
	sizeImage = OnGetDragSize(dcMem); // call virtual fn to get size
	CRect rc(CPoint(0, 0), sizeImage);

	// create image list: create bitmap and draw into it
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dcWin, sizeImage.cx, sizeImage.cy);

	CBitmap* pOldBitmap = dcMem.SelectObject(&bmp);

	dcMem.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

	COLORREF crMask;
	OnDrawDragData(dcMem, rc, crMask); // call virtual fn to draw

	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);

	// create image list and add bitmap to it
	if (!il.Create(sizeImage.cx, sizeImage.cy, ILC_COLORDDB | ILC_MASK, 0, 1))
		return FALSE;

	if (il.Add(&bmp, crMask) != 0)
		return FALSE;

	return TRUE;
}

//////////////////
// Get draw size for text: use DrawText with DT_CALCRECT.
//
CSize CDragDropText::OnGetDragSize(CDC& dc)
{
	CRect rc(0,0,0,0);
	dc.DrawText(m_text, &rc, DT_CALCRECT);

	if (rc.right>MAXWIDTH)
		rc.right = MAXWIDTH;
    
	return rc.Size();
}

//////////////////
// Call MFC/Windows to draw text.
//
void CDragDropText::OnDrawDragData(CDC& dc, const CRect& rc, COLORREF& crMask)
{
	crMask = 1;
	dc.DrawText(m_text, (LPRECT)&rc, DT_LEFT|DT_END_ELLIPSIS);
}

void CDragDropMgr::CancelDrag()
{
    if (IsDragging())
        SendDragMessage(WM_DD_DRAGABORT);

	// Always
	SetState(NONE);
}
