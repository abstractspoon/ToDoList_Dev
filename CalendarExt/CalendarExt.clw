; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDayViewListCtrl
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "calendarext.h"
LastPage=0

ClassCount=5
Class1=CCalendarPreferencesPage
Class2=CCalendarPreferencesDlg
Class3=CCalendarWnd
Class4=CTaskCalendarCtrl

ResourceCount=4
Resource1=IDD_PREFERENCES_PAGE
Resource2=IDD_PREFERENCES_DIALOG
Class5=CDayViewListCtrl
Resource3=IDR_TOOLBAR
Resource4=IDD_CALENDAR_DIALOG

[CLS:CCalendarPreferencesPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=CalendarPreferencesDlg.h
ImplementationFile=CalendarPreferencesDlg.cpp

[CLS:CCalendarPreferencesDlg]
Type=0
BaseClass=CDialog
HeaderFile=CalendarPreferencesDlg.h
ImplementationFile=CalendarPreferencesDlg.cpp

[CLS:CCalendarWnd]
Type=0
BaseClass=CDialog
HeaderFile=CalendarWnd.h
ImplementationFile=CalendarWnd.cpp

[CLS:CTaskCalendarCtrl]
Type=0
BaseClass=CCalendarCtrl
HeaderFile=TaskCalendarCtrl.h
ImplementationFile=TaskCalendarCtrl.cpp

[DLG:IDD_PREFERENCES_PAGE]
Type=1
Class=CCalendarPreferencesPage
ControlCount=15
Control1=IDC_SHOWMINICALENDAR,button,1342242819
Control2=IDC_DYNAMICTASKHEIGHT,button,1342242819
Control3=IDC_SHOWTASKSCONTINUOUS,button,1342242819
Control4=IDC_SHOWSTARTDATES,button,1342242819
Control5=IDC_SHOWCALCSTARTDATES,button,1342242819
Control6=IDC_SHOWDUEDATES,button,1342242819
Control7=IDC_SHOWCALCDUEDATES,button,1342242819
Control8=IDC_SHOWDONEDATES,button,1342242819
Control9=IDC_STATIC,button,1342177287
Control10=IDC_USECREATIONFORSTART,button,1342308361
Control11=IDC_USEDUEFORSTART,button,1342177289
Control12=IDC_USEDUEORTODAYFORSTART,button,1342177289
Control13=IDC_STATIC,button,1342177287
Control14=IDC_USESTARTFORDUE,button,1342308361
Control15=IDC_USESTARTORTODAYFORDUE,button,1342177289

[DLG:IDD_CALENDAR_DIALOG]
Type=1
Class=?
ControlCount=8
Control1=IDC_SELECTEDTASKDATES,static,1342312961
Control2=IDC_STATIC,static,1342308352
Control3=IDC_NUMWEEKS,combobox,1342242819
Control4=IDC_CALENDAR_FRAME,static,1342177288
Control5=IDC_SNAPMODES,combobox,1344340227
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_TB_PLACEHOLDER,static,1073741831

[DLG:IDD_PREFERENCES_DIALOG]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PPHOST,static,1342177288

[CLS:CDayViewListCtrl]
Type=0
HeaderFile=DayViewListCtrl.h
ImplementationFile=DayViewListCtrl.cpp
BaseClass=CListCtrl
Filter=W

[TB:IDR_TOOLBAR]
Type=1
Class=?
Command1=ID_CAL_GOTOTODAY
Command2=ID_CAL_PREFS
CommandCount=2

