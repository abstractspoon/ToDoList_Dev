# Microsoft Developer Studio Project File - Name="Shared" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Shared - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Shared.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Shared.mak" CFG="Shared - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Shared - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Shared - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Shared", HACAAAAA"
# PROP Scc_LocalPath "..\..\shared"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Shared - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Shared - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Shared - Win32 Unicode Debug"
# Name "Shared - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AcceleratorString.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BinaryData.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalendarCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckListBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Clipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorBrewer.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorBrewerCombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPickerEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPopupEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboListboxPositioner.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyWndContents.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlTextHighlighter.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DarkMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DateHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\DateTimeCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\DayOfWeekchecklistbox.cpp
# End Source File
# Begin Source File

SOURCE=.\DayOfWeekcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\DeferWndMove.cpp
# End Source File
# Begin Source File

SOURCE=.\DelayRedraw.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUnits.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\EditShortcutMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\EnBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\EnBitmapEx.cpp
# End Source File
# Begin Source File

SOURCE=.\EnBrowserCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EnCheckComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\EnColorDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EnCommandLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\EnEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\EnFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EnHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EnImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\EnListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EnMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\EnRecentFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\EnStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\EnString.cpp
# End Source File
# Begin Source File

SOURCE=.\EnToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\FileComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FileEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIcons.cpp
# End Source File
# Begin Source File

SOURCE=.\FileMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\FileRegister.cpp
# End Source File
# Begin Source File

SOURCE=.\FindReplace.cpp
# End Source File
# Begin Source File

SOURCE=.\FocusWatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FontCache.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicsMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupLine.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\HMXChartEx.cpp
# End Source File
# Begin Source File

SOURCE=.\HoldRedraw.cpp
# End Source File
# Begin Source File

SOURCE=.\hotkeyctrlex.cpp
# End Source File
# Begin Source File

SOURCE=.\HotTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\Icon.cpp
# End Source File
# Begin Source File

SOURCE=.\IconButton.cpp
# End Source File
# Begin Source File

SOURCE=.\IconCache.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessors.cpp
# End Source File
# Begin Source File

SOURCE=.\InputListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Localizer.cpp
# End Source File
# Begin Source File

SOURCE=.\MaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuIconMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MidnightTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniCalendarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniCalendarListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc.cpp
# End Source File
# Begin Source File

SOURCE=.\MonthCalCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MonthCheckComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MonthComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MouseWheelMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MSOutlookHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\MSProjectHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\MSWordHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\NcGutter.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDropTargetEx.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderedTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OwnerdrawComboBoxBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswordDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupListBoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageHost.cpp
# End Source File
# Begin Source File

SOURCE=.\RangeSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RCCtrlParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Recurrence.cpp
# End Source File
# Begin Source File

SOURCE=.\Regkey.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditBaseCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditNcBorder.cpp
# End Source File
# Begin Source File

SOURCE=.\Rtf2HtmlConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\RTLInputMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\RuntimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveFocus.cpp
# End Source File
# Begin Source File

SOURCE=.\ScopedTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollingPropertyPageHost.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionStatusWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SizeGrip.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBarACTEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBarProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\StickiesAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\StickiesWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StylesheetParamConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolButton.cpp
# End Source File
# Begin Source File

SOURCE=.\TabbedComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TabbedPropertyPageHost.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Themed.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolbarHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolTipCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeDragDropHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListSyncer.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeSelectionHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\Unzipper.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlParser.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\WebBrowserCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\WinClasses.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowIcons.cpp
# End Source File
# Begin Source File

SOURCE=.\WinHelpButton.cpp
# End Source File
# Begin Source File

SOURCE=.\WndPrompt.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkingWeek.cpp
# End Source File
# Begin Source File

SOURCE=.\XmlFile.cpp
# End Source File
# Begin Source File

SOURCE=.\XmlFileEx.cpp
# End Source File
# Begin Source File

SOURCE=.\XslFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Zipper.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\AcceleratorString.h
# End Source File
# Begin Source File

SOURCE=.\AutoComboBox.h
# End Source File
# Begin Source File

SOURCE=.\AutoFlag.h
# End Source File
# Begin Source File

SOURCE=.\BinaryData.h
# End Source File
# Begin Source File

SOURCE=.\BrowserDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalendarCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\CheckComboBox.h
# End Source File
# Begin Source File

SOURCE=.\CheckListBoxEx.h
# End Source File
# Begin Source File

SOURCE=.\Clipboard.h
# End Source File
# Begin Source File

SOURCE=.\ColorBrewer.h
# End Source File
# Begin Source File

SOURCE=.\ColorBrewerCombobox.h
# End Source File
# Begin Source File

SOURCE=.\ColorButton.h
# End Source File
# Begin Source File

SOURCE=.\ColorComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ColourPickerEx.h
# End Source File
# Begin Source File

