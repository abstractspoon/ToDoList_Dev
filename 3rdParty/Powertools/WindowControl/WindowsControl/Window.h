#ifndef __WINDOW_H_
#define __WINDOW_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWindow
class ATL_NO_VTABLE CWindow :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWindow, &CLSID_Window>,
	public IDispatchImpl<IWindow, &IID_IWindow, &LIBID_WINDOWSCONTROLLib>
{
public:
	CWindow( )
	{
        m_hWnd = NULL; // Default to the desktop
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WINDOW)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWindow)
	COM_INTERFACE_ENTRY(IWindow)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWindow
public:
	STDMETHOD(GetSubwindowWithTitle)(/*[in]*/ BSTR WindowTitle, /*[out, retval]*/ IDispatch** SubWindow);
	STDMETHOD(put_WindowHandle)(/*[in]*/ long pVal);
	STDMETHOD(get_Text)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Text)(/*[in]*/ BSTR newVal);
	STDMETHOD(PostMessage)(DWORD Message, WPARAM WParam, LPARAM LParam);
	STDMETHOD(get_WindowHandle)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Active)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Active)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(Close)();
	STDMETHOD(Activate)();
	STDMETHOD(MoveWindow)(/*[in]*/ WORD X, /*[in]*/ WORD Y, /*[in]*/ WORD Width, /*[in]*/ WORD Height);
	STDMETHOD(get_Y)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Y)(/*[in]*/ long newVal);
	STDMETHOD(get_X)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_X)(/*[in]*/ long newVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Width)(/*[in]*/ long newVal);
	STDMETHOD(get_Height)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Height)(/*[in]*/ long newVal);
	STDMETHOD(GetSubwindowWithID)(/*[in]*/ DWORD SubWindowID, /*[out, retval]*/ IDispatch** SubWindow);
	STDMETHOD(SendChar)(/*[in]*/ BSTR Character, /*[in]*/ LONG TransitionState);
	STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Enabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Enabled)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ParentWindow)(/*[out, retval]*/ IDispatch* *pVal);
	STDMETHOD(get_ID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(ButtonClick)(/*[in]*/ WORD X, /*[in]*/ WORD Y, /*[in]*/WORD KeyCode );
	STDMETHOD(get_Subwindow)(/*[in]*/ long Index, /*[out, retval]*/ IDispatch* *pVal);
	STDMETHOD(get_SubwindowCount)(/*[out, retval]*/ long *pVal);

protected:
	HRESULT SendKeyPress( WCHAR wchCharacter, DWORD dwKeyCode );
	HWND m_hWnd;
};

#endif //__WINDOW_H_
