#if !defined(AFX_STICKIESWND_H__CD9AB24A_FE93_46D1_98FA_FC1A192427DE__INCLUDED_)
#define AFX_STICKIESWND_H__CD9AB24A_FE93_46D1_98FA_FC1A192427DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// stickiesapi.h : header file
//

#include "StickiesAPI.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CStickiesAPI window

class CStickiesWnd : protected CWnd
{
// Construction
public:
	CStickiesWnd(const CString& sStickiesPath = _T(""));

	BOOL Initialize(CWnd* pOwner, const CString& sStickiesPath = _T(""));
	void Release();
	BOOL IsValid() const { return m_stickies.IsValid(); }

	BOOL CreateSticky(const CString& sTitle, CString& sStickyID, const CString& sContent = _T(""), time_t tAlarm = 0, COLORREF color = 0); // returns sticky ID
	BOOL DeleteSticky(const CString& sStickyID); 
	BOOL HasSticky(const CString& sStickyID);

	BOOL SetStickyTitle(const CString& sStickyID, const CString& sTitle);
	BOOL SetStickyContent(const CString& sStickyID, const CString& sContent);
	BOOL SetStickyAttribute(const CString& sStickyID, const CString& sAttribute, const CString& sValue);

	LRESULT SendMessage(const CString& sCommand, CString& sReply, const CString& sStickyID = _T(""), const CString& sExtra = _T(""));
	LRESULT SendMessage(const CString& sCommand);

// Attributes
protected:
	CMap<int, int, CString, CString&> m_mapReplies;
	int m_nCommandID;
	CStickiesAPI m_stickies;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStickiesAPI)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStickiesWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStickiesAPI)
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL InitStickiesAPI(const CString& sStickiesPath);

	static CString FormatStickyCreationString(const CString& sTitle, time_t tAlarm, COLORREF color);
	static CString EncodeString(const CString& sText);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STICKIESWND_H__CD9AB24A_FE93_46D1_98FA_FC1A192427DE__INCLUDED_)
