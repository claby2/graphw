# Graph

Every graph class (representing a graph type) inherits from the `Graph` class. Thus, every graph has access to its methods.

## Graph.add_node(label)

Add a node to the graph with a label.

If no argument is given, the node will be automatically assigned a label that is equal to its id.

If the label already exists, no node will be added.

***Parameters***

+ label (`std::string`, optional) - A string representing the label of the node to be added.

### Returns

Node - The node that was added. If no node was added (due to duplicate labels), the node's id would be equal to 0 and the label would be an empty `std::string`.

### Return type

Node

## Graph.add_edge(node1, node2)

Add an edge between node1 and node2.

The nodes node1 and node2 will be automatically added if they do not already exist in the graph.

Both parameters can either be given as a string - representing the label of the nodes - or Node.

***Parameters***

+ node1, node2 (nodes) - The nodes can be either both a `std::string` or Node`

## Graph.add_path(list)

Add a path to the graph.

***Parameters***

+ list (`std::list`) - A list of nodes which strictly holds either type `std::string` or Node. A path will be constructed from the nodes in the order they are given and added to the graph if they do not already exist.

***Examples***

```cpp
graphw::ArcDiagram g;
g.add_path({"0", "1", "2"});
```

## Graph.add_cycle(list)

Add a cycle to the graph.

***Parameters***

+ list (`std::list`) - A list of nodes which strictly holds either type `std::string` or Node. A cycle will be constructed from the nodes in the order they are given and added to the graph if they do not already exist.

***Examples***

```cpp
graphw::ArcDiagram g;
g.add_cycle({"0", "1", "2"});
```

## Graph.clear()

Remove all nodes and edges from the graph

## Graph.directed()

Return whether the graph is directed.

This is false by default.

## Graph.set_directed(new_directed)

Set the graph to be directed or not.

This is false by default.

***Parameters***

+ new_directed (bool) - The new value of graph directed state.

## Graph.number_of_nodes()

Return number of nodes.

## Graph.number_of_edges()

Return number of edges.

[Home](./readme.md)
