#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace TDLPluginHelpers
{
	// converts System::String to LPCWSTR, and frees memory on exit
	class TDLMarshalledString
	{
	public:
		TDLMarshalledString(String^ str) : m_wszGlobal(NULL)
		{
			m_wszGlobal = (LPCWSTR)Marshal::StringToHGlobalUni(str).ToPointer();
		}

		~TDLMarshalledString()
		{
			Marshal::FreeHGlobal((IntPtr)(void*)m_wszGlobal);
		}

		operator LPCWSTR() { return m_wszGlobal; }

	private:
		LPCWSTR m_wszGlobal;
	};

	#define MS(str) TDLMarshalledString(str)

////////////////////////////////////////////////////////////////////////////////////////////////

};

