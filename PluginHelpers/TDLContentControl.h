#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "PluginHelpers.h"
#include "TDLPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace TDLPluginHelpers
{
	public ref class TDLContentControl
	{
		ref class TDLNotify
		{
		public:
			TDLNotify(IntPtr hwndParent);
			TDLNotify(IntPtr hwndParent, IntPtr hwndFrom);

			bool NotifyMod();

		private:
			HWND m_hwndParent;
			HWND m_hwndFrom;
		};
	};

	public interface class ITDLContentControl
	{
		// custom/binary data format
		cli::array<Byte>^ GetContent();
		bool SetContent(cli::array<Byte>^ content, bool bResetSelection);

		// text content if supported. return false if not supported
		String^ GetTextContent();
		bool SetTextContent(String^ content, bool bResetSelection);

		bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos);

		bool Undo();
		bool Redo();

		void SetUITheme(TDLTheme^ theme);
		void SetReadOnly(bool bReadOnly);

		void SavePreferences(TDLPreferences^ prefs, String^ key);
		void LoadPreferences(TDLPreferences^ prefs, String^ key, bool appOnly);

		//virtual ISpellCheck* GetSpellCheckInterface() = 0;
	};

}


