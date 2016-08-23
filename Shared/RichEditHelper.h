// RichEditHelper.h: interface for the CRichEditHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHEDITHELPER_H__C498C86D_613F_42AD_9C93_6C773E6368E8__INCLUDED_)
#define AFX_RICHEDITHELPER_H__C498C86D_613F_42AD_9C93_6C773E6368E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <richedit.h>
#include <richole.h>

#if !defined EM_SETTYPOGRAPHYOPTIONS
#	define EM_SETTYPOGRAPHYOPTIONS (WM_USER + 202)
#	define EM_GETTYPOGRAPHYOPTIONS (WM_USER + 203)
#	define TO_ADVANCEDTYPOGRAPHY 1
#	define TO_SIMPLELINEBREAK 2 
#endif
struct ITextDocument;

enum RE_PASTE
{
	REP_ASFILEURL,
	REP_ASICONCOPY,
	REP_ASICONLINK,
	REP_ASIMAGE,
};

class CReBase
{
public:
	CReBase(HWND hwndRichEdit);
	virtual ~CReBase();

protected:
	HWND m_hwndRichedit;
};

class CTextDocument : public CReBase
{
public:
	CTextDocument(HWND hwndRichEdit);
	virtual ~CTextDocument();
	
	BOOL Valid() const { return (m_pDoc != NULL);	}
	operator ITextDocument*() { return m_pDoc; }

	BOOL Undo();
	BOOL Redo();
	
protected:
	ITextDocument* m_pDoc;
	LPRICHEDITOLE m_pRichEditOle;
};

class CRePauseUndo : public CTextDocument
{
public:
	CRePauseUndo(HWND hwndRichEdit);
	virtual ~CRePauseUndo();
};

class CReSaveCaret : public CReBase
{
public:
	CReSaveCaret(HWND hwndRichEdit);
	virtual ~CReSaveCaret();

protected:
	long m_lCaret;
};

class CReFileObject : public CReBase
{
public:
	CReFileObject(HWND hwndRichEdit);
	virtual ~CReFileObject();
	
	BOOL Insert(LPCTSTR szFilePath, BOOL bLink = FALSE);
	
protected:
	LPRICHEDITOLE m_pRichEditOle;
	LPOLEOBJECT m_pObject;
	LPSTORAGE m_pStorage;
	LPOLECLIENTSITE m_pClientSite;
	
protected:
	void Reset();
	
	BOOL Create(LPCTSTR szFilePath, BOOL bLink = FALSE);
	BOOL GetReObject(REOBJECT& reObj) const;
	
};

class CRichEditHelper
{
public:
	static BOOL CreateRichEdit20(CWnd& wnd, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle = 0, BOOL bAutoRTL = TRUE);
	static BOOL CreateRichEdit50(CWnd& wnd, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle = 0, BOOL bAutoRTL = TRUE);
	static BOOL InitRichEdit();
	static void ClearUndo(HWND hWnd);

	static BOOL PasteFile(HWND hWnd, LPCTSTR szFilePath, RE_PASTE nPasteHow);
	static BOOL PasteFiles(HWND hWnd, const CStringArray& aFiles, RE_PASTE nPasteHow);

protected:
	static BOOL PasteFileInternal(HWND hWnd, LPCTSTR szFilePath, RE_PASTE nPasteHow, BOOL& bUsedClipboard);
	static BOOL CreateRichEdit(CWnd& wnd, LPCTSTR szClass, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle, BOOL bAutoRTL);

};

#endif // !defined(AFX_RICHEDITHELPER_H__C498C86D_613F_42AD_9C93_6C773E6368E8__INCLUDED_)
