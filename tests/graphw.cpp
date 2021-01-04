#include "graphw.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Initial construction") {
    // Graphw should be empty on construction
    graphw::Graph graph;

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == 0);
    CHECK(edges == 0);
}

TEST_CASE("Add node") {
    graphw::Graph graph;
    graphw::Node node_1 = graph.add_node();
    graphw::Node node_2 = graph.add_node();

    // Assert that there are two nodes in the graph
    int expected = 2;
    int result = graph.number_of_nodes();
    REQUIRE(result == expected);

    // The nodes should be given default labels
    std::string expected_label_1 = "0";
    std::string expected_label_2 = "1";
    CHECK(node_1.label == expected_label_1);
    CHECK(node_2.label == expected_label_2);
}

TEST_CASE("Add node with duplicate label") {
    graphw::Graph graph;
    graphw::Node node_1 = graph.add_node("label");
    REQUIRE_THROWS(graph.add_node("label"));

    int expected = 1;
    int result = graph.number_of_nodes();
    REQUIRE(result == expected);
}

TEST_CASE("Add edge with labels") {
    graphw::Graph graph;
    graph.add_edge("a", "b");

    int expected = 1;
    int result = graph.number_of_edges();
    REQUIRE(result == expected);
}

TEST_CASE("Add edge with node objects") {
    graphw::Graph graph;
    graphw::Node node_1 = {0, "1"};
    graphw::Node node_2 = {1, "2"};

    graph.add_edge(node_1, node_2);
    int expected = 1;
    int result = graph.number_of_edges();
    REQUIRE(result == expected);
}

TEST_CASE("Add path with node object list") {
    graphw::Graph graph;
    int expected_nodes = 10;
    int expected_edges = 9;
    std::list<graphw::Node> nodes_list;
    // Fill nodes list
    for (int i = 0; i < expected_nodes; ++i) {
        nodes_list.push_back({i, std::to_string(i)});
    }
    graph.add_path(nodes_list);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();

    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add path with label list") {
    graphw::Graph graph;
    int expected_nodes = 10;
    int expected_edges = 9;
    std::list<std::string> label_list;
    // Fill label list
    for (int i = 0; i < expected_nodes; ++i) {
        label_list.push_back(std::to_string(i));
    }
    graph.add_path(label_list);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();

    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add cycle with label list") {
    graphw::Graph graph;
    int expected_nodes = 10;
    int expected_edges = 10;
    std::list<std::string> label_list;
    // Fill label list
    for (int i = 0; i < expected_nodes; ++i) {
        label_list.push_back(std::to_string(i));
    }
    graph.add_cycle(label_list);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();

    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add cycle with node object list") {
    graphw::Graph graph;
    int expected_nodes = 10;
    int expected_edges = 10;
    std::list<graphw::Node> nodes_list;
    // Fill nodes list
    for (int i = 0; i < expected_nodes; ++i) {
        nodes_list.push_back({i, std::to_string(i)});
    }
    graph.add_cycle(nodes_list);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();

    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add balanced tree") {
    graphw::Graph graph;
    int expected_nodes = 13;
    int expected_edges = 12;
    int children = 3;
    int height = 2;
    graph.add_balanced_tree(children, height);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add balanced tree with 0 height") {
    graphw::Graph graph;
    int children = 2;
    int height = 0;
    graph.add_balanced_tree(children, height);

    int nodes = graph.number_of_nodes();
    REQUIRE(nodes == 1);
}

TEST_CASE("Add balanced tree with 1 children") {
    graphw::Graph graph;
    int expected_nodes = 2;
    int expected_edges = 1;
    int children = 1;
    int height = 1;
    graph.add_balanced_tree(children, height);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add barbell with geometric sequence") {
    graphw::Graph graph;
    int expected_nodes = 7;
    int expected_edges = 6;
    int m1 = 2;
    int m2 = 3;
    graph.add_barbell(m1, m2);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();

    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add barbell with invalid graph properties") {
    graphw::Graph graph;
    CHECK_THROWS(graph.add_barbell(0, 1));
    CHECK_THROWS(graph.add_barbell(2, -1));
}

TEST_CASE("Add binomial tree") {
    graphw::Graph graph;
    int expected_nodes = 4;
    int expected_edges = 3;
    int order = 2;
    graph.add_binomial_tree(order);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add binomial tree with order 0") {
    graphw::Graph graph;
    int expected_nodes = 4;
    int order = 0;
    graph.add_binomial_tree(2);

    int nodes = graph.number_of_nodes();
    CHECK(nodes == expected_nodes);
}

