// RTFContentCtrl.h : main header file for the RTFCONTENTCTRL DLL
//

#if !defined(AFX_RTFCONTENTCTRL_H__2C6426B5_72CA_4C0F_9CE1_A03CD5C833C7__INCLUDED_)
#define AFX_RTFCONTENTCTRL_H__2C6426B5_72CA_4C0F_9CE1_A03CD5C833C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "resource.h"		// main symbols

#include "..\shared\Rtf2HtmlConverter.h"

#include "..\Interfaces\icontentcontrol.h"

//////////////////////////////////////////////////////////////////////

const LPCTSTR RTF_TYPEID = _T("849CF988-79FE-418A-A40D-01FE3AFCAB2C");

/////////////////////////////////////////////////////////////////////////////

class CRTFContentCtrlApp : public IContent, public CWinApp
{
public:
	CRTFContentCtrlApp();

	// IContent implementation
	LPCTSTR GetTypeID() const { return RTF_TYPEID; }
	LPCTSTR GetTypeDescription() const { return _T("Rich Text"); }
	HICON GetTypeIcon() const { return m_hIcon; }

	void SetLocalizer(ITransText* pTT);

	IContentControl* CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
						long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);
	void Release();

	int ConvertToHtml(const unsigned char* pContent, int nLength,
					  LPCTSTR szCharSet, LPTSTR& szHtml, LPCTSTR szImageDir);
	void FreeHtmlBuffer(LPTSTR& szHtml);

	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);

protected:
	CRtfHtmlConverter m_rtfHtml;
	HICON m_hIcon;

protected:
	BOOL InitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTFCONTENTCTRL_H__2C6426B5_72CA_4C0F_9CE1_A03CD5C833C7__INCLUDED_)
