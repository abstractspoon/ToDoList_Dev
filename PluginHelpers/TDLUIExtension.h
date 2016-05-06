#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "PluginHelpers.h"
#include "TDLTaskList.h"
#include "TDLPreferences.h"

#include "..\..\Interfaces\IUIExtension.h"

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace TDLPluginHelpers
{
   public ref class TDLTheme
   {
   public:
      TDLTheme(const UITHEME* pTheme);
      
	  enum class RenderStyle 
	  { 
		  Gradient, 
		  Glass, 
		  GlassWithGradient 
	  };
	  RenderStyle GetRenderStyle();

	  enum class AppColor 
	  { 
		  AppBackDark, 
		  AppBackLight, 
		  AppLinesDark, 
		  AppLinesLight,
		  AppText,
		  MenuBack,
		  ToolbarDark,
		  ToolbarLight,
		  StatusBarDark,
		  StatusBarLight,
		  StatusBarText,
	  };
	  Windows::Media::Color GetAppColor(AppColor color);
	  static Drawing::Color Map(Windows::Media::Color color);

	  String^ GetToolBarImagePath();
	  Windows::Media::Color GetToolbarTransparencyColor();

      
   private:
      UITHEME* m_pTheme;

   private:
      TDLTheme();
	  Windows::Media::Color GetColor(UInt32 rgbColor);
   };
   
////////////////////////////////////////////////////////////////////////////////////////////////

   public ref class TDLUIExtension
   {
   public: 
	   enum class UpdateType
	   {
		   Unknown = -1,

		   Edit,
		   Add,
		   Delete,
		   Move,

		   All = 0xffff
	   };
	   
	   enum class AppCommand
	   { 
		   Unknown = -1,

		   ExpandAll,
		   CollapseAll,
		   ExpandSelected,
		   CollapseSelected,
		   Sort,			// dwExtra is column ID
		   ToggleSort,		// dwExtra is column ID
		   SetFocus,
		   ResizeAttributeColumns,

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
		   MoveTask,

		   // new values here ONLY

		   All = 0xffff
	   };

	   static TaskAttribute Map(IUI_ATTRIBUTE attrib);
	   static UpdateType Map(IUI_UPDATETYPE type);
	   static AppCommand Map(IUI_APPCOMMAND cmd);
	   static IUI_HITTEST Map(HitResult test);
	   static IUI_ATTRIBUTE Map(TaskAttribute attrib);

   };

   public interface class ITDLUIExtension
   {
	   bool SelectTask(UInt32 dwTaskID);
	   bool SelectTasks(cli::array<UInt32>^ pdwTaskIDs);

	   void UpdateTasks(TDLTaskList^ tasks, TDLUIExtension::UpdateType nUpdate, TDLUIExtension::TaskAttribute nEditAttribute);
	   bool WantEditUpdate(TDLUIExtension::TaskAttribute nAttribute);
	   bool WantSortUpdate(TDLUIExtension::TaskAttribute nAttribute);
	   bool PrepareNewTask(TDLTaskList^ task);

	   bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos);
	   bool DoAppCommand(TDLUIExtension::AppCommand nCmd, DWORD dwExtra);
	   bool CanDoAppCommand(TDLUIExtension::AppCommand nCmd, DWORD dwExtra);

	   bool GetLabelEditRect(Int32% left, Int32% top, Int32% right, Int32% bottom); // screen coordinates
	   TDLUIExtension::HitResult HitTest(Int32 xPos, Int32 yPos);

	   void SetUITheme(TDLTheme^ theme);
	   void SetReadOnly(bool bReadOnly);

	   void SavePreferences(TDLPreferences^ prefs, String^ key);
	   void LoadPreferences(TDLPreferences^ prefs, String^ key, bool appOnly);
   };

   public ref class TDLNotify
   {
   public:
	   TDLNotify(IntPtr hwnd);

	   bool NotifyMod(TDLUIExtension::TaskAttribute nAttribute, DateTime value);
	   bool NotifyMod(TDLUIExtension::TaskAttribute nAttribute, double value);
	   bool NotifyMod(TDLUIExtension::TaskAttribute nAttribute, double time, TDLTask::TimeUnits units);
	   bool NotifyMod(TDLUIExtension::TaskAttribute nAttribute, int value);
	   bool NotifyMod(TDLUIExtension::TaskAttribute nAttribute, bool value);
	   bool NotifyMod(String^ sCustAttribID, String^ value);

	   bool NotifySelChange(UInt32 taskID);
	   bool NotifySelChange(cli::array<UInt32>^ pdwTaskIDs);

   private:
	   HWND m_hwnd;

   private:
	   bool DoNotify(const IUITASKMOD* pMod, int numMod);
   };

}


