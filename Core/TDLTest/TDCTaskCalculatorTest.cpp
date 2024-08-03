// ToDoCtrlDataTest.cpp: implementation of the CTDCTaskCalculatorTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCTaskCalculatorTest.h"

#include "..\todolist\todoctrldata.h"
#include "..\todolist\todoctrldatautils.h"
#include "..\todolist\tdcstruct.h"
#include "..\todolist\todoitem.h"
#include "..\todolist\taskfile.h"

#include "..\shared\datehelper.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskCalculatorTest::CTDCTaskCalculatorTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(utils)
{
}

CTDCTaskCalculatorTest::~CTDCTaskCalculatorTest()
{

}

TESTRESULT CTDCTaskCalculatorTest::Run()
{
	ClearTotals();

	Test();

	return GetTotals();
}

void CTDCTaskCalculatorTest::Test()
{
	BeginTest(_T("Test"));

	// Populate data model
	CTaskFile tasks;

	// Task1
	//   |
	//   |_ Task2
	//   |
	//   |_ Task3
	//        |
	//        |_ Task4
	
	CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);
	PopulateData(data);

	CTDCTaskCalculator calc(data);

	// Start Date ------------------------------------------------
	{
		// Assigned
		{
			m_aStyles.RemoveAll();

			ExpectEQ(calc.GetTaskStartDate(1), 45000.0);
			ExpectEQ(calc.GetTaskStartDate(2), 45001.0);
			ExpectEQ(calc.GetTaskStartDate(3), 45002.0);
			ExpectEQ(calc.GetTaskStartDate(4), 0.0); // completed task
		}

		// Earliest
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_USEEARLIESTSTARTDATE] = TRUE;

			ExpectEQ(calc.GetTaskStartDate(1), 45000.0);
			ExpectEQ(calc.GetTaskStartDate(2), 45001.0);
			ExpectEQ(calc.GetTaskStartDate(3), 45002.0);
			ExpectEQ(calc.GetTaskStartDate(4), 0.0); // completed task
		}

		// Latest
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_USELATESTSTARTDATE] = TRUE;

			ExpectEQ(calc.GetTaskStartDate(1), 45002.0);
			ExpectEQ(calc.GetTaskStartDate(2), 45001.0);
			ExpectEQ(calc.GetTaskStartDate(3), 45002.0);
			ExpectEQ(calc.GetTaskStartDate(4), 0.0); // completed task
		}
	}


	// Due Date -------------------------------------------------
	{
		// Assigned
		{
			m_aStyles.RemoveAll();

			ExpectEQ(calc.GetTaskDueDate(1), 45001.0);
			ExpectEQ(calc.GetTaskDueDate(2), 45002.0);
			ExpectEQ(calc.GetTaskDueDate(3), 45003.0);
			ExpectEQ(calc.GetTaskDueDate(4), 0.0); // completed task
		}

		// Earliest
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_USEEARLIESTDUEDATE] = TRUE;

			ExpectEQ(calc.GetTaskDueDate(1), 45001.0);
			ExpectEQ(calc.GetTaskDueDate(2), 45002.0);
			ExpectEQ(calc.GetTaskDueDate(3), 45003.0);
			ExpectEQ(calc.GetTaskDueDate(4), 0.0); // completed task
		}

		// Latest
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_USELATESTDUEDATE] = TRUE;

			ExpectEQ(calc.GetTaskDueDate(1), 45003.0);
			ExpectEQ(calc.GetTaskDueDate(2), 45002.0);
			ExpectEQ(calc.GetTaskDueDate(3), 45003.0);
			ExpectEQ(calc.GetTaskDueDate(4), 0.0); // completed task
		}
	}


	// Last Modified Date/User ---------------------------------
	{
		// Assigned
		{
			m_aStyles.RemoveAll();

			ExpectEQ(calc.GetTaskLastModifiedDate(1), 45002.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(2), 45003.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(3), 45004.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(4), 45005.0);

			ExpectEQ(calc.GetTaskLastModifiedBy(1), _T("User1"));
			ExpectEQ(calc.GetTaskLastModifiedBy(2), _T("User2"));
			ExpectEQ(calc.GetTaskLastModifiedBy(3), _T("User3"));
			ExpectEQ(calc.GetTaskLastModifiedBy(4), _T("User4"));
		}

		// Latest
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_USELATESTLASTMODIFIED] = TRUE;

			ExpectEQ(calc.GetTaskLastModifiedDate(1), 45005.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(2), 45003.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(3), 45005.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(4), 45005.0);

			ExpectEQ(calc.GetTaskLastModifiedBy(1), _T("User4"));
			ExpectEQ(calc.GetTaskLastModifiedBy(2), _T("User2"));
			ExpectEQ(calc.GetTaskLastModifiedBy(3), _T("User4"));
			ExpectEQ(calc.GetTaskLastModifiedBy(4), _T("User4"));
		}
	}

	// Priority ------------------------------------------------
	{
		// Because all our assigned dates are much earlier than 'now' none
		// of out tasks are overdue so we don't both with that extra check,
		// except for the 'no modifiers' check just to prove the point
		const BOOL NO_OVERDUE_CHECK = FALSE;

		// Assigned
		{
			// No modifiers
			{
				m_aStyles.RemoveAll();

				ExpectEQ(calc.GetTaskPriority(1, FALSE), 5);
				ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
				ExpectEQ(calc.GetTaskPriority(3, FALSE), 7);
				ExpectEQ(calc.GetTaskPriority(4, FALSE), 8);

				ExpectEQ(calc.GetTaskPriority(1, TRUE), 5);
				ExpectEQ(calc.GetTaskPriority(2, TRUE), 6);
				ExpectEQ(calc.GetTaskPriority(3, TRUE), 7);
				ExpectEQ(calc.GetTaskPriority(4, TRUE), 8);
			}

			// Done have lowest priority
			{
				// DON'T treat tasks with completed subtasks as completed
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;

					ExpectEQ(calc.GetTaskPriority(1, NO_OVERDUE_CHECK), 5);
					ExpectEQ(calc.GetTaskPriority(2, NO_OVERDUE_CHECK), 6);
					ExpectEQ(calc.GetTaskPriority(3, NO_OVERDUE_CHECK), 7);
					ExpectEQ(calc.GetTaskPriority(4, NO_OVERDUE_CHECK), 0); // completed task
				}

				// DO treat tasks with completed subtasks as completed
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;

					ExpectEQ(calc.GetTaskPriority(1, NO_OVERDUE_CHECK), 5);
					ExpectEQ(calc.GetTaskPriority(2, NO_OVERDUE_CHECK), 6);
					ExpectEQ(calc.GetTaskPriority(3, NO_OVERDUE_CHECK), 0); // implicitly completed
					ExpectEQ(calc.GetTaskPriority(4, NO_OVERDUE_CHECK), 0); // completed task
				}
			}

		}

		// Highest
		{
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
			}


			// TODO




			m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;
			m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = TRUE;
			m_aStyles[TDCS_DUEHAVEHIGHESTPRIORITY] = TRUE;

			// TODO
		}
	}


	// Risk ---------------------------------------------------
	{
		// Assigned
		{
			// No modifiers
			{
				m_aStyles.RemoveAll();

				ExpectEQ(calc.GetTaskRisk(1), 6);
				ExpectEQ(calc.GetTaskRisk(2), 7);
				ExpectEQ(calc.GetTaskRisk(3), 8);
				ExpectEQ(calc.GetTaskRisk(4), 9);
			}

			// Done have lowest risk
			{
				// DON'T treat tasks with completed subtasks as completed
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;

					ExpectEQ(calc.GetTaskRisk(1), 6);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
				}

				// DO treat tasks with completed subtasks as completed
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;

					ExpectEQ(calc.GetTaskRisk(1), 6);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 0); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
				}
			}
		}

		// Highest
		{
			// DON'T treat tasks with completed subtasks as completed
			{
				// Done included and lowest
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
					m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;

					ExpectEQ(calc.GetTaskRisk(1), 8);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
				}

				// Done included but NOT lowest
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
					m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = FALSE;

					ExpectEQ(calc.GetTaskRisk(1), 9);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 9);
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
				}

				// Done NOT included
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
					m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = FALSE;
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;

					ExpectEQ(calc.GetTaskRisk(1), 8);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
				}

				// Done NOT included and NOT lowest
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
					m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = FALSE;
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = FALSE;

					ExpectEQ(calc.GetTaskRisk(1), 8);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8);
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
				}
			}

			// DO treat tasks with completed subtasks as completed
			{
				// Done included and lowest
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
					m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;

					ExpectEQ(calc.GetTaskRisk(1), 7);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 0); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
				}

				// Done included but NOT lowest
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
					m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = FALSE;

					ExpectEQ(calc.GetTaskRisk(1), 9);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 9); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
				}

				// Done NOT included and lowest
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
					m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = FALSE;
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;

					ExpectEQ(calc.GetTaskRisk(1), 7);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 0); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
				}

				// Done NOT included and NOT lowest
				{
					m_aStyles.RemoveAll();
					m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
					m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
					m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = FALSE;
					m_aStyles[TDCS_DONEHAVELOWESTRISK] = FALSE;

					ExpectEQ(calc.GetTaskRisk(1), 7);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
				}
			}
		}
	}

	// Percent Done -------------------------------------------
	{
		// Assigned
		{
			m_aStyles.RemoveAll();

			ExpectEQ(calc.GetTaskPercentDone(1), 10);
			ExpectEQ(calc.GetTaskPercentDone(2), 20);
			ExpectEQ(calc.GetTaskPercentDone(3), 30);
			ExpectEQ(calc.GetTaskPercentDone(4), 100); // completed task
		}

		// Time Spent/Estimate
		{
			// Note: To keep the manual checks comprehensible we use only 
			//       the Time Estimate values assigned in PopulateData

			// No parental contribution
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_AUTOCALCPERCENTDONE] = TRUE;
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;

				ExpectEQ(calc.GetTaskPercentDone(1), (int)(100 * (0.0 + 50.0 + (0.0 + 70.0)) / (0.0 + 40.0 + (0.0 + 60.0))));	// parent
				ExpectEQ(calc.GetTaskPercentDone(2), (int)(100 * (50.0 / 40.0)));
				ExpectEQ(calc.GetTaskPercentDone(3), (int)(100 * (0.0 + 70.0) / (0.0 + 60.0)));									// parent
				ExpectEQ(calc.GetTaskPercentDone(4), (int)(100 * (70.0 / 60.0)));												// completed task
			}

			// Allow parent time tracking
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_AUTOCALCPERCENTDONE] = TRUE;
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;

				ExpectEQ(calc.GetTaskPercentDone(1), (int)(100 * (40.0 + 50.0 + (60.0 + 70.0)) / (30.0 + 40.0 + (50.0 + 60.0))));
				ExpectEQ(calc.GetTaskPercentDone(2), (int)(100 * (50.0 / 40.0)));
				ExpectEQ(calc.GetTaskPercentDone(3), (int)(100 * (60.0 + 70.0) / (50.0 + 60.0)));
				ExpectEQ(calc.GetTaskPercentDone(4), (int)(100 * (70.0 / 60.0))); // completed task
			}
		}

		// Average
		{
			m_aStyles.RemoveAll();

			m_aStyles[TDCS_AVERAGEPERCENTSUBCOMPLETION] = FALSE;
			m_aStyles[TDCS_INCLUDEDONEINAVERAGECALC] = FALSE;
			m_aStyles[TDCS_WEIGHTPERCENTCALCBYNUMSUB] = FALSE;
			// TODO
		}
	}

	// Cost ----------------------------------------------------
	{
		m_aStyles.RemoveAll();

		ExpectEQ(calc.GetTaskCost(1), 140.0);
		ExpectEQ(calc.GetTaskCost(2), 30.0);
		ExpectEQ(calc.GetTaskCost(3), 90.0);
		ExpectEQ(calc.GetTaskCost(4), 50.0); // completed task
	}
	
	// Time Remaining --------------------------------------------
	{
		TDC_UNITS nUnits = TDCU_NULL;

		// Using Due Date (Now - Due Date)
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_CALCREMAININGTIMEBYDUEDATE] = TRUE;

			// Note: +1 because due date has no time component -> end of day
			ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), ((45001.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), ((45002.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), ((45003.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), 0.0); // completed task
			ExpectEQ(nUnits, TDCU_NULL);
		}

		// Using Time Spent (Time Estimate - Time Spent)
		{
			// Note: To keep the manual checks comprehensible we use only 
			//       the Time Estimate values assigned in PopulateData

			// No parental contributions
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_CALCREMAININGTIMEBYSPENT] = TRUE;
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;

				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (0.0 + 40.0 + (0.0 + 60.0)) - (0.0 + 50.0 + (0.0 + 70.0))); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 - 50.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (0.0 + 60.0) - (0.0 + 70.0)); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (60.0 - 70.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
			}

			// Parental contributions
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_CALCREMAININGTIMEBYSPENT] = TRUE;
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;

				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (30.0 + 40.0 + (50.0 + 60.0)) - (40.0 + 50.0 + (60.0 + 70.0)));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 - 50.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (50.0 + 60.0) - (60.0 + 70.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (60.0 - 70.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
			}
		}

		// Using % Completion (Time Estimate * (100 - %) / 100)
		{
			// Note: To keep the manual checks comprehensible we 
			//       use only the % values assigned in PopulateData

			// No parental contributions
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_CALCREMAININGTIMEBYPERCENT] = TRUE;
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;

				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (0.0 + (40.0 * 0.8) + (0.0 + 0.0))); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 * 0.8));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (0.0 + 0.0)); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (0.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
			}

			// Parental contributions
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_CALCREMAININGTIMEBYPERCENT] = TRUE;
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;

				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), ((30.0 * 0.9) + (40.0 * 0.8) + ((50.0 * 0.7) + 0.0)));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 * 0.8));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), ((50.0 * 0.7) + 0.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (0.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
			}
		}

		// TODO
	}

	// Time Estimate --------------------------------------------
	{
		// No parental contributions
		{
			// NOT adjusting by % completion
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;
				m_aStyles[TDCS_USEPERCENTDONEINTIMEEST] = FALSE;

				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (0.0 + 40.0 + 0.0 + 60.0)); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (0.0 + 60.0)); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0)); // completed task
			}

			// Adjusting by % completion
			//
			// Note: To keep the manual checks comprehensible we 
			//       use only the % values assigned in PopulateData
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;
				m_aStyles[TDCS_USEPERCENTDONEINTIMEEST] = TRUE;

				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (0.0 + (40.0 * 0.8) + (0.0 + (60.0 * 0.0)))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0 * 0.8));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (0.0)); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0 * 0.0)); // completed task
			}
		}

		// Parental contributions
		{
			// NOT adjusting by % completion
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;
				m_aStyles[TDCS_USEPERCENTDONEINTIMEEST] = FALSE;

				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (30.0 + 40.0 + (50.0 + 60.0)));
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (50.0 + 60.0));
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0)); // completed task
			}

			// Adjusting by % completion
			//
			// Note: To keep the manual checks comprehensible we 
			//       use only the % values assigned in PopulateData
			{
				m_aStyles.RemoveAll();
				m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;
				m_aStyles[TDCS_USEPERCENTDONEINTIMEEST] = TRUE;

				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (30.0 * 0.9) + (40.0 * 0.8) + ((50.0 * 0.7) + (60.0 * 0.0)));
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0 * 0.8));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (50.0 * 0.7) + (60.0 * 0.0));
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0 * 0.0)); // completed task
			}
		}

	}
	
	// Time Spent -----------------------------------------------
	{
		// No parental contributions
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;

			ExpectEQ(calc.GetTaskTimeSpent(1, TDCU_DAYS), (0.0 + 50.0 + (0.0 + 70.0))); // parent task
			ExpectEQ(calc.GetTaskTimeSpent(2, TDCU_DAYS), (50.0));
			ExpectEQ(calc.GetTaskTimeSpent(3, TDCU_DAYS), (0.0 + 70.0)); // parent task
			ExpectEQ(calc.GetTaskTimeSpent(4, TDCU_DAYS), (70.0)); // completed task
		}

		// Parental contributions
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;

			ExpectEQ(calc.GetTaskTimeSpent(1, TDCU_DAYS), (40.0 + 50.0 + (60.0 + 70.0)));
			ExpectEQ(calc.GetTaskTimeSpent(2, TDCU_DAYS), (50.0));
			ExpectEQ(calc.GetTaskTimeSpent(3, TDCU_DAYS), (60.0 + 70.0));
			ExpectEQ(calc.GetTaskTimeSpent(4, TDCU_DAYS), (70.0)); // completed task
		}
	}
		
	// Flagging ------------------------------------------------
	{
		// Assigned
		{
			m_aStyles.RemoveAll();

			ExpectFalse(calc.IsTaskFlagged(1));
			ExpectFalse(calc.IsTaskFlagged(2));
			ExpectFalse(calc.IsTaskFlagged(3));
			ExpectTrue(calc.IsTaskFlagged(4));
		}

		// Inherited
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_TASKINHERITSSUBTASKFLAGS] = TRUE;

			ExpectTrue(calc.IsTaskFlagged(1));
			ExpectFalse(calc.IsTaskFlagged(2));
			ExpectTrue(calc.IsTaskFlagged(3));
			ExpectTrue(calc.IsTaskFlagged(4));
		}
	}

	// Locking --------------------------------------------------
	{
		// Assigned
		{
			m_aStyles.RemoveAll();

			ExpectTrue(calc.IsTaskLocked(1));
			ExpectFalse(calc.IsTaskLocked(2));
			ExpectFalse(calc.IsTaskLocked(3));
			ExpectFalse(calc.IsTaskLocked(4));
		}

		// Inherited
		{
			m_aStyles.RemoveAll();
			m_aStyles[TDCS_SUBTASKSINHERITLOCK] = TRUE;

			ExpectTrue(calc.IsTaskLocked(1));
			ExpectTrue(calc.IsTaskLocked(2));
			ExpectTrue(calc.IsTaskLocked(3));
			ExpectTrue(calc.IsTaskLocked(4));
		}
	}

	EndTest();
}


