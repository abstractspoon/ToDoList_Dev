# Microsoft Developer Studio Project File - Name="RTFContentCtrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RTFContentCtrl - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTFContentCtrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RTFContentCtrl.mak" CFG="RTFContentCtrl - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTFContentCtrl - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTFContentCtrl - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RTFContentCtrl", HIEAAAAA"
# PROP Scc_LocalPath "..\..\rtfcontentctrl"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTFContentCtrl - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTFContentCtrl___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "RTFContentCtrl___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_EXPORTING" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /Gy /D "_DEBUG" /D "_EXPORTING" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "PNG_SUPPORT" /FAs /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 msimg32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\ToDoList	mkdir ..\ToDoList\unicode_debug	copy unicode_debug\rtfcontentctrl.dll ..\todolist\unicode_debug /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RTFContentCtrl - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RTFContentCtrl___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "RTFContentCtrl___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_EXPORTING" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O1 /D "NDEBUG" /D "_EXPORTING" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "PNG_SUPPORT" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 msimg32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /OPT:REF
# SUBTRACT LINK32 /pdb:none /map
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\ToDoList	mkdir ..\ToDoList\unicode_release	copy unicode_release\rtfcontentctrl.dll ..\todolist\unicode_release /y	copy unicode_release\rtfcontentctrl.pdb ..\todolist\unicode_release /y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "RTFContentCtrl - Win32 Unicode Debug"
# Name "RTFContentCtrl - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CreateFileLinkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditWebLinkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertTableDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RRECRuler.cpp
# End Source File
# Begin Source File

SOURCE=.\RRECToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\RTFContentControl.cpp
# End Source File
# Begin Source File

SOURCE=.\RTFContentCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RTFContentCtrl.rc
# End Source File
# Begin Source File

SOURCE=.\RTFPreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RulerRichEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\RulerRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CreateFileLinkDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditWebLinkDlg.h
# End Source File
# Begin Source File

SOURCE=.\ids.h
# End Source File
# Begin Source File

SOURCE=.\InsertTableDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RRECRuler.h
# End Source File
# Begin Source File

SOURCE=.\RRECToolbar.h
# End Source File
# Begin Source File

SOURCE=.\RTFContentControl.h
# End Source File
# Begin Source File

SOURCE=.\RTFContentCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RTFPreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=.\RulerRichEdit.h
# End Source File
# Begin Source File

SOURCE=.\RulerRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\help_button.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\RTFContentCtrl.rc2
# End Source File
# Begin Source File

SOURCE=.\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\toolbar2.bmp
# End Source File
# End Group
# Begin Group "Libs"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\3rdParty\Zlib\ZLIBSTAT.LIB
# End Source File
# Begin Source File

SOURCE=..\3rdParty\Zlib\zlibstatD.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\RTFContentCtrl.vcxproj
# End Source File
# Begin Source File

SOURCE=.\RTFContentCtrl.vcxproj.filters
# End Source File
# End Target
# End Project
