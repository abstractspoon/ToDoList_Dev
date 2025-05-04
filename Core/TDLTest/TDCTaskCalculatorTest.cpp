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

const LPCTSTR ID_DATE = _T("Date");
const LPCTSTR ID_INTEGER = _T("Integer");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskCalculatorTest::CTDCTaskCalculatorTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CTDCTaskCalculatorTest"), utils)
{
	// Simple custom attributes
	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	attribDef.sUniqueID = attribDef.sLabel = ID_INTEGER;
	attribDef.SetAttributeType(TDCCA_INTEGER);
	attribDef.dwFeatures = TDCCAF_ACCUMULATE;

	m_aCustomAttribDefs.Add(attribDef);

	attribDef.sUniqueID = attribDef.sLabel = ID_DATE;
	attribDef.SetAttributeType(TDCCA_DATE);
	attribDef.dwFeatures = TDCCAF_MAXIMIZE;

	m_aCustomAttribDefs.Add(attribDef);
}

CTDCTaskCalculatorTest::~CTDCTaskCalculatorTest()
{

}

TESTRESULT CTDCTaskCalculatorTest::Run()
{
	ClearTotals();

	CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);
	PopulateDataModel(data);

	TestCalcs(data, FALSE); // References NOT included
	TestCalcs(data, TRUE);  // References included

	return GetTotals();
}

void CTDCTaskCalculatorTest::TestCalcs(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	TestGetTaskStartDate(data, bIncludeRefs);
	TestGetTaskDueDate(data, bIncludeRefs);
	TestGetTaskLastModifiedDateAndUser(data, bIncludeRefs);
	TestGetTaskPriority(data, bIncludeRefs);
	TestGetTaskRisk(data, bIncludeRefs);
	TestGetTaskPercentDone(data, bIncludeRefs);
	TestGetTaskCost(data, bIncludeRefs);
	TestGetTaskTimeEstimate(data, bIncludeRefs);
	TestGetTaskTimeSpent(data, bIncludeRefs);
	TestGetTaskTimeRemaining(data, bIncludeRefs);
	TestGetTaskFlag(data, bIncludeRefs);
	TestGetTaskLock(data, bIncludeRefs);
	TestGetTaskCustomAttributeData(data, bIncludeRefs);
}

void CTDCTaskCalculatorTest::InitialiseStyles(BOOL bIncludeRefs)
{
	m_aStyles.RemoveAll();
	m_aStyles[TDCS_INCLUDEREFERENCESINCALCS] = bIncludeRefs;
}

