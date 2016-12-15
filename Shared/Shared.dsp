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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /YX"stdafx.h" /FD /GZ /c
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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /YX"stdafx.h" /FD /c
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

SOURCE=.\autocombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BinaryData.cpp
# End Source File
# Begin Source File

SOURCE=.\browserdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\checkcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\checklistboxex.cpp
# End Source File
# Begin Source File

SOURCE=.\Clipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\colorbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\colorcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorDef.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\custombutton.cpp
# End Source File
# Begin Source File

SOURCE=.\DatabaseEx.cpp
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

SOURCE=.\deferWndMove.cpp
# End Source File
# Begin Source File

SOURCE=.\delayredraw.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUnits.cpp
# End Source File
# Begin Source File

SOURCE=.\DockManager.cpp
# End Source File
# Begin Source File

SOURCE=.\driveinfo.cpp
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

SOURCE=.\enbrowserctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EnCheckComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\encolordialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EnCommandLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\endatetimectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\enedit.cpp
# End Source File
# Begin Source File

SOURCE=.\enfiledialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EnHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\enlistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EnMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\EnRecentFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\enstatic.cpp
# End Source File
# Begin Source File

SOURCE=.\EnString.cpp
# End Source File
# Begin Source File

SOURCE=.\EnTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\entoolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\FileComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\fileedit.cpp
# End Source File
# Begin Source File

SOURCE=.\FileMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\FileRegister.cpp
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

SOURCE=.\framewndcontainer.cpp
# End Source File
# Begin Source File

SOURCE=.\GdiPlus.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicsMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupLine.cpp
# End Source File
# Begin Source File

SOURCE=.\historycombobox.cpp
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

SOURCE=.\iconbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\IconCache.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessors.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportExportMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportExportUIHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\InputListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LightBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Localizer.cpp
# End Source File
# Begin Source File

SOURCE=.\MASKEDIT.CPP
# End Source File
# Begin Source File

SOURCE=.\menubutton.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuIconMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc.cpp
# End Source File
# Begin Source File

SOURCE=.\MonthCalCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\monthcombobox.cpp
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

SOURCE=.\MultiSelTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NcGutter.cpp
# End Source File
# Begin Source File

SOURCE=.\odbcfieldcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\OdbcFields.cpp
# End Source File
# Begin Source File

SOURCE=.\odbctablecombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\OdbcTables.cpp
# End Source File
# Begin Source File

SOURCE=.\odbcvaluecombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderedTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OSVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\ownerdrawcomboboxbase.cpp
# End Source File
# Begin Source File

SOURCE=.\passworddialog.cpp
# End Source File
# Begin Source File

SOURCE=.\popupEditctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\popupListboxctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
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

SOURCE=.\RCCtrlParser.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordsetEx.cpp
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

SOURCE=.\richeditbasectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditNcBorder.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditSpellCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundCorner.cpp
# End Source File
# Begin Source File

SOURCE=.\Rtf2HtmlConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\RTLStyleMgr.cpp
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

SOURCE=.\SpellCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBarProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StickiesAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\StickiesWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=.\symbolbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\SysImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\tabbedcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\TabbedPropertyPageHost.cpp
# End Source File
# Begin Source File

SOURCE=.\tabctrlex.cpp
# End Source File
# Begin Source File

SOURCE=.\TasklistStorageMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Themed.cpp
# End Source File
# Begin Source File

SOURCE=.\timecombobox.cpp
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

SOURCE=.\TRAYICON.CPP
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeDragDropHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListSyncer.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeSelectionHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\UIExtensionMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\UIExtensionUIHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\UIExtensionWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UIThemeFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Unzipper.cpp
# End Source File
# Begin Source File

SOURCE=.\urlricheditctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowserctrl.cpp
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

SOURCE=.\winhelpbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\WndMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\WndPrompt.cpp
# End Source File
# Begin Source File

SOURCE=.\XmlFile.cpp
# End Source File
# Begin Source File

SOURCE=.\xmlfileex.cpp
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

