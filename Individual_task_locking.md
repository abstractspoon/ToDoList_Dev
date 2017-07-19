1. Checking Out Tasks
	1.1 Check for existence of 'TaskID.*'
		1.1.1 If exists fail checkout
	1.2 Create empty 'TaskID.UserName'
	1.3 Store 'TaskID' in list of checked out tasks
	1.4 Change task state to 'unlocked'

2. Checking In Tasks
	2.1 Save current task state to 'TaskID.CompName'
	2.2 Clear tasklist 'Modified' flag
	2.3 Check out entire tasklist
		2.3.1 If that fails then exit
	2.4 Merge 'TaskID.UserName' into tasklist 
	2.5 Check in tasklist
	2.6 Delete 'TaskID.UserName'
	2.7 Remove 'TaskID' from list of checked out tasks
	2.8 Change task state to 'locked'

3. Loading Tasklists
	3.1 Load base tasklist 
	3.2 Mark all tasks as 'locked'
	3.3 Check for '*.UserName'
		3.3.1 Check in all found tasks
			3.3.1.1 If that fails then load and merge tasks, and mark tasks as 'unlocked'

4. Saving Tasklists
	4.1 Save checked out task to 'TaskID.UserName'  
	4.2 If the tasklist is checked out then save the entire tasklist.

5. Checking Out Tasklists (required for adding, deleting, moving tasks)
	5.1 No change

6. Checking In Tasklists
	6.1 No change




