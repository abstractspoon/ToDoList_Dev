using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace DetectiveUIExtension
{
	public class TaskNode
	{
		// Data
		public string Title { get; private set; }
		public string ImagePath { get; private set; }
		public Color TextColor { get; private set; }

		public bool HasIcon { get; private set; }
		public bool IsFlagged { get; private set; }
		public bool IsParent { get; private set; }
		public bool IsTopLevel { get; private set; }
		public bool SomeSubtasksDone { get; private set; }
		public bool IsLocked { get; private set; }

		public uint UniqueId { get; private set; }
		public uint ParentId { get; private set; }

		public List<uint> ChildIds { get; private set; }
		public List<uint> DependIds { get; private set; }
		public List<uint> UserLinkIds { get; private set; }

		private bool Done;
		private bool GoodAsDone;

		public Point UserPosition;
		public Image Image;

		// -----------------------------------------------------------------

		public TaskNode()
		{
		}

		public TaskNode(Task task, string metaDataKey)
		{
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
			UniqueId = task.GetID();

			ChildIds = new List<uint>();
			DependIds = task.GetLocalDependency();
			UserLinkIds = null;

			UpdateImage(task);
			DecodeMetaData(task.GetMetaDataValue(metaDataKey));
		}

		public override string ToString()
		{
			return Title;
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

			if (UserLinkIds?.Count > 0)
			{
				metaData = metaData + string.Join(",", UserLinkIds);
			}

			return metaData;
		}

		public void DecodeMetaData(string metaData)
		{
			UserPosition = new Point(0, 0);
			UserLinkIds = new List<uint>();

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
					string[] linkIds = parts[1].Split(',');

					foreach (var linkId in linkIds)
					{
						uint id;

						if (uint.TryParse(linkId, out id))
							UserLinkIds.Add(id);
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
				catch (Exception e)
				{
					// keep going
				}
			}
		}

		public bool Update(Task task)
		{
			if (task.GetID() != UniqueId)
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

			if (task.IsAttributeAvailable(Task.Attribute.MetaData))
			{
				// TODO
			}

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true);
			GoodAsDone = task.IsGoodAsDone();

			Debug.Assert(task.GetParentID() == ParentId);
			IsTopLevel = (task.GetParentID() == 0);

			return true;
		}
	}

	// ------------------------------------------------------------

	public class TaskNodes : Dictionary<uint, TaskNode>
	{
		public TaskNode GetNode(uint uniqueId)
		{
			TaskNode item = null;
			TryGetValue(uniqueId, out item);

			return item;
		}

		public bool DeleteNode(uint uniqueId)
		{
			if (!ContainsKey(uniqueId))
				return false;

			Remove(uniqueId);
			return true;
		}

		public bool AddNode(TaskNode node)
		{
			if (ContainsKey(node.UniqueId))
				return false;

			Add(node.UniqueId, node);
			return true;
		}
	}


}

