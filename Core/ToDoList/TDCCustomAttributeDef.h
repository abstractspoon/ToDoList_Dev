#if !defined(AFX_TDCCUSTOMATTRIBUTEDEF_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCCUSTOMATTRIBUTEDEF_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////////////////

#include "tdcenumcontainers.h"

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCADATA;

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCUSTOMATTRIBUTECALCULATIONOPERAND
{
	TDCCUSTOMATTRIBUTECALCULATIONOPERAND();

	BOOL operator==(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const;

	void Clear();
	BOOL Set(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op);
	BOOL Set( TDC_ATTRIBUTE nAttribID, const CString& sCustAttribID);

	BOOL IsValid(BOOL bAllowNone = TRUE) const;
	BOOL IsCustom() const;

	TDC_ATTRIBUTE nAttributeID; // TDCA_CUSTOMATTRIBUTE for all custom attributes
	CString sCustAttribID;

	static BOOL IsValid(TDC_ATTRIBUTE nAttribID, const CString& sCustAttribID, BOOL bAllowNone = TRUE);
	static DWORD GetDataType(TDC_ATTRIBUTE nAttribID);
};

/////////////////////////////////////////////////////////////////////////////////////////////

const DWORD TDCCA_INVALID = TDCCA_STRING;

// -------------------------------------------------------

struct TDCCUSTOMATTRIBUTECALCULATION
{
	TDCCUSTOMATTRIBUTECALCULATION();

	BOOL operator==(const TDCCUSTOMATTRIBUTECALCULATION& calc) const;

	void Clear();
	BOOL IsValid(BOOL bAllowNone = TRUE) const;

	BOOL Set(const TDCCUSTOMATTRIBUTECALCULATION& calc);

	BOOL Set(TDC_ATTRIBUTE nFirstOpAttribID,
			 const CString& sFirstOpCustAttribID,
			 TDCCA_CALC_OPERATOR nOp,
			 TDC_ATTRIBUTE nSecondOpAttribID,
			 const CString& sSecondOpCustAttribID);

	BOOL Set(TDC_ATTRIBUTE nFirstOpAttribID,
			 const CString& sFirstOpCustAttribID,
			 TDCCA_CALC_OPERATOR nOp,
			 double dSecondOpValue);

	BOOL IsFirstOperandCustom() const;
	BOOL IsSecondOperandCustom() const;
	BOOL IsSecondOperandValue() const;
	
	static BOOL IsValidOperator(TDCCA_CALC_OPERATOR nOperator);
	static BOOL IsValidOperator(TDCCA_CALC_OPERATOR nOperator, DWORD dwFirstOpDataType, DWORD dwSecondOpDataType);

	static DWORD GetResultDataType(DWORD dwFirstOpDataType, TDCCA_CALC_OPERATOR nOperator, DWORD dwSecondOpDataType);

public:
	TDCCUSTOMATTRIBUTECALCULATIONOPERAND opFirst;
	TDCCA_CALC_OPERATOR nOperator;

	TDCCUSTOMATTRIBUTECALCULATIONOPERAND opSecond;
	double dSecondOperandValue;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCUSTOMATTRIBUTEDEFINITION
{
	friend class CTDCCustomAttribDefinitionArray;

	TDCCUSTOMATTRIBUTEDEFINITION(LPCTSTR szLabel = NULL, BOOL bEnable = TRUE);
	TDCCUSTOMATTRIBUTEDEFINITION(const TDCCUSTOMATTRIBUTEDEFINITION& otherDef);
	TDCCUSTOMATTRIBUTEDEFINITION& operator=(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);

	BOOL operator==(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;
	BOOL operator!=(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;
	BOOL Matches(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bIncAutoListData = FALSE) const;

	CString GetColumnTitle() const;
	CString GetToolTip() const;

	inline TDC_COLUMN GetColumnID() const { return nColumnID; }
	inline TDC_ATTRIBUTE GetAttributeID() const { return nAttributeID; }
	inline DWORD GetAttributeType() const { return dwAttribType; }

	UINT GetColumnHeaderAlignment() const;
	BOOL HasDefaultTextAlignment() const;

	BOOL SetAttributeType(DWORD dwType);
	BOOL SetDataType(DWORD dwDataType, BOOL bUpdateDefaultAlignment = TRUE);
	BOOL SetListType(DWORD dwListType);

	inline DWORD GetDataType() const { return (dwAttribType & TDCCA_DATAMASK); }
	inline DWORD GetListType() const { return (dwAttribType & TDCCA_LISTMASK); }

	inline BOOL IsDataType(DWORD dwDataType) const { return (GetDataType() == dwDataType); }
	inline BOOL IsList() const { return (GetListType() != TDCCA_NOTALIST); }
	inline BOOL IsAutoList() const { return ((GetListType() == TDCCA_AUTOLIST) || (GetListType() == TDCCA_AUTOMULTILIST)); }
	inline BOOL IsMultiList() const { return ((GetListType() == TDCCA_FIXEDMULTILIST) || (GetListType() == TDCCA_AUTOMULTILIST)); }
	inline BOOL IsFixedList() const { return ((GetListType() == TDCCA_FIXEDLIST) || (GetListType() == TDCCA_FIXEDMULTILIST)); }

	int GetUniqueListData(CStringArray& aData) const;
	CString EncodeListData() const;
	BOOL DecodeListData(const CString& sListData);
	BOOL GetListIconName(const CString& sImage, CString& sName) const;

	BOOL SupportsFeature(DWORD dwFeature) const;
	BOOL HasFeature(DWORD dwFeature) const;
	BOOL IsAggregated() const;
	BOOL ValidateData(TDCCADATA& data) const;

	BOOL SetCalculation(const TDCCUSTOMATTRIBUTECALCULATION& calc);
	BOOL IsCalculation() const { return calculation.IsValid(FALSE); }
	const TDCCUSTOMATTRIBUTECALCULATION& Calculation() const { return calculation; }

	CString GetNextListItem(const CString& sItem, BOOL bNext) const;
	CString GetImageName(const CString& sImage) const;

	CString FormatData(const TDCCADATA& data, BOOL bISODates, TCHAR cListSep = 0) const;
	CString FormatNumber(double dValue) const;
	CString FormatTimePeriod(const TDCCADATA& data) const { return FormatTimePeriod(data, dwFeatures); }
	BOOL GetDataAsDouble(const TDCCADATA& data, double& dValue, TDC_UNITS nUnits) const;

	int CalcLongestListItem(CDC* pDC) const;

	static BOOL IsCustomAttribute(TDC_ATTRIBUTE nAttribID);
	static BOOL IsCustomColumn(TDC_COLUMN nColID);
	static UINT GetDefaultHorzAlignment(DWORD dwAttribType);
	static TDC_ATTRIBUTEGROUP GetAttributeGroup(DWORD dwAttribType);
	static BOOL IsEncodedImageTag(const CString& sImage);
	static CString EncodeImageTag(const CString& sImage, const CString& sName);
	static BOOL DecodeImageTag(const CString& sTag, CString& sImage, CString& sName);
	static BOOL AttributeSupportsFeature(DWORD dwDataType, DWORD dwListType, DWORD dwFeature);
	static CString FormatNumber(double dValue, DWORD dwDataType, DWORD dwFeatures);
	static CString FormatTimePeriod(const TDCCADATA& data, DWORD dwFeatures);

	// ----------------------------------------------------------------
	CString sUniqueID;
	CString sColumnTitle;
	CString sLabel;
	UINT nTextAlignment; // DT_LEFT, DT_CENTER, DT_RIGHT
	DWORD dwFeatures;
	CStringArray aDefaultListData;
	mutable CStringArray aAutoListData;
	BOOL bEnabled;

private:
	// these are managed internally
	DWORD dwAttribType;
	TDC_COLUMN nColumnID;
	TDC_ATTRIBUTE nAttributeID;
	TDCCUSTOMATTRIBUTECALCULATION calculation;

	// ----------------------------------------------------------------

	BOOL SetTypes(DWORD dwDataType, DWORD dwListType);
	static DWORD ValidateListType(DWORD dwDataType, DWORD dwListType);
};

////////////////////////////////////////////////////////////////////////////

#define GET_CUSTDEF_RET(defs, key, def, ret) \
{                                            \
	int att = (defs).Find(key);              \
	if (att == -1)                           \
	{                                        \
		ASSERT(0);                           \
        return ret;                          \
	}                                        \
	def = &((defs)[att]);                    \
}

// alt = break. continue, return
#define GET_CUSTDEF_ALT(defs, key, def, alt) \
{                                            \
	int att = defs.Find(key);                \
	if (att == -1)                           \
	{                                        \
		ASSERT(0);                           \
        alt;                                 \
	}                                        \
	def = &((defs)[att]);                    \
}

// ----------------------------------------------------------------

class CTDCCustomAttribDefinitionArray : public CArray<TDCCUSTOMATTRIBUTEDEFINITION, TDCCUSTOMATTRIBUTEDEFINITION&>
{
public:
	CTDCCustomAttribDefinitionArray();
	CTDCCustomAttribDefinitionArray(const CTDCCustomAttribDefinitionArray& other);

	int Add(TDCCUSTOMATTRIBUTEDEFINITION& newElement);
	int Append(const CTDCCustomAttribDefinitionArray& aSrc);

	void InsertAt(int nIndex, TDCCUSTOMATTRIBUTEDEFINITION& newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);

	int Find(const CString& sAttribID, int nIgnore = -1) const;
	int Find(TDC_ATTRIBUTE nCustAttribID, int nIgnore = -1) const;
	int Find(TDC_COLUMN nCustColID, int nIgnore = -1) const;

	BOOL AnyHasFeature(DWORD dwFeature) const;
	BOOL MatchAny(const CTDCCustomAttribDefinitionArray& aAttribDefs) const;
	
	TDC_ATTRIBUTE GetAttributeID(TDC_COLUMN nCustColID) const;
	TDC_ATTRIBUTE GetAttributeID(const CString& sCustAttribID) const;

	CString GetAttributeTypeID(TDC_COLUMN nCustColID) const;
	CString GetAttributeTypeID(TDC_ATTRIBUTE nCustAttribID) const;

	int GetVisibleColumnIDs(CTDCColumnIDMap& mapCols, BOOL bAppend) const;

	CString FormatData(const TDCCADATA& data, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bISODates) const;
	CString FormatData(const TDCCADATA& data, const CString& sCustAttribID, BOOL bISODates) const;

	DWORD GetAttributeDataType(TDC_ATTRIBUTE nCustAttribID, BOOL bResolveCalcType = TRUE) const;
	DWORD GetAttributeDataType(TDC_COLUMN nCustColID, BOOL bResolveCalcType = TRUE) const;
	DWORD GetAttributeDataType(const CString& sCustAttribID, BOOL bResolveCalcType = TRUE) const;
	DWORD GetAttributeDataType(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bResolveCalcType = TRUE) const;

	BOOL IsListType(TDC_ATTRIBUTE nCustAttribID) const;
	BOOL IsListType(TDC_COLUMN nCustColID) const;
	BOOL IsListType(const CString& sCustAttribID) const;

	BOOL IsColumnSortable(TDC_COLUMN nCustColID) const;
	BOOL IsColumnEnabled(TDC_COLUMN nCustColID) const;

	// Calculation helpers requiring access to all attribute definitions
	BOOL IsValidCalculation(const TDCCUSTOMATTRIBUTECALCULATION& calc, BOOL bAllowNone = TRUE) const;
	DWORD GetCalculationOperandDataType(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const;
	DWORD GetCalculationResultDataType(const TDCCUSTOMATTRIBUTECALCULATION& calc) const;
	BOOL CalculationHasFeature(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, DWORD dwFeature) const;
	BOOL AnyCalculationUsesAnyAttribute(const CTDCAttributeMap& mapAttribIDs) const;

	// VC6 fixes
	const TDCCUSTOMATTRIBUTEDEFINITION& operator[](int nIndex) const { return ElementAt(nIndex); }
	TDCCUSTOMATTRIBUTEDEFINITION& operator[](int nIndex) { return ElementAt(nIndex); }

protected:
	void RebuildIDs();

	const TDCCUSTOMATTRIBUTEDEFINITION& ElementAt(int nIndex) const { return GetData()[nIndex]; }
	TDCCUSTOMATTRIBUTEDEFINITION& ElementAt(int nIndex) { return GetData()[nIndex]; }

	const TDCCUSTOMATTRIBUTEDEFINITION& GetDefinition(TDC_ATTRIBUTE nCustAttribID) const;
	const TDCCUSTOMATTRIBUTEDEFINITION& GetDefinition(TDC_COLUMN nCustColID) const;
	const TDCCUSTOMATTRIBUTEDEFINITION& GetDefinition(const CString& sCustAttribID) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCCUSTOMATTRIBUTEDEF_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