TEST_CASE("Add complete") {
    graphw::Graph graph;
    int expected_nodes = 2;
    int expected_edges = 1;
    int n = 2;
    graph.add_complete(n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add complete with n < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_complete(-1));
}

TEST_CASE("Add complete multipartite") {
    graphw::Graph graph;
    int expected_nodes = 6;
    int expected_edges = 11;
    std::list<int> subset_sizes{1, 2, 3};
    graph.add_complete_multipartite(subset_sizes);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add circular ladder") {
    graphw::Graph graph;
    int expected_nodes = 4;
    int expected_edges = 4;
    int n = 2;
    graph.add_circular_ladder(n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add circular ladder with n == 0") {
    graphw::Graph graph;
    int expected_nodes = 0;
    int n = 0;
    graph.add_circular_ladder(n);

    int nodes = graph.number_of_nodes();
    CHECK(nodes == expected_nodes);
}

TEST_CASE("Add circular ladder with n < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_circular_ladder(-1));
}

TEST_CASE("Add circulant") {
    graphw::Graph graph;
    int expected_nodes = 2;
    int expected_edges = 4;
    int n = 2;
    std::list<int> offsets{1, 2};
    graph.add_circulant(n, offsets);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add circulant with n < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_circulant(-1, {}));
}

TEST_CASE("Add empty") {
    graphw::Graph graph;
    int expected_nodes = 5;
    int expected_edges = 0;
    int n = 5;
    graph.add_empty(n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add empty with n < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_empty(-1));
}

TEST_CASE("Add full m-ary tree") {
    graphw::Graph graph;
    int expected_nodes = 3;
    int expected_edges = 2;
    int m = 2;
    int n = 3;
    graph.add_full_mary_tree(m, n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add full m-ary tree with n < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_full_mary_tree(1, -1));
}

TEST_CASE("Add full m-ary tree with m == 0") {
    graphw::Graph graph;
    int expected_nodes = 3;
    int expected_edges = 0;
    int m = 0;
    int n = 3;
    graph.add_full_mary_tree(m, n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add ladder") {
    graphw::Graph graph;
    int expected_nodes = 6;
    int expected_edges = 7;
    int n = 3;
    graph.add_ladder(n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add ladder with n < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_ladder(-1));
}

TEST_CASE("Add lollipop") {
    graphw::Graph graph;
    int expected_nodes = 4;
    int expected_edges = 4;
    int m = 3;
    int n = 1;
    graph.add_lollipop(m, n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add lollipop with n == 0") {
    graphw::Graph graph;
    int expected_nodes = 3;
    int expected_edges = 3;
    int m = 3;
    int n = 0;
    graph.add_lollipop(m, n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add lollipop with m < 2") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_lollipop(1, 1));
}

TEST_CASE("Add lollipop with n < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_lollipop(3, -1));
}

TEST_CASE("Add star") {
    graphw::Graph graph;
    int expected_nodes = 3;
    int expected_edges = 2;
    int k = 2;
    graph.add_star(k);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add star with k < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_star(-1));
}

TEST_CASE("Add star with k == 0") {
    graphw::Graph graph;
    int expected_nodes = 1;
    int k = 0;
    graph.add_star(k);

    int nodes = graph.number_of_nodes();
    CHECK(nodes == expected_nodes);
}

TEST_CASE("Add turan") {
    graphw::Graph graph;
    int expected_nodes = 3;
    int expected_edges = 2;
    int n = 3;
    int r = 2;
    graph.add_turan(n, r);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add turan with r < 1") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_turan(0, 1));
}

TEST_CASE("Add turan with r > n") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_turan(1, 0));
}

TEST_CASE("Add wheel") {
    graphw::Graph graph;
    int expected_nodes = 3;
    int expected_edges = 5;
    int n = 3;
    graph.add_wheel(n);

    int nodes = graph.number_of_nodes();
    int edges = graph.number_of_edges();
    CHECK(nodes == expected_nodes);
    CHECK(edges == expected_edges);
}

TEST_CASE("Add whell with n < 0") {
    graphw::Graph graph;
    REQUIRE_THROWS(graph.add_wheel(-1));
}
