using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{

	public class TaskExtensionItem : Calendar.Appointment
	{
		protected TaskItem m_RealItem;

		protected TaskExtensionItem(TaskItem item, UInt32 id) : base(item)
		{
			m_RealItem = item;
			Id = id;
			TextColor = SystemColors.ControlDarkDark;
		}

		public UInt32 RealTaskId { get { return m_RealItem.Id; } }
		public TaskItem RealTask { get { return m_RealItem; } }
	}

	// ---------------------------------------------------------------

	public class TaskFutureOccurrence : TaskExtensionItem
	{
		public TaskFutureOccurrence(TaskItem item, UInt32 id, DateTime start, DateTime end) : base(item, id)
		{
			Locked = true; // always (for now)

			StartDate = start;
			EndDate = TaskItem.CheckGetEndOfDay(end);
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return RealTask.IsLongAppt(start, end);
		}
	}

	// ---------------------------------------------------------------

	public class TaskCustomDateAttribute : TaskExtensionItem
	{
		public TaskCustomDateAttribute(TaskItem item, UInt32 id, string attribId, DateTime date) : base(item, id)
		{
			AttributeId = attribId;
			StartDate = OriginalDate = date;
			EndDate = StartDate.AddDays(1);
			Locked = item.Locked;

			if (EndDate == EndDate.Date)
				EndDate = EndDate.AddSeconds(-1); // end of day before
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return true; // always 24 hours
		}

		public void RestoreOriginalDate()
		{
			RealTask.CustomDates[AttributeId] = OriginalDate;
		}

		public void UpdateTaskDate()
		{
			RealTask.CustomDates[AttributeId] = StartDate;
		}

		public void ClearDate()
		{
			RealTask.CustomDates[AttributeId] = DateTime.MinValue;
		}

		private bool TreatAsDueToday
		{
			get
			{
				return (m_RealItem.TreatOverdueTasksAsDueToday && (base.EndDate.Date < DateTime.Now.Date));
			}
		}

		public override DateTime EndDate
		{
			get
			{
				return (TreatAsDueToday ? TaskItem.EndOfDay(DateTime.Now) : base.EndDate);
			}
			set	{ base.EndDate = value; }
		}

		public bool HasCalculatedEndDate { get {  return TreatAsDueToday; } }

		public string AttributeId { get; private set; }
		public DateTime OriginalDate { get; private set; }
	}

	// ---------------------------------------------------------------

	public class TaskTimeBlock : TaskExtensionItem
	{
		private Calendar.AppointmentDates m_OrgDates = new Calendar.AppointmentDates();
		private TimeBlock m_TimeBlock = null; // Reference back to task 

		public TaskTimeBlock(TaskItem item, UInt32 id, TimeBlock block) : base(item, id)
		{
			Locked = false; // Never

			// Copy dates
			StartDate = m_OrgDates.Start = block.Start;
			EndDate = m_OrgDates.End = TaskItem.CheckGetEndOfDay(block.End);

			// Reference back to original
			m_TimeBlock = block;
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return false; // Always
		}

		public void RestoreTimeBlock()
		{
			m_TimeBlock.Start = m_OrgDates.Start;
			m_TimeBlock.End = m_OrgDates.End;
		}

		public void UpdateTimeBlock()
		{
			m_TimeBlock.Start = StartDate;
			m_TimeBlock.End = EndDate;
		}

		public TimeBlock TimeBlock
		{
			get { return m_TimeBlock; }
		}
	}

}
