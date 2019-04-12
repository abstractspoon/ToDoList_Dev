#ifndef _GUI_H_
#define _GUI_H_

namespace Gui
{
	BOOL GetTrayWndRect(RECT& TrayRect);
	BOOL DoAnimation();
	void MinToTray(const HWND hWnd);
	void RestoreFromTray(const HWND hWnd, BOOL bForceMax = FALSE);
	BOOL IsObscured(const HWND hWnd);
	BOOL HasFocus(const HWND hWnd, BOOL bInclChildren);
}

#endif  // _GUI_H_