SOURCE=.\ColourPopupEx.h
# End Source File
# Begin Source File

SOURCE=.\ComboListboxPositioner.h
# End Source File
# Begin Source File

SOURCE=.\CopyWndContents.h
# End Source File
# Begin Source File

SOURCE=.\CtrlTextHighlighter.h
# End Source File
# Begin Source File

SOURCE=.\CustomButton.h
# End Source File
# Begin Source File

SOURCE=.\DarkMode.h
# End Source File
# Begin Source File

SOURCE=.\DateHelper.h
# End Source File
# Begin Source File

SOURCE=.\DateHelperEnums.h
# End Source File
# Begin Source File

SOURCE=.\DateTimeCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\DayOfWeekchecklistbox.h
# End Source File
# Begin Source File

SOURCE=.\DayOfWeekcombobox.h
# End Source File
# Begin Source File

SOURCE=.\DeferWndMove.h
# End Source File
# Begin Source File

SOURCE=.\DelayRedraw.h
# End Source File
# Begin Source File

SOURCE=.\DialogHelper.h
# End Source File
# Begin Source File

SOURCE=.\DlgUnits.h
# End Source File
# Begin Source File

SOURCE=.\DriveInfo.h
# End Source File
# Begin Source File

SOURCE=.\EditShortcutMgr.h
# End Source File
# Begin Source File

SOURCE=.\EnBitmap.h
# End Source File
# Begin Source File

SOURCE=.\EnBitmapEx.h
# End Source File
# Begin Source File

SOURCE=.\EnBrowserCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EnCheckComboBox.h
# End Source File
# Begin Source File

SOURCE=.\EnColorDialog.h
# End Source File
# Begin Source File

SOURCE=.\EnCommandLineInfo.h
# End Source File
# Begin Source File

SOURCE=.\EnEdit.h
# End Source File
# Begin Source File

SOURCE=.\EnFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\EnHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EnImageList.h
# End Source File
# Begin Source File

SOURCE=.\EnListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EnMenu.h
# End Source File
# Begin Source File

SOURCE=.\EnRecentFileList.h
# End Source File
# Begin Source File

SOURCE=.\EnStatic.h
# End Source File
# Begin Source File

SOURCE=.\EnString.h
# End Source File
# Begin Source File

SOURCE=.\EnToolBar.h
# End Source File
# Begin Source File

SOURCE=.\FileComboBox.h
# End Source File
# Begin Source File

SOURCE=.\FileEdit.h
# End Source File
# Begin Source File

SOURCE=.\FileIcons.h
# End Source File
# Begin Source File

SOURCE=.\FileMisc.h
# End Source File
# Begin Source File

SOURCE=.\FileRegister.h
# End Source File
# Begin Source File

SOURCE=.\FindReplace.h
# End Source File
# Begin Source File

SOURCE=.\FocusWatcher.h
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.h
# End Source File
# Begin Source File

SOURCE=.\FontCache.h
# End Source File
# Begin Source File

SOURCE=.\GraphicsMisc.h
# End Source File
# Begin Source File

SOURCE=.\GroupLine.h
# End Source File
# Begin Source File

SOURCE=.\HistoryComboBox.h
# End Source File
# Begin Source File

SOURCE=.\HMXChartEx.h
# End Source File
# Begin Source File

SOURCE=.\HoldRedraw.h
# End Source File
# Begin Source File

SOURCE=.\HookMgr.h
# End Source File
# Begin Source File

SOURCE=.\HookWndMgr.h
# End Source File
# Begin Source File

SOURCE=.\HotKeyCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\HotTracker.h
# End Source File
# Begin Source File

SOURCE=.\HtmlCharMap.h
# End Source File
# Begin Source File

SOURCE=.\Icon.h
# End Source File
# Begin Source File

SOURCE=.\IconButton.h
# End Source File
# Begin Source File

SOURCE=.\IconCache.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessors.h
# End Source File
# Begin Source File

SOURCE=.\InputListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LightBox.h
# End Source File
# Begin Source File

SOURCE=.\Localizer.h
# End Source File
# Begin Source File

SOURCE=.\MapEx.h
# End Source File
# Begin Source File

SOURCE=.\MaskEdit.h
# End Source File
# Begin Source File

SOURCE=.\MenuButton.h
# End Source File
# Begin Source File

SOURCE=.\MenuComboBox.h
# End Source File
# Begin Source File

SOURCE=.\MenuIconMgr.h
# End Source File
# Begin Source File

SOURCE=.\MessageBox.h
# End Source File
# Begin Source File

SOURCE=.\MidnightTimer.h
# End Source File
# Begin Source File

SOURCE=.\MiniCalendarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MiniCalendarListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Misc.h
# End Source File
# Begin Source File

SOURCE=.\MonthCalCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\MonthCheckComboBox.h
# End Source File
# Begin Source File

