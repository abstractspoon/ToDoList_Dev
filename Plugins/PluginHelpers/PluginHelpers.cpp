// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"

#include <Interfaces\IEnums.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::IO;
using namespace System::Xml;
using namespace System::Globalization;
using namespace System::Security::Cryptography;
using namespace System::Security::Cryptography::Xml;
using namespace System::Collections::Generic;

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

	auto licenseText = File::ReadAllText(licensePath);

	auto doc = gcnew XmlDocument();
	doc->LoadXml(licenseText);

	auto rsa = gcnew RSACryptoServiceProvider();
	rsa->FromXmlString(publicKey);

	auto nsMgr = gcnew XmlNamespaceManager(doc->NameTable);
	nsMgr->AddNamespace("sig", "http://www.w3.org/2000/09/xmldsig#");

	auto signedXml = gcnew SignedXml(doc);
	auto sig = (XmlElement^)doc->SelectSingleNode("//sig:Signature", nsMgr);

	if (sig == nullptr)
	{
		//Log.WarnFormat("Could not find this signature node on license:\r\n{0}", License);
		return false;
	}
	signedXml->LoadXml(sig);

	if (!signedXml->CheckSignature(rsa))
		return false;

	if (doc->FirstChild == nullptr)
	{
		//Log.WarnFormat("Could not find first child of:\r\n{0}", License);
		return false;
	}

	auto id = doc->SelectSingleNode("/license/@id");
	if (id == nullptr)
	{
		//Log.WarnFormat("Could not find id attribute in license:\r\n{0}", License);
		return false;
	}

	auto userId = gcnew Guid(id->Value);

	auto date = doc->SelectSingleNode("/license/@expiration");
	if (date == nullptr)
	{
		//Log.WarnFormat("Could not find expiration in license:\r\n{0}", License);
		return false;
	}

	auto expirationDate = DateTime::ParseExact(date->Value, "yyyy-MM-ddTHH:mm:ss.fffffff", CultureInfo::InvariantCulture);

	auto licenseType = doc->SelectSingleNode("/license/@type");
	if (licenseType == nullptr)
	{
		//Log.WarnFormat("Could not find license type in {0}", licenseType);
		return false;
	}

	auto licType = (/*LicenseType)Enum.Parse(typeof(LicenseType), */licenseType->Value);

	auto name = doc->SelectSingleNode("/license/name/text()");
	if (name == nullptr)
	{
		//Log.WarnFormat("Could not find licensee's name in license:\r\n{0}", License);
		return false;
	}

	//Name = name.Value;

	auto license = doc->SelectSingleNode("/license");
	int att = license->Attributes->Count;

	auto attributes = gcnew Dictionary<String^, String^>();

	while (att--)
	{
		auto attrib = license->Attributes[att];

		if (attrib->Name == "type" || attrib->Name == "expiration" || attrib->Name == "id")
			continue;

		attributes[attrib->Name] = attrib->Value;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
