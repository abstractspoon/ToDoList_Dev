// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "win32.h"

#include <Interfaces\IEnums.h>

#include <Shared\Misc.h>
#include <Shared\OSVersion.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::IO;
using namespace System::Reflection;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

class ModuleInitialiser
{
public:
	ModuleInitialiser()
	{
		try
		{
			Application::SetCompatibleTextRenderingDefault(false);
		}
		catch (...)
		{

		}
	}
};

static ModuleInitialiser* _init = new ModuleInitialiser();

////////////////////////////////////////////////////////////////////////////////////////////////

MarshalledString::MarshalledString(String^ str) : m_wszGlobal(NULL)
{
	if (str != nullptr)
		m_wszGlobal = (LPCWSTR)Marshal::StringToHGlobalUni(str).ToPointer();
}

MarshalledString::~MarshalledString()
{
	if (m_wszGlobal != NULL)
		Marshal::FreeHGlobal((IntPtr)(void*)m_wszGlobal);
}

MarshalledString::operator LPCWSTR() 
{ 
	return m_wszGlobal ? m_wszGlobal : L""; 
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool StringUtil::FindReplace(String^ source, String^ findText, String^ replaceText,
							bool matchWhole, bool matchCase, String^% result)
{
	MarshalledString msSearchIn(source), msSearchFor(findText), msReplaceWith(replaceText);
	CString sSearchIn(msSearchIn), sSearchFor(msSearchFor), sReplaceWith(msReplaceWith);

	if (0 == Misc::Replace(sSearchFor, sReplaceWith, sSearchIn, (BOOL)matchCase, (BOOL)matchWhole))
		return false;

	result = gcnew String(sSearchIn);
	return true;
}

bool StringUtil::Find(String^ source, String^ findText, bool matchWhole, bool matchCase)
{
	MarshalledString msSearchIn(source), msSearchFor(findText);
	CString sSearchIn(msSearchIn), sSearchFor(msSearchFor);

	return (-1 != Misc::Find(sSearchFor, sSearchIn, (BOOL)matchCase, (BOOL)matchWhole));
}

String^ StringUtil::Reverse(String^ source)
{
	if ((source == nullptr) || (source->Length < 2))
		return source;

	auto cArray = source->ToCharArray();
	Array::Reverse(cArray);

	return gcnew String(cArray);
}

int StringUtil::NaturalCompare(String^ str1, String^ str2)
{
	return Misc::NaturalCompare(MS(str1), MS(str2));
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::LogText(String^ text)
{
	if (s_sLogPath == nullptr)
	{
		String^ appFolder = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);
		s_sLogPath = Path::Combine(appFolder, "ToDoList.log");

		if (!File::Exists(s_sLogPath))
			s_sLogPath = String::Empty;
	}

	if (!String::IsNullOrEmpty(s_sLogPath))
		System::IO::File::AppendAllText(s_sLogPath, text + System::Environment::NewLine);
}

////////////////////////////////////////////////////////////////////////////////////////////////

OSVersion::OSV OSVersion::Ver::get()
{
	switch (COSVersion())
	{
	case OSV_XP:	return OSV::XP;
	case OSV_XPP:	return OSV::XP;
	case OSV_VISTA:	return OSV::Vista;
	case OSV_WIN7:	return OSV::Win7;
	case OSV_WIN8:	return OSV::Win8;
	case OSV_WIN81:	return OSV::Win8;
	case OSV_WIN10:	return OSV::Win10;
	}

	return OSV::Unknown;
}

////////////////////////////////////////////////////////////////////////////////////////////////
