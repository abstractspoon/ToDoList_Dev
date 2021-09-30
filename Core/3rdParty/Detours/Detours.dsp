# Microsoft Developer Studio Project File - Name="Detours" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=Detours - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Detours.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Detours.mak" CFG="Detours - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Detours - Win32 Unicode Release" (based on "Win32 (x86) External Target")
!MESSAGE "Detours - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Detours - Win32 Unicode Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f Makefile"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Makefile.exe"
# PROP BASE Bsc_Name "Makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f Makefile"
# PROP Rebuild_Opt "/a"
# PROP Target_File "Detours.lib"
# PROP Bsc_Name "Detours.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Detours - Win32 Unicode Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f Makefile"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Makefile.exe"
# PROP BASE Bsc_Name "Makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f Makefile"
# PROP Rebuild_Opt "/a"
# PROP Target_File "Detours.exe"
# PROP Bsc_Name "Detours.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Detours - Win32 Unicode Release"
# Name "Detours - Win32 Unicode Debug"

!IF  "$(CFG)" == "Detours - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "Detours - Win32 Unicode Debug"

!ENDIF 

# Begin Group "Lib.x86"

# PROP Default_Filter ".lib;.pdb"
# Begin Source File

SOURCE=.\lib.X86\detours.pdb
# End Source File
# Begin Source File

SOURCE=.\lib.X86\detours.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\creatwth.cpp
# End Source File
# Begin Source File

SOURCE=.\detours.cpp
# End Source File
# Begin Source File

SOURCE=.\detours.h
# End Source File
# Begin Source File

SOURCE=.\detver.h
# End Source File
# Begin Source File

SOURCE=.\disasm.cpp
# End Source File
# Begin Source File

SOURCE=.\disolarm.cpp
# End Source File
# Begin Source File

SOURCE=.\disolarm64.cpp
# End Source File
# Begin Source File

SOURCE=.\disolia64.cpp
# End Source File
# Begin Source File

SOURCE=.\disolx64.cpp
# End Source File
# Begin Source File

SOURCE=.\disolx86.cpp
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\modules.cpp
# End Source File
# Begin Source File

SOURCE=.\strsafe.cpp
# End Source File
# Begin Source File

SOURCE=.\strsafe.h
# End Source File
# Begin Source File

SOURCE=.\uimports.cpp
# End Source File
# End Target
# End Project
