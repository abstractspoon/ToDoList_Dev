//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 2001
//
//  File:       whotkey.cpp
//
//  Contents:   WHotKey.cpp : Defines the entry point for the application.
//
//
//  Author:     Jporkka, lylec,
//----------------------------------------------------------------------------

#include "stdafx.h"

#define MAX_HMONITORS 50
long     g_nMonitors;
HMONITOR g_hMonitors[MAX_HMONITORS];

char achUsage[] = "JumpWin v1.6 - Monitor switching utility for application windows.\n\n"
                  "Usage: JumpWin [-alt] [-ctrl]\n"
                  "\tBy default the key combination WIN+Enter moves the current active window to\n"
                  "\tthe next monitor. The restore position of the window will be scaled to be\n"
                  "\tproportionate to the size on the previous monitor. If the window is maximized, it\n"
                  "\twill be maximized on the new monitor as well. To disable the scaling, add the\n"
                  "\tSHIFT key to your key combination (e.g. SHIFT+WIN+Enter), as some applications\n"
                  "\tdo not handle resizing of their windows well.\n\n"
                  "Arguments:\n\n"
                  "\t-alt:\tMake the key combinations ALT+Enter and SHIFT+ALT+Enter\n"
                  "\t-ctrl:\tMake the key combinations CTRL+Enter and SHIFT+CTRL+Enter\n\n";


//+---------------------------------------------------------------------------
//
//  Function:   MonEnumProc
//
//  Synopsis:   The callback stub for enumerating the monitors on the system
//
//  Arguments:  [hMon]   --
//              [hdc]    --
//              [rect]   --
//              [dwData] --
//
//  Returns:    FALSE to abort the enumeration.
//
//----------------------------------------------------------------------------
BOOL _stdcall MonEnumProc(HMONITOR hMon, HDC hdc, LPRECT rect, long dwData)
{
    if (g_nMonitors < MAX_HMONITORS)
    {
        g_hMonitors[g_nMonitors] = hMon;
        g_nMonitors++;
    }
    return TRUE;
}

//
// ScaleWindowSizeAndPosition
//
// This function works to handle the case where monitors have
//   different resolutions by scaling the window size to be
//   the same relative size on each monitor. If the monitors
//   are the same size then it does nothing.
//
// The scaling is done by computing what percentage bigger or
//   smaller the new monitor is over the old. Then, the top/left
//   position and window size are scaled by the same amounts.
//
// This call should be made before WINDOWPLACEMENT has been updated
//   to put the window in its new location. This is because the calculations
//   are done by normalizing the RECTs to the position of pmiWin.
//
void ScaleWindowSizeAndPosition(MONITORINFO *pmiWin,
                                MONITORINFO *pmiNext,
                                WINDOWPLACEMENT *pwp)
{
    long lScale;
    long lSizeOld;
    long lSizeNew;
    int  xOffset;
    int  yOffset;

    // First, our calculations assume the upper left corner of pmiWin.rcWork
    //  is (0,0). So, normalize to this and then restore it at the end.

    xOffset = pmiWin->rcWork.left;
    yOffset = pmiWin->rcWork.top;

    OffsetRect(&pwp->rcNormalPosition, -xOffset, -yOffset);

    // First, check the horizontal direction.

    lSizeOld = pmiWin->rcWork.right - pmiWin->rcWork.left;
    lSizeNew = pmiNext->rcWork.right - pmiNext->rcWork.left;

    if (lSizeOld != lSizeNew)
    {
        // Multiply by 1000 to avoid needing to use floating point.
        lScale = (lSizeNew * 1000) / lSizeOld;

        pwp->rcNormalPosition.left  = (pwp->rcNormalPosition.left * lScale) / 1000;
        pwp->rcNormalPosition.right = (pwp->rcNormalPosition.right * lScale) / 1000;
    }

    // Now, check the vertical direction.

    lSizeOld = pmiWin->rcWork.bottom - pmiWin->rcWork.top;
    lSizeNew = pmiNext->rcWork.bottom - pmiNext->rcWork.top;

    if (lSizeOld != lSizeNew)
    {
        // Multiply by 1000 to avoid needing to use floating point.
        lScale = (lSizeNew * 1000) / lSizeOld;

        pwp->rcNormalPosition.top    = (pwp->rcNormalPosition.top * lScale) / 1000;
        pwp->rcNormalPosition.bottom = (pwp->rcNormalPosition.bottom * lScale) / 1000;
    }

    // Undo our normalization

    OffsetRect(&pwp->rcNormalPosition, xOffset, yOffset);
}

