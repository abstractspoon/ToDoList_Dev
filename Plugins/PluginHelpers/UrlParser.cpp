// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UrlParser.h"
#include "PluginHelpers.h"

#include <Shared\UrlParser.h>
#include <Shared\WebMisc.h>

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

HtmlLink::HtmlLink(String^ sUrl, String^ sText)
{
	Url = sUrl;
	Text = sText;
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
	List<UrlItem^>^ items = gcnew List<UrlItem^>();

	for (int nUrl = 0; nUrl < nNumUrls; nUrl++)
	{
		UrlItem^ item = gcnew UrlItem(aUrls[nUrl].cr.cpMin, 
									  aUrls[nUrl].cr.cpMax, 
									  gcnew String(aUrls[nUrl].sUrl));
		items->Add(item);
	}

	return items;
}

List<HtmlLink^>^ UrlParser::ExtractHtmlLinks(String^ sHtml)
{
	CStringArray aUrls, aText;

	MarshalledString msHtml(sHtml);
	CString html(msHtml);

	int nNumUrls = WebMisc::ExtractHtmlLinks(html, aUrls, aText);

	// Always returns a list
	List<HtmlLink^>^ items = gcnew List<HtmlLink^>();

	for (int nUrl = 0; nUrl < nNumUrls; nUrl++)
	{
		HtmlLink^ item = gcnew HtmlLink(gcnew String(aUrls[nUrl]), gcnew String(aText[nUrl]));
		items->Add(item);
	}

	return items;
}

bool UrlParser::IsFileProtocol(String^ sUrl)
{
	return (m_parser->IsFileProtocol(MS(sUrl)) != FALSE);
}

int UrlParser::GetUrlCount(String^ sText)
{
	CUrlArray aUrls;
	return m_parser->ParseText(MS(sText), aUrls);
}

int UrlParser::MatchProtocol(String^ sUrl)
{
	return m_parser->MatchProtocol(MS(sUrl));
}

bool UrlParser::ProtocolWantsNotification(int nProtocol)
{
	return (m_parser->ProtocolWantsNotification(nProtocol) != FALSE);
}

String^ UrlParser::GetUrlAsFile(String^ sUrl)
{
	if (!IsFileProtocol(sUrl))
		return nullptr;

	MarshalledString msUrl(sUrl);
	CString url(msUrl);

	return gcnew String(m_parser->GetUrlAsFile(url));
}

////////////////////////////////////////////////////////////////////////////////////////////////