void CTDCTaskCalculatorTest::PopulateData(CToDoCtrlData& data) const
{
	// Task1
	//   |
	//   |_ Task2
	//   |
	//   |_ Task3
	//        |
	//        |_ Task4

	CTaskFile tasks;

	HTASKITEM hTask1 = tasks.NewTask(_T("Task1"), NULL, 1, 0);
	HTASKITEM hTask2 = tasks.NewTask(_T("Task2"), hTask1, 2, 1);
	HTASKITEM hTask3 = tasks.NewTask(_T("Task3"), hTask1, 3, 1);
	HTASKITEM hTask4 = tasks.NewTask(_T("Task4"), hTask3, 4, 3);

	// Task1
	{
		tasks.SetTaskStartDate(hTask1, COleDateTime(45000.0));
		tasks.SetTaskDueDate(hTask1, COleDateTime(45001.0));
		tasks.SetTaskLastModified(hTask1, COleDateTime(45002.0), _T("User1"));

		tasks.SetTaskPriority(hTask1, 5);
		tasks.SetTaskRisk(hTask1, 6);
		tasks.SetTaskPercentDone(hTask1, 10);

		tasks.SetTaskCost(hTask1, 20);
		tasks.SetTaskTimeEstimate(hTask1, 30, TDCU_DAYS);
		tasks.SetTaskTimeSpent(hTask1, 40, TDCU_DAYS);

		tasks.SetTaskFlag(hTask1, FALSE);
		tasks.SetTaskLock(hTask1, TRUE); // The only locked task
	}

	// Task2
	{
		tasks.SetTaskStartDate(hTask2, COleDateTime(45001.0));
		tasks.SetTaskDueDate(hTask2, COleDateTime(45002.0));
		tasks.SetTaskLastModified(hTask2, COleDateTime(45003.0), _T("User2"));

		tasks.SetTaskPriority(hTask2, 6);
		tasks.SetTaskRisk(hTask2, 7);
		tasks.SetTaskPercentDone(hTask2, 20);

		tasks.SetTaskCost(hTask2, 30);
		tasks.SetTaskTimeEstimate(hTask2, 40, TDCU_DAYS);
		tasks.SetTaskTimeSpent(hTask2, 50, TDCU_DAYS);

		tasks.SetTaskFlag(hTask2, FALSE);
		tasks.SetTaskLock(hTask2, FALSE);
	}

	// Task3
	{
		tasks.SetTaskStartDate(hTask3, COleDateTime(45002.0));
		tasks.SetTaskDueDate(hTask3, COleDateTime(45003.0));
		tasks.SetTaskLastModified(hTask3, COleDateTime(45004.0), _T("User3"));

		tasks.SetTaskPriority(hTask3, 7);
		tasks.SetTaskRisk(hTask3, 8);
		tasks.SetTaskPercentDone(hTask3, 30);

		tasks.SetTaskCost(hTask3, 40);
		tasks.SetTaskTimeEstimate(hTask3, 50, TDCU_DAYS);
		tasks.SetTaskTimeSpent(hTask3, 60, TDCU_DAYS);

		tasks.SetTaskFlag(hTask3, FALSE);
		tasks.SetTaskLock(hTask3, FALSE);
	}

	// Task4
	{
		tasks.SetTaskStartDate(hTask4, COleDateTime(45003.0));
		tasks.SetTaskDueDate(hTask4, COleDateTime(45004.0));
		tasks.SetTaskDoneDate(hTask4, COleDateTime(45010.0)); // The only completed task
		tasks.SetTaskLastModified(hTask4, COleDateTime(45005.0), _T("User4"));

		tasks.SetTaskPriority(hTask4, 8);
		tasks.SetTaskRisk(hTask4, 9);
		tasks.SetTaskPercentDone(hTask4, 40);

		tasks.SetTaskCost(hTask4, 50);
		tasks.SetTaskTimeEstimate(hTask4, 60, TDCU_DAYS);
		tasks.SetTaskTimeSpent(hTask4, 70, TDCU_DAYS);

		tasks.SetTaskFlag(hTask4, TRUE); // The only flagged task
		tasks.SetTaskLock(hTask4, FALSE);
	}

	data.BuildDataModel(tasks);
}
