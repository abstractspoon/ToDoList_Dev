#include "stdafx.h"
#include "WindowsControl.h"
#include "Window.h"
#include "tchar.h"

//
// Gets the handle for the window this object represents
//
STDMETHODIMP CWindow::get_WindowHandle( long *pVal )
{
    *pVal = (long) m_hWnd;

	return S_OK;
}

//
// Sets the object's window handle
// This is used to change what window the object points to.  Generally, the
//   program should not use this function.  It is used by the GetSubwindow
//   functions to create a new window object pointing to the correct subwindow
//
STDMETHODIMP CWindow::put_WindowHandle( long pVal )
{
    HRESULT hrResult = S_OK;
    LONG nResult;

    //
    // Verify that window exists - All windows (except the desktop) have parent windows
    //
    if ( 0 != pVal )
    {
        nResult = GetWindowLong( (HWND) pVal, GWL_HWNDPARENT );
        if ( 0 == nResult )
        {
            nResult = GetLastError( );
            if ( 0 != nResult )
            {
                return E_INVALIDARG;
            }
        }
    }

    //
    // Set the internal reference
    //
    m_hWnd = (HWND) pVal;

    return hrResult;
}

//
// Posts a windows message to the current window
// Used to send messages that aren't natively supported by the control
//
STDMETHODIMP CWindow::PostMessage( DWORD Message, WPARAM WParam, LPARAM LParam )
{
    BOOL bResult;

    bResult = ::PostMessage( m_hWnd, Message, WParam, LParam );
    if ( !bResult )
    {
        return E_FAIL;
    }

	return S_OK;
}

//
// Returns the window text
//
STDMETHODIMP CWindow::get_Text( BSTR *pVal )
{
    HRESULT hrResult = S_OK;
    WCHAR* wszText;
    TCHAR* tszText;
    BOOL bResult;
    INT nTextLength;

    //
    // Get the length of the text for the window
    //
    nTextLength = SendMessage( m_hWnd, WM_GETTEXTLENGTH, NULL, NULL ) + 1;

    //
    // Allocate the buffer for the text
    //
    tszText = new TCHAR[ nTextLength ];
    if ( NULL == tszText )
    {
        return E_OUTOFMEMORY;
    }
    memset( tszText, 0, nTextLength * sizeof( TCHAR ) );

    do
    {
        //
        // Get the text from the window
        //
        bResult = SendMessage( m_hWnd, WM_GETTEXT, nTextLength, (long) tszText );
        if ( ( !bResult ) && ( nTextLength > 1 ) )
        {
            hrResult = E_FAIL;
            break;
        }

        //
        // Convert to a WCHAR string
        //
        nTextLength = _tcslen( tszText ) + 1;
        wszText = new WCHAR[ nTextLength ];
        if ( NULL == wszText )
        {
            hrResult = E_OUTOFMEMORY;
            break;
        }
        memset( wszText, 0, nTextLength * sizeof( WCHAR ) );


        do
        {
#ifdef UNICODE
            wcscpy( wszText, tszText );
#else // UNICODE
            mbstowcs( wszText, tszText, nTextLength );
#endif // UNICODE

            //
            // Create the BSTR that is returned to the client
            //
            *pVal = SysAllocString( wszText );
            if ( ( NULL == *pVal ) && ( nTextLength > 1 ) )
            {
                hrResult = E_OUTOFMEMORY;
                break;
            }
        }
        while ( FALSE );

        delete[] wszText;
        wszText = NULL;
    }
    while ( FALSE );

    delete[] tszText;
    tszText = NULL;

	return hrResult;
}

//
// Sets the window text
//
STDMETHODIMP CWindow::put_Text( BSTR newVal )
{
    HRESULT hrResult = S_OK;
    TCHAR* tszText;
    INT nTextLength;

    //
    // Create buffer for TCHAR version of text
    //
    nTextLength = SysStringLen( newVal ) + 1;
    tszText = new TCHAR[nTextLength];
    if ( NULL == tszText )
    {
        return E_OUTOFMEMORY;
    }

    do
    {
        //
        // Convert text
        //
#ifdef UNICODE
            wcscpy( tszText, newVal );
#else // UNICODE
            wcstombs( tszText, newVal, nTextLength );
#endif // UNICODE

        //
        // Set the window text
        //
        SendMessage( m_hWnd, WM_SETTEXT, NULL, (long) tszText );
    }
    while ( FALSE );

    delete[] tszText;
    tszText = NULL;

	return hrResult;
}

