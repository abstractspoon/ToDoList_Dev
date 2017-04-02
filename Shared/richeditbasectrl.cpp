// olericheditctrl.cpp : implementation file
//

#include "stdafx.h"
#include "richeditbasectrl.h"
#include "richedithelper.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "autoflag.h"
#include "misc.h"
#include "filemisc.h"
#include "graphicsmisc.h"
#include "enstring.h"
#include "regkey.h"
#include "osversion.h"
#include "clipboard.h"

#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);
const CRect DEFMARGINS = CRect(8, 4, 8, 0);

const LPCSTR  DEFAULTRTF				= "{\\rtf1\\ansi\\deff0\\f0\\fs60}";
const LPCTSTR RTF_TABLE_HEADER			= _T("{\\rtf1{\\pard{{\\trowd");
const LPCTSTR RTF_TEXT_INDENT			= _T("\\trgaph%d");
const LPCTSTR RTF_COLUMN_WIDTH			= _T("\\cellx%d");
const LPCTSTR RTF_CELL_BORDER_TOP		= _T("\\clbrdrt\\brdrdb\\brdrw1");
const LPCTSTR RTF_CELL_BORDER_LEFT		= _T("\\clbrdrl\\brdrdb\\brdrw1");
const LPCTSTR RTF_CELL_BORDERS_BOTTOM	= _T("\\clbrdrb\\brdrdb\\brdrw1");
const LPCTSTR RTF_CELL_BORDERS_RIGHT	= _T("\\clbrdrr\\brdrdb\\brdrw1");
const LPCTSTR RTF_TABLE_START			= _T("\\pard\\intbl");
const LPCTSTR RTF_COLUMN_CELL			= _T("\\cell");
const LPCTSTR RTF_ROW					= _T("{\\row}");
const LPCTSTR RTF_TABLE_FOOTER			= _T("}\\par}}}");
const LPCTSTR RTF_SOFT_RETURN			= _T("{\\rtf1{\\line}");
const LPCTSTR RTF_HORZ_LINE				= _T("{\\rtf1{\\pict\\wmetafile8\\picw26\\pich26\\picwgoal20000\\pichgoal15 ")
											_T("0100090000035000000000002700000000000400000003010800050000000b0200000000050000")
											_T("000c0202000200030000001e000400000007010400040000000701040027000000410b2000cc00")
											_T("010001000000000001000100000000002800000001000000010000000100010000000000000000")
											_T("000000000000000000000000000000000000000000ffffff00000000ff040000002701ffff0300")
											_T("00000000}}");

/////////////////////////////////////////////////////////////////////////////

struct STREAMINCOOKIE
{
	STREAMINCOOKIE(const CString& sContent) : sRTF(sContent), nStreamPos(0) 
	{
	}
	
	int GetLength() const 
	{ 
		return (sRTF.GetLength() * sizeof(TCHAR)); 
	}
	
	LPCSTR CopyFrom() const 
	{ 
		return (((LPCSTR)(LPCTSTR)sRTF) + nStreamPos); 
	}
	
	const CString& sRTF;
	int nStreamPos;
};

/////////////////////////////////////////////////////////////////////////////
// CRichEditBaseCtrl

CRichEditBaseCtrl::CRichEditBaseCtrl(BOOL bAutoRTL) 
	: 
	m_bEnableSelectOnFocus(FALSE), 
	m_bInOnFocus(FALSE), 
	m_rMargins(DEFMARGINS),
	m_bAutoRTL(bAutoRTL)
{
   m_callback.SetOwner(this);
}

CRichEditBaseCtrl::~CRichEditBaseCtrl()
{
   m_callback.Release();
}


BEGIN_MESSAGE_MAP(CRichEditBaseCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditBaseCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplaceCmd)
	ON_WM_SETFOCUS()
	ON_MESSAGE(EM_SETSEL, OnEditSetSelection)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditBaseCtrl message handlers

BOOL CRichEditBaseCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle)
{
	return (CRichEditHelper::CreateRichEdit50(*this, dwStyle, rect, pParentWnd, nID, dwExStyle, m_bAutoRTL) ||
			CRichEditHelper::CreateRichEdit20(*this, dwStyle, rect, pParentWnd, nID, dwExStyle, m_bAutoRTL));
}

int CRichEditBaseCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	ASSERT_VALID(this);
	
	SetOLECallback(&m_callback);
	EnableInlineSpellChecking(TRUE);
		
	return 0;
}

void CRichEditBaseCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CAutoFlag af(m_bInOnFocus, TRUE);

	CRichEditCtrl::OnSetFocus(pOldWnd);
}

LRESULT CRichEditBaseCtrl::OnEditSetSelection(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_bEnableSelectOnFocus || !m_bInOnFocus)
		return Default();

	// else
	return 0L;
}

