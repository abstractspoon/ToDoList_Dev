REM - Copy Resources to own build folders

xcopy ..\ToDoList_Resources\*.* .\ToDoList\Unicode_Debug\Resources\   /E /EXCLUDE:.\CopyResToBuildFolders_Exclude.txt /Y /D

xcopy ..\ToDoList_Resources\*.* .\ToDoList\Unicode_Release\Resources\ /E /EXCLUDE:.\CopyResToBuildFolders_Exclude.txt /Y /D

REM - Copy Resources to latest release

xcopy ..\ToDoList_Resources\*.* D:\ToDoList_Source_Versions\ToDoList_src.%1\ToDoList\Unicode_Debug\Resources\   /E /EXCLUDE:.\CopyResToBuildFolders_Exclude.txt /Y /D

xcopy ..\ToDoList_Resources\*.* D:\ToDoList_Source_Versions\ToDoList_src.%1\ToDoList\Unicode_Release\Resources\ /E /EXCLUDE:.\CopyResToBuildFolders_Exclude.txt /Y /D
