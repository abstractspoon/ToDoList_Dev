// ImportExportMgr.cpp: implementation of the CImportExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImportExportMgr.h"
#include "filemisc.h"
#include "localizer.h"
#include "misc.h"

#include "..\Interfaces\ITaskList.h"
#include "..\Interfaces\IImportExport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const LPCTSTR IMPORTER_KEY = _T("Importers");
const LPCTSTR EXPORTER_KEY = _T("Exporters");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImportExportMgr::CImportExportMgr() : m_bInitialized(FALSE)
{

}

CImportExportMgr::~CImportExportMgr()
{
	Release();
}

void CImportExportMgr::Release()
{
	if (!m_bInitialized)
		return;

	// cleanup
	int nInterface = m_aImporters.GetSize();

	while (nInterface--)
		m_aImporters[nInterface]->Release();

	m_aImporters.RemoveAll();

	nInterface = m_aExporters.GetSize();

	while (nInterface--)
		m_aExporters[nInterface]->Release();

	m_aExporters.RemoveAll();

	m_bInitialized = FALSE;
}

void CImportExportMgr::Initialize() const
{
	if (m_bInitialized)
		return;

	// we need a non-const pointer to update the array
	CImportExportMgr* pMgr = const_cast<CImportExportMgr*>(this);
	
	pMgr->m_bInitialized = TRUE;

	// look at every dll from wherever we are now
	CFileFind ff;
    CString sSearchPath = FileMisc::GetAppFilePath(), sFolder, sDrive;

	FileMisc::SplitPath(sSearchPath, &sDrive, &sFolder);
	FileMisc::MakePath(sSearchPath, sDrive, sFolder, _T("*"), _T(".dll"));

	BOOL bContinue = ff.FindFile(sSearchPath);
	
	while (bContinue)
	{
		bContinue = ff.FindNextFile();
		
		if (!ff.IsDots() && !ff.IsDirectory())
		{
			CString sDllPath = ff.GetFilePath();

			try
			{
				if (IsImportExportDll(sDllPath))
				{
					IImportTasklist* pImporter = CreateImportInterface(sDllPath);
					
					if (pImporter)
					{
						pImporter->SetLocalizer(CLocalizer::GetLocalizer());
						pMgr->m_aImporters.Add(pImporter);
					}
					
					IExportTasklist* pExporter = CreateExportInterface(sDllPath);
					
					if (pExporter)
					{
						pExporter->SetLocalizer(CLocalizer::GetLocalizer());
						pMgr->m_aExporters.Add(pExporter);
					}
				}
			}
			catch (...)
			{
			}
		}
	}
}

void CImportExportMgr::UpdateLocalizer()
{
	if (!m_bInitialized)
		return;

	int nInterface = m_aImporters.GetSize();

	while (nInterface--)
		m_aImporters[nInterface]->SetLocalizer(CLocalizer::GetLocalizer());

	nInterface = m_aExporters.GetSize();

	while (nInterface--)
		m_aExporters[nInterface]->SetLocalizer(CLocalizer::GetLocalizer());
}

int CImportExportMgr::GetNumImporters() const
{
	Initialize(); // initialize on demand

	return m_aImporters.GetSize();
}

int CImportExportMgr::GetNumExporters() const
{
	Initialize(); // initialize on demand

	return m_aExporters.GetSize();
}

CString CImportExportMgr::GetImporterMenuText(int nImporter) const
{
	Initialize(); // initialize on demand

	CString sText;

	if (nImporter >= 0 && nImporter < m_aImporters.GetSize())
	{
		ASSERT (m_aImporters[nImporter] != NULL);
		sText = m_aImporters[nImporter]->GetMenuText();
	}

	return Misc::Trim(sText);
}

CString CImportExportMgr::GetImporterFileExtension(int nImporter) const
{
	Initialize(); // initialize on demand

	CString sExt;

	if (nImporter >= 0 && nImporter < m_aImporters.GetSize())
	{
		ASSERT (m_aImporters[nImporter] != NULL);
		sExt = m_aImporters[nImporter]->GetFileExtension();
	}
	
	return Misc::Trim(sExt);
}

