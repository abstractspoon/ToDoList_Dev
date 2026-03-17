// FileMiscTest.cpp: implementation of the CMiscTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "MiscTest.h"

#include "..\shared\Misc.h"

#include "..\3rdParty\OSVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const CString DEFSEP = (Misc::GetListSeparator() + ' ');
const int DEFSEPLEN = DEFSEP.GetLength();

const LPCTSTR NULLSTRING = NULL;
const CString EMPTYSTRING;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiscTest::CMiscTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CMiscTest"), utils)
{

}

CMiscTest::~CMiscTest()
{
}

TESTRESULT CMiscTest::Run()
{
	ClearTotals();

	TestRegionalSettingsRetrievalPerformance();

	TestGetFormattedLength();
	TestFormatArray();
	TestHasPrefix();
	TestHasSuffix();
	TestAtof();
	TestIsNumber();
	TestRemoveDuplicates();
	TestSplitLines();

	return GetTotals();
}

void CMiscTest::TestGetFormattedLength()
{
	CTDCScopedTest test(*this, _T("Misc::GetFormattedLength"));

	{
		CStringArray aValues;
		aValues.Add(_T("a"));
		aValues.Add(_T("ab"));
		aValues.Add(_T("abc"));
		aValues.Add(_T("abcd"));

		// First 3 are equivalent and use default Windows separator
		ExpectEQ(Misc::GetFormattedLength(aValues), 10 + (3 * DEFSEPLEN));
		ExpectEQ(Misc::GetFormattedLength(aValues, NULL), 10 + (3 * DEFSEPLEN));
		ExpectEQ(Misc::GetFormattedLength(aValues, _T("")), 10 + (3 * DEFSEPLEN));

		ExpectEQ(Misc::GetFormattedLength(aValues, _T(",")), 13);
		ExpectEQ(Misc::GetFormattedLength(aValues, _T("...")), 19);
	}
	
	{
		CStringArray aValues;
		aValues.Add(_T("a"));
		aValues.Add(_T(""));
		aValues.Add(_T("ab"));
		aValues.Add(_T(""));
		aValues.Add(_T("abc"));
		aValues.Add(_T(""));
		aValues.Add(_T("abcd"));

		// Excluding empty items
		ExpectEQ(Misc::GetFormattedLength(aValues), 10 + (3 * DEFSEPLEN));
		ExpectEQ(Misc::GetFormattedLength(aValues, NULL), 10 + (3 * DEFSEPLEN));
		ExpectEQ(Misc::GetFormattedLength(aValues, _T("")), 10 + (3 * DEFSEPLEN));

		ExpectEQ(Misc::GetFormattedLength(aValues, _T(",")), 13);
		ExpectEQ(Misc::GetFormattedLength(aValues, _T("...")), 19);

		// Including empty items
		ExpectEQ(Misc::GetFormattedLength(aValues, NULL, TRUE), 10 + (6 * DEFSEPLEN));
		ExpectEQ(Misc::GetFormattedLength(aValues, _T(""), TRUE), 10 + (6 * DEFSEPLEN));

		ExpectEQ(Misc::GetFormattedLength(aValues, _T(","), TRUE), 16);
		ExpectEQ(Misc::GetFormattedLength(aValues, _T("..."), TRUE), 28);
	}
	
	{
		// Only empty items
		CStringArray aValues;
		aValues.Add(_T(""));
		aValues.Add(_T(""));
		aValues.Add(_T(""));

		// Excluding empty items
		ExpectEQ(Misc::GetFormattedLength(aValues), 0);
		ExpectEQ(Misc::GetFormattedLength(aValues, NULL), 0);
		ExpectEQ(Misc::GetFormattedLength(aValues, _T("")), 0);

		ExpectEQ(Misc::GetFormattedLength(aValues, _T(",")), 0);
		ExpectEQ(Misc::GetFormattedLength(aValues, _T("...")), 0);

		// Including empty items
		ExpectEQ(Misc::GetFormattedLength(aValues, NULL, TRUE), 2 * DEFSEPLEN);
		ExpectEQ(Misc::GetFormattedLength(aValues, _T(""), TRUE), 2 * DEFSEPLEN);

		ExpectEQ(Misc::GetFormattedLength(aValues, _T(","), TRUE), 2);
		ExpectEQ(Misc::GetFormattedLength(aValues, _T("..."), TRUE), 6);
	}
}

