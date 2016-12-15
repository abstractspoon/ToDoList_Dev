# Microsoft Developer Studio Project File - Name="ToDoList" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ToDoList - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ToDoList.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ToDoList.mak" CFG="ToDoList - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ToDoList - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "ToDoList - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ToDoList", OBDAAAAA"
# PROP Scc_LocalPath "..\..\todolist"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ToDoList - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ToDoList___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ToDoList___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "NO_DIAGRAM_TEMPLATE" /D "NO_XML_TEMPLATE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "_AFXDLL" /D "NO_DIAGRAM_TEMPLATE" /D "NO_XML_TEMPLATE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "PNG_SUPPORT" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Msimg32.lib msimg32.lib Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\ToDoList_Plugins	mkdir ..\..\ToDoList_Plugins\Debug	copy unicode_debug\ToDoList.exe ..\..\todolist_Plugins\debug /y	copy unicode_debug\ToDoList.pdb ..\..\todolist_Plugins\debug /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ToDoList - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ToDoList___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "ToDoList___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /Zi /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NO_DIAGRAM_TEMPLATE" /D "NO_XML_TEMPLATE" /D "USE_TRANSTEXT" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /Zi /O1 /D "USE_TRANSTEXT" /D "NDEBUG" /D "_AFXDLL" /D "NO_DIAGRAM_TEMPLATE" /D "NO_XML_TEMPLATE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "PNG_SUPPORT" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Msimg32.lib msimg32.lib Winmm.lib /nologo /subsystem:windows /pdb:"ToDoList.pdb" /map /machine:I386 /pdbtype:con /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"libc" /pdbtype:con /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none /map /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\ToDoList_Plugins	mkdir ..\..\ToDoList_Plugins\Release	copy unicode_release\ToDoList.exe ..\..\todolist_Plugins\release /y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ToDoList - Win32 Unicode Debug"
# Name "ToDoList - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FilteredToDoCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiTaskFile.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesExportPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesFile2Page.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesFilePage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesMultiUserPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesShortcutsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesTaskCalcPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesTaskDefPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesTaskPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesToolPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesUIPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesUITasklistColorsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesUITasklistPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesUIVisibilityPage.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabbedToDoCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskClipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListCsvExporter.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListCsvImporter.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListExporterBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListHtmlExporter.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListOutlookImporter.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListTdlExporter.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListTdlImporter.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListTxtExporter.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskTimeLog.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCAnonymizeTasklist.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCCustomAttributeHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCEnumContainers.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCImportExportMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCMergeToDoList.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCOutlookImportHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TdcRecurrence.cpp
# End Source File
# Begin Source File

SOURCE=.\tdcreminder.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCSearchParamHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCStartupOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCToDoCtrlPreferenceHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCToolsCmdlineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCToolsHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCUIExtensionHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCWebUpdateScript.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLAddLoggedTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLAnalyseLoggedTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLAttribListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLCmdlineOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLColumnAttribVisibilityListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLColumnListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLColumnSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLContentMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLContentTypeCombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLCsvImportExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLCustomAttribFeatureComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLCustomAttributeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLFileEncodingComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLFilterBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLFilterComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLFilterDateComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLFilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLFilterOptionComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\tdlfindresultslistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\tdlfindtaskattributecombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLFindTaskExpressionListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLFindTasksDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\tdliconcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLImpExpAttributeMappingListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLImportDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\tdlimportexportcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookObjectsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookObjectSetupListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLKeyboardShortcutDisplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLLanguageComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLLanguageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLMultiSortDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLOffsetDatesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLOleMessageFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLPasteFromClipboardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLPrefMigrationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLPrintDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLPriorityComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLQuickFindComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLRecurringTaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLReminderPeriodComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLReuseRecurringTaskDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLRiskComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLSendTasksDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLSetReminderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLShowReminderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTaskCtrlBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTaskIconDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTaskListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTasklistImportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTasklistStorageMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTaskTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTaskViewListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLThemeComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTimeTrackerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLToolsUserInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTransformDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLViewTabControl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLWebUpdatePromptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLWelcomeWizard.cpp
# End Source File
# Begin Source File

SOURCE=.\todocommentsctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlData.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlFind.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlReminders.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlUndo.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoitem.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoList.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoList.rc
# End Source File
# Begin Source File

SOURCE=.\ToDoListWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FilteredToDoCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MultiTaskFile.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesExportPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesFile2Page.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesFilePage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesGenPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesMultiUserPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesShortcutsPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesTaskCalcPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesTaskDefPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesTaskPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesToolPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesUIPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesUITasklistColorsPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesUITasklistPage.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesUIVisibilityPage.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabbedToDoCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TaskClipboard.h
# End Source File
# Begin Source File

SOURCE=.\TaskFile.h
# End Source File
# Begin Source File

SOURCE=.\TaskListCsvExporter.h
# End Source File
# Begin Source File

SOURCE=.\TaskListCsvImporter.h
# End Source File
# Begin Source File

