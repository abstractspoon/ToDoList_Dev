# Microsoft Developer Studio Project File - Name="Interfaces" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Interfaces - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Interfaces.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Interfaces.mak" CFG="Interfaces - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Interfaces - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Interfaces - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Interfaces", HACAAAAA"
# PROP Scc_LocalPath "..\..\Interfaces"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Interfaces - Win32 Unicode Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Interfaces - Win32 Unicode Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /YX"stdafx.h" /FD /c
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

# Name "Interfaces - Win32 Unicode Debug"
# Name "Interfaces - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ContentCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentTypeCombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportExportComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportExportMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportExportUIHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditSpellCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\TasklistStorageMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\UIExtensionHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\UIExtensionMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\UIThemeFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ContentCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ContentMgr.h
# End Source File
# Begin Source File

SOURCE=.\ContentTypeCombobox.h
# End Source File
# Begin Source File

SOURCE=.\IContentControl.h
# End Source File
# Begin Source File

SOURCE=.\IEncryption.h
# End Source File
# Begin Source File

SOURCE=.\IEnums.h
# End Source File
# Begin Source File

SOURCE=.\IFileStorage.h
# End Source File
# Begin Source File

SOURCE=.\IImportExport.h
# End Source File
# Begin Source File

SOURCE=.\ImportExportComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ImportExportMgr.h
# End Source File
# Begin Source File

SOURCE=.\ImportExportUIHelper.h
# End Source File
# Begin Source File

SOURCE=.\IPreferences.h
# End Source File
# Begin Source File

SOURCE=.\ISpellCheck.h
# End Source File
# Begin Source File

SOURCE=.\ITaskList.h
# End Source File
# Begin Source File

SOURCE=.\ITaskListStorage.h
# End Source File
# Begin Source File

SOURCE=.\ITransText.h
# End Source File
# Begin Source File

SOURCE=.\IUIExtension.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\RichEditSpellCheck.h
# End Source File
# Begin Source File

SOURCE=.\SpellCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpellChecker.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TasklistSchemaDef.h
# End Source File
# Begin Source File

SOURCE=.\TaskListStorageMgr.h
# End Source File
# Begin Source File

SOURCE=.\UIExtensionHelper.h
# End Source File
# Begin Source File

SOURCE=.\UIExtensionMgr.h
# End Source File
# Begin Source File

SOURCE=.\UITHEME.h
# End Source File
# Begin Source File

SOURCE=.\UIThemeFile.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Interfaces.vcxproj
# End Source File
# Begin Source File

SOURCE=.\Interfaces.vcxproj.filters
# End Source File
# End Target
# End Project
