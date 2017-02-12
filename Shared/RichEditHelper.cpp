// RichEditHelper.cpp: implementation of the CRichEditHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RichEditHelper.h"
#include "wclassdefines.h"

#include "..\3rdParty\TOM.h"
#include "..\3rdParty\clipboardbackup.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\webmisc.h"
#include "..\shared\enbitmap.h"
#include "..\shared\clipboard.h"

#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EXTERN_C const GUID CDECL IID_ITextDocument = 
	{ 0x8CC497C0, 0xA1DF, 0x11CE, { 0x80, 0x98, 0x00, 0xAA, 0x00, 0x47, 0xBE, 0x5D} };

#define RELEASE_INTERFACE(i) if (i) { i->Release(); i = NULL; }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReBase::CReBase(HWND hwndRichEdit) : m_hwndRichedit(hwndRichEdit)
{
}

CReBase::~CReBase()
{
	m_hwndRichedit = NULL;
}

//////////////////////////////////////////////////////////////////////

CReSaveCaret::CReSaveCaret(HWND hwndRichEdit) : CReBase(hwndRichEdit)
{
	if (m_hwndRichedit)
	{
		CHARRANGE cr;
		SendMessage(m_hwndRichedit, EM_EXGETSEL, 0, (LPARAM)&cr);

		// if the caret is at the end of the text then set it to LONG_MAX
		if (cr.cpMax == SendMessage(m_hwndRichedit, WM_GETTEXTLENGTH, 0, 0))
			m_lCaret = LONG_MAX;
		else
			m_lCaret = cr.cpMax;
	}
}

CReSaveCaret::~CReSaveCaret()
{
	if (m_hwndRichedit)
		SendMessage(m_hwndRichedit, EM_SETSEL, m_lCaret, m_lCaret);
}

//////////////////////////////////////////////////////////////////////

CTextDocument::CTextDocument(HWND hwndRichEdit) : CReBase(hwndRichEdit), m_pDoc(NULL), m_pRichEditOle(NULL)
{
	if (m_hwndRichedit)
	{
		SendMessage(hwndRichEdit, EM_GETOLEINTERFACE, 0, (LPARAM)&m_pRichEditOle);

		if (m_pRichEditOle)
			m_pRichEditOle->QueryInterface(IID_ITextDocument, (void**)&m_pDoc);
	}
}

CTextDocument::~CTextDocument()
{
	if (m_pDoc)
		m_pDoc->Release();

	if (m_pRichEditOle)
		m_pRichEditOle->Release();
}

BOOL CTextDocument::Undo()
{
	return Valid() && (m_pDoc->Undo(1, NULL) == S_OK);
}

BOOL CTextDocument::Redo()
{
	return Valid() && (m_pDoc->Redo(1, NULL) == S_OK);
}

//////////////////////////////////////////////////////////////////////

CRePauseUndo::CRePauseUndo(HWND hwndRichEdit) : CTextDocument(hwndRichEdit)
{
	if (m_hwndRichedit && Valid())
		m_pDoc->Undo(tomSuspend, NULL);
}

CRePauseUndo::~CRePauseUndo()
{
	if (m_hwndRichedit && Valid())
		m_pDoc->Undo(tomResume, NULL);
}

//////////////////////////////////////////////////////////////////////

BOOL CRichEditHelper::CreateRichEdit20(CWnd& wnd, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle, BOOL bAutoRTL)
{
	return CreateRichEdit(wnd, WC_RICHEDIT20, dwStyle, rect, pParentWnd, nID, dwExStyle, bAutoRTL);
}

BOOL CRichEditHelper::CreateRichEdit50(CWnd& wnd, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle, BOOL bAutoRTL)
{
	return CreateRichEdit(wnd, WC_RICHEDIT50, dwStyle, rect, pParentWnd, nID, dwExStyle, bAutoRTL);
}

