@echo off
mkdir ..\ToDoList
mkdir ..\ToDoList\unicode_debug
copy unicode_debug\encryptdecrypt.dll ..\todolist\unicode_debug /y

if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: A tool returned an error code from "Performing Post-Build Event..."
exit 1
:VCEnd