# Microsoft Developer Studio Project File - Name="3rdParty" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=3rdParty - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "3rdParty.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "3rdParty.mak" CFG="3rdParty - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "3rdParty - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "3rdParty - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/3rdParty", WQDAAAAA"
# PROP Scc_LocalPath "..\..\3rdparty"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "3rdParty - Win32 Unicode Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "3rdParty - Win32 Unicode Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "3rdParty - Win32 Unicode Debug"
# Name "3rdParty - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Base64Coder.cpp
# End Source File
# Begin Source File

SOURCE=.\CalendarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipboardBackup.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Compression.cpp
# End Source File
# Begin Source File

SOURCE=.\DibData.cpp
# End Source File
# Begin Source File

SOURCE=.\DragDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\FileVersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FontComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ggets.cpp
# End Source File
# Begin Source File

SOURCE=.\GUI.CPP
# End Source File
# Begin Source File

SOURCE=.\HMXChart.cpp
# End Source File
# Begin Source File

SOURCE=.\HMXDataset.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\LockableHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MemUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\msoutl.cpp
# End Source File
# Begin Source File

SOURCE=.\msword.cpp
# End Source File
# Begin Source File

SOURCE=.\NewRandom.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlWithTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Quantize.cpp
# End Source File
# Begin Source File

SOURCE=.\RegUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\rijndael.cpp
# End Source File
# Begin Source File

SOURCE=.\sha2.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellIcons.cpp
# End Source File
# Begin Source File

SOURCE=.\SizeComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\StatLink.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBarACT.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.cpp
# End Source File
# Begin Source File

SOURCE=.\XHTMLStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\XmlNodeWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\XNamedColors.cpp
# End Source File
# Begin Source File

SOURCE=.\XPTabCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Base64Coder.h
# End Source File
# Begin Source File

SOURCE=.\CalendarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ClipboardBackup.h
# End Source File
# Begin Source File

SOURCE=.\ColourPicker.h
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\Compression.h
# End Source File
# Begin Source File

SOURCE=.\DibData.h
# End Source File
# Begin Source File

SOURCE=.\DragDrop.h
# End Source File
# Begin Source File

SOURCE=.\FileVersionInfo.h
# End Source File
# Begin Source File

SOURCE=.\FontComboBox.h
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ggets.h
# End Source File
# Begin Source File

SOURCE=.\GUI.H
# End Source File
# Begin Source File

SOURCE=.\HMXChart.h
# End Source File
# Begin Source File

SOURCE=.\HMXDataset.h
# End Source File
# Begin Source File

SOURCE=.\Html2Rtf.h
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\Zlib\IOAPI.H
# End Source File
# Begin Source File

SOURCE=.\Zlib\IOWIN32.H
# End Source File
# Begin Source File

SOURCE=.\LockableHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MemUtil.h
# End Source File
# Begin Source File

SOURCE=.\msoutl.h
# End Source File
# Begin Source File

SOURCE=.\msproject.h
# End Source File
# Begin Source File

SOURCE=.\msword.h
# End Source File
# Begin Source File

SOURCE=.\NewRandom.h
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlWithTimer.h
# End Source File
# Begin Source File

SOURCE=.\ProgressThread.h
# End Source File
# Begin Source File

SOURCE=.\Quantize.h
# End Source File
# Begin Source File

SOURCE=.\RegUtil.h
# End Source File
# Begin Source File

SOURCE=.\rijndael.h
# End Source File
# Begin Source File

SOURCE=.\Schemadef.h
# End Source File
# Begin Source File

SOURCE=.\SendFileTo.h
# End Source File
# Begin Source File

SOURCE=.\sha2.h
# End Source File
# Begin Source File

SOURCE=.\ShellIcons.h
# End Source File
# Begin Source File

SOURCE=.\SizeComboBox.h
# End Source File
# Begin Source File

SOURCE=.\StatLink.h
# End Source File
# Begin Source File

SOURCE=.\StatusBarACT.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.h
# End Source File
# Begin Source File

SOURCE=.\TemplateSmartPtr.h
# End Source File
# Begin Source File

SOURCE=.\Tmschema.h
# End Source File
# Begin Source File

SOURCE=.\TOM.h
# End Source File
# Begin Source File

SOURCE=.\Zlib\UNZIP.H
# End Source File
# Begin Source File

SOURCE=.\Uxtheme.h
# End Source File
# Begin Source File

SOURCE=.\XHTMLStatic.h
# End Source File
# Begin Source File

SOURCE=.\XmlNodeWrapper.h
# End Source File
# Begin Source File

SOURCE=.\XNamedColors.h
# End Source File
# Begin Source File

SOURCE=.\XPTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Zlib\ZCONF.H
# End Source File
# Begin Source File

SOURCE=.\Zlib\ZIP.H
# End Source File
# Begin Source File

SOURCE=.\Zlib\ZLIB.H
# End Source File
# End Group
# Begin Source File

SOURCE=.\3rdParty.vcxproj
# End Source File
# Begin Source File

SOURCE=.\3rdParty.vcxproj.filters
# End Source File
# End Target
# End Project
