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

	TDC_ATTRIBUTE nAttribID; // TDCA_CUSTOMATTRIBUTE for all custom attributes
	CString sCustAttribID;

	static BOOL IsValid(TDC_ATTRIBUTE nAttribID, const CString& sCustAttribID, BOOL bAllowNone = TRUE);
	static DWORD GetDataType(TDC_ATTRIBUTE nAttribID);

};

/////////////////////////////////////////////////////////////////////////////////////////////

const DWORD TDCCA_INVALID = TDCCA_STRING;

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

	TDCCUSTOMATTRIBUTEDEFINITION(LPCTSTR szLabel = NULL);
	TDCCUSTOMATTRIBUTEDEFINITION(const TDCCUSTOMATTRIBUTEDEFINITION& otherDef);
	TDCCUSTOMATTRIBUTEDEFINITION& operator=(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);

	BOOL operator==(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;
	BOOL operator!=(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;
	BOOL Matches(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bIncAutoListData = FALSE) const;

	CString GetColumnTitle() const;
	CString GetToolTip() const;

	inline TDC_COLUMN GetColumnID() const { return nColID; }
	inline TDC_ATTRIBUTE GetAttributeID() const { return nAttribID; }
	inline DWORD GetAttributeType() const { return dwAttribType; }

	UINT GetColumnHeaderAlignment() const;
	BOOL HasDefaultHorzAlignment() const;

	BOOL SetAttributeType(DWORD dwType);
	BOOL SetDataType(DWORD dwDataType, BOOL bUpdateDefaultAlignment = TRUE);
	BOOL SetListType(DWORD dwListType);

	inline DWORD GetDataType() const { return (dwAttribType & TDCCA_DATAMASK); }
	inline DWORD GetListType() const { return (dwAttribType & TDCCA_LISTMASK); }
	inline BOOL HasFeature(DWORD dwFeature) const { return SupportsFeature(dwFeature) && (dwFeatures & dwFeature); }

	inline BOOL IsDataType(DWORD dwDataType) const { return (GetDataType() == dwDataType); }
	inline BOOL IsList() const { return (GetListType() != TDCCA_NOTALIST); }
	inline BOOL IsAutoList() const { return ((GetListType() == TDCCA_AUTOLIST) || (GetListType() == TDCCA_AUTOMULTILIST)); }
	inline BOOL IsMultiList() const { return ((GetListType() == TDCCA_FIXEDMULTILIST) || (GetListType() == TDCCA_AUTOMULTILIST)); }
	inline BOOL IsFixedList() const { return ((GetListType() == TDCCA_FIXEDLIST) || (GetListType() == TDCCA_FIXEDMULTILIST)); }

	int GetUniqueListData(CStringArray& aData) const;
	CString EncodeListData() const;
	BOOL DecodeListData(const CString& sListData);

	BOOL SupportsFeature(DWORD dwFeature) const;
	BOOL IsAggregated() const;

	BOOL SetCalculation(const TDCCUSTOMATTRIBUTECALCULATION& calc);
	const TDCCUSTOMATTRIBUTECALCULATION& Calculation() const { return calculation; }

	CString GetNextListItem(const CString& sItem, BOOL bNext) const;
	CString GetImageName(const CString& sImage) const;

	CString FormatData(const TDCCADATA& data, BOOL bISODates, TCHAR cListSep = 0) const;
	CString FormatNumber(double dValue) const;
	BOOL GetDataAsDouble(const TDCCADATA& data, double& dValue, TDC_UNITS nUnits) const;

	int CalcLongestListItem(CDC* pDC) const;

	static BOOL IsCustomAttribute(TDC_ATTRIBUTE nAttribID);
	static BOOL IsCustomColumn(TDC_COLUMN nColID);
	static UINT GetDefaultHorzAlignment(DWORD dwAttribType);
	static BOOL IsEncodedImageTag(const CString& sImage);
	static CString EncodeImageTag(const CString& sImage, const CString& sName);
	static BOOL DecodeImageTag(const CString& sTag, CString& sImage, CString& sName);
	static BOOL AttributeSupportsFeature(DWORD dwDataType, DWORD dwListType, DWORD dwFeature);
	static CString FormatNumber(double dValue, DWORD dwDataType, DWORD dwFeatures);

	// ----------------------------------------------------------------
	CString sUniqueID;
	CString sColumnTitle;
	CString sLabel;
	UINT nHorzAlignment; // DT_LEFT, DT_CENTER, DT_RIGHT
	DWORD dwFeatures;
	CStringArray aDefaultListData;
	mutable CStringArray aAutoListData;
	BOOL bEnabled;

private:
	// these are managed internally
	DWORD dwAttribType;
	TDC_COLUMN nColID;
	TDC_ATTRIBUTE nAttribID;
	TDCCUSTOMATTRIBUTECALCULATION calculation;
	// ----------------------------------------------------------------

	BOOL SetTypes(DWORD dwDataType, DWORD dwListType);
	static DWORD ValidateListType(DWORD dwDataType, DWORD dwListType);
};

////////////////////////////////////////////////////////////////////////////

#define GET_DEF_RET(defs, key, def, ret) \
{                                        \
	int nAttrib = (defs).Find(key);      \
	if (nAttrib == -1)                   \
	{                                    \
		ASSERT(0);                       \
        return ret;                      \
	}                                    \
	def = &((defs)[nAttrib]);            \
}

// alt = break. continue, return
#define GET_DEF_ALT(defs, key, def, alt) \
{                                        \
	int nAttrib = defs.Find(key);        \
	if (nAttrib == -1)                   \
	{                                    \
		ASSERT(0);                       \
        alt;                             \
	}                                    \
	def = &((defs)[nAttrib]);            \
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

	DWORD GetAttributeDataType(TDC_ATTRIBUTE nCustAttribID) const;
	DWORD GetAttributeDataType(TDC_COLUMN nCustColID) const;
	DWORD GetAttributeDataType(const CString& sCustAttribID) const;

	BOOL IsColumnSortable(TDC_COLUMN nCustColID) const;
	BOOL IsColumnEnabled(TDC_COLUMN nCustColID) const;

	// Calculation helpers requiring access to all attribute definitions
	BOOL IsValidCalculation(const TDCCUSTOMATTRIBUTECALCULATION& calc, BOOL bAllowNone = TRUE) const;
	DWORD GetCalculationOperandDataType(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const;
	DWORD GetCalculationResultDataType(const TDCCUSTOMATTRIBUTECALCULATION& calc) const;

	// VC6 fixes
	const TDCCUSTOMATTRIBUTEDEFINITION& operator[](int nIndex) const { return GetData()[nIndex]; }
	TDCCUSTOMATTRIBUTEDEFINITION& operator[](int nIndex) { return GetData()[nIndex]; }

protected:
	void RebuildIDs();

	const TDCCUSTOMATTRIBUTEDEFINITION& ElementAt(int nIndex) const { return GetData()[nIndex]; }
	TDCCUSTOMATTRIBUTEDEFINITION& ElementAt(int nIndex) { return GetData()[nIndex]; }
};

/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCCUSTOMATTRIBUTEDEF_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