void CMiscTest::TestFormatArray()
{
	CTDCScopedTest test(*this, _T("Misc::FormatArray"));

	{
		CStringArray aValues;
		aValues.Add(_T("a"));
		aValues.Add(_T("ab"));
		aValues.Add(_T("abc"));
		aValues.Add(_T("abcd"));

		CString sDefCompare = _T("a") + DEFSEP + 
								_T("ab") + DEFSEP + 
								_T("abc") + DEFSEP + 
								_T("abcd");

		ExpectEQ(Misc::FormatArray(aValues), sDefCompare);
		ExpectEQ(Misc::FormatArray(aValues, NULLSTRING), sDefCompare);
		ExpectEQ(Misc::FormatArray(aValues, _T("")), sDefCompare);

		ExpectEQ(Misc::FormatArray(aValues, _T(",")), _T("a,ab,abc,abcd"));
		ExpectEQ(Misc::FormatArray(aValues, _T("...")), _T("a...ab...abc...abcd"));

		ExpectTrue(ActualLengthMatchesCalculation(aValues, NULL));
		ExpectTrue(ActualLengthMatchesCalculation(aValues, _T("")));
		ExpectTrue(ActualLengthMatchesCalculation(aValues, _T(",")));
		ExpectTrue(ActualLengthMatchesCalculation(aValues, _T("...")));
	}

	{
		CStringArray aValues;
		aValues.Add(_T("a"));
		aValues.Add(_T(""));
		aValues.Add(_T("ab"));
		aValues.Add(_T(""));
		aValues.Add(_T("abc"));
		aValues.Add(_T(""));
		aValues.Add(_T("abcd"));

		// Excluding empty items
		{
			CString sDefCompare = _T("a") + DEFSEP + 
									_T("ab") + DEFSEP + 
									_T("abc") + DEFSEP + 
									_T("abcd");

			ExpectEQ(Misc::FormatArray(aValues), sDefCompare);
			ExpectEQ(Misc::FormatArray(aValues, NULLSTRING), sDefCompare);
			ExpectEQ(Misc::FormatArray(aValues, _T("")), sDefCompare);

			ExpectEQ(Misc::FormatArray(aValues, _T(",")), _T("a,ab,abc,abcd"));
			ExpectEQ(Misc::FormatArray(aValues, _T("...")), _T("a...ab...abc...abcd"));

			ExpectTrue(ActualLengthMatchesCalculation(aValues, NULL));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T("")));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T(",")));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T("...")));
		}

		// Including empty items
		{
			CString sDefCompare = _T("a") + DEFSEP + 
									_T("") + DEFSEP + 
									_T("ab") + DEFSEP + 
									_T("") + DEFSEP + 
									_T("abc") + DEFSEP + 
									_T("") + DEFSEP +
									_T("abcd");

			ExpectEQ(Misc::FormatArray(aValues, NULLSTRING, TRUE), sDefCompare);
			ExpectEQ(Misc::FormatArray(aValues, _T(""), TRUE), sDefCompare);

			ExpectEQ(Misc::FormatArray(aValues, _T(","), TRUE), _T("a,,ab,,abc,,abcd"));
			ExpectEQ(Misc::FormatArray(aValues, _T("..."), TRUE), _T("a......ab......abc......abcd"));

			ExpectTrue(ActualLengthMatchesCalculation(aValues, NULL, TRUE));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T(""), TRUE));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T(","), TRUE));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T(", "), TRUE));
		}
	}

	{
		// Only empty items
		CStringArray aValues;
		aValues.Add(_T(""));
		aValues.Add(_T(""));
		aValues.Add(_T(""));

		// Excluding empty items
		{
			ExpectEQ(Misc::FormatArray(aValues), _T(""));
			ExpectEQ(Misc::FormatArray(aValues, NULLSTRING), _T(""));
			ExpectEQ(Misc::FormatArray(aValues, _T("")), _T(""));

			ExpectEQ(Misc::FormatArray(aValues, _T(",")), _T(""));
			ExpectEQ(Misc::FormatArray(aValues, _T("...")), _T(""));

			ExpectTrue(ActualLengthMatchesCalculation(aValues, NULL));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T("")));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T(",")));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T("...")));
		}

		// Including empty items
		{
			CString sDefCompare = _T("") + DEFSEP + _T("") + DEFSEP + _T("");

			ExpectEQ(Misc::FormatArray(aValues, NULLSTRING, TRUE), sDefCompare);
			ExpectEQ(Misc::FormatArray(aValues, _T(""), TRUE), sDefCompare);

			ExpectEQ(Misc::FormatArray(aValues, _T(","), TRUE), _T(",,"));
			ExpectEQ(Misc::FormatArray(aValues, _T("..."), TRUE), _T("......"));

			ExpectTrue(ActualLengthMatchesCalculation(aValues, NULL, TRUE));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T(""), TRUE));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T(","), TRUE));
			ExpectTrue(ActualLengthMatchesCalculation(aValues, _T("..."), TRUE));
		}
	}
}

