## Notes ##

* I propose to re-purpose the task lock attribute (and column) for checking-in/out tasks when a tasklist has been 'added' to source control (Menu > Source Control > Add Tasklist to Source Control)
	* I will probably hide 'Menu > Edit > Lock Task Attributes' and add new menu items for checking-in/out tasks to the Source Control menu
		* 'Check-out Selected Task'
		* 'Check-in Selected Task'
		* 'Check-in All Checked-out Tasks'
* Checking-out the entire tasklist will still be required for adding, deleting and moving tasks
	* Having the tasklist checked out will still require individual tasks to be checked-out in order to modify their content.
* Files for each checked-out task will be stored in a folder having the name '[Tasklist].ssc' (aka 'Simple Source Control').
* Change CToDoCtrl::CanEditSelectedTask() to take a TDC_ATTRIBUTE argument so we can distinguish between Attribute edits, Lock edits and Create/Move/Delete operations

## Processes ##

#### Checking Out a Tasklist ####
1. Attempt to create an empty file having the name '[TaskList].tsc' using the 'CREATE_NEW' file creation flag
	* If this fails it means someone else has checked-out the tasklist so fail the check-out and notify the user
2. Replace this empty file with the header of the tasklist updated to include the check-out name
3. No need to re-save the tasklist with the check-out name
4. Mark the tasklist as checked-out as before

#### Checking In a Tasklist ####
1. Check tasklist in as before
2. No need to re-save the tasklist without the check-out name
3. Delete '[TaskList].tsc'

#### Checking Out a Single Task ####
1. Attempt to create an empty file having the name '[TaskID].tsc' using the 'CREATE_NEW' file creation flag
	* If this fails it means someone else has checked-out the task so fail the check-out and notify the user
2. Replace this empty file with the existing task state, saved as as a minimal tasklist ie. Just the task itself and the check-out name
3. Change task state to 'unlocked' in the UI
4. Keep a list of checked-out tasks per tasklist

#### Checking In a Task ####
1. Save current task state to '[TaskID].tsc' file(s) as a minimal tasklist
2. Clear tasklist 'Modified' flag
3. Check out entire tasklist
	* If that fails then fail the check-in and notify the user
4. Merge '[TaskID].tsc' file(s) into tasklist 
5. Check in tasklist if it wasn't previously checked out
    * Delete '[TaskID].tsc'
7. Stop tracking checked-in task(s)
8. Change task(s) state to 'locked'

#### Loading Tasklists ####
1. Load base tasklist 
2. Mark all tasks as 'locked'
3. Load any checked out tasks ('[TaskID].tsc' files) belonging to 'us', merge them into the base tasklist, and mark them as 'unlocked'

#### Saving Tasklists ####
1. Save all checked-out task(s) to '[TaskID].tsc' file(s)
2. If the tasklist is checked-out then save the entire tasklist.