void CTDCTaskCalculatorTest::TestGetTaskStartDate(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculatorTest::GetTaskStartDate"));
	CTDCTaskCalculator calc(data);

	// Assigned (unaffected by reference task)
	{
		InitialiseStyles(bIncludeRefs);

		ExpectEQ(calc.GetTaskStartDate(1), 45000.0);
		ExpectEQ(calc.GetTaskStartDate(2), 45001.0);
		ExpectEQ(calc.GetTaskStartDate(3), 45002.0);
		ExpectEQ(calc.GetTaskStartDate(4), 0.0); // completed task
		ExpectEQ(calc.GetTaskStartDate(5), 45004.0);
	}

	// Earliest (unaffected by reference task because its date is later)
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_USEEARLIESTSTARTDATE] = TRUE;

		ExpectEQ(calc.GetTaskStartDate(1), 45000.0);
		ExpectEQ(calc.GetTaskStartDate(2), 45001.0);
		ExpectEQ(calc.GetTaskStartDate(3), 45002.0);
		ExpectEQ(calc.GetTaskStartDate(4), 0.0); // completed task
		ExpectEQ(calc.GetTaskStartDate(5), 45004.0);
	}

	// Latest
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_USELATESTSTARTDATE] = TRUE;

		if (bIncludeRefs)
		{
			ExpectEQ(calc.GetTaskStartDate(1), 45004.0);
			ExpectEQ(calc.GetTaskStartDate(2), 45001.0);
			ExpectEQ(calc.GetTaskStartDate(3), 45004.0);
			ExpectEQ(calc.GetTaskStartDate(4), 0.0); // completed task
		}
		else
		{
			ExpectEQ(calc.GetTaskStartDate(1), 45002.0);
			ExpectEQ(calc.GetTaskStartDate(2), 45001.0);
			ExpectEQ(calc.GetTaskStartDate(3), 45002.0);
			ExpectEQ(calc.GetTaskStartDate(4), 0.0); // completed task
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskDueDate(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskDueDate"));
	CTDCTaskCalculator calc(data);

	// Assigned (unaffected by reference tasks)
	{
		InitialiseStyles(bIncludeRefs);

		ExpectEQ(calc.GetTaskDueDate(1), 45001.0);
		ExpectEQ(calc.GetTaskDueDate(2), 45002.0);
		ExpectEQ(calc.GetTaskDueDate(3), 45003.0);
		ExpectEQ(calc.GetTaskDueDate(4), 0.0); // completed task
		ExpectEQ(calc.GetTaskDueDate(5), 45005.0);
	}

	// Earliest (unaffected by reference tasks because its date is later)
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_USEEARLIESTDUEDATE] = TRUE;

		ExpectEQ(calc.GetTaskDueDate(1), 45001.0);
		ExpectEQ(calc.GetTaskDueDate(2), 45002.0);
		ExpectEQ(calc.GetTaskDueDate(3), 45003.0);
		ExpectEQ(calc.GetTaskDueDate(4), 0.0); // completed task
		ExpectEQ(calc.GetTaskDueDate(5), 45005.0);
	}

	// Latest
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_USELATESTDUEDATE] = TRUE;

		if (bIncludeRefs)
		{
			ExpectEQ(calc.GetTaskDueDate(1), 45005.0);
			ExpectEQ(calc.GetTaskDueDate(2), 45002.0);
			ExpectEQ(calc.GetTaskDueDate(3), 45005.0);
			ExpectEQ(calc.GetTaskDueDate(4), 0.0); // completed task
			ExpectEQ(calc.GetTaskDueDate(5), 45005.0);
		}
		else
		{
			ExpectEQ(calc.GetTaskDueDate(1), 45003.0);
			ExpectEQ(calc.GetTaskDueDate(2), 45002.0);
			ExpectEQ(calc.GetTaskDueDate(3), 45003.0);
			ExpectEQ(calc.GetTaskDueDate(4), 0.0); // completed task
			ExpectEQ(calc.GetTaskDueDate(5), 45005.0);
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskLastModifiedDateAndUser(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetLastModifiedDate/By"));
	CTDCTaskCalculator calc(data);

	// Assigned (unaffected by reference tasks)
	{
		InitialiseStyles(bIncludeRefs);

		ExpectEQ(calc.GetTaskLastModifiedDate(1), 45002.0);
		ExpectEQ(calc.GetTaskLastModifiedDate(2), 45003.0);
		ExpectEQ(calc.GetTaskLastModifiedDate(3), 45004.0);
		ExpectEQ(calc.GetTaskLastModifiedDate(4), 45005.0);
		ExpectEQ(calc.GetTaskLastModifiedDate(5), 45006.0);

		ExpectEQ(calc.GetTaskLastModifiedBy(1), _T("User1"));
		ExpectEQ(calc.GetTaskLastModifiedBy(2), _T("User2"));
		ExpectEQ(calc.GetTaskLastModifiedBy(3), _T("User3"));
		ExpectEQ(calc.GetTaskLastModifiedBy(4), _T("User4"));
		ExpectEQ(calc.GetTaskLastModifiedBy(5), _T("User5"));
	}

	// Latest
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_USELATESTLASTMODIFIED] = TRUE;

		if (bIncludeRefs)
		{
			ExpectEQ(calc.GetTaskLastModifiedDate(1), 45006.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(2), 45003.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(3), 45006.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(4), 45005.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(5), 45006.0);

			ExpectEQ(calc.GetTaskLastModifiedBy(1), _T("User5"));
			ExpectEQ(calc.GetTaskLastModifiedBy(2), _T("User2"));
			ExpectEQ(calc.GetTaskLastModifiedBy(3), _T("User5"));
			ExpectEQ(calc.GetTaskLastModifiedBy(4), _T("User4"));
			ExpectEQ(calc.GetTaskLastModifiedBy(5), _T("User5"));
		}
		else
		{
			ExpectEQ(calc.GetTaskLastModifiedDate(1), 45005.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(2), 45003.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(3), 45005.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(4), 45005.0);
			ExpectEQ(calc.GetTaskLastModifiedDate(5), 45006.0);

			ExpectEQ(calc.GetTaskLastModifiedBy(1), _T("User4"));
			ExpectEQ(calc.GetTaskLastModifiedBy(2), _T("User2"));
			ExpectEQ(calc.GetTaskLastModifiedBy(3), _T("User4"));
			ExpectEQ(calc.GetTaskLastModifiedBy(4), _T("User4"));
			ExpectEQ(calc.GetTaskLastModifiedBy(5), _T("User5"));
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskPriority(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskPriority"));
	CTDCTaskCalculator calc(data);

	// Assigned (unaffected by reference tasks)
	{
		// No modifiers
		{
			// Note: The 'bCheckOverdue' argument in GetTaskPriority is only 
			//       relevant in the presence of the TDCS_DUEHAVEHIGHESTPRIORITY
			//       modifier, so if that's not present we can ignore it
			InitialiseStyles(bIncludeRefs);

			ExpectEQ(calc.GetTaskPriority(1, FALSE), 5);
			ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
			ExpectEQ(calc.GetTaskPriority(3, FALSE), 7);
			ExpectEQ(calc.GetTaskPriority(4, FALSE), 8);
			ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);

			// Prove its irrelevance
			ExpectEQ(calc.GetTaskPriority(1, TRUE), calc.GetTaskPriority(1, FALSE));
			ExpectEQ(calc.GetTaskPriority(2, TRUE), calc.GetTaskPriority(2, FALSE));
			ExpectEQ(calc.GetTaskPriority(3, TRUE), calc.GetTaskPriority(3, FALSE));
			ExpectEQ(calc.GetTaskPriority(4, TRUE), calc.GetTaskPriority(4, FALSE));
			ExpectEQ(calc.GetTaskPriority(5, TRUE), calc.GetTaskPriority(5, FALSE));
		}

		// Done have lowest priority
		{
			// DON'T treat tasks with completed subtasks as completed
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;

				ExpectEQ(calc.GetTaskPriority(1, FALSE), 5);
				ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
				ExpectEQ(calc.GetTaskPriority(3, FALSE), 7);
				ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
				ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
			}

			// DO treat tasks with completed subtasks as completed
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 5);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 7);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 5);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 0); // implicitly completed
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}
		}

		// Due have highest priority
		{
			// DON'T treat tasks with completed subtasks as completed
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_DUEHAVEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;

				ExpectEQ(calc.GetTaskPriority(1, TRUE), 10);
				ExpectEQ(calc.GetTaskPriority(2, TRUE), 10);
				ExpectEQ(calc.GetTaskPriority(3, TRUE), 10);
				ExpectEQ(calc.GetTaskPriority(4, TRUE), 8); // completed task
				ExpectEQ(calc.GetTaskPriority(5, TRUE), 10);
			}

			// DO treat tasks with completed subtasks as completed
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_DUEHAVEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, TRUE), 10);
					ExpectEQ(calc.GetTaskPriority(2, TRUE), 10);
					ExpectEQ(calc.GetTaskPriority(3, TRUE), 10);
					ExpectEQ(calc.GetTaskPriority(4, TRUE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, TRUE), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, TRUE), 10);
					ExpectEQ(calc.GetTaskPriority(2, TRUE), 10);
					ExpectEQ(calc.GetTaskPriority(3, TRUE), 7); // implicitly completed
					ExpectEQ(calc.GetTaskPriority(4, TRUE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, TRUE), 10);
				}
			}
		}
	}

	// Highest
	{
		// DON'T treat tasks with completed subtasks as completed
		{
			// Done included and lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
				m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = TRUE;
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 7);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 7);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}

			// Done included but NOT lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
				m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = TRUE;
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = FALSE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 8);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 8);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}

			// Done NOT included
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
				m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = FALSE;
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 7);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 7);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}

			// Done NOT included and NOT lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
				m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = FALSE;
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = FALSE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 7);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 7);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}
		}

		// DO treat tasks with completed subtasks as completed
		{
			// Done included and lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
				m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = TRUE;
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 0); // implicitly completed
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}

			// Done included but NOT lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
				m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = TRUE;
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = FALSE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 8);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 8); // implicitly completed
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}

			// Done NOT included and lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
				m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = FALSE;
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 0); // implicitly completed
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 0); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}

			// Done NOT included and NOT lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
				m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = FALSE;
				m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = FALSE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 9);
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
				else
				{
					ExpectEQ(calc.GetTaskPriority(1, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(2, FALSE), 6);
					ExpectEQ(calc.GetTaskPriority(3, FALSE), 7); // implicitly completed
					ExpectEQ(calc.GetTaskPriority(4, FALSE), 8); // completed task
					ExpectEQ(calc.GetTaskPriority(5, FALSE), 9);
				}
			}
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskRisk(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskRisk"));
	CTDCTaskCalculator calc(data);

	// Assigned
	{
		// No modifiers
		{
			InitialiseStyles(bIncludeRefs);

			ExpectEQ(calc.GetTaskRisk(1), 6);
			ExpectEQ(calc.GetTaskRisk(2), 7);
			ExpectEQ(calc.GetTaskRisk(3), 8);
			ExpectEQ(calc.GetTaskRisk(4), 9);
			ExpectEQ(calc.GetTaskRisk(5), 10);
		}

		// Done have lowest risk
		{
			// DON'T treat tasks with completed subtasks as completed
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;

				ExpectEQ(calc.GetTaskRisk(1), 6);
				ExpectEQ(calc.GetTaskRisk(2), 7);
				ExpectEQ(calc.GetTaskRisk(3), 8);
				ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
				ExpectEQ(calc.GetTaskRisk(5), 10);
			}

			// DO treat tasks with completed subtasks as completed
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 6);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8); 
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 6);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 0); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}
		}
	}

	// Highest
	{
		// DON'T treat tasks with completed subtasks as completed
		{
			// Done included and lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
				m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 10);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 10);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 8);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}

			// Done included but NOT lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
				m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = FALSE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 10);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 10);
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 9);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 9);
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}

			// Done NOT included
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
				m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = FALSE;
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 10);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 10);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 8);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}

			// Done NOT included and NOT lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = FALSE;
				m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = FALSE;
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = FALSE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 10);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 10);
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 8);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8);
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}
		}

		// DO treat tasks with completed subtasks as completed
		{
			// Done included and lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
				m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 10);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 10);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 7);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 0); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}

			// Done included but NOT lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
				m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = FALSE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 10);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 10);
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 9);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 9); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}

			// Done NOT included and lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
				m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = FALSE;
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 10);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 10);
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 7);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 0); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 0); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}

			// Done NOT included and NOT lowest
			{
				InitialiseStyles(bIncludeRefs);
				m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
				m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
				m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = FALSE;
				m_aStyles[TDCS_DONEHAVELOWESTRISK] = FALSE;

				if (bIncludeRefs)
				{
					ExpectEQ(calc.GetTaskRisk(1), 10);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 10);
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
				else
				{
					ExpectEQ(calc.GetTaskRisk(1), 7);
					ExpectEQ(calc.GetTaskRisk(2), 7);
					ExpectEQ(calc.GetTaskRisk(3), 8); // implicitly completed
					ExpectEQ(calc.GetTaskRisk(4), 9); // completed task
					ExpectEQ(calc.GetTaskRisk(5), 10);
				}
			}
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskPercentDone(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskPercentDone"));
	CTDCTaskCalculator calc(data);

	// Assigned
	{
		InitialiseStyles(bIncludeRefs);

		ExpectEQ(calc.GetTaskPercentDone(1), 10);
		ExpectEQ(calc.GetTaskPercentDone(2), 20);
		ExpectEQ(calc.GetTaskPercentDone(3), 30);
		ExpectEQ(calc.GetTaskPercentDone(4), 100); // completed task
		ExpectEQ(calc.GetTaskPercentDone(5), 50);
	}

	// Time Spent/Estimate
	{
		// Note: To keep the manual checks comprehensible we use only 
		//       the Time Estimate values assigned in PopulateData

		// No parental contributions
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_AUTOCALCPERCENTDONE] = TRUE;
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskPercentDone(1), (int)(100 * (0.0 + 50.0 + (0.0 + 70.0 + 80.0)) / (0.0 + 40.0 + (0.0 + 60.0 + 70.0)))); // parent
				ExpectEQ(calc.GetTaskPercentDone(2), (int)(100 * (50.0 / 40.0)));
				ExpectEQ(calc.GetTaskPercentDone(3), (int)(100 * (0.0 + 70.0 + 80.0) / (0.0 + 60.0 + 70.0))); // parent
				ExpectEQ(calc.GetTaskPercentDone(4), (int)(100 * (70.0 / 60.0))); // completed task
				ExpectEQ(calc.GetTaskPercentDone(5), (int)(100 * (80.0 / 70.0)));
			}
			else
			{
				ExpectEQ(calc.GetTaskPercentDone(1), (int)(100 * (0.0 + 50.0 + (0.0 + 70.0)) / (0.0 + 40.0 + (0.0 + 60.0)))); // parent
				ExpectEQ(calc.GetTaskPercentDone(2), (int)(100 * (50.0 / 40.0)));
				ExpectEQ(calc.GetTaskPercentDone(3), (int)(100 * (0.0 + 70.0) / (0.0 + 60.0))); // parent
				ExpectEQ(calc.GetTaskPercentDone(4), (int)(100 * (70.0 / 60.0))); // completed task
				ExpectEQ(calc.GetTaskPercentDone(5), (int)(100 * (80.0 / 70.0)));
			}
		}

		// Parental contributions
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_AUTOCALCPERCENTDONE] = TRUE;
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskPercentDone(1), (int)(100 * (40.0 + 50.0 + (60.0 + 70.0 + 80.0)) / (30.0 + 40.0 + (50.0 + 60.0 + 70.0))));
				ExpectEQ(calc.GetTaskPercentDone(2), (int)(100 * (50.0 / 40.0)));
				ExpectEQ(calc.GetTaskPercentDone(3), (int)(100 * (60.0 + 70.0 + 80.0) / (50.0 + 60.0 + 70.0)));
				ExpectEQ(calc.GetTaskPercentDone(4), (int)(100 * (70.0 / 60.0))); // completed task
				ExpectEQ(calc.GetTaskPercentDone(5), (int)(100 * (80.0 / 70.0)));
			}
			else
			{
				ExpectEQ(calc.GetTaskPercentDone(1), (int)(100 * (40.0 + 50.0 + (60.0 + 70.0)) / (30.0 + 40.0 + (50.0 + 60.0))));
				ExpectEQ(calc.GetTaskPercentDone(2), (int)(100 * (50.0 / 40.0)));
				ExpectEQ(calc.GetTaskPercentDone(3), (int)(100 * (60.0 + 70.0) / (50.0 + 60.0)));
				ExpectEQ(calc.GetTaskPercentDone(4), (int)(100 * (70.0 / 60.0))); // completed task
				ExpectEQ(calc.GetTaskPercentDone(5), (int)(100 * (80.0 / 70.0)));
			}
		}
	}

	// Average subtask percentages
	{
		// Note: To keep the manual checks comprehensible I am not
		//       going to attempt to test TDCS_WEIGHTPERCENTCALCBYNUMSUB
		//       because it is too complex to express in simple terms

		// NOT include completed tasks 
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_AVERAGEPERCENTSUBCOMPLETION] = TRUE;
			m_aStyles[TDCS_INCLUDEDONEINAVERAGECALC] = FALSE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskPercentDone(1), 35);  // parent
				ExpectEQ(calc.GetTaskPercentDone(2), 20); 
				ExpectEQ(calc.GetTaskPercentDone(3), 50);  // parent
				ExpectEQ(calc.GetTaskPercentDone(4), 100); // completed task
				ExpectEQ(calc.GetTaskPercentDone(5), 50);
			}
			else
			{
				ExpectEQ(calc.GetTaskPercentDone(1), 10);  // parent
				ExpectEQ(calc.GetTaskPercentDone(2), 20); 
				ExpectEQ(calc.GetTaskPercentDone(3), 0);   // parent
				ExpectEQ(calc.GetTaskPercentDone(4), 100); // completed task
				ExpectEQ(calc.GetTaskPercentDone(5), 50);
			}
		}

		// Include completed tasks 
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_AVERAGEPERCENTSUBCOMPLETION] = TRUE;
			m_aStyles[TDCS_INCLUDEDONEINAVERAGECALC] = TRUE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskPercentDone(1), Misc::Round((20 + ((100 + 50) / 2)) / 2.0)); // parent
				ExpectEQ(calc.GetTaskPercentDone(2), 20);
				ExpectEQ(calc.GetTaskPercentDone(3), (100 + 50) / 2); // parent
				ExpectEQ(calc.GetTaskPercentDone(4), 100); // completed task
				ExpectEQ(calc.GetTaskPercentDone(5), 50);
			}
			else
			{
				ExpectEQ(calc.GetTaskPercentDone(1), (20 + 100) / 2); // parent
				ExpectEQ(calc.GetTaskPercentDone(2), 20); 
				ExpectEQ(calc.GetTaskPercentDone(3), 100); // parent
				ExpectEQ(calc.GetTaskPercentDone(4), 100); // completed task
				ExpectEQ(calc.GetTaskPercentDone(5), 50);
			}
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskCost(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculatorTest::GetTaskCost"));
	CTDCTaskCalculator calc(data);

	InitialiseStyles(bIncludeRefs);

	if (bIncludeRefs)
	{
		ExpectEQ(calc.GetTaskCost(1), (20.0 + 30.0 + (40.0 + (50.0 + 60.0))));
		ExpectEQ(calc.GetTaskCost(2), (30.0));
		ExpectEQ(calc.GetTaskCost(3), (40.0 + (50.0 + 60.0)));
		ExpectEQ(calc.GetTaskCost(4), (50.0));
		ExpectEQ(calc.GetTaskCost(5), (60.0));
	}
	else
	{
		ExpectEQ(calc.GetTaskCost(1), (20.0 + 30.0 + (40.0 + 50.0)));
		ExpectEQ(calc.GetTaskCost(2), (30.0));
		ExpectEQ(calc.GetTaskCost(3), (40.0 + 50.0));
		ExpectEQ(calc.GetTaskCost(4), (50.0));
		ExpectEQ(calc.GetTaskCost(5), (60.0));
	}
}