SOURCE=.\TaskListDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\TaskListExporterBase.h
# End Source File
# Begin Source File

SOURCE=.\TaskListHtmlExporter.h
# End Source File
# Begin Source File

SOURCE=.\TaskListOutlookImporter.h
# End Source File
# Begin Source File

SOURCE=..\Shared\TaskListStorageMgr.h
# End Source File
# Begin Source File

SOURCE=.\TaskListTdlExporter.h
# End Source File
# Begin Source File

SOURCE=.\TaskListTdlImporter.h
# End Source File
# Begin Source File

SOURCE=.\TaskListTxtExporter.h
# End Source File
# Begin Source File

SOURCE=.\TaskSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\TaskTimeLog.h
# End Source File
# Begin Source File

SOURCE=.\TDCAnonymizeTasklist.h
# End Source File
# Begin Source File

SOURCE=.\TDCCustomAttributeHelper.h
# End Source File
# Begin Source File

SOURCE=.\tdcenum.h
# End Source File
# Begin Source File

SOURCE=.\TDCEnumContainers.h
# End Source File
# Begin Source File

SOURCE=.\TDCFilter.h
# End Source File
# Begin Source File

SOURCE=.\TDCImageList.h
# End Source File
# Begin Source File

SOURCE=.\TDCImportExportMgr.h
# End Source File
# Begin Source File

SOURCE=.\tdcmapping.h
# End Source File
# Begin Source File

SOURCE=.\TDCMergeTasklist.h
# End Source File
# Begin Source File

SOURCE=.\tdcmsg.h
# End Source File
# Begin Source File

SOURCE=.\TDCOutlookImportHelper.h
# End Source File
# Begin Source File

SOURCE=.\TdcRecurrence.h
# End Source File
# Begin Source File

SOURCE=.\tdcreminder.h
# End Source File
# Begin Source File

SOURCE=.\TDCSearchParamHelper.h
# End Source File
# Begin Source File

SOURCE=.\TDCStartupOptions.h
# End Source File
# Begin Source File

SOURCE=.\tdcstatic.h
# End Source File
# Begin Source File

SOURCE=.\tdcstruct.h
# End Source File
# Begin Source File

SOURCE=.\tdcswitch.h
# End Source File
# Begin Source File

SOURCE=.\TDCToDoCtrlPreferenceHelper.h
# End Source File
# Begin Source File

SOURCE=.\TDCToolsCmdlineParser.h
# End Source File
# Begin Source File

SOURCE=.\TDCToolsHelper.h
# End Source File
# Begin Source File

SOURCE=.\TDCUIExtensionHelper.h
# End Source File
# Begin Source File

SOURCE=.\TDCWebUpdateScript.h
# End Source File
# Begin Source File

SOURCE=.\TDLAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLAddLoggedTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLAnalyseLoggedTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLAttribListBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLCmdlineOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLColumnAttribVisibilityListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TDLColumnListBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLColumnSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLContentMgr.h
# End Source File
# Begin Source File

SOURCE=.\TDLContentTypeCombobox.h
# End Source File
# Begin Source File

SOURCE=.\TDLCsvImportExportDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLCustomAttribFeatureComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLCustomAttributeDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLDialog.h
# End Source File
# Begin Source File

SOURCE=.\TDLExportDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLFileEncodingComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLFilterBar.h
# End Source File
# Begin Source File

SOURCE=.\TDLFilterComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLFilterDateComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLFilterDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLFilterOptionComboBox.h
# End Source File
# Begin Source File

SOURCE=.\tdlfindresultslistctrl.h
# End Source File
# Begin Source File

SOURCE=.\tdlfindtaskattributecombobox.h
# End Source File
# Begin Source File

SOURCE=.\TDLFindTaskExpressionListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TDLFindTasksDlg.h
# End Source File
# Begin Source File

SOURCE=.\tdliconcombobox.h
# End Source File
# Begin Source File

SOURCE=.\TDLImpExpAttributeMappingListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TDLImportDialog.h
# End Source File
# Begin Source File

SOURCE=.\tdlimportexportcombobox.h
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookObjectsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookObjectSetupListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TDLKeyboardShortcutDisplayDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLLanguageComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLLanguageDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLMultiSortDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLOffsetDatesDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLOleMessageFilter.h
# End Source File
# Begin Source File

SOURCE=.\TDLPasteFromClipboardDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLPrefMigrationDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLPrintDialog.h
# End Source File
# Begin Source File

SOURCE=.\TDLPriorityComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLQuickFindComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLRecurringTaskEdit.h
# End Source File
# Begin Source File

SOURCE=.\TDLReminderPeriodComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLReuseRecurringTaskDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLRiskComboBox.h
# End Source File
# Begin Source File

SOURCE=.\tdlschemadef.h
# End Source File
# Begin Source File

SOURCE=.\TDLSendTasksDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLSetReminderDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLShowReminderDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLTaskCtrlBase.h
# End Source File
# Begin Source File

SOURCE=.\TDLTaskIconDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLTaskListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TDLTasklistImportDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLTasklistStorageMgr.h
# End Source File
# Begin Source File

