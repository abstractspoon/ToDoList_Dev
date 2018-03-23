# ToDoList_Plugins

Note: These steps have only been tested up to VS2010.

**Steps for building/running the ToDoList_Plugins solution**

1. Clone the repo cleanly
   Note: You will also need to clone 'ToDoList_Dev' alongside as the plugins depend on some shared header files
2. Build the solution as-is ('PluginsHelper' should be the active project)
    * Note: The first time you build the solution, projects dependent on 'PluginHelpers' may fail to find the dll but just build again (not rebuild) and this second pass should succeed.
3. Close all instances of ToDoList
4. Run the solution as-is which will display an error saying ToDoList.exe could not be found
5. Copy just ToDoList.exe from your latest build to the location in the error message. DO NOT MODIFY THE PATH IN THE PROJECT FILE
6. Re-run the solution and ToDoList should now appear
7. Plugins should now be accessible (for debugging) in the following locations:
    1. 'Tools > Export Tasks' for MarkdownImpExp and SampleImpExp
    2. 'Comments Type' for MDContentControl
    3. 'Task View Tab Bar' for DayViewUIExtension, SampleUIExtension, StatsUIExtension and WordCloudUIExtension
