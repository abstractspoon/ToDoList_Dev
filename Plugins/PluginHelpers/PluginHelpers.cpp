// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"

#include <Interfaces\IEnums.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

static LPCWSTR PUBLIC_KEY = L"<RSAKeyValue><Modulus>9twJpwt/Ofe58BOdK5Cb8XKGP5bvgxGh3IYkvCqvdzOCH3pi9BvOX+/fsRo/7HFbNmPr3Txu+hBl1JVH9ACXDxm20oKqgl6TzIk33iV6SrbuiZASi1OPAiTmsWBGKTIwrG9KiQ8JGmBotV/v2gRflqKELwiMUOO9W2DlgJ6szq0=</Modulus><Exponent>AQAB</Exponent></RSAKeyValue>";

////////////////////////////////////////////////////////////////////////////////////////////////

class ModuleInitialiser
{
public:
	ModuleInitialiser()
	{
		System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
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

bool Licensing::CheckLicense(String^ typeId)
{
	String^ appFolder = System::IO::Path::GetDirectoryName(System::Reflection::Assembly::GetExecutingAssembly()->Location);
	String^ licenseFolder = System::IO::Path::Combine(appFolder, "Resources\\Licenses");

	String^ licensePath = System::IO::Path::Combine(licenseFolder, typeId + ".xml");

	return CheckLicense(gcnew String(PUBLIC_KEY), licensePath);
}

bool Licensing::CheckLicense(String^ publicKey, String^ licensePath)
{
	if (String::IsNullOrEmpty(publicKey) || String::IsNullOrEmpty(licensePath) || !System::IO::File::Exists(licensePath))
		return false;

	// TODO



	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
