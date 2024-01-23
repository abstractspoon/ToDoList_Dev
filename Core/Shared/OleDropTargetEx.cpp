// OleDropTargetEx.cpp: implementation of the COleDropTargetEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleDropTargetEx.h"

#include "clipboard.h"
#include "filemisc.h"
#include "webmisc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleDropTargetEx::COleDropTargetEx() 
{

}

COleDropTargetEx::~COleDropTargetEx()
{

}

void COleDropTargetEx::ResetDrag(CWnd* pWnd)
{
}

int COleDropTargetEx::GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles)
{
	BOOL bUnused;
	return GetDropFilePaths(pObject, aFiles, bUnused);
}

int COleDropTargetEx::GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles, BOOL& bFromText)
{
	ASSERT(pObject);

	aFiles.RemoveAll();
	bFromText = FALSE;

	if (!FileMisc::GetDropFilePaths(pObject, aFiles) && CClipboard::HasText(pObject))
	{
		// look for files and URLs in text
		CString sText = CClipboard::GetText(pObject);

		if (FileMisc::IsPath(sText) || (WebMisc::IsURL(sText) && (sText.FindOneOf(_T("\t\r\n")) == -1)))
		{
			aFiles.Add(sText);
			bFromText = TRUE;
		}
	}

	return aFiles.GetSize();
}