void CTDCTaskCalculatorTest::TestGetTaskTimeEstimate(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskTimeEstimate"));
	CTDCTaskCalculator calc(data);

	// No parental contributions
	{
		// NOT adjusting by % completion
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;
			m_aStyles[TDCS_USEPERCENTDONEINTIMEEST] = FALSE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (0.0 + 40.0 + 0.0 + (60.0 + 70.0))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (0.0 + (60.0 + 70.0))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0));       // completed task
				ExpectEQ(calc.GetTaskTimeEstimate(5, TDCU_DAYS), (70.0));
			}
			else
			{
				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (0.0 + 40.0 + 0.0 + 60.0)); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (0.0 + 60.0)); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0));       // completed task
				ExpectEQ(calc.GetTaskTimeEstimate(5, TDCU_DAYS), (70.0));
			}
		}

		// Adjusting by % completion
		//
		// Note: To keep the manual checks comprehensible we 
		//       use only the % values assigned in PopulateData
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;
			m_aStyles[TDCS_USEPERCENTDONEINTIMEEST] = TRUE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (0.0 + (40.0 * 0.8) + (0.0 + (60.0 * 0.0) + (70.0 * 0.5)))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0 * 0.8));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (0.0 + (60.0 * 0.0) + (70.0 * 0.5))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0 * 0.0)); // completed task
				ExpectEQ(calc.GetTaskTimeEstimate(5, TDCU_DAYS), (70.0 * 0.5));
			}
			else
			{
				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (0.0 + (40.0 * 0.8) + (0.0 + (60.0 * 0.0)))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0 * 0.8));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (0.0 + (60.0 * 0.0))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0 * 0.0)); // completed task
				ExpectEQ(calc.GetTaskTimeEstimate(5, TDCU_DAYS), (70.0 * 0.5));
			}
		}
	}

	// Parental contributions
	{
		// NOT adjusting by % completion
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;
			m_aStyles[TDCS_USEPERCENTDONEINTIMEEST] = FALSE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (30.0 + 40.0 + (50.0 + (60.0 + 70.0)))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (50.0 + (60.0 + 70.0))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0)); // completed task
				ExpectEQ(calc.GetTaskTimeEstimate(5, TDCU_DAYS), (70.0));
			}
			else
			{
				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (30.0 + 40.0 + (50.0 + 60.0)));
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (50.0 + 60.0));
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0)); // completed task
				ExpectEQ(calc.GetTaskTimeEstimate(5, TDCU_DAYS), (70.0));
			}
		}

		// Adjusting by % completion
		//
		// Note: To keep the manual checks comprehensible we 
		//       use only the % values assigned in PopulateData
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;
			m_aStyles[TDCS_USEPERCENTDONEINTIMEEST] = TRUE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (30.0 * 0.9) + (40.0 * 0.8) + ((50.0 * 0.7) + (60.0 * 0.0) + (70.0 * 0.5))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0 * 0.8));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (50.0 * 0.7) + (60.0 * 0.0) + (70.0 * 0.5)); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0 * 0.0)); // completed task
				ExpectEQ(calc.GetTaskTimeEstimate(5, TDCU_DAYS), (70.0 * 0.5));
			}
			else
			{
				ExpectEQ(calc.GetTaskTimeEstimate(1, TDCU_DAYS), (30.0 * 0.9) + (40.0 * 0.8) + ((50.0 * 0.7) + (60.0 * 0.0))); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(2, TDCU_DAYS), (40.0 * 0.8));
				ExpectEQ(calc.GetTaskTimeEstimate(3, TDCU_DAYS), (50.0 * 0.7) + (60.0 * 0.0)); // parent
				ExpectEQ(calc.GetTaskTimeEstimate(4, TDCU_DAYS), (60.0 * 0.0)); // completed task
				ExpectEQ(calc.GetTaskTimeEstimate(5, TDCU_DAYS), (70.0 * 0.5));
			}
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskTimeSpent(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskTimeSpent"));
	CTDCTaskCalculator calc(data);

	// No parental contributions
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;

		if (bIncludeRefs)
		{
			ExpectEQ(calc.GetTaskTimeSpent(1, TDCU_DAYS), (0.0 + 50.0 + (0.0 + (70.0 + 80.0)))); // parent task
			ExpectEQ(calc.GetTaskTimeSpent(2, TDCU_DAYS), (50.0));
			ExpectEQ(calc.GetTaskTimeSpent(3, TDCU_DAYS), (0.0 + (70.0 + 80.0))); // parent task
			ExpectEQ(calc.GetTaskTimeSpent(4, TDCU_DAYS), (70.0));       // completed task
			ExpectEQ(calc.GetTaskTimeSpent(5, TDCU_DAYS), (80.0));
		}
		else
		{
			ExpectEQ(calc.GetTaskTimeSpent(1, TDCU_DAYS), (0.0 + 50.0 + (0.0 + 70.0))); // parent task
			ExpectEQ(calc.GetTaskTimeSpent(2, TDCU_DAYS), (50.0));
			ExpectEQ(calc.GetTaskTimeSpent(3, TDCU_DAYS), (0.0 + 70.0)); // parent task
			ExpectEQ(calc.GetTaskTimeSpent(4, TDCU_DAYS), (70.0));       // completed task
			ExpectEQ(calc.GetTaskTimeSpent(5, TDCU_DAYS), (80.0));
		}
	}

	// Parental contributions
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;

		if (bIncludeRefs)
		{
			ExpectEQ(calc.GetTaskTimeSpent(1, TDCU_DAYS), (40.0 + 50.0 + (60.0 + (70.0 + 80.0))));
			ExpectEQ(calc.GetTaskTimeSpent(2, TDCU_DAYS), (50.0));
			ExpectEQ(calc.GetTaskTimeSpent(3, TDCU_DAYS), (60.0 + (70.0 + 80.0)));
			ExpectEQ(calc.GetTaskTimeSpent(4, TDCU_DAYS), (70.0)); // completed task
			ExpectEQ(calc.GetTaskTimeSpent(5, TDCU_DAYS), (80.0));
		}
		else
		{
			ExpectEQ(calc.GetTaskTimeSpent(1, TDCU_DAYS), (40.0 + 50.0 + (60.0 + 70.0)));
			ExpectEQ(calc.GetTaskTimeSpent(2, TDCU_DAYS), (50.0));
			ExpectEQ(calc.GetTaskTimeSpent(3, TDCU_DAYS), (60.0 + 70.0));
			ExpectEQ(calc.GetTaskTimeSpent(4, TDCU_DAYS), (70.0)); // completed task
			ExpectEQ(calc.GetTaskTimeSpent(5, TDCU_DAYS), (80.0));
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskTimeRemaining(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskTimeRemaining"));
	CTDCTaskCalculator calc(data);

	TDC_UNITS nUnits = TDCU_NULL;

	// Using Due Date (Now - Due Date)
	{
		// Assigned (unaffected by reference tasks)
		{
			InitialiseStyles(bIncludeRefs);
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
			ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), ((45005.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
		}

		// Earliest (unaffected by reference tasks because its date is later)
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_CALCREMAININGTIMEBYDUEDATE] = TRUE;
			m_aStyles[TDCS_USEEARLIESTDUEDATE] = TRUE;

			// Note: +1 because due date has no time component -> end of day
			ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), ((45001.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), ((45002.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), ((45003.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), 0.0); // completed task
			ExpectEQ(nUnits, TDCU_NULL);
			ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), ((45005.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
		}

		// Latest
		if (bIncludeRefs)
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_CALCREMAININGTIMEBYDUEDATE] = TRUE;
			m_aStyles[TDCS_USELATESTDUEDATE] = TRUE;

			// Note: +1 because due date has no time component -> end of day
			ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), ((45005.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001); // parent
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), ((45002.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), ((45005.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001); // parent
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), 0.0); // completed task
			ExpectEQ(nUnits, TDCU_NULL);
			ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), ((45005.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
		}
		else
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_CALCREMAININGTIMEBYDUEDATE] = TRUE;
			m_aStyles[TDCS_USELATESTDUEDATE] = TRUE;

			// Note: +1 because due date has no time component -> end of day
			ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), ((45003.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001); // parent
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), ((45002.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), ((45003.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001); // parent
			ExpectEQ(nUnits, TDCU_DAYS);
			ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), 0.0); // completed task
			ExpectEQ(nUnits, TDCU_NULL);
			ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), ((45005.0 + 1.0) - COleDateTime::GetCurrentTime()), 0.0001);
			ExpectEQ(nUnits, TDCU_DAYS);
		}
	}

	// Using Time Spent (Time Estimate - Time Spent)
	{
		// Note: To keep the manual checks comprehensible we use only 
		//       the Time Estimate values assigned in PopulateData

		// No parental contributions
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_CALCREMAININGTIMEBYSPENT] = TRUE;
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (0.0 + 40.0 + (0.0 + (60.0 + 70.0))) - (0.0 + 50.0 + (0.0 + (70.0 + 80.0)))); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 - 50.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (0.0 + (60.0 + 70.0)) - (0.0 + (70.0 + 80.0))); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (60.0 - 70.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), (70.0 - 80.0));
				ExpectEQ(nUnits, TDCU_DAYS);
			}
			else
			{
				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (0.0 + 40.0 + (0.0 + 60.0)) - (0.0 + 50.0 + (0.0 + 70.0))); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 - 50.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (0.0 + 60.0) - (0.0 + 70.0)); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (60.0 - 70.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), (70.0 - 80.0));
				ExpectEQ(nUnits, TDCU_DAYS);
			}
		}

		// Parental contributions
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_CALCREMAININGTIMEBYSPENT] = TRUE;
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (30.0 + 40.0 + (50.0 + (60.0 + 70.0))) - (40.0 + 50.0 + (60.0 + (70.0 + 80.0))));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 - 50.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (50.0 + (60.0 + 70.0)) - (60.0 + (70.0 + 80.0)));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (60.0 - 70.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), (70.0 - 80.0));
				ExpectEQ(nUnits, TDCU_DAYS);
			}
			else
			{
				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (30.0 + 40.0 + (50.0 + 60.0)) - (40.0 + 50.0 + (60.0 + 70.0)));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 - 50.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (50.0 + 60.0) - (60.0 + 70.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (60.0 - 70.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), (70.0 - 80.0));
				ExpectEQ(nUnits, TDCU_DAYS);
			}
		}
	}

	// Using % Completion (Time Estimate * (100 - %) / 100)
	{
		// Note: To keep the manual checks comprehensible we 
		//       use only the % values assigned in PopulateData

		// No parental contributions
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_CALCREMAININGTIMEBYPERCENT] = TRUE;
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = FALSE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (0.0 + (40.0 * 0.8) + (0.0 + (0.0 + (70.0 * 0.5))))); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 * 0.8));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (0.0 + (0.0 + (70.0 * 0.5)))); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (0.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), (70.0 * 0.5));
				ExpectEQ(nUnits, TDCU_DAYS);
			}
			else
			{
				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), (0.0 + (40.0 * 0.8) + (0.0 + 0.0))); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 * 0.8));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), (0.0 + 0.0)); // parent
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (0.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), (70.0 * 0.5));
				ExpectEQ(nUnits, TDCU_DAYS);
			}
		}

		// Parental contributions
		{
			InitialiseStyles(bIncludeRefs);
			m_aStyles[TDCS_CALCREMAININGTIMEBYPERCENT] = TRUE;
			m_aStyles[TDCS_ALLOWPARENTTIMETRACKING] = TRUE;

			if (bIncludeRefs)
			{
				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), ((30.0 * 0.9) + (40.0 * 0.8) + ((50.0 * 0.7) + (0.0 + (70.0 * 0.5)))));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 * 0.8));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), ((50.0 * 0.7) + (0.0 + (70.0 * 0.5))));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (0.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), (70.0 * 0.5));
				ExpectEQ(nUnits, TDCU_DAYS);
			}
			else
			{
				ExpectEQ(calc.GetTaskTimeRemaining(1, nUnits), ((30.0 * 0.9) + (40.0 * 0.8) + ((50.0 * 0.7) + 0.0)));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(2, nUnits), (40.0 * 0.8));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(3, nUnits), ((50.0 * 0.7) + 0.0));
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(4, nUnits), (0.0)); // completed task
				ExpectEQ(nUnits, TDCU_DAYS);
				ExpectEQ(calc.GetTaskTimeRemaining(5, nUnits), (70.0 * 0.5));
				ExpectEQ(nUnits, TDCU_DAYS);
			}
		}
	}
}