BOOL CImportExportMgr::ImporterHasFileExtension(int nImporter) const
{
	return !GetImporterFileExtension(nImporter).IsEmpty();
}

CString CImportExportMgr::GetImporterFileFilter(int nImporter) const
{
	Initialize(); // initialize on demand

	CString sFilter;

	if (nImporter >= 0 && nImporter < m_aImporters.GetSize())
	{
		ASSERT (m_aImporters[nImporter] != NULL);
		sFilter = m_aImporters[nImporter]->GetFileFilter();
	}

	return Misc::Trim(sFilter);
}

HICON CImportExportMgr::GetImporterIcon(int nImporter) const
{
	Initialize(); // initialize on demand

	if (nImporter >= 0 && nImporter < m_aImporters.GetSize())
	{
		ASSERT (m_aImporters[nImporter] != NULL);
		return m_aImporters[nImporter]->GetIcon();
	}

	return NULL;
}

CString CImportExportMgr::GetExporterMenuText(int nExporter) const
{
	Initialize(); // initialize on demand

	CString sText;

	if (nExporter >= 0 && nExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nExporter] != NULL);
		sText = m_aExporters[nExporter]->GetMenuText();
	}

	return Misc::Trim(sText);
}

CString CImportExportMgr::GetExporterFileExtension(int nExporter) const
{
	Initialize(); // initialize on demand

	CString sExt;

	if (nExporter >= 0 && nExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nExporter] != NULL);
		sExt = m_aExporters[nExporter]->GetFileExtension();
	}

	return Misc::Trim(sExt);
}

BOOL CImportExportMgr::ExporterHasFileExtension(int nExporter) const
{
	return !GetExporterFileExtension(nExporter).IsEmpty();
}

CString CImportExportMgr::GetExporterFileFilter(int nExporter) const
{
	Initialize(); // initialize on demand

	CString sFilter;

	if (nExporter >= 0 && nExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nExporter] != NULL);
		sFilter = m_aExporters[nExporter]->GetFileFilter();
	}

	return Misc::Trim(sFilter);
}

HICON CImportExportMgr::GetExporterIcon(int nExporter) const
{
	Initialize(); // initialize on demand
	
	if (nExporter >= 0 && nExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nExporter] != NULL);
		return m_aExporters[nExporter]->GetIcon();
	}
	
	return NULL;
}

IIMPORT_RESULT CImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent, IPreferences* pPrefs) const
{
	Initialize(); // initialize on demand

	if ((nByImporter < 0) || (nByImporter >= m_aImporters.GetSize()))
		return IIR_OTHER;

	CWaitCursor cursor;

	ASSERT (m_aImporters[nByImporter] != NULL);
	return m_aImporters[nByImporter]->Import(szSrcFile, pDestTasks, bSilent, pPrefs, IMPORTER_KEY);
}

BOOL CImportExportMgr::ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const
{
	Initialize(); // initialize on demand

	if (nByExporter >= 0 && nByExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nByExporter] != NULL);
		return m_aExporters[nByExporter]->Export(pSrcTasks, szDestFile, bSilent, pPrefs, EXPORTER_KEY);
	}

	// else
	return FALSE;
}

BOOL CImportExportMgr::ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const
{
	Initialize(); // initialize on demand

	if (nByExporter >= 0 && nByExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nByExporter] != NULL);
		return m_aExporters[nByExporter]->Export(pSrcTasks, szDestFile, bSilent, pPrefs, EXPORTER_KEY);
	}

	// else
	return FALSE;
}

int CImportExportMgr::FindImporter(LPCTSTR szFilePath)
{
	Initialize(); // initialize on demand

	CString sExt;
	FileMisc::SplitPath(szFilePath, NULL, NULL, NULL, &sExt);

	if (sExt.IsEmpty()) // no extension
		return -1;

	else if (sExt[0] == '.')
		sExt = sExt.Mid(1);

	int nImporter = m_aImporters.GetSize();

	while (nImporter--)
	{
		if (GetImporterFileExtension(nImporter).CompareNoCase(sExt) == 0) // match
			break;
	}
	
	return nImporter; // match or -1
}
