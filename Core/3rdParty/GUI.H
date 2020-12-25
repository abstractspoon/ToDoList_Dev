#ifndef _GUI_H_
#define _GUI_H_

namespace Gui
{
	BOOL GetTrayWndRect(RECT& TrayRect);
	BOOL DoAnimation();
	void MinToTray(HWND hWnd);
	void RestoreFromTray(HWND hWnd, BOOL bForceMax = FALSE);
	BOOL IsObscured(HWND hWnd);
	BOOL HasFocus(HWND hWnd, BOOL bInclChildren);
}

#endif  // _GUI_H_
