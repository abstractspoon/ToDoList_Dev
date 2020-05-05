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

const CRect DEFMARGINS					= CRect(8, 4, 8, 0);

/////////////////////////////////////////////////////////////////////////////

const UINT WM_REBC_REENABLECHANGENOTIFY = ::RegisterWindowMessage(_T("WM_REBC_REENABLECHANGENOTIFY"));

/////////////////////////////////////////////////////////////////////////////

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

// The use of whitespace within '{ HYPERLINK \" %s \"}' is ABSOLUTELY ESSENTIAL
// Reference: https://autohotkey.com/boards/viewtopic.php?t=43427
const LPCTSTR RTF_LINK					= _T("{\\rtf1{\\colortbl;\\red0\\green0\\blue255;}")
											_T("{\\field{\\*\\fldinst{ HYPERLINK \" %s \"}}")
											_T("{\\fldrslt{\\ul\\cf1\\cf1\\ul %s}}}}");

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
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()

	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplaceMsg)
	ON_REGISTERED_MESSAGE(WM_TTC_TOOLHITTEST, OnToolHitTest)
	ON_REGISTERED_MESSAGE(WM_REBC_REENABLECHANGENOTIFY, OnReenableChangeNotifications)

	ON_MESSAGE(EM_SETSEL, OnEditSetSelection)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_SETTEXT, OnSetText)

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
	
	Initialise();
		
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

LRESULT CRichEditBaseCtrl::OnSetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// prevent spurious change notifications
	TemporarilyDisableChangeNotifications();

	return Default();
}

LRESULT CRichEditBaseCtrl::OnSetText(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// prevent spurious change notifications occurring as
	// a result of spell-checking or auto-url detection
	TemporarilyDisableChangeNotifications();

	return Default();
}

void CRichEditBaseCtrl::OnDestroy()
{
	// destroy the find dialog. it will delete itself
	m_findState.DestroyDialog();

	CRichEditCtrl::OnDestroy();
}

void CRichEditBaseCtrl::PreSubclassWindow() 
{
	Initialise();

	CRichEditCtrl::PreSubclassWindow();
}

void CRichEditBaseCtrl::Initialise()
{
	SetOLECallback(&m_callback);
	EnableInlineSpellChecking(TRUE);
	EnableAutoFontChanging(FALSE);
	EnableAutoUrlDetection();
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

BOOL CRichEditBaseCtrl::InsertSimpleText(LPCWSTR szText, bool bAtEnd)
{
	if (!CanEdit() || (Misc::IsEmpty(szText) && bAtEnd))
		return FALSE;

	if (bAtEnd)
		MoveCaretToEnd();

	ReplaceSel(szText, TRUE);

	// update caret
	if (bAtEnd)
		MoveCaretToEnd();

	return TRUE;
}

BOOL CRichEditBaseCtrl::CopySimpleText()
{
	CString sSelText(GetSelText());

	if (sSelText.IsEmpty())
		return FALSE;
	
	// Replace 'CR' with 'CRLF"
	if ((sSelText.Find('\r') != -1) && (sSelText.Find('\n') == -1))
		sSelText.Replace(_T("\r"), _T("\r\n"));
	
	return CClipboard(*this).SetText(sSelText);
}

BOOL CRichEditBaseCtrl::CutSimpleText()
{
	// Allow copying even if we cannot edit
	if (!CopySimpleText() || !CanEdit())
		return FALSE;

	Clear();
	return TRUE;
}

BOOL CRichEditBaseCtrl::CanPasteSimpleText() const
{
	return (CanEdit() && CClipboard().HasText());
}

BOOL CRichEditBaseCtrl::CanCopySelectedText() const
{
	return HasSelection();
}

BOOL CRichEditBaseCtrl::CanCutSelectedText() const
{
	return (CanEdit() && HasSelection());
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

CString CRichEditBaseCtrl::GetSelText() const
{
	CHARRANGE cr;
	GetSel(cr);

	return GetTextRange(cr);
}

int CRichEditBaseCtrl::GetSelStart() const
{
	CHARRANGE cr;
	GetSel(cr);
	
	return cr.cpMin;
}

int CRichEditBaseCtrl::GetSelEnd() const
{
	CHARRANGE cr;
	GetSel(cr);
	
	return cr.cpMax;
}

CString CRichEditBaseCtrl::GetTextRange(const CHARRANGE& cr) const
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
		::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

		// Convert the Unicode text to ANSI.
		WideCharToMultiByte(CP_ACP, 0, lpszWChar, -1, szChar, nLength, NULL, NULL);

		delete lpszWChar;
	}
	else 
	{
		TEXTRANGE tr;
		tr.chrg = cr;
		tr.lpstrText = szChar;
		::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	}
#else
	if (CWinClasses::IsClass(*this, WC_RICHEDIT50)) 
	{
		TEXTRANGE tr;
		tr.chrg = cr;
		tr.lpstrText = szChar;
		::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	}
	else // must handle ansi
	{
		// create a Ansi buffer of the same length
		LPSTR lpszChar = new char[nLength];

		TEXTRANGEA tr;
		tr.chrg = cr;
		tr.lpstrText = lpszChar;
		::SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

		// Convert the Ansi text to Unicode.
		MultiByteToWideChar(CP_ACP, 0, lpszChar, -1, szChar, nLength);

		delete lpszChar;
	}
#endif

	CString sText(szChar);
	delete [] szChar;

	return sText;
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

	if (CanEdit())
		DoEditFindReplace(FALSE, nIDTitle);
}

