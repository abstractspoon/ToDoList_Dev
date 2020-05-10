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
# ADD LIB32 /nologo /out:"Unicode_Debug\ToDoList.lib"

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
# ADD LIB32 /nologo /out:"Unicode_Release\ToDoList.lib"

!ENDIF 

# Begin Target

# Name "ToDoListLib - Win32 Unicode Debug"
# Name "ToDoListLib - Win32 Unicode Release"
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

SOURCE=.\PreferencesTaskDef2Page.cpp
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

SOURCE=.\PreferencesUICustomToolbarPage.cpp
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

SOURCE=.\TDCAnonymizeTasklist.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCCustomAttribData.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCCustomAttributeDef.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCCustomAttributeUIHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCDialogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCEnumContainers.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCFindReplace.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCImportExportMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCMainMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCMergeTasklist.cpp
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

SOURCE=.\TDCSimpleTextContent.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCSourceControl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCStartupOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCTaskTimeLog.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCTaskTimeLogAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\TDCTimeTracking.cpp
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

SOURCE=.\TDLCleanupIniPreferencesDlg.cpp
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

SOURCE=.\TDLCommentsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLContentMgr.cpp
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

SOURCE=.\TDLCustomToolbar.cpp
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

SOURCE=.\TDLGoToTaskDlg.cpp
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

SOURCE=.\TDLImportOutlookDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookObjectsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookObjectSetupListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLInfoTipCtrl.cpp
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

SOURCE=.\TDLRegularityComboBox.cpp
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

SOURCE=.\TDLStylesheetParamConfigDlg.cpp
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

SOURCE=.\TDLTasklistSaveAsDlg.cpp
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

SOURCE=.\TDLThreadedExporterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLTimeTrackerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLToolbarButtonListCtrl.cpp
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

SOURCE=.\ToDoCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlData.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlDataUtils.cpp
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

SOURCE=.\PreferencesTaskDef2Page.h
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

SOURCE=.\PreferencesUICustomToolbarPage.h
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

SOURCE=.\TDCAnonymizeTasklist.h
# End Source File
# Begin Source File

SOURCE=.\tdccustomattribdata.h
# End Source File
# Begin Source File

SOURCE=.\TDCCustomAttributeDef.h
# End Source File
# Begin Source File

SOURCE=.\TDCCustomAttributeUIHelper.h
# End Source File
# Begin Source File

SOURCE=.\TDCDialogHelper.h
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

SOURCE=.\TDCFindReplace.h
# End Source File
# Begin Source File

SOURCE=.\TDCImageList.h
# End Source File
# Begin Source File

SOURCE=.\TDCImportExportMgr.h
# End Source File
# Begin Source File

SOURCE=.\TDCMainMenu.h
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

SOURCE=.\TDCSimpleTextContent.h
# End Source File
# Begin Source File

SOURCE=.\tdcsort.h
# End Source File
# Begin Source File

SOURCE=.\TDCSourceControl.h
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

SOURCE=.\TDCTaskTimeLog.h
# End Source File
# Begin Source File

SOURCE=.\TDCTaskTimeLogAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\TDCTimeTracking.h
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

SOURCE=.\TDLCleanupIniPreferencesDlg.h
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

SOURCE=.\TDLCommentsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TDLContentMgr.h
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

SOURCE=.\TDLCustomToolbar.h
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

SOURCE=.\TDLGoToTaskDlg.h
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

SOURCE=.\TDLImportOutlookDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookObjectsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLImportOutlookObjectSetupListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TDLInfoTipCtrl.h
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

SOURCE=.\TDLRegularityComboBox.h
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

SOURCE=.\TDLSendTasksDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLSetReminderDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLShowReminderDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLStylesheetParamConfigDlg.h
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

SOURCE=.\TDLTasklistSaveAsDlg.h
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

SOURCE=.\TDLThreadedExporterWnd.h
# End Source File
# Begin Source File

SOURCE=.\TDLTimeTrackerDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDLToolbarButtonListCtrl.h
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

SOURCE=.\ToDoCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlData.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlDataDefines.h
# End Source File
# Begin Source File

SOURCE=.\ToDoCtrlDataUtils.h
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

SOURCE=.\ToDoListWnd.h
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
