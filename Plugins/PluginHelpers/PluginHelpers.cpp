// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "win32.h"

#include <Interfaces\IEnums.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

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
	m_wszGlobal = (LPCWSTR)Marshal::StringToHGlobalUni(str).ToPointer();
}

MarshalledString::~MarshalledString()
{
	Marshal::FreeHGlobal((IntPtr)(void*)m_wszGlobal);
}

MarshalledString::operator LPCWSTR() 
{ 
	return m_wszGlobal; 
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogUtils::SetFont(Control^ ctrl, Font^ font)
{
	ctrl->Font = font;

	SetFont(ctrl->Controls, font);
}

void DialogUtils::SetFont(Control::ControlCollection^ ctrls, Font^ font)
{
	int nCtrl = ctrls->Count;

	while (nCtrl--)
		SetFont(ctrls[nCtrl], font); // RECURSIVE CALL
}

Control^ DialogUtils::Find(Control^ parent, String^ childName, bool recursive)
{
	auto ctrls = parent->Controls->Find(childName, recursive);

	return ((ctrls != nullptr) ? ctrls[0] : nullptr);
}

bool DialogUtils::SetEditCue(Control^ ctrl, String^ sCueText)
{
	if ((ctrl != nullptr) && (sCueText != nullptr))
		return Win32::SetEditCue(ctrl->Handle, sCueText);

	// else
	return false;
}

bool DialogUtils::SetEditCue(Control^ parent, String^ childName, String^ sCueText, bool recursiveSearch)
{
	return SetEditCue(Find(parent, childName, recursiveSearch), sCueText);
}

void DialogUtils::RecalcDropWidth(ComboBox^ combo)
{
	int maxWidth = 0;
	int nItem = combo->Items->Count;

	while (nItem--)
	{
		auto item = ASTYPE(combo->Items[nItem], String);

		if (item == nullptr)
			return;

		int itemWidth = TextRenderer::MeasureText(item, combo->Font).Width;
		maxWidth = Math::Max(itemWidth, maxWidth);
	}

	combo->DropDownWidth = (maxWidth + SystemInformation::VerticalScrollBarWidth);
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
