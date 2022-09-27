# Microsoft Developer Studio Project File - Name="ToDoList" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ToDoList - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ToDoList.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ToDoList.mak" CFG="ToDoList - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ToDoList - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "ToDoList - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ToDoList", OBDAAAAA"
# PROP Scc_LocalPath "..\..\todolist"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ToDoList - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ToDoList___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ToDoList___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "NO_DIAGRAM_TEMPLATE" /D "NO_XML_TEMPLATE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /D "_DEBUG" /D "_AFXDLL" /D "NO_DIAGRAM_TEMPLATE" /D "NO_XML_TEMPLATE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "PNG_SUPPORT" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Msimg32.lib msimg32.lib Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\3rdParty\Detours\lib.x86\detours.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\Plugins	mkdir ..\..\Plugins\Debug	copy unicode_debug\ToDoList.exe ..\..\Plugins\debug /y	copy unicode_debug\ToDoList.pdb ..\..\Plugins\debug /y	unicode_debug\TDLTest.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ToDoList - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ToDoList___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "ToDoList___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /Zi /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NO_DIAGRAM_TEMPLATE" /D "NO_XML_TEMPLATE" /D "USE_TRANSTEXT" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O1 /D "USE_TRANSTEXT" /D "NDEBUG" /D "_AFXDLL" /D "NO_DIAGRAM_TEMPLATE" /D "NO_XML_TEMPLATE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "PNG_SUPPORT" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Msimg32.lib msimg32.lib Winmm.lib /nologo /subsystem:windows /pdb:"ToDoList.pdb" /map /machine:I386 /pdbtype:con /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 ..\3rdParty\Detours\lib.x86\detours.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /MAPINFO:LINES /MAPINFO:EXPORTS /OPT:REF
# SUBTRACT LINK32 /pdb:none /map /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\Plugins	mkdir ..\..\Plugins\Release	copy unicode_release\ToDoList.exe ..\..\Plugins\release /y	unicode_release\TDLTest.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ToDoList - Win32 Unicode Debug"
# Name "ToDoList - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToDoList.cpp
# End Source File
# Begin Source File

SOURCE=.\ToDoList.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToDoList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\abstractspoon.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_extra_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_timetrack.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_tray_dark.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_tray_light.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_tray_timetrack_dark.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_tray_timetrack_light.ico
# End Source File
# Begin Source File

SOURCE=.\res\clock.ico
# End Source File
# Begin Source File

SOURCE=.\res\column_header.bmp
# End Source File
# Begin Source File

SOURCE=.\res\custattrib_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\custattrib_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\custom_attrib_icons.ico
# End Source File
# Begin Source File

SOURCE=.\res\datetime_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\datetime_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\depend_link.ico
# End Source File
# Begin Source File

SOURCE=.\res\English.gif
# End Source File
# Begin Source File

SOURCE=.\res\fileedit_browse.ico
# End Source File
# Begin Source File

SOURCE=.\res\fileedit_go.ico
# End Source File
# Begin Source File

SOURCE=.\res\find_dialog_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\find_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\find_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\help_button.ico
# End Source File
# Begin Source File

SOURCE=.\res\list_view_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\null.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlook_16.ico
# End Source File
# Begin Source File

SOURCE=.\res\outlooktypes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\plus.ico
# End Source File
# Begin Source File

SOURCE=.\res\prefs_dlg_std.ico
# End Source File
# Begin Source File

SOURCE=.\res\quickfind_next.ico
# End Source File
# Begin Source File

SOURCE=.\res\quickfind_prev.ico
# End Source File
# Begin Source File

SOURCE=.\res\reset.ico
# End Source File
# Begin Source File

SOURCE=.\res\search_prefs.ico
# End Source File
# Begin Source File

SOURCE=.\res\shield.ico
# End Source File
# Begin Source File

SOURCE=.\res\simpletext.ico
# End Source File
# Begin Source File

SOURCE=.\res\social_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\social_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\src_control_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\style_image.ico
# End Source File
# Begin Source File

SOURCE=.\res\style_para.ico
# End Source File
# Begin Source File

SOURCE=.\res\style_table.ico
# End Source File
# Begin Source File

SOURCE=.\res\style_wrap.ico
# End Source File
# Begin Source File

SOURCE=.\res\task_icons_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\task_tree_std.ico
# End Source File
# Begin Source File

SOURCE=..\TDLUpdate\res\TDLUpdate.ico
# End Source File
# Begin Source File

SOURCE=..\TDLUpdate\res\TDLUpdate_wiz_header.bmp
# End Source File
# Begin Source File

SOURCE=.\res\timetrac.bmp
# End Source File
# Begin Source File

SOURCE=.\res\timetrack_btns.bmp
# End Source File
# Begin Source File

SOURCE=.\res\timetrack_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\todolist.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\todolist.exe.xp.manifest
# End Source File
# Begin Source File

SOURCE=.\res\ToDoList.rc2
# End Source File
# Begin Source File

SOURCE=.\res\udtprefs_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\udtprefs_toolbar_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\update_filter.ico
# End Source File
# Begin Source File

SOURCE=.\res\wiz_header.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\XPcheckboxes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\YourLanguage.GIF
# End Source File
# End Group
# Begin Group "Binaries"

# PROP Default_Filter ""
# Begin Group "Unicode_Debug"

# PROP Default_Filter "exe;dll"
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Rtf.Converter.Html.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Rtf.Interpreter.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Rtf.Parser.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Solutions.Community.Rtf2Html.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Itenso.Sys.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Debug\Rtf2HtmlBridge.dll
# End Source File
# End Group
# Begin Group "Unicode_Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Rtf.Converter.Html.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Rtf.Interpreter.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Rtf.Parser.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Solutions.Community.Rtf2Html.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Itenso.Sys.dll
# End Source File
# Begin Source File

SOURCE=.\Unicode_Release\Rtf2HtmlBridge.dll
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ToDoList.vcxproj
# End Source File
# Begin Source File

SOURCE=.\ToDoList.vcxproj.filters
# End Source File
# Begin Source File

SOURCE=..\ToDoList_Core.sln
# End Source File
# End Target
# End Project
# Section ToDoList : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowserCtrl
# 	2:10:HeaderFile:webbrowserctrl.h
# 	2:8:ImplFile:webbrowserctrl.cpp
# End Section
# Section ToDoList : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowserctrl.h
# 	2:16:DefaultSinkClass:CWebBrowserCtrl
# End Section
