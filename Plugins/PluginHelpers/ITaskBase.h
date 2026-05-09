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
			// Required interface for TaskComboBox and TaskListView
			// Specific attributes not required can be stubbed out
			public interface class ITaskBase
			{
			public:
				// Required for TaskComboBox and TaskListView
				virtual property UInt32 Id			{ UInt32 get(); }
				virtual property String^ Title		{ String^ get(); }
				virtual property bool HasIcon		{ bool get(); }

				// Required by TaskComboBox
				virtual property String^ Position	{ String^ get(); }

				// Required by TaskListView
				virtual property bool IsLocked		{ bool get(); }
				virtual property bool IsParent		{ bool get(); }
				virtual property bool IsDone		{ bool get(); }

				virtual property Drawing::Color TextColor { Drawing::Color get(); }
			};
		}
	}
}
