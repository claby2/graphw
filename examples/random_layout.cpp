#include "../graphw/graphw.hpp"
#include "../graphw/graphw_draw.hpp"

int main() {
    // Construct random layout graph
    graphw::RandomLayout g;
    // Add lollipop graph to graph
    g.add_lollipop(4, 3);
    // Draw graph
    graphw::draw(g);
}