void CRichEditBaseCtrl::AdjustFindDialogPosition()
{
	long lStart, lEnd;
	GetSel(lStart, lEnd);

	CPoint point = GetCharPos(lStart);
	ClientToScreen(&point);

	m_findState.AdjustDialogPosition(point);
}

void CRichEditBaseCtrl::DoEditFindReplace(BOOL bFindOnly, UINT nIDTitle)
{
	CEnString sTitle(nIDTitle), sSelText(GetSelText());
	VERIFY(m_findState.Initialise(*this, this, bFindOnly, sTitle, sSelText));
}

void CRichEditBaseCtrl::OnFindNext(const CString& sFind, BOOL bNext, BOOL bCase, BOOL bWord)
{
	// Update state information for next time
	m_findState.UpdateState(sFind, bNext, bCase, bWord);

	if (!FindText())
		TextNotFound(m_findState.strFind);
	else
		AdjustFindDialogPosition();
}

void CRichEditBaseCtrl::OnReplaceSel(const CString& sFind, const CString& sReplace, 
									BOOL bNext, BOOL bCase, BOOL bWord)
{
	// Update state information for next time
	m_findState.UpdateState(sFind, sReplace, bNext, bCase, bWord);

	if (!SameAsSelected(m_findState.strFind, m_findState.bCaseSensitive, m_findState.bWholeWord))
	{
		if (!FindText())
		{
			TextNotFound(m_findState.strFind);
			return;
		}

		// else
		AdjustFindDialogPosition();
	}

	ReplaceSel(m_findState.strReplace, TRUE);

	if (!FindText())
		TextNotFound(m_findState.strFind);
	else
		AdjustFindDialogPosition();
}

void CRichEditBaseCtrl::OnReplaceAll(const CString& sFind, const CString& sReplace, BOOL bCase, BOOL bWord)
{
	// Update state information for next time
	m_findState.UpdateState(sFind, sReplace, TRUE, bCase, bWord);

	ReplaceAll(sFind, sReplace, (bCase != FALSE), (bWord != FALSE));
}

int CRichEditBaseCtrl::ReplaceAll(LPCTSTR szFind, LPCTSTR szReplace, bool bCaseSensitive, bool bWholeWord)
{
	int nNumReplaced = 0;

	// start searching at the beginning of the text so that we know to stop at the end
	SetSel(0, 0);

	CWaitCursor wait;

	while (FindText(szFind, TRUE, bCaseSensitive, bWholeWord, FALSE))
	{
		ReplaceSel(szReplace, TRUE);
		nNumReplaced++;
	}

	HideSelection(FALSE, FALSE);

	return nNumReplaced;
}

LRESULT CRichEditBaseCtrl::OnFindReplaceMsg(WPARAM wParam, LPARAM lParam)
{
	m_findState.HandleCmd(this, wParam, lParam);
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
	return FindText(m_findState.strFind, 
					m_findState.bFindNext, 
					m_findState.bCaseSensitive, 
					m_findState.bWholeWord, 
					bWrap);
}

