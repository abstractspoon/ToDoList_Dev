// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownGraphs.h"
#include "BurndownChart.h"
#include "BurndownStatic.h"

#include "..\shared\datehelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\misc.h"

/////////////////////////////////////////////////////////////////////////////

const COLORREF COLOR_RED = RGB(213, 0, 0);
const COLORREF COLOR_REDVIOLET = RGB(197, 17, 98);
const COLORREF COLOR_VIOLET = RGB(170, 0, 255);
const COLORREF COLOR_BLUEVIOLET = RGB(98, 0, 234);
const COLORREF COLOR_BLUE = RGB(41, 98, 255);
const COLORREF COLOR_BLUEGREEN = RGB(0, 184, 212);
const COLORREF COLOR_GREENBLUE = RGB(0, 191, 165);
const COLORREF COLOR_GREEN = RGB(0, 200, 83);
const COLORREF COLOR_GREENYELLOW = RGB(100, 221, 23);
const COLORREF COLOR_YELLOWGREEN = RGB(174, 234, 0);
const COLORREF COLOR_YELLOW = RGB(255, 214, 0);
const COLORREF COLOR_YELLOWORANGE = RGB(255, 171, 0);
const COLORREF COLOR_ORANGE = RGB(255, 109, 0);
const COLORREF COLOR_REDORANGE = RGB(221, 44, 0);

const COLORREF GRAPH_COLORS[] =
{
	COLOR_RED,
	COLOR_REDVIOLET,
	COLOR_VIOLET,
	COLOR_BLUEVIOLET,
	COLOR_BLUE,
	COLOR_BLUEGREEN,
	COLOR_GREENBLUE,
	COLOR_GREEN,
	COLOR_GREENYELLOW,
	COLOR_YELLOWGREEN,
	COLOR_YELLOW,
	COLOR_YELLOWORANGE,
	COLOR_ORANGE,
	COLOR_REDORANGE,
};
const int NUM_GRAPH_COLORS = (sizeof(GRAPH_COLORS) / sizeof(GRAPH_COLORS[0]));

/////////////////////////////////////////////////////////////////////////////////

const int GRAPH_LINE_THICKNESS = 1;
const int TREND_LINE_THICKNESS = 2;
const int MIN_XSCALE_SPACING   = GraphicsMisc::ScaleByDPIFactor(50); // pixels

/////////////////////////////////////////////////////////////////////////////////

static int SortProc(const void* pV1, const void* pV2)
{
	typedef CGraphBase* lpGraphBase;

	const CGraphBase* pGB1 = *(static_cast<const lpGraphBase*>(pV1));
	const CGraphBase* pGB2 = *(static_cast<const lpGraphBase*>(pV2));

	return Misc::NaturalCompare(pGB1->GetTitle(), pGB2->GetTitle());
}

// ----------------------------------------------------------

CGraphsMap::CGraphsMap()
{
	AddGraph(BCG_TIMESERIES_INCOMPLETETASKS,	new CIncompleteTasksTimeGraph());
	AddGraph(BCG_TIMESERIES_REMAININGDAYS,		new CRemainingDaysTimeGraph());
	AddGraph(BCG_TIMESERIES_STARTEDENDEDTASKS,	new CStartedEndedTasksTimeGraph());
	AddGraph(BCG_TIMESERIES_ESTIMATEDSPENTDAYS, new CEstimatedSpentDaysTimeGraph());

	AddGraph(BCG_FREQUENCY_CATEGORY,			new CCategoryFrequencyGraph());
	AddGraph(BCG_FREQUENCY_STATUS,				new CStatusFrequencyGraph());
	AddGraph(BCG_FREQUENCY_ALLOCTO,				new CAllocatedToFrequencyGraph());
	AddGraph(BCG_FREQUENCY_ALLOCBY,				new CAllocatedByFrequencyGraph());
	AddGraph(BCG_FREQUENCY_VERSION,				new CVersionFrequencyGraph());
	AddGraph(BCG_FREQUENCY_TAGS,				new CTagFrequencyGraph());
	AddGraph(BCG_FREQUENCY_PRIORITY,			new CPriorityFrequencyGraph());
	AddGraph(BCG_FREQUENCY_RISK,				new CRiskFrequencyGraph());

	AddGraph(BCG_MINMAX_ESTIMATEDSPENTDAYS,		new CEstimatedSpentDaysMinMaxGraph());
	AddGraph(BCG_MINMAX_DUEDONEDATES,			new CDueDoneDatesMinMaxGraph());
}

CGraphsMap::~CGraphsMap()
{
	POSITION pos = GetStartPosition();

	while (pos)
		delete GetNext(pos);

	RemoveAll();
}

BOOL CGraphsMap::AddGraph(BURNDOWN_GRAPH nGraph, CGraphBase* pGraph)
{
	if (HasGraph(nGraph))
	{
		ASSERT(0);
		delete pGraph;
		return FALSE;
	}

	pGraph->SetDisplayISODates(m_bISODates);
	SetAt(nGraph, pGraph);

	return TRUE;
}

CGraphBase* CGraphsMap::GetNext(POSITION& pos) const
{
	BURNDOWN_GRAPH nUnused;

	return GetNext(pos, nUnused);
}

CGraphBase* CGraphsMap::GetNext(POSITION& pos, BURNDOWN_GRAPH& nGraph) const
{
	CGraphBase* pGraph = NULL;
	GetNextAssoc(pos, nGraph, pGraph);

	return pGraph;
}

CGraphBase* CGraphsMap::GetGraph(BURNDOWN_GRAPH nGraph) const
{
	CGraphBase* pGraph = NULL;

	if (Lookup(nGraph, pGraph))
		ASSERT(pGraph);

	return pGraph;
}

CGraphBase* CGraphsMap::GetGraph(const CString& sCustomAttribID) const
{
	POSITION pos = GetStartPosition();

	while (pos)
	{
		CGraphBase* pGraph = GetNext(pos);
		CCustomAttributeGraph* pCGraph = dynamic_cast<CCustomAttributeGraph*>(pGraph);

		if (pCGraph && (pCGraph->GetUniqueID() == sCustomAttribID))
			return pGraph;
	}

	return NULL;
}

BOOL CGraphsMap::HasGraph(BURNDOWN_GRAPH nGraph) const
{
	return (GetGraph(nGraph) != NULL);
}

BURNDOWN_GRAPHTYPE CGraphsMap::GetType(BURNDOWN_GRAPH nGraph) const
{
	CGraphBase* pGraph = NULL;

	if (Lookup(nGraph, pGraph))
		return pGraph->GetType();

	return BCT_UNKNOWNTYPE;
}

