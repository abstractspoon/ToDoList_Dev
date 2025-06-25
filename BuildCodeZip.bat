ECHO OFF

REM - WE USE FULL PATHS THROUGHOUT - NO 'CD' WITH RELATIVE PATHS

PUSHD %~dp0

ECHO Zipping Code
ECHO ============
ECHO:

SET REPO=%CD%
SET PATHPZIP="D:\Tools\ProjectZip.exe"

ECHO REPO     = %REPO%
ECHO PATHPZIP = %PATHPZIP%
ECHO:

IF NOT EXIST %PATHPZIP% (
ECHO [41m Unable to locate ProjectZip.exe!![0m
PAUSE
EXIT
)

REM Options
REM ZP - Zip Path
REM ML - Make Lowercase
REM NM - No Message

ECHO Zipping %REPO%\Core\ToDoList_Core.sln
%PATHPZIP% %REPO%\Core\ToDoList_Core.sln /ZP %REPO%\..\ToDoList_Prev\9.2\ToDoList_src_Core.9.2._.zip /ML /NM

ECHO Zipping %REPO%\Plugins\ToDoList_Plugins.sln
%PATHPZIP% %REPO%\Plugins\ToDoList_Plugins.sln /ZP %REPO%\..\ToDoList_Prev\9.2\ToDoList_src_Plugins.9.2._.zip /ML /NM

ECHO:
ECHO [42m Zipping Code SUCCEEDED[0m
ECHO:

POPD
