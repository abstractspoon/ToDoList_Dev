#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "PluginHelpers.h"
#include "Preferences.h"
#include "UITheme.h"
#include "Tasklist.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{

			public ref class NeedLinkTooltipEventArgs : EventArgs
			{
			public:
				NeedLinkTooltipEventArgs(String^ href) { linkUri = href; }

				String^ linkUri; // IN
				String^ tooltip; // OUT
			};

			public delegate void NeedLinkTooltipEventHandler(Object^ sender, NeedLinkTooltipEventArgs^ args);

			// -----------------------------------------------------------

			public ref class NeedAttributeValuesEventArgs : EventArgs
			{
			public:
				NeedAttributeValuesEventArgs(Task::Attribute a) { attrib = a; }

				Task::Attribute attrib; // IN
				List<String^>^ values;  // OUT
			};

			public delegate void NeedAttributeValuesEventHandler(Object^ sender, NeedAttributeValuesEventArgs^ args);

			// -----------------------------------------------------------

			public ref class ContentControlWnd
			{
			public:
				ref class ParentNotify
				{
				public:
					ParentNotify(IntPtr hwndParent);
					ParentNotify(IntPtr hwndParent, IntPtr hwndFrom);

					bool NotifyChange();
					bool NotifyKillFocus();
					bool NotifyFailedLink(String^ sFailedLink);
					bool NotifyTaskLink(String^ sTaskLink);
					bool NotifyWantSpellcheck();
					
					String^ GetLinkTooltip(String^ sLink);
					List<String^>^ GetAttributeValues(Task::Attribute attrib);

					//bool GetClipboard();
					bool HasClipboard();
					bool DoHelp(String^ sHelpKey);

				private:
					HWND m_hwndParent;
					HWND m_hwndFrom;

				private:
					HWND GetFrom();
					String^ ValidateLink(String^ sLink);

					bool NotifySimple(UINT nMsg);
				};

				static bool GoToLink(String^ sLink, IntPtr hwndParent, IntPtr hwndFrom);
				static String^ HandleNeedLinkTooltip(String^ sLink, IntPtr hwndParent, IntPtr hwndFrom);
				static List<String^>^ HandleNeedAttributeList(Task::Attribute attrib, IntPtr hwndParent, IntPtr hwndFrom);
			};

			// -----------------------------------------------------------

			public interface class IContentControlWnd
			{
				// custom/binary data format
				cli::array<Byte>^ GetContent();
				bool SetContent(cli::array<Byte>^ content, bool bResetSelection);

				// text content if supported. return false if not supported
				String^ GetTextContent();
				bool SetTextContent(String^ content, bool bResetSelection);
				bool InsertTextContent(String^ text, bool bInsertAtEnd);

				bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos);

				bool Undo();
				bool Redo();

				void SetUITheme(UITheme^ theme);
				void SetReadOnly(bool bReadOnly);

				void SavePreferences(Preferences^ prefs, String^ key);
				void LoadPreferences(Preferences^ prefs, String^ key, bool appOnly);

				//virtual ISpellCheck* GetSpellCheckInterface() = 0;
			};
		}
	}
}

