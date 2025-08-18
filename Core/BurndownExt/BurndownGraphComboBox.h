#if !defined(AFX_BURNDOWNGRAPHCOMBOBOX_H__E072EBC4_11A8_4845_8899_F7958796AC12__INCLUDED_)
#define AFX_BURNDOWNGRAPHCOMBOBOX_H__E072EBC4_11A8_4845_8899_F7958796AC12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurndownGraphComboBox.h : header file
//

#include "BurndownGraphs.h"

#include "..\Shared\ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////

class CBurndownChart;

/////////////////////////////////////////////////////////////////////////////
// CBurndownGraphComboBox window

class CBurndownGraphComboBox : public COwnerdrawComboBoxBase
{
public:
	CBurndownGraphComboBox();
	virtual ~CBurndownGraphComboBox();

	BOOL Initialise(const CGraphsMap& mapGraphs);

	void DDX(CDataExchange* pDX, BURNDOWN_GRAPH& nGraph);

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURNDOWNGRAPHCOMBOBOX_H__E072EBC4_11A8_4845_8899_F7958796AC12__INCLUDED_)
