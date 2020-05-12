// FileMiscTest.cpp: implementation of the CRemapMenuIDsTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "RemapMenuIDsTest.h"

#include "..\ToDoList\resource.h"
#include "..\ToDoList\TDCFilter.h"

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
	TestShortcutsMgr();
	TestTDCFilter();

	return GetTotals();
}

void CRemapMenuIDsTest::TestTDLCustomToolbar()
{
	BeginTest(_T("CTDLCustomToolbar::RemapMenuItemIDs"));
	
	CMap<UINT, UINT, UINT, UINT&> mapCmdIDs;
	CToolbarButtonArray aButtons;

	{
		// Replace one menu item
		InitButtonArray(aButtons, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER5);

		mapCmdIDs.RemoveAll();
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER3] = ID_VIEW_ACTIVATEFILTER6;

		CTDLCustomToolbar::RemapMenuItemIDs(mapCmdIDs, aButtons);

		ExpectTrue(ID_VIEW_ACTIVATEFILTER1 == aButtons[0].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER2 == aButtons[1].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER6 == aButtons[2].nMenuID); // changed 
		ExpectTrue(ID_VIEW_ACTIVATEFILTER4 == aButtons[3].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER5 == aButtons[4].nMenuID);

		// Images remain unchanged
		ExpectEQ(_T("0"), aButtons[0].sImageID);
		ExpectEQ(_T("1"), aButtons[1].sImageID);
		ExpectEQ(_T("2"), aButtons[2].sImageID);
		ExpectEQ(_T("3"), aButtons[3].sImageID);
		ExpectEQ(_T("4"), aButtons[4].sImageID);
	}

	{
		// Replace two overlapping menu items
		InitButtonArray(aButtons, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER5);

		mapCmdIDs.RemoveAll();
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER4] = ID_VIEW_ACTIVATEFILTER5;
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER5] = ID_VIEW_ACTIVATEFILTER7;

		CTDLCustomToolbar::RemapMenuItemIDs(mapCmdIDs, aButtons);

		ExpectTrue(ID_VIEW_ACTIVATEFILTER1 == aButtons[0].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER2 == aButtons[1].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER3 == aButtons[2].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER5 == aButtons[3].nMenuID); // changed
		ExpectTrue(ID_VIEW_ACTIVATEFILTER7 == aButtons[4].nMenuID); // changed

		// Images remain unchanged
		ExpectEQ(_T("0"), aButtons[0].sImageID);
		ExpectEQ(_T("1"), aButtons[1].sImageID);
		ExpectEQ(_T("2"), aButtons[2].sImageID);
		ExpectEQ(_T("3"), aButtons[3].sImageID);
		ExpectEQ(_T("4"), aButtons[4].sImageID);
	}
	
	{
		// Swap two menu items
		InitButtonArray(aButtons, ID_VIEW_ACTIVATEFILTER1, ID_VIEW_ACTIVATEFILTER5);

		mapCmdIDs.RemoveAll();
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER1] = ID_VIEW_ACTIVATEFILTER2;
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER2] = ID_VIEW_ACTIVATEFILTER1;

		CTDLCustomToolbar::RemapMenuItemIDs(mapCmdIDs, aButtons);

		ExpectTrue(ID_VIEW_ACTIVATEFILTER2 == aButtons[0].nMenuID); // swapped
		ExpectTrue(ID_VIEW_ACTIVATEFILTER1 == aButtons[1].nMenuID); // swapped
		ExpectTrue(ID_VIEW_ACTIVATEFILTER3 == aButtons[2].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER4 == aButtons[3].nMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEFILTER5 == aButtons[4].nMenuID);

		// Images remain unchanged
		ExpectEQ(_T("0"), aButtons[0].sImageID);
		ExpectEQ(_T("1"), aButtons[1].sImageID);
		ExpectEQ(_T("2"), aButtons[2].sImageID);
		ExpectEQ(_T("3"), aButtons[3].sImageID);
		ExpectEQ(_T("4"), aButtons[4].sImageID);
	}

	EndTest();
}

void CRemapMenuIDsTest::TestShortcutsMgr()
{
	BeginTest(_T("CShortcutManager::RemapMenuItemIDs"));

	const DWORD SHORTCUT1 = 0x12345678;
	const DWORD SHORTCUT2 = 0x23456789;
	const DWORD SHORTCUT3 = 0x34567890;

	CMap<UINT, UINT, UINT, UINT&> mapCmdIDs;
	CShortcutManager mgrShortcuts;

	{
		// Replace one menu item
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

	{
		// Swap two menu items
		mgrShortcuts.DeleteAllShortcuts();

		mgrShortcuts.AddShortcut(ID_VIEW_ACTIVATEFILTER1, SHORTCUT1);
		mgrShortcuts.AddShortcut(ID_VIEW_ACTIVATEFILTER3, SHORTCUT3);

		mapCmdIDs.RemoveAll();
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER3] = ID_VIEW_ACTIVATEFILTER1;
		mapCmdIDs[ID_VIEW_ACTIVATEFILTER1] = ID_VIEW_ACTIVATEFILTER3;

		mgrShortcuts.RemapMenuItemIDs(mapCmdIDs);

		ExpectTrue(mgrShortcuts.GetShortcut(ID_VIEW_ACTIVATEFILTER1) == SHORTCUT3);
		ExpectTrue(mgrShortcuts.GetShortcut(ID_VIEW_ACTIVATEFILTER3) == SHORTCUT1);
	}
	
	EndTest();
}

