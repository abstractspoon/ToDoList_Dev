// ImportExportMgr.cpp: implementation of the CImportExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImportExportMgr.h"

#include "..\shared\filemisc.h"
#include "..\shared\localizer.h"
#include "..\shared\misc.h"
#include "..\shared\osversion.h"

#include "..\Interfaces\ITaskList.h"
#include "..\Interfaces\IImportExport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const CString IMPORTER_KEY(_T("Importers"));
const CString EXPORTER_KEY(_T("Exporters"));

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
					// Avoid instantiating Non-native (C#) modules on Linux
					if ((COSVersion() == OSV_LINUX) && !FileMisc::IsNativeModule(sDllPath))
						continue;

					IImportTasklist* pImporter = CreateImportInterface(sDllPath);
					
					if (pImporter)
						pMgr->AddImporter(pImporter);
					
					IExportTasklist* pExporter = CreateExportInterface(sDllPath);
					
					if (pExporter)
						pMgr->AddExporter(pExporter);
				}
			}
			catch (...)
			{
			}
		}
	}
}

BOOL CImportExportMgr::AddImporter(IImportTasklist* pImporter, int nPos)
{
	if (!pImporter || (FindImporterByType(pImporter->GetTypeID()) != -1))
	{
		ASSERT(0);
		return FALSE;
	}

	pImporter->SetLocalizer(CLocalizer::GetLocalizer());

	if (nPos < 0 || nPos >= m_aImporters.GetSize())
		m_aImporters.Add(pImporter);
	else
		m_aImporters.InsertAt(nPos, pImporter);

	return TRUE;
}

BOOL CImportExportMgr::AddExporter(IExportTasklist* pExporter, int nPos)
{
	if (!pExporter || (FindExporterByType(pExporter->GetTypeID()) != -1))
	{
		ASSERT(0);
		return FALSE;
	}

	pExporter->SetLocalizer(CLocalizer::GetLocalizer());

	if (nPos < 0 || nPos >= m_aExporters.GetSize())
		m_aExporters.Add(pExporter);
	else
		m_aExporters.InsertAt(nPos, pExporter);

	return TRUE;
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

CString CImportExportMgr::GetImporterFileExtension(int nImporter, BOOL bWithDot) const
{
	Initialize(); // initialize on demand

	CString sExt;

	if (nImporter >= 0 && nImporter < m_aImporters.GetSize())
	{
		ASSERT (m_aImporters[nImporter] != NULL);
		sExt = m_aImporters[nImporter]->GetFileExtension();
	}

	return FileMisc::FormatExtension(sExt, bWithDot);
}

BOOL CImportExportMgr::ImporterHasFileExtension(int nImporter) const
{
	return !GetImporterFileExtension(nImporter, TRUE).IsEmpty();
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

CString CImportExportMgr::GetImporterTypeID(int nImporter) const
{
	Initialize(); // initialize on demand

	CString sFilter;

	if (nImporter >= 0 && nImporter < m_aImporters.GetSize())
	{
		ASSERT (m_aImporters[nImporter] != NULL);
		sFilter = m_aImporters[nImporter]->GetTypeID();
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

CString CImportExportMgr::GetExporterFileExtension(int nExporter, BOOL bWithDot) const
{
	Initialize(); // initialize on demand

	CString sExt;

	if (nExporter >= 0 && nExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nExporter] != NULL);
		sExt = m_aExporters[nExporter]->GetFileExtension();
	}

	return FileMisc::FormatExtension(sExt, bWithDot);
}

BOOL CImportExportMgr::ExporterHasFileExtension(int nExporter, LPCTSTR szExt) const
{
	if (Misc::IsEmpty(szExt))
		return !GetExporterFileExtension(nExporter, FALSE).IsEmpty();

	CString sExt = GetExporterFileExtension(nExporter, (Misc::First(szExt) == '.'));

	return (sExt.CompareNoCase(szExt) == 0);
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

CString CImportExportMgr::GetExporterTypeID(int nExporter) const
{
	Initialize(); // initialize on demand

	if (nExporter >= 0 && nExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nExporter] != NULL);
		return m_aExporters[nExporter]->GetTypeID();
	}

	return _T("");
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

IIMPORTEXPORT_RESULT CImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent, IPreferences* pPrefs) const
{
	Initialize(); // initialize on demand

	if ((nByImporter < 0) || (nByImporter >= m_aImporters.GetSize()))
		return IIER_OTHER;

	CWaitCursor cursor;

	ASSERT (m_aImporters[nByImporter] != NULL);
	return m_aImporters[nByImporter]->Import(szSrcFile, pDestTasks, (bSilent != FALSE), pPrefs, GetImporterPreferenceKey(nByImporter));
}

IIMPORTEXPORT_RESULT CImportExportMgr::ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const
{
	Initialize(); // initialize on demand

	if (nByExporter >= 0 && nByExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nByExporter] != NULL);
		return m_aExporters[nByExporter]->Export(pSrcTasks, szDestFile, (bSilent != FALSE), pPrefs, GetExporterPreferenceKey(nByExporter));
	}

	// else
	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CImportExportMgr::ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const
{
	Initialize(); // initialize on demand

	if (nByExporter >= 0 && nByExporter < m_aExporters.GetSize())
	{
		ASSERT (m_aExporters[nByExporter] != NULL);
		return m_aExporters[nByExporter]->Export(pSrcTasks, szDestFile, (bSilent != FALSE), pPrefs, GetExporterPreferenceKey(nByExporter));
	}

	// else
	return IIER_BADFORMAT;
}

