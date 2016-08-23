; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CKanbanWnd
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "kanbanboard.h"
LastPage=0

ClassCount=4
Class1=CKanbanPreferencesPage
Class2=CKanbanWnd

ResourceCount=5
Resource1=IDR_TOOLBAR
Resource2=IDD_PREFERENCES_PAGE
Resource3=IDD_PREFERENCES_DIALOG
Resource4=IDD_KANBAN_DIALOG (English (U.S.))
Class3=CKanbanAttributeComboBox
Class4=CKanbanFixedColumnListCtrl
Resource5=IDR_COLUMN_TOOLBAR

[CLS:CKanbanPreferencesPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=KanbanPreferencesDlg.h
ImplementationFile=KanbanPreferencesDlg.cpp
Filter=D
VirtualFilter=idWC
LastObject=IDC_HIDEEMPTYCOLS

[CLS:CKanbanWnd]
Type=0
BaseClass=CDialog
HeaderFile=KanbanWnd.h
ImplementationFile=KanbanWnd.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_OPTIONS

[DLG:IDD_KANBAN_DIALOG (English (U.S.))]
Type=1
Class=CKanbanWnd
ControlCount=6
Control1=IDC_ATTRIBUTES_LABEL,static,1342308352
Control2=IDC_ATTRIBUTES,combobox,1344340227
Control3=IDC_CUSTOMATTRIB,combobox,1344340227
Control4=IDC_OPTIONS_LABEL,static,1342308352
Control5=IDC_OPTIONS,combobox,1344340755
Control6=IDC_TB_PLACEHOLDER,static,1073741831

[DLG:IDD_PREFERENCES_DIALOG]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PPHOST,static,1342177288

[DLG:IDD_PREFERENCES_PAGE]
Type=1
Class=CKanbanPreferencesPage
ControlCount=13
Control1=IDC_SORTSUBTASKSBELOWPARENT,button,1342242819
Control2=IDC_SHOWTASKCOLORASBAR,button,1342242819
Control3=IDC_COLORBARBYPRIORITY,button,1342242819
Control4=IDC_DISPLAYATTRIBUTES,listbox,1351680851
Control5=IDC_COLUMNGROUP,static,1342308352
Control6=IDC_ALWAYSSHOWBACKLOG,button,1342373891
Control7=IDC_FIXEDATTRIBLABEL,static,1342308352
Control8=IDC_ATTRIBUTES,combobox,1344340227
Control9=IDC_CUSTOMATTRIBID,combobox,1344340227
Control10=IDC_COLUMNDEFS,SysListView32,1350665229
Control11=IDC_TB_PLACEHOLDER,static,1073741831
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287

[TB:IDR_TOOLBAR]
Type=1
Class=CKanbanPreferencesPage
Command1=ID_PREFS
CommandCount=1

[TB:IDR_COLUMN_TOOLBAR]
Type=1
Class=?
Command1=ID_MOVECOL_UP
Command2=ID_MOVECOL_DOWN
Command3=ID_POPULATECOLUMNS
CommandCount=3

[CLS:CKanbanAttributeComboBox]
Type=0
HeaderFile=KanbanAttributeComboBox.h
ImplementationFile=KanbanAttributeComboBox.cpp
BaseClass=CComboBox
Filter=W
VirtualFilter=cWC
LastObject=CKanbanAttributeComboBox

[CLS:CKanbanFixedColumnListCtrl]
Type=0
HeaderFile=KanbanFixedColumnListCtrl.h
ImplementationFile=KanbanFixedColumnListCtrl.cpp
BaseClass=CInputListCtrl
Filter=W
VirtualFilter=FWC

