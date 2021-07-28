// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TDCDEPENDENCYTest.h"

#include "..\ToDoList\ToDoItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const CString TDLFULLPATH(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));
const CString XMLFULLPATH(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\Another Tasklist.xml"));
const CString OTHERFULLPATH(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Document.doc"));

const CString TDLRELATIVEPATH(_T("Local\\A Tasklist.tdl"));
const CString XMLRELATIVEPATH(_T("Local\\Another Tasklist.xml"));
const CString OTHERRELATIVEPATH(_T("Local\\A Document.doc"));

const CString FOLDERPATH(_T("C:\\Users\\Daniel Godson\\AppData"));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCDEPENDENCYTest::CTDCDEPENDENCYTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(utils)
{

}

CTDCDEPENDENCYTest::~CTDCDEPENDENCYTest()
{

}

TESTRESULT CTDCDEPENDENCYTest::Run()
{
	ClearTotals();

	TestTDCDEPENDENCY();
	TestTDCDependencyArray();

	return GetTotals();
}

void CTDCDEPENDENCYTest::TestTDCDEPENDENCY()
{
	BeginTest(_T("TDCDEPENDENCY"));

	{
		TDCDEPENDENCY depend;
		ExpectFalse(depend.IsValid());
		ExpectFalse(depend.IsLocal());
		ExpectEmpty(depend.Format());
	}

	{
		TDCDEPENDENCY depend(0, TDLFULLPATH);
		ExpectFalse(depend.IsValid());
		ExpectFalse(depend.IsLocal());
		ExpectEmpty(depend.Format());
	}

	{
		TDCDEPENDENCY depend(2, TDLFULLPATH);
		ExpectTrue(depend.IsValid());
		ExpectFalse(depend.IsLocal());
		ExpectEQ(depend.Format(), TDLFULLPATH + _T("?2"));
	}

	{
		TDCDEPENDENCY depend(4, XMLFULLPATH);
		ExpectTrue(depend.IsValid());
		ExpectFalse(depend.IsLocal());
		ExpectEQ(depend.Format(), XMLFULLPATH + _T("?4"));
	}

	{
		TDCDEPENDENCY depend(4, OTHERFULLPATH);
		ExpectFalse(depend.IsValid());
		ExpectFalse(depend.IsLocal());
		ExpectEmpty(depend.Format());
	}

	{
		TDCDEPENDENCY depend1(2, TDLFULLPATH), depend2;
		depend2.Parse(depend1.Format());

		ExpectTrue(depend1 == depend2);
	}

	{
		TDCDEPENDENCY depend1(2, TDLFULLPATH), depend2(2, TDLFULLPATH);
		ExpectTrue(depend1 == depend2);
	}

	{
		TDCDEPENDENCY depend(2, TDLFULLPATH);
		ExpectFalse(depend.IsLocal());
	}

	{
		TDCDEPENDENCY depend(2);
		ExpectTrue(depend.IsLocal());
		ExpectEQ(depend.Format(), _T("2"));
	}

	{
		const CString sDepend(TDLFULLPATH + _T("?23"));
		TDCDEPENDENCY depend;

		ExpectTrue(depend.Parse(sDepend));
		ExpectTrue(depend.IsValid());

		ExpectEQ(depend.dwTaskID, 23UL);
		ExpectEQ(depend.sTasklist, TDLFULLPATH);
	}

	{
		const CString sDepend(TDLRELATIVEPATH + _T("?23"));
		TDCDEPENDENCY depend;

		ExpectTrue(depend.Parse(sDepend));
		ExpectTrue(depend.IsValid());

		ExpectEQ(depend.dwTaskID, 23UL);
		ExpectEQ(depend.GetFullPath(FOLDERPATH), TDLFULLPATH);
	}

	{
		const CString sDepend(XMLFULLPATH + _T("?71"));
		TDCDEPENDENCY depend;

		ExpectTrue(depend.Parse(sDepend));
		ExpectTrue(depend.IsValid());

		ExpectEQ(depend.dwTaskID, 71UL);
		ExpectEQ(depend.sTasklist, XMLFULLPATH);

		ExpectEQ(depend.Format(), sDepend);
	}

	{
		const CString sDepend(TDLFULLPATH + _T("?-23"));
		TDCDEPENDENCY depend;

		ExpectFalse(depend.Parse(sDepend));
		ExpectFalse(depend.IsValid());

		ExpectEQ(depend.dwTaskID, 0UL);
		ExpectEmpty(depend.sTasklist);

		ExpectEmpty(depend.Format());
	}

	{
		const CString sDepend(OTHERFULLPATH + _T("?71"));
		TDCDEPENDENCY depend;

		ExpectFalse(depend.Parse(sDepend));
		ExpectFalse(depend.IsValid());

		ExpectEQ(depend.dwTaskID, 0UL);
		ExpectEmpty(depend.sTasklist);

		ExpectEmpty(depend.Format());
	}

	{
		TDCDEPENDENCY depend1(2, TDLFULLPATH), depend2(4, XMLFULLPATH);
		ExpectFalse(depend1 == depend2);

		depend2 = depend1;
		ExpectTrue(depend1 == depend2);
	}

	EndTest();
}

void CTDCDEPENDENCYTest::TestTDCDependencyArray()
{
	BeginTest(_T("CTDCDependencyArray"));


	{
		// local dependencies
		CTDCDependencyArray aDepends;

		ExpectFalse(aDepends.Add(0));
		ExpectFalse(aDepends.HasLocalDependency(0));

		ExpectTrue(aDepends.Add(11));
		ExpectTrue(aDepends.HasLocalDependency(11));

		ExpectFalse(aDepends.Add(11)); // No duplicates
		ExpectFalse(aDepends.Add(TDCDEPENDENCY(11))); // No duplicates

		ExpectTrue(aDepends.Add(7));
		ExpectTrue(aDepends.HasLocalDependency(7));

		ExpectEQ(aDepends.Format(_T("+")), _T("11+7"));

		CDWordArray aTaskIDs;
		ExpectEQ(aDepends.GetLocalDependencies(aTaskIDs), 2);
		ExpectEQ(aTaskIDs[0], 11UL);
		ExpectEQ(aTaskIDs[1], 7UL);

		CStringArray aStrDepends;
		ExpectEQ(aDepends.GetDependencies(aTaskIDs, aStrDepends), 2);
		ExpectEQ(aTaskIDs.GetSize(), 2);
		ExpectEQ(aStrDepends.GetSize(), 0);

		ExpectEQ(aDepends.Format(aStrDepends), 2);
		ExpectEQ(aStrDepends[0], _T("11"));
		ExpectEQ(aStrDepends[1], _T("7"));
						
		ExpectFalse(aDepends.RemoveLocalDependency(13)); // not exists
		ExpectTrue(aDepends.RemoveLocalDependency(11));
		ExpectFalse(aDepends.HasLocalDependency(11));

		ExpectEQ(aDepends.GetLocalDependencies(aTaskIDs), 1);
		ExpectEQ(aTaskIDs[0], 7UL);

		ExpectEQ(aDepends.Format(_T("+")), _T("7"));

		ExpectFalse(aDepends.ReplaceLocalDependency(11, 12)); // 11 removed
		ExpectTrue(aDepends.ReplaceLocalDependency(7, 12));
		ExpectFalse(aDepends.HasLocalDependency(7)); // 7 replaced
		ExpectTrue(aDepends.HasLocalDependency(12));
	}

	{
		// local dependencies
		CDWordArray aTaskIDs;

		aTaskIDs.Add(7);
		aTaskIDs.Add(11);
		aTaskIDs.Add(7); // duplicate
		aTaskIDs.Add(0); // invalid
		
		CTDCDependencyArray aDepends;
		aDepends.Set(aTaskIDs);

		ExpectEQ(aDepends.GetSize(), 2);
		ExpectEQ(aDepends[0].dwTaskID, 7UL);
		ExpectEQ(aDepends[1].dwTaskID, 11UL);
	}
	
	{
		// inter-tasklist dependencies
		CTDCDependencyArray aDepends;

		ExpectFalse(aDepends.Add(0, TDLFULLPATH));
		ExpectFalse(aDepends.Add(0, XMLFULLPATH));
		ExpectFalse(aDepends.Add(2, OTHERFULLPATH)); // Asserts

		ExpectTrue(aDepends.Add(11, TDLFULLPATH));
		ExpectFalse(aDepends.Add(11, TDLFULLPATH)); // No duplicates
		ExpectFalse(aDepends.HasLocalDependency(11)); // Has path
		ExpectTrue(aDepends.Add(7, XMLFULLPATH));
		ExpectFalse(aDepends.HasLocalDependency(7)); // Has path

		CDWordArray aTaskIDs;
		ExpectEQ(aDepends.GetLocalDependencies(aTaskIDs), 0);

		CStringArray aStrDepends;
		ExpectEQ(aDepends.GetDependencies(aTaskIDs, aStrDepends), 2);
		ExpectEQ(aTaskIDs.GetSize(), 0);
		ExpectEQ(aStrDepends.GetSize(), 2);

		ExpectEQ(aStrDepends[0], TDLFULLPATH + _T("?11"));
		ExpectEQ(aStrDepends[1], XMLFULLPATH + _T("?7"));
	}

	{
		// inter-tasklist dependencies
		CStringArray aStrDepends;

		aStrDepends.Add(TDLFULLPATH + _T("?11"));
		aStrDepends.Add(XMLFULLPATH + _T("?7"));
		aStrDepends.Add(TDLFULLPATH + _T("?11")); // duplicate
		aStrDepends.Add(OTHERFULLPATH + _T("?11")); // invalid
		aStrDepends.Add(TDLFULLPATH + _T("?0")); // invalid
		aStrDepends.Add(TDLFULLPATH); // invalid

		CTDCDependencyArray aDepends;
		aDepends.Set(aStrDepends);

		ExpectEQ(aDepends.GetSize(), 2);
		ExpectEQ(aDepends[0].dwTaskID, 11UL);
		ExpectEQ(aDepends[0].sTasklist, TDLFULLPATH);
		ExpectEQ(aDepends[1].dwTaskID, 7UL);
		ExpectEQ(aDepends[1].sTasklist, XMLFULLPATH);
	}
		
	EndTest();
}

