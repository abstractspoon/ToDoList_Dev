// TaskFileHtmlExporter.cpp: implementation of the CTaskListHtmlExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TasklistHtmlExporter.h"
#include "tdlrecurringtaskedit.h"

#include "..\shared\xmlfile.h"
#include "..\shared\filemisc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\localizer.h"
#include "..\shared\webmisc.h"

#include "..\3rdparty\stdiofileex.h"

#include "..\Interfaces\Preferences.h"
#include "..\Interfaces\TasklistSchemaDef.h"

#include <locale.h>
#include <Shlwapi.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static LPCTSTR COMMENTS_DONECOLOR = _T("#808080");
static LPCTSTR DOCTYPE = _T("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n\n");
static LPCTSTR TAB = _T("&nbsp;&nbsp;&nbsp;&nbsp;");
static LPCTSTR SPACE = _T("&nbsp;");

CTaskListHtmlExporter::CTaskListHtmlExporter() 
	: 
	STRIKETHRUDONE(TRUE), 
	EXPORTSTYLE(TDCEF_HTMLWRAP),
	ROOT(TRUE), 
	COMMENTSPERCENTWIDTH(30)
{
	// override base class ENDL
	ENDL = "\n";
	LISTSEPARATOR = (Misc::GetListSeparator() + ' '); // add space to allow wrapping
}

CTaskListHtmlExporter::~CTaskListHtmlExporter()
{
	
}

IIMPORTEXPORT_RESULT CTaskListHtmlExporter::ExportOutput(LPCTSTR szDestFilePath, const CString& sOutput) const
{
	CString sHtmlOutput;
	
	sHtmlOutput += DOCTYPE;
	sHtmlOutput += _T("<html>\n<head>\n");
	sHtmlOutput += _T("<style type=\"text/css\">\n");
	sHtmlOutput += _T("@media print { thead {display: table-header-group;} }\n");
	sHtmlOutput += _T("table { border-collapse: collapse; }\n");
	sHtmlOutput += _T("</style>\n");
	sHtmlOutput += _T("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n");
	sHtmlOutput += CHARSET;
	sHtmlOutput += _T("</head>\n<body>\n");
	sHtmlOutput += sOutput;
	sHtmlOutput += _T("</body>\n</html>\n");

	return CTaskListExporterBase::ExportOutput(szDestFilePath, sHtmlOutput);
}

bool CTaskListHtmlExporter::InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, DWORD dwFlags, 
									   IPreferences* pPrefs, LPCTSTR szKey)
{
	// base consts
	if (!CTaskListExporterBase::InitConsts(pTasks, szDestFilePath, dwFlags, pPrefs, szKey))
		return false;

	// then ours
	szKey = _T("Preferences");
		
	CString sFont = pPrefs->GetProfileString(szKey, _T("HTMLFont"), _T("Verdana"));
	int nFontSize = pPrefs->GetProfileInt(szKey, _T("HtmlFontSize"), 2);
	
	DEFAULTFONT.Format(_T("<font face='%s' size='%d'>"), sFont, nFontSize);
	HTMLNOTES.Empty();
	
	if (pPrefs->GetProfileInt(szKey, _T("ExportSpaceForNotes"), FALSE))
	{
		int nLine = pPrefs->GetProfileInt(szKey, _T("LineSpaces"), 8);
		
		if (nLine > 0)
		{
			HTMLNOTES = "<pre>";
			
			while (nLine--)
				HTMLNOTES += ENDL;
			
			HTMLNOTES += "</pre>";
		}
	}
	
	STRIKETHRUDONE = pPrefs->GetProfileInt(szKey, _T("StrikethroughDone"), TRUE);
	EXPORTSTYLE = GetExportStyle(dwFlags);

	if (!PRINTING)
	{
		TASKLISTLINK.Format(_T("tdl://%s?"), pTasks->GetFileName(true));
		TASKLISTLINK.Replace('\\', '/');
	}

	INDENT.Empty();

	if (pPrefs->GetProfileInt(szKey, _T("UseSpaceIndents"), TRUE))
	{
		int nSpace = pPrefs->GetProfileInt(szKey, _T("TextIndent"), 2);

		while (nSpace--)
			INDENT += SPACE;
	}
	else
	{
		INDENT = TAB;
	}
	
	// charset
#ifdef _UNICODE
	CString sCS = "UTF-16"; 
#else
	CString sCS = pTasks->GetHtmlCharSet();
#endif
	
	if (!sCS.IsEmpty())
		CHARSET.Format(_T("<meta http-equiv=\"content-type\" content=\"text/html; charset=%s\">\n"), sCS);

	if (WantAttribute(TDCA_COMMENTS))
	{
		COMMENTSPERCENTWIDTH = 30; // minimum

		switch (ARRATTRIBUTES.GetSize())
		{
		case 2: // title & comments
			COMMENTSPERCENTWIDTH = 60;
			break;

		case 3:
		case 4:
		case 5:
		case 6:
			COMMENTSPERCENTWIDTH = 50;
			break;

		case 7:
		case 8:
		case 9:
		case 10:
			COMMENTSPERCENTWIDTH = 40;
			break;
		}
	}
	
	return true;
}

