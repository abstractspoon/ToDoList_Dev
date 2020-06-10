# Microsoft Developer Studio Project File - Name="KanbanBoard" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=KanbanBoard - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KanbanBoard.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KanbanBoard.mak" CFG="KanbanBoard - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KanbanBoard - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "KanbanBoard - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/KanbanBoard", DDHAAAAA"
# PROP Scc_LocalPath "..\..\kanbanboard"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KanbanBoard - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_EXPORTING" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\ToDoList	mkdir ..\ToDoList\unicode_debug	copy unicode_debug\KanbanBoard.dll ..\todolist\unicode_debug /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "KanbanBoard - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_EXPORTING" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /OPT:REF
# SUBTRACT LINK32 /pdb:none /map
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\ToDoList	mkdir ..\ToDoList\unicode_release	copy unicode_release\KanbanBoard.dll ..\todolist\unicode_release /y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "KanbanBoard - Win32 Unicode Debug"
# Name "KanbanBoard - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\KanbanAttributeComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanBoard.rc
# End Source File
# Begin Source File

SOURCE=.\KanbanColumnCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanColumnCtrlArray.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanCustomAttributeComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanFixedColumnListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanOptionComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanPreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanStruct.cpp
# End Source File
# Begin Source File

SOURCE=.\KanbanWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\KanbanAttributeComboBox.h
# End Source File
# Begin Source File

SOURCE=.\KanbanBoard.h
# End Source File
# Begin Source File

SOURCE=.\KanbanColors.h
# End Source File
# Begin Source File

SOURCE=.\KanbanColumnCtrl.h
# End Source File
# Begin Source File

SOURCE=.\KanbanColumnCtrlArray.h
# End Source File
# Begin Source File

SOURCE=.\KanbanCtrl.h
# End Source File
# Begin Source File

SOURCE=.\KanbanCustomAttributeComboBox.h
# End Source File
# Begin Source File

SOURCE=.\KanbanEnum.h
# End Source File
# Begin Source File

SOURCE=.\KanbanFixedColumnListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Kanbanmsg.h
# End Source File
# Begin Source File

SOURCE=.\KanbanOptionComboBox.h
# End Source File
# Begin Source File

SOURCE=.\KanbanPreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=.\KanbanStatic.h
# End Source File
# Begin Source File

SOURCE=.\KanbanStruct.h
# End Source File
# Begin Source File

SOURCE=.\KanbanWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\column_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\column_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\help_button.ico
# End Source File
# Begin Source File

SOURCE=.\res\kanban.ico
# End Source File
# Begin Source File

SOURCE=.\res\KanbanBoard.rc2
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\XPcheckboxes.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\KanbanBoard.vcxproj
# End Source File
# Begin Source File

SOURCE=.\KanbanBoard.vcxproj.filters
# End Source File
# End Target
# End Project
