// OdbcTasklistStorage.h: interface for the COdbcTasklistStorage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCSTATIC_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_)
#define AFX_ODBCSTATIC_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Shared\databaseEx.h"
#include "..\Shared\recordsetEx.h"

#include "..\todolist\tdcenum.h"

//////////////////////////////////////////////////////////////////////

struct ODBCTASK;
struct ODBCSETUP;
struct ODBCTABLESETUP;
struct ODBCDATABASESETUP;
struct ODBCATTRIBUTESETUP;

class CDBVariant;
class COdbcTableComboBox;
class COdbcFieldComboBox;
class COdbcValueComboBox;

typedef CArray<ODBCDATABASESETUP, ODBCDATABASESETUP&> COdbcDatabaseSetupArray;

//////////////////////////////////////////////////////////////////////

class COdbcHelper  
{
public:
	static CString FormatName(const ODBCDATABASESETUP& dbSetup, DBEX_FORMAT nFmt = DBE_FMT_DSN_DB);

	static int FindDatabase(const ODBCDATABASESETUP& dbSetup, const COdbcDatabaseSetupArray& aDbSetup);
	static int FindDatabase(const CString& sConnect, const COdbcDatabaseSetupArray& aDbSetup);

	static BOOL VerifySetup(const ODBCSETUP& setup);
	static BOOL VerifySetup(const ODBCSETUP& setup, CString& sErrors);

	static BOOL VerifySetup(const ODBCDATABASESETUP& dbSetup);
	static BOOL VerifySetup(const ODBCDATABASESETUP& dbSetup, CString& sErrors);

	static BOOL GetTaskAttributeSetup(const ODBCSETUP& setup, TDC_ATTRIBUTE nAttrib, ODBCATTRIBUTESETUP& attrib);
	static int FindTaskAttributeSetup(const ODBCSETUP& setup, TDC_ATTRIBUTE nAttrib);

	static CString FormatFieldsForQuery(const ODBCSETUP& setup);
	static CString FormatFieldsForInsert(const ODBCSETUP& setup);
	static CString FormatFieldsAndValuesForUpdate(const ODBCSETUP& setup, const ODBCTASK& task, const CODBCFieldInfoArray& aInfo);
	static CString FormatValuesForInsert(const ODBCSETUP& setup, const ODBCTASK& task, const CODBCFieldInfoArray& aInfo, const CString& sTasklistID);
	static CString FormatFieldsAndValuesForParameterizedUpdate(const ODBCSETUP& setup, const ODBCTASK& task, const CODBCFieldInfoArray& aInfo, CDBVariantArray& aParams, BOOL bAppend = FALSE);
	static CString FormatValuesForParameterizedInsert(const ODBCSETUP& setup, const ODBCTASK& task, const CODBCFieldInfoArray& aInfo, const CString& sTasklistID, CDBVariantArray& aParams, BOOL bAppend = FALSE);

	static void MapAttributeValues(const ODBCSETUP& setup, const ODBCTASK& task, CMapStringToStringArray& map);

	static CString& AppendAttributeToList(const CString& sAttrib, CString& sAttribs);
	static CString& AppendAttributeToList(const CString& sAttrib, const CString& sValue, CString& sAttribs);

	static void InitCombo(COdbcValueComboBox& cbValues, CDatabaseEx& db, 
							const ODBCTABLESETUP& table, BOOL bAddEmptyItem = FALSE);
	static void InitCombo(COdbcFieldComboBox& cbFields, CDatabaseEx& db, 
							const ODBCTABLESETUP& table, BOOL bKeyField, BOOL bAddEmptyItem = FALSE);
	static void InitCombo(COdbcTableComboBox& cbTables, CDatabaseEx& db, 
							const ODBCTABLESETUP& table, BOOL bAddEmptyItem = FALSE);

	static void InitCombo(COdbcFieldComboBox& cbFields, CDatabaseEx& db, 
							const CString& sTable, const CString& sField, BOOL bAddEmptyItem = FALSE);
	static void InitCombo(COdbcTableComboBox& cbTables, CDatabaseEx& db, 
							const CString& sTable, BOOL bAddEmptyItem = FALSE);

	static CString FormatDateAsSQLString(time64_t tDate, const CODBCFieldInfo& info);
	static CString GetAttributeValueAsSQLString(const ODBCTASK& task, TDC_ATTRIBUTE nAttrib, const CODBCFieldInfo& info);
	static CDBVariant* GetAttributeValue(const ODBCTASK& task, TDC_ATTRIBUTE nAttrib);
	static CString FormatValueAsSQLString(const CString& sKeyValue, const CODBCFieldInfo& info, LPCTSTR szDefValue = _T(""));
	static CString QuoteValue(const CString& sValue, const CODBCFieldInfo& info);
	static CString QuoteValue(const CString& sValue, int nSQLType);

	static void SetVarValue(const CString& sValue, CDBVariant& var);
	static void SetVarValue(const time64_t& tValue, CDBVariant& var);
	static void SetVarValue(const int& nValue, CDBVariant& var);
	static void SetVarValue(const long& lValue, CDBVariant& var);
	static void SetVarValue(const double& dValue, CDBVariant& var);
};

#endif // !defined(AFX_ODBCSTATIC_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_)
