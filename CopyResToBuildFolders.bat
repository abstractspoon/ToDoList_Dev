ECHO OFF

pushd %~dp0
set REPO=%CD%
ECHO REPO=%REPO%

set RESREPO=%REPO%\..\ToDoList_Resources
ECHO RESREPO=%RESREPO%

set TESTREPO=%REPO%\..\ToDoList_Test
ECHO TESTREPO=%TESTREPO%

REM - Copy to Core
xcopy %RESREPO%\*.* %REPO%\Core\ToDoList\Unicode_Debug\Resources\   /E /EXCLUDE:%REPO%\CopyResToBuildFolders_Exclude.txt /Y 
xcopy %RESREPO%\*.* %REPO%\Core\ToDoList\Unicode_Release\Resources\   /E /EXCLUDE:%REPO%\CopyResToBuildFolders_Exclude.txt /Y 

xcopy %TESTREPO%\Tasklists\*.* %REPO%\Core\ToDoList\Unicode_Debug\Resources\Examples\  /Y   
xcopy %TESTREPO%\Tasklists\*.* %REPO%\Core\ToDoList\Unicode_Release\Resources\Examples\  /Y   

REM - Copy Resources to Plugins
xcopy %RESREPO%\*.* %REPO%\Plugins\Debug\Resources\   /E /EXCLUDE:%REPO%\CopyResToBuildFolders_Exclude.txt /Y 
xcopy %RESREPO%\*.* %REPO%\Plugins\Release\Resources\ /E /EXCLUDE:%REPO%\CopyResToBuildFolders_Exclude.txt /Y 

xcopy %TESTREPO%\Tasklists\*.* %REPO%\Plugins\Debug\Resources\Examples\  /Y   
xcopy %TESTREPO%\Tasklists\*.* %REPO%\Plugins\Release\Resources\Examples\  /Y   

popd
pause
