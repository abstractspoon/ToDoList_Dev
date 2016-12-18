REM - Copy Resources to own build folders

cd ..\..
echo %cd%

xcopy .\ToDoList_Resources\*.* .\ToDoList_Dev\ToDoList\Unicode_Debug\Resources\   /E /EXCLUDE:.\ToDoList_Dev\CopyResToBuildFolders_Exclude.txt /Y /D

xcopy .\ToDoList_Resources\*.* .\ToDoList_Dev\ToDoList\Unicode_Release\Resources\ /E /EXCLUDE:.\ToDoList_Dev\CopyResToBuildFolders_Exclude.txt /Y /D

REM - Copy Resources to latest release

xcopy .\ToDoList_Resources\*.* .\ToDoList_%1\ToDoList\Unicode_Debug\Resources\   /E /EXCLUDE:.\ToDoList_Dev\CopyResToBuildFolders_Exclude.txt /Y /D

xcopy .\ToDoList_Resources\*.* .\ToDoList_%1\ToDoList\Unicode_Release\Resources\ /E /EXCLUDE:.\ToDoList_Dev\CopyResToBuildFolders_Exclude.txt /Y /D
