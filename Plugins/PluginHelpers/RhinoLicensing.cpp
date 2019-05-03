// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RhinoLicensing.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::IO;
using namespace System::Reflection;
using namespace System::Xml;
using namespace System::Globalization;
using namespace System::Security::Cryptography;
using namespace System::Security::Cryptography::Xml;
using namespace System::Collections::Generic;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

static LPCWSTR PUBLIC_KEY = L"<RSAKeyValue><Modulus>9twJpwt/Ofe58BOdK5Cb8XKGP5bvgxGh3IYkvCqvdzOCH3pi9BvOX+/fsRo/7HFbNmPr3Txu+hBl1JVH9ACXDxm20oKqgl6TzIk33iV6SrbuiZASi1OPAiTmsWBGKTIwrG9KiQ8JGmBotV/v2gRflqKELwiMUOO9W2DlgJ6szq0=</Modulus><Exponent>AQAB</Exponent></RSAKeyValue>";

////////////////////////////////////////////////////////////////////////////////////////////////

bool RhinoLicensing::CheckLicense(String^ typeId)
{
	String^ appFolder = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);
	String^ licenseFolder = Path::Combine(appFolder, "Resources\\Licenses");

	if (Directory::Exists(licenseFolder))
	{
		auto licenseFiles = Directory::EnumerateFiles(licenseFolder, typeId + ".xml", SearchOption::AllDirectories);
		auto attributes = gcnew Dictionary<String^, String^>();

		for each (String^ licenseFile in licenseFiles)
		{
			if (!CheckLicense(gcnew String(PUBLIC_KEY), licenseFile, attributes))
				continue;

			// Validate attributes
			String ^licId, ^expiryDate, ^licType, ^email, ^pluginName, ^pluginId;

			if (!attributes->TryGetValue("id", licId) ||
				!attributes->TryGetValue("expiration", expiryDate) ||
				!attributes->TryGetValue("type", licType) ||
				!attributes->TryGetValue("email", email) ||
				!attributes->TryGetValue("plugin_name", pluginName) ||
				!attributes->TryGetValue("plugin_id", pluginId))
			{
				continue;
			}

			if (!pluginId->Equals(typeId))
				continue;

			// TODO
			//auto expirationDate = DateTime::ParseExact(date->Value, "yyyy-MM-ddTHH:mm:ss.fffffff", CultureInfo::InvariantCulture);
			//auto licType = (/*LicenseType)Enum.Parse(typeof(LicenseType), */licenseType->Value);
		}
	}

	
	return false;
}

bool RhinoLicensing::CheckLicense(String^ publicKey, String^ licensePath, /*out*/ Dictionary<String^, String^>^ attributes)
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

	// Extract attributes
	auto license = doc->SelectSingleNode("/license");
	int att = license->Attributes->Count;
	
	while (att--)
	{
		auto attrib = license->Attributes[att];

		attributes[attrib->Name] = attrib->Value;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
