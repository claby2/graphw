# Layouts

## Contents

+ [Arc Diagram](#arc-diagram)
+ [Circular Layout](#circular-layout)
+ [Random Layout](#random-layout)
+ [Spiral Layout](#spiral-layout)

graphw supports multiple graph types. All graph layouts can be either directed or undirected.

## Arc Diagram

Position nodes in an arc diagram style.

***Example***

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::ArcDiagram g;
    g.set_directed(true);
    graphw::draw(g);
}
```

## Circular Layout

Position nodes in a circle

***Additional methods and properties***

+ node radius (int, default 20) - The radius of each node.

Node radius can be set with `g.set_node_radius(new_node_radius)`.

Node radius can be returned with `g.node_radius()`.

***Example***

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::CircularLayout g;
    g.set_directed(true);
    g.set_node_radius(5);
    graphw::draw(g);
}
```

## Random Layout

Position nodes randomly.

***Additional methods and properties***

+ node radius (int, default 20) - The radius of each node.

Node radius can be set with `g.set_node_radius(new_node_radius)`.

Node radius can be returned with `g.node_radius()`.

***Example***

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::RandomLayout g;
    g.set_directed(true);
    g.set_node_radius(5);
    graphw::draw(g);
}
```

## Spiral Layout

Position nodes in a spiral.

***Additional methods and properties***

+ node radius (int, default 20) - The radius of each node.

node radius can be set with `g.set_node_radius(new_node_radius)`.

node radius can be returned with `g.node_radius()`.

+ resolution (float, default 0.35) - The compactness of the layout. Lower values would result in more compact layouts.

resolution can be set with `g.set_resolution(new_resolution)`.

resolution can be returned with `g.resolution()`.

+ equidistant (bool, default false) - If true, all nodes will have an equal distance between each other.

equidistant can be set with `g.set_equidistant(new_equidistant)`.

equidistant can be returned with `g.equidistant()`.

***Example***

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::SpiralLayout g;
    g.set_directed(true);
    g.set_node_radius(5);
    g.set_resolution(0.5);
    g.set_equidistant(true);
    graphw::draw(g);
}
```

[Home](./readme.md)
