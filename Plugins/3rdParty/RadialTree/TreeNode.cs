using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;

namespace RadialTree
{
    public class TreeNode<T>
    {
        public delegate bool TraversalDataDelegate(T data);
        public delegate bool TraversalNodeDelegate(TreeNode<T> node);

        private readonly T _data;
        private readonly int _level;
        private readonly List<TreeNode<T>> _children;
        private TreeNode<T> _parent;
        private Point _point = new Point(0, 0);

        public TreeNode(T data)
        {
            _data = data;
            _level = 0;
            _children = new List<TreeNode<T>>();
        }

        public TreeNode(T data, TreeNode<T> parent) : this(data)
        {
            _parent = parent;
            _level = _parent != null ? _parent.Level + 1 : 0;
        }

        public TreeNode(T data, List<TreeNode<T>> children, int level) : this(data)
        {
            _children = children;
            _level = level;

            if (_children != null)
                foreach (var child in _children)
                    child.Parent = this;
        }

        /// <summary>
        /// The nodes level within the tree.
        /// </summary>
        public int Level { get { return _level; } }

        /// <summary>
        /// Number of children the node has.
        /// </summary>
        public int Count { get { return _children.Count; } }

        /// <summary>
        /// Whether the node is the root of the tree.
        /// </summary>
        public bool IsRoot { get { return _parent == null; } }

        /// <summary>
        /// Whether the node is a leaf with no children.
        /// </summary>
        public bool IsLeaf { get { return _children.Count == 0; } }

        /// <summary>
        /// The child nodes of the tree.
        /// </summary>
        public List<TreeNode<T>> Children { get { return _children; } }

        /// <summary>
        /// Object/Data assigned to the node.
        /// </summary>
        public T Data { get { return _data; } }

        /// <summary>
        /// The parent of the node.
        /// </summary>
        public TreeNode<T> Parent { get { return _parent; } private set { _parent = value; } }

        /// <summary>
        /// The position of the node (Used for when drawing the tree).
        /// </summary>
        public Point Point { get { return _point; } set { _point = value; } }

        /// <summary>
        /// Find a node that is below this node (A child node, or child of the child).
        /// </summary>
        /// <returns>Match node if found.</returns>
        public TreeNode<T> FindTreeNode(Func<TreeNode<T>, bool> predicate)
        {
            var node = _children.FirstOrDefault(predicate);
            if (node != null) return node;

            foreach (var checkNode in _children)
            {
                node = checkNode.FindTreeNode(predicate);
                if (node != null) return node;
            }

            return null;
        }

        /// <summary>
        /// Finds every node that is below this node which matches.
        /// </summary>
        /// <returns>Matched nodes if found.</returns>
        public List<TreeNode<T>> FindTreeNodes(Func<TreeNode<T>, bool> predicate)
        {
            var nodes = _children.Where(predicate).ToList();

            foreach (var checkNode in _children)
            {
                nodes.AddRange(checkNode.FindTreeNodes(predicate));
            }

            return nodes;
        }

        public TreeNode<T> FindInChildren(T data)
        {
            int i = 0, l = Count;
            for (; i < l; ++i)
            {
                TreeNode<T> child = _children[i];
                if (child.Data.Equals(data)) return child;
            }

            return null;
        }

        /// <summary>
        /// Add a new child node.
        /// </summary>
        /// <returns>The newly created child node.</returns>
        public TreeNode<T> AddChild(T value)
        {
            TreeNode<T> node = new TreeNode<T>(value, this);
            _children.Add(node);

            return node;
        }

        /// <summary>
        /// Attempts to remove the specified node.
        /// </summary>
        /// <param name="node">Node to remove.</param>
        /// <returns>true if item successfully removed.</returns>
        public bool RemoveChild(TreeNode<T> node)
        {
            return _children.Remove(node);
        }

        public void Traverse(TraversalDataDelegate handler)
        {
            if (handler(_data))
            {
                int i = 0, l = Count;
                for (; i < l; ++i) _children[i].Traverse(handler);
            }
        }

        public void Traverse(TraversalNodeDelegate handler)
        {
            if (handler(this))
            {
                int i = 0, l = Count;
                for (; i < l; ++i) _children[i].Traverse(handler);
            }
        }

        public bool HasChild(T data)
        {
            return FindInChildren(data) != null;
        }

        public TreeNode<T> this[int key]
        {
            get { return _children[key]; }
        }

        public void Clear()
        {
            _children.Clear();
        }
	}
}