BOOL CRichEditHelper::CreateRichEdit(CWnd& wnd, LPCTSTR szClass, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle, BOOL bAutoRTL)
{
	InitRichEdit();
	
	if (bAutoRTL)
	{
		ASSERT(pParentWnd);
		DWORD dwParentExStyle = pParentWnd->GetExStyle();
		
		if (Misc::HasFlag(dwParentExStyle, WS_EX_RTLREADING))
			dwExStyle |= WS_EX_RTLREADING;
	}

	return wnd.CreateEx(dwExStyle, szClass, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CRichEditHelper::InitRichEdit()
{
	static HINSTANCE hRichEdit20 = NULL;
	static HINSTANCE hRichEdit50 = NULL;

	if (!AfxInitRichEdit())
		return FALSE;

	if (!hRichEdit20)
		hRichEdit20 = LoadLibrary(_T("riched20.dll"));

	if (!hRichEdit50)
		hRichEdit50 = LoadLibrary(_T("msftedit.dll"));

	return (hRichEdit20 != NULL || hRichEdit50 != NULL);
}

void CRichEditHelper::ClearUndo(HWND hWnd)
{
	ASSERT(hWnd);

	ITextDocument *pDoc;
	IUnknown *pUnk = NULL;
	
	SendMessage(hWnd, EM_GETOLEINTERFACE, 0, (LPARAM)&pUnk);
	
	if (pUnk)
	{
		if (pUnk->QueryInterface(IID_ITextDocument, (void**)&pDoc) == NOERROR)
		{
			pDoc->Undo(tomFalse, NULL);	// Prevents Undo and empties buffer.
			pDoc->Undo(tomTrue, NULL);  // Restarts Undo again
			pDoc->Release();
		}
		pUnk->Release();
	}
}

BOOL CRichEditHelper::PasteFiles(HWND hWnd, const CStringArray& aFiles, RE_PASTE nPasteHow)
{
	if (aFiles.GetSize() == 0)
		return FALSE;

	// save backup because we may be overwriting it
 	CClipboardBackup cbb(hWnd);
 	cbb.Backup();

	BOOL bRestoreClipboard = FALSE;

	UINT nFileCount = aFiles.GetSize(), nFilesPasted = 0;
	
	for (UINT i=0; i < nFileCount; i++)
	{
		const CString& sFile = Misc::GetItem(aFiles, i);
		BOOL bUsedClipboard = FALSE;

		if (PasteFileInternal(hWnd, sFile, nPasteHow, bUsedClipboard))
		{
			bRestoreClipboard |= bUsedClipboard;
			nFilesPasted++;
		}
	}

	if (bRestoreClipboard)
		cbb.Restore();

	return (nFileCount == nFilesPasted);
}

BOOL CRichEditHelper::PasteFile(HWND hWnd, LPCTSTR szFilePath, RE_PASTE nPasteHow)
{
	CStringArray aFiles;
	aFiles.Add(szFilePath);

	return PasteFiles(hWnd, aFiles, nPasteHow);
}

BOOL CRichEditHelper::PasteFileInternal(HWND hWnd, LPCTSTR szFilePath, RE_PASTE nPasteHow, BOOL& bUsedClipboard)
{
	bUsedClipboard = FALSE;

	if (FileMisc::PathExists(szFilePath)) // can be file or folder
	{
		switch (nPasteHow)
		{
		case REP_ASIMAGE:
			if (CEnBitmap::IsSupportedImageFile(szFilePath))
			{
				if (CEnBitmap::CopyImageFileToClipboard(hWnd, szFilePath, CLR_NONE, 8))
				{
					::SendMessage(hWnd, EM_PASTESPECIAL, CF_BITMAP, NULL);
					bUsedClipboard = TRUE;
	
					return TRUE;
				}
			}
			// else fall thru

		case REP_ASICONCOPY:
		case REP_ASICONLINK:
			// don't embed folders EVER!
			// or executables
			if (!FileMisc::FolderExists(szFilePath) && 
				!FileMisc::HasExtension(szFilePath, _T("exe")) && 
				CReFileObject(hWnd).Insert(szFilePath, (nPasteHow == REP_ASICONLINK)))
			{
				return TRUE;
			}
			// else fall thru

		case REP_ASFILEURL:
			{
				CString sLink;
				
				if (WebMisc::FormatFileURI(szFilePath, sLink, FALSE))
				{
					// if the path contains spaces then brace it
					if (sLink.Find(' ') != -1)
					{
						sLink = _T(" <") + sLink + _T(">");
					}
					else // add leading space
					{
						sLink = ' ' + sLink;
					}
				}

				// newline
				sLink += _T("\r\n");
	
				::SendMessage(hWnd, EM_REPLACESEL, TRUE, (LPARAM)(LPCTSTR)sLink);
			}
			// else fall thru

		default:
			break;
		}
	}

	// all else
    return FALSE;
}

//////////////////////////////////////////////////////////////////////

CReFileObject::CReFileObject(HWND hwndRichEdit) : 
   CReBase(hwndRichEdit), m_pObject(NULL), m_pClientSite(NULL), m_pRichEditOle(NULL), m_pStorage(NULL)

{
	SendMessage(m_hwndRichedit, EM_GETOLEINTERFACE, 0, (LPARAM)&m_pRichEditOle);
}

CReFileObject::~CReFileObject()
{
   Reset();
   RELEASE_INTERFACE(m_pRichEditOle);
}

BOOL CReFileObject::Create(LPCTSTR szFilePath, BOOL bLink)
{
   if (!m_pRichEditOle)
      return FALSE;
   
   if (!FileMisc::FileExists(szFilePath))
      return FALSE;

   Reset();

   USES_CONVERSION;

   LPLOCKBYTES pLockBytes = NULL;
   CLSID clsid = CLSID_NULL;

   SCODE sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &pLockBytes);
   ASSERT (pLockBytes);

   if (sc != S_OK)
      return FALSE;

   sc = ::StgCreateDocfileOnILockBytes(pLockBytes,
            STGM_SHARE_EXCLUSIVE |
            STGM_CREATE |
            STGM_READWRITE,
            0, &m_pStorage);

   if (sc != S_OK)
   {
      pLockBytes->Release();
      return FALSE;
   }

   FORMATETC formatEtc;

   formatEtc.cfFormat = 0;
   formatEtc.ptd = NULL;
   formatEtc.dwAspect = DVASPECT_ICON;
   formatEtc.lindex = -1;
   formatEtc.tymed = TYMED_NULL;

   // create object
   m_pRichEditOle->GetClientSite(&m_pClientSite);

   if (bLink)
   {
	   sc = ::OleCreateLinkToFile(T2OLE((LPTSTR)szFilePath),
							   IID_IUnknown,
							   OLERENDER_DRAW,
							   &formatEtc,
							   m_pClientSite,
							   m_pStorage,
							   (void**)&m_pObject);
   }
   else
   {
	   sc = ::OleCreateFromFile(clsid, T2OLE((LPTSTR)szFilePath),
							   IID_IUnknown,
							   OLERENDER_DRAW,
							   &formatEtc,
							   m_pClientSite,
							   m_pStorage,
							   (void**)&m_pObject);
   }

   if (m_pObject)
   {
      // convert to IOleObject
      LPUNKNOWN pUnk = m_pObject;

      pUnk->QueryInterface(IID_IOleObject, (void**)&m_pObject);
      pUnk->Release();
   }

   if (!m_pObject)
      Reset();
   else
	   OleSetContainedObject(m_pObject, TRUE);

   return (m_pObject != NULL);
}

BOOL CReFileObject::GetReObject(REOBJECT& reObj) const
{
   if (!m_pObject)
      return FALSE;

   ZeroMemory(&reObj, sizeof(reObj));

   reObj.cbStruct = sizeof(reObj);

   CLSID clsid;
   SCODE sc = m_pObject->GetUserClassID(&clsid);

   if (sc != S_OK)
      return FALSE;

   reObj.clsid = clsid;
   reObj.cp = REO_CP_SELECTION;
   reObj.dvaspect = DVASPECT_ICON;
   reObj.dwFlags = REO_RESIZABLE | REO_BELOWBASELINE;
   reObj.dwUser = 0;
   reObj.poleobj = m_pObject;
   reObj.polesite = m_pClientSite;
   reObj.pstg = m_pStorage;
   reObj.sizel.cx = reObj.sizel.cy = 0;

   return TRUE;
}


void CReFileObject::Reset()
{
   RELEASE_INTERFACE(m_pObject);
   RELEASE_INTERFACE(m_pStorage);
   RELEASE_INTERFACE(m_pClientSite);
}

BOOL CReFileObject::Insert(LPCTSTR szFilePath, BOOL bLink)
{
   if (Create(szFilePath, bLink))
   {
      REOBJECT reObj;

      if (GetReObject(reObj))
         return (S_OK == m_pRichEditOle->InsertObject(&reObj));
   }

   return FALSE;
}

//////////////////////////////////////////////////////////////////////
