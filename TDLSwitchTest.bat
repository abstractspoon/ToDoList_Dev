REM: "%1" is the path to ToDoList.exe 
REM: and is the first item on every line

REM: Start TDL and pause ---------------------------------------------
START "" "%1"
PAUSE

REM: Display help dialog ---------------------------------------------
REM: SWITCH_HELP1					
REM: "%1" -?
REM: SWITCH_HELP2					
REM: "%1" -h
REM: SWITCH_HELP3					
REM: "%1" -help

REM: Task  -----------------------------------------------------------
REM: SWITCH_INIFILE				
REM: "%1" -i

REM: Task  -----------------------------------------------------------
REM: SWITCH_TASKFILE				
REM: "%1" -f

REM: Task  -----------------------------------------------------------
REM: SWITCH_FORCEVISIBLE			
REM: "%1" -v

REM: Task  -----------------------------------------------------------
REM: SWITCH_NOPWORDPROMPT			
REM: "%1" -x

REM: Task  -----------------------------------------------------------
REM: SWITCH_LOGGING				
REM: "%1" -g

REM: Task  -----------------------------------------------------------
REM: SWITCH_IMPORT					
REM: "%1" -m

REM: Start Empty Instance --------------------------------------------
REM: SWITCH_STARTEMPTY				
REM: "%1" -e

REM: Create a new Tasklist -------------------------------------------
REM: SWITCH_COMMANDID				
"%1" -cmd 32780

REM: Task  -----------------------------------------------------------
REM: SWITCH_TASKLINK				
REM: "%1" -l

REM: Task  -----------------------------------------------------------
REM: SWITCH_ADDTODICT				
REM: "%1" -t

REM: Task  -----------------------------------------------------------
REM: SWITCH_TRANSUPPER				
REM: "%1" -u

REM: Task  -----------------------------------------------------------
REM: SWITCH_RESTART				
REM: "%1" -rst

REM: Task  -----------------------------------------------------------
REM: SWITCH_SAVEINTERMEDIATE		
REM: "%1" -si

REM: Task  -----------------------------------------------------------
REM: SWITCH_SAVEUIVISINTASKLIST	
REM: "%1" -z

REM: Task  -----------------------------------------------------------
REM: SWITCH_QUIET					
REM: "%1" -q

REM: New Task -----------------------------------------------------------
REM: SWITCH_NEWTASK				
"%1" -nt "nt (New Task)"

REM:   -----------------------------------------------------------
REM: SWITCH_SELECTTASKID			
REM: "%1" -tid

REM:   -----------------------------------------------------------
REM: SWITCH_PARENTID				
REM: "%1" -pid

REM:   -----------------------------------------------------------
REM: SWITCH_SIBLINGID				
REM: "%1" -bid

REM: Task Comments -------------------------------------------------------
REM: SWITCH_TASKCOMMENTS			
"%1" -nt "cm (Append to Task Comments)" -cm "New task comments"
"%1" -cm "This line was appended"

REM: Task  -----------------------------------------------------------
REM: SWITCH_TASKEXTID				
"%1" -nt "xid (Set External ID)"
"%1" -xid "0987654321"

REM: Task Category -----------------------------------------------------------
REM: SWITCH_TASKCATEGORY			
"%1" -nt "c (Task Category = c1,c2,c3)" -c c1 c2 c3
"%1" -nt "c (Task Category = c1 + c4 = c1,c4)" -c c1
"%1" -c +c4

REM: Task Status -----------------------------------------------------------
REM: SWITCH_TASKSTATUS				
"%1" -nt "s (Task Status = In Progress)" -s "In Progress"

REM: Task Priority -----------------------------------------------------------
REM: SWITCH_TASKPRIORITY			
"%1" -nt "p (Task Priority = 7)" -p 7

REM: Task Risk -----------------------------------------------------------
REM: SWITCH_TASKRISK				
"%1" -nt "r (Task Risk = 3)" -r 3

REM: Task Tag -----------------------------------------------------------
REM: SWITCH_TASKTAGS				
"%1" -nt "tg (Task Tag = t1,t2,t3)" -tg t1 t2 t3
"%1" -nt "tg (Task Tag = t1 + t4)" -tg t1
"%1" -tg +t4

REM: Task Cost -----------------------------------------------------------
REM: SWITCH_TASKCOST				
"%1" -nt "cs (Task Cost = 10)" -cs 10

"%1" -nt "cs (Task Cost = 10 + 4 = 14)" -cs 10
"%1" -cs +4

"%1" -nt "cs (Task Cost = 10 - 2 = 8)" -cs 10
"%1" -cs -2

REM: Task Dependency -----------------------------------------------------------
REM: SWITCH_TASKDEPENDENCY
REM: "%1" -nt "dp (Task Dependency = 10)" -dp 10

REM: Task Time Estimate -----------------------------------------------------------
REM: SWITCH_TASKTIMEEST			
"%1" -nt "te (Task Time Estimate)" -te

REM: Task Time Spent -----------------------------------------------------------
REM: SWITCH_TASKTIMESPENT			
"%1" -nt "ts (Task Time Spent)" -ts

REM: Task File Reference -----------------------------------------------------------
REM: SWITCH_TASKFILEREF			
"%1" -nt "fr (Task File Reference = Notepad.exe)" -fr "c:\windows\system32\notepad.exe"

REM: Task Allocated By -----------------------------------------------------------
REM: SWITCH_TASKALLOCBY			
"%1" -nt "ab (Task Allocated By = Mary)" -ab Mary

REM: Task Allocated To -----------------------------------------------------------
REM: SWITCH_TASKALLOCTO			
"%1" -nt "at (Task Allocated To = Dave, Bob)" -at Dave Bob
"%1" -nt "at (Task Allocated To = Dave + Pete)" -at Dave
"%1" -at +Pete

REM: Task Start Date -----------------------------------------------------------
REM: SWITCH_TASKSTARTDATE			
"%1" -nt "sd (Task Start Date = 9/8/7)" -sd "9/8/07"

REM: Task Start Time -----------------------------------------------------------
REM: SWITCH_TASKSTARTTIME			
"%1" -nt "st (Task Start Time)" -st

REM: Task Due Date -----------------------------------------------------------
REM: SWITCH_TASKDUEDATE			
"%1" -nt "dd (Task Due Date = 11/11/11)" -dd "11/11/11"

REM: Task Due Time -----------------------------------------------------------
REM: SWITCH_TASKDUETIME			
"%1" -nt "dt (Task Due Time)" -dt

REM: Task Completion Date -----------------------------------------------------------
REM: SWITCH_TASKDONEDATE			
"%1" -nt "cd (Task Completion Date = 17/4/21)" -cd "17/4/21"

REM: Task Completion Time -----------------------------------------------------------
REM: SWITCH_TASKDONETIME			
"%1" -nt "ct (Task Completion Time = 9PM)" -ct 

REM: Task Creation Date -----------------------------------------------------------
REM: SWITCH_TASKCREATEDATE			
"%1" -nt "md (Task Creation Date = 24/2/78)" -md "24/2/78"

REM: Task % Completion -----------------------------------------------------------
REM: SWITCH_TASKPERCENT			
"%1" -nt "pc (Task % Completion = 53)" -pc 53

"%1" -nt "pc (Task % Completion = 53 + 8 = 71)" -pc 53
"%1" -pc +8

"%1" -nt "pc (Task % Completion = 53 - 6 = 47)" -pc 53
"%1" -pc -6

REM: Task Version -----------------------------------------------------------
REM: SWITCH_TASKVERSION			
"%1" -nt "tv (Task Version = 0.1.2.3)" -tv "0.1.2.3"