void CRichEditBaseCtrl::OnDestroy()
{
	// destroy the find dialog. it will delete itself
	if (m_findState.pFindReplaceDlg)
	{
		m_findState.pFindReplaceDlg->DestroyWindow();
		m_findState.pFindReplaceDlg = NULL;
	}

	CRichEditCtrl::OnDestroy();
}

void CRichEditBaseCtrl::PreSubclassWindow() 
{
	SetOLECallback(&m_callback);
	EnableInlineSpellChecking(TRUE);
	
	CRichEditCtrl::PreSubclassWindow();
}

BOOL CRichEditBaseCtrl::Undo()
{
	return CTextDocument(GetSafeHwnd()).Undo();
}

BOOL CRichEditBaseCtrl::Redo()
{
	return CTextDocument(GetSafeHwnd()).Redo();
}

BOOL CRichEditBaseCtrl::PasteSpecial(CLIPFORMAT nFormat)
{
	if ((nFormat == 0) || !CClipboard().HasFormat(nFormat))
		return FALSE;

	// else
	::SendMessage(m_hWnd, EM_PASTESPECIAL, nFormat, NULL);
	return TRUE;
}

BOOL CRichEditBaseCtrl::PasteSimpleText()
{
	if (!CanPasteSimpleText())
		return FALSE;

	ReplaceSel(CClipboard().GetText(), TRUE);
	return TRUE;
}

BOOL CRichEditBaseCtrl::CanPasteSimpleText() const
{
	CClipboard cb;

#ifndef _UNICODE
	return cb.HasFormat(CF_TEXT);
#else
	return cb.HasFormat(CF_UNICODETEXT);
#endif
}

BOOL CRichEditBaseCtrl::SetTextEx(const CString& sText, DWORD dwFlags, UINT nCodePage)
{
	ASSERT(GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return FALSE;
	
	LPTSTR szText = NULL;
	int nTextLen = sText.GetLength();
	BOOL bWantUnicode = (nCodePage == CP_UNICODE);

#ifdef _UNICODE
	if (bWantUnicode)
		szText = (LPWSTR)(LPCTSTR)sText;

	else // ansi
		szText = (LPWSTR)Misc::WideToMultiByte(sText, nTextLen, nCodePage);
#else
	if (bWantUnicode)
		szText = (LPSTR)Misc::MultiByteToWide(sText, nTextLen, nCodePage);

	else // ansi
		szText = (LPSTR)(LPCSTR)sText;
#endif

	SETTEXTEX stex = { dwFlags, nCodePage };
	BOOL bResult = SendMessage(EM_SETTEXTEX, (WPARAM)&stex, (LPARAM)szText);

	// cleanup
	if (szText != (LPCTSTR)sText)
		delete [] szText;

	return bResult;
}

CString CRichEditBaseCtrl::GetSelText()
{
	CHARRANGE cr;
	GetSel(cr);

	return GetTextRange(cr);
}

CString CRichEditBaseCtrl::GetTextRange(const CHARRANGE& cr)
{
	int nLength = int(cr.cpMax - cr.cpMin + 1);

    // create an ANSI buffer 
    LPTSTR szChar = new TCHAR[nLength];
	ZeroMemory(szChar, nLength * sizeof(TCHAR));

#ifndef _UNICODE
	if (CWinClasses::IsClass(*this, WC_RICHEDIT50)) // must handle unicode 
	{
		// create a Unicode (Wide Character) buffer of the same length
		LPWSTR lpszWChar = new WCHAR[nLength];

		TEXTRANGEW tr;
		tr.chrg = cr;
		tr.lpstrText = lpszWChar;
		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);

		// Convert the Unicode text to ANSI.
		WideCharToMultiByte(CP_ACP, 0, lpszWChar, -1, szChar, nLength, NULL, NULL);

		delete lpszWChar;
	}
	else 
	{
		TEXTRANGE tr;
		tr.chrg = cr;
		tr.lpstrText = szChar;
		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	}
#else
	if (CWinClasses::IsClass(*this, WC_RICHEDIT50)) 
	{
		TEXTRANGE tr;
		tr.chrg = cr;
		tr.lpstrText = szChar;
		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	}
	else // must handle ansi
	{
		// create a Ansi buffer of the same length
		LPSTR lpszChar = new char[nLength];

		TEXTRANGEA tr;
		tr.chrg = cr;
		tr.lpstrText = lpszChar;
		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);

		// Convert the Ansi text to Unicode.
		MultiByteToWideChar(CP_ACP, 0, lpszChar, -1, szChar, nLength);

		delete lpszChar;
	}
#endif

	CString sText(szChar);
	delete [] szChar;

	return sText;
}

