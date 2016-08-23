; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=COdbcDatabaseSelectionDlg
LastTemplate=CComboBox
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "odbcstorage.h"
LastPage=0

ClassCount=6
Class1=COdbcDatabaseSetupDlg
Class2=COdbcDatabaseSetupListCtrl

ResourceCount=2
Resource1=IDD_DATABASESELECTION_DIALOG
Class3=COdbcDatabaseSelectionDlg
Class4=COdbcDatabaseSelectionListCtrl
Class5=COdbcTaskRecordset
Class6=COdbcValueComboBox
Resource2=IDD_DATABASESETUP_DIALOG

[CLS:COdbcDatabaseSetupDlg]
Type=0
BaseClass=CDialog
HeaderFile=OdbcDatabaseSetupDlg.h
ImplementationFile=OdbcDatabaseSetupDlg.cpp
LastObject=IDC_TASKS_TASKLISTKEYFIELD
Filter=D
VirtualFilter=dWC

[CLS:COdbcDatabaseSetupListCtrl]
Type=0
BaseClass=CInputListCtrl
HeaderFile=OdbcDatabaseSetupListCtrl.h
ImplementationFile=OdbcDatabaseSetupListCtrl.cpp

[DLG:IDD_SETUP_DIALOG]
Type=1
Class=COdbcDatabaseSetupDlg

[DLG:IDD_DATABASESELECTION_DIALOG]
Type=1
Class=COdbcDatabaseSelectionDlg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_DATABASE_LIST,SysListView32,1350632477
Control4=IDC_STATIC,static,1342308864
Control5=IDC_NEW_DATABASE,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_TASKLISTS,combobox,1344340227
Control10=IDC_ALLOCTOLIST,combobox,1344340227
Control11=IDC_DELETE_DATABASE,button,1342242816

[CLS:COdbcDatabaseSelectionDlg]
Type=0
HeaderFile=OdbcDatabaseSelectionDlg.h
ImplementationFile=OdbcDatabaseSelectionDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_TASKS_KEYFIELD

[CLS:COdbcDatabaseSelectionListCtrl]
Type=0
HeaderFile=OdbcDatabaseSelectionListCtrl.h
ImplementationFile=OdbcDatabaseSelectionListCtrl.cpp
BaseClass=CInputListCtrl
Filter=W
VirtualFilter=FWC

[CLS:COdbcTaskRecordset]
Type=0
HeaderFile=OdbcTaskRecordset.h
ImplementationFile=OdbcTaskRecordset.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:COdbcTaskRecordset]
DB=1
DBType=ODBC
ColumnCount=0

[CLS:COdbcValueComboBox]
Type=0
HeaderFile=..\shared\odbcvaluecombobox.h
ImplementationFile=..\shared\odbcvaluecombobox.cpp
BaseClass=CComboBox
Filter=W

[DLG:IDD_DATABASESETUP_DIALOG]
Type=1
Class=COdbcDatabaseSelectionDlg
ControlCount=28
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TASKATTRIBUTESETUP,SysListView32,1350665245
Control4=IDC_VALIDATE,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_TASKS_TABLENAME,combobox,1344340227
Control9=IDC_STATIC,static,1342308352
Control10=IDC_TASKS_KEYFIELD,combobox,1344340227
Control11=IDC_STATIC,static,1342308352
Control12=IDC_TASKS_PARENTKEYFIELD,combobox,1344340227
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_TASKLISTS_TABLENAME,combobox,1344340227
Control16=IDC_STATIC,static,1342308352
Control17=IDC_TASKLISTS_KEYFIELD,combobox,1344340227
Control18=IDC_STATIC,static,1342308352
Control19=IDC_TASKLISTS_NAMEFIELD,combobox,1344340227
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308352
Control22=IDC_ALLOCTO_TABLENAME,combobox,1344340227
Control23=IDC_STATIC,static,1342308352
Control24=IDC_ALLOCTO_KEYFIELD,combobox,1344340227
Control25=IDC_STATIC,static,1342308352
Control26=IDC_ALLOCTO_NAMEFIELD,combobox,1344340227
Control27=IDC_STATIC,static,1342308352
Control28=IDC_TASKS_TASKLISTKEYFIELD,combobox,1344340227