//
// Searches all subwindows for a window matching a specific title (not case-sensitive)
//
STDMETHODIMP CWindow::GetSubwindowWithTitle( BSTR WindowTitle, IDispatch** SubWindow )
{
    HRESULT hrResult = S_OK;
    TCHAR* tszWindowTitle;
    DWORD dwTitleLength;
    HWND hSubWindow;
    IWindow* pSubWindow = NULL;
    IDispatch* pSubWindowDispatch = NULL;;

    //
    // Validate args
    //
    if ( NULL == SubWindow )
    {
        return E_INVALIDARG;
    }

    //
    // Make buffer
    //
    dwTitleLength = wcslen( WindowTitle ) + 1;
    tszWindowTitle = new TCHAR[ dwTitleLength ];
    if ( NULL == tszWindowTitle )
    {
        return E_OUTOFMEMORY;
    }

    do
    {
        //
        // Convert window title
        //
#ifdef UNICODE
        wcscpy( tszWindowTitle, WindowTitle );
#else
        wcstombs( tszWindowTitle, WindowTitle, dwTitleLength );
#endif

        //
        // Get the subwindow
        //
        hSubWindow = FindWindowEx( m_hWnd, NULL, NULL, tszWindowTitle );

        //
        // Create a new instance of the CWindow class for the subwindow
        //
        hrResult = CoCreateInstance( CLSID_Window, NULL, CLSCTX_ALL, IID_IDispatch, (void**) &pSubWindowDispatch );
        if ( FAILED( hrResult ) )
        {
            break;
        }

        //
        // Get the IWindow interface
        //
        hrResult = pSubWindowDispatch->QueryInterface( IID_IWindow, (void**) &pSubWindow);
        if ( FAILED( hrResult ) )
        {
            pSubWindowDispatch->Release( );
            hrResult = E_FAIL;
            break;
        }

        do
        {
            //
            // Set the window handle
            //
            hrResult = pSubWindow->put_WindowHandle( (long) hSubWindow );
            if ( FAILED( hrResult ) )
            {
                pSubWindowDispatch->Release( );
                hrResult = E_FAIL;
                break;
            }
        }
        while ( FALSE );

        //
        // Get rid of the IWindow interface
        //
        pSubWindow->Release( );

        *SubWindow = pSubWindowDispatch;
    }
    while ( FALSE );

    delete[] tszWindowTitle;
    tszWindowTitle = NULL;

	return hrResult;
}


//
// Get number of subwindows
//
STDMETHODIMP CWindow::get_SubwindowCount( long *pVal )
{
    DWORD dwWindowCount;
    HWND hSubWindow;

    //
    // Validate args
    //
    if ( NULL == pVal )
    {
        return E_INVALIDARG;
    }

    //
    // Count subwindows
    //
    dwWindowCount = 0;
    hSubWindow = NULL;
    do
    {
        //
        // Find the next subwindow
        //
        hSubWindow = FindWindowEx( m_hWnd, hSubWindow, NULL, NULL );

        //
        // If there is a next window, increase count
        //
        if ( hSubWindow != NULL )
        {
            dwWindowCount++;
        }
    }
    while ( NULL != hSubWindow );

    *pVal = dwWindowCount;

	return S_OK;
}


//
// Gets a window object for a subwindow
//
STDMETHODIMP CWindow::get_Subwindow( long Index, IDispatch **pVal )
{
    HRESULT hrResult = S_OK;
    DWORD dwWindowCount;
    HWND hSubWindow;
    IWindow* pSubWindow = NULL;
    IDispatch* pSubWindowDispatch = NULL;

    //
    // Validate args
    //
    if ( ( Index < 1 ) || ( NULL == pVal ) )
    {
        return E_INVALIDARG;
    }

    //
    // Find the specified child window by index
    //
    dwWindowCount = 0;
    hSubWindow = NULL;
    do
    {
        //
        // Find the next subwindow
        //
        hSubWindow = FindWindowEx( m_hWnd, hSubWindow, NULL, NULL );

        //
        // If there is a next window, increase count
        //
        if ( hSubWindow != NULL )
        {
            dwWindowCount++;
        }
    }
    while ( ( dwWindowCount < (DWORD) Index ) && ( NULL != hSubWindow ) );

    //
    // Index was out of range
    //
    if ( NULL == hSubWindow )
    {
        return E_INVALIDARG;
    }

    //
    // Create new window instance
    //
    hrResult = CoCreateInstance( CLSID_Window, NULL, CLSCTX_ALL, IID_IDispatch, (void**) &pSubWindowDispatch );
    if ( FAILED( hrResult ) )
    {
        return hrResult;
    }

    //
    // Get the IWindow interface
    //
    hrResult = pSubWindowDispatch->QueryInterface( IID_IWindow, (void**) &pSubWindow);
    if ( FAILED( hrResult ) )
    {
        pSubWindowDispatch->Release( );
        hrResult = E_FAIL;
        return hrResult;
    }

    do
    {
        //
        // Set the window handle
        //
        hrResult = pSubWindow->put_WindowHandle( (long) hSubWindow );
        if ( FAILED( hrResult ) )
        {
            pSubWindowDispatch->Release( );
            hrResult = E_FAIL;
            break;
        }
    }
    while ( FALSE );

    //
    // Get rid of the IWindow interface
    //
    pSubWindow->Release( );

    *pVal = pSubWindowDispatch;

	return hrResult;
}