void CRichEditBaseCtrl::SelectCurrentWord()
{
	CHARRANGE cr;
	GetSel(cr);

	if (cr.cpMin == cr.cpMax) // nothing already selected
	{
		cr.cpMin = SendMessage(EM_FINDWORDBREAK, WB_LEFT, cr.cpMin);
		cr.cpMax = SendMessage(EM_FINDWORDBREAK, WB_RIGHTBREAK, cr.cpMax + 1);

		SetSel(cr);
	}
}

BOOL CRichEditBaseCtrl::InsertSoftReturn()
{
	return SetTextEx(RTF_SOFT_RETURN);
}

BOOL CRichEditBaseCtrl::InsertHorizontalLine()
{
	return SetTextEx(RTF_HORZ_LINE);
}

BOOL CRichEditBaseCtrl::InsertTable(int nRows, int nCols, int nColWidth, int nTextIndent, DWORD dwBorders)
{
	ASSERT(GetSafeHwnd());

	if (!GetSafeHwnd() || nRows <= 0 || nCols <= 0 || nColWidth == 0)
		return FALSE;

	// table initialization
	CString sTable(RTF_TABLE_HEADER);

	// text indent
	if (nTextIndent > 0)
	{
		CString sIndent;
		sIndent.Format(RTF_TEXT_INDENT, nTextIndent);
		sTable += sIndent;
	}

	// column width setup
	CString sWidth;
	int nWidth = nColWidth;

	for (int nCol = 0; nCol < nCols; nCol++)
	{
		// borders
		if (Misc::HasFlag(dwBorders, REBCB_TOP))
			sTable += RTF_CELL_BORDER_TOP;

		if (Misc::HasFlag(dwBorders, REBCB_LEFT))
			sTable += RTF_CELL_BORDER_LEFT;

		if (Misc::HasFlag(dwBorders, REBCB_BOTTOM))
			sTable += RTF_CELL_BORDERS_BOTTOM;

		if (Misc::HasFlag(dwBorders, REBCB_RIGHT))
			sTable += RTF_CELL_BORDERS_RIGHT;

		// column widths
		sWidth.Format(RTF_COLUMN_WIDTH, nWidth);
		sTable += sWidth;
		nWidth += nColWidth;
	}

	sTable += RTF_TABLE_START;

	// rows
	for (int nRow = 0; nRow < nRows; nRow++)
	{
		CString sRow('{');

		for (int nCol = 0; nCol < nCols; nCol++)
			sRow += RTF_COLUMN_CELL;

		sRow += '}';
		sRow += RTF_ROW;

		sTable += sRow;
	}

	// footer
	sTable += RTF_TABLE_FOOTER;

	// paste table
	return SetTextEx(sTable);
}

CRichEditBaseCtrl::CRichEditOleCallback::CRichEditOleCallback() : m_pOwner(NULL)
{
	m_pStorage = NULL;
	m_iNumStorages = 0;
	m_dwRef = 0;

	// set up OLE storage
	HRESULT hResult = ::StgCreateDocfile(NULL, STGM_TRANSACTED | 
		STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_DELETEONRELEASE,
		0, &m_pStorage );

	if ( m_pStorage == NULL ||
		hResult != S_OK )
	{
		AfxThrowOleException( hResult );
	}
}

CRichEditBaseCtrl::CRichEditOleCallback::~CRichEditOleCallback()
{
	if (m_pStorage)
   {
      m_pStorage->Release();
      m_pStorage = NULL;
   }
}


HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::GetNewStorage(LPSTORAGE* lplpstg)
{
	m_iNumStorages++;
	WCHAR tName[50];

#if _MSC_VER >= 1400
	swprintf_s(tName, 50, L"REOLEStorage%d", m_iNumStorages);
#else
	swprintf(tName, L"REOLEStorage%d", m_iNumStorages);
#endif

	HRESULT hResult = m_pStorage->CreateStorage(tName, 
		STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE ,
		0, 0, lplpstg );

	if (hResult != S_OK )
	{
		::AfxThrowOleException( hResult );
	}

	return hResult;
}

HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::QueryInterface(REFIID iid, void ** ppvObject)
{

	HRESULT hr = S_OK;
	*ppvObject = NULL;
	
	if ( iid == IID_IUnknown ||
		iid == IID_IRichEditOleCallback )
	{
		*ppvObject = this;
		AddRef();
		hr = NOERROR;
	}
	else
	{
		hr = E_NOINTERFACE;
	}

	return hr;
}

ULONG STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::AddRef()
{
	return ++m_dwRef;
}



ULONG STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::Release()
{
	if ( --m_dwRef == 0 )
	{
		if (m_pStorage)
         m_pStorage->Release();

      m_pStorage = NULL;

		return 0;
	}

	return m_dwRef;
}


HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::GetInPlaceContext(LPOLEINPLACEFRAME FAR *lplpFrame,
	LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
   if (m_pOwner)
      return m_pOwner->GetInPlaceContext(lplpFrame, lplpDoc, lpFrameInfo);

	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
   if (m_pOwner)
      return m_pOwner->ShowContainerUI(fShow);

	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp)
{
   if (m_pOwner)
      return m_pOwner->QueryInsertObject(lpclsid, lpstg, cp);

	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::DeleteObject(LPOLEOBJECT lpoleobj)
{
   if (m_pOwner)
      return m_pOwner->DeleteObject(lpoleobj);

	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
	DWORD reco, BOOL fReally, HGLOBAL hMetaPict)
{
   if (m_pOwner)
      return m_pOwner->QueryAcceptData(lpdataobj, lpcfFormat, reco, fReally, hMetaPict);

	return S_OK;
}


HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::ContextSensitiveHelp(BOOL fEnterMode)
{
   if (m_pOwner)
      return m_pOwner->ContextSensitiveHelp(fEnterMode);

	return S_OK;
}



HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::GetClipboardData(CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj)
{
   if (m_pOwner)
      return m_pOwner->GetClipboardData(lpchrg, reco, lplpdataobj);

	return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
   if (m_pOwner)
      return m_pOwner->GetDragDropEffect(fDrag, grfKeyState, pdwEffect);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE 
CRichEditBaseCtrl::CRichEditOleCallback::GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg,
	HMENU FAR *lphmenu)
{
   if (m_pOwner)
      return m_pOwner->GetContextMenu(seltyp, lpoleobj, lpchrg, lphmenu);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CRichEditBaseCtrl Find & Replace

void CRichEditBaseCtrl::DoEditFind(UINT nIDTitle)
{
	ASSERT_VALID(this);
	DoEditFindReplace(TRUE, nIDTitle);
}

void CRichEditBaseCtrl::DoEditReplace(UINT nIDTitle)
{
	ASSERT_VALID(this);
	DoEditFindReplace(FALSE, nIDTitle);
}

void CRichEditBaseCtrl::AdjustDialogPosition(CDialog* pDlg)
{
	ASSERT(pDlg != NULL);
	long lStart, lEnd;
	GetSel(lStart, lEnd);
	CPoint point = GetCharPos(lStart);
	ClientToScreen(&point);
	CRect rectDlg;
	pDlg->GetWindowRect(&rectDlg);
	if (rectDlg.PtInRect(point))
	{
		if (point.y > rectDlg.Height())
			rectDlg.OffsetRect(0, point.y - rectDlg.bottom - 20);
		else
		{
			int nVertExt = GetSystemMetrics(SM_CYSCREEN);
			if (point.y + rectDlg.Height() < nVertExt)
				rectDlg.OffsetRect(0, 40 + point.y - rectDlg.top);
		}
		pDlg->MoveWindow(&rectDlg);
	}
}

void CRichEditBaseCtrl::DoEditFindReplace(BOOL bFindOnly, UINT nIDTitle)
{
	ASSERT_VALID(this);
	
	if (m_findState.pFindReplaceDlg != NULL)
	{
		if (m_findState.bFindOnly == bFindOnly)
		{
			m_findState.pFindReplaceDlg->SetActiveWindow();
			m_findState.pFindReplaceDlg->ShowWindow(SW_SHOW);
			return;
		}
		else
		{
			ASSERT(m_findState.bFindOnly != bFindOnly);
			m_findState.pFindReplaceDlg->SendMessage(WM_CLOSE);
			ASSERT(m_findState.pFindReplaceDlg == NULL);
			ASSERT_VALID(this);
		}
	}
	CString strFind = GetSelText();

	// if selection is empty or spans multiple lines use old find text
	if (strFind.IsEmpty() || (strFind.FindOneOf(_T("\n\r")) != -1))
		strFind = m_findState.strFind;

	CString strReplace = m_findState.strReplace;
	m_findState.pFindReplaceDlg = NewFindReplaceDlg();
	ASSERT(m_findState.pFindReplaceDlg != NULL);

	DWORD dwFlags = NULL;
	if (m_findState.bNext)
		dwFlags |= FR_DOWN;
	if (m_findState.bCase)
		dwFlags |= FR_MATCHCASE;
	if (m_findState.bWord)
		dwFlags |= FR_WHOLEWORD;

	// hide stuff that RichEdit doesn't support
	dwFlags |= FR_HIDEUPDOWN;
	
	if (!m_findState.pFindReplaceDlg->Create(bFindOnly, strFind, strReplace, dwFlags, this))
	{
		m_findState.pFindReplaceDlg = NULL;
		ASSERT_VALID(this);
		return;
	}
	ASSERT(m_findState.pFindReplaceDlg != NULL);

	// set the title
	if (nIDTitle)
	{
		CEnString sTitle(nIDTitle);
		m_findState.pFindReplaceDlg->SetWindowText(sTitle);
	}

	m_findState.bFindOnly = bFindOnly;
	m_findState.pFindReplaceDlg->SetActiveWindow();
	m_findState.pFindReplaceDlg->ShowWindow(SW_SHOW);
	ASSERT_VALID(this);
}

void CRichEditBaseCtrl::OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord)
{
	ASSERT_VALID(this);
	
	m_findState.strFind = lpszFind;
	m_findState.bCase = bCase;
	m_findState.bWord = bWord;
	m_findState.bNext = bNext;

	if (!FindText())
		TextNotFound(m_findState.strFind);
	else
		AdjustDialogPosition(m_findState.pFindReplaceDlg);

	ASSERT_VALID(this);
}

void CRichEditBaseCtrl::OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,
	BOOL bWord, LPCTSTR lpszReplace)
{
	ASSERT_VALID(this);
	
	m_findState.strFind = lpszFind;
	m_findState.strReplace = lpszReplace;
	m_findState.bCase = bCase;
	m_findState.bWord = bWord;
	m_findState.bNext = bNext;

	if (!SameAsSelected(m_findState.strFind, m_findState.bCase, m_findState.bWord))
	{
		if (!FindText())
		{
			TextNotFound(m_findState.strFind);
			return;
		}
		else
			AdjustDialogPosition(m_findState.pFindReplaceDlg);
	}

	ReplaceSel(m_findState.strReplace, TRUE);

	if (!FindText())
		TextNotFound(m_findState.strFind);
	else
		AdjustDialogPosition(m_findState.pFindReplaceDlg);

	ASSERT_VALID(this);
}

