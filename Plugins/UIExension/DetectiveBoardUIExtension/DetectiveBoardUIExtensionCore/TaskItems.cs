using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace DetectiveBoardUIExtension
{
	public class UserLink
	{
		public enum EndArrows
		{
			None,
			Start,
			Finish,
			Both
		}

		// ---------------------------------------------------

		static private char[] Delimiter = new char[] {':'};

		// ---------------------------------------------------

		private Color m_Color = Color.Empty;
		private int m_Thickness = 1;

		// ---------------------------------------------------

		public static Color DefaultColor = Color.Red;

		public uint FromId { get; private set; } = 0;
		public uint ToId { get; private set; } = 0;

		public Color Color
		{
			get { return (m_Color == Color.Empty) ? DefaultColor : m_Color; }
			set { m_Color = value; }
		} 

		public int Thickness
		{
			get { return m_Thickness; }
			set { m_Thickness = Math.Min(Math.Max(1, value), 5); }
		}

		public EndArrows Arrows = EndArrows.None;
		public string Label = string.Empty;
		public string Type = string.Empty;

		public UserLink(uint fromId, uint toId)
		{
			Debug.Assert((fromId != 0) && (toId != 0) && (fromId != toId));

			FromId = fromId;
			ToId = toId;
		}

		public bool ChangeToId(uint toId)
		{
			if ((toId == 0) || (toId == FromId))
				return false;

			ToId = toId;
			return true;
		}

		public bool IdsMatch(uint fromId, uint toId)
		{
			return ((FromId == fromId) && (ToId == toId));
		}

		public bool IdsMatch(UserLink other)
		{
			if (other == null)
				return false;

			return ((FromId == other.FromId) && (ToId == other.ToId));
		}

		public override string ToString()
		{
			return Encode();
		}

		const int IdIndex = 0;
		const int ColorIndex = 1;
		const int ThicknessIndex = 2;
		const int ArrowsIndex = 3;
		const int LabelIndex = 4;
		const int TypeIndex = 5;

		public static bool TryParse(string input, uint taskId, out UserLink link)
		{
			link = null;
			string[] parts = input.Split(Delimiter);

			if (parts.Count() <= ArrowsIndex)
				return false;

			uint toId;
			int argb, thickness, arrows;
			string text, type;

			if (!uint.TryParse(parts[IdIndex], out toId) ||
				!int.TryParse(parts[ColorIndex], out argb) ||
				!int.TryParse(parts[ThicknessIndex], out thickness) ||
				!int.TryParse(parts[ArrowsIndex], out arrows))
			{
				return false;
			}

			// some validation
			if ((toId <= 0) || (thickness < 0) || (arrows < 0) || (arrows >= 4))
				return false;

			link = new UserLink(taskId, toId);
			link.Color = ((argb == 0) ? Color.Empty : Color.FromArgb(argb));
			link.Thickness = ((thickness == 0) ? 1 : thickness);
			link.Arrows = (EndArrows)arrows;

			if (parts.Count() > LabelIndex)
				link.Label = parts[LabelIndex];

			if (parts.Count() > TypeIndex)
				link.Type = parts[TypeIndex];

			return true;
		}

		public string Encode()
		{
			return string.Format("{0}:{1}:{2}:{3}:{4}:{5}", 
								ToId.ToString(), 
								Color.ToArgb(), 
								Thickness.ToString(), 
								((int)Arrows).ToString(),
								Label,
								Type);
		}
	}

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

		public uint TaskId { get; private set; }
		public uint ParentId { get; private set; }

		public List<uint> ChildIds { get; private set; }
		public List<uint> DependIds { get; private set; }
		public List<UserLink> UserLinks { get; private set; }

		private bool Done;
		private bool GoodAsDone;

		public Point UserPosition;
		public bool HasUserPosition { get { return (UserPosition != NullPoint); } }
		public Image Image;

		// -----------------------------------------------------------------

		public TaskItem()
		{
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

			ParentId = task.GetParentID();
			TaskId = task.GetID();

			ChildIds = new List<uint>();
			DependIds = task.GetLocalDependency();
			UserLinks = null;

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

			if (UserLinks?.Count > 0)
			{
				metaData = metaData + string.Join(",", UserLinks);
			}

			return metaData;
		}

		public void DecodeMetaData(string metaData)
		{
			UserPosition = NullPoint;
			UserLinks = new List<UserLink>();

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
							UserLinks.Add(link);
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
			return UserLinks?.Find(x => (x.ToId == toId));
		}

		public bool HasUserLink(uint toId)
		{
			return (FindUserLink(toId) != null);
		}

		public bool DeleteUserLink(UserLink link)
		{
			if (UserLinks.Remove(link))
				return true;

			Debug.Assert(FindUserLink(link.ToId) == null);
			return false;
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
							break;
						}
					}
				}
				catch (Exception /*e*/)
				{
					// keep going
				}
			}
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

			if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
				Done = task.IsDone();

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
	}

	// ------------------------------------------------------------

	public class TaskItems : Dictionary<uint, TaskItem>
	{
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

		public UserLink FindUserLink(uint id1, uint id2, bool andReverse)
		{
			var link = GetTaskItem(id1)?.FindUserLink(id2);

			if ((link == null) && andReverse)
				link = GetTaskItem(id2)?.FindUserLink(id1);

			return link;
		}

		public bool HasUserLink(uint id1, uint id2, bool andReverse)
		{
			return (FindUserLink(id1, id2, andReverse) != null);
		}

		public bool HasUserLink(uint id)
		{
			if (GetTaskItem(id)?.UserLinks?.Count > 0)
				return true;

			foreach (var taskItem in Values)
			{
				if ((taskItem.TaskId != id) && taskItem.HasUserLink(id))
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

		public bool AddUserLink(UserLink link)
		{
			var fromTask = GetTaskItem(link.FromId);
			var toTask = GetTaskItem(link.ToId);

			if ((fromTask == null) || (toTask == null))
				return false;

			fromTask.UserLinks.Add(link);
			return true;
		}

		public bool IsTaskLocked(uint id)
		{
			var task = GetTaskItem(id);

			return ((task == null) || task.IsLocked);
		}

	}


}

