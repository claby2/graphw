# Graph Drawing

## Contents

+ [Changing graph colors](#changing-graph-colors)
+ [Saving graph as BMP](#saving-graph-as-bmp)
+ [Animation](#animation)

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

## Animation

Animating the graph procedurally visualizes the neighbors of each node consecutively.

In each render during the animation, a node's neighbors are added and rendered to the screen.

Press `r` to restart the animation at any time.

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::CircularLayout g;
    graphw::animate(g);
}
```

***Parameters***

+ g (Graph) - The graph to be animated.
+ width (int, optional) - The width of the window. The default value is `window_width` which is 640.
+ height (int, optional) - The height of the window. The default value is `window_height` which is 480.
+ fps (float, fps) - The frame rate (frames per second) of the animation. The default value is 60.0.

[Home](./readme.md)
