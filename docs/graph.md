# Graph

Every graph class (representing a graph type) inherits from the `Graph` class. Thus, every graph has access to its methods.

## Graph.add_node(label)

Add a node to the graph with a label.

If no argument is given, the node will be automatically assigned a label that is equal to its id.

Will throw an exception if the given label already exists.

***Parameters***

+ label (`std::string`, optional) - A string representing the label of the node to be added.

***Returns***

Node - The node that was added. If no node was added (due to duplicate labels), the node's id would be equal to 0 and the label would be an empty `std::string`.

## Graph.add_edge(node1, node2)

Add an edge between node1 and node2.

The nodes node1 and node2 will be automatically added if they do not already exist in the graph.

Both parameters can either be given as a string - representing the label of the nodes - or Node.

***Parameters***

+ node1, node2 (nodes) - The nodes can be either both a `std::string` or Node.

## Graph.clear()

Remove all nodes and edges from the graph

## Graph.directed()

Return whether the graph is directed.

This is false by default.

***Returns***

bool - Returns true if the graph is directed.

## Graph.set_directed(new_directed)

Set the graph to be directed or not.

This is false by default.

***Parameters***

+ new_directed (bool) - The new value of graph directed state.

## Graph.number_of_nodes()

Return number of nodes.

***Returns***

int - The number of nodes registered in the graph.

## Graph.number_of_edges()

Return number of edges.

***Returns***

int - The number of edges registered in the graph.

## Graph.get_adjacency_list(delimiter)

Return adjacency list as a string with the given delimiter.

The adjacency list represents the graph's labels and their relations with each other through edges.
Each row in the adjacency list represents a node and its neighbors.
Each row begins with a node label with the rest of the elements in the row representing the neighboring node labels.
Because of this, the number of rows in the adjacency list should be equal to the number of nodes in the graph.
The adjacency list is returned as one continuous `std::string` with a new line after each row.

***Parameters***

+ delimiter (`std::string`, optional) - A string representing the separation between labels in a row. The default value is `" "`.

***Returns***

`std::string` - The adjacency list of the graph

## Graph.density()

Return the density of the graph.

The density represents the number of edges compared to the maximum number of possible edges.
The calculation to get the density of a graph with m edges and n nodes is dependent on the directed state of the graph:

If the graph is directed, the density is calculated with: m / (n * (n - 1)).

If the graph is undirected, the density is calculated with: 2m / (n * (n - 1)).

***Returns***

float - The density of the graph.

## Graph.degree(label)

Return the degree of a node with a given label.

The degree of a node is equal to the number of edges that directly neighbor.

***Parameters***

+ label (`std::string`) - The label of the node.

***Returns***

int - The degree of the given node.

## Graph.average_degree()

Return the average degree of the graph.

This method returns the average degree of the graph which is calculated by adding up the degrees of all nodes and dividing it by the number of nodes in the graph.

***Returns***

int - The average degree of the graph.

## Graph.get_neighbors()

Return a list of labels representing the neighbor of a given node label.

***Parameters***

+ label (`std::string`) - The label of the node.

***Returns***

`std::list` - A list where each element represents a label of a neighboring node.

## Graph.get_non_neighbors()

Return a list of labels representing the non-neighbors of a given node label.

Non-neighbors are any nodes that are not neighbors to a given node (with the exception of the node itself).

***Parameters***

+ label (`std::string`) - The label of the node.

***Returns***

`std::list` - A list where each element represents a label of a non-neighbor.

## Graph.get_common_neighbors(label1, label2)

Return a list of labels representing the common neighbor(s) given two node labels.

***Parameters***

+ label1, label2 (`std::string`) - Labels in the graph

***Returns***

`std::list` - A list where each element represents a common neighbor.

[Home](./readme.md)