//+---------------------------------------------------------------------------
//
//  Function:   MoveWindowToMonitor
//
//  Synopsis:   Move a window from its current montor to the "next"
//              monitor, optionally scaleing the window to fit when the
//              monitors have differing resolutions.
//
//  Arguments:  [win]     -- The target window
//              [hNewMon] -- The target monitor
//              [fScale]  -- TRUE to resize the window to fit.
//
//----------------------------------------------------------------------------
void MoveWindowToMonitor(HWND win, HMONITOR hNewMon, BOOL fScale)
{
    HMONITOR hWinMon = MonitorFromWindow(win, MONITOR_DEFAULTTONEAREST);
    MONITORINFO miWin = {sizeof(MONITORINFO)};
    GetMonitorInfo(hWinMon, &miWin);

    MONITORINFO miNext = {sizeof(MONITORINFO)};
    GetMonitorInfo(hNewMon, &miNext);

    if (hNewMon != hWinMon)
    {
        WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};

        if (!GetWindowPlacement(win, &wp))
            return;

        //
        // The problem here is that rcMonitor and rcWork are both in screen coordinates,
        //   while rcNormalPosition is in workspace coordinates. Unfortunately, (0,0) for
        //   workspace coordinates can change depending on what monitor you're talking about.
        //
        // So, we convert wp.rcNormalPosition to screen coordinates, do our calculations,
        //   and then convert it back afterward.
        //
        OffsetRect(&wp.rcNormalPosition,
                   miWin.rcWork.left - miWin.rcMonitor.left,
                   miWin.rcWork.top - miWin.rcMonitor.top);

        int offsetx = miNext.rcWork.left - miWin.rcWork.left;
        int offsety = miNext.rcWork.top  - miWin.rcWork.top;

        if (fScale)
        {
            LONG style = GetWindowLong(win, GWL_STYLE);
            if (style & WS_SIZEBOX)
                ScaleWindowSizeAndPosition(&miWin, &miNext, &wp);
        }        

        OffsetRect(&wp.rcNormalPosition, offsetx, offsety);

        // Convert to workspace coordinates in the new workspace
        OffsetRect(&wp.rcNormalPosition,
                   -(miNext.rcWork.left - miNext.rcMonitor.left),
                   -(miNext.rcWork.top - miNext.rcMonitor.top));

        UINT showCmd = wp.showCmd;

        if (showCmd == SW_SHOWMAXIMIZED)
        {
//            ShowWindow(win, SW_HIDE);
            wp.showCmd = SW_RESTORE;
            SetWindowPlacement(win, &wp);

            wp.showCmd = showCmd;
            SetWindowPlacement(win, &wp);
        }
        else if (showCmd == SW_NORMAL)
        {
            SetWindowPlacement(win, &wp);
        }
    }

}

void JumpWindow(BOOL fScale)
{
    HWND win = GetForegroundWindow();
    if (win == 0)
        return;

    HMONITOR hWinMon = MonitorFromWindow(win, MONITOR_DEFAULTTONEAREST);

    g_nMonitors = 0;
    EnumDisplayMonitors(0, 0, MonEnumProc, 0);
    int i;
    for(i = 0; i < g_nMonitors; ++i)
    {
        if (g_hMonitors[i] == hWinMon)
            break;
    }

    if (i < g_nMonitors)
    {
        ++i;
        if (i >= g_nMonitors)
            i = 0;

        MoveWindowToMonitor(win, g_hMonitors[i], fScale);
    }
}

static bool AmIOnlyOne()
{
    HANDLE hMutex = CreateMutex(0, FALSE, "JumpWinMutex");

    if (!hMutex)
    {
        return false;
    }

    if( GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return false;
    }

    return true;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    UINT nModifier = MOD_WIN;

    if (lpCmdLine[0] == '-' || lpCmdLine[0] == '/')
    {
        lpCmdLine++;
        if ( !lstrcmpi(lpCmdLine, "alt") )
            nModifier = MOD_ALT;
        else if ( !lstrcmpi(lpCmdLine, "ctrl") )
            nModifier = MOD_CONTROL;
        else if ( !lstrcmpi(lpCmdLine, "?") )
        {
            MessageBox(NULL, achUsage, "JumpWin", MB_OK);

            return 0;
        }
    }

    BOOL fReturn = RegisterHotKey(0, 1, nModifier, VK_RETURN);
    BOOL fShiftReturn = RegisterHotKey(0, 2, nModifier | MOD_SHIFT, VK_RETURN);
    BOOL fOnlyOne = AmIOnlyOne();

    if (!fShiftReturn || !fReturn || !fOnlyOne)
        return 0;

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0)
    {
        if (msg.message == WM_HOTKEY && msg.wParam  == 1)
        {
            JumpWindow(true);
        }
        else if (msg.message == WM_HOTKEY && msg.wParam == 2)
        {
            JumpWindow(false);
        }
    }

    return 0;
}



