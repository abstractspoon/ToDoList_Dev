#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public interface class ITaskBase
			{
			public:
				virtual property UInt32 Id			{ UInt32 get(); }
				virtual property String^ Title		{ String^ get(); }
				virtual property String^ Position	{ String^ get(); }
				virtual property int Depth			{ int get(); }
				virtual property bool HasIcon		{ bool get(); }
			};
		}
	}
}