void CRichEditBaseCtrl::OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase, BOOL bWord)
{
	ASSERT_VALID(this);
	
	// start searching at the beginning of the text so that we know to stop at the end
	SetSel(0, 0);

	m_findState.strFind = lpszFind;
	m_findState.strReplace = lpszReplace;
	m_findState.bCase = bCase;
	m_findState.bWord = bWord;
	m_findState.bNext = TRUE;

	CWaitCursor wait;

	while (FindText(FALSE))
		ReplaceSel(m_findState.strReplace, TRUE);

	TextNotFound(m_findState.strFind);
	HideSelection(FALSE, FALSE);

	ASSERT_VALID(this);
}

LRESULT CRichEditBaseCtrl::OnFindReplaceCmd(WPARAM, LPARAM lParam)
{
	ASSERT_VALID(this);
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier(lParam);
	ASSERT(pDialog != NULL);
	
	ASSERT(pDialog == m_findState.pFindReplaceDlg);
	if (pDialog->IsTerminating())
	{
		m_findState.pFindReplaceDlg = NULL;
		SetFocus();
	}
	else if (pDialog->FindNext())
	{
		OnFindNext(pDialog->GetFindString(), pDialog->SearchDown(),
			pDialog->MatchCase(), pDialog->MatchWholeWord());
	}
	else if (pDialog->ReplaceCurrent())
	{
		ASSERT(!m_findState.bFindOnly);
		OnReplaceSel(pDialog->GetFindString(),
			pDialog->SearchDown(), pDialog->MatchCase(), pDialog->MatchWholeWord(),
			pDialog->GetReplaceString());
	}
	else if (pDialog->ReplaceAll())
	{
		ASSERT(!m_findState.bFindOnly);
		OnReplaceAll(pDialog->GetFindString(), pDialog->GetReplaceString(),
			pDialog->MatchCase(), pDialog->MatchWholeWord());
	}
	ASSERT_VALID(this);
	return 0;
}

BOOL CRichEditBaseCtrl::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase, BOOL /*bWord*/)
{
	// check length first
	size_t nLen = lstrlen(lpszCompare);
	long lStartChar, lEndChar;
	GetSel(lStartChar, lEndChar);
	if (nLen != (size_t)(lEndChar - lStartChar))
		return FALSE;

	// length is the same, check contents
	CString strSelect = GetSelText();
	return (bCase && lstrcmp(lpszCompare, strSelect) == 0) ||
		(!bCase && lstrcmpi(lpszCompare, strSelect) == 0);
}

BOOL CRichEditBaseCtrl::FindText(BOOL bWrap)
{
	return FindText(m_findState.strFind, m_findState.bCase, m_findState.bWord, bWrap);
}

