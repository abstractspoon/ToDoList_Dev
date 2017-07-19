1. Checking Out Tasks
	1. Check for existence of 'TaskID.*'
		1. If exists fail checkout
	2. Create empty 'TaskID.UserName'
	3. Store 'TaskID' in list of checked out tasks
	4. Change task state to 'unlocked'

2. Checking In Tasks
	1. Save current task state to 'TaskID.CompName'
	2. Clear tasklist 'Modified' flag
	3. Check out entire tasklist
		1. If that fails then exit
	4. Merge 'TaskID.UserName' into tasklist 
	5. Check in tasklist
	6. Delete 'TaskID.UserName'
	7. Remove 'TaskID' from list of checked out tasks
	8. Change task state to 'locked'

3. Loading Tasklists
	1. Load base tasklist 
	2. Mark all tasks as 'locked'
	3. Check for '*.UserName' files
		1. Check in all found tasks
			1. If that fails then load and merge tasks, and mark tasks as 'unlocked'

4. Saving Tasklists
	1. Save checked out task to 'TaskID.UserName'  
	2. If the tasklist is checked out then save the entire tasklist.

5. Checking Out Tasklists (required for adding, deleting, moving tasks)
	1. No change

6. Checking In Tasklists
	1. No change




