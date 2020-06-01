ECHO OFF
CLS

pushd %~dp0
set REPO=%CD%
ECHO REPO=%REPO%

if NOT EXIST %REPO%\Core exit
if NOT EXIST %REPO%\Plugins exit

ECHO ON

REM - Make sure caller has updated ToDoList version number
pause

REM - Build core app in VC6
cd %REPO%\Core
"C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe" .\ToDoList_Core.dsw /MAKE "ALL - Win32 Unicode Release" 

REM - Build plugins using MSBuild for reliability
SET MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe"
SET BUILDPARAMS=/t:Build /p:Configuration=Release /m /v:normal /noWarn:MSB3267;MSB3305;LNK4248;CS1762;LNK4221;MSB3026
SET VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140\

REM - Build ToDoList_Core libraries in VS2010 for linking into PluginsHelpers
cd %REPO%\Plugins

%MSBUILD% .\Core\ToDoList_Core_For_Plugins.sln %BUILDPARAMS%

REM - Build rest of plugins
%MSBUILD% .\ToDoList_Plugins.sln %BUILDPARAMS%

REM - Allow caller to cancel building Zip
pause

CALL %REPO%\BuildReleaseZip.bat
CALL %REPO%\BuildCodeZip.bat

REM - Open repo folder
Explorer.exe "%REPO%"

popd
pause
