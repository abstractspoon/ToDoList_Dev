## Notes ##

* I propose to re-purpose the task lock attribute (and column) for checking-in/out tasks when a tasklist has been 'added' to source control (Menu > Source Control > Add Tasklist to Source Control)
	* I will probably hide 'Menu > Edit > Lock Task Attributes'	and add new menu items for checking-in/out tasks to the Source Control menu
* Checking-out the entire tasklist will still be required for adding, deleting and moving tasks
* Files for each checked-out task will be stored in a folder having the same name as the tasklist but with the extension '.SourceControl'
* To prevent a possible conflict arising between someone who has a single task checked out and someone who checks out the entire tasklist and tries to modifies the same task, the user with the checked-out tasklist would need to actively monitor the '.SourceControl' folder (see above) so that tasks checked-out to other users could be marked 'locked' to prevent editing.

## Processes ##

#### Checking Out Tasks ####
	1. Create empty '<TaskID>' to prevent anyone else checking-out the task
		* If that fails then exit the check-out 
	2. Change task state to 'unlocked'
	3. Keep track of checked-out task

#### Checking In Tasks ####
	1. Save current task(s) state to '<TaskID>' file(s)
	2. Clear tasklist 'Modified' flag
	3. Check out entire tasklist
		* If that fails then exit the check-in
	4. Merge '<TaskID>' file(s) into tasklist 
	5. Check in tasklist
	6. Delete '<TaskID>'
	7. Stop tracking checked-in task(s)
	8. Change task(s) state to 'locked'

#### Loading Tasklists ####
	1. Load base tasklist 
	2. Mark all tasks as 'locked'
	3. Load any checked out tasks ('<TaskID>' files), merge them into the base tasklist, and mark them as 'unlocked'

#### Saving Tasklists ####
	1. Save checked-out task(s) to '<TaskID>' file(s)
	2. If the tasklist is checked out then save the entire tasklist.

#### Checking Out Tasklists ####
	1. To prevent a possible conflict the '.SourceControl' folder (see above) will need to be actively monitored for file creations and deletions so that tasks checked-out to other users can be marked 'locked' to prevent editing.

#### Checking In Tasklists ####
	1. No change