SOURCE=.\autocombobox.h
# End Source File
# Begin Source File

SOURCE=.\AutoFlag.h
# End Source File
# Begin Source File

SOURCE=.\BaseTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BinaryData.h
# End Source File
# Begin Source File

SOURCE=.\browserdlg.h
# End Source File
# Begin Source File

SOURCE=.\checkcombobox.h
# End Source File
# Begin Source File

SOURCE=.\checklistboxex.h
# End Source File
# Begin Source File

SOURCE=.\Clipboard.h
# End Source File
# Begin Source File

SOURCE=.\colorbutton.h
# End Source File
# Begin Source File

SOURCE=.\colorcombobox.h
# End Source File
# Begin Source File

SOURCE=.\ColorDef.h
# End Source File
# Begin Source File

SOURCE=.\ContentCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ContentMgr.h
# End Source File
# Begin Source File

SOURCE=.\custombutton.h
# End Source File
# Begin Source File

SOURCE=.\DatabaseEx.h
# End Source File
# Begin Source File

SOURCE=.\DateHelper.h
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

SOURCE=.\deferWndMove.h
# End Source File
# Begin Source File

SOURCE=.\delayredraw.h
# End Source File
# Begin Source File

SOURCE=.\DialogHelper.h
# End Source File
# Begin Source File

SOURCE=.\DlgUnits.h
# End Source File
# Begin Source File

SOURCE=.\DockManager.h
# End Source File
# Begin Source File

SOURCE=.\driveinfo.h
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

SOURCE=.\enbrowserctrl.h
# End Source File
# Begin Source File

SOURCE=.\EnCheckComboBox.h
# End Source File
# Begin Source File

SOURCE=.\encolordialog.h
# End Source File
# Begin Source File

SOURCE=.\EnCommandLineInfo.h
# End Source File
# Begin Source File

SOURCE=.\endatetimectrl.h
# End Source File
# Begin Source File

SOURCE=.\enedit.h
# End Source File
# Begin Source File

SOURCE=.\enfiledialog.h
# End Source File
# Begin Source File

SOURCE=.\EnHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\enlistctrl.h
# End Source File
# Begin Source File

SOURCE=.\EnMenu.h
# End Source File
# Begin Source File

SOURCE=.\EnRecentFileList.h
# End Source File
# Begin Source File

SOURCE=.\enstatic.h
# End Source File
# Begin Source File

SOURCE=.\EnString.h
# End Source File
# Begin Source File

SOURCE=.\EnTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\entoolbar.h
# End Source File
# Begin Source File

SOURCE=.\FileComboBox.h
# End Source File
# Begin Source File

SOURCE=.\fileedit.h
# End Source File
# Begin Source File

SOURCE=.\FILEMISC.H
# End Source File
# Begin Source File

SOURCE=.\FileRegister.h
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

SOURCE=.\framewndcontainer.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlus.h
# End Source File
# Begin Source File

SOURCE=.\GraphicsMisc.h
# End Source File
# Begin Source File

SOURCE=.\GroupLine.h
# End Source File
# Begin Source File

SOURCE=.\historycombobox.h
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

SOURCE=.\hotkeyctrlex.h
# End Source File
# Begin Source File

SOURCE=.\HotTracker.h
# End Source File
# Begin Source File

SOURCE=.\HtmlCharMap.h
# End Source File
# Begin Source File

SOURCE=.\iconbutton.h
# End Source File
# Begin Source File

SOURCE=.\IconCache.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessors.h
# End Source File
# Begin Source File

SOURCE=.\ImportExportMgr.h
# End Source File
# Begin Source File

SOURCE=.\ImportExportUIHelper.h
# End Source File
# Begin Source File

SOURCE=.\InputListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LightBox.h
# End Source File
# Begin Source File

SOURCE=.\LimitSingleInstance.h
# End Source File
# Begin Source File

SOURCE=.\Localizer.h
# End Source File
# Begin Source File

SOURCE=.\mapex.h
# End Source File
# Begin Source File

SOURCE=.\MASKEDIT.H
# End Source File
# Begin Source File

