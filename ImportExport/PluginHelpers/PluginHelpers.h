#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

class IPreferences;
class ITaskList13;
class ITransText;

typedef void* HTASKITEM;

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning( push )
#undef GetProfileInt
#undef GetProfileString

////////////////////////////////////////////////////////////////////////////////////////////////

namespace PluginHelpers
{

////////////////////////////////////////////////////////////////////////////////////////////////

   public ref class CPreferences
   {
   public:
      CPreferences(IPreferences* pPrefs);
      
      int GetProfileInt(String^ sSection, String^ sEntry, int nDefault);
      bool WriteProfileInt(String^ sSection, String^ sEntry, int nValue);

      String^ GetProfileString(String^ sSection, String^ sEntry, String^ sDefault);
      bool WriteProfileString(String^ sSection, String^ sEntry, String^ sValue);

      double GetProfileDouble(String^ sSection, String^ sEntry, double dDefault);
      bool WriteProfileDouble(String^ sSection, String^ sEntry, double dValue);
      
   private:
      IPreferences* m_pPrefs;

   private:
      CPreferences();
   };

////////////////////////////////////////////////////////////////////////////////////////////////

#define DEF_GETTASKVALFUNC(fn, t) t fn(IntPtr hTask)
#define DEF_GETTASKVALFUNC_IDX(fn, t) t fn(IntPtr hTask, int nIndex)

   public ref class CTaskList
   {
   public:
      CTaskList(ITaskList13* pTaskList);
      CTaskList(const ITaskList13* pTaskList);
      
      IntPtr NewTask(String^ sTitle, IntPtr hParent);
      IntPtr GetFirstTask(IntPtr hParent);
      
      DEF_GETTASKVALFUNC(GetNextTask, IntPtr);
      DEF_GETTASKVALFUNC(GetTaskParent, IntPtr);

      DEF_GETTASKVALFUNC(GetTaskTitle, String^);
      DEF_GETTASKVALFUNC(GetTaskComments, String^);
      DEF_GETTASKVALFUNC(GetTaskAllocatedBy, String^);
      DEF_GETTASKVALFUNC(GetTaskStatus, String^);
      DEF_GETTASKVALFUNC(GetTaskFileReferencePath, String^);
      DEF_GETTASKVALFUNC(GetTaskWebColor, String^);
      DEF_GETTASKVALFUNC(GetTaskPriorityWebColor, String^);
      DEF_GETTASKVALFUNC(GetTaskVersion, String^);

      DEF_GETTASKVALFUNC_IDX(GetTaskAllocatedTo, String^);
      DEF_GETTASKVALFUNC_IDX(GetTaskCategory, String^);
      DEF_GETTASKVALFUNC_IDX(GetTaskTag, String^);
      DEF_GETTASKVALFUNC_IDX(GetTaskDependency, String^);
   
      DEF_GETTASKVALFUNC(GetTaskID, UInt32);
      DEF_GETTASKVALFUNC(GetTaskColor, UInt32);
      DEF_GETTASKVALFUNC(GetTaskPriorityColor, UInt32);
      DEF_GETTASKVALFUNC(GetTaskPosition, UInt32);
      DEF_GETTASKVALFUNC(GetTaskPriority, UInt32);

      DEF_GETTASKVALFUNC(GetTaskPercentDone, Byte);

      double GetTaskTimeEstimate(HTASKITEM hTask, Char% cUnits);
      double GetTaskTimeSpent(HTASKITEM hTask, Char% cUnits);

      DEF_GETTASKVALFUNC(GetTaskLastModified, Int64);
      DEF_GETTASKVALFUNC(GetTaskDoneDate, Int64);
      DEF_GETTASKVALFUNC(GetTaskDueDate, Int64);
      DEF_GETTASKVALFUNC(GetTaskStartDate, Int64);

      DEF_GETTASKVALFUNC(GetTaskDoneDateString, String^);
      DEF_GETTASKVALFUNC(GetTaskDueDateString, String^);
      DEF_GETTASKVALFUNC(GetTaskStartDateString, String^);

      DEF_GETTASKVALFUNC(IsTaskDone, Boolean);
      DEF_GETTASKVALFUNC(IsTaskDue, Boolean);
      
      // TODO
      
   private:
      ITaskList13* m_pTaskList;
      const ITaskList13* m_pConstTaskList;

   private:
      CTaskList();
   };

////////////////////////////////////////////////////////////////////////////////////////////////

   public ref class CTransText
   {
   public:
      CTransText(ITransText* pTaskList);
      
      // TODO
      
   private:
      ITransText* m_pTransText;

   private:
      CTransText();
   };


////////////////////////////////////////////////////////////////////////////////////////////////

}

#pragma warning( pop )

