
# RadialTree
A **C# Tree Structure** &amp; **Radial Tree layout algorithm.** 

The package is available via NuGet. For Unity, download the source. 

## How to use

### Tree Data Structure
#### Basic tree setup
```csharp
TreeNode<string> treeRoot = new TreeNode<string>("Example Data (Root)");
treeRoot.AddChild("FirstChild");
treeRoot.AddChild("SecondChild");
```
For the further functions - see the TreeNode source file.

### Radial Tree Layout

```csharp
var listOutput = new List<RadialPoint<string>>();
RadialPositions(tree, 0, 2 * Math.PI, 1, 1, listOutput);
```
The position for each node is held in the TreeNode as well as the list output. 

Iterating through the list output, placing the nodes and drawing lines between child/parent nodes - you can render a radial layout.

![Example radial tree layout](https://raw.githubusercontent.com/WilliamApted/RadialTree/main/Images/BasicExample.jpg)

## Contributing
If you would like to contribute, awesome, just create a PR.
