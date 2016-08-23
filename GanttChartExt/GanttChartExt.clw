; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGanttChartWnd
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ganttchartext.h"
LastPage=0

ClassCount=4
Class1=CGanttChartExtWnd
Class2=CGanttChartWnd
Class3=CGanttPreferencesPage

ResourceCount=5
Resource1=IDR_TOOLBAR
Resource2=IDD_CREATEDEPENDS_DIALOG
Resource3=IDD_PREFERENCES_DIALOG
Resource4=IDD_PREFERENCES_PAGE
Class4=CGanttCreateDependsDlg
Resource5=IDD_GANTTTREE_DIALOG (English (U.S.))

[CLS:CGanttChartExtWnd]
Type=0
BaseClass=CDialog
HeaderFile=GanttChartExtWnd.h
ImplementationFile=GanttChartExtWnd.cpp
LastObject=IDC_DISPLAYALLOCTO

[CLS:CGanttChartWnd]
Type=0
BaseClass=CDialog
HeaderFile=GanttChartWnd.h
ImplementationFile=GanttChartWnd.cpp
Filter=W
VirtualFilter=dWC
LastObject=ID_GANTT_DELETEDEPENDS

[CLS:CGanttPreferencesPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=GanttPreferencesDlg.h
ImplementationFile=GanttPreferencesDlg.cpp
Filter=W
VirtualFilter=idWC
LastObject=CGanttPreferencesPage

[DLG:IDD_GANTTTREE_DIALOG (English (U.S.))]
Type=1
Class=CGanttChartWnd
ControlCount=10
Control1=IDC_SELECTEDTASKDATES,static,1342312961
Control2=IDC_DISPLAY_LABEL,static,1342308352
Control3=IDC_DISPLAY,combobox,1344340499
Control4=IDC_TREEHEADER,SysHeader32,1342177280
Control5=IDC_GANTTTREE,SysTreeView32,1342259263
Control6=IDC_GANTTLIST,SysListView32,1342275597
Control7=IDC_SNAPMODES,combobox,1344340227
Control8=IDC_SNAPMODES_LABEL,static,1342308352
Control9=IDC_SELECTEDTASKDATES_LABEL,static,1342308352
Control10=IDC_TB_PLACEHOLDER,static,1073741831

[DLG:IDD_PREFERENCES_DIALOG]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PPHOST,static,1342177288

[DLG:IDD_PREFERENCES_PAGE]
Type=1
Class=CGanttPreferencesPage
ControlCount=21
Control1=IDC_DISPLAYALLOCTO,button,1342242819
Control2=IDC_DISPLAYPROGRESS,button,1342242819
Control3=IDC_AUTOSCROLLSELECTION,button,1342242819
Control4=IDC_USETAGFORMILESTONE,button,1342242819
Control5=IDC_MILESTONETAG,edit,1350631552
Control6=IDC_COLORSGROUP,static,1342308352
Control7=IDC_WEEKENDCOLOR,button,1342242819
Control8=IDC_SETWEEKENDCOLOR,button,1342243328
Control9=IDC_TODAYCOLOR,button,1342242819
Control10=IDC_SETTODAYCOLOR,button,1342243328
Control11=IDC_DEFAULTCOLOR,button,1342242819
Control12=IDC_SETDEFAULTCOLOR,button,1342243328
Control13=65535,button,1342177287
Control14=IDC_DEFAULTPARENTCOLORS,button,1342308361
Control15=IDC_NOPARENTCOLOR,button,1342177289
Control16=IDC_SPECIFYPARENTCOLOR,button,1342177289
Control17=IDC_SETPARENTCOLOR,button,1342374400
Control18=IDC_DATESGROUP,static,1342308352
Control19=IDC_CALCULATEPARENTDATES,button,1342242819
Control20=IDC_CALCULATEMISSINGSTARTDATE,button,1342242819
Control21=IDC_CALCULATEMISSINGDUEDATE,button,1342242819

[TB:IDR_TOOLBAR]
Type=1
Class=CGanttChartWnd
Command1=ID_GANTT_GOTOTODAY
Command2=ID_GANTT_PREFS
Command3=ID_GANTT_NEWDEPENDS
Command4=ID_GANTT_EDITDEPENDS
Command5=ID_GANTT_DELETEDEPENDS
CommandCount=5

[DLG:IDD_CREATEDEPENDS_DIALOG]
Type=1
Class=CGanttCreateDependsDlg
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,button,1342177287
Control3=IDC_CURRENTSTAGE,static,1342308352

[CLS:CGanttCreateDependsDlg]
Type=0
HeaderFile=GanttCreateDependsDlg.h
ImplementationFile=GanttCreateDependsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDCANCEL
VirtualFilter=dWC

