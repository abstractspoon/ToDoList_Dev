1. Checking Out Tasks
	1.1 Check for existence of 'TaskID.*'
		1.1.1 If exists fail checkout
	1.2 Create 'TaskID.CompName'
	1.3 Store 'TaskID' in map of checked out tasks
	1.4 Change task state to 'unlocked'

2. Checking In Tasks
	1.1 Save 'TaskID.CompName'
	1.2 Checkout entire tasklist
		1.1.1 If that fails then exit
	1.3 Merge 'TaskID.CompName' into tasklist 
	1.4 Checkin tasklist
	1.5 Delete 'TaskID.CompName'
	1.6 Remove 'TaskID' from map of checked out tasks
	1.7 Change task state to 'locked'

3. Loading Tasklists
	1.1 Load base tasklist 
	1.2 Mark all tasks as 'locked'
	1.3 Check for '*.CompName'
		1.3.1 Checkin all found tasks
			1.3.1.1 If that fails then load and merge tasks, and mark tasks as 'unlocked'

4. Saving Tasklists




5. Checking Out Tasklists



6. Checking In Tasklists