CString CGraphsMap::GetTitle(BURNDOWN_GRAPH nGraph) const
{
	CGraphBase* pGraph = NULL;

	if (Lookup(nGraph, pGraph))
		return pGraph->GetTitle();

	return _T("");
}

int CGraphsMap::GetGraphs(BURNDOWN_GRAPHTYPE nType, CGraphArray& aGraphs, BOOL bSorted) const
{
	CArray<CGraphBase*, CGraphBase*&> aTemp;
	POSITION pos = GetStartPosition();

	while (pos)
	{
		CGraphBase* pGraph = GetNext(pos);

		if (pGraph && pGraph->HasType(nType))
			aTemp.Add(pGraph);
	}

	if (bSorted)
		Misc::SortArrayT<CGraphBase*>(aTemp, SortProc);

	int nItem = aTemp.GetSize();
	aGraphs.SetSize(nItem);

	while (nItem--)
		aGraphs[nItem] = aTemp[nItem]->GetGraph();

	return aGraphs.GetSize();
}

int CGraphsMap::GetColors(CGraphColorMap& mapColors) const
{
	mapColors.RemoveAll();
	POSITION pos = GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		const CGraphBase* pGraph = GetNext(pos, nGraph);

		CColorArray& aColors = const_cast<CColorArray&>(pGraph->GetColors());

		if (IsCustomAttributeGraph(nGraph))
			mapColors[GetCustomAttributeID(pGraph)] = aColors;
		else
			mapColors[Misc::Format(nGraph)] = aColors;
	}

	return mapColors.GetCount();
}

void CGraphsMap::SetAttributes(const CGraphAttributes& attrib)
{
	POSITION pos = GetStartPosition();
	BURNDOWN_GRAPH nGraph;
	BURNDOWN_GRAPHOPTION nOption;

	while (pos)
	{
		CGraphBase* pGraph = GetNext(pos, nGraph);
		CColorArray aColors;

		if (IsCustomAttributeGraph(nGraph))
		{
			CString sCustAttribID = GetCustomAttributeID(pGraph);

			attrib.GetColors(sCustAttribID, aColors);
			nOption = attrib.GetOption(sCustAttribID);
		}
		else
		{
			attrib.GetColors(nGraph, aColors);
			nOption = attrib.GetOption(nGraph);
		}
				
		if (aColors.GetSize())
		{
			ASSERT(nOption != BGO_INVALID);
			pGraph->SetColors(aColors);
			pGraph->SetOption(nOption);
		}
	}
}

int CGraphsMap::GetMaxColorCount() const
{
	int nMax = 0;
	POSITION pos = GetStartPosition();

	while (pos)
	{
		int nNumGraphColors = GetNext(pos)->GetColors().GetSize();
		nMax = max(nMax, nNumGraphColors);
	}

	return nMax;
}

BOOL CGraphsMap::Update(const CCustomAttributeDefinitionArray& aCustAttribDefs)
{
	BOOL bChange = FALSE;

	// Remove any graphs for no longer existing attributes
	// else Update the rest
	POSITION pos = GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		CGraphBase* pGraph = GetNext(pos, nGraph);

		if (IsCustomAttributeGraph(nGraph))
		{
			int nExist = aCustAttribDefs.Find(nGraph);
			
			if (nExist == -1)
			{
				RemoveKey(nGraph);
				bChange = TRUE;
			}
			else
			{
				CCustomAttributeGraph* pCGraph = dynamic_cast<CCustomAttributeGraph*>(pGraph);
				ASSERT(pCGraph);

				if (pCGraph)
				{
					const CUSTOMATTRIBDEF& defExist = aCustAttribDefs[nExist];
					bChange |= pCGraph->UpdateDefinition(defExist.sLabel, defExist.sListData);
				}
			}
		}
	}

	// Add any new graphs
	int nDef = aCustAttribDefs.GetSize();

	while (nDef--)
	{
		const CUSTOMATTRIBDEF& def = aCustAttribDefs[nDef];

		if (!HasGraph(def.nGraph))
		{
			CGraphBase* pGraph = NULL;

			switch (def.nType)
			{
			case BCT_TIMESERIES:	pGraph = new CCustomAttributeTimeSeriesGraph(def);	break;
			case BCT_FREQUENCY:		pGraph = new CCustomAttributeFrequencyGraph(def);	break;
//			case BCT_MINMAX:		pGraph = new CCustomAttributeMinMaxGraph(def);		break;

			default:
				ASSERT(0);
				break;
			}

			if (pGraph)
				bChange |= AddGraph(def.nGraph, pGraph);
		}
	}

	return bChange;
}

CString CGraphsMap::GetCustomAttributeID(BURNDOWN_GRAPH nGraph) const
{
	return GetCustomAttributeID(GetGraph(nGraph));
}

CString CGraphsMap::GetCustomAttributeID(const CGraphBase* pGraph) const
{
	const CCustomAttributeGraph* pCGraph = dynamic_cast<const CCustomAttributeGraph*>(pGraph);

	if (!pCGraph)
		return _T("");

	return pCGraph->GetUniqueID();
}

void CGraphsMap::SetDisplayISODates(BOOL bISO)
{
	m_bISODates = bISO;

	POSITION pos = GetStartPosition();

	while (pos)
		GetNext(pos)->SetDisplayISODates(bISO);
}

/////////////////////////////////////////////////////////////////////////////

CGraphBase::CGraphBase(BURNDOWN_GRAPH nGraph, BURNDOWN_GRAPHTYPE nType, BURNDOWN_GRAPHOPTION nOption)
	: 
	m_nGraph(nGraph), 
	m_nType(nType),
	m_nOption(nOption),
	m_dDataMin(HMX_DATASET_VALUE_INVALID),
	m_dDataMax(HMX_DATASET_VALUE_INVALID)
{
	ASSERT((nGraph != BCG_UNKNOWNGRAPH) && (nType != BCT_UNKNOWNTYPE));
}

CGraphBase::~CGraphBase()
{
}

void CGraphBase::UpdateDatasetColors(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	int nNumColors = m_aColors.GetSize();
	nNumColors = min(nNumColors, HMX_MAX_DATASET);

	for (int nColor = 0; nColor < nNumColors; nColor++)
		SetDatasetColor(datasets[nColor], m_aColors[nColor]);
}

void CGraphBase::SetDatasetColor(CHMXDataset& dataset, COLORREF crBase)
{
	dataset.SetLineColor(GraphicsMisc::Darker(crBase, 0.05, FALSE));
	dataset.SetFillColor(GraphicsMisc::Lighter(crBase, 0.25, FALSE));
}

