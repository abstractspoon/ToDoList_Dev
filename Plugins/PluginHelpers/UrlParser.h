#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

class CUrlParser;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class UrlItem
			{
			public:
				UrlItem(int nStart, int nEnd, String^ sUrl);

				int StartPos, EndPos;
				String^ Url;
			};

			public ref class HtmlLink
			{
			public:
				HtmlLink(String^ sUrl, String^ sText);

				String^ Url;
				String^ Text;
			};

			public ref class UrlParser
			{
			public:
				UrlParser();
				virtual ~UrlParser();

				List<UrlItem^>^ ParseText(String^ sText);
				List<HtmlLink^>^ ExtractHtmlLinks(String^ sHtml);

				bool IsFileProtocol(String^ sUrl);
				int GetUrlCount(String^ sText);
				int MatchProtocol(String^ sUrl);
				bool ProtocolWantsNotification(int nProtocol);
				String^ GetUrlAsFile(String^ sUrl);
								
			protected:
				CUrlParser* m_parser;
			};

		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

