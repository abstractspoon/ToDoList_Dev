#ifndef EWINDOWTARGET_H
#define EWINDOWTARGET_H

//==================================================================
// Class EWindowTarget by Giovanni Bavestrelli
// heavily adapted by d.r.godson
//==================================================================

// Class that keeps an internal hidden window for message handling
// All messages are passed to virtual function HandleWindowMessage
class EWindowTarget
{
public:
	EWindowTarget();
	virtual ~EWindowTarget();

	LRESULT ESendMessage( UINT message, WPARAM wParam = 0, LPARAM lParam = 0 );
	BOOL EPostMessage( UINT message, WPARAM wParam = 0, LPARAM lParam = 0 );
	
protected: 				  
	// Override it to handle your messages				  
	virtual long EOnWndMessage(UINT, WPARAM, LPARAM);
	
	// Override to modify window creation styles
	virtual void ECustomizeCreation(CREATESTRUCT & cs){}
	
	HWND EGetHWND() const { return m_hWnd; }
	CWnd* EGetWnd() const { return CWnd::FromHandle(m_hWnd); }
	BOOL EIsWindowOpen() const { return m_hWnd && IsWindow(m_hWnd);}
	HWND EOpenWindow();  // Creates window
	void ECloseWindow(); // Destroy window
	
private:
	HWND m_hWnd; // Handle of internal hidden window

	// Window Procedure of internal window 
	static LRESULT CALLBACK EWindowTargetProc(HWND, UINT, WPARAM, LPARAM); 
	long EHandleWindowMessage(HWND, UINT, WPARAM, LPARAM);
	
};


#endif 