void CGraphBase::ClearData(CHMXDataset datasets[HMX_MAX_DATASET])
{
	int nDataset = HMX_MAX_DATASET;

	while (nDataset--)
		datasets[nDataset].ClearData();
}

BOOL CGraphBase::GetDataMinMax(double& dMin, double& dMax) const
{
	dMin = m_dDataMin;
	dMax = m_dDataMax;

	return ((dMin != HMX_DATASET_VALUE_INVALID) && (dMax != HMX_DATASET_VALUE_INVALID));
}

BURNDOWN_GRAPHOPTION CGraphBase::GetOption() const
{
	return m_nOption;
}

CString CGraphBase::FormatDate(const COleDateTime& date) const
{
	if (!CDateHelper::IsDateSet(date))
		return _T("");

	if (m_bISODates)
		return CDateHelper::FormatDate(date, DHFD_ISO);

	return CDateHelper::FormatDate(date);
}

COLORREF CGraphBase::GetColor(int nColor) const
{
	int nNumColors = m_aColors.GetSize();

	if ((nColor < 0) || (nNumColors == 0))
	{
		ASSERT(0);
		return CLR_NONE;
	}

	// Rotate through colours
	nColor = (nColor % nNumColors);
	ASSERT(nColor < nNumColors);

	return m_aColors[nColor];
}

BOOL CGraphBase::SetOption(BURNDOWN_GRAPHOPTION nOption)
{
	if (!IsValidOption(nOption))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nOption == m_nOption)
		return FALSE;

	m_nOption = nOption;
	return TRUE;
}

BOOL CGraphBase::InitColorPalette(int nNumColors, int nStartColor)
{
	if ((nNumColors <= 0) || (nNumColors > NUM_GRAPH_COLORS) || (nStartColor >= NUM_GRAPH_COLORS))
	{
		ASSERT(0);
		return FALSE;
	}

	m_aColors.RemoveAll();

	int nOffset = 3; // Rotate through the colours
	int nColorIndex = nStartColor;

	for (int nColor = 0; nColor < nNumColors; nColor++)
	{
		COLORREF crNext = GRAPH_COLORS[nColorIndex];

		if (m_aColors.Has(crNext))
		{
			nColorIndex++;
			nColor--;
		}
		else
		{
			m_aColors.Add(crNext);

			nColorIndex += nOffset;
			nColorIndex %= NUM_GRAPH_COLORS;
		}
	}

	return TRUE;
}

BOOL CGraphBase::InitColorPalette(COLORREF color1, COLORREF color2, COLORREF color3)
{
	return (m_aColors.Set(color1, color2, color3) > 0);
}

BOOL CGraphBase::SetColors(const CColorArray& aColors)
{
	if (!aColors.GetSize() || (aColors[0] == CLR_NONE))
	{
		ASSERT(0);
		return FALSE;
	}

	if (Misc::MatchAll(m_aColors, aColors, TRUE))
		return FALSE;

	m_aColors.Copy(aColors);
	return TRUE;
}

const CColorArray& CGraphBase::GetColors() const
{
	return m_aColors;
}

void CGraphBase::RecalcDataMinMax(const CHMXDataset datasets[HMX_MAX_DATASET], double dIgnoreVal) const
{
	m_dDataMin = m_dDataMax = HMX_DATASET_VALUE_INVALID;

	// First dataset
	if (!datasets[0].GetMinMax(m_dDataMin, m_dDataMax, TRUE, dIgnoreVal))
		return;

	// Rest of datasets
	for (int f = 1; f < HMX_MAX_DATASET; f++)
	{
		double dMin, dMax;

		if (!datasets[f].GetMinMax(dMin, dMax, TRUE, dIgnoreVal))
			break;

		m_dDataMin = min(m_dDataMin, dMin);
		m_dDataMax = max(m_dDataMax, dMax);
	}
}

BOOL CGraphBase::IsValidOption(BURNDOWN_GRAPHOPTION nOption) const
{
	return ::IsValidOption(nOption, m_nType);
}

BOOL CGraphBase::HasOption(BURNDOWN_GRAPHOPTION nOption) const
{
	return (nOption == m_nOption);
}

////////////////////////////////////////////////////////////////////////////////////////

CTimeSeriesGraph::CTimeSeriesGraph(BURNDOWN_GRAPH nGraph) : CGraphBase(nGraph, BCT_TIMESERIES, BGO_TREND_NONE)
{
}

CTimeSeriesGraph::~CTimeSeriesGraph()
{
}

BOOL CTimeSeriesGraph::CalculateBestFitLine(const CHMXDataset& datasetSrc, CHMXDataset& datasetDest)
{
	int nNumData = datasetSrc.GetDatasetSize();

	if (nNumData < 2)
		return FALSE;

	double dSumX = 0, dSumXSquared = 0, dSumY = 0, dSumXY = 0;
	int nData;

	for (nData = 0; nData < nNumData; nData++)
	{
		double dXValue = nData, dYValue;
		VERIFY(datasetSrc.GetData(nData, dYValue));

		dSumX			+= dXValue;
		dSumY			+= dYValue;
		dSumXSquared	+= (dXValue * dXValue);
		dSumXY			+= (dXValue * dYValue);
	}
	
	double dSlope = ((nNumData * dSumXY) - (dSumX * dSumY)) / ((nNumData * dSumXSquared) - (dSumX * dSumX));
	double dIntercept = ((dSumXSquared * dSumY) - (dSumX * dSumXY)) / ((dSumXSquared * nNumData) - (dSumX * dSumX));

	// Calculate the data points
	datasetDest.SetDatasetSize(nNumData);

	for (nData = 0; nData < nNumData; nData++)
	{
		double dLine = ((dSlope * nData) + dIntercept);
		dLine = max(dLine, 0.0);

		datasetDest.SetData(nData, dLine);
	}

	return TRUE;
}

