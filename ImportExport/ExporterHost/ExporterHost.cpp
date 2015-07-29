// ExporterHost.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\Interfaces\iimportexport.h"


int _tmain(int argc, _TCHAR* argv[])
{
   IExportTasklist* pExp = CreateExportInterface(_T(".\\ExporterBridge.dll"));

   if (pExp)
   {
      ITaskList* pTL = NULL;

      pExp->Export(pTL, _T("export_path.txt"), false, NULL, NULL);
   }


}

