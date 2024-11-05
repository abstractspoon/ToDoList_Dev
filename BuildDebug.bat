pushd %~dp0
set REPO=%CD%
ECHO REPO=%REPO%

if NOT EXIST %REPO%\Core exit
if NOT EXIST %REPO%\Plugins exit

SET MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe"

REM - Build core app using MSBuild 
cd %REPO%\Core
%MSBUILD% .\ToDoList_Core.sln /t:Build /p:Configuration="Unicode Debug" /m /v:normal

REM - Check for build errors
pause

REM - Build plugins using MSBuild 
SET BUILDPARAMS=/t:Build /p:Configuration=Debug /m /v:normal /noWarn:MSB3267;MSB3305;LNK4248;CS1762;LNK4221;MSB3026

REM - This is required to get the warning suppression to work
SET VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140\

cd %REPO%\Plugins
%MSBUILD% .\ToDoList_Plugins.sln %BUILDPARAMS%

REM - Check for build errors
pause