BOOL CTimeSeriesGraph::CalculateMovingAverage(int nWindowSize, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest)
{
	int nNumData = datasetSrc.GetDatasetSize();

	if (nNumData < nWindowSize)
		return FALSE;

	datasetDest.SetDatasetSize(nNumData);

	// The first value is the first data value
	double dValue = 0, dPrevAverage = 0, dPrevWindowStart = 0;

	datasetSrc.GetData(0, dValue);
	datasetDest.SetData(0, dValue);
	
	// The first 'nWindow' values are simple accumulated averages
	int nData;

	for (nData = 1; nData < nWindowSize; nData++)
	{
		datasetDest.GetData((nData - 1), dPrevAverage);
		datasetSrc.GetData(nData, dValue);

		dValue = (((dPrevAverage * nData) + dValue) / (nData + 1));
		datasetDest.SetData(nData, dValue);
	}

	// All the rest require the rolling 
	for (nData = nWindowSize; nData < nNumData; nData++)
	{
		datasetDest.GetData((nData - 1), dPrevAverage);
		datasetSrc.GetData(nData - nWindowSize, dPrevWindowStart);
		datasetSrc.GetData(nData, dValue);

		dValue = (((dPrevAverage * nWindowSize) - dPrevWindowStart + dValue) / nWindowSize);
		datasetDest.SetData(nData, dValue);
	}
	
	return TRUE;
}

void CTimeSeriesGraph::RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const
{
	// Because months have unequal lengths we need
	// to specify labels on a day by day basis
	nLabelStep = 1;

	int nNumDays = calculator.GetTotalDays();
	aLabels.SetSize(nNumDays + 1);

	// build ticks
	COleDateTime dtTick = calculator.GetStartDate();
	CDateHelper dh;

	BURNDOWN_GRAPHSCALE nScale = CalculateRequiredScale(nAvailWidth, nNumDays, MIN_XSCALE_SPACING);

	for (int nDay = 0; nDay <= nNumDays; )
	{
		aLabels.SetAt(nDay, FormatDate(dtTick));

		// next Tick date
		COleDateTime dtNextTick(dtTick);

		switch (nScale)
		{
		case BCS_DAY:
			dtNextTick.m_dt += 1.0;
			break;

		case BCS_WEEK:
			dh.OffsetDate(dtNextTick, 1, DHU_WEEKS);
			break;

		case BCS_MONTH:
			dh.OffsetDate(dtNextTick, 1, DHU_MONTHS);
			break;

		case BCS_2MONTH:
			dh.OffsetDate(dtNextTick, 2, DHU_MONTHS);
			break;

		case BCS_QUARTER:
			dh.OffsetDate(dtNextTick, 3, DHU_MONTHS);
			break;

		case BCS_HALFYEAR:
			dh.OffsetDate(dtNextTick, 6, DHU_MONTHS);
			break;

		case BCS_YEAR:
			dh.OffsetDate(dtNextTick, 1, DHU_YEARS);
			break;

		default:
			ASSERT(0);
		}

		nDay += (int)(dtNextTick.m_dt - dtTick.m_dt);

		dtTick = dtNextTick;
	}
}

void CTimeSeriesGraph::UpdateDatasetColors(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CGraphBase::UpdateDatasetColors(datasets);

	if (GetOption() != BGO_TREND_NONE)
	{
		int nNumGraphs = GetColors().GetSize();
		CGraphBase::UpdateDatasetColors(&datasets[nNumGraphs]);
	}
}

BOOL CTimeSeriesGraph::GetDataMinMax(double& dMin, double& dMax) const
{
	if (!CGraphBase::GetDataMinMax(dMin, dMax))
		return FALSE;

	dMin = 0.0;
	return TRUE;
}

BOOL CTimeSeriesGraph::OnOptionChanged(BURNDOWN_GRAPHOPTION /*nOption*/, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	return CalculateTrendLines(datasets);
}

BOOL CTimeSeriesGraph::CalculateTrendLine(BURNDOWN_GRAPHOPTION nOption, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest)
{
	BOOL bSuccess = FALSE;

	switch (nOption)
	{
	case BGO_TREND_NONE:
		bSuccess = TRUE;
		break;

	case BGO_TREND_BESTFIT:
		bSuccess = CalculateBestFitLine(datasetSrc, datasetDest);
		break;

	case BGO_TREND_7DAYAVERAGE:
		bSuccess = CalculateMovingAverage(7, datasetSrc, datasetDest);
		break;

	case BGO_TREND_30DAYAVERAGE:
		bSuccess = CalculateMovingAverage(30, datasetSrc, datasetDest);
		break;

	case BGO_TREND_90DAYAVERAGE:
		bSuccess = CalculateMovingAverage(90, datasetSrc, datasetDest);
		break;

	default:
		ASSERT(0);
		break;
	}

	if ((nOption == BGO_TREND_NONE) || !bSuccess)
		datasetDest.Reset();

	if (bSuccess)
	{
		datasetDest.SetSize(TREND_LINE_THICKNESS);
		datasetDest.SetStyle(HMX_DATASET_STYLE_LINE_DOTTED); 
		datasetDest.SetLineColor(GraphicsMisc::Darker(datasetSrc.GetLineColor(), 0.2, FALSE));
	}

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////

CIncompleteTasksTimeGraph::CIncompleteTasksTimeGraph()
	:
	CTimeSeriesGraph(BCG_TIMESERIES_INCOMPLETETASKS)
{
	InitColorPalette(COLOR_GREEN);
}

CString CIncompleteTasksTimeGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_INCOMPLETE);
}

void CIncompleteTasksTimeGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[0].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[0].SetSize(GRAPH_LINE_THICKNESS);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays)
	{
		datasets[0].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		int nItemFrom = 0;
		int nNumNotDone = 0;

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// First value is always zero
			if (nDay > 0)
				nNumNotDone = calculator.GetIncompleteTaskCount(date, nItemFrom, nItemFrom);

			datasets[0].SetData(nDay, nNumNotDone);
		}

		CalculateTrendLines(datasets);
	}

	RecalcDataMinMax(datasets);
}

CString CIncompleteTasksTimeGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	CString sTooltip;
	double dNumTasks;

	if (datasets[0].GetData(nHit, dNumTasks))
	{
		double dDate = (calculator.GetStartDate() + nHit);
		sTooltip.Format(CEnString(IDS_TOOLTIP_INCOMPLETE), CDateHelper::FormatDate(dDate), (int)dNumTasks);
	}

	return sTooltip;
}

BOOL CIncompleteTasksTimeGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	return CalculateTrendLine(GetOption(), datasets[0], datasets[1]);
}

//////////////////////////////////////////////////////////////////////////////////////

CRemainingDaysTimeGraph::CRemainingDaysTimeGraph()
	:
	CTimeSeriesGraph(BCG_TIMESERIES_REMAININGDAYS)
{
	InitColorPalette(COLOR_BLUE, COLOR_YELLOW);
}

CString CRemainingDaysTimeGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_REMAINING);
}

void CRemainingDaysTimeGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[REMAINING_ESTIMATE].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[REMAINING_ESTIMATE].SetSize(GRAPH_LINE_THICKNESS);
	
	datasets[REMAINING_SPENT].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[REMAINING_SPENT].SetSize(GRAPH_LINE_THICKNESS);
	
	// build the graph
	int nNumDays = calculator.GetTotalDays();
	
	if (nNumDays > 0)
	{
		datasets[REMAINING_ESTIMATE].SetDatasetSize(nNumDays + 1);
		datasets[REMAINING_SPENT].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		double dTotalEst = calculator.GetDaysEstimated();

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// Time Estimate
			double dEst = ((nDay * dTotalEst) / nNumDays);
			datasets[REMAINING_ESTIMATE].SetData(nDay, dTotalEst - dEst);
		
			// Time Spent
			double dSpent = calculator.GetDaysSpent(date);
			datasets[REMAINING_SPENT].SetData(nDay, dTotalEst - dSpent);
		}

		// Ensure last Time Estimate value is always zero
		datasets[REMAINING_ESTIMATE].SetData(nNumDays, 0.0);

		CalculateTrendLines(datasets);
	}

	RecalcDataMinMax(datasets);
}

CString CRemainingDaysTimeGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate() + nHit), dNumEst, dNumSpent;
	CString sTooltip;

	if (datasets[REMAINING_SPENT].GetData(nHit, dNumSpent) &&
		datasets[REMAINING_ESTIMATE].GetData(nHit, dNumEst))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_REMAINING), CDateHelper::FormatDate(dDate), (int)dNumEst, (int)dNumSpent);
	}

	return sTooltip;
}

BOOL CRemainingDaysTimeGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	// No need to calculate trend of remaining estimate because it's already a straight line
	return CalculateTrendLine(GetOption(), datasets[REMAINING_SPENT], datasets[REMAINING_SPENT + 1]);
}

//////////////////////////////////////////////////////////////////////////////////////

CStartedEndedTasksTimeGraph::CStartedEndedTasksTimeGraph()
	:
	CTimeSeriesGraph(BCG_TIMESERIES_STARTEDENDEDTASKS)
{
	InitColorPalette(COLOR_RED, COLOR_GREEN);
}

CString CStartedEndedTasksTimeGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_STARTEDENDED);
}

void CStartedEndedTasksTimeGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[STARTED_TASKS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[STARTED_TASKS].SetSize(GRAPH_LINE_THICKNESS);

	datasets[ENDED_TASKS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ENDED_TASKS].SetSize(GRAPH_LINE_THICKNESS);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays > 0)
	{
		datasets[STARTED_TASKS].SetDatasetSize(nNumDays + 1);
		datasets[ENDED_TASKS].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		int nNumStarted = 0, nNumDone = 0;

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// First value is always zero
			if (nDay > 0)
				calculator.GetStartedEndedTasks(date, nNumStarted, nNumDone);

			datasets[STARTED_TASKS].SetData(nDay, nNumStarted);
			datasets[ENDED_TASKS].SetData(nDay, nNumDone);
		}

		CalculateTrendLines(datasets);
	}

	RecalcDataMinMax(datasets);
}

CString CStartedEndedTasksTimeGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate() + nHit), dNumStarted, nNumDone;
	CString sTooltip;

	if (datasets[STARTED_TASKS].GetData(nHit, dNumStarted) &&
		datasets[ENDED_TASKS].GetData(nHit, nNumDone))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_STARTEDENDED), CDateHelper::FormatDate(dDate), (int)dNumStarted, (int)nNumDone);
	}

	return sTooltip;
}

BOOL CStartedEndedTasksTimeGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	if (CalculateTrendLine(GetOption(), datasets[STARTED_TASKS], datasets[STARTED_TASKS + 2]) &&
		CalculateTrendLine(GetOption(), datasets[ENDED_TASKS], datasets[ENDED_TASKS + 2]))
	{
		return TRUE;
	}

	datasets[STARTED_TASKS + 2].Reset();
	datasets[ENDED_TASKS + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CEstimatedSpentDaysTimeGraph::CEstimatedSpentDaysTimeGraph()
	:
	CTimeSeriesGraph(BCG_TIMESERIES_ESTIMATEDSPENTDAYS)
{
	InitColorPalette(COLOR_VIOLET, COLOR_GREEN);
}

CString CEstimatedSpentDaysTimeGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTDAYS);
}

void CEstimatedSpentDaysTimeGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[ESTIMATED_DAYS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ESTIMATED_DAYS].SetSize(GRAPH_LINE_THICKNESS);

	datasets[SPENT_DAYS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[SPENT_DAYS].SetSize(GRAPH_LINE_THICKNESS);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays > 0)
	{
		datasets[ESTIMATED_DAYS].SetDatasetSize(nNumDays + 1);
		datasets[SPENT_DAYS].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		double dDaysEst = 0.0, dDaysSpent = 0;

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// First value is always zero
			if (nDay > 0)
				calculator.GetDaysEstimatedSpent(date, dDaysEst, dDaysSpent);

			datasets[ESTIMATED_DAYS].SetData(nDay, dDaysEst);
			datasets[SPENT_DAYS].SetData(nDay, dDaysSpent);
		}

		CalculateTrendLines(datasets);
	}

	RecalcDataMinMax(datasets);
}

CString CEstimatedSpentDaysTimeGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate() + nHit), dDaysEst, dDaysSpent;
	CString sTooltip;

	if (datasets[ESTIMATED_DAYS].GetData(nHit, dDaysEst) &&
		datasets[SPENT_DAYS].GetData(nHit, dDaysSpent))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_ESTIMATEDSPENT), CDateHelper::FormatDate(dDate), dDaysEst, dDaysSpent);
	}

	return sTooltip;
}

BOOL CEstimatedSpentDaysTimeGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	if (CalculateTrendLine(GetOption(), datasets[ESTIMATED_DAYS], datasets[ESTIMATED_DAYS + 2]) &&
		CalculateTrendLine(GetOption(), datasets[SPENT_DAYS], datasets[SPENT_DAYS + 2]))
	{
		return TRUE;
	}

	datasets[ESTIMATED_DAYS + 2].Reset();
	datasets[SPENT_DAYS + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CEstimatedSpentCostGraph::CEstimatedSpentCostGraph()
	:
	CTimeSeriesGraph(BCG_TIMESERIES_ESTIMATEDSPENTCOST)
{
	InitColorPalette(COLOR_YELLOW, COLOR_VIOLET);
}

CString CEstimatedSpentCostGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTCOST);
}

void CEstimatedSpentCostGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[ESTIMATED_COST].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ESTIMATED_COST].SetSize(GRAPH_LINE_THICKNESS);

	datasets[SPENT_COST].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[SPENT_COST].SetSize(GRAPH_LINE_THICKNESS);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays > 0)
	{
		datasets[ESTIMATED_COST].SetDatasetSize(nNumDays + 1);
		datasets[SPENT_COST].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		double dCostEst = 0.0, dCostSpent = 0;

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// First value is always zero
			if (nDay > 0)
				calculator.GetCostEstimatedSpent(date, dCostEst, dCostSpent);

			datasets[ESTIMATED_COST].SetData(nDay, dCostEst);
			datasets[SPENT_COST].SetData(nDay, dCostSpent);
		}

		CalculateTrendLines(datasets);
	}

	RecalcDataMinMax(datasets);
}

CString CEstimatedSpentCostGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate() + nHit), dCostEst, dCostSpent;
	CString sTooltip;

	if (datasets[ESTIMATED_COST].GetData(nHit, dCostEst) &&
		datasets[SPENT_COST].GetData(nHit, dCostSpent))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_ESTIMATEDSPENT), CDateHelper::FormatDate(dDate), dCostEst, dCostSpent);
	}

	return sTooltip;
}

BOOL CEstimatedSpentCostGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	if (CalculateTrendLine(GetOption(), datasets[ESTIMATED_COST], datasets[ESTIMATED_COST + 2]) &&
		CalculateTrendLine(GetOption(), datasets[SPENT_COST], datasets[SPENT_COST + 2]))
	{
		return TRUE;
	}

	datasets[ESTIMATED_COST + 2].Reset();
	datasets[SPENT_COST + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CFrequencyGraph::CFrequencyGraph(BURNDOWN_GRAPH nGraph) 
	: 
	CGraphBase(nGraph, BCT_FREQUENCY, BGO_FREQUENCY_BAR)
{
}

CFrequencyGraph::~CFrequencyGraph()
{

}

void CFrequencyGraph::RebuildXScale(const CStatsItemCalculator& /*calculator*/, int /*nAvailWidth*/, CStringArray& aLabels, int& nLabelStep) const
{
	nLabelStep = 1;
	aLabels.Copy(m_aAttribValues);
}

void CFrequencyGraph::BuildGraph(const CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	// Save off attrib values for building horizontal labels
	m_aAttribValues.RemoveAll();

	int nNumAttrib = aFrequencies.GetSize();

	if (nNumAttrib)
	{
		datasets[0].SetDatasetSize(nNumAttrib);

		for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
		{
			const FREQUENCYITEM& fi = aFrequencies[nAtt];
			datasets[0].SetData(nAtt, fi.nCount);

			if (fi.sLabel.IsEmpty())
				m_aAttribValues.Add(CEnString(IDS_NONE));
			else
				m_aAttribValues.Add(fi.sLabel);
		}

		UpdateGraphStyles(datasets[0]);
	}

	RecalcDataMinMax(datasets);
}

BOOL CFrequencyGraph::GetDataMinMax(double& dMin, double& dMax) const
{
	if (!CGraphBase::GetDataMinMax(dMin, dMax))
		return FALSE;

	dMin = 0.0;
	return TRUE;
}

CString CFrequencyGraph::GetTooltip(const CStatsItemCalculator& /*calculator*/, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	CString sTooltip;
	double dCount = 0.0;

	if (datasets[0].GetData(nHit, dCount))
	{
		ASSERT(m_aAttribValues.GetSize() > nHit);
		sTooltip.Format(CEnString(IDS_TOOLTIP_ATTRIBFREQUENCY), m_aAttribValues[nHit], (int)dCount);
	}

	return sTooltip;
}

BOOL CFrequencyGraph::HasAxes() const 
{ 
	return (!HasOption(BGO_FREQUENCY_DONUT) && 
			!HasOption(BGO_FREQUENCY_PIE)); 
}

BOOL CFrequencyGraph::OnOptionChanged(BURNDOWN_GRAPHOPTION /*nOption*/, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	return UpdateGraphStyles(datasets[0]);
}

BOOL CFrequencyGraph::UpdateGraphStyles(CHMXDataset& dataset) const
{
	switch (GetOption())
	{
	case BGO_FREQUENCY_BAR:
		dataset.SetSize(5);
		dataset.SetStyle(HMX_DATASET_STYLE_VBAR);
		break;

	case BGO_FREQUENCY_LINE:
		dataset.SetSize(1);
		dataset.SetStyle(HMX_DATASET_STYLE_AREALINE);
		break;

	case BGO_FREQUENCY_PIE:
		dataset.SetSize(1);
		dataset.SetStyle(HMX_DATASET_STYLE_PIELINE);
		break;

	case BGO_FREQUENCY_DONUT:
		dataset.SetSize(1);
		dataset.SetStyle(HMX_DATASET_STYLE_DONUTLINE);
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------

CCategoryFrequencyGraph::CCategoryFrequencyGraph() 
	: 
	CFrequencyGraph(BCG_FREQUENCY_CATEGORY)
{
	InitColorPalette(7, 1);
}

CString CCategoryFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_CATEGORY);
}

void CCategoryFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetCategoryFrequencies(aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CStatusFrequencyGraph::CStatusFrequencyGraph()
	:
	CFrequencyGraph(BCG_FREQUENCY_STATUS)
{
	InitColorPalette(7, 3);
}

CString CStatusFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_STATUS);
}

void CStatusFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetStatusFrequencies(aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CAllocatedToFrequencyGraph::CAllocatedToFrequencyGraph()
	:
	CFrequencyGraph(BCG_FREQUENCY_ALLOCTO)
{
	InitColorPalette(7, 5);
}

CString CAllocatedToFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_ALLOCTO);
}

void CAllocatedToFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetAllocatedToFrequencies(aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CAllocatedByFrequencyGraph::CAllocatedByFrequencyGraph()
	:
	CFrequencyGraph(BCG_FREQUENCY_ALLOCBY)
{
	InitColorPalette(7, 7);
}

CString CAllocatedByFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_ALLOCBY);
}

void CAllocatedByFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetAllocatedByFrequencies(aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CVersionFrequencyGraph::CVersionFrequencyGraph()
	:
	CFrequencyGraph(BCG_FREQUENCY_VERSION)
{
	InitColorPalette(7, 9);
}

CString CVersionFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_VERSION);
}

void CVersionFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetVersionFrequencies(aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CTagFrequencyGraph::CTagFrequencyGraph()
	:
	CFrequencyGraph(BCG_FREQUENCY_TAGS)
{
	InitColorPalette(7, 11);
}

CString CTagFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_TAGS);
}

void CTagFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetTagFrequencies(aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CPriorityFrequencyGraph::CPriorityFrequencyGraph()
	:
	CFrequencyGraph(BCG_FREQUENCY_PRIORITY)
{
	InitColorPalette(11, 13);
}

CString CPriorityFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_PRIORITY);
}

void CPriorityFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetPriorityFrequencies(aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CRiskFrequencyGraph::CRiskFrequencyGraph()
	:
	CFrequencyGraph(BCG_FREQUENCY_RISK)
{
	InitColorPalette(11, 2);
}

CString CRiskFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_RISK);
}

void CRiskFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetRiskFrequencies(aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

/////////////////////////////////////////////////////////////////////////////

CMinMaxGraph::CMinMaxGraph(BURNDOWN_GRAPH nGraph) 
	:
	CGraphBase(nGraph, BCT_MINMAX, BGO_MINMAX_NONE),
	m_nItemOffset(0)
{
}

CMinMaxGraph::~CMinMaxGraph()
{
}

void CMinMaxGraph::RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const
{
	nLabelStep = 1;
	m_nItemOffset = 0;

	int nFrom, nTo;
	int nNumItems = calculator.GetItemRange(nFrom, nTo);

	aLabels.SetSize(nNumItems);

	// build ticks
	if (nNumItems)
	{
		m_nItemOffset = nFrom; // For tooltip handling

		COleDateTime dtTick = calculator.GetStartDate(), dtItem;
		CDateHelper dh;

#if 0
		int nInterval = max(1, (nNumItems / 10));

		for (int nItem = nFrom; nItem < nTo; nItem += nInterval)
		{
			if (calculator.GetItemEndDate(nItem, dtTick))
				aLabels.SetAt(nItem - nFrom, dh.FormatDate(dtTick));
		}
#else
		BURNDOWN_GRAPHSCALE nScale = CalculateRequiredScale(nAvailWidth, calculator.GetTotalDays(), MIN_XSCALE_SPACING);
		int nLastTick = -1;

		for (int nItem = nFrom; nItem <= nTo; nItem++)
		{
			if (!calculator.GetItemStartDate(nItem, dtItem))
				continue;
			
			// Skip items until we get to the next Tick date
			if (dtItem < dtTick)
				continue;

			aLabels.SetAt(nItem - nFrom, FormatDate(dtItem));
			nLastTick = nItem;

			// next Tick date
			do
			{
				switch (nScale)
				{
				case BCS_DAY:
					dtTick.m_dt += 1.0;
					break;

				case BCS_WEEK:
					dh.OffsetDate(dtTick, 1, DHU_WEEKS);
					break;

				case BCS_MONTH:
					dh.OffsetDate(dtTick, 1, DHU_MONTHS);
					break;

				case BCS_2MONTH:
					dh.OffsetDate(dtTick, 2, DHU_MONTHS);
					break;

				case BCS_QUARTER:
					dh.OffsetDate(dtTick, 3, DHU_MONTHS);
					break;

				case BCS_HALFYEAR:
					dh.OffsetDate(dtTick, 6, DHU_MONTHS);
					break;

				case BCS_YEAR:
					dh.OffsetDate(dtTick, 1, DHU_YEARS);
					break;

				default:
					ASSERT(0);
				}
			}
			while (dtTick <= dtItem);
		}

		// Add label for last item further away than 1 item
		if ((nLastTick < (nTo - 1)) && calculator.GetItemEndDate(nTo, dtItem))
			aLabels.SetAt(nTo - nFrom, dh.FormatDate(dtItem));
#endif
	}
}

BOOL CMinMaxGraph::OnOptionChanged(BURNDOWN_GRAPHOPTION /*nOption*/, CHMXDataset /*datasets*/[HMX_MAX_DATASET]) const
{
	return FALSE;
}

BOOL CMinMaxGraph::GetDataMinMax(double& dMin, double& dMax) const
{
	if (!CGraphBase::GetDataMinMax(dMin, dMax))
		return FALSE;

	dMin = 0.0;
	return TRUE;
}

// ---------------------------------------------------------------------------

CEstimatedSpentDaysMinMaxGraph::CEstimatedSpentDaysMinMaxGraph()
	:
	CMinMaxGraph(BCG_MINMAX_ESTIMATEDSPENTDAYS)
{
	InitColorPalette(COLOR_GREEN, COLOR_RED);
}

CString CEstimatedSpentDaysMinMaxGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTDAYS);
}

void CEstimatedSpentDaysMinMaxGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[ESTIMATED_DAYS].SetStyle(HMX_DATASET_STYLE_MINMAX);
	datasets[ESTIMATED_DAYS].SetSize(GRAPH_LINE_THICKNESS);
	datasets[ESTIMATED_DAYS].SetMarker(HMX_DATASET_MARKER_CIRCLE);

	datasets[SPENT_DAYS].SetStyle(HMX_DATASET_STYLE_MINMAX);
	datasets[SPENT_DAYS].SetSize(GRAPH_LINE_THICKNESS);
	datasets[SPENT_DAYS].SetMarker(HMX_DATASET_MARKER_CIRCLE);

	// build the graph
	int nFrom, nTo;
	int nNumItems = calculator.GetItemRange(nFrom, nTo);

	if (nNumItems)
	{
		datasets[ESTIMATED_DAYS].SetDatasetSize(nNumItems);
		datasets[SPENT_DAYS].SetDatasetSize(nNumItems);

		for (int nItem = nFrom; nItem <= nTo; nItem++)
		{
			double dDaysEst = 0.0, dDaysSpent = 0;

			if (calculator.GetItemDaysEstimatedSpent(nItem, dDaysEst, dDaysSpent))
			{
				datasets[ESTIMATED_DAYS].SetData(nItem - nFrom, dDaysEst);
				datasets[SPENT_DAYS].SetData(nItem - nFrom, dDaysSpent);
			}
		}
	}

	RecalcDataMinMax(datasets);
}

CString CEstimatedSpentDaysMinMaxGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset /*datasets*/[HMX_MAX_DATASET], int nHit) const
{
	int nItem = (nHit + m_nItemOffset);
	CString sTitle = calculator.GetItemTitle(nItem), sTooltip;

	if (!sTitle.IsEmpty())
	{
		double dDaysEst = 0.0, dDaysSpent = 0.0;
		calculator.GetItemDaysEstimatedSpent(nItem, dDaysEst, dDaysSpent);

		sTooltip.Format(CEnString(IDS_TOOLTIP_ESTIMATEDSPENT), sTitle, dDaysEst, dDaysSpent);
	}

	return sTooltip;
}

// ---------------------------------------------------------------------------