BOOL CRichEditBaseCtrl::FindText(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord, BOOL bWrap)
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

	DWORD dwFlags = 0;
	dwFlags |= bNext ? FR_DOWN : 0;
	dwFlags |= bCase ? FR_MATCHCASE : 0;
	dwFlags |= bWord ? FR_WHOLEWORD : 0;

	// if we find the text return TRUE
	if (FindAndSelect(dwFlags, ft) != -1)
		return TRUE;

	// else we need to restart the search from the beginning
	if (bWrap)
	{
		if (bNext)
		{
			ft.chrg.cpMin = 0;
			ft.chrg.cpMax = -1; // the end
		}
		else
		{
			ft.chrg.cpMin = GetTextLength() - 1;
			ft.chrg.cpMax = 0;
		}

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
	
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = dwMask;

	if (GetSelectionCharFormat(cf))
		return (cf.dwEffects & dwEffect);

	// else
	return FALSE;
}

BOOL CRichEditBaseCtrl::SetSelectedEffect(DWORD dwMask, DWORD dwEffect)
{
	CharFormat cf;
	cf.dwMask = dwMask;
	cf.dwEffects = dwEffect;
	
	return SetSelectedEffect(cf);
}

BOOL CRichEditBaseCtrl::SetSelectedEffect(const CHARFORMAT2& cf)
{
	return SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
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
	ptCaret.x += 4; // estimate 1/2 char width
	
	return ptCaret;
}

void CRichEditBaseCtrl::MoveCaretToEnd()
{
	SetSel(-1, 0);
}

void CRichEditBaseCtrl::SetCaretPos(int nPos)
{
	SetSel(nPos, nPos);
}

int CRichEditBaseCtrl::CharFromPoint(const CPoint& point) const
{
	// Test if the point is beyond the end of the text
	int nLastChar = (GetWindowTextLength() - 1);

	CPoint ptLastChar;
	PointFromChar(nLastChar, ptLastChar);

	if (point.y > (ptLastChar.y + GetLineHeight()))
		return -1;

	POINTL ptl = { point.x, point.y };
	
	return ::SendMessage(GetSafeHwnd(), EM_CHARFROMPOS, 0, (LPARAM)&ptl);
}

void CRichEditBaseCtrl::PointFromChar(int nCharPos, CPoint& point) const
{
	POINTL ptl = { 0 };
	::SendMessage(GetSafeHwnd(), EM_POSFROMCHAR, (WPARAM)&ptl, nCharPos);

	point.x = ptl.x;
	point.y = ptl.y;
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

BOOL CRichEditBaseCtrl::Save(const CString& filename)
{
	CString str = GetRTF(); // returns multibyte encoded string
	
	// save as multibyte
	return FileMisc::SaveFile(filename, (LPCSTR)(LPCTSTR)str);
}

BOOL CRichEditBaseCtrl::Load(const CString& filename)
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
	// Don't notify our parent about this since
	// that's where it came from
	TemporarilyDisableChangeNotifications();

	CString sRTF = (rtf.IsEmpty() ? DEFAULTRTF : rtf);
	
	STREAMINCOOKIE cookie(sRTF);
	EDITSTREAM es = { (DWORD)&cookie, 0, StreamInCB };
	
	StreamIn(SF_RTF, es);
}

BOOL CRichEditBaseCtrl::EnableInlineSpellChecking(BOOL bEnable)
{
	if (!SupportsInlineSpellChecking())
		return FALSE;

	// Prevent spell-checking from causing a change notification
	if (bEnable)
		TemporarilyDisableChangeNotifications();

	ASSERT(GetSafeHwnd());

	EnableLanguageOptions(IMF_SPELLCHECKING, bEnable);
	EnableEditStyles((RECBES_USECTF | RECBES_CTFALLOWEMBED | RECBES_CTFALLOWSMARTTAG | RECBES_CTFALLOWPROOFING), bEnable);

	return TRUE;
}

BOOL CRichEditBaseCtrl::EnableAutoFontChanging(BOOL bEnable)
{
	return EnableLanguageOptions(IMF_AUTOFONT, bEnable);
}

