using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace EvidenceBoardUIExtension
{
	public class TaskItem
	{
		public static readonly Point NullPoint = new Point(int.MinValue, int.MinValue);
		public static string MetaDataKey = string.Empty;

		public string Title { get; private set; }
		public string ImagePath { get; private set; }
		public Color TextColor { get; private set; }

		public bool HasIcon { get; private set; }
		public bool IsFlagged { get; private set; }
		public bool IsParent { get; private set; }
		public bool IsTopLevel { get; private set; }
		public bool SomeSubtasksDone { get; private set; }
		public bool IsLocked { get; private set; }

		public DateTime StartDate, EndDate;

		public uint TaskId { get; private set; }
		public uint ParentId { get; private set; }

		public List<uint> ChildIds { get; private set; }
		public List<uint> DependIds { get; private set; }

		List<UserLink> m_UserLinks;

		public IEnumerable<UserLink> UserLinks { get { return m_UserLinks; } }

		private bool Done;
		private bool GoodAsDone;

		public Point UserPosition;
		public bool HasUserPosition { get { return (UserPosition != NullPoint); } }
		public Image Image;

		// -----------------------------------------------------------------

		public TaskItem()
		{
			StartDate = EndDate = DateTime.MinValue;
		}

		public TaskItem(Task task)
		{
			Debug.Assert(!string.IsNullOrWhiteSpace(MetaDataKey));

			Title = task.GetTitle();
			TextColor = task.GetTextDrawingColor();
			HasIcon = (task.GetIcon().Length > 0);
			IsFlagged = task.IsFlagged(false);
			IsParent = task.IsParent();
			IsTopLevel = (task.GetParentID() == 0);
			Done = task.IsDone();
			GoodAsDone = task.IsGoodAsDone();
			SomeSubtasksDone = task.HasSomeSubtasksDone();
			IsLocked = task.IsLocked(true);

			StartDate = task.GetStartDate(true);
			EndDate = (Done ? task.GetDoneDate() : task.GetDueDate(true));

			ParentId = task.GetParentID();
			TaskId = task.GetID();

			ChildIds = new List<uint>();
			DependIds = task.GetLocalDependency();
			m_UserLinks = null;

			UpdateImage(task);
			DecodeMetaData(task.GetMetaDataValue(MetaDataKey));
		}

		public override string ToString()
		{
#if DEBUG
			return string.Format("{0} ({1})", Title, TaskId);
#else
			return Title;
#endif
		}

		public int CalcImageHeight(int width)
		{
			if (Image == null)
				return 0;

			return ((width * Image.Height) / Image.Width);
		}

		public bool HasLocalDependencies { get { return (DependIds != null) && (DependIds.Count > 0); } }

		public bool IsDone(bool includeGoodAsDone)
		{
			if (includeGoodAsDone && GoodAsDone)
				return true;

			return Done;
		}

		public string EncodeMetaData()
		{
			string metaData = string.Format("{0},{1}|", UserPosition.X, UserPosition.Y);

			if (HasUserLinks)
				metaData = metaData + string.Join(",", UserLinks);

			return metaData;
		}

		public void DecodeMetaData(string metaData)
		{
			UserPosition = NullPoint;
			m_UserLinks = new List<UserLink>();

			if (string.IsNullOrWhiteSpace(metaData))
				return;

			string[] parts = metaData.Split(new char[1] { '|' }, StringSplitOptions.RemoveEmptyEntries);

			if (parts.Count() > 0)
			{
				string[] coords = parts[0].Split(',');

				if (coords.Count() == 2)
				{
					int x, y;

					if (int.TryParse(coords[0], out x) && int.TryParse(coords[1], out y))
					{
						UserPosition.X = x;
						UserPosition.Y = y;
					}
					else
					{
						Debug.Assert(false);
						return;
					}
				}
				else
				{
					Debug.Assert(false);
					return;
				}

				if (parts.Count() == 2)
				{
					string[] linkDatas = parts[1].Split(',');

					foreach (var linkData in linkDatas)
					{
						UserLink link;

						if (UserLink.TryParse(linkData, TaskId, out link))
							m_UserLinks.Add(link);
						else
							Debug.Assert(false);
					}
				}
			}
			else
			{
				Debug.Assert(false);
				return;
			}
		}

		public UserLink FindUserLink(uint toId)
		{
			return m_UserLinks?.Find(x => (x.ToId == toId));
		}

		public bool HasUserLink(uint toId)
		{
			return (FindUserLink(toId) != null);
		}

		public bool HasUserLinks
		{
			get { return ((m_UserLinks != null) && (m_UserLinks.Count > 0)); }
		}

		public bool HasUserLink(string type)
		{
			return (m_UserLinks?.Find(x => (x.Attributes.Type == type)) != null);
		}

		public bool DeleteUserLink(UserLink link)
		{
			if (m_UserLinks.Remove(link))
				return true;

			Debug.Assert(FindUserLink(link.ToId) == null);
			return false;
		}

		public UserLink AddUserLink(uint toId, UserLinkAttributes attrib)
		{
			if ((toId == 0) || (toId == TaskId) || HasUserLink(toId))
				return null;

			var newLink = new UserLink(TaskId, toId, attrib);
			m_UserLinks.Add(newLink);

			return newLink;
		}

		private void UpdateImage(Task task)
		{
			var filePaths = task.GetFileLink(true);

			foreach (var path in filePaths)
			{
				try
				{
					if (path != ImagePath)
					{
						var image = Image.FromFile(path);

						if (image != null)
						{
							ImagePath = path;
							Image = image;
							return;
						}
					}
				}
				catch (Exception /*e*/)
				{
					// keep going
				}
			}

			Image = null;
			ImagePath = string.Empty;
		}

		public bool Update(Task task)
		{
			if (task.GetID() != TaskId)
				return false;

			if (task.GetReferenceID() != 0)
				return false;

			if (task.IsAttributeAvailable(Task.Attribute.Title))
				Title = task.GetTitle();

			if (task.IsAttributeAvailable(Task.Attribute.Icon))
				HasIcon = (task.GetIcon().Length > 0);

			if (task.IsAttributeAvailable(Task.Attribute.Flag))
				IsFlagged = task.IsFlagged(false);

			if (task.IsAttributeAvailable(Task.Attribute.Color))
				TextColor = task.GetTextDrawingColor();

			if (task.IsAttributeAvailable(Task.Attribute.SubtaskDone))
				SomeSubtasksDone = task.HasSomeSubtasksDone();

			if (task.HasAttribute(Task.Attribute.StartDate))
				StartDate = task.GetStartDate(true);

			if (!Done && task.HasAttribute(Task.Attribute.DueDate))
				EndDate = task.GetDueDate(true);

			if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
			{
				Done = task.IsDone();
				EndDate = task.GetDoneDate();
			}

			if (task.IsAttributeAvailable(Task.Attribute.FileLink))
				UpdateImage(task);

			if (task.IsAttributeAvailable(Task.Attribute.Dependency))
				DependIds = task.GetLocalDependency();

			if (task.IsAttributeAvailable(Task.Attribute.MetaData))
				DecodeMetaData(task.GetMetaDataValue(MetaDataKey));

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true);
			GoodAsDone = task.IsGoodAsDone();

			Debug.Assert(task.GetParentID() == ParentId);
			IsTopLevel = (task.GetParentID() == 0);

			return true;
		}

		public static bool IsValidDate(DateTime date)
		{
			return (date != DateTime.MinValue);
		}

		public void MinMax(ref DateTime startDate, ref DateTime endDate)
		{
			if (IsValidDate(StartDate))
			{
				if (!IsValidDate(startDate) ||(StartDate < startDate))
					startDate = StartDate;
			}

			if (IsValidDate(EndDate))
			{
				if (!IsValidDate(endDate) || (EndDate > endDate))
					endDate = EndDate;
			}
		}

		public bool IntersectsWith(DateTime min, DateTime max)
		{
			if (IsValidDate(StartDate) && (StartDate >= min) && (StartDate <= max))
				return true;

			if (IsValidDate(EndDate) && (EndDate >= min) && (EndDate <= max))
				return true;

			return false;
		}
	}

	///////////////////////////////////////////////////////////////////////

	public class TaskItems : Dictionary<uint, TaskItem>
	{
		public bool Empty { get { return (Count == 0); } }

		public TaskItem GetTaskItem(uint uniqueId)
		{
			TaskItem item = null;
			TryGetValue(uniqueId, out item);

			return item;
		}

		public bool DeleteTask(uint uniqueId)
		{
			if (!ContainsKey(uniqueId))
				return false;

			Remove(uniqueId);
			return true;
		}

		public bool AddTask(TaskItem node)
		{
			if (ContainsKey(node.TaskId))
				return false;

			Add(node.TaskId, node);
			return true;
		}

		public UserLink FindUserLink(uint fromId, uint toId)
		{
			return GetTaskItem(fromId)?.FindUserLink(toId);
		}

		public bool HasUserLink(uint fromId, uint toId)
		{
			return (FindUserLink(fromId, toId) != null);
		}

		public bool HasUserLink(UserLink link)
		{
			return ((link != null) && (FindUserLink(link.FromId, link.ToId) != null));
		}

		public bool HasUserLink(uint id)
		{
			var taskItem = GetTaskItem(id);

			if ((taskItem == null) || (taskItem.UserLinks == null) || !taskItem.HasUserLinks)
				return false;

			foreach (var other in Values)
			{
				if ((other.TaskId != id) && other.HasUserLink(id))
					return true;
			}

			return false;
		}

		public bool DeleteUserLink(UserLink link)
		{
			var task = GetTaskItem(link.FromId);
			
			if (task == null)
				return false;

			return task.DeleteUserLink(link);
		}

		public bool ChangeUserLinkTarget(UserLink link, uint newToId)
		{
			var fromTask = GetTaskItem(link.FromId);
			var toTask = GetTaskItem(newToId);

			return ((fromTask != null) && (toTask != null) && link.ChangeToId(newToId));
		}

		public bool HasDependency(uint id1, uint id2)
		{
			// Check one way
			var task1 = GetTaskItem(id1);

			if ((task1 != null) && task1.DependIds.Contains(id2))
				return true;

			// then the other
			var task2 = GetTaskItem(id2);

			return ((task2 != null) && task2.DependIds.Contains(id1));
		}

		public bool IsTaskLocked(uint id)
		{
			var task = GetTaskItem(id);

			return ((task == null) || task.IsLocked);
		}

		public bool GetDateRange(out DateTime from, out DateTime to)
		{
			from = to = DateTime.MinValue;

			foreach (var task in Values)
				task.MinMax(ref from, ref to);

			return (TaskItem.IsValidDate(from) && TaskItem.IsValidDate(to) && (from < to));
		}

	}


}
