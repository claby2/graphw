# Graph Generation

Graph generators in graphw allow the ability to easily create complex graphs.
The graph generation methods in the graph class work by adding a set of nodes and/or edges.
This means that multiple types of graphs can be combined in one layout.

For example, adding and drawing a complete multipartite graph can be done as follows:

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::CircularLayout g;
    // Add complete multipartite graph
    g.add_complete_multipartite({4, 2, 5, 3, 1});
    graphw::draw(g);
}
```

The following example adds a complete multipartite graph and a binomial tree to the same graph:

```cpp
graphw::CircularLayout g;
// Add complete multipartite graph
g.add_complete_multipartite({4, 2, 5, 3, 1});
// Add binomial tree graph
g.add_binomial_tree(3);
graphw::draw(g);
```

## Graph.add_balanced_tree(children, height)

Add a balanced m-ary tree with a given branching factor and height.

The number of nodes in this graph is derived from the formula for the sum of a geometric sequence.
This means that the number of nodes is equal to (1 - children^height) / (1 - children).

***Parameters***

+ children (int) - The number of children per node which is the branching factor of the tree.
+ height (int) - The height of the tree.

## Graph.add_barbell(m1, m2)

Add a barbell graph which consists of two complete graphs with a path in between.

Each clique (complete graph) has m1 nodes.
The path connecting the two cliques will consist of m2 nodes.

***Parameters***

+ m1 (int) - The size of each clique (complete graph) in the barbell graph.
+ m2 (int) - The number of nodes in the path which connects both cliques together in the barbell graph.

## Graph.add_binomial_tree(order)

Add a binomial tree with a given order.

A binomial tree with an order of k would contain 2^k nodes and 2^k - 1 edges.

***Parameters***

+ order (int) - The order of the binomial tree.

## Graph.add_complete(n)

Add a complete graph with n nodes.

In a complete graph, every node is connected to one another with a unique edge.
This graph will also have (n(n - 1)) / 2 edges.

***Parameters***

+ n (int) - The number of nodes in the complete graph.

## Graph.add_complete_multipartite(subset_sizes)

Add a complete multipartite graph with a given list of subset sizes.

***Parameters***

+ list (`std::list`) - A list of integers that each represent the number of nodes in each subset of the complete multipartite graph.

***Examples***

Creating a complete multipartite graph with the subsets of 1, 2, and 3 nodes respectively.

```cpp
// Add complete multipartite graph with subsets of 1, 2, and 3 nodes
g.add_complete_multipartite({1, 2, 3});

// Output adjacency list
g.get_adjacency_list();

// Output:

// 0 1 2 3 4 5
// 1 0 3 4 5
// 2 0 3 4 5
// 3 0 1 2
// 4 0 1 2
// 5 0 1 2
```

## Graph.add_circular_ladder(n)

Add a circular ladder graph with a length of n.

This method calls upon the `add_ladder(n)` method with two additional edges.

***Parameters***

+ n (int) - The length of the circular ladder graph.

## Graph.add_circulant(n, offsets)

Add a circulant graph with n nodes given a list of offset values.

***Parameters***

+ n (int) - The number of nodes in the circulant graph.
+ offsets (list) - A list of integers which includes the node offsets.

***Examples***

Adding a circulant graph with 5 nodes and offsets {1, 2, 3} can be done as follows:

```cpp
// Add a circulant graph with 5 nodes and offsets {1, 2, 3}
g.add_circulant(5, {1, 2, 3});
```

## Graph.add_empty(n)

Add an empty graph with no edges and n nodes.

***Parameters***

+ n (int) - The number of nodes in the empty graph.

## Graph.add_full_mary_tree(m, n)

Add a rooted m-ary tree where each node has m children.
The tree is also constrained with a maximum number of n nodes.

***Parameters***

+ m (int) - The number of children per node which is the branching factor of the tree.
+ n (int) - The number of nodes in the tree.

## Graph.add_ladder(n)

Add a planar ladder graph with 2n nodes and 3n - 2 edges.

***Parameters***

+ n (int) - The length of the ladder.

## Graph.add_lollipop(m, n)

Add a lollipop graph that combines a clique of size m (complete graph) and a path of size n.

***Parameters***

+ m (int) - The size of the clique (complete graph) in the lollipop graph.
+ n (int) - The size of the path in the lollipop graph.

## Graph.add_star(k)

Add a star graph that includes k + 1 nodes.

The star contains a single-center node connected to k outer nodes.

***Parameters***

+ k (int) - The number of outer nodes in the star graph.

## Graph.add_turan(n, r)

Add a Turán graph with n nodes.

The Turán graph is constructed by forming a complete multipartite graph with calculated subset sizes and values derived from n and r.
The graph would contain r - (n % r) subsets with the size value of floor(n / r) and n % r subsets with the size value of floor((n / r) + 1).

***Parameters***

+ n (int) - The number of nodes in the Turán graph.
+ r (int) - The number of partitions (subsets) in the Turán graph.

## Graph.add_wheel(n)

Add a wheel graph that connects a node to all other nodes in a cycle.

The method works by first constructing a star graph through `add_star(n - 1)`.

***Parameters***

+ n (int) - The number of nodes in the wheel graph.

[Home](./readme.md)
