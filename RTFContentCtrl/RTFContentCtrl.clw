; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCreateFileLinkDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "rtfcontentctrl.h"
LastPage=0

ClassCount=7
Class1=CCreateFileLinkPage
Class2=CCreateFileLinkDlg
Class3=CEditWebLinkDlg
Class4=CInsertTableDlg
Class5=CRTFContentControl
Class6=CRulerRichEdit
Class7=CRulerRichEditCtrl

ResourceCount=7
Resource1=IDD_PREFERENCES_PAGE
Resource2=IDD_INSERTTABLE_DIALOG
Resource3=TOOLBAR_CONTROL
Resource4=IDD_EDITWEBLINK_DIALOG
Resource5=IDD_PREFERENCES_DIALOG
Resource6=IDD_LINKTOFILE_DIALOG
Resource7=IDR_POPUP

[CLS:CCreateFileLinkPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=CreateFileLinkDlg.h
ImplementationFile=CreateFileLinkDlg.cpp

[CLS:CCreateFileLinkDlg]
Type=0
BaseClass=CDialog
HeaderFile=CreateFileLinkDlg.h
ImplementationFile=CreateFileLinkDlg.cpp

[CLS:CEditWebLinkDlg]
Type=0
BaseClass=CDialog
HeaderFile=EditWebLinkDlg.h
ImplementationFile=EditWebLinkDlg.cpp

[CLS:CInsertTableDlg]
Type=0
BaseClass=CDialog
HeaderFile=InsertTableDlg.h
ImplementationFile=InsertTableDlg.cpp

[CLS:CRTFContentControl]
Type=0
BaseClass=CRulerRichEditCtrl
HeaderFile=RTFContentControl.h
ImplementationFile=RTFContentControl.cpp

[CLS:CRulerRichEdit]
Type=0
BaseClass=CUrlRichEditCtrl
HeaderFile=RulerRichEdit.h
ImplementationFile=RulerRichEdit.cpp

[CLS:CRulerRichEditCtrl]
Type=0
BaseClass=CWnd
HeaderFile=RulerRichEditCtrl.h
ImplementationFile=RulerRichEditCtrl.cpp

[DLG:IDD_EDITWEBLINK_DIALOG]
Type=1
Class=CEditWebLinkDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_WEBLINK,edit,1350631552

[DLG:IDD_INSERTTABLE_DIALOG]
Type=1
Class=CInsertTableDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_NUMROWS,combobox,1344339970
Control6=IDC_NUMCOLS,combobox,1344339970
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COLWIDTH,combobox,1344339970
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177287

[TB:TOOLBAR_CONTROL]
Type=1
Class=?
Command1=ID_EDIT_FONT
Command2=ID_EDIT_BOLD
Command3=ID_EDIT_ITALIC
Command4=ID_EDIT_UNDERLINE
Command5=ID_EDIT_STRIKETHRU
Command6=ID_EDIT_GROWFONT
Command7=ID_EDIT_SHRINKFONT
Command8=ID_EDIT_LEFTALIGN
Command9=ID_EDIT_CENTERALIGN
Command10=ID_EDIT_RIGHTALIGN
Command11=ID_EDIT_JUSTIFY
Command12=ID_EDIT_BULLET
Command13=ID_EDIT_NUMBER
Command14=ID_EDIT_OUTDENT
Command15=ID_EDIT_INDENT
Command16=ID_EDIT_INSERTTABLE
Command17=ID_EDIT_TEXTCOLOR
Command18=ID_EDIT_BACKCOLOR
Command19=ID_EDIT_WORDWRAP
Command20=ID_EDIT_INLINESPELLCHECK
Command21=ID_PREFERENCES
CommandCount=21

[MNU:IDR_POPUP]
Type=1
Class=?
Command1=ID_EDIT_BOLD
Command2=ID_EDIT_ITALIC
Command3=ID_EDIT_UNDERLINE
Command4=ID_EDIT_STRIKETHRU
Command5=ID_EDIT_GROWFONT
Command6=ID_EDIT_SHRINKFONT
Command7=ID_EDIT_SUBSCRIPT
Command8=ID_EDIT_SUPERSCRIPT
Command9=ID_EDIT_LEFTALIGN
Command10=ID_EDIT_CENTERALIGN
Command11=ID_EDIT_RIGHTALIGN
Command12=ID_EDIT_JUSTIFY
Command13=ID_EDIT_INDENT
Command14=ID_EDIT_OUTDENT
Command15=ID_EDIT_INSERTTABLE
Command16=ID_EDIT_HORZRULE
Command17=ID_EDIT_CUT
Command18=ID_EDIT_COPY
Command19=ID_EDIT_PASTE
Command20=ID_EDIT_DELETE
Command21=ID_EDIT_COPYASHTML
Command22=ID_EDIT_COPYFORMATTING
Command23=ID_EDIT_PASTESIMPLE
Command24=ID_EDIT_PASTEASREF
Command25=ID_EDIT_PASTEFORMATTING
Command26=ID_EDIT_SELECT_ALL
Command27=ID_EDIT_FIND
Command28=ID_EDIT_FINDREPLACE
Command29=ID_EDIT_OPENURL
Command30=ID_EDIT_COPYURL
Command31=ID_EDIT_FILEBROWSE
Command32=ID_EDIT_SPELLCHECK
Command33=ID_EDIT_SHOWTOOLBAR
Command34=ID_EDIT_SHOWRULER
Command35=ID_EDIT_WORDWRAP
Command36=ID_EDIT_INLINESPELLCHECK
Command37=ID_PREFERENCES
CommandCount=37

[DLG:IDD_LINKTOFILE_DIALOG]
Type=1
Class=?
ControlCount=9
Control1=IDC_STATIC,button,1342177287
Control2=IDC_MAKEDEFAULT,button,1342242819
Control3=IDOK,button,1342373889
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342177296
Control6=IDC_FILEURL,button,1342308361
Control7=IDC_FILECOPY,button,1342186505
Control8=IDC_FILELINK,button,1342186505
Control9=IDC_FILEIMAGE,button,1342186505

[DLG:IDD_PREFERENCES_DIALOG]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342373889
Control2=IDCANCEL,button,1342242816
Control3=IDC_PPHOST,static,1342177287

[DLG:IDD_PREFERENCES_PAGE]
Type=1
Class=?
ControlCount=7
Control1=IDC_USEMSWORD,button,1342242819
Control2=IDC_LINKTOFILE_GROUP,static,1342308352
Control3=IDC_PROMPTFORFILELINK,button,1342242819
Control4=IDC_FILEURL,button,1342308361
Control5=IDC_FILECOPY,button,1342186505
Control6=IDC_FILELINK,button,1342186505
Control7=IDC_FILEIMAGE,button,1342186505

