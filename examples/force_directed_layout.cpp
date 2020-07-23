#include "../graphw/graphw.hpp"
#include "../graphw/graphw_draw.hpp"

int main() {
    // Construct force directed layout
    graphw::ForceDirectedLayout g;
    // Add binomial tree to graph
    g.add_binomial_tree(3);
    // Draw graph
    graphw::draw(g);
}