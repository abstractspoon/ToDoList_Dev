ECHO OFF

pushd %~dp0
set REPO=%CD%
ECHO REPO=%REPO%

set RESREPO=%REPO%\..\ToDoList_Resources
ECHO RESREPO=%RESREPO%

set TESTREPO=%REPO%\..\ToDoList_Test
ECHO TESTREPO=%TESTREPO%

set EXCLUDEFILE=%REPO%\CopyResToBuildFolders_Exclude.txt

REM - Copy to Core
xcopy %RESREPO%\*.* %REPO%\Core\ToDoList\Unicode_Debug\Resources\  /E /EXCLUDE:%EXCLUDEFILE% /Y 
xcopy %RESREPO%\*.* %REPO%\Core\ToDoList\Unicode_Release\Resources\  /E /EXCLUDE:%EXCLUDEFILE% /Y 

xcopy %RESREPO%\*.jpg %REPO%\Core\ToDoList\Unicode_Debug\Resources\Icons\  /s /Y 
xcopy %RESREPO%\*.jpg %REPO%\Core\ToDoList\Unicode_Release\Resources\Icon\  /s /Y 

xcopy %TESTREPO%\Tasklists\*.* %REPO%\Core\ToDoList\Unicode_Debug\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y 
xcopy %TESTREPO%\Tasklists\*.* %REPO%\Core\ToDoList\Unicode_Release\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y  

xcopy %TESTREPO%\Checklists\*.* %REPO%\Core\ToDoList\Unicode_Debug\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y  
xcopy %TESTREPO%\Checklists\*.* %REPO%\Core\ToDoList\Unicode_Release\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y   

xcopy "%TESTREPO%\Sample Languages\*.*" %REPO%\Core\ToDoList\Unicode_Debug\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y   
xcopy "%TESTREPO%\Sample Languages\*.*" %REPO%\Core\ToDoList\Unicode_Release\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y   

REM - Copy Resources to Plugins
xcopy %RESREPO%\*.* %REPO%\Plugins\Debug\Resources\  /E /EXCLUDE:%EXCLUDEFILE% /Y 
xcopy %RESREPO%\*.* %REPO%\Plugins\Release\Resources\  /E /EXCLUDE:%EXCLUDEFILE% /Y 

xcopy %TESTREPO%\Tasklists\*.* %REPO%\Plugins\Debug\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y  
xcopy %TESTREPO%\Tasklists\*.* %REPO%\Plugins\Release\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y   

xcopy %TESTREPO%\Checklists\*.* %REPO%\Plugins\Debug\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y   
xcopy %TESTREPO%\Checklists\*.* %REPO%\Plugins\Release\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y   

xcopy "%TESTREPO%\Sample Languages\*.*" %REPO%\Plugins\Debug\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y 
xcopy "%TESTREPO%\Sample Languages\*.*" %REPO%\Plugins\Release\Resources\Examples\  /E /EXCLUDE:%EXCLUDEFILE% /Y  

popd
pause