void CRemapMenuIDsTest::TestTDCFilter()
{
	BeginTest(_T("CTDCFilter::BuildAdvancedFilterMenuItemMapping"));

	CStringArray aOldFilterNames;
	aOldFilterNames.Add(_T("Filter1"));
	aOldFilterNames.Add(_T("Filter2"));
	aOldFilterNames.Add(_T("Filter3"));
	aOldFilterNames.Add(_T("Filter4"));

	CStringArray aNewFilterNames;
	CMap<UINT, UINT, UINT, UINT&> mapCmdIDs;
	POSITION pos;
	UINT nOldMenuID, nNewMenuID;

	{
		// Add new filter to end -> nothing needs remapping
		aNewFilterNames.Copy(aOldFilterNames);
		aNewFilterNames.Add(_T("Filter5"));

		int nNumIDs = CTDCFilter::BuildAdvancedFilterMenuItemMapping(aOldFilterNames, aNewFilterNames, mapCmdIDs);
		ExpectEQ(nNumIDs, 0);
	}

	{
		// Delete filter from end -> last menu item gets mapped to zero
		aNewFilterNames.Copy(aOldFilterNames);
		aNewFilterNames.RemoveAt(3);

		int nNumIDs = CTDCFilter::BuildAdvancedFilterMenuItemMapping(aOldFilterNames, aNewFilterNames, mapCmdIDs);
		ExpectEQ(nNumIDs, 1);

		pos = mapCmdIDs.GetStartPosition();
		ExpectTrue(NULL != pos);

		mapCmdIDs.GetNextAssoc(pos, nOldMenuID, nNewMenuID);
		ExpectTrue(ID_VIEW_ACTIVATEADVANCEDFILTER4 == nOldMenuID);
		ExpectTrue(0 == nNewMenuID);
	}

	{
		// Insert new second filter 
		aNewFilterNames.Copy(aOldFilterNames);
		aNewFilterNames.InsertAt(1, _T("Filter5"));

		// MenuItems for 2, 3 and 4 get incremented
		int nNumIDs = CTDCFilter::BuildAdvancedFilterMenuItemMapping(aOldFilterNames, aNewFilterNames, mapCmdIDs);
		ExpectEQ(nNumIDs, 3);

		pos = mapCmdIDs.GetStartPosition();
		ExpectTrue(NULL != pos);

		while (pos)
		{
			mapCmdIDs.GetNextAssoc(pos, nOldMenuID, nNewMenuID);

			switch (nOldMenuID)
			{
			case ID_VIEW_ACTIVATEADVANCEDFILTER2:
				ExpectTrue(nNewMenuID == ID_VIEW_ACTIVATEADVANCEDFILTER3);
				break;

			case ID_VIEW_ACTIVATEADVANCEDFILTER3:
				ExpectTrue(nNewMenuID == ID_VIEW_ACTIVATEADVANCEDFILTER4);
				break;

			case ID_VIEW_ACTIVATEADVANCEDFILTER4:
				ExpectTrue(nNewMenuID == ID_VIEW_ACTIVATEADVANCEDFILTER5);
				break;

			default:
				ExpectEmpty(_T("Invalid menu item ID"));
				break;
			}
		}
	}

	{
		// Delete second filter
		aNewFilterNames.Copy(aOldFilterNames);
		aNewFilterNames.RemoveAt(1);

		// 3 and 4 get shifted down one place and the old second item is set to zero
		int nNumIDs = CTDCFilter::BuildAdvancedFilterMenuItemMapping(aOldFilterNames, aNewFilterNames, mapCmdIDs);
		ExpectEQ(nNumIDs, 3);

		POSITION pos = mapCmdIDs.GetStartPosition();
		ExpectTrue(NULL != pos);

		while (pos)
		{
			mapCmdIDs.GetNextAssoc(pos, nOldMenuID, nNewMenuID);

			switch (nOldMenuID)
			{
			case ID_VIEW_ACTIVATEADVANCEDFILTER2:
				ExpectTrue(nNewMenuID == 0);
				break;

			case ID_VIEW_ACTIVATEADVANCEDFILTER3:
				ExpectTrue(nNewMenuID == ID_VIEW_ACTIVATEADVANCEDFILTER2);
				break;

			case ID_VIEW_ACTIVATEADVANCEDFILTER4:
				ExpectTrue(nNewMenuID == ID_VIEW_ACTIVATEADVANCEDFILTER3);
				break;

			default:
				ExpectEmpty(_T("Invalid menu item ID"));
				break;
			}
		}
	}

	EndTest();
}

void CRemapMenuIDsTest::InitButtonArray(CToolbarButtonArray& aButtons, UINT nIDFrom, UINT nIDTo)
{
	aButtons.SetSize(nIDTo - nIDFrom + 1);

	for (UINT nID = nIDFrom; nID <= nIDTo; nID++)
	{
		int nBtn = (nID - nIDFrom);

		aButtons[nBtn].nMenuID = nID;
		aButtons[nBtn].sImageID = Misc::Format(nBtn);
	}
}
