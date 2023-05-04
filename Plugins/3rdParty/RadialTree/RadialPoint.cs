namespace RadialTree
{
    public class RadialPoint<T>
    {
        public TreeNode<T> Node { get; set; }

        public Point Point { get; set; }

        public Point ParentPoint { get; set; }
    }
}
