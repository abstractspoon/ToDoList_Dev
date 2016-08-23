#if !defined(AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_)
#define AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_

// RulerRichEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEdit window

#include "..\shared\urlricheditctrl.h"
#include "..\shared\richedithelper.h"

/////////////////////////////////////////////////////////////////////////////
// Helper structs

struct CharFormat : public CHARFORMAT2
{
	CharFormat(DWORD mask = 0)
	{
		Reset(mask);
	};

	void Reset(DWORD mask = 0)
	{
		memset(this, 0, sizeof (CharFormat));
		cbSize = sizeof(CharFormat);
		dwMask = mask;
	};

	BOOL operator==(const CharFormat& cf)
	{
		return (memcmp(this, &cf, sizeof(cf)) == 0);
	}

};

struct ParaFormat2 : public PARAFORMAT2
{
	ParaFormat2(DWORD mask = 0)
	{
		Reset(mask);
	}

	void Reset(DWORD mask = 0)
	{
		memset(this, 0, sizeof (ParaFormat2));
		cbSize = sizeof(ParaFormat2);
		dwMask = mask;
	}

	BOOL HasTabs() const
	{
		// find the first non-zero tabstop
		int t = MAX_TAB_STOPS;
		
		while (t--)
		{
			if (rgxTabs[t] != 0)
				return TRUE;
		}
		
		return FALSE; // all zeroes
	}

};

struct ParaFormat : public PARAFORMAT
{
	ParaFormat(DWORD mask = 0)
	{
		Reset(mask);
	}

	void Reset(DWORD mask = 0)
	{
		memset(this, 0, sizeof (ParaFormat));
		cbSize = sizeof(ParaFormat);
		dwMask = mask;
	}

	BOOL HasTabs() const
	{
		// find the first non-zero tabstop
		int t = MAX_TAB_STOPS;
		
		while (t--)
		{
			if (rgxTabs[t] != 0)
				return TRUE;
		}
		
		return FALSE; // all zeroes
	}
};

/////////////////////////////////////////////////////////////////////////////

class CRulerRichEdit : public CUrlRichEditCtrl
{
public:
// Construction/destruction
	CRulerRichEdit();
	virtual ~CRulerRichEdit();

	BOOL IsIMEComposing() const { return m_bIMEComposing; }

	void SetFileLinkOption(RE_PASTE nLinkOption, BOOL bDefault);
	RE_PASTE GetFileLinkOption() const { return m_nFileLinkOption; }
	BOOL IsFileLinkOptionDefault() const { return m_bLinkOptionIsDefault; }
	BOOL AppendSourceUrls(LPCTSTR szUrls);

protected:
	BOOL m_bIMEComposing;
	RE_PASTE m_nFileLinkOption;
	BOOL m_bLinkOptionIsDefault;

protected:
// Message handlers
	//{{AFX_MSG(CRulerRichEdit)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnDropFiles(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnIMEStartComposition(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnIMEEndComposition(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

	virtual CLIPFORMAT GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format);
	virtual HRESULT GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
	virtual HRESULT GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR* lpchrg,
									HMENU FAR* lphmenu);

	virtual CFindReplaceDialog* NewFindReplaceDlg();
};

#endif // !defined(AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_)
