// FileMiscTest.cpp: implementation of the CRemapMenuIDsTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "RemapMenuIDsTest.h"

#include "..\ToDoList\resource.h"

#include "..\shared\FileMisc.h"
#include "..\shared\Misc.h"
#include "..\shared\ShortcutManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRemapMenuIDsTest::CRemapMenuIDsTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CRemapMenuIDsTest::~CRemapMenuIDsTest()
{
}

TESTRESULT CRemapMenuIDsTest::Run()
{
	ClearTotals();

	TestTDLCustomToolbar();
	TestPreferencesShortcutsPage();

	return GetTotals();
}

void CRemapMenuIDsTest::TestTDLCustomToolbar()
{
	BeginTest(_T("CTDLCustomToolbar::RemapMenuItemIDs"));
	
	CMap<UINT, UINT, UINT, UINT&> mapCmdIDs;
	CToolbarButtonArray aButtons;

	{
		InitButtonArray(aButtons, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER5);

		mapCmdIDs.RemoveAll();
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER3] = ID_VIEW_ACTIVATEFILTER6;

		CTDLCustomToolbar::RemapMenuItemIDs(mapCmdIDs, aButtons);

		ExpectTrue(ID_VIEW_ACTIVATEFILTER1 == aButtons[0].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER2 == aButtons[1].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER6 == aButtons[2].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER4 == aButtons[3].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER5 == aButtons[4].nMenuID);
	}

	{
		InitButtonArray(aButtons, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER5);

		mapCmdIDs.RemoveAll();
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER4] = ID_VIEW_ACTIVATEFILTER5;
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER5] = ID_VIEW_ACTIVATEFILTER7;

		CTDLCustomToolbar::RemapMenuItemIDs(mapCmdIDs, aButtons);

		ExpectTrue(ID_VIEW_ACTIVATEFILTER1 == aButtons[0].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER2 == aButtons[1].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER3 == aButtons[2].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER5 == aButtons[3].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER7 == aButtons[4].nMenuID);
	}


	{
		InitButtonArray(aButtons, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER5);

		mapCmdIDs.RemoveAll();
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER1] = ID_VIEW_ACTIVATEFILTER2;
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER2] = ID_VIEW_ACTIVATEFILTER1;

		CTDLCustomToolbar::RemapMenuItemIDs(mapCmdIDs, aButtons);

		ExpectTrue(ID_VIEW_ACTIVATEFILTER2 == aButtons[0].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER1 == aButtons[1].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER3 == aButtons[2].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER4 == aButtons[3].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER5 == aButtons[4].nMenuID);
	}

	// TODO
	
	EndTest();
}

void CRemapMenuIDsTest::TestPreferencesShortcutsPage()
{
	BeginTest(_T("CPreferencesShortcutsPage::RemapMenuItemIDs"));

	const DWORD SHORTCUT1 = 0x12345678;
	const DWORD SHORTCUT2 = 0x23456789;
	const DWORD SHORTCUT3 = 0x34567890;

	CMap<UINT, UINT, UINT, UINT&> mapCmdIDs;
	CShortcutManager mgrShortcuts;

	{
		mgrShortcuts.DeleteAllShortcuts();

		mgrShortcuts.AddShortcut(ID_VIEW_ACTIVATEFILTER1, SHORTCUT1);
		mgrShortcuts.AddShortcut(ID_VIEW_ACTIVATEFILTER3, SHORTCUT2);
		mgrShortcuts.AddShortcut(ID_VIEW_ACTIVATEFILTER5, SHORTCUT3);

		mapCmdIDs.RemoveAll();
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER3] = ID_VIEW_ACTIVATEFILTER6;

		mgrShortcuts.RemapMenuItemIDs(mapCmdIDs);

		ExpectTrue(mgrShortcuts.GetShortcut(ID_VIEW_ACTIVATEFILTER1) == SHORTCUT1);
		ExpectTrue(mgrShortcuts.GetShortcut(ID_VIEW_ACTIVATEFILTER6) == SHORTCUT2);
		ExpectTrue(mgrShortcuts.GetShortcut(ID_VIEW_ACTIVATEFILTER5) == SHORTCUT3);
		ExpectTrue(mgrShortcuts.GetShortcut(ID_VIEW_ACTIVATEFILTER3) == 0);
	}
	
	EndTest();
}

void CRemapMenuIDsTest::InitButtonArray(CToolbarButtonArray& aButtons, UINT nIDFrom, UINT nIDTo)
{
	aButtons.SetSize(nIDTo - nIDFrom + 1);

	for (UINT nID = nIDFrom; nID <= nIDTo; nID++)
	{
		aButtons[nID - nIDFrom].nMenuID = nID;
	}
}
