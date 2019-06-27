
#include "stdafx.h"
#include "OutlookUtil.h"
#include "DragDropUtil.h"
#include "PluginHelpers.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Microsoft::VisualStudio::OLE::Interop;
// using namespace Microsoft::Office::Interop::Outlook;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

#define CFSTR_RENPRIVATEMESSAGES  L"RenPrivateMessages" 
#define CFSTR_RENPRIVATESRCFOLDER L"RenPrivateSourceFolder"
#define CFSTR_FILEDESCRIPTORW     L"FileGroupDescriptorW"
#define CFSTR_FILECONTENTS        L"FileContents"

static const CLIPFORMAT CF_RENPRIVATEMESSAGES = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATEMESSAGES);
static const CLIPFORMAT CF_RENPRIVATESRCFOLDER = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATESRCFOLDER);
static const CLIPFORMAT CF_FILECONTENTS = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILECONTENTS);
static const CLIPFORMAT CF_FILEDESCRIPTOR = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);

////////////////////////////////////////////////////////////////////////////////////////////////

bool OutlookUtil::IsOutlookItem(Microsoft::VisualStudio::OLE::Interop::IDataObject^ obj)
{
	return (DragDropUtil::ObjectHasFormat(obj, CF_RENPRIVATEMESSAGES) &&
			DragDropUtil::ObjectHasFormat(obj, CF_RENPRIVATESRCFOLDER) &&
			DragDropUtil::ObjectHasFormat(obj, CF_FILECONTENTS) &&
			DragDropUtil::ObjectHasFormat(obj, CF_FILEDESCRIPTOR));
}

String^ OutlookUtil::FormatItemAsUrl(String^ title, String^ id, bool forceId)
{
	// TODO
	return String::Format(L"outlook://{0}", id);
}

/*
bool OutlookUtil::GetSelectedOutlookObjectTitle(String^% title)
{
	auto app = gcnew Application();

	if (app == nullptr)
		return false;

	auto selection = app->ActiveExplorer()->Selection;

	if ((selection == nullptr) || (selection->Count == 0))
		return false;

	auto item = selection[1];

	if (ISTYPE(item, Microsoft::Office::Interop::Outlook::MailItem))
	{
		title = ASTYPE(item, MailItem)->Subject;
		id = ASTYPE(item, MailItem)EntryID;
	}
	else if(ISTYPE(item, Microsoft::Office::Interop::Outlook::CalendarItem))
	{
		title = ASTYPE(item, MailItem)->Subject;
		id = ASTYPE(item, MailItem)EntryID;
	}
	else if(ISTYPE(item, Microsoft::Office::Interop::Outlook::NoteItem))
	{
		title = ASTYPE(item, MailItem)->Subject;
		id = ASTYPE(item, MailItem)EntryID;
	}
	else if(ISTYPE(item, Microsoft::Office::Interop::Outlook::TaskItem))
	{
		title = ASTYPE(item, MailItem)->Subject;
		id = ASTYPE(item, MailItem)EntryID;
	}
	else if(ISTYPE(item, Microsoft::Office::Interop::Outlook::JournalItem))
	{
		title = ASTYPE(item, MailItem)->Subject;
		id = ASTYPE(item, MailItem)EntryID;
	}
	else if (ISTYPE(item, Microsoft::Office::Interop::Outlook::ContactItem))
	{
		title = ASTYPE(item, MailItem)->Subject;
		id = ASTYPE(item, MailItem)EntryID;
	}


}
*/
