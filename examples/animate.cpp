#include "graphw.hpp"
#include "graphw_draw.hpp"

int main() {
    // Construct force directed layout
    graphw::CircularLayout g;
    // Add two turan graphs
    g.add_turan(26, 8);
    g.add_turan(13, 4);
    // Draw graph
    graphw::animate(g);
}
