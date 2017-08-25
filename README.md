# ToDoList_Plugins

Note: These steps have only been tested up to VS2010.

**Steps for building/running ImportExport solution for the first time**

1. Clone the repo cleanly
2. Build the SampleImpExp solution as-is (SampleImpExpCore should be the active project)
3. Close all other instances of ToDoList
4. Run the solution as-is which will display an error saying ToDoList.exe could not be found.
5. Copy just ToDoList.exe to the location in the error message. DO NOT MODIFY THE PATH IN THE PROJECT FILE.
6. Re-run the solution and ToDoList should now appear
7. 'Tools > Export Tasks' and 'Sample (.smp)' should appear in the 'Format' droplist
