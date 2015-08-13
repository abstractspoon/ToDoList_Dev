#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

class IPreferences;
class ITaskList14;
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

   public ref class CTaskList
   {
   public:
      CTaskList(ITaskList14* pTaskList);        // GET & SET
      CTaskList(const ITaskList14* pTaskList);  // GET ONLY
      
      // GETTERS ----------------------------------------------------

      String^ GetReportTitle();
      String^ GetReportDate();
      String^ GetMetaData(String^ sKey);
      
      UInt32  GetCustomAttributeCount();
      String^ GetCustomAttributeLabel(int nIndex);
      String^ GetCustomAttributeID(int nIndex);
      String^ GetCustomAttributeValue(int nIndex, String^ sItem);

      UInt32 GetTaskCount();
      IntPtr FindTask(UInt32 dwTaskID);
      
      IntPtr GetFirstTask(IntPtr hTask);
      IntPtr GetNextTask(IntPtr hTask);
      IntPtr GetTaskParent(IntPtr hTask);

      String^ GetTaskTitle(IntPtr hTask);
      String^ GetTaskComments(IntPtr hTask);
      String^ GetTaskAllocatedBy(IntPtr hTask);
      String^ GetTaskStatus(IntPtr hTask);
      String^ GetTaskWebColor(IntPtr hTask);
      String^ GetTaskPriorityWebColor(IntPtr hTask);
      String^ GetTaskVersion(IntPtr hTask);
      String^ GetTaskExternalID(IntPtr hTask);
      String^ GetTaskCreatedBy(IntPtr hTask);
      String^ GetTaskPositionString(IntPtr hTask);
      String^ GetTaskIcon(IntPtr hTask);

      UInt32 GetTaskID(IntPtr hTask);
      UInt32 GetTaskColor(IntPtr hTask);
      UInt32 GetTaskTextColor(IntPtr hTask);
      UInt32 GetTaskPriorityColor(IntPtr hTask);
      UInt32 GetTaskPosition(IntPtr hTask);
      UInt32 GetTaskPriority(IntPtr hTask);
      UInt32 GetTaskRisk(IntPtr hTask);

      UInt32 GetTaskCategoryCount(IntPtr hTask);
      UInt32 GetTaskAllocatedToCount(IntPtr hTask);
      UInt32 GetTaskTagCount(IntPtr hTask);
      UInt32 GetTaskDependencyCount(IntPtr hTask);
      UInt32 GetTaskFileReferenceCount(IntPtr hTask);

      String^ GetTaskAllocatedTo(IntPtr hTask, int nIndex);
      String^ GetTaskCategory(IntPtr hTask, int nIndex);
      String^ GetTaskTag(IntPtr hTask, int nIndex);
      String^ GetTaskDependency(IntPtr hTask, int nIndex);
      String^ GetTaskFileReference(IntPtr hTask, int nIndex);

      Byte GetTaskPercentDone(IntPtr hTask);
      double GetTaskCost(IntPtr hTask);

      Int64 GetTaskLastModified(IntPtr hTask);
      Int64 GetTaskDoneDate(IntPtr hTask);
      Int64 GetTaskDueDate(IntPtr hTask);
      Int64 GetTaskStartDate(IntPtr hTask);
      Int64 GetTaskCreationDate(IntPtr hTask);

      String^ GetTaskDoneDateString(IntPtr hTask);
      String^ GetTaskDueDateString(IntPtr hTask);
      String^ GetTaskStartDateString(IntPtr hTask);
      String^ GetTaskCreationDateString(IntPtr hTask);

      Boolean IsTaskDone(IntPtr hTask);
      Boolean IsTaskDue(IntPtr hTask);
      Boolean IsTaskGoodAsDone(IntPtr hTask);
      Boolean IsTaskFlagged(IntPtr hTask);
      
      double GetTaskTimeEstimate(IntPtr hTask, Char% cUnits);
      double GetTaskTimeSpent(IntPtr hTask, Char% cUnits);

      Boolean GetTaskRecurrence(IntPtr hTask); // TODO
      Boolean TaskHasAttribute(IntPtr hTask, String^ sAttrib);

      String^ GetTaskAttribute(IntPtr hTask, String^ sAttrib);
      String^ GetTaskCustomAttributeData(IntPtr hTask, String^ sID);
      String^ GetTaskMetaData(IntPtr hTask, String^ sKey);

      // SETTERS -----------------------------------------------------
      
      Boolean AddCustomAttribute(String^ sID, String^ sLabel);
      Boolean SetMetaData(String^ sKey, String^ sValue);
      Boolean ClearMetaData(String^ sKey);

      IntPtr NewTask(String^ sTitle, IntPtr hParent);

      Boolean SetTaskTitle(IntPtr hTask, String^ sTitle);
      Boolean SetTaskComments(IntPtr hTask, String^ sComments);
      Boolean SetTaskAllocatedBy(IntPtr hTask, String^ sAllocBy);
      Boolean SetTaskStatus(IntPtr hTask, String^ sStatus);
      Boolean SetTaskVersion(IntPtr hTask, String^ sVersion);
      Boolean SetTaskExternalID(IntPtr hTask, String^ sExternalID);
      Boolean SetTaskCreatedBy(IntPtr hTask, String^ sCreatedBy);
      Boolean SetTaskPosition(IntPtr hTask, String^ sPosition);
      Boolean SetTaskIcon(IntPtr hTask, String^ sIcon);

      Boolean AddTaskAllocatedTo(IntPtr hTask, String^ sAllocTo);
      Boolean AddTaskCategory(IntPtr hTask, String^ sCategory);
      Boolean AddTaskTag(IntPtr hTask, String^ sTag);
      Boolean AddTaskDependency(IntPtr hTask, String^ sDependency);
      Boolean AddTaskFileReference(IntPtr hTask, String^ sFileLink);

      Boolean SetTaskColor(IntPtr hTask, UINT32 color );
      Boolean SetTaskPriority(IntPtr hTask, Byte nPriority);
      Boolean SetTaskRisk(IntPtr hTask, Byte Risk);

      Boolean SetTaskPercentDone(IntPtr hTask, Byte nPercent);
      Boolean SetTaskCost(IntPtr hTask, double dCost);
      Boolean SetTaskFlag(IntPtr hTask, Boolean bFlag);

      Boolean SetTaskLastModified(IntPtr hTask, Int64 dtLastMod);
      Boolean SetTaskDoneDate(IntPtr hTask, Int64 dtCompletion);
      Boolean SetTaskDueDate(IntPtr hTask, Int64 dtDue);
      Boolean SetTaskStartDate(IntPtr hTask, Int64 dtStart);
      Boolean SetTaskCreationDate(IntPtr hTask, Int64 dtCreation);

      Boolean SetTaskTimeEstimate(IntPtr hTask, double dTime, Char cUnits);
      Boolean SetTaskTimeSpent(IntPtr hTask, double dTime, Char cUnits);
      Boolean SetTaskCustomAttributeData(IntPtr hTask, String^ sID, String^ sValue);
      Boolean ClearTaskCustomAttributeData(IntPtr hTask, String^ sID);
      Boolean SetTaskMetaData(IntPtr hTask, String^ sKey, String^ sValue);
      Boolean ClearTaskMetaData(IntPtr hTask, String^ sKey);

   private: // -------------------------------------------------------
      ITaskList14* m_pTaskList;
      const ITaskList14* m_pConstTaskList;

   private: // -------------------------------------------------------
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

