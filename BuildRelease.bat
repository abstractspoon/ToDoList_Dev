ECHO OFF
CLS

pushd %~dp0
set REPO=%CD%
ECHO REPO=%REPO%

if NOT EXIST %REPO%\Core exit
if NOT EXIST %REPO%\Plugins exit

ECHO ON

REM - Build Core App
cd %REPO%\Core
"C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe" .\ToDoList_Core.dsw /MAKE "ALL - Win32 Unicode Release" 

REM - Copy ToDoList.pdb to symbols folder
%REPO%\Core\ToDoList\Unicode_Release\ToDoList.exe -ver
SET /P TDLVER=< .\ver.txt
DEL .\ver.txt

MKDIR %REPO%\..\ToDoList_Symbols
MKDIR %REPO%\..\ToDoList_Symbols\%TDLVER%
COPY /Y /B %REPO%\Core\ToDoList\Unicode_Release\ToDoList.pdb %REPO%\..\ToDoList_Symbols\%TDLVER%\

REM - Build Plugins
cd %REPO%\Plugins

REM - Rebuild 3rdParty by itself because PluginHelpers is dependent on it
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com" .\3rdParty.sln /Build "Release"

REM - Rebuild PluginHelpers by itself because everything else is dependent on it
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com" .\PluginHelpers.sln /Build "Release"

REM - Build rest of plugins
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com" .\ToDoList_Plugins.sln /Build "Release"

REM Allow caller to cancel building Zip
pause

CALL %REPO%\BuildReleaseZip.bat

popd
