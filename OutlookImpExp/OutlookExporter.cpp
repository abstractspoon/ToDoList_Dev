// OutlookExporter.cpp: implementation of the COutlookExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutlookImpExp.h"
#include "OutlookExporter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutlookExporter::COutlookExporter()
{

}

COutlookExporter::~COutlookExporter()
{

}

void COutlookExporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

bool COutlookExporter::Export(const ITaskList* /*pSrcTaskFile*/, LPCTSTR /*szDestFilePath*/, BOOL /*bSilent*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	return false;
}
