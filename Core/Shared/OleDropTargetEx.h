// OleDropTargetEx.h: interface for the COleDropTargetEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLEDROPTARGETEX_H__430ACDA9_05BE_46C3_ADC1_D8AEC5C5EE41__INCLUDED_)
#define AFX_OLEDROPTARGETEX_H__430ACDA9_05BE_46C3_ADC1_D8AEC5C5EE41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxole.h>

class COleDropTargetEx : public COleDropTarget  
{
public:
	COleDropTargetEx();
	virtual ~COleDropTargetEx();

	static int GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles);
	static int GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles, BOOL& bFromText);

private:

protected:
	virtual void ResetDrag(CWnd* pWnd);

};

#endif // !defined(AFX_OLEDROPTARGETEX_H__430ACDA9_05BE_46C3_ADC1_D8AEC5C5EE41__INCLUDED_)
