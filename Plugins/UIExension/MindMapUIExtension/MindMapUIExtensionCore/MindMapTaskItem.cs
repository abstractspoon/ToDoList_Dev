using System;
using System.Drawing;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
	class MindMapTaskItem
	{
		// Data
		private String m_Title;
		private UInt32 m_TaskID;
        private UInt32 m_ParentID;
		private UInt32 m_ReferenceId;
		private List<UInt32> m_LocalDepends;
		private Color m_TextColor;
		private bool m_HasIcon;
		private bool m_IsFlagged;
		private bool m_IsParent;
        private bool m_IsDone;
        private bool m_IsGoodAsDone;
        private bool m_SomeSubtasksDone;
		private bool m_IsLocked;

		// -----------------------------------------------------------------

		public MindMapTaskItem(String label)
		{
			m_Title = label;
			m_TaskID = 0;
            m_ParentID = 0;
			m_TextColor = new Color();
			m_HasIcon = false;
			m_IsFlagged = false;
			m_IsParent = false;
			m_IsDone = false;
            m_IsGoodAsDone = false;
            m_SomeSubtasksDone = false;
			m_IsLocked = false;
			m_ReferenceId = 0;
			m_LocalDepends = null;
		}

		public MindMapTaskItem(Task task)
		{
			m_Title = task.GetTitle();
			m_TaskID = task.GetID();
			m_ParentID = task.GetParentID();
			m_TextColor = task.GetTextDrawingColor();
			m_HasIcon = (task.GetIcon().Length > 0);
			m_IsFlagged = task.IsFlagged(false);
			m_IsParent = task.IsParent();
            m_IsDone = task.IsDone();
            m_IsGoodAsDone = task.IsGoodAsDone();
            m_SomeSubtasksDone = task.HasSomeSubtasksDone();
			m_IsLocked = task.IsLocked(true);
			m_ReferenceId = task.GetReferenceID();
			m_LocalDepends = task.GetLocalDependency();
		}

		public void FixupParentID(MindMapTaskItem parent)
		{
            m_ParentID = parent.ID;
		}

        public bool FixupParentalStatus(int nodeCount, UIExtension.TaskIcon taskIcons)
        {
            bool wasParent = m_IsParent;

            if (nodeCount == 0)
            {
                m_IsParent = (!m_HasIcon && taskIcons.Get(m_TaskID));
            }
            else
            {
                m_IsParent = true;
            }

            return (m_IsParent != wasParent);
        }

		public override string ToString() 
		{
			return Title;
		}

		public void Update(Task task, HashSet<Task.Attribute> attribs)
		{
			// TODO
		}

		public String Title 
		{ 
			get 
			{ 
#if DEBUG
				return String.Format("{0} ({1})", m_Title, m_TaskID); 
#else
				return m_Title;
#endif
			} 

			set // only works for the root
			{
				if (!IsTask && !String.IsNullOrWhiteSpace(value))
					m_Title = value;
			}
		}
		
		public UInt32 ID { get { return m_TaskID; } }
        public UInt32 ParentID { get { return m_ParentID; } }
		public UInt32 ReferenceId { get { return m_ReferenceId; } }
		public Color TextColor { get { return m_TextColor; } }
		public bool HasIcon { get { return m_HasIcon; } }
		public bool IsFlagged { get { return m_IsFlagged; } }
		public bool IsParent { get { return m_IsParent; } }
		public bool IsLocked { get { return m_IsLocked; } }
		public bool IsReference { get { return (m_ReferenceId != 0); } }
		public bool IsTask { get { return (m_TaskID != 0); } }
        public bool HasSomeSubtasksDone { get { return m_SomeSubtasksDone; } }
		public bool HasLocalDependencies {  get { return (m_LocalDepends != null) && (m_LocalDepends.Count > 0); } }
		public List<UInt32> LocalDependencies { get { return m_LocalDepends; } }

		public bool IsDone(bool includeGoodAsDone) 
        { 
            if (includeGoodAsDone && m_IsGoodAsDone)
                return true;

            return m_IsDone; 
        }

		public bool SetDone(bool done = true)
		{
			if (m_IsDone == done)
				return false;

			m_IsDone = done;
			return true;
		}

		public bool ProcessTaskUpdate(Task task)
		{
			if (task.GetID() != m_TaskID)
				return false;

			if (task.IsAttributeAvailable(Task.Attribute.Title))
				m_Title = task.GetTitle();

			if (task.IsAttributeAvailable(Task.Attribute.Icon))
				m_HasIcon = (task.GetIcon().Length > 0);

			if (task.IsAttributeAvailable(Task.Attribute.Flag))
				m_IsFlagged = task.IsFlagged(false);

			if (task.IsAttributeAvailable(Task.Attribute.Color))
				m_TextColor = task.GetTextDrawingColor();

            if (task.IsAttributeAvailable(Task.Attribute.SubtaskDone))
                m_SomeSubtasksDone = task.HasSomeSubtasksDone();

            if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
                m_IsDone = task.IsDone();

			if (task.IsAttributeAvailable(Task.Attribute.Dependency))
				m_LocalDepends = task.GetLocalDependency();

			m_IsParent = task.IsParent();
			m_IsLocked = task.IsLocked(true);
            m_IsGoodAsDone = task.IsGoodAsDone();
			m_ReferenceId = task.GetReferenceID();

			return true;
		}
	}

}