BOOL CRichEditBaseCtrl::EnableLanguageOptions(DWORD dwOptions, BOOL bEnable)
{
	return EnableStateFlags(GetSafeHwnd(), 
							EM_GETLANGOPTIONS, 
							EM_SETLANGOPTIONS, 
							dwOptions, 
							bEnable);
}

BOOL CRichEditBaseCtrl::EnableEditStyles(DWORD dwStyles, BOOL bEnable)
{
	return EnableStateFlags(GetSafeHwnd(), 
							EM_GETEDITSTYLE, 
							EM_SETEDITSTYLE, 
							dwStyles, 
							bEnable);
}

BOOL CRichEditBaseCtrl::EnableStateFlags(HWND hWnd, UINT nGetMsg, UINT nSetMsg, DWORD dwFlags, BOOL bEnable)
{
	ASSERT(::IsWindow(hWnd));
	ASSERT(dwFlags);

	DWORD dwCurFlags = ::SendMessage(hWnd, nGetMsg, 0, 0), dwNewFlags(dwCurFlags);

	if (Misc::ModifyFlags(dwNewFlags, 
						(bEnable ? 0 : dwFlags),  // remove
						(bEnable ? dwFlags : 0))) // add
	{
		::SendMessage(hWnd, nSetMsg, 0, dwNewFlags);
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
	DWORD dwEditFlags = (RECBES_USECTF | RECBES_CTFALLOWEMBED | RECBES_CTFALLOWSMARTTAG | RECBES_CTFALLOWPROOFING);

	return (Misc::HasFlag(dwLangOpt, dwLangFlags) && Misc::HasFlag(dwEditStyle, dwEditFlags));
}

BOOL CRichEditBaseCtrl::SupportsInlineSpellChecking()
{
	return (COSVersion() >= OSV_WIN8);
}

CLIPFORMAT CRichEditBaseCtrl::GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format, 
														const CLIPFORMAT fmtPreferred[], int nNumFmts, BOOL bAllowFallback)
{
	CDWordArray aFormatIDs;
	
#ifdef _DEBUG
	CStringArray aFormatNames;
	CString sFormatNames, sFormatIDs;
	
	if (CClipboard::GetAvailableFormats(lpDataOb, aFormatIDs, aFormatNames))
	{
		sFormatNames = Misc::FormatArray(aFormatNames, ',', TRUE);
		sFormatIDs = Misc::FormatArray(aFormatIDs, ',');
	}
#else
	CClipboard::GetAvailableFormats(lpDataOb, aFormatIDs);
#endif
	
	for (int nFmt = 0; nFmt < nNumFmts; nFmt++)
	{
		UINT nFormat = fmtPreferred[nFmt];
		
		if (format && (format == nFormat))
			return format;
		
		if (Misc::HasT((DWORD)nFormat, aFormatIDs))
			return nFormat;
	}

	if (bAllowFallback)
	{
		// If a format was passed in then use that
		if (format)
			return format;

		// else try for plain text
		if (Misc::HasT((DWORD)CB_TEXTFORMAT, aFormatIDs))
			return CB_TEXTFORMAT;
	}

	return 0;
}

BOOL CRichEditBaseCtrl::EnableToolTips(BOOL bEnable)
{
	if (bEnable && !m_tooltip.GetSafeHwnd())
	{
		if (!m_tooltip.Create(this, (TTS_NOPREFIX | TTS_ALWAYSTIP)))
			return FALSE;

		m_tooltip.SetMaxTipWidth(1024); // for '\n' support
		return TRUE;
	}
	
	// else
	if (!bEnable && m_tooltip.GetSafeHwnd())
		return m_tooltip.DestroyToolTipCtrl();
	
	return TRUE;
}

BOOL CRichEditBaseCtrl::PreTranslateMessage(MSG* pMsg)
{
	FilterToolTipMessage(pMsg);
	
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

void CRichEditBaseCtrl::FilterToolTipMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.FilterToolTipMessage(pMsg);
}

LRESULT CRichEditBaseCtrl::OnToolHitTest(WPARAM wp, LPARAM lp)
{
	CPoint pt(wp);
	TOOLINFO* pTI = (TOOLINFO*)lp;
	
	return OnToolHitTest(pt, pTI);
}

int CRichEditBaseCtrl::OnToolHitTest(CPoint pt, TOOLINFO* pTI) const
{
	return CRichEditCtrl::OnToolHitTest(pt, pTI);
}

