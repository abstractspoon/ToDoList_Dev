// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UrlParser.h"
#include "PluginHelpers.h"

#include "..\..\Core\Shared\UrlParser.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

UrlItem::UrlItem(int nStart, int nEnd, String^ sUrl)
{
	StartPos = nStart;
	EndPos = nEnd;
	Url = sUrl;
}

////////////////////////////////////////////////////////////////////////////////////////////////

UrlParser::UrlParser() : m_parser(new CUrlParser)
{
	m_parser->AddProtocol(L"tdl://");
}

UrlParser::~UrlParser()
{
	delete m_parser;
}


List<UrlItem^>^ UrlParser::ParseText(String^ sText)
{
	CUrlArray aUrls;
	int nNumUrls = m_parser->ParseText(MS(sText), aUrls);

	// Always returns a list
	System::Collections::Generic::List<UrlItem^>^ items = gcnew System::Collections::Generic::List<UrlItem^>();

	for (int nUrl = 0; nUrl < nNumUrls; nUrl++)
	{
		UrlItem^ item = gcnew UrlItem(aUrls[nUrl].cr.cpMin, 
									  aUrls[nUrl].cr.cpMax, 
									  gcnew String(aUrls[nUrl].sUrl));

		items->Add(item);
	}

	return items;
}

bool UrlParser::HasProtocol(String^ sUrl)
{
	return (m_parser->MatchProtocol(MS(sUrl)) != -1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