SOURCE=.\menubutton.h
# End Source File
# Begin Source File

SOURCE=.\MenuIconMgr.h
# End Source File
# Begin Source File

SOURCE=.\Misc.h
# End Source File
# Begin Source File

SOURCE=.\MonthCalCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\monthcombobox.h
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

SOURCE=.\MultiSelTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NcGutter.h
# End Source File
# Begin Source File

SOURCE=.\odbcfieldcombobox.h
# End Source File
# Begin Source File

SOURCE=.\OdbcFields.h
# End Source File
# Begin Source File

SOURCE=.\odbctablecombobox.h
# End Source File
# Begin Source File

SOURCE=.\OdbcTables.h
# End Source File
# Begin Source File

SOURCE=.\odbcvaluecombobox.h
# End Source File
# Begin Source File

SOURCE=.\OrderedTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OSVersion.h
# End Source File
# Begin Source File

SOURCE=.\ownerdrawcomboboxbase.h
# End Source File
# Begin Source File

SOURCE=.\passworddialog.h
# End Source File
# Begin Source File

SOURCE=.\popupEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\popupListboxctrl.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
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

SOURCE=.\RCCtrlParser.h
# End Source File
# Begin Source File

SOURCE=.\RecordsetEx.h
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

SOURCE=.\richeditbasectrl.h
# End Source File
# Begin Source File

SOURCE=.\RichEditHelper.h
# End Source File
# Begin Source File

SOURCE=.\RichEditNcBorder.h
# End Source File
# Begin Source File

SOURCE=.\RichEditSpellCheck.h
# End Source File
# Begin Source File

SOURCE=.\RoundCorner.h
# End Source File
# Begin Source File

SOURCE=.\Rtf2HtmlConverter.h
# End Source File
# Begin Source File

SOURCE=.\RTLStyleMgr.h
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

SOURCE=.\SpellCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpellChecker.h
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

SOURCE=.\stringres.h
# End Source File
# Begin Source File

SOURCE=.\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\symbolbutton.h
# End Source File
# Begin Source File

SOURCE=.\SysColors.h
# End Source File
# Begin Source File

SOURCE=.\SysImageList.h
# End Source File
# Begin Source File

SOURCE=.\tabbedcombobox.h
# End Source File
# Begin Source File

SOURCE=.\TabbedPropertyPageHost.h
# End Source File
# Begin Source File

SOURCE=.\tabctrlex.h
# End Source File
# Begin Source File

SOURCE=.\TaskListStorageMgr.h
# End Source File
# Begin Source File

SOURCE=.\Themed.h
# End Source File
# Begin Source File

SOURCE=.\timecombobox.h
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

SOURCE=.\TRAYICON.H
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlHelper.h
# End Source File
# Begin Source File

SOURCE=.\TreeDragDropHelper.h
# End Source File
# Begin Source File

SOURCE=.\TreeListSyncer.h
# End Source File
# Begin Source File

SOURCE=.\TreeSelectionHelper.h
# End Source File
# Begin Source File

SOURCE=.\UIExtensionMgr.h
# End Source File
# Begin Source File

SOURCE=.\UIExtensionUIHelper.h
# End Source File
# Begin Source File

SOURCE=.\UIExtensionWnd.h
# End Source File
# Begin Source File

SOURCE=.\UIThemeFile.h
# End Source File
# Begin Source File

SOURCE=.\Unzipper.h
# End Source File
# Begin Source File

SOURCE=.\urlricheditctrl.h
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.h
# End Source File
# Begin Source File

SOURCE=.\wclassdefines.h
# End Source File
# Begin Source File

SOURCE=.\webbrowserctrl.h
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

SOURCE=.\winstyles.h
# End Source File
# Begin Source File

SOURCE=.\WndMisc.h
# End Source File
# Begin Source File

SOURCE=.\WndPrompt.h
# End Source File
# Begin Source File

SOURCE=.\XmlCharMap.h
# End Source File
# Begin Source File

SOURCE=.\XmlFile.h
# End Source File
# Begin Source File

SOURCE=.\xmlfileex.h
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