void CTDCTaskCalculatorTest::TestGetTaskFlag(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskFlag"));
	CTDCTaskCalculator calc(data);

	// Assigned
	{
		InitialiseStyles(bIncludeRefs);

		ExpectFalse(calc.IsTaskFlagged(1));
		ExpectFalse(calc.IsTaskFlagged(2));
		ExpectFalse(calc.IsTaskFlagged(3));
		ExpectTrue(calc.IsTaskFlagged(4)); // flagged
		ExpectFalse(calc.IsTaskFlagged(5));
	}

	// Inherited
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_TASKINHERITSSUBTASKFLAGS] = TRUE;

		ExpectTrue(calc.IsTaskFlagged(1)); // parent
		ExpectFalse(calc.IsTaskFlagged(2));
		ExpectTrue(calc.IsTaskFlagged(3)); // parent
		ExpectTrue(calc.IsTaskFlagged(4)); // flagged
		ExpectFalse(calc.IsTaskFlagged(5));
	}
}

void CTDCTaskCalculatorTest::TestGetTaskLock(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskLock"));
	CTDCTaskCalculator calc(data);

	// Assigned
	{
		InitialiseStyles(bIncludeRefs);

		ExpectTrue(calc.IsTaskLocked(1)); // locked
		ExpectFalse(calc.IsTaskLocked(2));
		ExpectFalse(calc.IsTaskLocked(3));
		ExpectFalse(calc.IsTaskLocked(4));
		ExpectFalse(calc.IsTaskLocked(5));
	}

	// Inherited
	{
		InitialiseStyles(bIncludeRefs);
		m_aStyles[TDCS_SUBTASKSINHERITLOCK] = TRUE;

		ExpectTrue(calc.IsTaskLocked(1)); // locked
		ExpectTrue(calc.IsTaskLocked(2));
		ExpectTrue(calc.IsTaskLocked(3));
		ExpectTrue(calc.IsTaskLocked(4));
		ExpectFalse(calc.IsTaskLocked(5));
	}
}