SOURCE=.\TDLTaskTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TDLTaskViewListBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLThemeComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TDLTimeTrackerDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLToolsUserInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLTransformDialog.h
# End Source File
# Begin Source File

SOURCE=.\TDLViewTabControl.h
# End Source File
# Begin Source File

SOURCE=.\TDLWebUpdatePromptDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLWelcomeWizard.h
# End Source File
# Begin Source File

SOURCE=.\tdstringres.h
# End Source File
# Begin Source File

SOURCE=.\todocommentsctrl.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlData.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlFind.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlMgr.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlReminders.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlUndo.h
# End Source File
# Begin Source File

SOURCE=.\ToDoitem.h
# End Source File
# Begin Source File

SOURCE=.\ToDoList.h
# End Source File
# Begin Source File

SOURCE=.\ToDoListWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\abstractspoon.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_extra_metro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_extra_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_timetrack.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_toolbar_metro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\appointment.ico
# End Source File
# Begin Source File

SOURCE=.\res\clock.ico
# End Source File
# Begin Source File

SOURCE=.\res\column_header.bmp
# End Source File
# Begin Source File

SOURCE=.\res\contact.ico
# End Source File
# Begin Source File

SOURCE=.\res\custattrib_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\custattrib_toolbar_metro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\custattrib_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\custom_attrib_icons.ico
# End Source File
# Begin Source File

SOURCE=.\res\depend_link.ico
# End Source File
# Begin Source File

SOURCE=.\res\English.gif
# End Source File
# Begin Source File

SOURCE=.\res\fileedit_browse.ico
# End Source File
# Begin Source File

SOURCE=.\res\fileedit_go.ico
# End Source File
# Begin Source File

SOURCE=.\res\find_dialog_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\find_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\find_toolbar_metro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\find_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\flaggedmail.ico
# End Source File
# Begin Source File

SOURCE=.\res\folder.ico
# End Source File
# Begin Source File

SOURCE=.\res\help_button.ico
# End Source File
# Begin Source File

SOURCE=.\res\journal.ico
# End Source File
# Begin Source File

SOURCE=.\res\list_view_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\mail.ico
# End Source File
# Begin Source File

SOURCE=.\res\note.ico
# End Source File
# Begin Source File

SOURCE=.\res\null.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook_16.ico
# End Source File
# Begin Source File

SOURCE=.\res\prefs_dlg_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\reset.ico
# End Source File
# Begin Source File

SOURCE=.\res\shield.ico
# End Source File
# Begin Source File

SOURCE=.\res\simpletext.ico
# End Source File
# Begin Source File

SOURCE=.\res\social_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\social_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\src_control_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\style_para.ico
# End Source File
# Begin Source File

SOURCE=.\res\style_table.ico
# End Source File
# Begin Source File

SOURCE=.\res\style_wrap.ico
# End Source File
# Begin Source File

SOURCE=.\res\task.ico
# End Source File
# Begin Source File

SOURCE=.\res\task_icons_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\task_tree_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\timetrac.bmp
# End Source File
# Begin Source File

SOURCE=.\res\timetrack_btns.bmp
# End Source File
# Begin Source File

SOURCE=.\res\timetrack_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToDoList.rc2
# End Source File
# Begin Source File

SOURCE=.\res\update_filter.ico
# End Source File
# Begin Source File

SOURCE=.\res\wiz_header.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\XPcheckboxes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\YourLanguage.GIF
# End Source File
# End Group
# Begin Group "Binaries"

# PROP Default_Filter ""
# Begin Group "Unicode_Debug"

# PROP Default_Filter "exe;dll"
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Rtf.Converter.Html.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Rtf.Interpreter.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Rtf.Parser.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Solutions.Community.Rtf2Html.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Sys.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Rtf2HtmlBridge.dll
# End Source File
# End Group
# Begin Group "Unicode_Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Rtf.Converter.Html.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Rtf.Interpreter.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Rtf.Parser.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Solutions.Community.Rtf2Html.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Sys.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Rtf2HtmlBridge.dll
# End Source File
# End Group
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\Interfaces\IContentControl.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\IEncryption.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\IFileStorage.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\IImportExport.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\IPreferences.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ISpellCheck.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ITaskList.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ITaskListStorage.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\ITransText.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\IUIExtension.h
# End Source File
# Begin Source File

SOURCE=..\Interfaces\UITHEME.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\RES\todolist.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\ToDoList.vcxproj
# End Source File
# Begin Source File

SOURCE=.\ToDoList.vcxproj.filters
# End Source File
# Begin Source File

SOURCE=.\ToDoList_All.sln
# End Source File
# End Target
# End Project
# Section ToDoList : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowserCtrl
# 	2:10:HeaderFile:webbrowserctrl.h
# 	2:8:ImplFile:webbrowserctrl.cpp
# End Section
# Section ToDoList : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowserctrl.h
# 	2:16:DefaultSinkClass:CWebBrowserCtrl
# End Section
