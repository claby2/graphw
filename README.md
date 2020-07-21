# graphw

graphw is a C++ library for graph drawing and network analysis. The library implements various algorithms to construct, analyze, and model complex networks.

## Getting Started

### Dependencies

Install [SDL2](http://libsdl.org/download-2.0.php) to draw graphs.

### Installation

1. Download the headers in [`graphw/`](https://github.com/claby2/graphw/tree/master/graphw).
2. Either put the header files in a central location (with a specified path) or directly in your project tree.

Documentation can be found in [`docs/`](./docs/readme.md).

## Examples

### Basic example

Here is a simple example which constructs and models a graph in a circular layout:

```cpp
#include <graphw/graphw.hpp>
#include <graphw/graphw_draw.hpp>

int main() {
    graphw::CircularLayout g;
    g.add_edge("a", "b");
    g.add_edge("b", "c");
    g.add_edge("c", "a");
    graphw::draw(g);
}
```

### Additional examples

More examples can be found in [`examples/`](https://github.com/claby2/graphw/tree/master/examples).

To build the examples:

```console
$ make build-examples
```

## Benchmarking

Benchmarking does not require any external dependencies.

Compile with:

```console
$ make benchmark-compile
```

Benchmark with:

```console
$ make benchmark
```

## Testing

Testing requires [Catch2](https://github.com/catchorg/Catch2/).

Compile with:

```console
$ make test-compile
```

Test with:

```console
$ make test
```
