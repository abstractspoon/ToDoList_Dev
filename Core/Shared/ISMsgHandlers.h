// ISMsgHandlers.h: interface for the CISMsgHandler derived classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISMSGHANDLERS_H__F4E70803_C655_4E97_9993_D927A8219B26__INCLUDED_)
#define AFX_ISMSGHANDLERS_H__F4E70803_C655_4E97_9993_D927A8219B26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ISMsgManager.h"
#include "winclasses.h"
#include "wclassdefines.h"

#include <dbt.h>
#include <afxpriv.h>

#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#	define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

#define BEGINNOTIFY(cls) if (sClass.CompareNoCase(cls) == 0) { switch (nNotifyCode) { default:break;
#define ENDNOTIFY } }

#define CASE2STRING(item) case item: return #item; 
#define ADDOPTION(option, flags, string) if ((flags & option) == option) { if (!string.IsEmpty()) string += ", "; string += #option; }

#define WP(msg) msg->wParam
#define LP(msg) msg->lParam

// decided this should be global because a few classes needed it
CString MapHitTest(UINT uHitTest)
{
	switch (uHitTest)
	{
		CASE2STRING(HTTRANSPARENT)
		CASE2STRING(HTNOWHERE)
		CASE2STRING(HTCLIENT)
		CASE2STRING(HTCAPTION)
		CASE2STRING(HTSYSMENU)
		CASE2STRING(HTSIZE)
		CASE2STRING(HTMENU)
		CASE2STRING(HTHSCROLL)
		CASE2STRING(HTVSCROLL)
		CASE2STRING(HTMINBUTTON)
		CASE2STRING(HTMAXBUTTON)
		CASE2STRING(HTLEFT)
		CASE2STRING(HTRIGHT)
		CASE2STRING(HTTOP)
		CASE2STRING(HTTOPLEFT)
		CASE2STRING(HTTOPRIGHT)
		CASE2STRING(HTBOTTOM)
		CASE2STRING(HTBOTTOMLEFT)
		CASE2STRING(HTBOTTOMRIGHT)
		CASE2STRING(HTBORDER)
		CASE2STRING(HTOBJECT)
		CASE2STRING(HTCLOSE)
		CASE2STRING(HTHELP)
			
	default:
		CASE2STRING(HTERROR)
	}
}


/* bare bones class
class CISMH_ : public CISMsgHandler
{
public:
	CISMH_(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};
*/

// WM_CREATE
class CISMH_Create : public CISMsgHandler
{
public:
	CISMH_Create(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPCREATESTRUCT pCS = (LPCREATESTRUCT)LP(pMsg);
		ASSERT (pCS);

		CString sClass;
		::GetClassName(pMsg->hwnd, sClass.GetBuffer(64), 64);
		sClass.ReleaseBuffer();

/*		// class name could be an atom
		if (!HIWORD(pCS->lpszClass) || (sClass.IsEmpty() && pCS->lpszClass))
		{
			int nRet = GlobalGetAtomName((ATOM)pCS->lpszClass, sClass.GetBuffer(128), 128);
			sClass.ReleaseBuffer();
		}
*/
		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = LPCREATESTRUCT");
		AddParam(pParams, "   params: 0x%08X", pCS->lpCreateParams);
		AddParam(pParams, "   instance: 0x%08X", pCS->hInstance);
		AddParam(pParams, "   hmenu: 0x%08X", pCS->hMenu);
		AddParam(pParams, "   hwndParent: 0x%08X", pCS->hwndParent);
		AddParam(pParams, "   class: %s", sClass);
		AddParam(pParams, "   text/caption: %s", pCS->lpszName);
		AddParam(pParams, "   x: %d", pCS->x);
		AddParam(pParams, "   y: %d", pCS->y);
		AddParam(pParams, "   cx: %d", pCS->cx);
		AddParam(pParams, "   cy: %d", pCS->cy);
		AddParam(pParams, "   style: 0x%08X", pCS->style);
		AddParam(pParams, "   extended style: 0x%08X", pCS->dwExStyle);

		return TRUE;
	}
	
};

// WM_L/R/MBUTTONDOWN/UP/DBLCLK
class CISMH_Mouse : public CISMsgHandler
{
public:
	CISMH_Mouse(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flags: 0x%04X", LOWORD(WP(pMsg)));
		AddParam(pParams, "lp = client cursor pos: (%d, %d)", GET_X_LPARAM(LP(pMsg)), GET_Y_LPARAM(LP(pMsg)));

		return TRUE;
	}
	
};

// WM_PRINT/CLIENT
class CISMH_Print : public CISMsgHandler
{
public:
	CISMH_Print(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hdc: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = options: %s", MapOptions(LP(pMsg)));

		return TRUE;
	}
	
	CString MapOptions(UINT uOptions)
	{
		CString sOptions;

		ADDOPTION(PRF_CHECKVISIBLE, uOptions, sOptions);
		ADDOPTION(PRF_CHILDREN, uOptions, sOptions);
		ADDOPTION(PRF_CLIENT, uOptions, sOptions);
		ADDOPTION(PRF_ERASEBKGND, uOptions, sOptions);
		ADDOPTION(PRF_NONCLIENT, uOptions, sOptions);
		ADDOPTION(PRF_OWNED, uOptions, sOptions);

		return sOptions;
	}
};
// WM_SHOWWINDOW
class CISMH_Show : public CISMsgHandler
{
public:
	CISMH_Show(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: %s", WP(pMsg) ? "show" : "hide");
		AddParam(pParams, "lp = status: %s", MapStatus(LP(pMsg)));

		return TRUE;
	}
	
	CString MapStatus(UINT uStatus)
	{
		switch (uStatus)
		{
			CASE2STRING(SW_OTHERUNZOOM)
			CASE2STRING(SW_OTHERZOOM)
			CASE2STRING(SW_PARENTCLOSING)
			CASE2STRING(SW_PARENTOPENING)
		}

		return "ShowWindow() called";
	}
};

// WM_NOTIFY
class CISMH_Notify : public CISMsgHandler
{
public:
	CISMH_Notify(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

    LPNMHDR pNMHDR = (LPNMHDR)LP(pMsg);
    CString sCode = MapCode(pNMHDR->code);

    // encode the actual notification into the message name
    if (!sCode.IsEmpty())
      sMsg.Format("%s (%s)", m_sMsg, sCode);
    else
      sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = control id: 0x%04X (%d)", WP(pMsg), WP(pMsg));

		if (pNMHDR->code == NM_CUSTOMDRAW)
		{
			AddParam(pParams, "lp = LPNMCUSTOMDRAW");

			AddParam(pParams, "   NMHDR.hwnd: 0x%08X", pNMHDR->hwndFrom);
			AddParam(pParams, "   NMDHR.control id: 0x%04X", pNMHDR->idFrom, pNMHDR->idFrom);
			AddParam(pParams, "   NMHDR.code: NM_CUSTOMDRAW");

			LPNMCUSTOMDRAW pNMCD = (LPNMCUSTOMDRAW)LP(pMsg);

			AddParam(pParams, "   draw stage: %s", MapDrawStage(pNMCD->dwDrawStage));
			AddParam(pParams, "   hdc: 0x%08X", pNMCD->hdc);
			AddParam(pParams, "   rect: (%d, %d, %d, %d)", pNMCD->rc.left, pNMCD->rc.top, pNMCD->rc.right, pNMCD->rc.bottom);
			AddParam(pParams, "   item spec: 0x%08X", pNMCD->dwItemSpec);
			AddParam(pParams, "   item state: 0x%08X", pNMCD->uItemState);
			AddParam(pParams, "   item lparam: 0x%08X", pNMCD->lItemlParam);
		}
		else
		{
			AddParam(pParams, "   hwnd: 0x%08X", pNMHDR->hwndFrom);
			AddParam(pParams, "   control id: 0x%04X", pNMHDR->idFrom, pNMHDR->idFrom);
			AddParam(pParams, "   code: %s", sCode);
		}

		return TRUE;
	}
	
