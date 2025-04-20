# Microsoft Developer Studio Project File - Name="MySpellCheck" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MySpellCheck - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MySpellCheck.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MySpellCheck.mak" CFG="MySpellCheck - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MySpellCheck - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MySpellCheck - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MySpellCheck", ZXDAAAAA"
# PROP Scc_LocalPath "..\..\myspellcheck"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MySpellCheck - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MySpellCheck___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MySpellCheck___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MYSPELLCHECK_EXPORTS" /D "_EXPORTING" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "MYSPELLCHECK_EXPORTS" /D "_EXPORTING" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\ToDoList	mkdir ..\ToDoList\unicode_debug	copy unicode_debug\myspellcheck.dll ..\todolist\unicode_debug /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "MySpellCheck - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MySpellCheck___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "MySpellCheck___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MYSPELLCHECK_EXPORTS" /D "_EXPORTING" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O1 /D "_USRDLL" /D "MYSPELLCHECK_EXPORTS" /D "_EXPORTING" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /OPT:REF
# SUBTRACT LINK32 /pdb:none /map
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\ToDoList	mkdir ..\ToDoList\unicode_release	copy unicode_release\myspellcheck.dll ..\todolist\unicode_release /y	copy unicode_release\myspellcheck.pdb ..\todolist\unicode_release /y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "MySpellCheck - Win32 Unicode Debug"
# Name "MySpellCheck - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\3rdParty\myspell\affentry.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\affixmgr.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\csutil.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\dictmgr.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\hashmgr.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\myspell.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\MySpellCheck.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\mythes.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\suggestmgr.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\3rdParty\myspell\affentry.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\affixmgr.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\atypes.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\baseaffix.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\csutil.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\dictmgr.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\hashmgr.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\htypes.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\myspell.hxx
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\mythes.hxx
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\myspell\suggestmgr.hxx
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\3rdParty\myspell\license.readme
# End Source File
# Begin Source File

SOURCE=.\MySpellCheck.vcxproj
# End Source File
# Begin Source File

SOURCE=.\MySpellCheck.vcxproj.filters
# End Source File
# End Target
# End Project
