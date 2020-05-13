# Microsoft Developer Studio Project File - Name="ToDoListLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ToDoListLib - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ToDoListLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ToDoListLib.mak" CFG="ToDoListLib - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ToDoListLib - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ToDoListLib - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ToDoListLib - Win32 Unicode Debug"

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

!ELSEIF  "$(CFG)" == "ToDoListLib - Win32 Unicode Release"

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

# Name "ToDoListLib - Win32 Unicode Debug"
# Name "ToDoListLib - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\ToDoList\FilteredToDoCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToDoList\MultiTaskFile.cpp
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

SOURCE=..\ToDoList\TDCRecurrence.cpp
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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\ToDoList\FilteredToDoCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\MultiTaskFile.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesExportPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesFile2Page.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesFilePage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesGenPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesMultiUserPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesShortcutsPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesTaskCalcPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesTaskDef2Page.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesTaskDefPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesTaskPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesToolPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUICustomToolbarPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUIPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUITasklistColorsPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUITasklistPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\PreferencesUIVisibilityPage.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TabbedToDoCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskClipboard.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskFile.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListCsvExporter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListCsvImporter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListDropTarget.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListExporterBase.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListHtmlExporter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListOutlookImporter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListTdlExporter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListTdlImporter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskListTxtExporter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TaskSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCAnonymizeTasklist.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdccustomattribdata.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCCustomAttributeDef.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCCustomAttributeUIHelper.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCDialogHelper.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcenum.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCEnumContainers.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCFilter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCFindReplace.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCImageList.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCImportExportMgr.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCMainMenu.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcmapping.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCMergeTasklist.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcmsg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCOutlookImportHelper.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCRecurrence.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcreminder.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCSearchParamHelper.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCSimpleTextContent.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcsort.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCSourceControl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCStartupOptions.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcstatic.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcstruct.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdcswitch.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCTaskTimeLog.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCTaskTimeLogAnalysis.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCTimeTracking.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCToDoCtrlPreferenceHelper.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCToolsCmdlineParser.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCToolsHelper.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDCWebUpdateScript.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLAboutDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLAddLoggedTimeDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLAnalyseLoggedTimeDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLAttribListBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCleanupIniPreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCmdlineOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLColumnAttribVisibilityListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLColumnListBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLColumnSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCommentsCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLContentMgr.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCsvImportExportDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCustomAttribFeatureComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCustomAttributeDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLCustomToolbar.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLDialog.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLExportDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFileEncodingComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterBar.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterDateComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFilterOptionComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdlfindresultslistctrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdlfindtaskattributecombobox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFindTaskExpressionListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLFindTasksDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLGoToTaskDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdliconcombobox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImpExpAttributeMappingListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImportDialog.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImportOutlookDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImportOutlookObjectsDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLImportOutlookObjectSetupListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLInfoTipCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLKeyboardShortcutDisplayDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLLanguageComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLLanguageDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLMultiSortDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLOffsetDatesDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLOleMessageFilter.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLPrefMigrationDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLPrintDialog.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLPriorityComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLQuickFindComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLRecurringTaskEdit.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLRegularityComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLReminderPeriodComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLReuseRecurringTaskDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLRiskComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLSendTasksDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLSetReminderDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLShowReminderDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLStylesheetParamConfigDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskCtrlBase.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskIconDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTasklistImportDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTasklistSaveAsDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTasklistStorageMgr.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTaskViewListBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLThemeComboBox.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLThreadedExporterWnd.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTimeTrackerDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLToolbarButtonListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLToolsUserInputDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLTransformDialog.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLViewTabControl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLWebUpdatePromptDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\TDLWelcomeWizard.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\tdstringres.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlData.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlDataDefines.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlDataUtils.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlFind.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlMgr.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlReminders.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoCtrlUndo.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoitem.h
# End Source File
# Begin Source File

SOURCE=..\ToDoList\ToDoListWnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ToDoListLib.vcxproj
# End Source File
# Begin Source File

SOURCE=.\ToDoListLib.vcxproj.filters
# End Source File
# End Target
# End Project