SOURCE=.\MonthComboBox.h
# End Source File
# Begin Source File

SOURCE=.\MouseWheelMgr.h
# End Source File
# Begin Source File

SOURCE=.\MSOutlookHelper.h
# End Source File
# Begin Source File

SOURCE=.\MSOutlookHelperEnum.h
# End Source File
# Begin Source File

SOURCE=.\MSProjectHelper.h
# End Source File
# Begin Source File

SOURCE=.\MSWordHelper.h
# End Source File
# Begin Source File

SOURCE=.\NcGutter.h
# End Source File
# Begin Source File

SOURCE=.\OleDropTargetEx.h
# End Source File
# Begin Source File

SOURCE=.\OrderedTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OwnerdrawComboBoxBase.h
# End Source File
# Begin Source File

SOURCE=.\PasswordDialog.h
# End Source File
# Begin Source File

SOURCE=.\PopupEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PopupListBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesBase.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageHost.h
# End Source File
# Begin Source File

SOURCE=.\RangeSliderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RCCtrlParser.h
# End Source File
# Begin Source File

SOURCE=.\Recurrence.h
# End Source File
# Begin Source File

SOURCE=.\Regkey.h
# End Source File
# Begin Source File

SOURCE=.\RemoteFile.h
# End Source File
# Begin Source File

SOURCE=.\RemoteFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\RichEditBaseCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RichEditHelper.h
# End Source File
# Begin Source File

SOURCE=.\RichEditNcBorder.h
# End Source File
# Begin Source File

SOURCE=.\Rtf2HtmlConverter.h
# End Source File
# Begin Source File

SOURCE=.\RTLInputMgr.h
# End Source File
# Begin Source File

SOURCE=.\RuntimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SaveFocus.h
# End Source File
# Begin Source File

SOURCE=.\ScopedTimer.h
# End Source File
# Begin Source File

SOURCE=.\ScrollingPropertyPageHost.h
# End Source File
# Begin Source File

SOURCE=.\SendFileToEx.h
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\SessionStatusWnd.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutManager.h
# End Source File
# Begin Source File

SOURCE=.\SizeGrip.h
# End Source File
# Begin Source File

SOURCE=.\SoundEdit.h
# End Source File
# Begin Source File

SOURCE=.\StatusBarACTEx.h
# End Source File
# Begin Source File

SOURCE=.\StatusBarProgress.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StickiesAPI.h
# End Source File
# Begin Source File

SOURCE=.\StickiesWnd.h
# End Source File
# Begin Source File

SOURCE=.\StringRes.h
# End Source File
# Begin Source File

SOURCE=.\StylesheetParamConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\SymbolButton.h
# End Source File
# Begin Source File

SOURCE=.\TabbedComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TabbedPropertyPageHost.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\Themed.h
# End Source File
# Begin Source File

SOURCE=.\TimeComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TimeEdit.h
# End Source File
# Begin Source File

SOURCE=.\TimeHelper.h
# End Source File
# Begin Source File

SOURCE=.\ToolbarHelper.h
# End Source File
# Begin Source File

SOURCE=.\ToolTipCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlHelper.h
# End Source File
# Begin Source File

SOURCE=.\TreeDragDropHelper.h
# End Source File
# Begin Source File

SOURCE=.\TreeListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TreeListSyncer.h
# End Source File
# Begin Source File

SOURCE=.\TreeSelectionHelper.h
# End Source File
# Begin Source File

SOURCE=.\Unzipper.h
# End Source File
# Begin Source File

SOURCE=.\UrlParser.h
# End Source File
# Begin Source File

SOURCE=.\UrlRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.h
# End Source File
# Begin Source File

SOURCE=.\WClassDefines.h
# End Source File
# Begin Source File

SOURCE=.\WebBrowserCtrl.h
# End Source File
# Begin Source File

SOURCE=.\WebMisc.h
# End Source File
# Begin Source File

SOURCE=.\WinClasses.h
# End Source File
# Begin Source File

SOURCE=.\WindowIcons.h
# End Source File
# Begin Source File

SOURCE=.\WinHelpButton.h
# End Source File
# Begin Source File

SOURCE=.\WinStyles.h
# End Source File
# Begin Source File

SOURCE=.\WndPrompt.h
# End Source File
# Begin Source File

SOURCE=.\WorkingWeek.h
# End Source File
# Begin Source File

SOURCE=.\XmlCharMap.h
# End Source File
# Begin Source File

SOURCE=.\XmlFile.h
# End Source File
# Begin Source File

SOURCE=.\XmlFileEx.h
# End Source File
# Begin Source File

SOURCE=.\XslFile.h
# End Source File
# Begin Source File

SOURCE=.\Zipper.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Shared.vcxproj
# End Source File
# Begin Source File

SOURCE=.\Shared.vcxproj.filters
# End Source File
# End Target
# End Project