//
// Simulate a button click
//
STDMETHODIMP CWindow::ButtonClick( WORD X, WORD Y, WORD KeyCode )
{
    //
    // Simulate a button click with a button down / button up pair
    //
    ::PostMessage( m_hWnd, WM_LBUTTONDOWN, (WPARAM) KeyCode | MK_LBUTTON, (LPARAM) ( ( Y << 16 ) || X ) );
    ::PostMessage( m_hWnd, WM_LBUTTONUP, (WPARAM) KeyCode, (LPARAM) ( ( Y << 16 ) || X ) );

	return S_OK;
}



STDMETHODIMP CWindow::get_ID(long *pVal)
{
    //
    // Retrieve the control ID number
    //
    *pVal = GetWindowLong( m_hWnd, GWL_ID );

	return S_OK;
}


STDMETHODIMP CWindow::get_ParentWindow( IDispatch **pVal )
{
    HRESULT hrResult = S_OK;
    HWND hParentWindow;
    IWindow* pParentWindow = NULL;
    IDispatch* pParentWindowDispatch = NULL;

    //
    // Validate args
    //
    if ( NULL == pVal )
    {
        return E_POINTER;
    }

    //
    // Get the handle to the parent window
    //
    hParentWindow = (HWND) GetWindowLong( m_hWnd, GWL_HWNDPARENT );

    //
    // Create new window instance
    //
    hrResult = CoCreateInstance( CLSID_Window, NULL, CLSCTX_ALL, IID_IDispatch, (void**) &pParentWindowDispatch );
    if ( FAILED( hrResult ) )
    {
        return hrResult;
    }

    //
    // Get the IWindow interface
    //
    hrResult = pParentWindowDispatch->QueryInterface( IID_IWindow, (void**) &pParentWindow);
    if ( FAILED( hrResult ) )
    {
        pParentWindowDispatch->Release( );
        hrResult = E_FAIL;
        return hrResult;
    }

    do
    {
        //
        // Set the window handle
        //
        hrResult = pParentWindow->put_WindowHandle( (long) hParentWindow );
        if ( FAILED( hrResult ) )
        {
            pParentWindowDispatch->Release( );
            hrResult = E_FAIL;
            break;
        }
    }
    while ( FALSE );

    //
    // Get rid of the IWindow interface
    //
    pParentWindow->Release( );

    *pVal = pParentWindowDispatch;

	return hrResult;
}

//
// Gets if the window is enabled or not
//
STDMETHODIMP CWindow::get_Enabled( VARIANT_BOOL *pVal )
{
    HRESULT hrResult = S_OK;
    LONG nStyles;


    if ( NULL == pVal )
    {
        return E_POINTER;
    }

    //
    // Get window styles
    //
    nStyles = GetWindowLong( m_hWnd, GWL_STYLE );

    //
    // Use window styles to determine if window is enabled
    //
    if ( ( nStyles & WS_DISABLED)  == WS_DISABLED )
    {
        *pVal = FALSE;
    }
    else
    {
        *pVal = TRUE;
    }

	return hrResult;
}

STDMETHODIMP CWindow::put_Enabled( VARIANT_BOOL newVal )
{
    HRESULT hrResult = S_OK;

    //
    // Enable / disable window
    //
    ::EnableWindow( m_hWnd, newVal );

	return hrResult;
}

