// EDialog.h: interface for the EDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGEBOX_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_)
#define AFX_MESSAGEBOX_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////////////////////////

class CMessageBox
{
public:
	static void SetAppName(const CString& sAppName);
	static void DisableSimpleErrorMessages(BOOL bDisable = TRUE) { s_bDisableSimpleErrorMessages = bDisable; }

	static int Show(HWND hwndParent, const CString& sCaption, const CString& sInstruction, const CString& sText, UINT nFlags = MB_OK);
	static int Show(const CWnd* pWnd, const CString& sCaption, const CString& sInstruction, const CString& sText, UINT nFlags = MB_OK);

	static int AfxShow(HWND hwndParent, const CString& sInstruction, const CString& sText, UINT nFlags = MB_OK);
	static int AfxShow(const CWnd* pWnd, const CString& sInstruction, const CString& sText, UINT nFlags = MB_OK);
	
	static int AfxShow(const CString& sInstruction, const CString& sText, UINT nFlags = MB_OK);
	static int AfxShow(UINT nInstructionID, const CString& sText, UINT nFlags = MB_OK);
	static int AfxShow(UINT nInstructionID, UINT nTextID, UINT nFlags);

	static int AfxShow(const CString& sText, UINT nFlags = MB_OK);
	static int AfxShow(UINT nTextID, UINT nFlags = MB_OK);

protected:
	static CString s_sAppName;
	static BOOL s_bDisableSimpleErrorMessages;
};

#endif // !defined(AFX_MESSAGEBOX_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_)
