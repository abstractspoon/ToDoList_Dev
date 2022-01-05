#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include <CommCtrl.h>

#include "PluginHelpers.h"
#include "TaskList.h"
#include "Preferences.h"
#include "UITheme.h"

#include <Interfaces\IUIExtension.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

enum GM_ITEMSTATE;

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
				// -----------------------------------------------

				enum class UpdateType
				{
					Unknown = -1,

					Edit,
					New,
					Delete,

					All = 0xffff
				};

				// -----------------------------------------------

				enum class ExpandTask
				{ 
					ExpandAllTasks,
					CollapseAllTasks,
					ExpandSelectedTasks,
					CollapseSelectedTasks,
				};

				// -----------------------------------------------

				enum class GetTask
				{ 
					GetNextTask,		
					GetNextVisibleTask,		
					GetNextTopLevelTask,
					GetPrevTask,		
					GetPrevVisibleTask,
					GetPrevTopLevelTask,
				};

				// -----------------------------------------------

				enum class SelectTask
				{ 
					SelectFirstTask,			
					SelectNextTask,				
					SelectNextTaskInclCurrent,	
					SelectPrevTask,				
					SelectLastTask,				
				};

				// -----------------------------------------------

				enum class HitResult
				{
					Nowhere,
					Tasklist,
					ColumnHeader,
					Task,
				};

				// -----------------------------------------------

				enum class AppCursorType
				{
					LockedTask,
					NoDrag,
				};

				// -----------------------------------------------

				static UpdateType Map(IUI_UPDATETYPE type);
				static IUI_HITTEST Map(HitResult test);

				static bool Map(IUI_APPCOMMAND nCmd, GetTask% getTask);
				static bool Map(IUI_APPCOMMAND nCmd, SelectTask% selectTask);

				static Windows::Forms::Cursor^ AppCursor(AppCursorType cursor);
				static Windows::Forms::Cursor^ HandCursor();

				static bool SaveImageToFile(Drawing::Bitmap^ image, String^ filepath);

				// -----------------------------------------------

				ref class ParentNotify
				{
				public:
					ParentNotify(IntPtr hwndParent);

					bool AddMod(Task::Attribute nAttribute, DateTime value);
					bool AddMod(Task::Attribute nAttribute, double value);
					bool AddMod(Task::Attribute nAttribute, double time, Task::TimeUnits units);
					bool AddMod(Task::Attribute nAttribute, int value);
					bool AddMod(Task::Attribute nAttribute, bool value);
					bool AddMod(Task::Attribute nAttribute, String^ value);
					bool AddMod(String^ sCustAttribID, String^ value);

					bool NotifyMod();
					bool NotifyMod(Task::Attribute nAttribute, DateTime value);
					bool NotifyMod(Task::Attribute nAttribute, double value);
					bool NotifyMod(Task::Attribute nAttribute, double time, Task::TimeUnits units);
					bool NotifyMod(Task::Attribute nAttribute, int value);
					bool NotifyMod(Task::Attribute nAttribute, bool value);
					bool NotifyMod(Task::Attribute nAttribute, String^ value);
					bool NotifyMod(String^ sCustAttribID, String^ value);

					bool NotifyMove(UInt32 taskID, UInt32 parentTaskID, UInt32 afterSiblingID);
					bool NotifyCopy(UInt32 taskID, UInt32 parentTaskID, UInt32 afterSiblingID);

					bool NotifySelChange(UInt32 taskID);
					bool NotifySelChange(cli::array<UInt32>^ pdwTaskIDs);

					bool NotifyEditLabel();
					bool NotifyEditIcon();

					bool NotifyDoHelp(String^ helpID);

				private:
					// Managed version of IUITASKMOD
					// Note: C++/CLI does not support unions
					ref class IUITaskMod
					{
					public:
						IUITaskMod(Task::Attribute attrib, DateTime value);
						IUITaskMod(Task::Attribute attrib, double value);
						IUITaskMod(Task::Attribute attrib, double time, Task::TimeUnits units);
						IUITaskMod(Task::Attribute attrib, int value);
						IUITaskMod(Task::Attribute attrib, bool value);
						IUITaskMod(Task::Attribute attrib, String^ value);

						IUITaskMod(String^ customAttribId, String^ value);

						bool CopyTo(IUITASKMOD& mod);

					public:
						Task::Attribute nAttrib;
						UInt32 dwSelectedTaskID;		
						String^ szCustomAttribID;

						//union
						//{
							int nValue;
							double dValue;
							String^ szValue;
							DateTime tValue;
							bool bValue;
							UInt32 crValue;
						//};

						//union
						//{
							Task::TimeUnits nTimeUnits;	
						//};

					private:
						enum class DataType
						{
							Double,
							Date,
							Time,
							Integer,
							Bool,
							Text,
							Color,
							Custom,
						};

						DataType dataType;
					};

				private:
					HWND m_hwndParent;
					List<IUITaskMod^>^ m_TaskMods;

				private:
					void ClearMods();
					bool NotifyMod(bool bClearModsAlways);
					bool DoNotify(const IUITASKMOD* pMod, int numMod);
					bool DoNotify(const IUITASKMOVE* pMove);
				};

				// -----------------------------------------------

				ref class TaskIcon
				{
				public:
					TaskIcon(IntPtr hwndParent);

					bool Get(UInt32 dwTaskID);
					bool Draw(Drawing::Graphics^ dc, Int32 x, Int32 y);
					
				private:
					HWND m_hwndParent;
					HIMAGELIST m_hilTaskImages;
					int m_iImage;
				};

				// -----------------------------------------------

				ref class ShortcutOverlay
				{
				public:
					static bool Draw(Drawing::Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy);
				};

				// -----------------------------------------------

				ref class SelectionRect
				{
				public:
					enum class Style
					{
						Selected,
						SelectedNotFocused,
						DropHighlighted,
					};

				public:
					static bool Draw(IntPtr hwnd, Drawing::Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy, bool transparent);
					static bool Draw(IntPtr hwnd, Drawing::Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy, Style style, bool transparent);

					static Drawing::Color GetColor(Style style);
					static GM_ITEMSTATE Map(Style style);
				};

				// -----------------------------------------------

				ref class TaskRecurrences
				{
				public:
					TaskRecurrences(IntPtr hwndParent);

					List<Tuple<DateTime, DateTime>^>^ Get(UInt32 dwTaskID, DateTime^ rangeStart, DateTime^ rangeEnd);

				private:
					HWND m_hwndParent;
				};

				// -----------------------------------------------

				ref class TaskDependency
				{
				public:
					static void DrawHorizontalArrowHead(Drawing::Graphics^ graphics, int x, int y, Drawing::Font^ font, bool left);
					static void DrawVerticalArrowHead(Drawing::Graphics^ graphics, int x, int y, Drawing::Font^ font, bool up);

				private:
					static cli::array<Drawing::Point>^ CalcHorizontalArrowHead(int x, int y, Drawing::Font^ font, bool left);
					static cli::array<Drawing::Point>^ CalcVerticalArrowHead(int x, int y, Drawing::Font^ font, bool up);
				};

				// -----------------------------------------------
			};

			public interface class IUIExtension
			{
				bool SelectTask(UInt32 taskID);
				bool SelectTasks(cli::array<UInt32>^ taskIDs);

				void UpdateTasks(TaskList^ tasks, UIExtension::UpdateType update);
				bool WantTaskUpdate(Task::Attribute attribute);
				bool WantSortUpdate(Task::Attribute attribute);
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