//
// Returns true if window is visible
//
STDMETHODIMP CWindow::get_Visible( VARIANT_BOOL *pVal )
{
    BOOL bIsVisible;

    bIsVisible = ::IsWindowVisible( m_hWnd );
    *pVal = (short) bIsVisible;

	return S_OK;
}

//
// Sets if a window is visible
//
STDMETHODIMP CWindow::put_Visible( VARIANT_BOOL newVal )
{
    if ( newVal )
    {
        ::ShowWindow( m_hWnd, SW_SHOWNOACTIVATE );
    }
    else
    {
        ::ShowWindow( m_hWnd, SW_HIDE );
    }

	return S_OK;
}

STDMETHODIMP CWindow::SendChar( BSTR Character, LONG TransitionState )
{
    HRESULT hrResult = S_OK;
    DWORD dwStringLength;
    DWORD dwIndex;

    if ( NULL == Character )
    {
        return S_OK;
    }

	//
    // Get length of string
    //
    dwStringLength = SysStringLen( Character );

    //
    // Loop through all characters in string, and send them to the window
    //
    for ( dwIndex = 0; dwIndex < dwStringLength; dwIndex++ )
    {
        hrResult = SendKeyPress( ((WCHAR*) Character)[dwIndex], (DWORD) TransitionState );
        if ( FAILED( hrResult ) )
        {
            break;
        }
    }

	return hrResult;
}


HRESULT CWindow::SendKeyPress( WCHAR wchCharacter, DWORD dwKeyCode )
{
    HRESULT hrResult = S_OK;
    LPARAM lKeyCode;

    //
    // Calculate scan code for character
    //
    lKeyCode = (LPARAM) ( dwKeyCode & 0x01ff0000 ) |  0x00400001; // 1 key down press

    //
    // Send message - WM_CHAR
    //
    ::PostMessage( m_hWnd, WM_CHAR, (WPARAM) wchCharacter, lKeyCode );

    return hrResult;
}


STDMETHODIMP CWindow::GetSubwindowWithID( DWORD SubWindowID, IDispatch** SubWindow )
{
    HRESULT hrResult = S_OK;
    HWND hSubWindow;
    IWindow* pSubWindow = NULL;
    IDispatch* pSubWindowDispatch = NULL;
    DWORD dwSubwindowID;

    //
    // Validate args
    //
    if ( NULL == SubWindow )
    {
        return E_INVALIDARG;
    }

    do
    {
        //
        // Find the correct subwindow
        //
        hSubWindow = NULL;
        do
        {
            hSubWindow = FindWindowEx( m_hWnd, hSubWindow, NULL, NULL );
            dwSubwindowID = (DWORD) ::GetWindowLong( hSubWindow, GWL_ID );
        }
        while ( ( NULL != hSubWindow ) && ( SubWindowID != dwSubwindowID ) );

        if ( NULL == hSubWindow )
        {
            hrResult = E_INVALIDARG;
            break;
        }

        //
        // Create a new instance of the CWindow class for the subwindow
        //
        hrResult = CoCreateInstance( CLSID_Window, NULL, CLSCTX_ALL, IID_IDispatch, (void**) &pSubWindowDispatch );
        if ( FAILED( hrResult ) )
        {
            break;
        }

        //
        // Get the IWindow interface
        //
        hrResult = pSubWindowDispatch->QueryInterface( IID_IWindow, (void**) &pSubWindow);
        if ( FAILED( hrResult ) )
        {
            pSubWindowDispatch->Release( );
            hrResult = E_FAIL;
            break;
        }

        do
        {
            //
            // Set the window handle
            //
            hrResult = pSubWindow->put_WindowHandle( (long) hSubWindow );
            if ( FAILED( hrResult ) )
            {
                pSubWindowDispatch->Release( );
                hrResult = E_FAIL;
                break;
            }
        }
        while ( FALSE );

        //
        // Get rid of the IWindow interface
        //
        pSubWindow->Release( );

        *SubWindow = pSubWindowDispatch;
    }
    while ( FALSE );

	return hrResult;
}


STDMETHODIMP CWindow::get_Height( long *pVal )
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Validate parameters
    //
    if ( NULL == pVal )
    {
        return E_POINTER;
    }

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    *pVal = rectWindowDimensions.bottom - rectWindowDimensions.top;

	return hrResult;
}


