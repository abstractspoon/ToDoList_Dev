#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include <CommCtrl.h>

#include "PluginHelpers.h"
#include "TaskList.h"
#include "Preferences.h"
#include "UITheme.h"

#include "..\..\ToDoList_Dev\Interfaces\IUIExtension.h"

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class UIExtension
			{
			public: 
				enum class UpdateType
				{
					Unknown = -1,

					Edit,
					New,
					Delete,

					All = 0xffff
				};

				enum class ExpandTask
				{ 
					ExpandAllTasks,
					CollapseAllTasks,
					ExpandSelectedTasks,
					CollapseSelectedTasks,
				};

				enum class GetTask
				{ 
					GetNextTask,		
					GetNextTopLevelTask,
					GetPrevTask,		
					GetPrevTopLevelTask,
				};

				enum class SelectTask
				{ 
					SelectFirstTask,			
					SelectNextTask,				
					SelectNextTaskInclCurrent,	
					SelectPrevTask,				
					SelectLastTask,				
				};

				enum class HitResult
				{
					Nowhere,
					Tasklist,
					ColumnHeader,
					Task,
				};

				enum class TaskAttribute
				{
					Unknown = -1,

					Title = 0,
					DoneDate,
					DueDate,
					StartDate,
					Priority,
					Color,
					AllocTo,
					AllocBy,
					Status,
					Category,
					Percent,
					TimeEstimate,
					TimeSpent,
					FileReference,
					Comments,
					Flag,
					CreationDate,
					CreatedBy,
					Risk,			
					ExternalId,	
					Cost,			
					Dependency,	
					Recurrence,	
					Version,		
					Position,
					Id,
					LastModified,
					Icon,
					Tag,
					CustomAttribute,
					OffsetTask,

					// new values here ONLY

				};

				static TaskAttribute Map(IUI_ATTRIBUTE attrib);
				static Collections::Generic::HashSet<TaskAttribute>^ Map(const IUI_ATTRIBUTE* pAttrib, int numAttrib);
				static UpdateType Map(IUI_UPDATETYPE type);
				static IUI_HITTEST Map(HitResult test);
				static IUI_ATTRIBUTE Map(TaskAttribute attrib);

				ref class ParentNotify
				{
				public:
					ParentNotify(IntPtr hwndParent);

					bool NotifyMod(UIExtension::TaskAttribute nAttribute, DateTime value);
					bool NotifyMod(UIExtension::TaskAttribute nAttribute, double value);
					bool NotifyMod(UIExtension::TaskAttribute nAttribute, double time, Task::TimeUnits units);
					bool NotifyMod(UIExtension::TaskAttribute nAttribute, int value);
					bool NotifyMod(UIExtension::TaskAttribute nAttribute, bool value);
					bool NotifyMod(UIExtension::TaskAttribute nAttribute, String^ value);
					bool NotifyMod(String^ sCustAttribID, String^ value);

					bool NotifyMove(UInt32 taskID, UInt32 parentTaskID, UInt32 afterSiblingID);
					bool NotifyCopy(UInt32 taskID, UInt32 parentTaskID, UInt32 afterSiblingID);

					bool NotifySelChange(UInt32 taskID);
					bool NotifySelChange(cli::array<UInt32>^ pdwTaskIDs);

					bool NotifyEditTaskLabel();

				private:
					HWND m_hwndParent;

				private:
					bool DoNotify(const IUITASKMOD* pMod, int numMod);
					bool DoNotify(const IUITASKMOVE* pMove);
				};

				ref class TaskIcon
				{
				public:
					TaskIcon(IntPtr hwndParent);

					bool Get(UInt32 dwTaskID);
					bool Draw(System::Drawing::Graphics^ dc, Int32 x, Int32 y);
					
				private:
					HWND m_hwndParent;
					HIMAGELIST m_hilTaskImages;
					int m_iImage;
				};

				ref class SelectionRect
				{
				public:
					SelectionRect();

					bool Draw(IntPtr hwnd, System::Drawing::Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy);
					bool Draw(System::Drawing::Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy, bool focused);

				private:
					System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_visExplorerSelected;
					System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_visExplorerFocused;
				};

			};

			public interface class IUIExtension
			{
				bool SelectTask(UInt32 taskID);
				bool SelectTasks(cli::array<UInt32>^ taskIDs);

				void UpdateTasks(TaskList^ tasks, UIExtension::UpdateType update, Collections::Generic::HashSet<UIExtension::TaskAttribute>^ attribs);
				bool WantEditUpdate(UIExtension::TaskAttribute attribute);
				bool WantSortUpdate(UIExtension::TaskAttribute attribute);
				bool PrepareNewTask(Task^% task);

				bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos);

				bool GetLabelEditRect(Int32% left, Int32% top, Int32% right, Int32% bottom); // screen coordinates
				UIExtension::HitResult HitTest(Int32 xPos, Int32 yPos);

				void SetUITheme(UITheme^ theme);
				void SetReadOnly(bool bReadOnly);

				void SavePreferences(Preferences^ prefs, String^ key);
				void LoadPreferences(Preferences^ prefs, String^ key, bool appOnly);
			};
		}
	}
}

