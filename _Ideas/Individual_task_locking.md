## Notes ##

* I propose to re-purpose the task lock attribute (and column) for checking-in/out tasks when a tasklist has been 'added' to source control (Menu > Source Control > Add Tasklist to Source Control)
	* I will probably hide 'Menu > Edit > Lock Task Attributes'	and add new menu items for checking-in/out tasks to the Source Control menu
* Checking-out the entire tasklist will still be required for adding, deleting and moving tasks
* Files for each checked-out task will be stored in a folder having the same name as the tasklist but with the extension '.SourceControl'

## Problems ##

* I can see a possible conflict arising when someone has a task checked out and then someone else checks out the entire tasklist and modifies the same task. In such a case the person with the single task checked out would always 'win' because before they could merge their changes into the tasklist the other would need to have already checked-in the tasklist. Perhaps in fact this could be avoided by ensuring that the individual checked-out tasks would be locked against changes by the person who has checked out the whole tasklist. This would then mean that the user with the checked-out tasklist would need to actively monitor the '.SourceControl' folder (see above) so that the lock states could be kept synchronised.

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
	1. No change

#### Checking In Tasklists ####
	1. No change