	CString MapCode(UINT uCode)
	{
		switch (uCode)
		{
			CASE2STRING(NM_CLICK)
			CASE2STRING(NM_DBLCLK)
			CASE2STRING(NM_RCLICK)
			CASE2STRING(NM_RDBLCLK)
			CASE2STRING(NM_RETURN)
			CASE2STRING(NM_SETFOCUS)
			CASE2STRING(NM_KILLFOCUS)
			CASE2STRING(NM_OUTOFMEMORY)
			CASE2STRING(NM_HOVER)
			CASE2STRING(NM_NCHITTEST)       
			CASE2STRING(NM_KEYDOWN)         
			CASE2STRING(NM_RELEASEDCAPTURE) 
			CASE2STRING(NM_SETCURSOR)       
			CASE2STRING(NM_CHAR)            
			CASE2STRING(NM_CUSTOMDRAW)     

      CASE2STRING(TVN_SELCHANGINGA)   
      CASE2STRING(TVN_SELCHANGINGW)   
      CASE2STRING(TVN_SELCHANGEDA)    
      CASE2STRING(TVN_SELCHANGEDW)    
      CASE2STRING(TVN_GETDISPINFOA)   
      CASE2STRING(TVN_GETDISPINFOW)   
      CASE2STRING(TVN_SETDISPINFOA)   
      CASE2STRING(TVN_SETDISPINFOW)   
      CASE2STRING(TVN_ITEMEXPANDINGA) 
      CASE2STRING(TVN_ITEMEXPANDINGW) 
      CASE2STRING(TVN_ITEMEXPANDEDA)  
      CASE2STRING(TVN_ITEMEXPANDEDW)  
      CASE2STRING(TVN_BEGINDRAGA)     
      CASE2STRING(TVN_BEGINDRAGW)     
      CASE2STRING(TVN_BEGINRDRAGA)    
      CASE2STRING(TVN_BEGINRDRAGW)    
      CASE2STRING(TVN_DELETEITEMA)    
      CASE2STRING(TVN_DELETEITEMW)    
      CASE2STRING(TVN_BEGINLABELEDITA)
      CASE2STRING(TVN_BEGINLABELEDITW)
      CASE2STRING(TVN_ENDLABELEDITA)  
      CASE2STRING(TVN_ENDLABELEDITW)  
      CASE2STRING(TVN_KEYDOWN)        
      CASE2STRING(TVN_GETINFOTIPA)    
      CASE2STRING(TVN_GETINFOTIPW)    
      CASE2STRING(TVN_SINGLEEXPAND)   
		}

		return "";
	}

	CString MapDrawStage(UINT uStage)
	{
		switch (uStage)
		{
			CASE2STRING(CDDS_POSTERASE)
			CASE2STRING(CDDS_POSTPAINT)
			CASE2STRING(CDDS_PREERASE)
			CASE2STRING(CDDS_PREPAINT)
			CASE2STRING(CDDS_ITEM)
			CASE2STRING(CDDS_ITEMPOSTERASE)
			CASE2STRING(CDDS_ITEMPOSTPAINT)
			CASE2STRING(CDDS_ITEMPREERASE)
			CASE2STRING(CDDS_ITEMPREPAINT)
			CASE2STRING(CDDS_SUBITEM)
		}

		return "";
	}
};
// WM_ACTIVATE
class CISMH_Activate : public CISMsgHandler
{
public:
	CISMH_Activate(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: %s", MapStatus(WP(pMsg)));
		AddParam(pParams, "lp = hwnd being %s: 0x%08X", (WP(pMsg) == WA_INACTIVE) ? "activated" : "deactivated", LP(pMsg));

		return TRUE;
	}
	
	CString MapStatus(UINT uStatus)
	{
		switch (uStatus)
		{
			CASE2STRING(WA_ACTIVE)
			CASE2STRING(WA_CLICKACTIVE)
			CASE2STRING(WA_INACTIVE)
		}

		return "";
	}
	
};