int CTaskListHtmlExporter::GetExportStyle(DWORD dwFlags)
{
	if (dwFlags & TDCEF_HTMLPARA)
		return TDCEF_HTMLPARA;

	if (dwFlags & TDCEF_HTMLTABLE)
		return TDCEF_HTMLTABLE;

	// all else
	return TDCEF_HTMLWRAP;
}

IIMPORTEXPORT_RESULT CTaskListHtmlExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, dwFlags, pPrefs, szKey);
}

IIMPORTEXPORT_RESULT CTaskListHtmlExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	return CTaskListExporterBase::Export(pSrcTaskFile, szDestFilePath, dwFlags, pPrefs, szKey);
}

CString CTaskListHtmlExporter::FormatTitle(const ITASKLISTBASE* pTasks) const
{
	CString sTitleBlock;
			
	// title and date
	CString sTitle = pTasks->GetReportTitle();
	CString sDate = pTasks->GetReportDate();
			
	if (!sTitle.IsEmpty())
	{
		CString sProjTitle;
		sProjTitle.Format(_T("<h2>%s</h2>%s"), sTitle, sDate);
		
		sTitleBlock += DEFAULTFONT;
		sTitleBlock += sProjTitle;
	}
	else if (!sDate.IsEmpty())
	{
		sTitleBlock += DEFAULTFONT;
		sTitleBlock += sDate;
	}

	sTitleBlock += _T("<p/>");
	
	if (IsTableStyle())
	{
		sTitleBlock += _T("<table border=\"1\">\n");
	}

	return sTitleBlock;
}

CString CTaskListHtmlExporter::FormatHeader(const ITASKLISTBASE* pTasks) const
{
	CString sHeader = CTaskListExporterBase::FormatHeader(pTasks);

	if (IsTableStyle() && !sHeader.IsEmpty())
	{
		sHeader = _T("<thead><tr>") + sHeader + _T("</tr></thead>");
	}

	return sHeader;
}

CString CTaskListHtmlExporter::FormatHeaderItem(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const
{
	CString sItem;

	if (IsTableStyle())
	{
		if (nAttrib == TDCA_COMMENTS)
			sItem.Format(_T("<th width=\"%d%%\">%s</th>"), COMMENTSPERCENTWIDTH, sAttribLabel);
		else
			sItem.Format(_T("<th>%s</th>"), sAttribLabel);
	}

	return sItem;
}

CString CTaskListHtmlExporter::ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const
{
	// snapshot depth so we only embolden top-level tasks
	ROOT = (nDepth <= 1);

	CString sTask = CTaskListExporterBase::ExportTask(pTasks, hTask, nDepth);

	if (IsTableStyle())
	{
		sTask = _T("<tr>") + sTask + _T("</tr>");
	}
	else // Paragraph and Wrapped
	{
		// indent
		if (pTasks->IsAttributeAvailable(TDCA_POSITION))
		{
			if (nDepth > 1)
			{
				sTask = _T("<blockquote>") + sTask;

				if (!pTasks->GetFirstTask(hTask)) // no sub-tasks
					sTask += _T("</blockquote>");
			}
		}
		else
		{
			if (!ROOT)
				sTask = _T("<li>") + sTask;
			
			// subtasks
			if (pTasks->GetFirstTask(hTask))
				sTask += _T("<ul>");
		}

		// wrapped tasks without subtasks
		if (IsWrappedStyle() && !pTasks->GetFirstTask(hTask))
			sTask += _T("<br>");

		sTask += _T("<br>");
	}

	return sTask;
}

CString CTaskListHtmlExporter::ExportSubtasks(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const
{
	CString sSubtasks = CTaskListExporterBase::ExportSubtasks(pTasks, hTask, nDepth);

	if (IsTableStyle())
	{
		if (nDepth == 0) // root
		{
			ASSERT(hTask == NULL);

			sSubtasks += _T("</table>\n");
		}
	}
	else // Paragraph and Wrapped
	{ 
		if (!sSubtasks.IsEmpty())
		{
			if (pTasks->IsAttributeAvailable(TDCA_POSITION))
				sSubtasks += _T("</blockquote>");
			else
				sSubtasks += _T("</ul>");
		}
	}

	return sSubtasks;
}

// virtual override
CString CTaskListHtmlExporter::FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel, const CString& sValue) const
{
	return FormatAttribute(nAttrib, sAttribLabel, sValue, TRUE);
}

// non-virtual helper
CString CTaskListHtmlExporter::FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel, const CString& sValue, BOOL bEncodeVal) const
{
	CString sFmtAttrib;
	CString sAttribVal(sValue);

	if (bEncodeVal)
		TXT2XML(sAttribVal);

	// Must process empty values in table format
	if (IsTableStyle() || !sAttribVal.IsEmpty())
	{
		switch (nAttrib)
		{
		case TDCA_POSITION:
			sFmtAttrib.Format(_T("%s%s"), sAttribVal, SPACE);
			break;

		case TDCA_TASKNAME:
			{
				// embolden top-level tasks only
				if (ROOT)
					sFmtAttrib += _T("<b>") + sAttribVal + _T("</b>");
				else
					sFmtAttrib += sAttribVal;

				if (IsParagraphStyle())
					sFmtAttrib += _T("<br>");
			}
			break;

		// all else
		default:
			if (IsWrappedStyle())
			{
				if (nAttrib == TDCA_COMMENTS)
				{
					sFmtAttrib = ENDL + sAttribVal;
				}
				else
				{
					if (sAttribLabel.IsEmpty())
						sFmtAttrib.Format(_T(" | %s"), sAttribVal);
					else
						sFmtAttrib.Format(_T(" | %s: %s"), sAttribLabel, sAttribVal);
				}
			}
			else if (IsParagraphStyle())
			{
				if (sAttribLabel.IsEmpty())
					sFmtAttrib = sAttribVal;
				else
					sFmtAttrib.Format(_T("%s: %s"), sAttribLabel, sAttribVal);

				if (nAttrib != TDCA_COMMENTS)
					sFmtAttrib += _T("<br>");
			}
			else // Wrapped
			{
				// special case: custom attrib
				if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
					sFmtAttrib = FormatTableCell(sAttribVal);
				else
					sFmtAttrib = sAttribVal;
			}
		}
	}

	return sFmtAttrib;
}

