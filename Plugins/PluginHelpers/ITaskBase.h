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
			// Required interface for TaskComboBox and TaskListView
			// Specific attributes not required can be stubbed out
			public interface class ITaskBase
			{
			public:
				// Required for TaskComboBox and TaskListView
				virtual property UInt32 Id			{ UInt32 get(); }
				virtual property String^ Title		{ String^ get(); }
				virtual property bool HasIcon		{ bool get(); }
				virtual property String^ Position	{ String^ get(); }

				// Required by TaskListView
				virtual property bool IsLocked		{ bool get(); }
				virtual property bool IsParent		{ bool get(); }
				virtual property bool IsDone		{ bool get(); }

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
