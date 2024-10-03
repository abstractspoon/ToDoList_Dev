// HMXDataset.cpp: implementation of the CHMXDataset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HMXDataset.h"
#include "ColorDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHMXDataset::CHMXDataset()
{
	Reset();
}

CHMXDataset::~CHMXDataset()
{

}

void CHMXDataset::Reset()
{
	ClearData();

	m_nSize = 2;
	m_nStyle = HMX_DATASET_STYLE_LINE;
	m_crLine = RGB(0, 0, 0);
	m_crFill = CLR_NONE;
	m_nMarker = HMX_DATASET_MARKER_NONE;
}

void CHMXDataset::Copy(const CHMXDataset& other)
{
	m_data.Copy(other.m_data);

	m_nSize = other.m_nSize;
	m_nStyle = other.m_nStyle;
	m_crLine = other.m_crLine;
	m_crFill = other.m_crFill;
	m_nMarker = other.m_nMarker;

	m_bSetMinTo = other.m_bSetMinTo;
	m_bSetMaxTo = other.m_bSetMaxTo;
	m_dSetMinTo = other.m_dSetMinTo;
	m_dSetMaxTo = other.m_dSetMaxTo;
}

void CHMXDataset::ClearData()
{
	m_data.RemoveAll();

	m_bSetMinTo = m_bSetMaxTo = FALSE;
	m_dSetMinTo = m_dSetMaxTo = -1;
}

BOOL CHMXDataset::HasData() const
{
	return (m_data.GetSize() > 0);
}

BOOL CHMXDataset::AddData(double nData)
{
	m_data.Add(nData);

	return TRUE;
}

BOOL CHMXDataset::SetData(int nIndex, double nData)
{
	if (nIndex < 0 || nIndex >= m_data.GetSize())
		return FALSE;
	
	m_data.SetAt(nIndex, nData);

	return TRUE;
}


int CHMXDataset::GetDatasetSize() const
{
	return m_data.GetSize();
}

void CHMXDataset::SetDatasetSize(int nSize)
{
	m_data.SetSize(nSize);
}

BOOL CHMXDataset::GetData(int nIndex, double &nSample) const
{
	if (nIndex < 0 || nIndex >= m_data.GetSize())
		return FALSE;
	
	nSample = m_data.GetAt(nIndex);
	
	return TRUE;
}

BOOL CHMXDataset::SetStyle(HMX_DATASET_STYLE nStyle)
{
	m_nStyle = nStyle;
	
	return TRUE;
}

BOOL CHMXDataset::SetLineColor(COLORREF clr)
{
	m_crLine = clr;

	return TRUE;
}

BOOL CHMXDataset::SetFillColor(COLORREF clr)
{
	m_crFill = clr;
	
	return TRUE;
}

BOOL CHMXDataset::SetMarker(HMX_DATASET_MARKER nMarker)
{
	m_nMarker = nMarker;

	return TRUE;
}

HMX_DATASET_MARKER CHMXDataset::GetMarker() const
{
	return m_nMarker;
}

BOOL CHMXDataset::SetSize(int nSize)
{
	// between 1 an 10	
	m_nSize = min(nSize, 10);
	m_nSize = max(m_nSize,  1);

	return TRUE;
}

BOOL CHMXDataset::GetMinMax(double& nMin, double& nMax, BOOL bDataOnly, double dIgnoreVal) const
{
	double dMin = HMX_DATASET_VALUE_INVALID, dMax = -HMX_DATASET_VALUE_INVALID;

	if (m_data.GetSize() > 0) 
	{
		double temp = 0.0;
		BOOL bFirst = TRUE;

		for (int f=0; f<m_data.GetSize(); f++)
		{
			VERIFY(GetData(f, temp));

			if (temp == HMX_DATASET_VALUE_INVALID)
				continue;
			
			if (temp == dIgnoreVal)
				continue;

			if (bFirst)
			{
				bFirst = FALSE;
				dMin = dMax = temp;
			}
			else
			{
				dMin = min(dMin, temp);
				dMax = max(dMax, temp);
			}
		}

		if (m_bSetMinTo && !bDataOnly)
			dMin = min(dMin, m_dSetMinTo);

		if (m_bSetMaxTo && !bDataOnly)
			dMax = max(dMax, m_dSetMaxTo);

		nMin = dMin;
		nMax = dMax;

		return TRUE;
	} 
	else if (!bDataOnly && m_bSetMinTo && m_bSetMaxTo)
	{
		nMin = m_dSetMinTo;
		nMax = m_dSetMaxTo;

		return TRUE;
	}

	// else
	return FALSE;
}

void CHMXDataset::SetMin(double dMin)
{
	m_bSetMinTo = TRUE;
	m_dSetMinTo = dMin;
}

void CHMXDataset::SetMax(double dMax)
{
	m_bSetMaxTo = TRUE;
	m_dSetMaxTo = dMax;
}

int CHMXDataset::GetSize() const
{
	return m_nSize;
}

COLORREF CHMXDataset::GetLineColor() const
{
	return m_crLine;
}

COLORREF CHMXDataset::GetFillColor() const
{
	if (m_crFill == CLR_NONE)
		return RGBX::AdjustLighting(m_crLine, 0.25, FALSE);

	// else
	return m_crFill;
}

HMX_DATASET_STYLE CHMXDataset::GetStyle() const
{
	return m_nStyle;
}

