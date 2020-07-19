#include "../graphw/graphw.hpp"
#include "../graphw/graphw_draw.hpp"

int main() {
    // Construct arc diagram
    graphw::ArcDiagram g;
    // Add cycle to graph
    g.add_cycle({"0", "1", "2", "3", "4"});
    // Draw graph
    graphw::draw(g);
}