CDueDoneDatesMinMaxGraph::CDueDoneDatesMinMaxGraph()
	:
	CMinMaxGraph(BCG_MINMAX_DUEDONEDATES)
{
	InitColorPalette(COLOR_BLUEGREEN, COLOR_ORANGE);
}

CString CDueDoneDatesMinMaxGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_DUEDONEDATES);
}

void CDueDoneDatesMinMaxGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[DUE_DATE].SetStyle(HMX_DATASET_STYLE_MINMAX);
	datasets[DUE_DATE].SetSize(GRAPH_LINE_THICKNESS);
	datasets[DUE_DATE].SetMarker(HMX_DATASET_MARKER_CIRCLE);

	datasets[DONE_DATE].SetStyle(HMX_DATASET_STYLE_MINMAX);
	datasets[DONE_DATE].SetSize(GRAPH_LINE_THICKNESS);
	datasets[DONE_DATE].SetMarker(HMX_DATASET_MARKER_CIRCLE);

	// build the graph
	int nFrom, nTo;
	int nNumItems = calculator.GetItemRange(nFrom, nTo);

	if (nNumItems)
	{
		datasets[DUE_DATE].SetDatasetSize(nNumItems);
		datasets[DONE_DATE].SetDatasetSize(nNumItems);

		for (int nItem = nFrom; nItem <= nTo; nItem++)
		{
			COleDateTime dtDue, dtDone;

			if (calculator.GetItemDueDoneDates(nItem, dtDue, dtDone))
			{
				datasets[DUE_DATE].SetData(nItem - nFrom, (int)dtDue.m_dt);
				datasets[DONE_DATE].SetData(nItem - nFrom, (int)dtDone.m_dt);
			}
		}
	}

	RecalcDataMinMax(datasets, 0.0); // Ignore unset dates
}

BOOL CDueDoneDatesMinMaxGraph::GetDataMinMax(double& dMin, double& dMax) const
{
	// Bypass CMinMaxGraph
	return CGraphBase::GetDataMinMax(dMin, dMax);
}

CString CDueDoneDatesMinMaxGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset /*datasets*/[HMX_MAX_DATASET], int nHit) const
{
	int nItem = (nHit + m_nItemOffset);
	CString sTitle = calculator.GetItemTitle(nItem), sTooltip;

	if (!sTitle.IsEmpty())
	{
		COleDateTime dtDue, dtDone;
		calculator.GetItemDueDoneDates(nItem, dtDue, dtDone);

		sTooltip.Format(CEnString(IDS_TOOLTIP_DUEDONEDATES), sTitle, dtDue.Format(VAR_DATEVALUEONLY), dtDone.Format(VAR_DATEVALUEONLY));
	}

	return sTooltip;
}

////////////////////////////////////////////////////////////////////////////////

CCustomAttributeGraph::CCustomAttributeGraph(const CUSTOMATTRIBDEF& def)
	:
	m_custDefinition(def)
{
}

BOOL CCustomAttributeGraph::UpdateDefinition(const CString& sLabel, const CString& sListData)
{
	if ((m_custDefinition.sLabel == sLabel) && (m_custDefinition.sListData == sListData))
		return FALSE;

	m_custDefinition.sLabel = sLabel;
	m_custDefinition.sListData = sListData;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

CCustomAttributeTimeSeriesGraph::CCustomAttributeTimeSeriesGraph(const CUSTOMATTRIBDEF& def)
	:
	CTimeSeriesGraph(def.nGraph),
	CCustomAttributeGraph(def)
{
	InitColorPalette(1, def.nGraph - BCG_CUSTOMATTRIB_FIRST);
}

CString CCustomAttributeTimeSeriesGraph::GetTitle() const
{
	return CEnString(IDS_CUSTOMATTRIB_GRAPH, m_custDefinition.sLabel);
}

void CCustomAttributeTimeSeriesGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[0].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[0].SetSize(GRAPH_LINE_THICKNESS);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays > 0)
	{
		datasets[0].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			double dValue = calculator.GetTotalAttribValue(m_custDefinition.sUniqueID, date);
			datasets[0].SetData(nDay, dValue);
		}

		CalculateTrendLines(datasets);
	}

	RecalcDataMinMax(datasets);
}

BOOL CCustomAttributeTimeSeriesGraph::GetDataMinMax(double& dMin, double& dMax) const
{
	// Bypass CMinMaxGraph
	if (!CGraphBase::GetDataMinMax(dMin, dMax))
		return FALSE;

	if ((dMin > 0.0) && (dMax > 0.0))
	{
		dMin = 0.0;
	}
	else if ((dMin < 0.0) && (dMax < 0.0))
	{
		dMax = 0.0;
	}

	return TRUE;
}

CString CCustomAttributeTimeSeriesGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dValue = 0;
	CString sTooltip;

	if (datasets[0].GetData(nHit, dValue))
	{
		double dDate = (calculator.GetStartDate() + nHit);
		sTooltip.Format(_T("%s\n%s"), CDateHelper::FormatDate(dDate), Misc::Format(dValue));
	}

	return sTooltip;
}

BOOL CCustomAttributeTimeSeriesGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	return CalculateTrendLine(GetOption(), datasets[0], datasets[1]);
}

////////////////////////////////////////////////////////////////////////////////

CCustomAttributeFrequencyGraph::CCustomAttributeFrequencyGraph(const CUSTOMATTRIBDEF& def)
	:
	CFrequencyGraph(def.nGraph),
	CCustomAttributeGraph(def)
{
	InitColorPalette(NUM_GRAPH_COLORS, (def.nGraph - BCG_CUSTOMATTRIB_FIRST));
}

CString CCustomAttributeFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_CUSTOMATTRIB_GRAPH, m_custDefinition.sLabel);
}

void CCustomAttributeFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetCustomAttributeFrequencies(m_custDefinition.sUniqueID, aFrequencies);

	CFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

////////////////////////////////////////////////////////////////////////////////

/*
CCustomAttributeMinMaxGraph::CCustomAttributeMinMaxGraph(const CUSTOMATTRIBDEF& def)
	:
	CMinMaxGraph(def.nGraph),
	CCustomAttributeGraph(def)
{
	InitColorPalette(2, def.nGraph - BCG_CUSTOMATTRIB_FIRST);
}

CString CCustomAttributeMinMaxGraph::GetTitle() const
{
	return CEnString(IDS_CUSTOMATTRIB_GRAPH, m_custDefinition.sLabel);
}

void CCustomAttributeMinMaxGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	// TODO
}

CString CCustomAttributeMinMaxGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	// TODO
	return _T("");
}

*/
