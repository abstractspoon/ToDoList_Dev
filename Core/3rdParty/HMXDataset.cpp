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
	m_data.RemoveAll();

	m_nSize = 2;
	m_nStyle = HMX_DATASET_STYLE_LINE;
	m_crLine = RGB(0, 0, 0);
	m_crFill = CLR_NONE;
	m_nMarker = HMX_DATASET_MARKER_NONE;

	m_bSetMinTo = m_bSetMaxTo = false;
	m_dSetMinTo = m_dSetMaxTo = -1;
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

	m_bSetMinTo = m_bSetMaxTo = false;
	m_dSetMinTo = m_dSetMaxTo = -1;
}

bool CHMXDataset::HasData() const
{
	return (m_data.GetSize() > 0);
}

bool CHMXDataset::AddData(double nData)
{
	m_data.Add(nData);

	return true;
}

bool CHMXDataset::SetData(int nIndex, double nData)
{
	if (nIndex < 0 || nIndex >= m_data.GetSize())
		return false;
	
	m_data.SetAt(nIndex, nData);

	return true;
}


int CHMXDataset::GetDatasetSize() const
{
	return m_data.GetSize();
}

void CHMXDataset::SetDatasetSize(int nSize)
{
	m_data.SetSize(nSize);
}

bool CHMXDataset::GetData(int nIndex, double &nSample) const
{
	if (nIndex < 0 || nIndex >= m_data.GetSize())
		return false;
	
	nSample = m_data.GetAt(nIndex);
	
	return true;
}

bool CHMXDataset::SetStyle(HMX_DATASET_STYLE nStyle)
{
	m_nStyle = nStyle;
	
	return true;
}

bool CHMXDataset::SetLineColor(COLORREF clr)
{
	m_crLine = clr;

	return true;
}

bool CHMXDataset::SetFillColor(COLORREF clr)
{
	m_crFill = clr;
	
	return true;
}

bool CHMXDataset::SetMarker(HMX_DATASET_MARKER nMarker)
{
	m_nMarker = nMarker;

	return true;
}

HMX_DATASET_MARKER CHMXDataset::GetMarker() const
{
	return m_nMarker;
}

bool CHMXDataset::SetSize(int nSize)
{
	// between 1 an 10	
	m_nSize = min(nSize, 10);
	m_nSize = max(m_nSize,  1);

	return true;
}

bool CHMXDataset::GetMinMax(double& nMin, double& nMax, bool bDataOnly) const
{
	// following lines help me to solve some problems with invalid values
	double dMin = HMX_DATASET_VALUE_INVALID, dMax = -HMX_DATASET_VALUE_INVALID;

	if (m_data.GetSize() > 0) 
	{
		double temp = 0;
		GetData(0, temp);
		
		if (temp != HMX_DATASET_VALUE_INVALID) 
		{
			dMin = dMax = temp;
		}

		for (int f=1; f<m_data.GetSize(); f++)
		{
			GetData(f, temp);

			if (temp != HMX_DATASET_VALUE_INVALID) 
			{
				if (temp < dMin)
					dMin = temp;

				if (temp > dMax)
					dMax = temp;
			}
		}

		if (m_bSetMinTo && !bDataOnly)
			nMin = min(dMin, m_dSetMinTo);
		else
			nMin = dMin;

		if (m_bSetMaxTo && !bDataOnly)
			nMax = max(dMax, m_dSetMaxTo);
		else
			nMax = dMax;

		return true;
	} 
	else if (!bDataOnly && m_bSetMinTo && m_bSetMaxTo)
	{
		nMin = m_dSetMinTo;
		nMax = m_dSetMaxTo;

		return true;
	}

	// else
	return false;
}

void CHMXDataset::SetMin(double dMin)
{
	m_bSetMinTo = true;
	m_dSetMinTo = dMin;
}

void CHMXDataset::SetMax(double dMax)
{
	m_bSetMaxTo = true;
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

