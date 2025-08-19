// TDCCustomAttributeDef.cpp: implementation of the TDCCUSTOMATTRIBUTEDEFINITION class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tdccustomattributeDef.h"
#include "tdccustomattribdata.h"
#include "tdcmapping.h"
#include "todoitem.h"

#include "..\Shared\Misc.h"
#include "..\Shared\GraphicsMisc.h"

/////////////////////////////////////////////////////////////////////////////////////////////

TDCCUSTOMATTRIBUTEDEFINITION FALLBACK(NULL, FALSE);

/////////////////////////////////////////////////////////////////////////////////////////////

TDCCUSTOMATTRIBUTECALCULATIONOPERAND::TDCCUSTOMATTRIBUTECALCULATIONOPERAND()
{
	Clear();
}

BOOL TDCCUSTOMATTRIBUTECALCULATIONOPERAND::operator==(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const
{
	return ((nAttributeID == op.nAttributeID) &&
			(sCustAttribID == op.sCustAttribID));
}

void TDCCUSTOMATTRIBUTECALCULATIONOPERAND::Clear()
{
	nAttributeID = TDCA_NONE;
	sCustAttribID.Empty();
}

BOOL TDCCUSTOMATTRIBUTECALCULATIONOPERAND::Set(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op)
{
	if (!op.IsValid())
		return FALSE;

	*this = op;
	return TRUE;
}

BOOL TDCCUSTOMATTRIBUTECALCULATIONOPERAND::Set(TDC_ATTRIBUTE nAttID, const CString& sCustID)
{
	if (!IsValid(nAttID, sCustID))
		return FALSE;

	nAttributeID = nAttID;
	sCustAttribID = sCustID;

	return TRUE;
}

BOOL TDCCUSTOMATTRIBUTECALCULATIONOPERAND::IsValid(BOOL bAllowNone) const
{
	return IsValid(nAttributeID, sCustAttribID, bAllowNone);
}

BOOL TDCCUSTOMATTRIBUTECALCULATIONOPERAND::IsValid(TDC_ATTRIBUTE nAttribID, const CString& sCustAttribID, BOOL bAllowNone)
{
	switch (nAttribID)
	{
	case TDCA_COST:
	case TDCA_CREATIONDATE:
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_LASTMODDATE:
	case TDCA_PERCENT:
	case TDCA_PRIORITY:
	case TDCA_RISK:
	case TDCA_STARTDATE:
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
	case TDCA_TODAY:
		return sCustAttribID.IsEmpty();

	case TDCA_NONE:
		if (bAllowNone)
			return sCustAttribID.IsEmpty();
		break;

	case TDCA_CUSTOMATTRIB:
		return !sCustAttribID.IsEmpty();
	}

	// all else
	return FALSE;
}

BOOL TDCCUSTOMATTRIBUTECALCULATIONOPERAND::IsCustom() const
{
	return (IsValid(FALSE) && (nAttributeID == TDCA_CUSTOMATTRIB));
}

DWORD TDCCUSTOMATTRIBUTECALCULATIONOPERAND::GetDataType(TDC_ATTRIBUTE nAttribID)
{
	switch (nAttribID)
	{
	case TDCA_COST:
		return TDCCA_DOUBLE;

	case TDCA_PERCENT:
	case TDCA_PRIORITY:
	case TDCA_RISK:
		return TDCCA_INTEGER;

	case TDCA_CREATIONDATE:
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_LASTMODDATE:
	case TDCA_STARTDATE:
	case TDCA_TODAY:
		return TDCCA_DATE;

	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return TDCCA_TIMEPERIOD;

	case TDCA_NONE:
		break;

	case TDCA_CUSTOMATTRIB:
		// unknowable
		break;
	}

	// all else
	return TDCCA_INVALID;
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCCUSTOMATTRIBUTECALCULATION::TDCCUSTOMATTRIBUTECALCULATION()
{
	Clear();
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::operator==(const TDCCUSTOMATTRIBUTECALCULATION& calc) const
{
	return ((opFirst == calc.opFirst) &&
			(nOperator == calc.nOperator) &&
			(opSecond == calc.opSecond) &&
			(dSecondOperandValue == calc.dSecondOperandValue));
}

void TDCCUSTOMATTRIBUTECALCULATION::Clear()
{
	opFirst.Clear();
	nOperator = TDCCAC_ADD;
	opSecond.Clear();
	dSecondOperandValue = 0.0;
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::Set(const TDCCUSTOMATTRIBUTECALCULATION& calc)
{
	if (!calc.IsValid())
		return FALSE;

	// else
	*this = calc;
	return TRUE;
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::Set(TDC_ATTRIBUTE nFirstOpAttribID,
										const CString& sFirstOpCustAttribID,
										TDCCA_CALC_OPERATOR nOp,
										TDC_ATTRIBUTE nSecondOpAttribID,
										const CString& sSecondOpCustAttribID)
{
	TDCCUSTOMATTRIBUTECALCULATION temp;

	if (!temp.opFirst.Set(nFirstOpAttribID, sFirstOpCustAttribID))
		return FALSE;

	if (!temp.opSecond.Set(nSecondOpAttribID, sSecondOpCustAttribID))
		return FALSE;

	temp.nOperator = nOp;

	if (!temp.IsValid())
		return FALSE;

	*this = temp;
	return TRUE;
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::Set(TDC_ATTRIBUTE nFirstOpAttribID,
										const CString& sFirstOpCustAttribID,
										TDCCA_CALC_OPERATOR nOp,
										double dSecondOpValue)
{
	TDCCUSTOMATTRIBUTECALCULATION temp;

	if (!temp.opFirst.Set(nFirstOpAttribID, sFirstOpCustAttribID))
		return FALSE;

	temp.nOperator = nOp;
	temp.opSecond.Clear();
	temp.dSecondOperandValue = dSecondOpValue;

	if (!temp.IsValid())
		return FALSE;

	*this = temp;
	return TRUE;
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::IsFirstOperandCustom() const
{
	return opFirst.IsCustom();
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::IsSecondOperandCustom() const
{
	return opSecond.IsCustom();
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::IsSecondOperandValue() const
{
	return (opSecond.IsValid() && (opSecond.nAttributeID == TDCA_NONE));
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::IsValidOperator(TDCCA_CALC_OPERATOR nOperator, DWORD dwFirstOpDataType, DWORD dwSecondOpDataType)
{
	if (!IsValidOperator(nOperator))
		return FALSE;
	
	switch (dwFirstOpDataType)
	{
	case TDCCA_INTEGER:
	case TDCCA_DOUBLE:
	case TDCCA_FRACTION:
		switch (dwSecondOpDataType)
		{
		case TDCCA_DOUBLE:
		case TDCCA_FRACTION:
		case TDCCA_INTEGER:
			return TRUE; // all operators
		}
		break;

	case TDCCA_DATE:
		switch (dwSecondOpDataType)
		{
		case TDCCA_DOUBLE:
		case TDCCA_INTEGER:
		case TDCCA_FRACTION:
		case TDCCA_TIMEPERIOD:
			return ((nOperator == TDCCAC_ADD) || (nOperator == TDCCAC_SUBTRACT));

		case TDCCA_DATE:
			return (nOperator == TDCCAC_SUBTRACT);
		}
		break;

	case TDCCA_TIMEPERIOD:
		switch (dwSecondOpDataType)
		{
		case TDCCA_DOUBLE:
		case TDCCA_INTEGER:
		case TDCCA_FRACTION:
			return TRUE; // all operators

		case TDCCA_TIMEPERIOD:
			return ((nOperator == TDCCAC_ADD) || (nOperator == TDCCAC_SUBTRACT));
		}
		break;
	}

	// all else
	return FALSE;
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::IsValidOperator(TDCCA_CALC_OPERATOR nOperator)
{
	switch (nOperator)
	{
	case TDCCAC_ADD:
	case TDCCAC_SUBTRACT:
	case TDCCAC_MULTIPLY:
	case TDCCAC_DIVIDE:
		return TRUE;
	}

	// all else
	return FALSE;
}

BOOL TDCCUSTOMATTRIBUTECALCULATION::IsValid(BOOL bAllowNone) const
{
	if (!opFirst.IsValid(bAllowNone))
		return FALSE;

	if (!IsValidOperator(nOperator))
		return FALSE;

	if (!IsSecondOperandValue() && !opSecond.IsValid(bAllowNone))
		return FALSE;

	if (!IsFirstOperandCustom() && opFirst.IsValid(FALSE) &&
		!IsSecondOperandCustom() && opSecond.IsValid(FALSE))
	{
		DWORD dwFirstOpDataType = TDCCUSTOMATTRIBUTECALCULATIONOPERAND::GetDataType(opFirst.nAttributeID);
		DWORD dwSecondOpDataType = TDCCUSTOMATTRIBUTECALCULATIONOPERAND::GetDataType(opSecond.nAttributeID);

		if (TDCCA_INVALID == GetResultDataType(dwFirstOpDataType, nOperator, dwSecondOpDataType))
			return FALSE;
	}

	return TRUE;
}

DWORD TDCCUSTOMATTRIBUTECALCULATION::GetResultDataType(DWORD dwFirstOpDataType, TDCCA_CALC_OPERATOR nOperator, DWORD dwSecondOpDataType)
{
	if (!IsValidOperator(nOperator, dwFirstOpDataType, dwSecondOpDataType))
		return TDCCA_INVALID;

	switch (dwFirstOpDataType)
	{
	case TDCCA_INTEGER:
		switch (dwSecondOpDataType)
		{
		case TDCCA_DOUBLE:
		case TDCCA_FRACTION:
			return TDCCA_DOUBLE;

		case TDCCA_INTEGER:
			return TDCCA_INTEGER;
		}
		break;

	case TDCCA_DOUBLE:
	case TDCCA_FRACTION:
		return TDCCA_DOUBLE;

	case TDCCA_DATE:
		switch (dwSecondOpDataType)
		{
		case TDCCA_DOUBLE:
		case TDCCA_INTEGER:
		case TDCCA_FRACTION:
		case TDCCA_TIMEPERIOD:
			ASSERT((nOperator == TDCCAC_ADD) || (nOperator == TDCCAC_SUBTRACT));
			return TDCCA_DATE;

		case TDCCA_DATE:
			ASSERT(nOperator == TDCCAC_SUBTRACT);
			return TDCCA_TIMEPERIOD;
		}
		break;

	case TDCCA_TIMEPERIOD:
		switch (dwSecondOpDataType)
		{
		case TDCCA_DOUBLE:
		case TDCCA_INTEGER:
		case TDCCA_FRACTION:
		case TDCCA_TIMEPERIOD:
			return TDCCA_TIMEPERIOD;
		}
		break;
	}

	// all else
	return TDCCA_INVALID;
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCCUSTOMATTRIBUTEDEFINITION::TDCCUSTOMATTRIBUTEDEFINITION(LPCTSTR szLabel, BOOL bEnable)
	:
	sLabel(szLabel),
	dwAttribType(TDCCA_STRING),
	nTextAlignment(DT_LEFT),
	dwFeatures(TDCCAF_SORT),
	nColumnID(TDCC_NONE),
	nAttributeID(TDCA_NONE),
	bEnabled(bEnable)
{

}

TDCCUSTOMATTRIBUTEDEFINITION::TDCCUSTOMATTRIBUTEDEFINITION(const TDCCUSTOMATTRIBUTEDEFINITION& otherDef)
{
	*this = otherDef;
}

TDCCUSTOMATTRIBUTEDEFINITION& TDCCUSTOMATTRIBUTEDEFINITION::operator=(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	dwAttribType = attribDef.dwAttribType;
	sUniqueID = attribDef.sUniqueID;
	sColumnTitle = attribDef.sColumnTitle;
	sLabel = attribDef.sLabel;
	nTextAlignment = attribDef.nTextAlignment;
	dwFeatures = attribDef.dwFeatures;
	nColumnID = attribDef.nColumnID;
	nAttributeID = attribDef.nAttributeID;
	bEnabled = attribDef.bEnabled;

	calculation.Clear();
	aDefaultListData.RemoveAll();
	aAutoListData.RemoveAll();

	if (IsDataType(TDCCA_CALCULATION))
	{
		calculation = attribDef.calculation;
	}
	else if (attribDef.IsList())
	{
		aDefaultListData.Copy(attribDef.aDefaultListData);

		if (attribDef.IsAutoList())
			aAutoListData.Copy(attribDef.aAutoListData);
	}

	return *this;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::Matches(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bIncAutoListData) const
{
	if ((dwAttribType != attribDef.dwAttribType) ||
		(sUniqueID.CompareNoCase(attribDef.sUniqueID) != 0) ||
		(sColumnTitle != attribDef.sColumnTitle) ||
		(sLabel != attribDef.sLabel) ||
		(nTextAlignment != attribDef.nTextAlignment) ||
		(dwFeatures != attribDef.dwFeatures) ||
		(nColumnID != attribDef.nColumnID) ||
		(nAttributeID != attribDef.nAttributeID) ||
		(bEnabled != attribDef.bEnabled))
	{
		return FALSE;
	}

	if (IsList())
	{
		if (!Misc::MatchAll(aDefaultListData, attribDef.aDefaultListData, TRUE, TRUE))
			return FALSE;

		if (bIncAutoListData && !Misc::MatchAll(aAutoListData, attribDef.aAutoListData, TRUE, TRUE))
			return FALSE;
	}
	else if (IsDataType(TDCCA_CALCULATION))
	{
		if (!(calculation == attribDef.calculation))
			return FALSE;
	}

	return TRUE;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::operator==(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const
{
	// NOTE: we ignore temporary auto-list data
	return Matches(attribDef, FALSE);
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::operator!=(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const
{
	// NOTE: we ignore temporary auto-list data
	return !Matches(attribDef, FALSE);
}

CString TDCCUSTOMATTRIBUTEDEFINITION::GetColumnTitle() const
{
	return (sColumnTitle.IsEmpty() ? sLabel : sColumnTitle);
}

CString TDCCUSTOMATTRIBUTEDEFINITION::GetToolTip() const
{
	return (sColumnTitle.GetLength() > sLabel.GetLength() ? sColumnTitle : sLabel);
}

UINT TDCCUSTOMATTRIBUTEDEFINITION::GetColumnHeaderAlignment() const
{
	switch (nTextAlignment)
	{
	case DT_CENTER: return HDF_CENTER;
	case DT_RIGHT:	return HDF_RIGHT;
	case DT_LEFT:	return HDF_LEFT;
	}

	// all else
	ASSERT(0);
	return HDF_LEFT;
}

UINT TDCCUSTOMATTRIBUTEDEFINITION::GetDefaultHorzAlignment(DWORD dwAttribType)
{
	switch (dwAttribType & TDCCA_DATAMASK)
	{
	case TDCCA_DATE:
	case TDCCA_INTEGER:
	case TDCCA_DOUBLE:
	case TDCCA_TIMEPERIOD:
	case TDCCA_FRACTION:
		return DT_RIGHT;

	case TDCCA_ICON:
	case TDCCA_BOOL:
	case TDCCA_FILELINK:
		return DT_CENTER;
	}

	// All else
	return DT_LEFT;
}

TDC_ATTRIBUTEGROUP TDCCUSTOMATTRIBUTEDEFINITION::GetAttributeGroup(DWORD dwAttribType)
{
	switch (dwAttribType & TDCCA_DATAMASK)
	{
	case TDCCA_STRING:
		switch (dwAttribType & TDCCA_LISTMASK)
		{
		case TDCCA_FIXEDMULTILIST:
		case TDCCA_AUTOMULTILIST:
			return TDCAG_MULTITEXT;
		}
		return TDCAG_SINGLETEXT;

	case TDCCA_FILELINK:
		return TDCAG_SINGLETEXT;

	case TDCCA_DATE:
		return TDCAG_DATETIME;

	case TDCCA_INTEGER:
	case TDCCA_DOUBLE:
	case TDCCA_FRACTION:
		return TDCAG_NUMERIC;

	case TDCCA_TIMEPERIOD:
		return TDCAG_TIMEPERIOD;

	case TDCCA_BOOL:
	case TDCCA_ICON:
		return TDCAG_OTHER;

	case TDCCA_CALCULATION:
		break;
	}

	ASSERT(0);
	return TDCAG_NONE;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::HasDefaultTextAlignment() const
{
	return (nTextAlignment == GetDefaultHorzAlignment(dwAttribType));
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::SetAttributeType(DWORD dwType)
{
	// Must be a valid data/list pairing
	return SetTypes((dwType & TDCCA_DATAMASK), (dwType & TDCCA_LISTMASK));
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::SetDataType(DWORD dwDataType, BOOL bUpdateDefaultAlignment)
{
	// Forcibly validate the existing list type to match this data type
	DWORD dwListType = ValidateListType(dwDataType, GetListType());

	DWORD dwPrevType = GetDataType();
	VERIFY(SetTypes((dwDataType & TDCCA_DATAMASK), dwListType));

	// Set default alignment if the previous also had the default
	if (bUpdateDefaultAlignment)
	{
		if (nTextAlignment == GetDefaultHorzAlignment(dwPrevType))
			nTextAlignment = GetDefaultHorzAlignment(dwAttribType);
	}

	return TRUE;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::SetListType(DWORD dwListType)
{
	// Must be a valid list type for this data type
	return SetTypes(GetDataType(), (dwListType & TDCCA_LISTMASK));
}

int TDCCUSTOMATTRIBUTEDEFINITION::GetUniqueListData(CStringArray& aData) const
{
	aData.Copy(aAutoListData);
	Misc::AppendItems(aDefaultListData, aData, TRUE);

	return aData.GetSize();
}

CString TDCCUSTOMATTRIBUTEDEFINITION::EncodeListData() const
{
	if (!IsList())
		return _T("");

	CString sListData = Misc::FormatArray(aDefaultListData, '\n');

	if (IsAutoList())
	{
		sListData += '\t';
		sListData += Misc::FormatArray(aAutoListData, '\n');
	}

	return sListData;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::DecodeListData(const CString& sListData)
{
	if (IsList() && !sListData.IsEmpty())
	{
		CString sDefData(sListData), sAutoData;

		Misc::Split(sDefData, sAutoData, '\t');
		Misc::Split(sDefData, aDefaultListData, '\n');

		if (IsAutoList())
			Misc::Split(sAutoData, aAutoListData, '\n');

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::GetListIconName(const CString& sImage, CString& sName) const
{
	sName.Empty();

	if (IsDataType(TDCCA_ICON) && IsList())
	{
		for (int nItem = 0; nItem < aDefaultListData.GetSize(); nItem++)
		{
			CString sListItem(aDefaultListData[nItem]);

			if ((sListItem.Find(sImage) == 0) && (sListItem[sImage.GetLength()] == ':'))
				sName = sListItem.Mid(sImage.GetLength() + 1);
		}
	}
	else
	{
		ASSERT(0);
	}

	return !sName.IsEmpty();
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::SupportsFeature(DWORD dwFeature) const
{
	return AttributeSupportsFeature(GetDataType(), GetListType(), dwFeature);
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::HasFeature(DWORD dwFeature) const
{
	return (SupportsFeature(dwFeature) && Misc::HasFlag(dwFeatures, dwFeature));
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::AttributeSupportsFeature(DWORD dwDataType, DWORD dwListType, DWORD dwFeature)
{
	switch (dwFeature)
	{
	case TDCCAF_SORT:
	case TDCCAF_INHERITPARENTCHANGES:
		// sorting and inheritance works on all data types
		return TRUE;

	case TDCCAF_FILTER:
		return ((dwDataType == TDCCA_DATE) || (dwListType != TDCCA_NOTALIST));

	case TDCCAF_EXCLUDEBLANKITEM:
		return ((dwListType == TDCCA_AUTOLIST) || (dwListType == TDCCA_FIXEDLIST));

	default:
		// Other features not supported on multi-list types
		if ((dwListType == TDCCA_AUTOMULTILIST) || (dwListType == TDCCA_FIXEDMULTILIST))
			return FALSE;
		break;
	}

	// Attributes
	switch (dwDataType)
	{
	case TDCCA_DOUBLE:
		return ((dwFeature == TDCCAF_ACCUMULATE) ||
				(dwFeature == TDCCAF_MAXIMIZE) ||
				(dwFeature == TDCCAF_MINIMIZE) ||
				(dwFeature == TDCCAF_HIDEZERO) ||
				(dwFeature == TDCCAF_ONEDECIMAL));

	case TDCCA_FRACTION:
		return ((dwFeature == TDCCAF_DISPLAYASPERCENT) ||
				(dwFeature == TDCCAF_MAXIMIZE) ||
				(dwFeature == TDCCAF_MINIMIZE) ||
				(dwFeature == TDCCAF_HIDEZERO) ||
				(dwFeature == TDCCAF_ONEDECIMAL));
		
	case TDCCA_INTEGER:
		return ((dwFeature == TDCCAF_ACCUMULATE) ||
				(dwFeature == TDCCAF_MAXIMIZE) ||
				(dwFeature == TDCCAF_MINIMIZE) ||
				(dwFeature == TDCCAF_HIDEZERO));

	case TDCCA_TIMEPERIOD:
		// 'Hide zero values' controlled by app preference
		return ((dwFeature == TDCCAF_ACCUMULATE) ||
				(dwFeature == TDCCAF_MAXIMIZE) ||
				(dwFeature == TDCCAF_MINIMIZE));

	case TDCCA_DATE:
		return ((dwFeature == TDCCAF_MAXIMIZE) ||
				(dwFeature == TDCCAF_MINIMIZE) ||
				(dwFeature == TDCCAF_SHOWTIME));

	case TDCCA_BOOL:
	case TDCCA_STRING:
	case TDCCA_FILELINK:
	case TDCCA_ICON:
		break;

	case TDCCA_CALCULATION:
		// Unknowable. Caller responsible for extracting the 
		// calculation result type and using that as the argument
 		break;

	default:
		ASSERT(0);
		break;
	}

	return FALSE;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::IsAggregated() const
{
	return HasFeature(TDCCAF_ACCUMULATE) ||
			HasFeature(TDCCAF_MAXIMIZE) ||
			HasFeature(TDCCAF_MINIMIZE);
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::ValidateData(TDCCADATA& data) const
{
	if (!data.IsEmpty())
	{
		CString sData = data.AsString();

		switch (GetDataType())
		{
		case TDCCA_INTEGER:
		case TDCCA_DOUBLE:
		case TDCCA_FRACTION:
			return Misc::IsNumber(sData);

		case TDCCA_DATE:
			{
				if (Misc::IsNumber(sData))
					return TRUE;

				COleDateTime date;

				if (CDateHelper::DecodeDate(sData, date, TRUE))
				{
					data.Set(date);
					return TRUE;
				}
			}
			return FALSE;

		case TDCCA_TIMEPERIOD:
			return data.IsTimePeriod();

		case TDCCA_BOOL:
		case TDCCA_ICON:
		case TDCCA_STRING:
		case TDCCA_FILELINK:
			break;

		default:
		case TDCCA_CALCULATION:
			ASSERT(0);
			break;
		}
	}

	return TRUE;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::SetCalculation(const TDCCUSTOMATTRIBUTECALCULATION& calc)
{
	if (!calculation.Set(calc))
		return FALSE;

	dwAttribType = TDCCA_CALCULATION;
	return TRUE;
}

CString TDCCUSTOMATTRIBUTEDEFINITION::GetNextListItem(const CString& sItem, BOOL bNext) const
{
	DWORD dwListType = GetListType();

	ASSERT(dwListType != TDCCA_NOTALIST);

	switch (aDefaultListData.GetSize())
	{
	case 0:
		return sItem;

	default:
		{
			int nFind = Misc::Find(sItem, aDefaultListData);
			ASSERT((nFind != -1) || sItem.IsEmpty());

			if (bNext)
			{
				nFind++;

				if (nFind < aDefaultListData.GetSize())
					return aDefaultListData[nFind];
			}
			else // prev
			{
				if (nFind == -1)
					nFind = aDefaultListData.GetSize();

				nFind--;

				if (nFind >= 0)
					return aDefaultListData[nFind];
			}

			// all else
			return _T("");
		}
		break;
	}
}

CString TDCCUSTOMATTRIBUTEDEFINITION::GetImageName(const CString& sImage) const
{
	CString sName;
	int nTag = Misc::Find(sImage, aDefaultListData);

	if (nTag != -1)
	{
		CString sTag = aDefaultListData[nTag], sDummy;
		VERIFY(DecodeImageTag(sTag, sDummy, sName));
	}

	return sName;
}

int TDCCUSTOMATTRIBUTEDEFINITION::CalcLongestListItem(CDC* pDC) const
{
	if (!IsList())
	{
		ASSERT(0);
		return 0;
	}

	int nItem = aDefaultListData.GetSize(), nLongest = 0;

	while (nItem--)
	{
		const CString& sItem = Misc::GetItem(aDefaultListData, nItem);
		int nItemLen = 0;

		switch (GetDataType())
		{
		case TDCCA_STRING:
		case TDCCA_INTEGER:
		case TDCCA_DOUBLE:
		case TDCCA_FRACTION:
		case TDCCA_DATE:
		case TDCCA_BOOL:
		case TDCCA_TIMEPERIOD:
			nItemLen = pDC->GetTextExtent(sItem).cx;
			break;

		case TDCCA_ICON:
			{
				nItemLen = GraphicsMisc::ScaleByDPIFactor(16); // for the icon

				// check for trailing text
				CString sDummy, sName;

				if (DecodeImageTag(sItem, sDummy, sName) && !sName.IsEmpty())
				{
					nItemLen += GraphicsMisc::ScaleByDPIFactor(2); // offset
					nItemLen += pDC->GetTextExtent(sName).cx;
				}
			}
			break;

		default:
			ASSERT(0);
			break;
		}

		nLongest = max(nLongest, nItemLen);
	}

	return nLongest;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::IsEncodedImageTag(const CString& sImage)
{
	return (sImage.Find(':') != -1);
}

CString TDCCUSTOMATTRIBUTEDEFINITION::EncodeImageTag(const CString& sImage, const CString& sName)
{
	if (IsEncodedImageTag(sImage))
		return sImage;

	return (sImage + ':' + sName);
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(const CString& sTag, CString& sImage, CString& sName)
{
	sImage.Empty();
	sName.Empty();

	CStringArray aParts;
	int nNumParts = Misc::Split(sTag, aParts, ':');

	switch (nNumParts)
	{
	case 2:
		sName = aParts[1];
		// fall thru
	case 1:
		sImage = aParts[0];
		break;

	case 0:
		break;
	}

	return !sImage.IsEmpty();
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::SetTypes(DWORD dwDataType, DWORD dwListType)
{
	// List type must have been validate before this point
	DWORD dwValidListType = ValidateListType(dwDataType, dwListType);

	if (dwValidListType != dwListType)
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bWasCalc = IsDataType(TDCCA_CALCULATION);

	dwAttribType = (dwDataType | dwListType);

	// Update calculation
	BOOL bIsCalc = IsDataType(TDCCA_CALCULATION);

	// Clear calculation if switching to or from
	if (Misc::StatesDiffer(bWasCalc, bIsCalc))
		calculation.Clear();

	return TRUE;
}

DWORD TDCCUSTOMATTRIBUTEDEFINITION::ValidateListType(DWORD dwDataType, DWORD dwListType)
{
	// ensure list-type cannot be applied to dates or times,
	// or only partially to icons, flags and file links
	switch (dwDataType)
	{
	case TDCCA_STRING:
	case TDCCA_INTEGER:
	case TDCCA_DOUBLE:
		// all types supported
		break;

	case TDCCA_DATE:
	case TDCCA_TIMEPERIOD:
	case TDCCA_BOOL:
	case TDCCA_CALCULATION:
		dwListType = TDCCA_NOTALIST;
		break;

	case TDCCA_FRACTION:
		if (dwListType)
		{
			if ((dwListType != TDCCA_FIXEDLIST) && (dwListType != TDCCA_AUTOLIST))
				dwListType = TDCCA_FIXEDLIST;
		}
		break;

	case TDCCA_ICON:
		if (dwListType)
		{
			if ((dwListType != TDCCA_FIXEDLIST) && (dwListType != TDCCA_FIXEDMULTILIST))
				dwListType = TDCCA_FIXEDLIST;
		}
		break;

	case TDCCA_FILELINK:
		if (dwListType)
		{
			dwListType = TDCCA_AUTOLIST;
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	return dwListType;
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(TDC_ATTRIBUTE nAttribID)
{
	if ((nAttribID >= TDCA_CUSTOMATTRIB_FIRST) && (nAttribID <= TDCA_CUSTOMATTRIB_LAST))
		return TRUE;

	return (nAttribID == TDCA_CUSTOMATTRIB_ALL);
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(TDC_COLUMN nColID)
{
	return (nColID >= TDCC_CUSTOMCOLUMN_FIRST && nColID <= TDCC_CUSTOMCOLUMN_LAST);
}

CString TDCCUSTOMATTRIBUTEDEFINITION::FormatData(const TDCCADATA& data, BOOL bISODates, TCHAR cListSep) const
{
	if (IsList())
		return data.FormatAsArray(cListSep);

	// else
	switch (GetDataType())
	{
	case TDCCA_DATE:
		return data.FormatAsDate(bISODates, HasFeature(TDCCAF_SHOWTIME));

	case TDCCA_DOUBLE:
	case TDCCA_INTEGER:
		return FormatNumber(data.AsDouble());

	case TDCCA_FRACTION:
		return FormatNumber(data.AsFraction());

	case TDCCA_TIMEPERIOD:
		return FormatTimePeriod(data, dwFeatures);
	}

	// all else
	return data.AsString();
}

CString TDCCUSTOMATTRIBUTEDEFINITION::FormatTimePeriod(const TDCCADATA& data, DWORD dwFeatures)
{
	if (!data.IsTimePeriod())
	{
		ASSERT(0);
		return _T("");
	}

	TDCTIMEPERIOD time;
	VERIFY(data.AsTimePeriod(time));

	if ((dwFeatures & TDCCAF_HIDEZERO) && (time.dAmount == 0.0))
		return _T("");

	// else
	return time.Format(((dwFeatures & TDCCAF_ONEDECIMAL) ? 1 : 2));
}

CString TDCCUSTOMATTRIBUTEDEFINITION::FormatNumber(double dValue) const
{
	return FormatNumber(dValue, GetDataType(), dwFeatures);
}

CString TDCCUSTOMATTRIBUTEDEFINITION::FormatNumber(double dValue, DWORD dwDataType, DWORD dwFeatures)
{
	if ((dwFeatures & TDCCAF_HIDEZERO) && (dValue == 0.0))
		return _T("");

	// else
	BOOL bOneDecimal = (dwFeatures & TDCCAF_ONEDECIMAL);

	switch (dwDataType)
	{
	case TDCCA_FRACTION:
		if (dwFeatures & TDCCAF_DISPLAYASPERCENT)
		{
			return Misc::Format((dValue * 100), (bOneDecimal ? 1 : 0), _T("%"));
		}
		// else fall thru

	case TDCCA_DOUBLE:
		{
			CString sNumber = Misc::FormatNumber(dValue);

			if (bOneDecimal)
			{
				int nDecimal = sNumber.Find(Misc::GetDecimalSeparator());

				if (nDecimal > 0)
					sNumber = sNumber.Left(nDecimal + 2);
			}

			return sNumber;
		}
		break;

	case TDCCA_INTEGER:
		return Misc::FormatNumber((int)dValue);
	}

	ASSERT(0);
	return _T("");
}

BOOL TDCCUSTOMATTRIBUTEDEFINITION::GetDataAsDouble(const TDCCADATA& data, double& dValue, TDC_UNITS nUnits) const
{
	switch (GetDataType())
	{
	case TDCCA_TIMEPERIOD:
		if (IsValidUnits(nUnits))
		{
			TDCTIMEPERIOD time;

			if (data.AsTimePeriod(time))
			{
				time.SetUnits(nUnits, TRUE); // Convert to requested units
				dValue = time.dAmount;

				return TRUE;
			}
		}
		break;

	case TDCCA_FRACTION:
		dValue = data.AsFraction();
		return TRUE;

	case TDCCA_CALCULATION:
	case TDCCA_DOUBLE:
	case TDCCA_INTEGER:
	case TDCCA_DATE:
		ASSERT(data.IsEmpty() || Misc::IsNumber(data.AsString()));
		dValue = data.AsDouble();
		return TRUE;
	}

	// All else
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////

CTDCCustomAttribDefinitionArray::CTDCCustomAttribDefinitionArray()
{
}

CTDCCustomAttribDefinitionArray::CTDCCustomAttribDefinitionArray(const CTDCCustomAttribDefinitionArray& other)
{
	Copy(other);
}

int CTDCCustomAttribDefinitionArray::Add(TDCCUSTOMATTRIBUTEDEFINITION& newElement)
{
	int nIndex = CArray<TDCCUSTOMATTRIBUTEDEFINITION, TDCCUSTOMATTRIBUTEDEFINITION&>::Add(newElement);
	RebuildIDs();

	return nIndex;
}

void CTDCCustomAttribDefinitionArray::InsertAt(int nIndex, TDCCUSTOMATTRIBUTEDEFINITION& newElement, int nCount)
{
	CArray<TDCCUSTOMATTRIBUTEDEFINITION, TDCCUSTOMATTRIBUTEDEFINITION&>::InsertAt(nIndex, newElement, nCount);
	RebuildIDs();
}

void CTDCCustomAttribDefinitionArray::RemoveAt(int nIndex, int nCount)
{
	CArray<TDCCUSTOMATTRIBUTEDEFINITION, TDCCUSTOMATTRIBUTEDEFINITION&>::RemoveAt(nIndex, nCount);
	RebuildIDs();
}

BOOL CTDCCustomAttribDefinitionArray::MatchAny(const CTDCCustomAttribDefinitionArray& aAttribDefs) const
{
	for (int nDef = 0; nDef < aAttribDefs.GetSize(); nDef++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nDef];

		if (Find(attribDef.GetAttributeID()) != -1)
			return TRUE;
	}

	return FALSE;
}

int CTDCCustomAttribDefinitionArray::Find(const CString& sAttribID, int nIgnore) const
{
	ASSERT(!sAttribID.IsEmpty());

	if (!sAttribID.IsEmpty())
	{
		int nDef = GetSize();

		while (nDef--)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = ElementAt(nDef);

			if ((nDef != nIgnore) && (attribDef.sUniqueID.CompareNoCase(sAttribID) == 0))
				return nDef;
		}
	}

	return -1;
}

int CTDCCustomAttribDefinitionArray::Find(TDC_COLUMN nCustColID, int nIgnore) const
{
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nCustColID));

	TDC_ATTRIBUTE nCustAttribID = TDC::MapColumnToAttribute(nCustColID);

	if (nCustAttribID == TDCA_NONE)
		return -1;

	return Find(nCustAttribID, nIgnore);
}

int CTDCCustomAttribDefinitionArray::Find(TDC_ATTRIBUTE nCustAttribID, int nIgnore) const
{
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nCustAttribID));

	int nDef = GetSize();

	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = ElementAt(nDef);

		if ((nDef != nIgnore) && (attribDef.GetAttributeID() == nCustAttribID))
			return nDef;
	}

	return -1;
}

int CTDCCustomAttribDefinitionArray::Append(const CTDCCustomAttribDefinitionArray& aSrc)
{
	int nOrgSize = GetSize();

	for (int nDef = 0; nDef < aSrc.GetSize(); nDef++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aSrc[nDef];

		// Append unique items only
		if (Find(attribDef.sUniqueID) == -1)
		{
			TDCCUSTOMATTRIBUTEDEFINITION def = aSrc[nDef];
			Add(def);
		}
		// else skip
	}

	return (GetSize() - nOrgSize);
}

BOOL CTDCCustomAttribDefinitionArray::AnyHasFeature(DWORD dwFeature) const
{
	int nDef = GetSize();

	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = ElementAt(nDef);

		if (def.HasFeature(dwFeature))
			return TRUE;

		if (def.IsCalculation() && CalculationHasFeature(def, dwFeature))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDCCustomAttribDefinitionArray::CalculationHasFeature(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, DWORD dwFeature) const
{
	if (!attribDef.IsDataType(TDCCA_CALCULATION))
		return FALSE;

	if (!Misc::HasFlag(attribDef.dwFeatures, dwFeature))
		return FALSE;

	DWORD dwResultType = GetCalculationResultDataType(attribDef.calculation);

	return TDCCUSTOMATTRIBUTEDEFINITION::AttributeSupportsFeature(dwResultType, attribDef.GetListType(), dwFeature);
}

BOOL CTDCCustomAttribDefinitionArray::AnyCalculationUsesAnyAttribute(const CTDCAttributeMap& mapAttribIDs) const
{
	int nDef = GetSize();

	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = ElementAt(nDef);

		if (!def.IsCalculation())
			continue;

		// First operand
		TDC_ATTRIBUTE nFirstOpAttribID = (def.calculation.opFirst.IsCustom() ? 
										  GetAttributeID(def.calculation.opFirst.sCustAttribID) :
										  def.calculation.opFirst.nAttributeID);
		
		if (mapAttribIDs.Has(nFirstOpAttribID))
			return TRUE;

		// Second operand
		TDC_ATTRIBUTE nSecondOpAttribID = (def.calculation.opSecond.IsCustom() ? 
										  GetAttributeID(def.calculation.opSecond.sCustAttribID) :
										  def.calculation.opSecond.nAttributeID);
		
		if (mapAttribIDs.Has(nSecondOpAttribID))
			return TRUE;
	}

	return FALSE;
}

TDC_ATTRIBUTE CTDCCustomAttribDefinitionArray::GetAttributeID(TDC_COLUMN nCustColID) const
{
	return GetDefinition(nCustColID).GetAttributeID();
}

TDC_ATTRIBUTE CTDCCustomAttribDefinitionArray::GetAttributeID(const CString& sCustAttribID) const
{
	return GetDefinition(sCustAttribID).GetAttributeID();
}

CString CTDCCustomAttribDefinitionArray::GetAttributeTypeID(TDC_ATTRIBUTE nCustAttribID) const
{
	return GetDefinition(nCustAttribID).sUniqueID;
}

CString CTDCCustomAttribDefinitionArray::GetAttributeTypeID(TDC_COLUMN nCustColID) const
{
	return GetDefinition(nCustColID).sUniqueID;
}

int CTDCCustomAttribDefinitionArray::GetVisibleColumnIDs(CTDCColumnIDMap& mapCols, BOOL bAppend) const
{
	if (!bAppend)
		mapCols.RemoveAll();

	int nNumCols = mapCols.GetCount();
	int nDef = GetSize();

	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = ElementAt(nDef);

		if (def.bEnabled)
			mapCols.Add(def.GetColumnID());
	}

	return (mapCols.GetCount() - nNumCols);
}

const TDCCUSTOMATTRIBUTEDEFINITION& CTDCCustomAttribDefinitionArray::GetDefinition(TDC_ATTRIBUTE nCustAttribID) const
{
	int nDef = Find(nCustAttribID);

	return ((nDef != -1) ? ElementAt(nDef) : FALLBACK);
}

const TDCCUSTOMATTRIBUTEDEFINITION& CTDCCustomAttribDefinitionArray::GetDefinition(TDC_COLUMN nCustColID) const
{
	int nDef = Find(nCustColID);

	return ((nDef != -1) ? ElementAt(nDef) : FALLBACK);
}

const TDCCUSTOMATTRIBUTEDEFINITION& CTDCCustomAttribDefinitionArray::GetDefinition(const CString& sCustAttribID) const
{
	int nDef = Find(sCustAttribID);

	return ((nDef != -1) ? ElementAt(nDef) : FALLBACK);
}

DWORD CTDCCustomAttribDefinitionArray::GetAttributeDataType(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bResolveCalcType) const
{
	if (bResolveCalcType && attribDef.IsDataType(TDCCA_CALCULATION))
		return GetCalculationResultDataType(attribDef.calculation);

	return attribDef.GetDataType();
}

DWORD CTDCCustomAttribDefinitionArray::GetAttributeDataType(TDC_ATTRIBUTE nCustAttribID, BOOL bResolveCalcType) const
{
	return GetAttributeDataType(GetDefinition(nCustAttribID), bResolveCalcType);
}

DWORD CTDCCustomAttribDefinitionArray::GetAttributeDataType(TDC_COLUMN nCustColID, BOOL bResolveCalcType) const
{
	return GetAttributeDataType(GetDefinition(nCustColID), bResolveCalcType);
}

DWORD CTDCCustomAttribDefinitionArray::GetAttributeDataType(const CString& sCustAttribID, BOOL bResolveCalcType) const
{
	return GetAttributeDataType(GetDefinition(sCustAttribID), bResolveCalcType);
}

BOOL CTDCCustomAttribDefinitionArray::IsListType(TDC_ATTRIBUTE nCustAttribID) const
{
	return GetDefinition(nCustAttribID).IsList();
}

BOOL CTDCCustomAttribDefinitionArray::IsListType(TDC_COLUMN nCustColID) const
{
	return GetDefinition(nCustColID).IsList();
}

BOOL CTDCCustomAttribDefinitionArray::IsListType(const CString& sCustAttribID) const
{
	return GetDefinition(sCustAttribID).IsList();
}

CString CTDCCustomAttribDefinitionArray::FormatData(const TDCCADATA& data, const CString& sCustAttribID, BOOL bISODates) const
{
	const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = GetDefinition(sCustAttribID);
	return FormatData(data, attribDef, bISODates);
}

CString CTDCCustomAttribDefinitionArray::FormatData(const TDCCADATA& data, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bISODates) const
{
	if (!attribDef.IsDataType(TDCCA_CALCULATION))
		return attribDef.FormatData(data, bISODates);

	// A bit of hackery for calculation types
	TDCCUSTOMATTRIBUTEDEFINITION temp = attribDef;
	temp.SetDataType(GetAttributeDataType(attribDef, TRUE));

	return temp.FormatData(data, bISODates);
}

BOOL CTDCCustomAttribDefinitionArray::IsColumnSortable(TDC_COLUMN nCustColID) const
{
	return GetDefinition(nCustColID).HasFeature(TDCCAF_SORT);
}

BOOL CTDCCustomAttribDefinitionArray::IsColumnEnabled(TDC_COLUMN nCustColID) const
{
	return GetDefinition(nCustColID).bEnabled;
}

void CTDCCustomAttribDefinitionArray::RebuildIDs()
{
	int nCustColID = TDCC_CUSTOMCOLUMN_FIRST;
	int nCustAttribID = TDCA_CUSTOMATTRIB_FIRST;

	for (int nDef = 0; nDef < GetSize(); nDef++)
	{
		TDCCUSTOMATTRIBUTEDEFINITION& attribDef = ElementAt(nDef);

		attribDef.nColumnID = (TDC_COLUMN)nCustColID++;
		attribDef.nAttributeID = (TDC_ATTRIBUTE)nCustAttribID++;
	}
}

DWORD CTDCCustomAttribDefinitionArray::GetCalculationOperandDataType(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const
{
	DWORD dwDataType = TDCCUSTOMATTRIBUTECALCULATIONOPERAND::GetDataType(op.nAttributeID);

	if ((dwDataType == TDCCA_INVALID) && (op.nAttributeID == TDCA_CUSTOMATTRIB))
	{
		int nDef = Find(op.sCustAttribID);

		if (nDef != -1)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = ElementAt(nDef);

			if (attribDef.IsDataType(TDCCA_CALCULATION))
				return GetCalculationResultDataType(attribDef.calculation); // RECURSIVE CALL

			if (!attribDef.IsMultiList())
				return attribDef.GetDataType();
		}
	}

	return dwDataType;
}

DWORD CTDCCustomAttribDefinitionArray::GetCalculationResultDataType(const TDCCUSTOMATTRIBUTECALCULATION& calc) const
{
	DWORD dwFirstOpDataType = GetCalculationOperandDataType(calc.opFirst);
	DWORD dwSecondOpDataType = (calc.IsSecondOperandValue() ? TDCCA_DOUBLE : GetCalculationOperandDataType(calc.opSecond));

	return TDCCUSTOMATTRIBUTECALCULATION::GetResultDataType(dwFirstOpDataType, calc.nOperator, dwSecondOpDataType);
}

BOOL CTDCCustomAttribDefinitionArray::IsValidCalculation(const TDCCUSTOMATTRIBUTECALCULATION& calc, BOOL bAllowNone) const
{
	return (calc.IsValid(bAllowNone) && (GetCalculationResultDataType(calc) != TDCCA_INVALID));
}

/////////////////////////////////////////////////////////////////////////////

