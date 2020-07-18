#include <algorithm>
#include <string>
#include <list>

#include <catch2/catch.hpp>

#include "../graphw/graphw.hpp"
#include "../graphw/graphw_draw.hpp"

TEST_CASE("testing node and edge registration") {
    SECTION("input and clear label and node") {
        graphw::Graph graph;
        // Check if initial number of nodes and edges are 0
        REQUIRE(graph.number_of_nodes() == 0);
        REQUIRE(graph.number_of_edges() == 0);
        // Add 10 nodes and 9 edges
        for(int i = 0; i < 10; i++) {
            graph.add_node();
            if(i > 0) {
                // Create edge between current node and previous node
                graph.add_edge(std::to_string(i - 1), std::to_string(i));
            }
        }
        // Check number of nodes and edges
        CHECK(graph.number_of_nodes() == 10);
        CHECK(graph.number_of_edges() == 9);
        // Clear graph and require that nodes and edges are removed
        graph.clear();
        REQUIRE(graph.number_of_nodes() == 0);
        REQUIRE(graph.number_of_edges() == 0);
        // Add 10 nodes with labels but add 9 edges with node struct
        graphw::Node current_node;
        graphw::Node previous_node;
        for(int i = 0; i < 10; i++) {
            current_node = graph.add_node();
            if(i > 0) {
                graph.add_edge(previous_node, current_node);
            }
            previous_node = current_node;
        }
        // Check number of nodes and edges
        CHECK(graph.number_of_nodes() == 10);
        CHECK(graph.number_of_edges() == 9);
        // Clear graph and require that nodes and edges are removed
        graph.clear();
        REQUIRE(graph.number_of_nodes() == 0);
        REQUIRE(graph.number_of_edges() == 0);
    }
    SECTION("input duplicate label") {
        graphw::Graph graph;
        // Label to be added 10 times
        std::string label = "Foo";
        for(int i = 0; i < 10; i++) {
            graph.add_node(label);
        }
        // Check number of nodes
        CHECK(graph.number_of_nodes() == 1);
    }
    SECTION("automatically add nodes from edge creation") {
        SECTION("input label") {
            graphw::Graph graph;
            // Add edge between two labels which do not exist yet
            graph.add_edge("node1", "node2");
            // Check number of nodes
            CHECK(graph.number_of_nodes() == 2);
        }
        SECTION("input node") {
            graphw::Graph graph;
            graphw::Node node1;
            graphw::Node node2;
            node1.label = "node1";
            node2.label = "node2";
            // Add edge between two nodes which do not exist yet
            graph.add_edge(node1, node2);
            // Check number of nodes
            CHECK(graph.number_of_nodes() == 2);
        }
    }
}

TEST_CASE("testing graph path") {
    SECTION("input label") {
        graphw::Graph graph;
        // n represents number of nodes in path list
        auto n = GENERATE(0, 1, 10);
        std::list<std::string> path;
        // Add n nodes to path
        for(int i = 0; i < n; i++) {
            path.push_back(std::to_string(i));
        }
        graph.add_path(path);
        // Check number of nodes and edges
        CHECK(graph.number_of_nodes() == n);
        // Equal to max to make sure it is non-negative
        CHECK(graph.number_of_edges() == std::max(n - 1, 0));
    }
    SECTION("input node") {
        graphw::Graph graph;
        // n represents number of nodes in path list
        auto n = GENERATE(0, 1, 10);
        std::list<graphw::Node> path;
        // Add n nodes to path
        for(int i = 0; i < n; i++) {
            graphw::Node node;
            node.label = std::to_string(i);
            path.push_back(node);
        }
        graph.add_path(path);
        // Check number of nodes and edges
        CHECK(graph.number_of_nodes() == n);
        // Equal to max to make sure it is non-negative
        CHECK(graph.number_of_edges() == std::max(n - 1, 0));
    }
}

TEST_CASE("testing graph cycle") {
    SECTION("input label") {
        graphw::Graph graph;
        // n represents number of nodes in cycle list
        auto n = GENERATE(0, 1, 10);
        std::list<std::string> cycle;
        // Add n nodes to cycle
        for(int i = 0; i < n; i++) {
            cycle.push_back(std::to_string(i));
        }
        graph.add_cycle(cycle);
        // Check number of nodes and edges
        CHECK(graph.number_of_nodes() == n);
        CHECK(graph.number_of_edges() == n);
    }
    SECTION("input node") {
        graphw::Graph graph;
        // n represents number of nodes in cycle list
        auto n = GENERATE(0, 1, 10);
        std::list<graphw::Node> cycle;
        // Add n nodes to cycle
        for(int i = 0; i < n; i++) {
            graphw::Node node;
            node.label = std::to_string(i);
            cycle.push_back(node);
        }
        graph.add_cycle(cycle);
        // Check number of nodes and edges
        CHECK(graph.number_of_nodes() == n);
        CHECK(graph.number_of_edges() == n);
    }
}

TEST_CASE("testing graph encapsulation") {
    SECTION("arc diagram") {
        graphw::ArcDiagram arc_diagram;
        // Check default value
        CHECK(arc_diagram.directed() == false);
        // Set new value
        arc_diagram.set_directed(true);
        // Check new value
        CHECK(arc_diagram.directed() == true);
    }
    SECTION("circular layout") {
        graphw::CircularLayout circular_layout;
        // Check default values
        CHECK(circular_layout.directed() == false);
        CHECK(circular_layout.node_radius() == 20);
        // Set new values
        circular_layout.set_directed(true);
        circular_layout.set_node_radius(0);
        // Check new values
        CHECK(circular_layout.directed() == true);
        CHECK(circular_layout.node_radius() == 0);
    }
    SECTION("random layout") {
        graphw::RandomLayout random_layout;
        // Check default values
        CHECK(random_layout.directed() == false);
        CHECK(random_layout.node_radius() == 20);
        // Set new values
        random_layout.set_directed(true);
        random_layout.set_node_radius(0);
        // Check new values
        CHECK(random_layout.directed() == true);
        CHECK(random_layout.node_radius() == 0);
    }
    SECTION("spiral layout") {
        graphw::SpiralLayout spiral_layout;
        // Check default values
        CHECK(spiral_layout.directed() == false);
        CHECK(spiral_layout.node_radius() == 10);
        CHECK(spiral_layout.resolution() == 0.35f);
        CHECK(spiral_layout.equidistant() == false);
        // Set new values
        spiral_layout.set_directed(true);
        spiral_layout.set_node_radius(0);
        spiral_layout.set_resolution(1.0);
        spiral_layout.set_equidistant(true);
        // Check new values
        CHECK(spiral_layout.directed() == true);
        CHECK(spiral_layout.node_radius() == 0);
        CHECK(spiral_layout.resolution() == 1.0);
        CHECK(spiral_layout.equidistant() == true);
    }
}