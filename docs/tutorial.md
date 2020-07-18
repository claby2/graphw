# Tutorial

## Contents

+ [Installation](#installation)
+ [Creating and displaying a graph](#creating-and-displaying-a-graph)
+ [Adding nodes](#adding-nodes)
+ [Adding edges](#adding-edges)
+ [Drawing the graph](#drawing-the-graph)

## Installation

1. Download the headers in [`/graphw`](https://github.com/claby2/graphw/tree/master/graphw).
2. Either put the header files in a central location (with a specified path) or directly in your project tree.

## Create a graph

graphw features multiple graph types. We will be creating and displaying an Arc Diagram in this tutorial.

Create an empty Arc Diagram without any nodes or edges.

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::ArcDiagram g;
}
```

The `g` object manages and stores the nodes and edges of the graph.

## Adding nodes

Each node has an id (`int`) and a label (`std::string`). Each label uniquely identifies a single node. This means that adding a node with an existing label would not work and the request would be ignored.

Add two nodes with the label `0` and `1` respectively to the graph.

```cpp
g.add_node("0");
g.add_node("1");
```

Additionally, a node can be added without passing a label as an argument. If this is the case, the method would automatically assign a label. This new label would be equal to the node id as a `std::string`.

At this point, the graph `g` contains two nodes.

## Adding edges

Add an edge between two nodes given their labels.

```cpp
g.add_edge("0", "1");
```

## Drawing the graph

The same function, `draw()` can be used to draw and display any graph type. Note that this requires [SDL2](http://libsdl.org/download-2.0.php).

```cpp
graphw::draw(g);
```

[Home](./readme.md)