// WM_KILL/SETFOCUS
class CISMH_Focus : public CISMsgHandler
{
public:
	CISMH_Focus(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hwnd: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_ENABLE
class CISMH_Enable : public CISMsgHandler
{
public:
	CISMH_Enable(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: %s", WP(pMsg) ? "enable" : "disable");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_SETREDRAW
class CISMH_Redraw : public CISMsgHandler
{
public:
	CISMH_Redraw(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: %s", WP(pMsg) ? "enable" : "disable");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_SYS/KEYXXX
class CISMH_Key : public CISMsgHandler
{
public:
	CISMH_Key(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = virt key: %04X", WP(pMsg));
		AddParam(pParams, "lp = flags: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_SYS/CHARXXX
class CISMH_Char : public CISMsgHandler
{
public:
	CISMH_Char(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = char code: %c", WP(pMsg));
		AddParam(pParams, "lp = flags: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_SETTEXT
class CISMH_SetText : public CISMsgHandler
{
public:
	CISMH_SetText(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = text: %s", (LPCTSTR)LP(pMsg));

		return TRUE;
	}
	
};

// WM_MDIXXX
class CISMH_Mdi : public CISMsgHandler
{
public:
	CISMH_Mdi(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hwnd: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_GETTEXT
class CISMH_GetText : public CISMsgHandler
{
public:
	CISMH_GetText(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = buffer size: %d", WP(pMsg));
		AddParam(pParams, "lp = buffer: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_HSCROLL
class CISMH_HScroll : public CISMsgHandler
{
public:
	CISMH_HScroll(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		switch (LOWORD(WP(pMsg)))
		{
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			AddParam(pParams, "wp = req: %s, pos: %d", MapRequest(LOWORD(WP(pMsg))), HIWORD(WP(pMsg)));
			break; 

		default:
			AddParam(pParams, "wp = req: %s", MapRequest(LOWORD(WP(pMsg))));
		}

		AddParam(pParams, "lp = scrollbar hwnd: 0x%08X", LP(pMsg));

		return TRUE;
	}
	

	CString MapRequest(UINT uReq)
	{
		switch (uReq)
		{
			CASE2STRING(SB_ENDSCROLL)
			CASE2STRING(SB_LEFT)
			CASE2STRING(SB_RIGHT)
			CASE2STRING(SB_LINELEFT)
			CASE2STRING(SB_LINERIGHT)
			CASE2STRING(SB_PAGELEFT)
			CASE2STRING(SB_PAGERIGHT)
			CASE2STRING(SB_THUMBPOSITION)
			CASE2STRING(SB_THUMBTRACK)
		}

		return "";
	}
};

// WM_HSCROLL
class CISMH_VScroll : public CISMsgHandler
{
public:
	CISMH_VScroll(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		switch (LOWORD(WP(pMsg)))
		{
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			AddParam(pParams, "wp = req: %s, pos: %d", MapRequest(LOWORD(WP(pMsg))), HIWORD(WP(pMsg)));
			break; 

		default:
			AddParam(pParams, "wp = req: %s", MapRequest(LOWORD(WP(pMsg))));
		}

		AddParam(pParams, "lp = scrollbar hwnd: 0x%08X", LP(pMsg));

		return TRUE;
	}

	CString MapRequest(UINT uReq)
	{
		switch (uReq)
		{
			CASE2STRING(SB_ENDSCROLL)
			CASE2STRING(SB_BOTTOM)
			CASE2STRING(SB_TOP)
			CASE2STRING(SB_LINEUP)
			CASE2STRING(SB_LINEDOWN)
			CASE2STRING(SB_PAGEUP)
			CASE2STRING(SB_PAGEDOWN)
			CASE2STRING(SB_THUMBPOSITION)
			CASE2STRING(SB_THUMBTRACK)
		}

		return "";
	}
	
};

// WM_COMMAND
class CISMH_Cmd : public CISMsgHandler
{
public:
	CISMH_Cmd(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		switch (HIWORD(WP(pMsg)))
		{
		case 1:
			AddParam(pParams, "wp = cmd id: %d, accelerator", LOWORD(WP(pMsg)));
			break;

		case 0:
			AddParam(pParams, "wp = cmd id: %d, menu", LOWORD(WP(pMsg)));
			break;

		default:
      {
        HWND hWnd = (HWND)LP(pMsg);
        CString sClass = CWinClasses::GetClass(hWnd);
        int nCtrlID = LOWORD(WP(pMsg));
        int nNotifyCode = HIWORD(WP(pMsg));

			  AddParam(pParams, "wp = control id: %04X (%d), notify code: %s", nCtrlID, nCtrlID, MapControlNotification(sClass, nNotifyCode));
        AddParam(pParams, "lp = control hwnd: 0x%08X, class: %s", hWnd, sClass);
      }
			break;
		}

		return TRUE;
	}

  CString MapControlNotification(const CString& sClass, UINT nNotifyCode)
  {
    BEGINNOTIFY(WC_BUTTON)
      CASE2STRING(BN_CLICKED)
    ENDNOTIFY

    BEGINNOTIFY(WC_STATIC)
    ENDNOTIFY

    BEGINNOTIFY(WC_EDIT)
      CASE2STRING(EN_SETFOCUS)
      CASE2STRING(EN_KILLFOCUS)   
      CASE2STRING(EN_CHANGE)      
      CASE2STRING(EN_UPDATE)      
      CASE2STRING(EN_ERRSPACE)    
      CASE2STRING(EN_MAXTEXT)     
      CASE2STRING(EN_HSCROLL)     
//       CASE2STRING(EN_ALIGN_LTR_EC)
//       CASE2STRING(EN_ALIGN_RTL_EC)
    ENDNOTIFY

    BEGINNOTIFY(WC_COMBOBOX)
    ENDNOTIFY

    BEGINNOTIFY(WC_COMBOLBOX)
    ENDNOTIFY

    BEGINNOTIFY(WC_LISTBOX)  
    ENDNOTIFY

    BEGINNOTIFY(WC_SCROLLBAR)
    ENDNOTIFY

    BEGINNOTIFY(WC_TOOLBAR)  
    ENDNOTIFY

    BEGINNOTIFY(WC_SPIN)     
    ENDNOTIFY

    BEGINNOTIFY(WC_PROGRESS) 
    ENDNOTIFY

    BEGINNOTIFY(WC_SLIDER)
    ENDNOTIFY

    BEGINNOTIFY(WC_HOTKEY)   
    ENDNOTIFY

    BEGINNOTIFY(WC_SHELLDLLDEFVIEW)  
    ENDNOTIFY

    BEGINNOTIFY(WC_STATUSBAR)       
    ENDNOTIFY

    BEGINNOTIFY(WC_ANIMATE)          
    ENDNOTIFY

    BEGINNOTIFY(WC_RICHEDIT)         
      CASE2STRING(EN_SETFOCUS)
      CASE2STRING(EN_KILLFOCUS)   
      CASE2STRING(EN_CHANGE)      
      CASE2STRING(EN_UPDATE)      
      CASE2STRING(EN_ERRSPACE)    
      CASE2STRING(EN_MAXTEXT)     
      CASE2STRING(EN_HSCROLL)     
//       CASE2STRING(EN_ALIGN_LTR_EC)
//       CASE2STRING(EN_ALIGN_RTL_EC)
    ENDNOTIFY

    BEGINNOTIFY(WC_RICHEDIT20)       
      CASE2STRING(EN_SETFOCUS)
      CASE2STRING(EN_KILLFOCUS)   
      CASE2STRING(EN_CHANGE)      
      CASE2STRING(EN_UPDATE)      
      CASE2STRING(EN_ERRSPACE)    
      CASE2STRING(EN_MAXTEXT)     
      CASE2STRING(EN_HSCROLL)     
//       CASE2STRING(EN_ALIGN_LTR_EC)
//       CASE2STRING(EN_ALIGN_RTL_EC)
    ENDNOTIFY

    BEGINNOTIFY(WC_RICHEDIT50)       
      CASE2STRING(EN_SETFOCUS)
      CASE2STRING(EN_KILLFOCUS)   
      CASE2STRING(EN_CHANGE)      
      CASE2STRING(EN_UPDATE)      
      CASE2STRING(EN_ERRSPACE)    
      CASE2STRING(EN_MAXTEXT)     
      CASE2STRING(EN_HSCROLL)     
//       CASE2STRING(EN_ALIGN_LTR_EC)
//       CASE2STRING(EN_ALIGN_RTL_EC)
    ENDNOTIFY

    BEGINNOTIFY(WC_DATETIMEPICK)     
    ENDNOTIFY

    BEGINNOTIFY(WC_MONTHCAL)         
    ENDNOTIFY

    BEGINNOTIFY(WC_REBAR)            
    ENDNOTIFY

    BEGINNOTIFY(WC_TOOLTIPS) 
    ENDNOTIFY

    BEGINNOTIFY(WC_THUMBNAILVIEW) 
    ENDNOTIFY

    BEGINNOTIFY(WC_IE) 
    ENDNOTIFY

    BEGINNOTIFY(WC_SHDOCOBJVW) 
    ENDNOTIFY

    BEGINNOTIFY(WC_SHELLEMBEDDING) 
    ENDNOTIFY
  }

};

// WM_SYSCOMMAND
class CISMH_SysCmd : public CISMsgHandler
{
public:
	CISMH_SysCmd(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = cmd type: %s", MapSysCmd(WP(pMsg)));
		AddParam(pParams, "lp = cursor screen pos: (%d, %d)", GET_X_LPARAM(LP(pMsg)), GET_Y_LPARAM(LP(pMsg)));

		return TRUE;
	}
	
	CString MapSysCmd(UINT uCmd)
	{
		switch (uCmd)
		{
			CASE2STRING(SC_CLOSE)
			CASE2STRING(SC_CONTEXTHELP)
			CASE2STRING(SC_DEFAULT)
			CASE2STRING(SC_HOTKEY)
			CASE2STRING(SC_HSCROLL)
			CASE2STRING(SC_KEYMENU)
			CASE2STRING(SC_MAXIMIZE)
			CASE2STRING(SC_MINIMIZE)
			CASE2STRING(SC_MONITORPOWER)
			CASE2STRING(SC_MOUSEMENU)
			CASE2STRING(SC_MOVE)
			CASE2STRING(SC_NEXTWINDOW)
			CASE2STRING(SC_PREVWINDOW)
			CASE2STRING(SC_RESTORE)
			CASE2STRING(SC_SCREENSAVE)
			CASE2STRING(SC_SIZE)
			CASE2STRING(SC_TASKLIST)
			CASE2STRING(SC_VSCROLL)
		}

		return "";
	}
};

// WM_GETDLGCODE
class CISMH_GetDlgCode : public CISMsgHandler
{
public:
	CISMH_GetDlgCode(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");

		CString sMsgParam;

		if (LP(pMsg) && CISMsgManager::HandleMsg((MSG*)LP(pMsg), sMsgParam))
			AddParam(pParams, "lp = msg: %s", sMsgParam);
		else
			AddParam(pParams, "lp = msg: 0");

		return TRUE;
	}
	
};

// WM_MOVE
class CISMH_Move : public CISMsgHandler
{
public:
	CISMH_Move(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = parent client pos: (%d, %d)", GET_X_LPARAM(LP(pMsg)), GET_Y_LPARAM(LP(pMsg)));

		return TRUE;
	}
	
};

// WM_SIZE
class CISMH_Size : public CISMsgHandler
{
public:
	CISMH_Size(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: %04X", LOWORD(WP(pMsg)));
		AddParam(pParams, "lp = client size: %d x %d", GET_X_LPARAM(LP(pMsg)), GET_Y_LPARAM(LP(pMsg)));

		return TRUE;
	}
	
};

// WM_MOUSEWHEEL
class CISMH_MouseWheel : public CISMsgHandler
{
public:
	CISMH_MouseWheel(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flags: %04X, delta: %d", LOWORD(WP(pMsg)), HIWORD(WP(pMsg)));
		AddParam(pParams, "lp = cursor client pos: (%d, %d)", GET_X_LPARAM(LP(pMsg)), GET_Y_LPARAM(LP(pMsg)));

		return TRUE;
	}
	
};

// WM_NCHITTEST
class CISMH_NcHitTest : public CISMsgHandler
{
public:
	CISMH_NcHitTest(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = cursor screen pos: (%d, %d)", GET_X_LPARAM(LP(pMsg)), GET_Y_LPARAM(LP(pMsg)));

		return TRUE;
	}
};

// WM_NCL/R/MBUTTONDOWN/UP/DBLCLK
class CISMH_NcMouse : public CISMsgHandler
{
public:
	CISMH_NcMouse(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hit test: %s", MapHitTest(WP(pMsg)));
		AddParam(pParams, "lp = cursor screen pos: (%d, %d)", GET_X_LPARAM(LP(pMsg)), GET_Y_LPARAM(LP(pMsg)));

		return TRUE;
	}
};

// WM_CTLCOLORXXX
class CISMH_CtlColor : public CISMsgHandler
{
public:
	CISMH_CtlColor(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = dc: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = hwnd: 0x%08X", LP(pMsg));

		return TRUE;
	}
};

// WM_ERASEBKGND
class CISMH_EraseBkgnd : public CISMsgHandler
{
public:
	CISMH_EraseBkgnd(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)

	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hdc: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_NCPAINT
class CISMH_NcPaint : public CISMsgHandler
{
public:
	CISMH_NcPaint(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		if (WP(pMsg) == 1)
			AddParam(pParams, "wp = update region: 1 (entire window)");
		else
			AddParam(pParams, "wp = update region: 0x%08X", WP(pMsg));

		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_TIMER
class CISMH_Timer : public CISMsgHandler
{
public:
	CISMH_Timer(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = id: 0x%04X (%d)", WP(pMsg));
		AddParam(pParams, "lp = callback address: 0x%08X", LP(pMsg));

		return TRUE;
	}
};

// WM_SETCURSOR
class CISMH_SetCursor : public CISMsgHandler 
{
public:
	CISMH_SetCursor(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hwnd: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = hittest: %s, mouse msg: 0x%04X", MapHitTest(LOWORD(LP(pMsg))), HIWORD(LOWORD(LP(pMsg))));

		return TRUE;
	}
	
};

// WM_GETMINMAXINFO
class CISMH_MinMax : public CISMsgHandler
{
public:
	CISMH_MinMax(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		MINMAXINFO* pMMI = (MINMAXINFO*)LP(pMsg);
		ASSERT (pMMI);

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = MINMAXINFO");
		AddParam(pParams, "   ptMaxSize = (%d, %d)", pMMI->ptMaxSize.x, pMMI->ptMaxSize.y); 
		AddParam(pParams, "   ptMaxPosition = (%d, %d)", pMMI->ptMaxPosition.x, pMMI->ptMaxPosition.y); 
		AddParam(pParams, "   ptMinTrackSize = (%d, %d)", pMMI->ptMinTrackSize.x, pMMI->ptMinTrackSize.y); 
		AddParam(pParams, "   ptMaxTrackSize = (%d, %d)", pMMI->ptMaxTrackSize.x, pMMI->ptMaxTrackSize.y); 

		return TRUE;
	}
	
};

// WM_QUERYENDSESSION
class CISMH_QueryEnd : public CISMsgHandler
{
public:
	CISMH_QueryEnd(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = logoff flag: %s", (LP(pMsg) & ENDSESSION_LOGOFF) ? "logging off" : "shutting down"); 

		return TRUE;
	}
	
};

// WM_QUIT
class CISMH_Quit : public CISMsgHandler
{
public:
	CISMH_Quit(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = exit code: %d", WP(pMsg));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_ENDSESSION
class CISMH_EndSession : public CISMsgHandler
{
public:
	CISMH_EndSession(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = end-session flag: %s", WP(pMsg) ? "ending" : "not ending");
		AddParam(pParams, "lp = logoff flag: %s", (LP(pMsg) & ENDSESSION_LOGOFF) ? "logging off" : "shutting down"); 

		return TRUE;
	}
	
};

// WM_WININICHANGE
class CISMH_WinIniChange : public CISMsgHandler
{
public:
	CISMH_WinIniChange(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = system parameter area: %s", LP(pMsg) ? (LPCTSTR)LP(pMsg) : "<unknown>");

		return TRUE;
	}
	
};

// WM_SETTINGCHANGE
class CISMH_SettingChange : public CISMsgHandler
{
public:
	CISMH_SettingChange(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = system param area: %s", LP(pMsg) ? (LPCTSTR)LP(pMsg) : "<unknown>");

		return TRUE;
	}
	
};

// WM_DEVMODECHANGE
class CISMH_DevModeChange : public CISMsgHandler
{
public:
	CISMH_DevModeChange(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = device: ", LP(pMsg) ? (LPCTSTR)LP(pMsg) : "");

		return TRUE;
	}
	
};

// WM_ACTIVATEAPP
class CISMH_ActivateApp : public CISMsgHandler
{
public:
	CISMH_ActivateApp(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: %s", WP(pMsg) ? "activating" : "deactivating");
		AddParam(pParams, "lp = thread id: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_NEXTDLGCTL
class CISMH_NextDlgCtrl : public CISMsgHandler
{
public:
	CISMH_NextDlgCtrl(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

//		AddParam(pParams, "wp = <not used>");
//		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_SPOOLERSTATUS
class CISMH_Spooler : public CISMsgHandler
{
public:
	CISMH_Spooler(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: 0x%04X", WP(pMsg));
		AddParam(pParams, "lp = jobs left: %d", LP(pMsg));

		return TRUE;
	}
	
};

// WM_DRAWITEM
class CISMH_DrawItem : public CISMsgHandler
{
public:
	CISMH_DrawItem(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPDRAWITEMSTRUCT lpDI = (LPDRAWITEMSTRUCT)LP(pMsg);

		AddParam(pParams, "wp = control id: %d (0x%04X)", WP(pMsg), WP(pMsg));
		AddParam(pParams, "lp = LPDRAWITEMSTRUCT");
		AddParam(pParams, "   type: %s", MapType(lpDI->CtlType)); 

		if (lpDI->CtlType == ODT_MENU)
		{
			AddParam(pParams, "   hmenu: 0x%08X", lpDI->hwndItem); 
			AddParam(pParams, "   item id: 0x%04X", lpDI->itemID); 
		}
		else
		{
			AddParam(pParams, "   control id: %d (0x%04X)", lpDI->CtlID, lpDI->CtlID); 
			AddParam(pParams, "   hwnd: 0x%08X", lpDI->hwndItem); 
			AddParam(pParams, "   item index: %d", lpDI->itemID); 
		}

		AddParam(pParams, "   item action: 0x%08X", lpDI->itemAction); 
		AddParam(pParams, "   item state: 0x%08X", lpDI->itemState); 
		AddParam(pParams, "   hdc: 0x%08X", lpDI->hDC); 
		AddParam(pParams, "   rect: (%d, %d, %d, %d)", lpDI->rcItem.left, lpDI->rcItem.top, lpDI->rcItem.right, lpDI->rcItem.bottom);
		AddParam(pParams, "   item data: 0x%08X", lpDI->itemData); 

		return TRUE;
	}
	
	CString MapType(UINT uType)
	{
		switch (uType)
		{
			CASE2STRING(ODT_BUTTON)
			CASE2STRING(ODT_COMBOBOX)
			CASE2STRING(ODT_LISTBOX)
			CASE2STRING(ODT_LISTVIEW)
			CASE2STRING(ODT_MENU)
			CASE2STRING(ODT_STATIC)
			CASE2STRING(ODT_TAB)
		}

		return "<unknown>";
	}
};

// WM_MEASUREITEM
class CISMH_MeasureItem : public CISMH_DrawItem
{
public:
	CISMH_MeasureItem(UINT uMsg, LPCTSTR szMsg) : CISMH_DrawItem(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPMEASUREITEMSTRUCT lpMI = (LPMEASUREITEMSTRUCT)LP(pMsg);

		AddParam(pParams, "wp = control id: %d (0x%04X)", WP(pMsg), WP(pMsg));
		AddParam(pParams, "lp = LPDRAWITEMSTRUCT");
		AddParam(pParams, "   type: %s", MapType(lpMI->CtlType)); 

		if (lpMI->CtlType == ODT_MENU)
			AddParam(pParams, "   item id: 0x%04X", lpMI->itemID); 
		else
		{
			AddParam(pParams, "   control id: %d (0x%04X)", lpMI->CtlID, lpMI->CtlID); 
			AddParam(pParams, "   item index: %d", lpMI->itemID); 
		}

		AddParam(pParams, "   item width: %d", lpMI->itemWidth); 
		AddParam(pParams, "   item height: %d", lpMI->itemHeight); 
		AddParam(pParams, "   item data: 0x%08X", lpMI->itemData); 

		return TRUE;
	}
	
};

// WM_DELETEITEM
class CISMH_DeleteItem : public CISMsgHandler
{
public:
	CISMH_DeleteItem(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPDELETEITEMSTRUCT lpDI = (LPDELETEITEMSTRUCT)LP(pMsg);

		AddParam(pParams, "wp = control id: %d (0x%04X)", WP(pMsg), WP(pMsg));
		AddParam(pParams, "lp = LPDELETEITEMSTRUCT");
		AddParam(pParams, "   type: %s", (lpDI->CtlType == ODT_LISTBOX) ? "ODT_LISTBOX" : "ODT_COMBOBOX"); 
		AddParam(pParams, "   control id: %d (0x%04X)", lpDI->CtlID, lpDI->CtlID); 
		AddParam(pParams, "   hwnd: 0x%08X", lpDI->hwndItem); 
		AddParam(pParams, "   item index: %d", lpDI->itemID); 
		AddParam(pParams, "   item data: 0x%08X", lpDI->itemData); 

		return TRUE;
	}
	
};

// WM_VKEYTOITEM
class CISMH_VkeyItem : public CISMsgHandler
{
public:
	CISMH_VkeyItem(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = virtual key: 0x%04X, caret pos: %d", LOWORD(WP(pMsg)), HIWORD(WP(pMsg)));
		AddParam(pParams, "lp = hwnd: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_CHARTOITEM
class CISMH_CharItem : public CISMsgHandler
{
public:
	CISMH_CharItem(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = char key: '%c', caret pos: %d", LOWORD(WP(pMsg)), HIWORD(WP(pMsg)));
		AddParam(pParams, "lp = hwnd: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_SETFONT
class CISMH_SetFont : public CISMsgHandler
{
public:
	CISMH_SetFont(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hfont: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = flag: %s", LP(pMsg) ? "redraw" : "do not redraw");

		return TRUE;
	}
	
};

// WM_SETHOTKEY
class CISMH_SetHotkey : public CISMsgHandler
{
public:
	CISMH_SetHotkey(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = virtual key: 0x%04X, modifiers: %s", LOWORD(WP(pMsg)), MapModifiers(HIWORD(WP(pMsg))));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
	CString MapModifiers(UINT uMods)
	{
		CString sModifiers;

		ADDOPTION(HOTKEYF_ALT, uMods, sModifiers);
		ADDOPTION(HOTKEYF_CONTROL, uMods, sModifiers);
		ADDOPTION(HOTKEYF_EXT, uMods, sModifiers);
		ADDOPTION(HOTKEYF_SHIFT, uMods, sModifiers);
		
		return sModifiers;
	}
	
};

// WM_COMPACTING
class CISMH_Compacting : public CISMsgHandler
{
public:
	CISMH_Compacting(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = ratio: %d%%", (WP(pMsg) * 100) / 0xffff);
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_WINDOWPOSCHANGED/CHANGING
class CISMH_WPosChange : public CISMsgHandler
{
public:
	CISMH_WPosChange(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPWINDOWPOS lpwp = (LPWINDOWPOS)LP(pMsg);

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = LPWINDOWPOS");
		AddParam(pParams, "   hwnd: 0x%08X", lpwp->hwnd); 
		AddParam(pParams, "   insert after: 0x%08X", lpwp->hwndInsertAfter); 
		AddParam(pParams, "   x: %d", lpwp->x); 
		AddParam(pParams, "   y: %d", lpwp->y); 
		AddParam(pParams, "   cx: %d", lpwp->cx); 
		AddParam(pParams, "   cy: %d", lpwp->cy); 
		AddParam(pParams, "   flags: %s", MapFlags(lpwp->flags)); 

		return TRUE;
	}
	
	CString MapFlags(UINT uFlags)
	{
		CString sFlags;

		ADDOPTION(SWP_DRAWFRAME, uFlags, sFlags);
		ADDOPTION(SWP_FRAMECHANGED, uFlags, sFlags);
		ADDOPTION(SWP_HIDEWINDOW, uFlags, sFlags);
		ADDOPTION(SWP_NOACTIVATE, uFlags, sFlags);
		ADDOPTION(SWP_NOCOPYBITS, uFlags, sFlags);
		ADDOPTION(SWP_NOMOVE, uFlags, sFlags);
		ADDOPTION(SWP_NOOWNERZORDER, uFlags, sFlags);
		ADDOPTION(SWP_NOREDRAW, uFlags, sFlags);
		ADDOPTION(SWP_NOREPOSITION, uFlags, sFlags);
		ADDOPTION(SWP_NOSENDCHANGING, uFlags, sFlags);
		ADDOPTION(SWP_NOSIZE, uFlags, sFlags);
		ADDOPTION(SWP_NOZORDER, uFlags, sFlags);
		ADDOPTION(SWP_SHOWWINDOW, uFlags, sFlags);

		return sFlags;
	}
};

// WM_POWER
class CISMH_Power : public CISMsgHandler
{
public:
	CISMH_Power(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = event: %s", MapEvent(WP(pMsg)));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}

	CString MapEvent(UINT uEvent)
	{
		switch (uEvent)
		{
			CASE2STRING(PWR_CRITICALRESUME)
			CASE2STRING(PWR_SUSPENDREQUEST)
			CASE2STRING(PWR_SUSPENDRESUME)
		}

		return "<unknown>";
	}

};

// WM_COPYDATA
class CISMH_CopyData : public CISMsgHandler
{
public:
	CISMH_CopyData(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		PCOPYDATASTRUCT pData = (PCOPYDATASTRUCT)LP(pMsg);

		AddParam(pParams, "wp = hwnd: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = PCOPYDATASTRUCT");
		AddParam(pParams, "   data id: %d (0x%08X)", pData->dwData, pData->dwData);
		AddParam(pParams, "   data size: %d bytes", pData->cbData);
		AddParam(pParams, "   data: 0x%08X", pData->lpData);

		return TRUE;
	}
	
};

// WM_INPUTLANGCHANGEREQUEST
class CISMH_InputLangReq : public CISMsgHandler
{
public:
	CISMH_InputLangReq(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = locale id: 0x%08X", LP(pMsg));

		return TRUE;
	}

};

// WM_INPUTLANGCHANGE
class CISMH_InputLang : public CISMsgHandler
{
public:
	CISMH_InputLang(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = char set: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = locale id: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_TCARD
class CISMH_Tcard : public CISMsgHandler
{
public:
	CISMH_Tcard(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = action: %s", MapAction(WP(pMsg)));
		AddParam(pParams, "lp = action data: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
	CString MapAction(UINT uAction)
	{
		switch (uAction)
		{
			CASE2STRING(IDABORT)
			CASE2STRING(IDCANCEL)
			CASE2STRING(IDCLOSE)
			CASE2STRING(IDHELP)
			CASE2STRING(IDIGNORE)
			CASE2STRING(IDOK)
			CASE2STRING(IDNO)
			CASE2STRING(IDRETRY)
			CASE2STRING(HELP_TCARD_DATA)
//			CASE2STRING(HELP_TCARD_NEXT)
			CASE2STRING(HELP_TCARD_OTHER_CALLER)
			CASE2STRING(IDYES)
		}

		return "<unknown>";
	}
};

// WM_HELP
class CISMH_Help : public CISMsgHandler
{
public:
	CISMH_Help(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPHELPINFO lpHI = (LPHELPINFO)LP(pMsg);

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = LPHELPINFO");
		AddParam(pParams, "   context type: %s", lpHI->iContextType == HELPINFO_MENUITEM ? "HELPINFO_MENUITEM" : "HELPINFO_WINDOW");

		if (lpHI->iContextType == HELPINFO_MENUITEM)
		{
			AddParam(pParams, "   menu item: %d (0x%08X)", lpHI->iCtrlId);
			AddParam(pParams, "   hmenu: 0x%08X", lpHI->hItemHandle);
		}
		else
		{
			AddParam(pParams, "   control id: %d (0x%08X)", lpHI->iCtrlId);
			AddParam(pParams, "   hwnd: 0x%08X", lpHI->hItemHandle);
		}

		AddParam(pParams, "   help context: 0x%08X", lpHI->dwContextId);
		AddParam(pParams, "   screen cursor pos: (%d, %d)", lpHI->MousePos.x, lpHI->MousePos.y);

		return TRUE;
	}
	
};

// WM_NOTIFYFORMAT
class CISMH_NotifyFormat : public CISMsgHandler
{
public:
	CISMH_NotifyFormat(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = %s hwnd: 0x%08X", LP(pMsg) == NF_QUERY ? "control" : "parent", WP(pMsg));
		AddParam(pParams, "lp = cmd: %s", LP(pMsg) == NF_QUERY ? "NF_QUERY" : "NF_REQUERY");

		return TRUE;
	}
	
};

// WM_CONTEXTMENU
class CISMH_ContextMenu : public CISMsgHandler
{
public:
	CISMH_ContextMenu(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hwnd: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = screen cursor pos: (%d, %d)", GET_X_LPARAM(LP(pMsg)), GET_X_LPARAM(LP(pMsg)));

		return TRUE;
	}
	
};

// WM_STYLECHANGED/CHANGING
class CISMH_StyleChange : public CISMsgHandler
{
public:
	CISMH_StyleChange(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPSTYLESTRUCT lpSS = (LPSTYLESTRUCT)LP(pMsg);

		AddParam(pParams, "wp = type: %s", WP(pMsg) == GWL_STYLE ? "GWL_STYLE" : "GWL_EXSTYLE");
		AddParam(pParams, "lp = old styles: 0x%08X, new styles: 0x%08X", lpSS->styleOld, lpSS->styleNew);

		return TRUE;
	}
	
};

// WM_DISPLAYCHANGE
class CISMH_DispChange : public CISMsgHandler
{
public:
	CISMH_DispChange(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = screen depth: %d bpp", WP(pMsg));
		AddParam(pParams, "lp = screen res: %d x %d", LOWORD(LP(pMsg)), HIWORD(LP(pMsg)));

		return TRUE;
	}
	
};

// WM_GETICON
class CISMH_GetIcon : public CISMsgHandler
{
public:
	CISMH_GetIcon(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = type: %s", WP(pMsg) == ICON_BIG ? "ICON_BIG" : "ICON_SMALL");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_SETICON
class CISMH_SetIcon : public CISMsgHandler
{
public:
	CISMH_SetIcon(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = type: %s", WP(pMsg) == ICON_BIG ? "ICON_BIG" : "ICON_SMALL");
		AddParam(pParams, "lp = hicon: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_NCCALCSIZE
class CISMH_NcCalcSize : public CISMH_WPosChange
{
public:
	CISMH_NcCalcSize(UINT uMsg, LPCTSTR szMsg) : CISMH_WPosChange(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = validation flag: %d", WP(pMsg) ? 1 : 0);

		if (WP(pMsg))
		{
			LPNCCALCSIZE_PARAMS pNCCS = (LPNCCALCSIZE_PARAMS)LP(pMsg);
			AddParam(pParams, "lp = LPNCCALCSIZE_PARAMS");
			AddParam(pParams, "   rect[0]: (%d, %d, %d, %d)", pNCCS->rgrc[0].left, pNCCS->rgrc[0].top, pNCCS->rgrc[0].right, pNCCS->rgrc[0].bottom);
			AddParam(pParams, "   rect[1]: (%d, %d, %d, %d)", pNCCS->rgrc[1].left, pNCCS->rgrc[1].top, pNCCS->rgrc[1].right, pNCCS->rgrc[1].bottom);
			AddParam(pParams, "   rect[2]: (%d, %d, %d, %d)", pNCCS->rgrc[2].left, pNCCS->rgrc[2].top, pNCCS->rgrc[2].right, pNCCS->rgrc[2].bottom);
			AddParam(pParams, "   LPWINDOWPOS");
			AddParam(pParams, "      hwnd: 0x%08X", pNCCS->lppos->hwnd); 
			AddParam(pParams, "      insert after: 0x%08X", pNCCS->lppos->hwndInsertAfter); 
			AddParam(pParams, "      x: %d", pNCCS->lppos->x); 
			AddParam(pParams, "      y: %d", pNCCS->lppos->y); 
			AddParam(pParams, "      cx: %d", pNCCS->lppos->cx); 
			AddParam(pParams, "      cy: %d", pNCCS->lppos->cy); 
			AddParam(pParams, "      flags: %s", MapFlags(pNCCS->lppos->flags)); 
		}
		else
		{
			LPRECT pRect = (LPRECT)LP(pMsg);
			AddParam(pParams, "lp = rect: (%d, %d, %d, %d)", pRect->left, pRect->top, pRect->right, pRect->bottom);
		}

		return TRUE;
	}
	
};

// WM_NCACTIVATE
class CISMH_NcActivate : public CISMsgHandler
{
public:
	CISMH_NcActivate(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = new state: %s", WP(pMsg) ? "active" : "inactive");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_COMPAREITEM
class CISMH_CompareItem : public CISMsgHandler
{
public:
	CISMH_CompareItem(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPCOMPAREITEMSTRUCT lpCI = (LPCOMPAREITEMSTRUCT)LP(pMsg);

		AddParam(pParams, "wp = control id: %d (0x%04X)", WP(pMsg), WP(pMsg));
		AddParam(pParams, "lp = LPDELETEITEMSTRUCT");
		AddParam(pParams, "   type: %s", (lpCI->CtlType == ODT_LISTBOX) ? "ODT_LISTBOX" : "ODT_COMBOBOX"); 
		AddParam(pParams, "   control id: %d (0x%04X)", lpCI->CtlID, lpCI->CtlID); 
		AddParam(pParams, "   hwnd: 0x%08X", lpCI->hwndItem); 
		AddParam(pParams, "   item1 index: %d", lpCI->itemID1); 
		AddParam(pParams, "   item1 data: 0x%08X", lpCI->itemData1); 
		AddParam(pParams, "   item2 index: %d", lpCI->itemID2); 
		AddParam(pParams, "   item2 data: 0x%08X", lpCI->itemData2); 
		AddParam(pParams, "   locale id: 0x%08X", lpCI->dwLocaleId); 

		return TRUE;
	}
	
};

// WM_INITDIALOG
class CISMH_InitDlg : public CISMsgHandler
{
public:
	CISMH_InitDlg(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = default control hwnd: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = user data: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_INITMENU
class CISMH_InitMenu : public CISMsgHandler
{
public:
	CISMH_InitMenu(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hmenu: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_INITMENUPOPUP
class CISMH_InitMenuPopup : public CISMsgHandler
{
public:
	CISMH_InitMenuPopup(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hsubmenu: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = item pos: %d, %s", LOWORD(LP(pMsg)), HIWORD(LP(pMsg)) ? "window menu" : "not window menu");

		return TRUE;
	}
	
};

// WM_MENUSELECT
class CISMH_MenuSel : public CISMsgHandler
{
public:
	CISMH_MenuSel(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		UINT uFlags = HIWORD(WP(pMsg));

		if (uFlags & MF_POPUP)
			AddParam(pParams, "wp = popup index: %d, flags: %s", LOWORD(WP(pMsg)), MapFlags(uFlags));
		else
			AddParam(pParams, "wp = item id: %d (0x%08X), flags: %s", LOWORD(WP(pMsg)), MapFlags(uFlags));

		AddParam(pParams, "lp = hmenu: 0x%08X", LP(pMsg));

		return TRUE;
	}

	CString MapFlags(UINT uFlags)
	{
		CString sFlags;

		ADDOPTION(MF_BITMAP, uFlags, sFlags);
		ADDOPTION(MF_CHECKED, uFlags, sFlags);
		ADDOPTION(MF_DISABLED, uFlags, sFlags);
		ADDOPTION(MF_GRAYED, uFlags, sFlags);
		ADDOPTION(MF_HILITE, uFlags, sFlags);
		ADDOPTION(MF_MOUSESELECT, uFlags, sFlags);
		ADDOPTION(MF_OWNERDRAW, uFlags, sFlags);
		ADDOPTION(MF_POPUP, uFlags, sFlags);
		ADDOPTION(MF_SYSMENU, uFlags, sFlags);

		return sFlags;
	}
	
};

// WM_MENUCHAR
class CISMH_MenuChar : public CISMsgHandler
{
public:
	CISMH_MenuChar(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = char key: '%c', type: %s", LOWORD(WP(pMsg)), HIWORD(WP(pMsg)) == MF_SYSMENU ? "MF_SYSMENU" : "MF_POPUP");
		AddParam(pParams, "lp = hmenu: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_ENTERIDLE
class CISMH_EnterIdle : public CISMsgHandler
{
public:
	CISMH_EnterIdle(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		if (WP(pMsg) == MSGF_MENU)
		{
			AddParam(pParams, "wp = reason: MSGF_MENU");
			AddParam(pParams, "lp = owner hwnd: 0x%08X", LP(pMsg));
		}
		else
		{
			AddParam(pParams, "wp = reason: MSGF_DIALOG");
			AddParam(pParams, "lp = hwnd: 0x%08X", LP(pMsg));
		}

		return TRUE;
	}
	
};

// WM_PARENTNOTIFY
#ifndef WM_XBUTTONDOWN
	#define WM_XBUTTONDOWN  0x020B
#endif

class CISMH_ParentNotify : public CISMsgHandler
{
public:
	CISMH_ParentNotify(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		UINT uMsg = LOWORD(WP(pMsg));

		if (uMsg == WM_CREATE || uMsg == WM_DESTROY)
		{
			AddParam(pParams, "wp = msg: %s, control id: %d (0x%04X)", MapMsg(uMsg), HIWORD(WP(pMsg)), HIWORD(WP(pMsg)));
			AddParam(pParams, "lp = hwnd: 0x%08X", LP(pMsg));
		}
		else
		{
			AddParam(pParams, "wp = msg: %s", MapMsg(uMsg));
			AddParam(pParams, "lp = cursor pos: (%d, %d)", GET_X_LPARAM(LP(pMsg)), GET_Y_LPARAM(LP(pMsg)));
		}

		return TRUE;
	}
	
	CString MapMsg(UINT uMsg)
	{
		switch (uMsg)
		{
			CASE2STRING(WM_CREATE)
			CASE2STRING(WM_DESTROY)
			CASE2STRING(WM_LBUTTONDOWN)
			CASE2STRING(WM_MBUTTONDOWN)
			CASE2STRING(WM_RBUTTONDOWN)
			CASE2STRING(WM_XBUTTONDOWN)
		}

		return "<unknown>";
	}
};

// WM_ENTERMENULOOP
class CISMH_EnterMenu : public CISMsgHandler
{
public:
	CISMH_EnterMenu(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: %s", WP(pMsg) ? "TrackPopupMenu()" : "not TrackPopupMenu()");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_EXITMENULOOP
class CISMH_ExitMenu : public CISMsgHandler
{
public:
	CISMH_ExitMenu(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = status: %s", WP(pMsg) ? "shortcut menu" : "not shortcut menu");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_NEXTMENU
class CISMH_NextMenu : public CISMsgHandler
{
public:
	CISMH_NextMenu(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPMDINEXTMENU lpMNM = (LPMDINEXTMENU)LP(pMsg);

		AddParam(pParams, "wp = virtual key: 0x%04X", WP(pMsg));
		AddParam(pParams, "lp = LPMDINEXTMENU");
		AddParam(pParams, "   current hmenu: 0x%08X", lpMNM->hmenuIn);
		AddParam(pParams, "   next hmenu: 0x%08X", lpMNM->hmenuNext);
		AddParam(pParams, "   owner hwnd: 0x%08X", lpMNM->hwndNext);

		return TRUE;
	}
	
};

// WM_SIZING/MOVING
class CISMH_SizingMoving : public CISMsgHandler
{
public:
	CISMH_SizingMoving(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = edge: %s", MapEdge(WP(pMsg)));

		LPRECT pRect = (LPRECT)LP(pMsg);
		AddParam(pParams, "lp = drag rect: (%d, %d, %d, %d)", pRect->left, pRect->top, pRect->right, pRect->bottom);

		return TRUE;
	}
	
	CString MapEdge(UINT uEdge)
	{
		switch (uEdge)
		{
			CASE2STRING(WMSZ_BOTTOM)
			CASE2STRING(WMSZ_BOTTOMLEFT)
			CASE2STRING(WMSZ_BOTTOMRIGHT)
			CASE2STRING(WMSZ_LEFT)
			CASE2STRING(WMSZ_RIGHT)
			CASE2STRING(WMSZ_TOP)
			CASE2STRING(WMSZ_TOPLEFT)
			CASE2STRING(WMSZ_TOPRIGHT)
		}

		return "<unknown>";
	}
};

// WM_CAPTURECHANGED
class CISMH_Capture : public CISMsgHandler
{
public:
	CISMH_Capture(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = other hwnd: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_POWERBROADCAST
class CISMH_PowerBroadcast : public CISMsgHandler
{
public:
	CISMH_PowerBroadcast(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = event: %s", MapEvent(WP(pMsg)));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
	CString MapEvent(UINT uEvent)
	{
		switch (uEvent)
		{
			CASE2STRING(PBT_APMBATTERYLOW)
			CASE2STRING(PBT_APMOEMEVENT)
			CASE2STRING(PBT_APMPOWERSTATUSCHANGE)
			CASE2STRING(PBT_APMQUERYSUSPEND)
			CASE2STRING(PBT_APMQUERYSUSPENDFAILED)
			CASE2STRING(PBT_APMRESUMEAUTOMATIC)
			CASE2STRING(PBT_APMRESUMECRITICAL)
			CASE2STRING(PBT_APMRESUMESUSPEND)
			CASE2STRING(PBT_APMSUSPEND) 
		}

		return "<unknown>";
	}
};

// WM_DEVICECHANGE
class CISMH_DevChange : public CISMsgHandler
{
public:
	CISMH_DevChange(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = event: %s", MapEvent(WP(pMsg)));
		AddParam(pParams, "lp = event data: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
	CString MapEvent(UINT uEvent)
	{
		switch (uEvent)
		{
			CASE2STRING(DBT_CONFIGCHANGECANCELED)
			CASE2STRING(DBT_CONFIGCHANGED)
//			CASE2STRING(DBT_CUSTOMEVENT)
			CASE2STRING(DBT_DEVICEARRIVAL)
			CASE2STRING(DBT_DEVICEQUERYREMOVE)
			CASE2STRING(DBT_DEVICEQUERYREMOVEFAILED)
			CASE2STRING(DBT_DEVICEREMOVECOMPLETE)
			CASE2STRING(DBT_DEVICEREMOVEPENDING)
			CASE2STRING(DBT_DEVICETYPESPECIFIC)
			CASE2STRING(DBT_QUERYCHANGECONFIG)
			CASE2STRING(DBT_USERDEFINED)  
		}

		return "<unknown>";
	}
	
};

// WM_MDICREATE
class CISMH_MdiCreate : public CISMsgHandler
{
public:
	CISMH_MdiCreate(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		LPMDICREATESTRUCT lpCS = (LPMDICREATESTRUCT)LP(pMsg);

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = LPMDICREATESTRUCT");
		AddParam(pParams, "   class: %s", lpCS->szClass);
		AddParam(pParams, "   caption: %s", lpCS->szTitle);
		AddParam(pParams, "   hwndOwner: 0x%08X", lpCS->hOwner);
		AddParam(pParams, "   x: %d", lpCS->x);
		AddParam(pParams, "   y: %d", lpCS->y);
		AddParam(pParams, "   cx: %d", lpCS->cx);
		AddParam(pParams, "   cy: %d", lpCS->cy);
		AddParam(pParams, "   style: 0x%08X", lpCS->style);
		AddParam(pParams, "   lparam: 0x%08X", lpCS->lParam);

		return TRUE;
	}
	
};

// WM_MDINEXT
class CISMH_MdiNext : public CISMsgHandler
{
public:
	CISMH_MdiNext(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = current hwnd: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = flag: %s", LP(pMsg) ? "next" : "previous");

		return TRUE;
	}
	
};

// WM_MDITILE
class CISMH_MdiTile : public CISMsgHandler
{
public:
	CISMH_MdiTile(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = flag: %s", (WP(pMsg) == MDITILE_VERTICAL) ? "vertical" : "horizontal");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_MDICASCADE
class CISMH_MdiCascade : public CISMsgHandler
{
public:
	CISMH_MdiCascade(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = arrange flag: %s", (WP(pMsg) == MDITILE_SKIPDISABLED) ? "skip disabled" : "zorder");
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_MDIGETACTIVE
class CISMH_MdiGetActive : public CISMsgHandler
{
public:
	CISMH_MdiGetActive(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = <not used>");
		AddParam(pParams, "lp = LPBOOL: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_MDISETMENU
class CISMH_MdiSetMenu : public CISMsgHandler
{
public:
	CISMH_MdiSetMenu(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = frame hmenu: 0x%08X", WP(pMsg));
		AddParam(pParams, "wp = window hmenu: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_DROPFILES
class CISMH_Drop : public CISMsgHandler
{
public:
	CISMH_Drop(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = hdrop: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_PALETTECHANGED
class CISMH_PalChange : public CISMsgHandler
{
public:
	CISMH_PalChange(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = causer hwnd: 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_HOTKEY
class CISMH_Hotkey : public CISMsgHandler
{
public:
	CISMH_Hotkey(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = type: %s", MapType(WP(pMsg)));
		AddParam(pParams, "lp = key modifiers: %s", MapMods(LP(pMsg)));

		return TRUE;
	}
	
	CString MapType(UINT uType)
	{
		switch(uType)
		{
			CASE2STRING(IDHOT_SNAPDESKTOP)
			CASE2STRING(IDHOT_SNAPWINDOW)
		}

		return "<unknown>";
	}

	CString MapMods(UINT uMods)
	{
		CString sMods;

		ADDOPTION(MOD_ALT, uMods, sMods);
		ADDOPTION(MOD_CONTROL, uMods, sMods);
		ADDOPTION(MOD_SHIFT, uMods, sMods);
		ADDOPTION(MOD_WIN, uMods, sMods);

		return sMods;
	}
};

// WM_MENUDRAG/WM_MENURBUTTONUP
class CISMH_MenuDrag : public CISMsgHandler
{
public:
	CISMH_MenuDrag(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = item index: %d", WP(pMsg));
		AddParam(pParams, "lp = hmenu: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_MENUCOMMAND
class CISMH_MenuCmd : public CISMsgHandler
{
public:
	CISMH_MenuCmd(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = 0x%08X", WP(pMsg));
		AddParam(pParams, "lp = hmenu: 0x%08X", LP(pMsg));

		return TRUE;
	}
	
};

// WM_UPDATE/CHANGEUISTATE
#ifndef UIS_SET
	#define UIS_SET         1
	#define UIS_CLEAR       2
	#define UIS_INITIALIZE  3
	#define UISF_HIDEFOCUS   0x1
	#define UISF_HIDEACCEL   0x2
#endif

class CISMH_UiState : public CISMsgHandler
{
public:
	CISMH_UiState(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = action: %s, element: %s", MapAction(LOWORD(WP(pMsg))), MapElements(HIWORD(WP(pMsg))));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
	CString MapAction(UINT uAction)
	{
		switch(uAction)
		{
			CASE2STRING(UIS_CLEAR)
			CASE2STRING(UIS_INITIALIZE)
			CASE2STRING(UIS_SET)
		}

		return "none";
	}

	CString MapElements(UINT uElements)
	{
		CString sElement;

		ADDOPTION(UISF_HIDEACCEL, uElements, sElement);
		ADDOPTION(UISF_HIDEFOCUS, uElements, sElement);
	//	ADDOPTION(UISF_ACTIVE, uElements, sElement);

		return sElement;
	}
};

// WM_SET/POPMESSAGESTRING
class CISMH_SetMessageStr : public CISMsgHandler
{
public:
	CISMH_SetMessageStr(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = string id: 0x%04X", WP(pMsg));
		AddParam(pParams, "lp = text: %s", LP(pMsg) ? (LPCTSTR)LP(pMsg) : "");

		return TRUE;
	}
	
};

// WM_IDLEUPDATECMDUI
class CISMH_CmdUI : public CISMsgHandler
{
public:
	CISMH_CmdUI(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = disable (if no handler) flag: %d", WP(pMsg));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};

// WM_FLOATSTATUS
class CISMH_FloatStatus : public CISMsgHandler
{
public:
	CISMH_FloatStatus(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

		AddParam(pParams, "wp = status: %s", MapStatus(WP(pMsg)));
		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
	CString MapStatus(UINT uStatus)
	{
		CString sStatus;

		ADDOPTION(FS_SHOW, uStatus, sStatus);
		ADDOPTION(FS_HIDE, uStatus, sStatus);
		ADDOPTION(FS_ACTIVATE, uStatus, sStatus);
		ADDOPTION(FS_DEACTIVATE, uStatus, sStatus);
		ADDOPTION(FS_ENABLE, uStatus, sStatus);
		ADDOPTION(FS_DISABLE, uStatus, sStatus);
		ADDOPTION(FS_SYNCACTIVE, uStatus, sStatus);

		return sStatus;
	}
};

class CISMH_User : public CISMsgHandler
{
public:
	CISMH_User(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
    ASSERT (pMsg->message >= WM_USER && pMsg->message <= 0x7FFF);

    sMsg = GetUserMessageName(pMsg);

    if (sMsg.IsEmpty())
        sMsg.Format("WM_USER + %d", pMsg->message - WM_USER);

    // default params

		return TRUE;
	}
	
  CString GetUserMessageName(const MSG* pMsg)
  {
    ASSERT(pMsg->message >= WM_USER && pMsg->message <= 0x7FFF);

    // not very efficient 
    CString sMsgName, sClass = CWinClasses::GetClass(pMsg->hwnd);

//     BEGINNOTIFY(WC_BUTTON)
//       CASE2STRING(BM_GETCHECK)   
//       CASE2STRING(BM_SETCHECK)      
//       CASE2STRING(BM_GETSTATE)      
//       CASE2STRING(BM_SETSTATE)    
//       CASE2STRING(BM_SETSTYLE)     
//       CASE2STRING(BM_CLICK)
//       CASE2STRING(BM_GETIMAGE)
//       CASE2STRING(BM_SETIMAGE)
//     ENDNOTIFY     
// 
//     BEGINNOTIFY(WC_STATIC)
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_EDIT)
// //       CASE2STRING(EM_)
// //       CASE2STRING()   
// //       CASE2STRING()      
// //       CASE2STRING()      
// //       CASE2STRING()    
// //       CASE2STRING()     
// //       CASE2STRING()     
// //       CASE2STRING()
// //       CASE2STRING()
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_COMBOBOX)
//       CASE2STRING(CB_ADDSTRING)     
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_COMBOLBOX)
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_LISTBOX)  
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_SCROLLBAR)
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_TOOLBAR)  
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_SPIN)     
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_PROGRESS) 
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_SLIDER)
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_HOTKEY)   
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_SHELLDLLDEFVIEW)  
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_STATUSBAR)       
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_ANIMATE)          
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_RICHEDIT)         
// //       CASE2STRING()
// //       CASE2STRING()   
// //       CASE2STRING()      
// //       CASE2STRING()      
// //       CASE2STRING()    
// //       CASE2STRING()     
// //       CASE2STRING()     
// //       CASE2STRING()
// //       CASE2STRING()
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_RICHEDIT20)       
// //       CASE2STRING()
// //       CASE2STRING()   
// //       CASE2STRING()      
// //       CASE2STRING()      
// //       CASE2STRING()    
// //       CASE2STRING()     
// //       CASE2STRING()     
// //       CASE2STRING()
// //       CASE2STRING()
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_RICHEDIT50)       
// //       CASE2STRING()
// //       CASE2STRING()   
// //       CASE2STRING()      
// //       CASE2STRING()      
// //       CASE2STRING()    
// //       CASE2STRING()     
// //       CASE2STRING()     
// //       CASE2STRING()
// //       CASE2STRING()
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_DATETIMEPICK)     
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_MONTHCAL)         
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_REBAR)            
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_TOOLTIPS) 
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_THUMBNAILVIEW) 
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_IE) 
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_SHDOCOBJVW) 
//     ENDNOTIFY
// 
//     BEGINNOTIFY(WC_SHELLEMBEDDING) 
//     ENDNOTIFY
 
	return sMsgName;
  }

};

class CISMH_App : public CISMsgHandler
{
public:
	CISMH_App(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
    ASSERT (pMsg->message >= WM_APP && pMsg->message <= 0xBFFF);

    sMsg.Format("WM_APP + %d", pMsg->message - WM_APP);

    // default params

		return TRUE;
	}
	
};

class CISMH_Registered : public CISMsgHandler
{
public:
	CISMH_Registered(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message >= 0xC000 && pMsg->message <= 0xFFFF);

		sMsg = GetMessageName(pMsg->message);

		if (!pParams)
			return TRUE;

    // default params

		return TRUE;
	}

protected:
  CMap<UINT, UINT, CString, CString&> m_mapRegisteredMsgs;

protected:
  CString GetMessageName(UINT nMsg)
  {
    ASSERT(nMsg >= 0xC000 && nMsg <= 0xFFFF);

    CString sMsgName;

    if (nMsg >= 0xC000 && nMsg <= 0xFFFF)
    {
      if (!m_mapRegisteredMsgs.Lookup(nMsg, sMsgName))
      {
        int nLen = ::GetClipboardFormatName(nMsg, sMsgName.GetBuffer(512), 512);
        sMsgName.ReleaseBuffer(nLen);
        ASSERT(!sMsgName.IsEmpty());

        m_mapRegisteredMsgs[nMsg] = sMsgName;
      }
    }

    return sMsgName;
  }

};

/*
class CISMH_ : public CISMsgHandler
{
public:
	CISMH_(UINT uMsg, LPCTSTR szMsg) : CISMsgHandler(uMsg, szMsg) {}

	virtual BOOL HandleMsg(const MSG* pMsg, CString& sMsg, CStringArray* pParams)
	{
		ASSERT (pMsg->message == m_uMsg);

		sMsg = m_sMsg;

		if (!pParams)
			return TRUE;

//		AddParam(pParams, "wp = <not used>");
//		AddParam(pParams, "lp = <not used>");

		return TRUE;
	}
	
};
*/


#endif // !defined(AFX_ISMSGHANDLERS_H__F4E70803_C655_4E97_9993_D927A8219B26__INCLUDED_)