CString CTaskListHtmlExporter::FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth, 
											   TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const
{
	// base processing
	CString sItem = CTaskListExporterBase::FormatAttribute(pTasks, hTask, nDepth, nAttrib, sAttribLabel);

	// extra processing
	CString sTextColor = ((ITaskList*)pTasks)->GetTaskAttribute(hTask, TDL_TASKTEXTWEBCOLOR);
	CString sBackColor = ((ITaskList*)pTasks)->GetTaskAttribute(hTask, TDL_TASKBACKWEBCOLOR);
	BOOL bColor = TRUE;
	BOOL bStrikeThru = (STRIKETHRUDONE && pTasks->IsTaskDone(hTask));
	BOOL bBlockQuote = FALSE;
	
	switch (nAttrib)
	{
	case TDCA_POSITION:
		if (IsTableStyle())
		{
			// Indent subtasks in table view only
			while (--nDepth)
				sItem = (INDENT + sItem);
		}
		break;

	case TDCA_TASKNAME:
		if (IsTableStyle())
		{
			// Indent subtasks in table view only
			if (!pTasks->IsAttributeAvailable(TDCA_POSITION))
			{
				while (--nDepth)
					sItem = (INDENT + sItem);
			}
		}

		if (!PRINTING)
		{
			DWORD dwTaskID = pTasks->GetTaskID(hTask);

			CString sTaskLink;
			sTaskLink.Format(_T(" (<a href=\"%s%ld\">tdl://%ld</a>)"), TASKLISTLINK, dwTaskID, dwTaskID);

			sItem += sTaskLink;
		}
		break;

	case TDCA_PARENTID:
		// ignore if not set
		if (pTasks->GetTaskParentID(hTask) == 0)
			sItem.Empty();
		break;
		
	case TDCA_PRIORITY:
		{
			int nPriority = pTasks->GetTaskPriority(hTask, TRUE);
			CString sPriority;

			if (nPriority >= 0)
			{
				CString sPriorityCol = pTasks->GetTaskPriorityWebColor(hTask);
				sPriority.Format(_T("<font color='%s'>%d</font>"), sPriorityCol, nPriority);

				sItem = FormatAttribute(nAttrib, sAttribLabel, sPriority, FALSE); // FALSE = Don't encode
			}
		}
		break;
		
	case TDCA_FILELINK:
		if (!sItem.IsEmpty() && !PRINTING)
		{
			// do it over creating a link for each file ref
			CString sFileLinks, sTasklistFolder = FileMisc::GetFolderFromFilePath(TASKLISTPATH);
			int nNumFiles = pTasks->GetTaskFileLinkCount(hTask);

			for (int nFile = 0; nFile < nNumFiles; nFile++) 
			{ 
				CString sFilePath = pTasks->GetTaskFileLink(hTask, nFile), sFileName, sTarget;

				if (WebMisc::IsURL(sFilePath))
				{
					sFileName = sFilePath;

					if (!WebMisc::IsFileURI(sFilePath))
						sTarget = _T("target=\"_blank\"");
				}
				else
				{
					FileMisc::MakeFullPath(sFilePath, sTasklistFolder);
					sFileName = FileMisc::GetFileNameFromPath(sFilePath);

					// handle the absence of a filename
					if (sFileName.IsEmpty())
						sFileName = sFilePath;

					WebMisc::FormatFileURI(sFilePath, sFilePath);
				}

				if (!sFileLinks.IsEmpty())
					sFileLinks += LISTSEPARATOR;

				CString sFileLink;
				sFileLink.Format(_T("<a href=\"%s\" %s>%s</a>"), sFilePath, sTarget, sFileName);

				sFileLinks += sFileLink;
			} 

			sItem = FormatAttribute(nAttrib, sAttribLabel, sFileLinks, FALSE); // FALSE = Don't encode
		}
		break;
		
	case TDCA_DEPENDENCY:
		if (!sItem.IsEmpty() && !PRINTING)
		{
			// do it over creating a link for each dependency
			CString sDepends;
			int nNumDepends = pTasks->GetTaskDependencyCount(hTask);

			for (int nDepend = 0; nDepend < nNumDepends; nDepend++) 
			{ 
				CString sLinkText = pTasks->GetTaskDependency(hTask, nDepend);
				TDCDEPENDENCY depend(sLinkText);

				if (depend.IsLocal())
					depend.sTasklist = TASKLISTPATH;

				CString sDepend;
				sDepend.Format(_T("<a href=\"%s\">%s</a>"), depend.Format(_T(""), TRUE), sLinkText);

				if (!sDepends.IsEmpty())
					sDepends += LISTSEPARATOR;

				sDepends += sDepend;
			} 

			sItem = FormatAttribute(nAttrib, sAttribLabel, sDepends, FALSE); // FALSE = Don't encode
		}
		break;
		
	case TDCA_COMMENTS:
		if (pTasks->TaskHasAttribute(hTask, TDCA_HTMLCOMMENTS))
		{
			sItem = pTasks->GetTaskAttribute(hTask, TDCA_HTMLCOMMENTS);
			sItem.TrimRight();
			
			// note: we reset the font after the comments because the font
			// face and size may well have been changed
			if (!sItem.IsEmpty())
				sItem += DEFAULTFONT;
			
			bColor = FALSE;
			bStrikeThru = FALSE;
			bBlockQuote = !IsTableStyle();
		}
		else if (!sItem.IsEmpty())
		{
			// Note: Comments have already been HTML encoded

			// replace carriage returns with <br>
			sItem.Replace(ENDL, _T("<br>"));
			
			// replace tab characters with multiple &nbsp;
			sItem.Replace(_T("\t"), TAB);
			
			sTextColor = pTasks->IsTaskDone(hTask) ? COMMENTS_DONECOLOR : _T("#606060");
			sBackColor.Empty();
			bStrikeThru = FALSE;
			bBlockQuote = !IsTableStyle();
		}
		break;
	}
	
	if (!sItem.IsEmpty())
	{
		if (bStrikeThru)
		{
			CString sStrikeThru;
			sStrikeThru.Format(_T("<s>%s</s>"), sItem);
			sItem = sStrikeThru;
		}
		
		if (bColor)
		{
			CString sColored;

			if (sBackColor.IsEmpty())
				sColored.Format(_T("<span style=\"color:%s\">%s</span>"), sTextColor, sItem);
			else
				sColored.Format(_T("<span style=\"color:%s;background-color:%s;\">%s</span>"), sTextColor, sBackColor, sItem);

			sItem = sColored;
		}
		
		if (bBlockQuote)
		{
			CString sBlockQuote;
			sBlockQuote.Format(_T("<blockquote>%s</blockquote>"), sItem);
			sItem = sBlockQuote;
		}
	}

	// we've already handled custom attrib above
	if (IsTableStyle() && !TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
	{
		if (sItem.IsEmpty())
			sItem = SPACE;

		sItem = FormatTableCell(sItem);
	}

    return sItem;
}

CString CTaskListHtmlExporter::FormatTableCell(const CString& sValue)
{
	return (_T("<td valign=\"top\">") + sValue + _T("</td>"));
}