BOOL CRichEditBaseCtrl::FindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord, BOOL bWrap)
{
	CWaitCursor wait;

	ASSERT(lpszFind != NULL);

	if (!lpszFind || !*lpszFind)
		return FALSE;

	// get the current selection because this is where we 
	// always start searching from
	FINDTEXTEX ft;
	GetSel(ft.chrg);

	// convert text to multibyte string for RichEdit50W
#ifdef _UNICODE
	ft.lpstrText = (LPTSTR)lpszFind;
#else
	ft.lpstrText = (LPTSTR)A2BSTR((LPTSTR)lpszFind);
#endif

	// is there is a selection? for instance, previously found text
	if (ft.chrg.cpMin < ft.chrg.cpMax) 
	{
		// If byte at beginning of selection is a DBCS lead byte,
		// increment by one extra byte.
		CHARRANGE cr = { ft.chrg.cpMin, ft.chrg.cpMin + 1 };
		CString ch = GetTextRange(cr);

		if (!ch.IsEmpty() && _istlead(ch[0]))
		{
			ASSERT(ft.chrg.cpMax - ft.chrg.cpMin >= 2);
			ft.chrg.cpMin++;
		}

		// then shift the selection start forward a char so that we don't simply
		// find the already selected text.
		ft.chrg.cpMin++;
	}

	// always search to the end of the text
	ft.chrg.cpMax = GetTextLength();

	DWORD dwFlags = FR_DOWN;
	dwFlags |= bCase ? FR_MATCHCASE : 0;
	dwFlags |= bWord ? FR_WHOLEWORD : 0;

	// if we find the text return TRUE
	if (FindAndSelect(dwFlags, ft) != -1)
		return TRUE;

	// else we need to restart the search from the beginning
	if (bWrap)
	{
		ft.chrg.cpMin = 0;
		ft.chrg.cpMax = GetTextLength();

		return (FindAndSelect(dwFlags, ft) != -1);
	}

	// else
	return FALSE;
}

long CRichEditBaseCtrl::FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft)
{
	long index = (long)::SendMessage(m_hWnd, EM_FINDTEXTEX, dwFlags, (LPARAM)&ft);

	if (index == -1)
		index = (long)::SendMessage(m_hWnd, EM_FINDTEXTEXW, dwFlags, (LPARAM)&ft);

	if (index != -1) // i.e. we found something
		SetSel(ft.chrgText);
	
	return index;
}

void CRichEditBaseCtrl::TextNotFound(LPCTSTR /*lpszFind*/)
{
	ASSERT_VALID(this);
	MessageBeep(MB_ICONHAND);
}

BOOL CRichEditBaseCtrl::IsFindDialog(HWND hwnd) const
{
	if (m_findState.pFindReplaceDlg)
		return (m_findState.pFindReplaceDlg->GetSafeHwnd() == hwnd);

	return FALSE;
}

void CRichEditBaseCtrl::SetMargins(int nLeft, int nTop, int nRight, int nBottom)
{
	SetMargins(CRect(nLeft, nTop, nRight, nBottom));
}

void CRichEditBaseCtrl::SetMargins(int nMargin)
{
	SetMargins(CRect(nMargin, nMargin, nMargin, nMargin));
}

void CRichEditBaseCtrl::SetMargins(LPCRECT pMargins)
{
	if (pMargins == NULL)
		m_rMargins.SetRectEmpty();
	else
		m_rMargins = *pMargins;

	CRect rClient;
	GetClientRect(rClient);

	rClient -= m_rMargins;

	SetRect(rClient);
}

void CRichEditBaseCtrl::OnSize(UINT nType, int cx, int cy)
{
	CRichEditCtrl::OnSize(nType, cx, cy);

	CRect rClient(0, 0, cx, cy);
	rClient -= m_rMargins;

	SetRect(rClient);
}

