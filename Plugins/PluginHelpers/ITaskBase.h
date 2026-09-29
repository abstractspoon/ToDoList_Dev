#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::CompilerServices;

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
				virtual property bool HasIcon		{ bool get(); }
				virtual property String^ Position	{ String^ get(); }

				virtual property Drawing::Color TextColor { Drawing::Color get(); }
			};

			//////////////////////////////////////////////////////////////
			
			[ExtensionAttribute]
			public ref class ITaskBaseExt abstract sealed 
			{
			public:        
				[ExtensionAttribute]
				static bool IsTopLevel(ITaskBase^ task) 
				{
					return (task->Position->IndexOf('.') == -1);
				}
				
				[ExtensionAttribute]
				static int GetDepth(ITaskBase^ task) 
				{
					int depth = 0;

					for each (auto c in task->Position)
					{
						if (c == '.')
							depth++;
					}

					return depth;
				}
			};
		}
	}
}
