#include "../graphw/graphw.hpp"
#include "../graphw/graphw_draw.hpp"

int main() {
    // Construct circular layout graph
    graphw::CircularLayout g;
    // Add path to graph
    g.add_path({"0", "1", "2", "3", "4"});
    // Draw graph
    graphw::draw(g);
}