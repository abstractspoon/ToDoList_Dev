////////////////////////////////////////////////////////////////
// MSDN Magazine -- October 2004
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET 2003 on Windows XP. Tab size=3.
//
// Implementation of generic inter-app drag-drop manager
//

#pragma once
#include <afxTempl.h>

// registered messages sent by drag-drop manager
const UINT WM_DD_DRAGENTER = RegisterWindowMessage(_T("WM_DD_DRAGENTER"));
const UINT WM_DD_PREDRAGMOVE  = RegisterWindowMessage(_T("WM_DD_PREDRAGMOVE"));
const UINT WM_DD_DRAGOVER  = RegisterWindowMessage(_T("WM_DD_DRAGOVER"));
const UINT WM_DD_DRAGDROP  = RegisterWindowMessage(_T("WM_DD_DRAGDROP"));
const UINT WM_DD_DRAGABORT = RegisterWindowMessage(_T("WM_DD_DRAGABORT"));

// return values for WM_DD_DRAGOVER
enum
{
	DD_DROPEFFECT_NONE = 0,
	DD_DROPEFFECT_MOVE,
	DD_DROPEFFECT_COPY,
	DD_DROPEFFECT_LINK
};

// bit flags used for window type in window map
enum
{
    DDW_NONE   = 0x00,
    DDW_SOURCE = 0x01,   // window is drag-drop source
    DDW_TARGET = 0x02,   // window is drag-drop target
};

// Used to create drag-drop window map. Map is an array of these structs,
// each entry specifies a child window ID and whether the window is a source,
// target, or both.
struct DRAGDROPWND 
{
	UINT id;				// window ID
	UINT type;				// DDW_ flags above
};

// Abstract drag-drop data knows how to draw itself.
//
class CDragDropData 
{
public:
	CDragDropData() { }
	virtual ~CDragDropData() { }

	virtual CImageList* CreateDragImage(CWnd* pWnd, CRect& rc);

	// derived classes must implement these:
	virtual CSize OnGetDragSize(CDC& dc) = 0;
	virtual void  OnDrawData(CDC& dc, CRect& rc, COLORREF& crMask) = 0;
	virtual void* OnGetData() = 0;

protected:
	CBitmap			m_bitmap; // bitmap used for drawing

};

//////////////////
// Concrete class for drag-drop text data.
//
class CDragDropText : public CDragDropData 
{
protected:
	enum { MAXWIDTH=64 };
	CString m_text;
public:
    CDragDropText(LPCTSTR text) : m_text(text) { }
	~CDragDropText() { }

	virtual CSize OnGetDragSize(CDC& dc);
	virtual void  OnDrawData(CDC& dc, CRect& rc, COLORREF& crMask);
	virtual void* OnGetData() { return (void*)(LPCTSTR)m_text; }
};

//////////////////
// Drag-drop structure passed as LPARAM in WM_DD_ messages.
//
struct DRAGDROPINFO 
{
	HWND hwndSource;		// source window
	HWND hwndTarget;		// target window
	BOOL bLeftDrag;			// left or right button drag
	CDragDropData* pData;	// data to drag/drop
	POINT pt;				// current point (cursor) in client coords
							// of whatever window is identified by WPARAM
};

//////////////////
// Inter-app drag-drop manager. To use this, instantiate one instance in your
// main window class and call Install from your OnCreate or OnInitDialog
// method. You must also override your main window's PreTranslateMessage to
// call CDragDropMgr::PreTranslateMessage.
//
typedef CMap<HWND, HWND, UINT, UINT&> HWNDMAP;

class CDragDropMgr 
{
public:
	CDragDropMgr();
	virtual ~CDragDropMgr();

	// Call this to initialize. 2nd arg is array of DRAGDROPWND's, one for
	// each source/target child window.
	BOOL Install(CWnd *pMainWnd, DRAGDROPWND* pWnds);
	BOOL Install(CWnd *pMainWnd, HWND hwnd, int type = DDW_SOURCE | DDW_TARGET);

	// You must call this from your main window's PreTranslateMessage.
	UINT ProcessMessage(const MSG* pMsg, BOOL bAllowNcDrag = FALSE);

	// Call these to add/remove source/target windows dynamically.
	void AddWindow(HWND hwnd, int type);
	void RemoveWindow(HWND hwnd);

	void DragShowNolock(BOOL bShow);
	inline BOOL IsDragging() { return m_iState>=LDRAGGING; }
	void CancelDrag();
    
protected:
	enum { NONE=0, CAPTURED, LDRAGGING, RDRAGGING }; // internal states

	// static stuff 
	CWnd*			m_pMainWnd;				 // main window
	HWNDMAP			m_mapHwnd;				 // map of source/target windows

	// dyanmic stuff used during dragging
	DRAGDROPINFO	m_ddi;					 // data during drag/drop
	UINT			m_iState;				 // current state: CAPTURED/DRAGGING
	HWND			m_hwndTracking;	    	 // window w/mouse capture
	CPoint			m_ptOrg;				 // original point start of drag
	CImageList*		m_pDragImage;			 // imagelist for dragging
	HCURSOR			m_hCursorSave;			 // save cursor
	HINSTANCE		m_hInstOle32;

protected:
	// mouse input handlers: can override if you derive
	virtual BOOL OnButtonDown(const MSG& msg);
	virtual BOOL OnButtonUp(const MSG& msg);
	virtual BOOL OnMouseMove(const MSG& msg);

	// internal helper functions
	void SetState(UINT iState);
	
    inline BOOL IsCaptured() { return m_iState>=CAPTURED; }
    inline BOOL IsSourceWnd(HWND hwnd) { return (GetWindowType(hwnd) & DDW_SOURCE); } 
    inline BOOL IsTargetWnd(HWND hwnd) { return (GetWindowType(hwnd) & DDW_TARGET); }
	UINT GetWindowType(HWND hwnd);

	UINT SendDragMessage(UINT nMessage);
	
};