CString CImportExportMgr::GetExporterPreferenceKey(int nExporter) const
{
	if (nExporter >= 0 && nExporter < m_aExporters.GetSize())
	{
		ASSERT(m_aExporters[nExporter] != NULL);
		return (EXPORTER_KEY + '\\' + m_aExporters[nExporter]->GetTypeID());
	}
}

CString CImportExportMgr::GetImporterPreferenceKey(int nImporter) const
{
	if (nImporter >= 0 && nImporter < m_aImporters.GetSize())
	{
		ASSERT(m_aImporters[nImporter] != NULL);
		return (IMPORTER_KEY + '\\' + m_aImporters[nImporter]->GetTypeID());
	}
}

int CImportExportMgr::FindImporterByPath(LPCTSTR szFilePath) const
{
	CString sExt = FileMisc::GetExtension(szFilePath, FALSE);

	if (sExt.IsEmpty())
		return -1;

	Initialize(); // initialize on demand

	int nImporter = m_aImporters.GetSize();

	while (nImporter--)
	{
		if (GetImporterFileExtension(nImporter, FALSE).CompareNoCase(sExt) == 0) // match
			break;
	}
	
	return nImporter; // match or -1
}

int CImportExportMgr::FindExporterByPath(LPCTSTR szFilePath) const
{
	CString sExt = FileMisc::GetExtension(szFilePath, FALSE);

	if (sExt.IsEmpty())
		return -1;

	Initialize(); // initialize on demand

	int nExporter = m_aExporters.GetSize();

	while (nExporter--)
	{
		if (GetExporterFileExtension(nExporter, FALSE).CompareNoCase(sExt) == 0) // match
			break;
	}
	
	return nExporter; // match or -1
}

int CImportExportMgr::FindImporterByType(LPCTSTR szTypeID) const
{
	if (Misc::IsEmpty(szTypeID))
		return -1;

	Initialize(); // initialize on demand

	int nImporter = m_aImporters.GetSize();

	while (nImporter--)
	{
		if (GetImporterTypeID(nImporter).CompareNoCase(szTypeID) == 0) // match
			break;
	}
	
	return nImporter; // match or -1
}

int CImportExportMgr::FindExporterByType(LPCTSTR szTypeID) const
{
	if (Misc::IsEmpty(szTypeID))
		return -1;

	Initialize(); // initialize on demand

	int nExporter = m_aExporters.GetSize();

	while (nExporter--)
	{
		if (GetExporterTypeID(nExporter).CompareNoCase(szTypeID) == 0) // match
			break;
	}
	
	return nExporter; // match or -1
}
