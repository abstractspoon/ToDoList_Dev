// ISMsgManager.h: interface for the CISMsgManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISMSGMANAGER_H__975F4620_557F_44FB_81FF_ECD5C55D9015__INCLUDED_)
#define AFX_ISMSGMANAGER_H__975F4620_557F_44FB_81FF_ECD5C55D9015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CISMsgHandler
{
public:
	CISMsgHandler(UINT uMsg, LPCTSTR szMsg, BOOL bHasParams = TRUE) : 
		m_uMsg(uMsg), m_sMsg(szMsg), m_bHasParams(bHasParams) {}
	virtual ~CISMsgHandler() {}
	
	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;
		return TRUE;
	}

	inline BOOL HasParams() { return m_bHasParams; }

	static void AddParam(CStringArray* pParams, const char *fmt, ...)
	{
		ASSERT (pParams);

		static char gBuffer[1024];
		ZeroMemory(gBuffer, 1024);
		
		va_list marker;
		va_start(marker, fmt);
		vsprintf(gBuffer, fmt, marker);
		va_end(marker);
		
		pParams->Add(gBuffer);
	}
	
protected:
	UINT m_uMsg;
	CString m_sMsg;
	BOOL m_bHasParams;

};

class CISMsgManager
{
public:
	virtual ~CISMsgManager();
	static BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams = NULL);

protected:
	static CISMsgManager& GetInstance() { static CISMsgManager isMsgMgr; return isMsgMgr; }

protected:
	CISMsgManager();

	BOOL AddHandler(UINT uMsg, CISMsgHandler* pMsgHandler);
	CISMsgHandler* GetHandler(UINT uMsg);
	void CreateMessageMap();
	void DeleteMessageMap();

protected:
	CMap<UINT, UINT, CISMsgHandler*, CISMsgHandler*&> m_mapHandlers;
  CMap<UINT, UINT, CString, CString&> m_mapRegisteredMsgs;

  BOOL GetRegisteredMessageName(UINT nMsg, CString& sMsgName);
  BOOL GetUserMessageName(UINT nMsg, HWND hWnd, CString& sMsgName);
};


#endif // !defined(AFX_ISMSGMANAGER_H__975F4620_557F_44FB_81FF_ECD5C55D9015__INCLUDED_)
