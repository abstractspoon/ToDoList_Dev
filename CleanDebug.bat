title ToDoList_Dev Debug Clean

ECHO OFF
CLS

ECHO ToDoList_Dev Debug Clean
ECHO ========================
ECHO:

PUSHD %~dp0
SET REPO=%CD%
ECHO REPO = %REPO%
ECHO:

IF NOT EXIST %REPO%\Core EXIT
IF NOT EXIST %REPO%\Plugins EXIT

REM - Use MSBuild for both core and plugins
SET MSBUILD="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"

ECHO MSBUILD  = %MSBUILD%
ECHO:

IF NOT EXIST %MSBUILD% (
ECHO [41m Unable to locate MSBuild.exe!![0m
ECHO:
PAUSE
EXIT
)

ECHO Cleaning ToDoList Core 
ECHO ======================
ECHO:

SET SOLUTION=%REPO%\Core\ToDoList_Core.sln

ECHO SOLUTION = %SOLUTION%
ECHO:

%MSBUILD% %SOLUTION% /t:Clean /p:Configuration="Unicode Debug" /m /v:normal
ECHO:

ECHO Cleaning ToDoList Plugins
ECHO =========================
ECHO:

SET SOLUTION=%REPO%\Plugins\ToDoList_Plugins.sln

ECHO SOLUTION = %SOLUTION%
ECHO:

%MSBUILD% %SOLUTION% /t:Clean /p:Configuration=Debug /m /v:normal

PAUSE
POPD
