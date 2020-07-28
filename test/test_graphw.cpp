#include <stdlib.h>

#include <algorithm>
#include <catch2/catch.hpp>
#include <limits>
#include <list>
#include <string>

#include "../graphw/graphw.hpp"
#include "../graphw/graphw_draw.hpp"

TEST_CASE("testing node and edge registration") {
    SECTION("input and clear label and node") {
        graphw::Graph graph;
        // Check if initial number of nodes and edges are 0
        REQUIRE(graph.number_of_nodes() == 0);
        REQUIRE(graph.number_of_edges() == 0);
        // Add 10 nodes and 9 edges
        for (int i = 0; i < 10; i++) {
            graph.add_node();
            if (i > 0) {
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
        for (int i = 0; i < 10; i++) {
            current_node = graph.add_node();
            if (i > 0) {
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
        for (int i = 0; i < 10; i++) {
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

TEST_CASE("testing graph generation") {
    SECTION("add path") {
        SECTION("input label") {
            graphw::Graph graph;
            // n represents number of nodes in path list
            auto n = GENERATE(0, 1, 10);
            std::list<std::string> path;
            // Add n nodes to path
            for (int i = 0; i < n; i++) {
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
            for (int i = 0; i < n; i++) {
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
    SECTION("add cycle") {
        SECTION("input label") {
            graphw::Graph graph;
            // n represents number of nodes in cycle list
            auto n = GENERATE(0, 1, 10);
            std::list<std::string> cycle;
            // Add n nodes to cycle
            for (int i = 0; i < n; i++) {
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
            for (int i = 0; i < n; i++) {
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
    SECTION("add balanced tree") {
        graphw::Graph graph;
        // Input is in the form {children, height}
        struct Input {
            int children;
            int height;
        };
        std::vector<Input> inputs = {
            {1, 3},
            {2, 2},
            {3, 1},
            {4, 0},
            {5, -1}};
        std::vector<std::string> expected_output = {
            "0 1 \n"
            "1 0 2 \n"
            "2 1 3 \n"
            "3 2 \n",

            "0 1 2 \n"
            "1 0 3 4 \n"
            "2 0 5 6 \n"
            "3 1 \n"
            "4 1 \n"
            "5 2 \n"
            "6 2 \n",

            "0 1 2 3 \n"
            "1 0 \n"
            "2 0 \n"
            "3 0 \n",

            "0 \n",

            ""};
        for (int i = 0; i < 5; i++) {
            Input input = inputs[i];
            graph.add_balanced_tree(input.children, input.height);
            CHECK(graph.get_adjacency_list() == expected_output[i]);
            graph.clear();
        }
    }
    SECTION("add barbell") {
        graphw::Graph graph;
        // Input is in the form {m1, m2}
        struct Input {
            int m1;
            int m2;
        };
        std::vector<Input> inputs = {
            {1, 3},
            {2, 2},
            {3, 1},
            {4, 0},
            {5, -1}};
        std::vector<std::string> expected_output = {
            "",

            "0 1 \n"
            "1 0 2 \n"
            "2 3 1 \n"
            "3 2 4 \n"
            "4 5 3 \n"
            "5 4 \n",

            "0 1 2 \n"
            "1 0 2 \n"
            "2 0 1 3 \n"
            "3 2 4 \n"
            "4 5 6 3 \n"
            "5 4 6 \n"
            "6 4 5 \n",

            "0 1 2 3 \n"
            "1 0 2 3 \n"
            "2 0 1 3 \n"
            "3 0 1 2 4 \n"
            "4 5 6 7 3 \n"
            "5 4 6 7 \n"
            "6 4 5 7 \n"
            "7 4 5 6 \n",

            ""};
        for (int i = 0; i < 5; i++) {
            Input input = inputs[i];
            if (input.m1 < 2) {
                try {
                    graph.add_barbell(input.m1, input.m2);
                } catch (graphw::GraphwError& e) {
                    CHECK(std::string(e.what()) == "Invalid graph properties, m1 should be >=2");
                }
            } else if (input.m2 < 0) {
                try {
                    graph.add_barbell(input.m1, input.m2);
                } catch (graphw::GraphwError& e) {
                    CHECK(std::string(e.what()) == "Invalid graph properties, m2 should be >=0");
                }
            } else {
                graph.add_barbell(input.m1, input.m2);
                CHECK(graph.get_adjacency_list() == expected_output[i]);
            }
            graph.clear();
        }
    }
    SECTION("add binomial tree") {
        graphw::Graph graph;
        auto order = GENERATE(0, 1, 2, 3, 4);
        std::vector<std::string> expected_output = {
            "0 \n",

            "0 1 \n"
            "1 0 \n",

            "0 1 2 \n"
            "1 0 \n"
            "2 3 0 \n"
            "3 2 \n",

            "0 1 2 4 \n"
            "1 0 \n"
            "2 3 0 \n"
            "3 2 \n"
            "4 5 6 0 \n"
            "5 4 \n"
            "6 7 4 \n"
            "7 6 \n",

            "0 1 2 4 8 \n"
            "1 0 \n"
            "2 3 0 \n"
            "3 2 \n"
            "4 5 6 0 \n"
            "5 4 \n"
            "6 7 4 \n"
            "7 6 \n"
            "8 9 10 12 0 \n"
            "9 8 \n"
            "10 11 8 \n"
            "11 10 \n"
            "12 13 14 8 \n"
            "13 12 \n"
            "14 15 12 \n"
            "15 14 \n"};
        graph.add_binomial_tree(order);
        CHECK(graph.get_adjacency_list() == expected_output[order]);
        graph.clear();
    }
    SECTION("add complete") {
        graphw::Graph graph;
        auto n = GENERATE(-1, 0, 1, 2, 3);
        std::vector<std::string> expected_output = {
            "",

            "",

            "0 \n",

            "0 1 \n"
            "1 0 \n",

            "0 1 2 \n"
            "1 0 2 \n"
            "2 0 1 \n"};
        if (n == -1) {
            try {
                graph.add_complete(n);
            } catch (graphw::GraphwError& e) {
                CHECK(std::string(e.what()) == ("Negative number of nodes not valid: " + std::to_string(n)));
            }
        } else {
            graph.add_complete(n);
            CHECK(graph.get_adjacency_list() == expected_output[n + 1]);
        }
        graph.clear();
    }
    SECTION("add complete multipartite") {
        graphw::Graph graph;
        std::vector<std::list<int> > inputs = {
            {1, 2, 3, -1},          // Should throw exception due to number < 0
            {0, 0, 0, 3, 1, 2, 0},  // Zero-valued numbers should be ignored
            {1, 2, 3, 4, 5},
            {1, 3, 5, 2, 4},
            {4, 2, 5, 3, 1},
        };
        std::vector<std::string> expected_output = {
            "",

            "0 3 4 5 \n"
            "1 3 4 5 \n"
            "2 3 4 5 \n"
            "3 0 1 2 4 5 \n"
            "4 0 1 2 3 \n"
            "5 0 1 2 3 \n",

            "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 \n"
            "1 0 3 4 5 6 7 8 9 10 11 12 13 14 \n"
            "2 0 3 4 5 6 7 8 9 10 11 12 13 14 \n"
            "3 0 1 2 6 7 8 9 10 11 12 13 14 \n"
            "4 0 1 2 6 7 8 9 10 11 12 13 14 \n"
            "5 0 1 2 6 7 8 9 10 11 12 13 14 \n"
            "6 0 1 2 3 4 5 10 11 12 13 14 \n"
            "7 0 1 2 3 4 5 10 11 12 13 14 \n"
            "8 0 1 2 3 4 5 10 11 12 13 14 \n"
            "9 0 1 2 3 4 5 10 11 12 13 14 \n"
            "10 0 1 2 3 4 5 6 7 8 9 \n"
            "11 0 1 2 3 4 5 6 7 8 9 \n"
            "12 0 1 2 3 4 5 6 7 8 9 \n"
            "13 0 1 2 3 4 5 6 7 8 9 \n"
            "14 0 1 2 3 4 5 6 7 8 9 \n",

            "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 \n"
            "1 0 4 5 6 7 8 9 10 11 12 13 14 \n"
            "2 0 4 5 6 7 8 9 10 11 12 13 14 \n"
            "3 0 4 5 6 7 8 9 10 11 12 13 14 \n"
            "4 0 1 2 3 9 10 11 12 13 14 \n"
            "5 0 1 2 3 9 10 11 12 13 14 \n"
            "6 0 1 2 3 9 10 11 12 13 14 \n"
            "7 0 1 2 3 9 10 11 12 13 14 \n"
            "8 0 1 2 3 9 10 11 12 13 14 \n"
            "9 0 1 2 3 4 5 6 7 8 11 12 13 14 \n"
            "10 0 1 2 3 4 5 6 7 8 11 12 13 14 \n"
            "11 0 1 2 3 4 5 6 7 8 9 10 \n"
            "12 0 1 2 3 4 5 6 7 8 9 10 \n"
            "13 0 1 2 3 4 5 6 7 8 9 10 \n"
            "14 0 1 2 3 4 5 6 7 8 9 10 \n",

            "0 4 5 6 7 8 9 10 11 12 13 14 \n"
            "1 4 5 6 7 8 9 10 11 12 13 14 \n"
            "2 4 5 6 7 8 9 10 11 12 13 14 \n"
            "3 4 5 6 7 8 9 10 11 12 13 14 \n"
            "4 0 1 2 3 6 7 8 9 10 11 12 13 14 \n"
            "5 0 1 2 3 6 7 8 9 10 11 12 13 14 \n"
            "6 0 1 2 3 4 5 11 12 13 14 \n"
            "7 0 1 2 3 4 5 11 12 13 14 \n"
            "8 0 1 2 3 4 5 11 12 13 14 \n"
            "9 0 1 2 3 4 5 11 12 13 14 \n"
            "10 0 1 2 3 4 5 11 12 13 14 \n"
            "11 0 1 2 3 4 5 6 7 8 9 10 14 \n"
            "12 0 1 2 3 4 5 6 7 8 9 10 14 \n"
            "13 0 1 2 3 4 5 6 7 8 9 10 14 \n"
            "14 0 1 2 3 4 5 6 7 8 9 10 11 12 13 \n"};
        for (int i = 0; i < 5; i++) {
            if (i == 0) {
                try {
                    graph.add_complete_multipartite(inputs[i]);
                } catch (graphw::GraphwError& e) {
                    CHECK(std::string(e.what()) == "Negative number of nodes not valid");
                }
            } else {
                graph.add_complete_multipartite(inputs[i]);
                CHECK(graph.get_adjacency_list() == expected_output[i]);
            }
            graph.clear();
        }
    }
    SECTION("add circular ladder") {
        graphw::Graph graph;
        auto n = GENERATE(-1, 0, 1, 2, 3);
        std::vector<std::string> expected_output = {
            "",

            "",

            "0 1 \n"
            "1 0 \n",

            "0 2 1 \n"
            "1 0 3 \n"
            "2 0 3 \n"
            "3 1 2 \n",

            "0 3 1 2 \n"
            "1 0 4 2 \n"
            "2 1 5 0 \n"
            "3 0 4 5 \n"
            "4 1 3 5 \n"
            "5 2 4 3 \n",
        };
        if (n == -1) {
            try {
                graph.add_circular_ladder(n);
            } catch (graphw::GraphwError& e) {
                CHECK(std::string(e.what()) == ("Negative number of nodes not valid: " + std::to_string(n)));
            }
        } else {
            graph.add_circular_ladder(n);
            CHECK(graph.get_adjacency_list() == expected_output[n + 1]);
        }
        graph.clear();
    }
    SECTION("add circulant") {
        graphw::Graph graph;
        // Input is in the form {n, {offsets}}
        struct Input {
            int n;
            std::list<int> offsets;
        };
        std::vector<Input> inputs = {
            {-1, {1, 2, 3}},  // Should throw exception due to n < 0
            {0, {1, 2, 3}},
            {10, {1}},
            {5, {1, 2}},
            {4, {3, -2, 1}}};
        std::vector<std::string> expected_output = {
            "",

            "",

            "0 1 9 \n"
            "1 0 2 \n"
            "2 1 3 \n"
            "3 2 4 \n"
            "4 3 5 \n"
            "5 4 6 \n"
            "6 5 7 \n"
            "7 6 8 \n"
            "8 7 9 \n"
            "9 8 0 \n",

            "0 1 2 3 4 \n"
            "1 0 2 3 4 \n"
            "2 0 1 3 4 \n"
            "3 1 2 4 0 \n"
            "4 2 3 0 1 \n",

            "0 3 2 1 \n"
            "1 0 3 2 \n"
            "2 0 1 3 \n"
            "3 0 1 2 \n"};
        for (int i = 0; i < 5; i++) {
            if (i == 0) {
                try {
                    graph.add_circulant(inputs[i].n, inputs[i].offsets);
                } catch (graphw::GraphwError& e) {
                    CHECK(std::string(e.what()) == ("Negative number of nodes not valid: " + std::to_string(inputs[i].n)));
                }
            } else {
                graph.add_circulant(inputs[i].n, inputs[i].offsets);
                CHECK(graph.get_adjacency_list() == expected_output[i]);
            }
            graph.clear();
        }
    }
    SECTION("add empty graph") {
        graphw::Graph graph;
        auto n = GENERATE(-1, 0, 1, 2, 3);
        std::vector<std::string> expected_output = {
            "",

            "",

            "0 \n",

            "0 \n"
            "1 \n",

            "0 \n"
            "1 \n"
            "2 \n"};
        if (n == -1) {
            try {
                graph.add_empty(n);
            } catch (graphw::GraphwError& e) {
                CHECK(std::string(e.what()) == ("Negative number of nodes not valid: " + std::to_string(n)));
            }
        } else {
            graph.add_empty(n);
            CHECK(graph.get_adjacency_list() == expected_output[n + 1]);
        }
        graph.clear();
    }
    SECTION("add full mary tree") {
        graphw::Graph graph;
        // Input is in the form {m, n}
        struct Input {
            int m;
            int n;
        };
        std::vector<Input> inputs = {
            {0, 3},
            {2, 2},
            {4, 3},
            {2, 12},
            {8, -1}  // Should expect exception as n < 0
        };
        std::vector<std::string> expected_output = {
            "0 \n"
            "1 \n"
            "2 \n",

            "0 1 \n"
            "1 0 \n",

            "0 1 2 \n"
            "1 0 \n"
            "2 0 \n",

            "0 1 2 \n"
            "1 0 3 4 \n"
            "2 0 5 6 \n"
            "3 1 7 8 \n"
            "4 1 9 10 \n"
            "5 2 11 \n"
            "6 2 \n"
            "7 3 \n"
            "8 3 \n"
            "9 4 \n"
            "10 4 \n"
            "11 5 \n",

            ""};
        for (int i = 0; i < 5; i++) {
            Input input = inputs[i];
            if (input.n < 0) {
                try {
                    graph.add_full_mary_tree(input.m, input.n);
                } catch (graphw::GraphwError& e) {
                    CHECK(std::string(e.what()) == ("Negative number of nodes not valid: " + std::to_string(input.n)));
                }
            } else {
                graph.add_full_mary_tree(input.m, input.n);
                CHECK(graph.get_adjacency_list() == expected_output[i]);
            }
            graph.clear();
        }
    }
    SECTION("add ladder") {
        graphw::Graph graph;
        auto n = GENERATE(-1, 0, 1, 2, 3);
        std::vector<std::string> expected_output = {
            "",

            "",

            "0 1 \n"
            "1 0 \n",

            "0 2 1 \n"
            "1 0 3 \n"
            "2 0 3 \n"
            "3 1 2 \n",

            "0 3 1 \n"
            "1 0 4 2 \n"
            "2 1 5 \n"
            "3 0 4 \n"
            "4 1 3 5 \n"
            "5 2 4 \n",
        };
        if (n == -1) {
            try {
                graph.add_ladder(n);
            } catch (graphw::GraphwError& e) {
                CHECK(std::string(e.what()) == ("Negative number of nodes not valid: " + std::to_string(n)));
            }
        } else {
            graph.add_ladder(n);
            CHECK(graph.get_adjacency_list() == expected_output[n + 1]);
        }
        graph.clear();
    }
    SECTION("add lollipop") {
        graphw::Graph graph;
        // Input is in the form {m, n}
        struct Input {
            int m;
            int n;
        };
        std::vector<Input> inputs = {
            {1, 3},  // Should expect exception as m < 2
            {2, 2},
            {3, 1},
            {4, 0},
            {5, -1}  // Should expect exception as n < 0
        };
        std::vector<std::string> expected_output = {
            "",

            "0 1 \n"
            "1 0 2 \n"
            "2 1 3 \n"
            "3 2 \n",

            "0 1 2 \n"
            "1 0 2 \n"
            "2 0 1 3 \n"
            "3 2 \n",

            "0 1 2 3 \n"
            "1 0 2 3 \n"
            "2 0 1 3 \n"
            "3 0 1 2 \n"

            ""};
        for (int i = 0; i < 5; i++) {
            Input input = inputs[i];
            if (input.m < 2) {
                try {
                    graph.add_lollipop(input.m, input.n);
                } catch (graphw::GraphwError& e) {
                    CHECK(std::string(e.what()) == "Invalid graph properties, m should be >=2");
                }
            } else if (input.n < 0) {
                try {
                    graph.add_lollipop(input.m, input.n);
                } catch (graphw::GraphwError& e) {
                    CHECK(std::string(e.what()) == "Invalid graph properties, n should be >=0");
                }
            } else {
                graph.add_lollipop(input.m, input.n);
                CHECK(graph.get_adjacency_list() == expected_output[i]);
            }
            graph.clear();
        }
    }
    SECTION("add star graph") {
        graphw::Graph graph;
        auto k = GENERATE(-1, 0, 1, 2, 3);
        std::vector<std::string> expected_output = {
            "",

            "0 \n",

            "0 1 \n"
            "1 0 \n",

            "0 1 2 \n"
            "1 0 \n"
            "2 0 \n",

            "0 1 2 3 \n"
            "1 0 \n"
            "2 0 \n"
            "3 0 \n"};
        if (k == -1) {
            try {
                graph.add_star(k);
            } catch (graphw::GraphwError& e) {
                CHECK(std::string(e.what()) == ("Negative number of nodes not valid: " + std::to_string(k)));
            }
        } else {
            graph.add_star(k);
            CHECK(graph.get_adjacency_list() == expected_output[k + 1]);
        }
        graph.clear();
    }
    SECTION("add turan graph") {
        graphw::Graph graph;
        // Input is in the form {n, r}
        struct Input {
            int n;
            int r;
        };
        std::vector<Input> inputs = {
            {5, -5},  // Should expect exception as r must satisfy the condition 1 <= r <= n
            {3, 4},   // Should except exception as r must satisfy the condition 1 <= r <= n
            {6, 3},
            {8, 4},
            {13, 4},
        };
        std::vector<std::string> expected_output = {
            "",

            "",

            "0 2 3 4 5 \n"
            "1 2 3 4 5 \n"
            "2 0 1 4 5 \n"
            "3 0 1 4 5 \n"
            "4 0 1 2 3 \n"
            "5 0 1 2 3 \n",

            "0 2 3 4 5 6 7 \n"
            "1 2 3 4 5 6 7 \n"
            "2 0 1 4 5 6 7 \n"
            "3 0 1 4 5 6 7 \n"
            "4 0 1 2 3 6 7 \n"
            "5 0 1 2 3 6 7 \n"
            "6 0 1 2 3 4 5 \n"
            "7 0 1 2 3 4 5 \n",

            "0 3 4 5 6 7 8 9 10 11 12 \n"
            "1 3 4 5 6 7 8 9 10 11 12 \n"
            "2 3 4 5 6 7 8 9 10 11 12 \n"
            "3 0 1 2 6 7 8 9 10 11 12 \n"
            "4 0 1 2 6 7 8 9 10 11 12 \n"
            "5 0 1 2 6 7 8 9 10 11 12 \n"
            "6 0 1 2 3 4 5 9 10 11 12 \n"
            "7 0 1 2 3 4 5 9 10 11 12 \n"
            "8 0 1 2 3 4 5 9 10 11 12 \n"
            "9 0 1 2 3 4 5 6 7 8 \n"
            "10 0 1 2 3 4 5 6 7 8 \n"
            "11 0 1 2 3 4 5 6 7 8 \n"
            "12 0 1 2 3 4 5 6 7 8 \n"};
        for (int i = 0; i < 5; i++) {
            Input input = inputs[i];
            if (input.r < 1 || input.r > input.n) {
                try {
                    graph.add_turan(input.n, input.r);
                } catch (graphw::GraphwError& e) {
                    CHECK(std::string(e.what()) == ("r must satisfy the condition 1 <= r <= n"));
                }
            } else {
                graph.add_turan(input.n, input.r);
                CHECK(graph.get_adjacency_list() == expected_output[i]);
            }
            graph.clear();
        }
    }
    SECTION("add wheel") {
        graphw::Graph graph;
        auto n = GENERATE(-1, 0, 1, 2, 7);
        std::vector<std::string> expected_output = {
            "",

            "",

            "0 \n",

            "0 1 \n"
            "1 0 \n",

            "0 1 2 3 4 5 6 \n"
            "1 0 2 6 \n"
            "2 0 1 3 \n"
            "3 0 2 4 \n"
            "4 0 3 5 \n"
            "5 0 4 6 \n"
            "6 0 5 1 \n",
        };
        if (n == -1) {
            try {
                graph.add_wheel(n);
            } catch (graphw::GraphwError& e) {
                CHECK(std::string(e.what()) == ("Negative number of nodes not valid: " + std::to_string(n)));
            }
        } else {
            graph.add_wheel(n);
            if (n == 7) {
                CHECK(graph.get_adjacency_list() == expected_output[4]);
            } else {
                CHECK(graph.get_adjacency_list() == expected_output[n + 1]);
            }
        }
        graph.clear();
    }
}

TEST_CASE("testing density calculation") {
    SECTION("directed") {
        graphw::Graph graph;
        graph.set_directed(true);
        graph.add_balanced_tree(2, 4);
        CHECK(graph.density() == Approx(0.0322581));
    }
    SECTION("undirected") {
        graphw::Graph graph;
        graph.set_directed(false);
        graph.add_balanced_tree(2, 4);
        CHECK(graph.density() == Approx(0.0645161));
    }
}

TEST_CASE("testing degree") {
    SECTION("degree") {
        graphw::Graph graph;
        // Add nodes and edges
        graph.add_edge("0", "1");
        graph.add_node("2");
        graph.add_edge("1", "3");
        // Check existing labels
        CHECK(graph.degree("0") == 1);
        CHECK(graph.degree("1") == 2);
        CHECK(graph.degree("2") == 0);
        CHECK(graph.degree("3") == 1);
        // Check fake label
        try {
            graph.degree("fake_label");
        } catch (graphw::GraphwError& e) {
            CHECK(std::string(e.what()) == "Given label does not exist");
        }
    }
    SECTION("average degree") {
        graphw::Graph graph;
        // Add binomial tree of order 4
        graph.add_binomial_tree(4);
        // Check average degree
        CHECK(graph.average_degree() == Approx(1.8750));
    }
}

TEST_CASE("testing get neighbors") {
    graphw::Graph graph;
    // Create complete graph
    graph.add_complete(3);
    // All nodes should be connected to each other in a complete graph
    std::list<std::string> expected_output = {"1", "2"};
    CHECK(graph.get_neighbors("0") == expected_output);
    try {
        graph.get_neighbors("fake_label");
    } catch (graphw::GraphwError& e) {
        CHECK(std::string(e.what()) == "Given label does not exist");
    }
}

TEST_CASE("testing get non neighbors") {
    graphw::Graph graph;
    // Add binomial tree of order 4
    graph.add_binomial_tree(4);
    std::list<std::string> expected_output = {
        "0",
        "1",
        "2",
        "3",
        "6",
        "7",
        "8",
        "9",
        "10",
        "11",
        "12",
        "13",
        "14",
        "15"};
    CHECK(graph.get_non_neighbors("5") == expected_output);
    try {
        graph.get_non_neighbors("fake_label");
    } catch (graphw::GraphwError& e) {
        CHECK(std::string(e.what()) == "Given label does not exist");
    }
}

TEST_CASE("testing get common neighbors") {
    graphw::Graph graph;
    graph.add_complete(5);
    std::list<std::string> expected_output = {"0", "3", "4"};
    CHECK(graph.get_common_neighbors("1", "2") == expected_output);
    try {
        graph.get_common_neighbors("fake_label", "0");
    } catch (graphw::GraphwError& e) {
        CHECK(std::string(e.what()) == "Given label does not exist");
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
        CHECK(spiral_layout.resolution() == Approx(0.35));
        CHECK(spiral_layout.equidistant() == false);
        // Set new values
        spiral_layout.set_directed(true);
        spiral_layout.set_node_radius(0);
        spiral_layout.set_resolution(1.0);
        spiral_layout.set_equidistant(true);
        // Check new values
        CHECK(spiral_layout.directed() == true);
        CHECK(spiral_layout.node_radius() == 0);
        CHECK(spiral_layout.resolution() == Approx(1.0));
        CHECK(spiral_layout.equidistant() == true);
    }
    SECTION("force directed layout") {
        graphw::ForceDirectedLayout force_directed_layout;
        // Check default values
        CHECK(force_directed_layout.directed() == false);
        CHECK(force_directed_layout.node_radius() == 20);
        CHECK(force_directed_layout.iterations() == 300);
        // Set new values
        force_directed_layout.set_directed(true);
        force_directed_layout.set_node_radius(0);
        force_directed_layout.set_iterations(0);
        // Check new values
        CHECK(force_directed_layout.directed() == true);
        CHECK(force_directed_layout.node_radius() == 0);
        CHECK(force_directed_layout.iterations() == 0);
    }
}

TEST_CASE("testing graph rendering") {
    // Test graph rendering by verifying node positioning
    SECTION("arc diagram") {
        graphw::ArcDiagram arc_diagram;
        arc_diagram.add_empty(10);
        std::vector<graphw::Position> expected_output = {
            {32, 240},
            {96, 240},
            {160, 240},
            {224, 240},
            {288, 240},
            {352, 240},
            {416, 240},
            {480, 240},
            {544, 240},
            {608, 240}};
        std::vector<graphw::Position> output = graphw::render(arc_diagram);
        CHECK(output.size() == expected_output.size());
        for (int i = 0; i < expected_output.size(); i++) {
            CHECK(output[i].x == expected_output[i].x);
            CHECK(output[i].y == expected_output[i].y);
        }
    }
    SECTION("circular layout") {
        graphw::CircularLayout circular_layout;
        circular_layout.add_empty(10);
        std::vector<graphw::Position> expected_output = {
            {535, 240},
            {493, 366},
            {386, 444},
            {253, 444},
            {146, 366},
            {105, 239},
            {146, 113},
            {253, 35},
            {386, 35},
            {493, 113}};
        std::vector<graphw::Position> output = graphw::render(circular_layout);
        CHECK(output.size() == expected_output.size());
        for (int i = 0; i < expected_output.size(); i++) {
            CHECK(output[i].x == expected_output[i].x);
            CHECK(output[i].y == expected_output[i].y);
        }
    }
    SECTION("random layout") {
        // Set random seed to fixed 123
        srand(123);
        graphw::RandomLayout random_layout;
        random_layout.add_empty(10);
        std::vector<graphw::Position> expected_output = {
            {380, 26},
            {267, 343},
            {81, 474},
            {638, 451},
            {551, 157},
            {34, 306},
            {93, 82},
            {371, 447},
            {617, 451},
            {486, 463}};
        std::vector<std::pair<float, float> > random_positions;
        // Simulate first render random
        std::vector<graphw::Position> output = graphw::render_random(
            random_layout,
            random_positions,
            true);
        CHECK(output.size() == expected_output.size());
        for (int i = 0; i < expected_output.size(); i++) {
            CHECK(output[i].x == expected_output[i].x);
            CHECK(output[i].y == expected_output[i].y);
        }
    }
    SECTION("spiral layout") {
        SECTION("not equidistant") {
            graphw::SpiralLayout spiral_layout;
            // Set equidistant to false
            spiral_layout.set_equidistant(false);
            spiral_layout.add_empty(10);
            std::vector<graphw::Position> expected_output = {
                {320, 240},
                {359, 254},
                {384, 294},
                {383, 350},
                {348, 407},
                {282, 448},
                {191, 460},
                {90, 429},
                {0, 353},
                {-62, 236}};
            std::vector<graphw::Position> output = graphw::render(spiral_layout);
            CHECK(output.size() == expected_output.size());
            for (int i = 0; i < expected_output.size(); i++) {
                CHECK(output[i].x == expected_output[i].x);
                CHECK(output[i].y == expected_output[i].y);
            }
        }
        SECTION("equidistant") {
            graphw::SpiralLayout spiral_layout;
            // Set equidistant to true
            spiral_layout.set_equidistant(true);
            spiral_layout.add_empty(10);
            std::vector<graphw::Position> expected_output = {
                {493, 259},
                {488, 315},
                {465, 367},
                {427, 410},
                {379, 441},
                {325, 458},
                {268, 460},
                {212, 447},
                {161, 421},
                {118, 383}};
            std::vector<graphw::Position> output = graphw::render(spiral_layout);
            CHECK(output.size() == expected_output.size());
            for (int i = 0; i < expected_output.size(); i++) {
                CHECK(output[i].x == expected_output[i].x);
                CHECK(output[i].y == expected_output[i].y);
            }
        }
    }
    SECTION("force directed layout") {
        // Set random seed to fixed 123
        srand(123);
        graphw::ForceDirectedLayout force_directed_layout;
        force_directed_layout.add_complete(10);
        std::vector<graphw::Position> expected_output = {
            {598, 311},
            {222, 284},
            {330, 457},
            {213, 450},
            {83, 389},
            {386, 118},
            {159, 120},
            {358, 111},
            {168, 191},
            {61, 42}};
        std::vector<std::pair<float, float> > random_positions;
        // Simulate first render random
        std::vector<graphw::Position> output = graphw::render_random(
            force_directed_layout,
            random_positions,
            true);
        CHECK(output.size() == expected_output.size());
        for (int i = 0; i < expected_output.size(); i++) {
            CHECK(output[i].x == expected_output[i].x);
            CHECK(output[i].y == expected_output[i].y);
        }
    }
}

TEST_CASE("testing draw color setting") {
    // Return color struct
    auto get_color = [](uint8_t red, uint8_t green, uint8_t blue) {
        graphw::Color color = {red, green, blue};
        return color;
    };
    // Test default values
    // Background: 0x00, 0x00, 0x00
    // Edges: 0xFF, 0xFF, 0xFF
    // Nodes: 0xFF, 0xFF, 0xFF
    CHECK(graphw::background_color == get_color(0xFF, 0xFF, 0xFF));
    CHECK(graphw::edge_color == get_color(0x00, 0x00, 0x00));
    CHECK(graphw::node_color == get_color(0x00, 0x00, 0x00));
    // Set new colors
    graphw::set_background_color(200, 0xF4, 0);
    graphw::set_edge_color(200, 0, 0xF4);
    graphw::set_node_color(0, 200, 0xF4);
    // Check new colors
    CHECK(graphw::background_color == get_color(0xC8, 0xF4, 0x00));
    CHECK(graphw::edge_color == get_color(0xC8, 0x00, 0xF4));
    CHECK(graphw::node_color == get_color(0x00, 0xC8, 0xF4));
}