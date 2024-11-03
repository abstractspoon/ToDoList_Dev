ECHO OFF
CLS

pushd %~dp0
set REPO=%CD%
ECHO REPO=%REPO%

if NOT EXIST %REPO%\Core exit
if NOT EXIST %REPO%\Plugins exit

ECHO ON

REM - Remember to update ToDoList version number
REM - Remember to pull latest translations
pause

REM - Build core app in VC6
cd %REPO%\Core

REM - Detours
REM "C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe" .\3rdParty\Detours\Detours.dsw /MAKE "ALL - Win32 Unicode Debug"

REM - Core app
set OUTPUT_FILE=%REPO%\Core\ToDoList\Unicode_Debug\Core_Build_Output.txt
"C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe" .\ToDoList_Core.dsw /MAKE "ALL - Win32 Unicode Debug" /OUT %OUTPUT_FILE% 

REM - Check for compile errors
ECHO OFF

findstr /C:") : error" %OUTPUT_FILE%
if %errorlevel%==1 (
REM - Check for link errors
findstr /C:"Error executing link.exe" %OUTPUT_FILE%
)
if %errorlevel%==0 (
echo Build Errors!
pause
exit
)

REM Run units tests
ECHO ON

cd TDLTest\Unicode_Debug

TDLTest > %OUTPUT_FILE%

ECHO OFF
findstr /C:"tests FAILED" %OUTPUT_FILE%
if %errorlevel%==0 (
echo Test Errors!
pause
exit
)

REM - Build plugins using MSBuild for reliability
ECHO ON

SET MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe"
SET BUILDPARAMS=/t:Build /p:Configuration=Debug /m /v:normal /noWarn:MSB3267;MSB3305;LNK4248;CS1762;LNK4221;MSB3026

REM - This is required to get the warning suppression to work
SET VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140\

cd %REPO%\Plugins
%MSBUILD% .\ToDoList_Plugins.sln %BUILDPARAMS%

pause
