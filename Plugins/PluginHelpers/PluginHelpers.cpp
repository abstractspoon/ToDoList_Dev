// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"

#include <Interfaces\IEnums.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

class ModuleInitialiser
{
public:
	ModuleInitialiser()
	{
		try
		{
			System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
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

void DialogUtils::SetFont(System::Windows::Forms::Control^ ctrl, System::Drawing::Font^ font)
{
	ctrl->Font = font;

	SetFont(ctrl->Controls, font);
}

void DialogUtils::SetFont(System::Windows::Forms::Control::ControlCollection^ ctrls, System::Drawing::Font^ font)
{
	int nCtrl = ctrls->Count;

	while (nCtrl--)
		SetFont(ctrls[nCtrl], font); // RECURSIVE CALL
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::LogText(String^ text)
{
	String^ appFolder = System::IO::Path::GetDirectoryName(System::Reflection::Assembly::GetExecutingAssembly()->Location);
	String^ logPath = System::IO::Path::Combine(appFolder, "ToDoList.log");

	if (System::IO::File::Exists(logPath))
	{
		System::IO::File::AppendAllText(logPath, text + System::Environment::NewLine);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
