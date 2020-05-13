#if !defined(AFX_TDCSIMPLETEXTCONTENT_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_)
#define AFX_TDCSIMPLETEXTCONTENT_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tdlcommentsctrl.h : header file
//

#include "..\shared\icon.h"

#include "..\Interfaces\IContentControl.h"

/////////////////////////////////////////////////////////////////////////////
// CTDCSimpleTextContent

class CTDCSimpleTextContent : public IContent
{
public:
	CTDCSimpleTextContent();
	virtual ~CTDCSimpleTextContent();

	LPCTSTR GetTypeID() const;
	LPCTSTR GetTypeDescription() const;
	HICON GetTypeIcon() const;

	IContentControl* CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
								long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

	void SetLocalizer(ITransText* pTT);
	void Release();

	int ConvertToHtml(const unsigned char* pContent, int nLength, LPCTSTR szCharset,
						LPTSTR& szHtml, LPCTSTR szImageDir);
	void FreeHtmlBuffer(LPTSTR& szHtml);

	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);

protected:
	CIcon m_icon;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDCSIMPLETEXTCONTENT_H__E9A5D982_4D40_43B2_A071_E3BE70D122B7__INCLUDED_)