STDMETHODIMP CWindow::put_Height(long newVal)
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    bResult = ::MoveWindow( m_hWnd,
        rectWindowDimensions.left,
        rectWindowDimensions.top,
        rectWindowDimensions.right - rectWindowDimensions.left,
        newVal,
        TRUE );
    if ( !bResult )
    {
        return E_FAIL;
    }

	return hrResult;
}


STDMETHODIMP CWindow::get_Width( long *pVal )
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Validate parameters
    //
    if ( NULL == pVal )
    {
        return E_POINTER;
    }

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    *pVal = rectWindowDimensions.right - rectWindowDimensions.left;

	return hrResult;
}

STDMETHODIMP CWindow::put_Width( long newVal )
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    bResult = ::MoveWindow( m_hWnd,
        rectWindowDimensions.left,
        rectWindowDimensions.top,
        newVal,
        rectWindowDimensions.bottom - rectWindowDimensions.top,
        TRUE );
    if ( !bResult )
    {
        return E_FAIL;
    }

	return hrResult;
}

STDMETHODIMP CWindow::get_X( long *pVal )
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Validate parameters
    //
    if ( NULL == pVal )
    {
        return E_POINTER;
    }

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    *pVal = rectWindowDimensions.left;

	return hrResult;
}

STDMETHODIMP CWindow::put_X( long newVal )
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    bResult = ::MoveWindow( m_hWnd,
        newVal,
        rectWindowDimensions.top,
        rectWindowDimensions.right - rectWindowDimensions.left,
        rectWindowDimensions.bottom - rectWindowDimensions.top,
        TRUE );
    if ( !bResult )
    {
        return E_FAIL;
    }

	return hrResult;
}

STDMETHODIMP CWindow::get_Y( long *pVal )
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Validate parameters
    //
    if ( NULL == pVal )
    {
        return E_POINTER;
    }

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    *pVal = rectWindowDimensions.top;

	return hrResult;
}

STDMETHODIMP CWindow::put_Y( long newVal )
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    bResult = ::MoveWindow( m_hWnd,
        rectWindowDimensions.left,
        newVal,
        rectWindowDimensions.right - rectWindowDimensions.left,
        rectWindowDimensions.bottom - rectWindowDimensions.top,
        TRUE );
    if ( !bResult )
    {
        return E_FAIL;
    }

	return hrResult;
}

STDMETHODIMP CWindow::MoveWindow( WORD X, WORD Y, WORD Width, WORD Height )
{
    HRESULT hrResult = S_OK;
    BOOL bResult;
    RECT rectWindowDimensions;

    //
    // Get the window dimensions
    //
    bResult = ::GetWindowRect( m_hWnd, &rectWindowDimensions );
    if ( !bResult )
    {
        return E_FAIL;
    }

    bResult = ::MoveWindow( m_hWnd,
        X,
        Y,
        Width,
        Height,
        TRUE );
    if ( !bResult )
    {
        return E_FAIL;
    }

	return hrResult;
}

STDMETHODIMP CWindow::Activate( )
{
    HRESULT hrResult = S_OK;
    INT nResult;

    nResult = ::ShowWindow( m_hWnd, SW_NORMAL );
    if ( !nResult )
    {
        hrResult = E_FAIL;
        return hrResult;
    }

    nResult = ::SetForegroundWindow( m_hWnd );
    if ( !nResult )
    {
        hrResult = E_FAIL;
        return hrResult;
    }

	return hrResult;
}


STDMETHODIMP CWindow::Close( )
{
    //
    // Post a close message to the window
    //
    ::PostMessage( m_hWnd, WM_CLOSE, (WPARAM) 0, (LPARAM) 0 );

	return S_OK;
}


STDMETHODIMP CWindow::get_Active( VARIANT_BOOL *pVal )
{
	HWND hWndActiveWindow;

    if ( NULL == pVal )
        return E_POINTER;

    hWndActiveWindow = GetForegroundWindow();

    *pVal = ( m_hWnd == hWndActiveWindow );

	return S_OK;
}


STDMETHODIMP CWindow::put_Active( VARIANT_BOOL newVal )
{
    HWND hwndDesktop;
    INT nResult;

    if ( newVal )
    {
    	return Activate();
    }
    else
    {
        hwndDesktop = GetDesktopWindow();
        nResult = SetForegroundWindow( hwndDesktop );
        if ( !nResult )
        {
            return HRESULT_FROM_WIN32( GetLastError() );
        }

        return S_OK;
    }
}
