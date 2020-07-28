# Graph Drawing

## Contents

+ [Changing graph colors](#changing-graph-colors)
+ [Saving graph as BMP](#saving-graph-as-bmp)

## Changing graph colors

The following functions must be called before the graph is drawn, and the color of the element should be passed in RGB format.

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::CircularLayout g;
    graphw::set_background_color(0, 0, 0);
    graphw::set_edge_color(255, 255, 255);
    graphw::set_node_color(255, 255, 255);
    graphw::draw(g);
}
```

## Saving graph as BMP

The function should be called before drawing and will save the BMP file as `file.bmp`.

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::CircularLayout g;
    graphw::set_save("file.bmp");
    graphw::draw(g);
}
```

[Home](./readme.md)
