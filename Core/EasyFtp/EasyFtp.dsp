# Microsoft Developer Studio Project File - Name="EasyFtp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EasyFtp - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EasyFtp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EasyFtp.mak" CFG="EasyFtp - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EasyFtp - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "EasyFtp - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EasyFtp", DRDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EasyFtp - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NUnicode Release" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NUnicode Release" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NUnicode Release" /mktyplib203 /win32
# ADD MTL /nologo /D "NUnicode Release" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NUnicode Release" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NUnicode Release" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "EasyFtp - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_Unicode Debug" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_Unicode Debug" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_Unicode Debug" /mktyplib203 /win32
# ADD MTL /nologo /D "_Unicode Debug" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_Unicode Debug" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_Unicode Debug" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept


!ENDIF 

# Begin Target

# Name "EasyFtp - Win32 Unicode Release"
# Name "EasyFtp - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Shared\AutoFlag.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\deferWndMove.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\DlgUnits.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\driveinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\EasyFtp.cpp
# End Source File
# Begin Source File

SOURCE=.\EasyFtp.rc
# End Source File
# Begin Source File

SOURCE=..\Shared\EnCommandLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\enedit.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\fileedit.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\FILEMISC.CPP
# End Source File
# Begin Source File

SOURCE=..\Shared\FolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\HotTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\MASKEDIT.CPP
# End Source File
# Begin Source File

SOURCE=..\Shared\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\RCCtrlParser.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\RemoteFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\RemoteFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\RuntimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\ServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Shared\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\SysImageList.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\Themed.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\WinClasses.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Shared\AutoFlag.h
# End Source File
# Begin Source File

SOURCE=..\Shared\deferWndMove.h
# End Source File
# Begin Source File

SOURCE=..\Shared\DlgUnits.h
# End Source File
# Begin Source File

SOURCE=..\Shared\driveinfo.h
# End Source File
# Begin Source File

SOURCE=.\EasyFtp.h
# End Source File
# Begin Source File

SOURCE=..\Shared\EnCommandLineInfo.h
# End Source File
# Begin Source File

SOURCE=..\Shared\enedit.h
# End Source File
# Begin Source File

SOURCE=..\Shared\fileedit.h
# End Source File
# Begin Source File

SOURCE=..\Shared\FILEMISC.H
# End Source File
# Begin Source File

SOURCE=..\Shared\FolderDialog.h
# End Source File
# Begin Source File

SOURCE=..\Shared\HotTracker.h
# End Source File
# Begin Source File

SOURCE=..\Shared\MASKEDIT.H
# End Source File
# Begin Source File

SOURCE=..\Shared\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=..\Shared\RCCtrlParser.h
# End Source File
# Begin Source File

SOURCE=..\Shared\RemoteFile.h
# End Source File
# Begin Source File

SOURCE=..\Shared\RemoteFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\Shared\RuntimeDlg.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\Schemadef.h
# End Source File
# Begin Source File

SOURCE=..\Shared\ServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Shared\Subclass.h
# End Source File
# Begin Source File

SOURCE=..\Shared\SysImageList.h
# End Source File
# Begin Source File

SOURCE=..\Shared\Themed.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\Tmschema.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\Uxtheme.h
# End Source File
# Begin Source File

SOURCE=..\Shared\wclassdefines.h
# End Source File
# Begin Source File

SOURCE=..\Shared\WinClasses.h
# End Source File
# Begin Source File

SOURCE=..\Shared\winstyles.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\EasyFtp.ico
# End Source File
# Begin Source File

SOURCE=.\res\EasyFtp.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