void CRichEditBaseCtrl::SetParaAlignment(int alignment)
{
	ParaFormat2	para(PFM_ALIGNMENT);
	para.wAlignment = (WORD) alignment;
	
	SetParaFormat(para);
}

int CRichEditBaseCtrl::GetParaAlignment() const
{
	ParaFormat pf(PFM_ALIGNMENT);
	GetParaFormat(pf);
				
	return (int)pf.wAlignment;
}

BOOL CRichEditBaseCtrl::SetSelectedWebLink(const CString& sWebLink, const CString& sText)
{
	CHARRANGE cr;
	GetSel(cr);

	CString sLinkText = (sText.IsEmpty() ? GetTextRange(cr) : sText);
	
	if (sLinkText.IsEmpty())
		return FALSE;

	CString sRTFLink;
	sRTFLink.Format(RTF_LINK, sWebLink, sLinkText);

	return SetTextEx(sRTFLink);
}

BOOL CRichEditBaseCtrl::EnableAutoUrlDetection(DWORD dwFlags)
{
	if (SendMessage(EM_AUTOURLDETECT, dwFlags, NULL) != 0)
		return FALSE;

	EnableEditStyles(RECBES_NOFOCUSLINKNOTIFY, (dwFlags != 0));
	SetEventMask(GetEventMask() | ENM_LINK);

	return TRUE;
}

BOOL CRichEditBaseCtrl::EnableAutoUrlDetection(const CStringArray& aProtocols, DWORD dwFlags)
{
	if (!dwFlags || !aProtocols.GetSize())
		return EnableAutoUrlDetection(dwFlags);

	// Only Windows 8 and above supports custom protocols
	if (COSVersion() < OSV_WIN8)
	{
		EnableAutoUrlDetection(FALSE);
		return FALSE;
	}

	// Build list of protocols
	CString sProtocols;
	int nProt = aProtocols.GetSize();

	while (nProt--)
	{
		CString sProtocol(aProtocols[nProt]);
		sProtocol.TrimRight(_T(":/"));

		sProtocols += sProtocol;
		sProtocols += ':';
	}

	if (SendMessage(EM_AUTOURLDETECT, dwFlags, (LPARAM)(LPCTSTR)sProtocols) != 0)
		return FALSE;
	
	EnableEditStyles(RECBES_NOFOCUSLINKNOTIFY, (dwFlags != 0));
	SetEventMask(GetEventMask() | ENM_LINK);

	return TRUE;
}

BOOL CRichEditBaseCtrl::IsAutoUrlDetectionEnabled() const
{
	return ::SendMessage(GetSafeHwnd(), EM_GETAUTOURLDETECT, 0, 0);
}

BOOL CRichEditBaseCtrl::IsStartOfLine(int nCharPos) const
{
	int nLine = ::SendMessage(GetSafeHwnd(), EM_LINEFROMCHAR, nCharPos, 0);
	int nLineStart = ::SendMessage(GetSafeHwnd(), EM_LINEINDEX, nLine, 0);

	return (nCharPos == nLineStart);
}

BOOL CRichEditBaseCtrl::IsEndOfLine(int nCharPos) const
{
	int nLine = ::SendMessage(GetSafeHwnd(), EM_LINEFROMCHAR, nCharPos, 0);
	int nLineStart = ::SendMessage(GetSafeHwnd(), EM_LINEINDEX, nLine, 0);
	int nLineLen = ::SendMessage(GetSafeHwnd(), EM_LINELENGTH, nLineStart, 0);
	int nEndPosOfLine = (nLineStart + nLineLen);

	return (nCharPos == nEndPosOfLine);
}

void CRichEditBaseCtrl::TemporarilyDisableChangeNotifications()
{
	if ((GetEventMask() & ENM_CHANGE) == ENM_CHANGE)
	{
		SetEventMask(GetEventMask() & ~ENM_CHANGE);
		PostMessage(WM_REBC_REENABLECHANGENOTIFY);
	}
}

LRESULT CRichEditBaseCtrl::OnReenableChangeNotifications(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	ASSERT((GetEventMask() & ENM_CHANGE) == 0);

	SetEventMask(GetEventMask() | ENM_CHANGE);
	return 0L;
}
