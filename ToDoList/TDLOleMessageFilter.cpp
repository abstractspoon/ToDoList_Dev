// TDLOleMessageFilter.cpp: implementation of the CTDLOleMessageFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLOleMessageFilter.h"
#include <winbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLOleMessageFilter::CTDLOleMessageFilter()
{

}

CTDLOleMessageFilter::~CTDLOleMessageFilter()
{

}

int CTDLOleMessageFilter::OnBusyDialog(HTASK htaskBusy)
{
	//DWORD dwThreadID = (DWORD)htaskBusy;

	return COleMessageFilter::OnBusyDialog(htaskBusy);
}

int CTDLOleMessageFilter::OnNotRespondingDialog(HTASK htaskBusy)
{


	return COleMessageFilter::OnNotRespondingDialog(htaskBusy);
}
