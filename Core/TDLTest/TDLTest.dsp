# Microsoft Developer Studio Project File - Name="TDLTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TDLTest - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TDLTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TDLTest.mak" CFG="TDLTest - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TDLTest - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "TDLTest - Win32 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/TDLTest", TXGAAAAA"
# PROP Scc_LocalPath "..\..\tdltest"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TDLTest - Win32 Unicode Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\ToDoList	mkdir ..\ToDoList\unicode_debug	copy unicode_debug\TDLTest.exe ..\todolist\unicode_debug /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TDLTest - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\ToDoList	mkdir ..\ToDoList\unicode_release	copy unicode_release\TDLTest.exe ..\todolist\unicode_release /y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TDLTest - Win32 Unicode Debug"
# Name "TDLTest - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DateHelperTest.cpp
# End Source File
# Begin Source File

SOURCE=.\FileMiscTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportExportMgrTest.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TaskFileTest.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCRECURRENCETest.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTest.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTest.rc
# End Source File
# Begin Source File

SOURCE=.\TDLTestBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeHelperTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlDataTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlTaskLinkTest.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DateHelperTest.h
# End Source File
# Begin Source File

SOURCE=.\FileMiscTest.h
# End Source File
# Begin Source File

SOURCE=.\ImportExportMgrTest.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TaskFileTest.h
# End Source File
# Begin Source File

SOURCE=.\TDCRECURRENCETest.h
# End Source File
# Begin Source File

SOURCE=.\TDLTest.h
# End Source File
# Begin Source File

SOURCE=.\TDLTestBase.h
# End Source File
# Begin Source File

SOURCE=.\TimeHelperTest.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlDataTest.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlTaskLinkTest.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\app_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\clock.ico
# End Source File
# Begin Source File

SOURCE=.\res\depend_link.ico
# End Source File
# Begin Source File

SOURCE=.\res\null.ico
# End Source File
# Begin Source File

SOURCE=.\res\plus.ico
# End Source File
# End Group
# Begin Group "ToDoList Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\Interfaces\ContentCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ContentMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ContentTypeCombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\FilteredToDoCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ImportExportComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ImportExportMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ImportExportUIHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\MultiTaskFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesExportPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesFile2Page.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesFilePage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesGenPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesMultiUserPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesShortcutsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesTaskCalcPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesTaskDef2Page.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesTaskDefPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesTaskPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesToolPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUICustomToolbarPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUIPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUITasklistColorsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUITasklistPage.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUIVisibilityPage.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\RichEditSpellCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\SpellCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TabbedToDoCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskClipboard.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskFile.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListCsvExporter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListCsvImporter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListExporterBase.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListHtmlExporter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListOutlookImporter.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\TasklistStorageMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListTdlExporter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListTdlImporter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListTxtExporter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCAnonymizeTasklist.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCCustomAttribData.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCCustomAttributeDef.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCCustomAttributeUIHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCDialogHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCEnumContainers.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCFindReplace.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCImageList.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCImportExportMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCMainMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCMergeTasklist.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCOutlookImportHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TdcRecurrence.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcreminder.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCSearchParamHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCSimpleTextContent.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCSourceControl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCStartupOptions.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCTaskTimeLog.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCTaskTimeLogAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCTimeTracking.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCToDoCtrlPreferenceHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCToolsCmdlineParser.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCToolsHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCWebUpdateScript.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLAddLoggedTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLAnalyseLoggedTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLAttribListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCleanupIniPreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCmdlineOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLColumnAttribVisibilityListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLColumnListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLColumnSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCommentsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLContentMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCsvImportExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCustomAttribFeatureComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCustomAttributeDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCustomToolbar.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFileEncodingComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterBar.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterDateComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterOptionComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdlfindresultslistctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdlfindtaskattributecombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFindTaskExpressionListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFindTasksDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLGoToTaskDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdliconcombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImpExpAttributeMappingListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImportDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImportOutlookDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImportOutlookObjectsDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImportOutlookObjectSetupListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLInfoTipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLKeyboardShortcutDisplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLLanguageComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLLanguageDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLMultiSortDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLOffsetDatesDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLOleMessageFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLPrefMigrationDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLPrintDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLPriorityComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLQuickFindComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLRecurringTaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLRegularityComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLReminderPeriodComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLReuseRecurringTaskDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLRiskComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLSendTasksDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLSetReminderDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLShowReminderDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLStylesheetParamConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskCtrlBase.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskIconDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTasklistImportDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTasklistSaveAsDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTasklistStorageMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskViewListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLThemeComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLThreadedExporterWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTimeTrackerDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLToolbarButtonListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLToolsUserInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTransformDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLViewTabControl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLWebUpdatePromptDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLWelcomeWizard.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlData.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlDataUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlFind.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlReminders.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlUndo.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoitem.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoListWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\UIExtensionHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\UIExtensionMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\Interfaces\UIThemeFile.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\TDLTest.vcxproj
# End Source File
# Begin Source File

SOURCE=.\TDLTest.vcxproj.filters
# End Source File
# End Target
# End Project