BOOL CMiscTest::ActualLengthMatchesCalculation(const CStringArray& aValues, LPCTSTR szSep, BOOL bIncEmpty)
{
	return (Misc::FormatArray(aValues, szSep, bIncEmpty).GetLength() == 
			Misc::GetFormattedLength(aValues, szSep, bIncEmpty));
}

void CMiscTest::TestHasPrefix()
{
	CTDCScopedTest test(*this, _T("Misc::HasPrefix"));

	// Note: FALSE for last argument is more permissive
	{
		// Empty arguments
		ExpectFalse(Misc::HasPrefix(_T(""), _T(""), TRUE));
		ExpectFalse(Misc::HasPrefix(_T(""), _T(""), FALSE));
		ExpectFalse(Misc::HasPrefix(_T(""), _T("abc"), TRUE));
		ExpectFalse(Misc::HasPrefix(_T(""), _T("abc"), FALSE));
		ExpectFalse(Misc::HasPrefix(_T("abc"), _T(""), TRUE));
		ExpectFalse(Misc::HasPrefix(_T("abc"), _T(""), FALSE));
	}

	{
		// Variable case
		ExpectTrue(Misc::HasPrefix(_T("a"), _T("abc"), TRUE));
		ExpectTrue(Misc::HasPrefix(_T("ab"), _T("abc"), TRUE));
		ExpectTrue(Misc::HasPrefix(_T("abc"), _T("abc"), TRUE));

		ExpectTrue(Misc::HasPrefix(_T("a"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasPrefix(_T("ab"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasPrefix(_T("abc"), _T("abc"), FALSE));

		ExpectFalse(Misc::HasPrefix(_T("A"), _T("abc"), TRUE));
		ExpectFalse(Misc::HasPrefix(_T("AB"), _T("abc"), TRUE));
		ExpectFalse(Misc::HasPrefix(_T("ABC"), _T("abc"), TRUE));

		ExpectTrue(Misc::HasPrefix(_T("A"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasPrefix(_T("AB"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasPrefix(_T("ABC"), _T("abc"), FALSE));

		ExpectTrue(Misc::HasPrefix(_T("A"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasPrefix(_T("Ab"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasPrefix(_T("AbC"), _T("abc"), FALSE));
	}

	{
		// Leading whitespace
		ExpectFalse(Misc::HasPrefix(_T("a"), _T(" abc"), FALSE));
		ExpectFalse(Misc::HasPrefix(_T("ab"), _T("\tabc"), FALSE));
		ExpectFalse(Misc::HasPrefix(_T("abc"), _T("\nabc"), FALSE));

		ExpectFalse(Misc::HasPrefix(_T("A"), _T(" abc"), FALSE));
		ExpectFalse(Misc::HasPrefix(_T("AB"), _T("\tabc"), FALSE));
		ExpectFalse(Misc::HasPrefix(_T("ABC"), _T("\nabc"), FALSE));
	}
}

void CMiscTest::TestHasSuffix()
{
	CTDCScopedTest test(*this, _T("Misc::HasSuffix"));

	// Note: FALSE for last argument is more permissive
	{
		// Empty arguments
		ExpectFalse(Misc::HasSuffix(_T(""), _T(""), TRUE));
		ExpectFalse(Misc::HasSuffix(_T(""), _T(""), FALSE));
		ExpectFalse(Misc::HasSuffix(_T(""), _T("abc"), TRUE));
		ExpectFalse(Misc::HasSuffix(_T(""), _T("abc"), FALSE));
		ExpectFalse(Misc::HasSuffix(_T("abc"), _T(""), TRUE));
		ExpectFalse(Misc::HasSuffix(_T("abc"), _T(""), FALSE));
	}

	{
		// Variable case
		ExpectTrue(Misc::HasSuffix(_T("c"), _T("abc"), TRUE));
		ExpectTrue(Misc::HasSuffix(_T("bc"), _T("abc"), TRUE));
		ExpectTrue(Misc::HasSuffix(_T("abc"), _T("abc"), TRUE));

		ExpectTrue(Misc::HasSuffix(_T("c"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasSuffix(_T("bc"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasSuffix(_T("abc"), _T("abc"), FALSE));

		ExpectFalse(Misc::HasSuffix(_T("C"), _T("abc"), TRUE));
		ExpectFalse(Misc::HasSuffix(_T("BC"), _T("abc"), TRUE));
		ExpectFalse(Misc::HasSuffix( _T("ABC"), _T("abc"),TRUE));

		ExpectTrue(Misc::HasSuffix(_T("C"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasSuffix(_T("BC"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasSuffix(_T("ABC"), _T("abc"), FALSE));

		ExpectTrue(Misc::HasSuffix(_T("C"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasSuffix(_T("bC"), _T("abc"), FALSE));
		ExpectTrue(Misc::HasSuffix(_T("AbC"), _T("abc"), FALSE));
	}

	{
		// Trailing whitespace
		ExpectFalse(Misc::HasSuffix(_T("c"), _T("abc "), FALSE));
		ExpectFalse(Misc::HasSuffix(_T("bc"), _T("abc\t"), FALSE));
		ExpectFalse(Misc::HasSuffix(_T("abc"), _T("abc\n"), FALSE));

		ExpectFalse(Misc::HasSuffix(_T("C"), _T("abc "), FALSE));
		ExpectFalse(Misc::HasSuffix(_T("BC"), _T("abc\t"), FALSE));
		ExpectFalse(Misc::HasSuffix(_T("ABC"), _T("abc\n"), FALSE));
	}
}

void  CMiscTest::TestAtof()
{
	CTDCScopedTest test(*this, _T("Misc::Atof"));
	
	TestAtof("C");
	TestAtof("en-GB");
	TestAtof("fr-FR");
	TestAtof("fr-BE");
	TestAtof("zh-CN");
	TestAtof("hu-HU");
	TestAtof("ar-DZ");
	TestAtof("fa-IR");

	// This fails on windows 7 probably because the locale is not supported
	if (COSVersion() >= OSV_WIN8)
		TestAtof("en-BE");
}

void CMiscTest::TestAtof(const CString& sLocale)
{
	ExpectEQ(CTempLocale::Current(), _T("C"));
	{
		CTempLocale loc(LC_ALL, sLocale);

#if _MSC_VER > 1200
		// This test fails under VC6 because of something in setlocale.
		ExpectEQ(CTempLocale::Current(), sLocale);
#endif

		ExpectEQ(Misc::Atof(_T("10.12345")), 10.12345);
		ExpectEQ(Misc::Atof(_T("-10.12345")), -10.12345);

		ExpectEQ(Misc::Atof(_T("10,12345")), 10.12345);
		ExpectEQ(Misc::Atof(_T("-10,12345")), -10.12345);

		ExpectEQ(Misc::Atof(_T("10'12345")), 10.0);
		ExpectEQ(Misc::Atof(_T("-10'12345")), -10.0);
	}
	ExpectEQ(CTempLocale::Current(), _T("C"));
}

void CMiscTest::TestIsNumber()
{
	// Invalid formats
	{
		ExpectFalse(Misc::IsNumber(_T("-")));
		ExpectFalse(Misc::IsNumber(_T("+")));
		ExpectFalse(Misc::IsNumber(_T("a")));
		ExpectFalse(Misc::IsNumber(_T("+a")));
		ExpectFalse(Misc::IsNumber(_T("5+")));
		ExpectFalse(Misc::IsNumber(_T("--7")));
		ExpectFalse(Misc::IsNumber(_T("3-")));
		ExpectFalse(Misc::IsNumber(_T("1.a")));
		ExpectFalse(Misc::IsNumber(_T("1#8")));
		ExpectFalse(Misc::IsNumber(_T("-3-5")));
		ExpectFalse(Misc::IsNumber(_T("+2+0")));
		ExpectFalse(Misc::IsNumber(_T("1.3.45")));
		ExpectFalse(Misc::IsNumber(_T("1..45")));
		ExpectFalse(Misc::IsNumber(_T("1.,45")));
	}

	// Valid formats
	{
		// Integer
		{
			ExpectTrue(Misc::IsNumber(_T("-1")));
			ExpectTrue(Misc::IsNumber(_T("+2")));
			ExpectTrue(Misc::IsNumber(_T("37983")));
			ExpectTrue(Misc::IsNumber(_T("45")));
		}

		// Period decimal
		{
			ExpectTrue(Misc::IsNumber(_T("-1.23")));
			ExpectTrue(Misc::IsNumber(_T("+2.56")));
			ExpectTrue(Misc::IsNumber(_T("3.89")));
			ExpectTrue(Misc::IsNumber(_T("456.789")));
			ExpectTrue(Misc::IsNumber(_T("-23.")));
			ExpectTrue(Misc::IsNumber(_T("+56.")));
			ExpectTrue(Misc::IsNumber(_T(".89")));
			ExpectTrue(Misc::IsNumber(_T("-.23")));
			ExpectTrue(Misc::IsNumber(_T("+.56")));
			ExpectTrue(Misc::IsNumber(_T("-23.")));
			ExpectTrue(Misc::IsNumber(_T("+56.")));
			ExpectTrue(Misc::IsNumber(_T("89.")));
		}

		// Comma decimal
		{
			ExpectTrue(Misc::IsNumber(_T("-1,23")));
			ExpectTrue(Misc::IsNumber(_T("+2,56")));
			ExpectTrue(Misc::IsNumber(_T("3,89")));
			ExpectTrue(Misc::IsNumber(_T("456,789")));
			ExpectTrue(Misc::IsNumber(_T("-,23")));
			ExpectTrue(Misc::IsNumber(_T("+,56")));
			ExpectTrue(Misc::IsNumber(_T(",89")));
			ExpectTrue(Misc::IsNumber(_T("-,23")));
			ExpectTrue(Misc::IsNumber(_T("+,56")));
			ExpectTrue(Misc::IsNumber(_T("-23,")));
			ExpectTrue(Misc::IsNumber(_T("+56,")));
			ExpectTrue(Misc::IsNumber(_T("89,")));
		}

		// User decimal
		{
			CString sDecSep = Misc::GetDecimalSeparator();

			ExpectTrue(Misc::IsNumber(_T("-1") + sDecSep + _T("23")));
			ExpectTrue(Misc::IsNumber(_T("+2") + sDecSep + _T("56")));
			ExpectTrue(Misc::IsNumber(_T("3") + sDecSep + _T("89")));
			ExpectTrue(Misc::IsNumber(_T("456") + sDecSep + _T("789")));
			ExpectTrue(Misc::IsNumber(_T("-") + sDecSep + _T("23")));
			ExpectTrue(Misc::IsNumber(_T("+") + sDecSep + _T("56")));
			ExpectTrue(Misc::IsNumber(sDecSep + _T("89")));
		}
	}
}

void CMiscTest::TestRemoveDuplicates()
{
	CTDCScopedTest test(*this, _T("Misc::RemoveDuplicates"));

	// CStringArray
	{
		// Case sensitive
		{
			// No duplicates exist
			{
				CStringArray aItems;

				aItems.Add(_T("abc"));
				aItems.Add(_T("abC"));
				aItems.Add(_T("Abc"));
				aItems.Add(_T("ABC"));
				aItems.Add(_T("aBC"));
				aItems.Add(_T("AbC"));
				aItems.Add(_T("ABc"));

				ExpectEQ(Misc::RemoveDuplicates(aItems, TRUE), 0);
				ExpectEQ(aItems.GetSize(), 7);

				ExpectEQ(aItems[0], _T("abc"));
				ExpectEQ(aItems[1], _T("abC"));
				ExpectEQ(aItems[2], _T("Abc"));
				ExpectEQ(aItems[3], _T("ABC"));
				ExpectEQ(aItems[4], _T("aBC"));
				ExpectEQ(aItems[5], _T("AbC"));
				ExpectEQ(aItems[6], _T("ABc"));
			}

			// Some duplicates exist
			{
				CStringArray aItems;

				aItems.Add(_T("abc"));
				aItems.Add(_T("abC"));
				aItems.Add(_T("abc")); // dupe
				aItems.Add(_T("Abc"));
				aItems.Add(_T("ABC"));
				aItems.Add(_T("aBC"));
				aItems.Add(_T("ABC")); // dupe
				aItems.Add(_T("AbC"));
				aItems.Add(_T("ABc"));
				aItems.Add(_T("AbC")); // dupe
				aItems.Add(_T("abc")); // dupe

				ExpectEQ(Misc::RemoveDuplicates(aItems, TRUE), 4);
				ExpectEQ(aItems.GetSize(), 7);

				ExpectEQ(aItems[0], _T("abc"));
				ExpectEQ(aItems[1], _T("abC"));
				ExpectEQ(aItems[2], _T("Abc"));
				ExpectEQ(aItems[3], _T("ABC"));
				ExpectEQ(aItems[4], _T("aBC"));
				ExpectEQ(aItems[5], _T("AbC"));
				ExpectEQ(aItems[6], _T("ABc"));
			}
		}

		// Case Insensitive
		{
			CStringArray aItems;

			aItems.Add(_T("abc"));
			aItems.Add(_T("abC"));
			aItems.Add(_T("Abc"));
			aItems.Add(_T("ABC"));
			aItems.Add(_T("aBC"));
			aItems.Add(_T("AbC"));
			aItems.Add(_T("ABc"));

			ExpectEQ(Misc::RemoveDuplicates(aItems, FALSE), 6);
			ExpectEQ(aItems.GetSize(), 1);

			ExpectEQ(aItems[0], _T("abc")); // only the first item remains
		}
	}

	// CDWordArray
	{
		// No duplicates exist
		{
			CDWordArray aItems;

			aItems.Add(1);
			aItems.Add(2);
			aItems.Add(3);
			aItems.Add(5);
			aItems.Add(7);
			aItems.Add(11);
			aItems.Add(13);

			ExpectEQ(Misc::RemoveDuplicates(aItems), 0);
			ExpectEQ(aItems.GetSize(), 7);

			ExpectEQ(aItems[0], 1UL);
			ExpectEQ(aItems[1], 2UL);
			ExpectEQ(aItems[2], 3UL);
			ExpectEQ(aItems[3], 5UL);
			ExpectEQ(aItems[4], 7UL);
			ExpectEQ(aItems[5], 11UL);
			ExpectEQ(aItems[6], 13UL);
		}

		// Some duplicates exist
		{
			CDWordArray aItems;

			aItems.Add(1);
			aItems.Add(2);
			aItems.Add(1);  // dupe
			aItems.Add(3);
			aItems.Add(5);
			aItems.Add(7);
			aItems.Add(5);  // dupe
			aItems.Add(11);
			aItems.Add(13);
			aItems.Add(11); // dupe
			aItems.Add(5);  // dupe

			ExpectEQ(Misc::RemoveDuplicates(aItems), 4);
			ExpectEQ(aItems.GetSize(), 7);

			ExpectEQ(aItems[0], 1UL);
			ExpectEQ(aItems[1], 2UL);
			ExpectEQ(aItems[2], 3UL);
			ExpectEQ(aItems[3], 5UL);
			ExpectEQ(aItems[4], 7UL);
			ExpectEQ(aItems[5], 11UL);
			ExpectEQ(aItems[6], 13UL);
		}
	}
}

void CMiscTest::TestRemoveItems()
{
	CTDCScopedTest test(*this, _T("Misc::RemoveItems"));

	// Case sensitive
	{
		// No matches
		{
			CStringArray aItems, aFrom;

			aFrom.Add(_T("abc"));
			aFrom.Add(_T("abC"));
			aFrom.Add(_T("Abc"));

			aItems.Add(_T("ABC"));
			aItems.Add(_T("aBC"));
			aItems.Add(_T("AbC"));
			aItems.Add(_T("ABc"));

			ExpectEQ(Misc::RemoveItems(aItems, aFrom, TRUE), 0);
			ExpectEQ(aFrom.GetSize(), 3);

			ExpectEQ(aFrom[0], _T("abc"));
			ExpectEQ(aFrom[1], _T("abC"));
			ExpectEQ(aFrom[2], _T("Abc"));
		}

		// Some matches
		{
			CStringArray aItems, aFrom;

			aFrom.Add(_T("abc"));
			aFrom.Add(_T("abC"));
			aFrom.Add(_T("Abc"));
			aFrom.Add(_T("ABC"));
			aFrom.Add(_T("aBC"));
			aFrom.Add(_T("AbC"));
			aFrom.Add(_T("ABc"));
			aFrom.Add(_T("cde"));

			aItems.Add(_T("abc")); // match
			aItems.Add(_T("ABC")); // match
			aItems.Add(_T("AbC")); // match
			aItems.Add(_T("abc")); // match & dupe
			aItems.Add(_T("bcd")); // NO match

			ExpectEQ(Misc::RemoveItems(aItems, aFrom, TRUE), 3);
			ExpectEQ(aFrom.GetSize(), 5);

			ExpectEQ(aFrom[0], _T("abC"));
			ExpectEQ(aFrom[1], _T("Abc"));
			ExpectEQ(aFrom[2], _T("aBC"));
			ExpectEQ(aFrom[3], _T("ABc"));
			ExpectEQ(aFrom[4], _T("cde"));
		}
	}

	// Case Insensitive
	{
		CStringArray aItems, aFrom;

		aFrom.Add(_T("abc"));
		aFrom.Add(_T("abC"));
		aFrom.Add(_T("Abc"));
		aFrom.Add(_T("ABC"));
		aFrom.Add(_T("aBC"));
		aFrom.Add(_T("AbC"));
		aFrom.Add(_T("ABc"));
		aFrom.Add(_T("bcd"));

		aItems.Add(_T("ABC"));

		ExpectEQ(Misc::RemoveItems(aItems, aFrom, TRUE), 7);
		ExpectEQ(aFrom.GetSize(), 1);

		ExpectEQ(aFrom[0], _T("bcd"));
	}
}

void CMiscTest::TestAppendItems()
{
	CTDCScopedTest test(*this, _T("Misc::AppendItems"));

	// Because what's interesting about this function is that
	// it can call RemoveDuplicates, we don't need to exhaustively
	// test that aspect of it

	// No Remove duplicates
	{
		CStringArray aTo, aFrom;

		aFrom.Add(_T("abc"));
		aFrom.Add(_T("abC"));
		aFrom.Add(_T("Abc"));

		aTo.Add(_T("ABC"));
		aTo.Add(_T("aBC"));
		aTo.Add(_T("AbC"));
		aTo.Add(_T("ABc"));
		aTo.Add(_T("abc")); // dupe
		aTo.Add(_T("abC")); // dupe
		aTo.Add(_T("Abc")); // dupe

		ExpectEQ(Misc::AppendItems(aFrom, aTo, FALSE), 7);
		ExpectEQ(aFrom.GetSize(), 10);

		ExpectEQ(aTo[0], _T("abc"));
		ExpectEQ(aTo[1], _T("abC"));
		ExpectEQ(aTo[2], _T("Abc"));
		ExpectEQ(aTo[3], _T("ABC"));
		ExpectEQ(aTo[4], _T("aBC"));
		ExpectEQ(aTo[5], _T("AbC"));
		ExpectEQ(aTo[6], _T("ABc"));
		ExpectEQ(aTo[7], _T("abc"));
		ExpectEQ(aTo[8], _T("abC"));
		ExpectEQ(aTo[9], _T("Abc"));
	}

	// Remove duplicates case-sensitively
	{
		CStringArray aTo, aFrom;

		aFrom.Add(_T("abc"));
		aFrom.Add(_T("abC"));
		aFrom.Add(_T("Abc"));

		aTo.Add(_T("ABC"));
		aTo.Add(_T("aBC"));
		aTo.Add(_T("AbC"));
		aTo.Add(_T("ABc"));
		aTo.Add(_T("abc")); // dupe
		aTo.Add(_T("abC")); // dupe
		aTo.Add(_T("Abc")); // dupe

		ExpectEQ(Misc::AppendItems(aFrom, aTo, TRUE, TRUE), 4);
		ExpectEQ(aFrom.GetSize(), 7);

		ExpectEQ(aTo[0], _T("abc"));
		ExpectEQ(aTo[1], _T("abC"));
		ExpectEQ(aTo[2], _T("Abc"));
		ExpectEQ(aTo[3], _T("ABC"));
		ExpectEQ(aTo[4], _T("aBC"));
		ExpectEQ(aTo[5], _T("AbC"));
		ExpectEQ(aTo[6], _T("ABc"));
	}

	// Remove duplicates case-insensitively
	{
		CStringArray aTo, aFrom;

		aFrom.Add(_T("abc"));
		aFrom.Add(_T("abC"));
		aFrom.Add(_T("Abc"));

		aTo.Add(_T("ABC"));
		aTo.Add(_T("aBC"));
		aTo.Add(_T("AbC"));
		aTo.Add(_T("ABc"));
		aTo.Add(_T("abc")); // dupe
		aTo.Add(_T("abC")); // dupe
		aTo.Add(_T("Abc")); // dupe

		ExpectEQ(Misc::AppendItems(aFrom, aTo, TRUE, FALSE), 0);
		ExpectEQ(aFrom.GetSize(), 1);

		ExpectEQ(aTo[0], _T("abc"));
	}
}

void CMiscTest::TestSplitLines()
{
	CTDCScopedTest test(*this, _T("Misc::SplitLines"));

	const CString PARA_1 = _T("ToDoList can be navigated equally easily using the keyboard or the mouse and uses standard Windows keyboard and mouse actions for navigating.");
	const CString PARA_2 = _T("In addition to this,	ToDoList keeps track of your navigation history so that you can easily track backwards and forwards in a similar way to browsing the web.");
	const CString PARA_3 = _T("ToDoList also supports multiple selection of tasks for editing, moving, deleting, exporting and printing.");
	const CString PARA_4 = _T("1. Local files/folders\n2. Network files\\folders\n3. Internet addresses(www.)\n4. Email addresses(mailto://)");

	// No maximum line length
	{
		{
			const CString sText = (PARA_1 + '\n' + PARA_2 + '\n' + PARA_3);
			CStringArray aLines;

			ExpectEQ(Misc::SplitLines(sText, aLines), 3);
			ExpectEQ(aLines[0], PARA_1);
			ExpectEQ(aLines[1], PARA_2);
			ExpectEQ(aLines[2], PARA_3);
		}

		{
			const CString sText = (PARA_1 + PARA_2 + PARA_3);
			CStringArray aLines;

			ExpectEQ(Misc::SplitLines(sText, aLines), 1);
			ExpectEQ(aLines[0], sText);
		}

		{
			const CString sText = (PARA_1 + _T("\n\n\n") + PARA_2 + _T("\n\n") + PARA_3);
			CStringArray aLines;

			ExpectEQ(Misc::SplitLines(sText, aLines), 6);
			ExpectEQ(aLines[0], PARA_1);
			ExpectEQ(aLines[1], EMPTYSTRING);
			ExpectEQ(aLines[2], EMPTYSTRING);
			ExpectEQ(aLines[3], PARA_2);
			ExpectEQ(aLines[4], EMPTYSTRING);
			ExpectEQ(aLines[5], PARA_3);
		}
	}

	// With maximum line length
	{
		{
			CStringArray aLines;
			const int MAXLINELEN = 50;

			ExpectEQ(Misc::SplitLines(PARA_1, aLines, MAXLINELEN), 3);
			ExpectEQ(aLines[0], _T("ToDoList can be navigated equally easily using the "));
			ExpectEQ(aLines[1], _T("keyboard or the mouse and uses standard Windows "));
			ExpectEQ(aLines[2], _T("keyboard and mouse actions for navigating."));

			int nTotalLen = 0;

			for (int nLine = 0; nLine < aLines.GetSize(); nLine++)
			{
				// The line length can include the trailing delimiter
				ExpectTrue(aLines[nLine].GetLength() <= (MAXLINELEN + 1));
				nTotalLen += (aLines[nLine].GetLength());
			}

			ExpectEQ(nTotalLen, PARA_1.GetLength());
		}

		{
			CStringArray aLines;
			const int MAXLINELEN = 20;

			ExpectEQ(Misc::SplitLines(PARA_4, aLines, MAXLINELEN), 8);
			ExpectEQ(aLines[0], _T("1. Local files/"));
			ExpectEQ(aLines[1], _T("folders"));
			ExpectEQ(aLines[2], _T("2. Network files\\"));
			ExpectEQ(aLines[3], _T("folders"));
			ExpectEQ(aLines[4], _T("3. Internet "));
			ExpectEQ(aLines[5], _T("addresses(www.)"));
			ExpectEQ(aLines[6], _T("4. Email "));
			ExpectEQ(aLines[7], _T("addresses(mailto://)"));

			int nTotalLen = 0;

			for (int nLine = 0; nLine < aLines.GetSize(); nLine++)
			{
				// The line length can include the trailing delimiter
				ExpectTrue(aLines[nLine].GetLength() <= (MAXLINELEN + 1));
				nTotalLen += (aLines[nLine].GetLength());
			}

			ExpectEQ(nTotalLen, PARA_4.GetLength() - 3); // embedded newlines not included
		}
	}
}

///////////////////////////////////////////////////////////////////////

void CMiscTest::TestRegionalSettingsRetrievalPerformance()
{
	if (!m_utils.GetWantPerformanceTests())
	{
		_tprintf(_T("Add '-p' to run CMiscTest::TestRegionalSettingsPerformance\n"));
		return;
	}

	CTDCScopedTest test(*this, _T("RegionalSettingsRetrievalPerformance"));

	CString sSetting;
	const int NUM_LOOPS = 100000;
	DWORD dwTickStart = GetTickCount();

	for (int nLoop = 0; nLoop <= NUM_LOOPS; nLoop++)
	{
		sSetting = Misc::GetListSeparator();
		sSetting = Misc::GetDecimalSeparator();
		sSetting = Misc::GetDefCharset();
		sSetting = Misc::GetAM();
		sSetting = Misc::GetPM();
		sSetting = Misc::GetTimeSeparator();
		sSetting = Misc::GetTimeFormat();
		sSetting = Misc::GetShortDateFormat();
		sSetting = Misc::GetDateSeparator();
	}

	_tprintf(_T("Test took %ld ms to retrieve 9 settings %d times\n"), (GetTickCount() - dwTickStart), NUM_LOOPS);
}