void CTDCTaskCalculatorTest::TestGetTaskCustomAttributeData(const CToDoCtrlData& data, BOOL bIncludeRefs)
{
	CTDCScopedTest test(*this, _T("CTDCTaskCalculator::GetTaskCustomAttributeData"));
	CTDCTaskCalculator calc(data);

	InitialiseStyles(bIncludeRefs);

	// Accumulated integer
	{
		int nAtt = m_aCustomAttribDefs.Find(ID_INTEGER);
		ExpectTrue(nAtt == 0);

		if (nAtt == 0)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nAtt];
			double dValue = 0.0;

			if (bIncludeRefs)
			{
				ExpectTrue(calc.GetTaskCustomAttributeData(1, attribDef, dValue));
				ExpectEQ(dValue, (-37 + 53.0 + (17.0 + 5.0) - 11.0));

				ExpectTrue(calc.GetTaskCustomAttributeData(2, attribDef, dValue));
				ExpectEQ(dValue, 53.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(3, attribDef, dValue));
				ExpectEQ(dValue, (17.0 + 5.0 - 11.0));

				ExpectTrue(calc.GetTaskCustomAttributeData(4, attribDef, dValue));
				ExpectEQ(dValue, 5.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(5, attribDef, dValue));
				ExpectEQ(dValue, -11.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(6, attribDef, dValue)); // Reference
				ExpectEQ(dValue, -11.0);
			}
			else
			{
				ExpectTrue(calc.GetTaskCustomAttributeData(1, attribDef, dValue));
				ExpectEQ(dValue, (-37 + 53.0 + (17.0 + 5.0)));

				ExpectTrue(calc.GetTaskCustomAttributeData(2, attribDef, dValue));
				ExpectEQ(dValue, 53.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(3, attribDef, dValue));
				ExpectEQ(dValue, (17.0 + 5.0));

				ExpectTrue(calc.GetTaskCustomAttributeData(4, attribDef, dValue));
				ExpectEQ(dValue, 5.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(5, attribDef, dValue));
				ExpectEQ(dValue, -11.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(6, attribDef, dValue)); // Reference
				ExpectEQ(dValue, -11.0);
			}
		}
	}

	// Maximised date
	{
		int nAtt = m_aCustomAttribDefs.Find(ID_DATE);
		ExpectTrue(nAtt == 1);

		if (nAtt == 1)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nAtt];
			double dValue = 0.0;

			if (bIncludeRefs)
			{
				ExpectTrue(calc.GetTaskCustomAttributeData(1, attribDef, dValue));
				ExpectEQ(dValue, 45034.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(2, attribDef, dValue));
				ExpectEQ(dValue, 45023.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(3, attribDef, dValue));
				ExpectEQ(dValue, 45034.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(4, attribDef, dValue));
				ExpectEQ(dValue, 45031.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(5, attribDef, dValue));
				ExpectEQ(dValue, 45034.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(6, attribDef, dValue)); // Reference
				ExpectEQ(dValue, 45034.0);
			}
			else
			{
				ExpectTrue(calc.GetTaskCustomAttributeData(1, attribDef, dValue));
				ExpectEQ(dValue, 45031.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(2, attribDef, dValue));
				ExpectEQ(dValue, 45023.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(3, attribDef, dValue));
				ExpectEQ(dValue, 45031.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(4, attribDef, dValue));
				ExpectEQ(dValue, 45031.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(5, attribDef, dValue));
				ExpectEQ(dValue, 45034.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(6, attribDef, dValue)); // Reference
				ExpectEQ(dValue, 45034.0);
			}
		}
	}
}

