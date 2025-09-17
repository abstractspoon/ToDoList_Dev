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

				enum class HitTestReason
				{
					None,
					InfoTip,
					ContextMenu,
				};

				// -----------------------------------------------

				enum class HitTestResult
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

				enum class OleDragCursorType
				{
					No,
					Move,
					Copy,
					Link,
				};

				// -----------------------------------------------

				static UpdateType MapUpdateType(IUI_UPDATETYPE type);
				static IUI_HITTEST MapHitTestResult(HitTestResult result);
				static HitTestReason MapHitTestReason(IUI_HITTESTREASON reason);

				static bool MapGetTaskCmd(IUI_APPCOMMAND nCmd, GetTask% getTask);
				static bool MapSelectTaskCmd(IUI_APPCOMMAND nCmd, SelectTask% selectTask);

				static Windows::Forms::Cursor^ AppCursor(AppCursorType cursorType);
				static Windows::Forms::Cursor^ OleDragCursor(OleDragCursorType cursorType);
				static Windows::Forms::Cursor^ HandCursor();

				static bool SaveImageToFile(Drawing::Bitmap^ image, String^ filepath);

				// -----------------------------------------------

				ref class ParentNotify
				{
				public:
					ParentNotify(IntPtr hwndParent);

					bool AddMod(Task::Attribute attrib, DateTime value);
					bool AddMod(Task::Attribute attrib, double value);
					bool AddMod(Task::Attribute attrib, double time, Task::TimeUnits units);
					bool AddMod(Task::Attribute attrib, int value);
					bool AddMod(Task::Attribute attrib, bool value);
					bool AddMod(Task::Attribute attrib, String^ value);
					bool AddMod(String^ sCustAttribID, String^ value);

					bool AddMod(Task::Attribute attrib, double value, bool append);
					bool AddMod(Task::Attribute attrib, double time, Task::TimeUnits units, bool append);
					bool AddMod(Task::Attribute attrib, int value, bool append);
					bool AddMod(Task::Attribute attrib, String^ value, bool append);

					bool AddMod(UInt32 taskID, Task::Attribute attrib, DateTime value);
					bool AddMod(UInt32 taskID, Task::Attribute attrib, double value);
					bool AddMod(UInt32 taskID, Task::Attribute attrib, double time, Task::TimeUnits units);
					bool AddMod(UInt32 taskID, Task::Attribute attrib, int value);
					bool AddMod(UInt32 taskID, Task::Attribute attrib, bool value);
					bool AddMod(UInt32 taskID, Task::Attribute attrib, String^ value);
					bool AddMod(UInt32 taskID, String^ sCustAttribID, String^ value);

					bool AddMod(UInt32 taskID, Task::Attribute attrib, double value, bool append);
					bool AddMod(UInt32 taskID, Task::Attribute attrib, double time, Task::TimeUnits units, bool append);
					bool AddMod(UInt32 taskID, Task::Attribute attrib, int value, bool append);
					bool AddMod(UInt32 taskID, Task::Attribute attrib, String^ value, bool append);

					bool NotifyMod();
					bool NotifyMod(Task::Attribute attrib, DateTime value);
					bool NotifyMod(Task::Attribute attrib, double value);
					bool NotifyMod(Task::Attribute attrib, double time, Task::TimeUnits units);
					bool NotifyMod(Task::Attribute attrib, int value);
					bool NotifyMod(Task::Attribute attrib, bool value);
					bool NotifyMod(Task::Attribute attrib, String^ value);
					bool NotifyMod(String^ sCustAttribID, String^ value);

					bool NotifyMod(Task::Attribute attrib, double value, bool append);
					bool NotifyMod(Task::Attribute attrib, double time, Task::TimeUnits units, bool append);
					bool NotifyMod(Task::Attribute attrib, int value, bool append);
					bool NotifyMod(Task::Attribute attrib, String^ value, bool append);

					bool NotifyMove(UInt32 taskID, UInt32 parentTaskID, UInt32 afterSiblingID);
					bool NotifyCopy(UInt32 taskID, UInt32 parentTaskID, UInt32 afterSiblingID);

					bool NotifySelChange(UInt32 taskID);
					bool NotifySelChange(cli::array<UInt32>^ pdwTaskIDs);

					bool NotifyEditLabel();
					bool NotifyEditIcon();

					bool NotifyTasklistMetaData(String^ metaData);

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

						IUITaskMod(Task::Attribute attrib, double value, bool append);
						IUITaskMod(Task::Attribute attrib, double time, Task::TimeUnits units, bool append);
						IUITaskMod(Task::Attribute attrib, int value, bool append);
						IUITaskMod(Task::Attribute attrib, String^ value, bool append);

						IUITaskMod(String^ customAttribId, String^ value);

						IUITaskMod(UInt32 taskID, Task::Attribute attrib, DateTime value);
						IUITaskMod(UInt32 taskID, Task::Attribute attrib, double value);
						IUITaskMod(UInt32 taskID, Task::Attribute attrib, double time, Task::TimeUnits units);
						IUITaskMod(UInt32 taskID, Task::Attribute attrib, int value);
						IUITaskMod(UInt32 taskID, Task::Attribute attrib, bool value);
						IUITaskMod(UInt32 taskID, Task::Attribute attrib, String^ value);

						IUITaskMod(UInt32 taskID, Task::Attribute attrib, double value, bool append);
						IUITaskMod(UInt32 taskID, Task::Attribute attrib, double time, Task::TimeUnits units, bool append);
						IUITaskMod(UInt32 taskID, Task::Attribute attrib, int value, bool append);
						IUITaskMod(UInt32 taskID, Task::Attribute attrib, String^ value, bool append);

						IUITaskMod(UInt32 taskID, String^ customAttribId, String^ value);

						bool CopyTo(IUITASKMOD& mod);

					public:
						Task::Attribute nAttrib;
						UInt32 dwSelectedTaskID;		
						String^ szCustomAttribID;
						bool bAppend;

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
							bool bCostIsRate;
							UInt32 dwPrevDependID;
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

					static property int IconSize { int get(); }

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
						None = -1,
						Selected,
						SelectedNotFocused,
						DropHighlighted,
					};

				public:
					static bool Draw(IntPtr hwnd, Drawing::Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy, bool transparent);
					static bool Draw(IntPtr hwnd, Drawing::Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy, Style style, bool transparent);
					static bool Draw(IntPtr hwnd, Drawing::Graphics^ dc, Int32 x, Int32 y, Int32 cx, Int32 cy, Style style, bool transparent, bool clipLeft, bool clipRight);

					static Drawing::Color GetTextColor(Style style, Drawing::Color baseColor);
					static Drawing::Color GetBorderColor(Style style);
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

				ref class ArrowHeads
				{
				public:
					enum class Direction
					{
						None = -1,
						Left,
						Up,
						Right,
						Down,
					};

					static void Draw(Drawing::Graphics^ graphics, Drawing::Pen^ pen, int x, int y, int size, Direction dir);
					static void Draw(Drawing::Graphics^ graphics, Drawing::Pen^ pen, int x, int y, int size, float angleDegrees);

					static void Draw(Drawing::Graphics^ graphics, Drawing::Pen^ pen, int x, int y, int size, int offset, Direction dir);
					static void Draw(Drawing::Graphics^ graphics, Drawing::Pen^ pen, int x, int y, int size, int offset, float angleDegrees);

				protected:
					static cli::array<Drawing::Point>^ Calculate(int x, int y, int size, int offset, Direction dir);
					static cli::array<Drawing::Point>^ Offset(cli::array<Drawing::Point>^ arrow, int amount, Direction dir);
				};

				// -----------------------------------------------

				ref class DependencyArrows : ArrowHeads
				{
				public:
					static void Draw(Drawing::Graphics^ graphics, int x, int y, Drawing::Font^ font, Direction dir);
					static int Size(Drawing::Font^ font);

				};

				// -----------------------------------------------
			};

			public interface class IUIExtension
			{
				bool SelectTask(UInt32 taskID);
				bool SelectTasks(cli::array<UInt32>^ taskIDs);
				bool ScrollToSelectedTask();
				bool CanScrollToSelectedTask();

				void UpdateTasks(TaskList^ tasks, UIExtension::UpdateType update);
				bool WantTaskUpdate(Task::Attribute attribute);
				bool WantSortUpdate(Task::Attribute attribute);
				bool PrepareNewTask(Task^% task);

				bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos);
				bool DoIdleProcessing();
				bool GetLabelEditRect(Int32% left, Int32% top, Int32% right, Int32% bottom); // screen coordinates

				UIExtension::HitTestResult HitTest(Int32 xPos, Int32 yPos, UIExtension::HitTestReason reason);
				UInt32 HitTestTask(Int32 xPos, Int32 yPos, UIExtension::HitTestReason reason);

				void SetUITheme(UITheme^ theme);
				void SetReadOnly(bool bReadOnly);

				void SavePreferences(Preferences^ prefs, String^ key);
				void LoadPreferences(Preferences^ prefs, String^ key, bool appOnly);
			};
		}
	}
}

