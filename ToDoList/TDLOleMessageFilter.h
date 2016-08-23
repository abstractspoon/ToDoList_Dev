// TDLOleMessageFilter.h: interface for the CTDLOleMessageFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLOLEMESSAGEFILTER_H__49CDBE75_1BC1_4253_A2D1_F379C7779254__INCLUDED_)
#define AFX_TDLOLEMESSAGEFILTER_H__49CDBE75_1BC1_4253_A2D1_F379C7779254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxole.h>

class CTDLOleMessageFilter : public COleMessageFilter  
{
public:
	CTDLOleMessageFilter();
	virtual ~CTDLOleMessageFilter();

protected:
	virtual int OnBusyDialog(HTASK htaskBusy);
	virtual int OnNotRespondingDialog(HTASK htaskBusy);

};

#endif // !defined(AFX_TDLOLEMESSAGEFILTER_H__49CDBE75_1BC1_4253_A2D1_F379C7779254__INCLUDED_)