// ---------------------------------------------------------------------------------

void CTDCTaskCalculatorTest::PopulateDataModel(CToDoCtrlData& data) const
{
	// Task1
	//   |
	//   |_ Task2
	//   |
	//   |_ Task3
	//        |
	//        |_ Task4 - Completed
	//        |
	//        |_ Task6 - Reference to 'Task5'
	//
	// Task5

	CTaskFile tasks;
	tasks.SetCustomAttributeDefs(m_aCustomAttribDefs);

	HTASKITEM hTask1 = tasks.NewTask(_T("Task1"), NULL, 1, 0);
	HTASKITEM hTask2 = tasks.NewTask(_T("Task2"), hTask1, 2, 1);
	HTASKITEM hTask3 = tasks.NewTask(_T("Task3"), hTask1, 3, 1);
	HTASKITEM hTask4 = tasks.NewTask(_T("Task4"), hTask3, 4, 3); // - The only completed task 
	HTASKITEM hTask5 = tasks.NewTask(_T("Task5"), NULL, 5, 0);
	HTASKITEM hTask6 = tasks.NewTask(_T("Task6"), hTask3, 6, 3); // - The only reference task

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

		tasks.SetTaskCustomAttributeData(hTask1, ID_INTEGER, TDCCADATA(-37));
		tasks.SetTaskCustomAttributeData(hTask1, ID_DATE, TDCCADATA(45020.0));
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

		tasks.SetTaskCustomAttributeData(hTask2, ID_INTEGER, TDCCADATA(53));
		tasks.SetTaskCustomAttributeData(hTask2, ID_DATE, TDCCADATA(45023.0));
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

		tasks.SetTaskCustomAttributeData(hTask3, ID_INTEGER, TDCCADATA(17));
		tasks.SetTaskCustomAttributeData(hTask3, ID_DATE, TDCCADATA(45015.0));
	}

	// Task4 - The only completed task
	{
		tasks.SetTaskStartDate(hTask4, COleDateTime(45003.0));
		tasks.SetTaskDueDate(hTask4, COleDateTime(45004.0));
		tasks.SetTaskDoneDate(hTask4, COleDateTime(45010.0)); 
		tasks.SetTaskLastModified(hTask4, COleDateTime(45005.0), _T("User4"));

		tasks.SetTaskPriority(hTask4, 8);
		tasks.SetTaskRisk(hTask4, 9);
		tasks.SetTaskPercentDone(hTask4, 40);

		tasks.SetTaskCost(hTask4, 50);
		tasks.SetTaskTimeEstimate(hTask4, 60, TDCU_DAYS);
		tasks.SetTaskTimeSpent(hTask4, 70, TDCU_DAYS);

		tasks.SetTaskFlag(hTask4, TRUE); // The only flagged task
		tasks.SetTaskLock(hTask4, FALSE);

		tasks.SetTaskCustomAttributeData(hTask4, ID_INTEGER, TDCCADATA(5));
		tasks.SetTaskCustomAttributeData(hTask4, ID_DATE, TDCCADATA(45031.0));
	}

	// Task5
	{
		tasks.SetTaskStartDate(hTask5, COleDateTime(45004.0));
		tasks.SetTaskDueDate(hTask5, COleDateTime(45005.0));
		tasks.SetTaskLastModified(hTask5, COleDateTime(45006.0), _T("User5"));

		tasks.SetTaskPriority(hTask5, 9);
		tasks.SetTaskRisk(hTask5, 10);
		tasks.SetTaskPercentDone(hTask5, 50);

		tasks.SetTaskCost(hTask5, 60);
		tasks.SetTaskTimeEstimate(hTask5, 70, TDCU_DAYS);
		tasks.SetTaskTimeSpent(hTask5, 80, TDCU_DAYS);

		tasks.SetTaskFlag(hTask5, FALSE);
		tasks.SetTaskLock(hTask5, FALSE);

		tasks.SetTaskCustomAttributeData(hTask5, ID_INTEGER, TDCCADATA(-11));
		tasks.SetTaskCustomAttributeData(hTask5, ID_DATE, TDCCADATA(45034.0));
	}

	// Task6
	{
		tasks.SetTaskReferenceID(hTask6, 5);
	}

	data.BuildDataModel(tasks);

#ifdef _DEBUG
// 	tasks.SetXmlHeader(DEFAULT_UNICODE_HEADER);
// 	tasks.Save(_T(" CTDCTaskCalculatorTest_PopulateDataModel.txt"), SFEF_UTF16);
#endif
}