DWORD CRichEditBaseCtrl::GetSelectionCharFormat(CHARFORMAT2 &cf) const
{
	ASSERT(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(cf);
	return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

DWORD CRichEditBaseCtrl::GetSelectionCharFormat(CHARFORMAT &cf) const
{
	ASSERT(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(cf);
	return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

BOOL CRichEditBaseCtrl::SetSelectionCharFormat(CHARFORMAT2 &cf)
{
	ASSERT(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(cf);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

BOOL CRichEditBaseCtrl::SetSelectionCharFormat(CHARFORMAT &cf)
{
	ASSERT(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(cf);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

BOOL CRichEditBaseCtrl::SetParaFormat(PARAFORMAT2 &pf)
{
	ASSERT(::IsWindow(m_hWnd));
	pf.cbSize = sizeof(pf);
	BOOL bResult = (BOOL)::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

	// Windows seems not to sent EN_CHANGE when using PARAFORMAT2
	// so we must send it ourselves
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)GetSafeHwnd());

	return bResult;
}

BOOL CRichEditBaseCtrl::SetParaFormat(PARAFORMAT &pf)
{
	ASSERT(::IsWindow(m_hWnd));
	pf.cbSize = sizeof(pf);
	return (BOOL)::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

DWORD CRichEditBaseCtrl::GetParaFormat(PARAFORMAT2 &pf) const
{
	ASSERT(::IsWindow(m_hWnd));
	pf.cbSize = sizeof(pf);
	return (DWORD)::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
}

DWORD CRichEditBaseCtrl::GetParaFormat(PARAFORMAT &pf) const
{
	ASSERT(::IsWindow(m_hWnd));
	pf.cbSize = sizeof(pf);
	return (DWORD)::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
}

BOOL CRichEditBaseCtrl::SelectionHasEffect(DWORD dwMask, DWORD dwEffect) const
{
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(cf));
	
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = dwMask;

	if (GetSelectionCharFormat(cf))
		return (cf.dwEffects & dwEffect);

	// else
	return FALSE;
}

BOOL CRichEditBaseCtrl::CanEdit() const
{
	return (IsWindowEnabled() && !(GetStyle() & ES_READONLY));
}

BOOL CRichEditBaseCtrl::HasSelection() const
{
	CHARRANGE cr = { 0 };
	GetSel(cr);
				
	return ((cr.cpMax - cr.cpMin) != 0);
}

void CRichEditBaseCtrl::SetFirstVisibleLine(int nLine)
{
	int nFirst = GetFirstVisibleLine();
	
	if (nLine < nFirst)
	{
		int nPrevFirst;
		
		do
		{
			LineScroll(-1);
			
			nPrevFirst = nFirst;
			nFirst = GetFirstVisibleLine();
		}
		while  (nLine < nFirst && (nFirst != nPrevFirst));
	}
	else if (nLine > nFirst)
	{
		int nPrevFirst;
		
		do
		{
			LineScroll(1);
			
			nPrevFirst = nFirst;
			nFirst = GetFirstVisibleLine();
		}
		while  (nLine > nFirst && (nFirst != nPrevFirst));
	}
}

int CRichEditBaseCtrl::GetLineHeight() const
{
	CHARFORMAT cf;
	GetDefaultCharFormat(cf);
	
	// convert height in Twips to pixels
	int nTwipsPerInch = 1440;
    int nPixelsPerInch = GraphicsMisc::PixelsPerInch();
	
	return ((cf.yHeight * nPixelsPerInch) / nTwipsPerInch + 2);
}

CPoint CRichEditBaseCtrl::GetCaretPos() const
{
	if (::GetFocus() != GetSafeHwnd())
	{
		ASSERT (0);
		return CPoint(0, 0);
	}
	
	CPoint ptCaret = CWnd::GetCaretPos();
	ptCaret.y += GetLineHeight() / 2; // half line height
	ptCaret.x += 4; // estaimate 1/2 char width
	
	return ptCaret;
}

int CRichEditBaseCtrl::CharFromPoint(const CPoint& point) const
{
	POINTL ptl = { point.x, point.y };
	
	return ::SendMessage(GetSafeHwnd(), EM_CHARFROMPOS, 0, (LPARAM)&ptl);
}

/////////////////////////////////////////////////////////////////////////////
// Stream callback functions
// Callbacks to the Save and Load functions.

DWORD CRichEditBaseCtrl::StreamOutCB(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CMemFile* pFile = (CMemFile*)dwCookie;
	
	pFile->Write(pbBuff, cb);
	*pcb = cb;
	
	return 0;
}

DWORD CRichEditBaseCtrl::StreamOutLenCB(DWORD dwCookie, LPBYTE /*pbBuff*/, LONG cb, LONG *pcb)
{
	int* pByteLen = (int*)dwCookie;
	
	*pByteLen += cb;
	*pcb = cb;
	
	return 0;
}

DWORD CRichEditBaseCtrl::StreamInCB(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	STREAMINCOOKIE* pCookie = (STREAMINCOOKIE*)dwCookie;
	int nBytes = min(cb, pCookie->GetLength());
	
#pragma warning(disable:4996)
	strncpy((char*)pbBuff, pCookie->CopyFrom(), nBytes);
#pragma warning(default:4996)
	
	*pcb = nBytes;
	pCookie->nStreamPos += nBytes;
	
	return 0;
}

BOOL CRichEditBaseCtrl::Save(CString& filename)
{
	CString str = GetRTF(); // returns multibyte encoded string
	
	// save as multibyte
	return FileMisc::SaveFile(filename, (LPCSTR)(LPCTSTR)str);
}

BOOL CRichEditBaseCtrl::Load(CString& filename)
{
	CString str;
	
	if (!FileMisc::LoadFile(filename, str))
		return FALSE;
	
	if (!str.IsEmpty())
	{
#ifdef _UNICODE
		// str is unicode but RTF must be multibyte
		Misc::EncodeAsMultiByte(str);
#endif
		
		SetRTF(str);
	}
	
	return TRUE;
}

CString CRichEditBaseCtrl::GetRTF() const
{
	// stream to mem file in big chunks
	// note: we use a BIG file because it gives us excellent
	// performance on content containing images
	CMemFile file(2024 * 1024);
	EDITSTREAM es = { (DWORD)&file, 0, StreamOutCB };

	const_cast<CRichEditBaseCtrl*>(this)->StreamOut(SF_RTF, es);
	
	// then copy to string
	CString sRTF;
	int nLen = (int)file.GetLength();
	
	LPTSTR szRTF = sRTF.GetBuffer(nLen);
	
	file.SeekToBegin();
	file.Read((void*)szRTF, nLen);
	
	sRTF.ReleaseBuffer(nLen);
	return sRTF;
}

int CRichEditBaseCtrl::GetRTFLength() const
{
	int nByteLen = 0;
	EDITSTREAM es = { (DWORD)&nByteLen, 0, StreamOutLenCB };
	
	const_cast<CRichEditBaseCtrl*>(this)->StreamOut(SF_RTF, es);
	
	return nByteLen;
}

void CRichEditBaseCtrl::SetRTF(const CString& rtf)
{
	CString sRTF = (rtf.IsEmpty() ? DEFAULTRTF : rtf);
	
	STREAMINCOOKIE cookie(sRTF);
	EDITSTREAM es = { (DWORD)&cookie, 0, StreamInCB };
	
	StreamIn(SF_RTF, es);
}

BOOL CRichEditBaseCtrl::EnableInlineSpellChecking(BOOL bEnable)
{
	if (!SupportsInlineSpellChecking())
		return FALSE;

	ASSERT(GetSafeHwnd());

	DWORD dwLangOpt = SendMessage(EM_GETLANGOPTIONS);
	DWORD dwLangFlags = IMF_SPELLCHECKING;

	if (Misc::ModifyFlags(dwLangOpt, 
						(bEnable ? 0 : dwLangFlags),  // remove
						(bEnable ? dwLangFlags : 0))) // add
	{
		SendMessage(EM_SETLANGOPTIONS, 0, dwLangOpt);
	}
		
	DWORD dwEditStyle = SendMessage(EM_GETEDITSTYLE);
	DWORD dwEditFlags = (SES_USECTF | SES_CTFALLOWEMBED | SES_CTFALLOWSMARTTAG | SES_CTFALLOWPROOFING);

	if (Misc::ModifyFlags(dwLangOpt, 
						(bEnable ? 0 : dwEditFlags),  // remove
						(bEnable ? dwEditFlags : 0))) // add
	{
		SendMessage(EM_SETEDITSTYLE, 0, dwEditStyle);
	}

	return TRUE;
}

BOOL CRichEditBaseCtrl::IsInlineSpellCheckingEnabled() const
{
	if (!SupportsInlineSpellChecking())
		return FALSE;

	ASSERT(GetSafeHwnd());

	DWORD dwLangOpt = ::SendMessage(GetSafeHwnd(), EM_GETLANGOPTIONS, 0, 0);
	DWORD dwLangFlags = IMF_SPELLCHECKING;

	DWORD dwEditStyle = ::SendMessage(GetSafeHwnd(), EM_GETEDITSTYLE, 0, 0);
	DWORD dwEditFlags = (SES_USECTF | SES_CTFALLOWEMBED | SES_CTFALLOWSMARTTAG | SES_CTFALLOWPROOFING);

	return (Misc::HasFlag(dwLangOpt, dwLangFlags) && Misc::HasFlag(dwEditStyle, dwEditFlags));
}

BOOL CRichEditBaseCtrl::SupportsInlineSpellChecking()
{
	return (COSVersion() >= OSV_WIN8);
}

CLIPFORMAT CRichEditBaseCtrl::GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format, const CLIPFORMAT fmtPreferred[], int nNumFmts)
{
	CDWordArray aFormatIDs;
	
#ifdef _DEBUG
	CStringArray aFormatNames;
	CString sFormatNames, sFormatIDs;
	
	if (CClipboard::GetAvailableFormats(lpDataOb, aFormatIDs, aFormatNames))
	{
		sFormatNames = Misc::FormatArray(aFormatNames);
		sFormatIDs = Misc::FormatArray(aFormatIDs);
	}
#else
	CClipboard::GetAvailableFormats(lpDataOb, aFormatIDs);
#endif
	
	for (int nFmt = 0; nFmt < nNumFmts; nFmt++)
	{
		UINT nFormat = fmtPreferred[nFmt];
		
		if (format && (format == nFormat))
			return format;
		
		if (Misc::FindT(aFormatIDs, (DWORD)nFormat) != -1)
			return nFormat;
	}
	
	if (format)
		return format;
	
#ifndef _UNICODE
	return CF_TEXT;
#else
	return CF_UNICODETEXT;
#endif
}
