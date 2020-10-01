#include <stdlib.h>

#include <chrono>
#include <exception>
#include <iostream>
#include <list>
#include <vector>

#include "../graphw/graphw.hpp"
#include "../graphw/graphw_draw.hpp"

std::vector<std::string> test_names;
std::vector<int> times;

struct BenchmarkError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Timer {
   public:
    Timer() : start_timepoint(std::chrono::high_resolution_clock::now()) {}
    ~Timer() { stop(); }
    void stop() {
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start_timepoint)
                .count();
        times.push_back(duration);
    }

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint;
};

// Call this function before every test case
void test_case(std::string name) { test_names.push_back(name); }

// Format and print the results
void print_results() {
    if (test_names.size() != times.size()) {
        throw BenchmarkError("Results misaligned");
    } else {
        std::vector<std::string> time_strings;
        // Find the largest time
        auto max_time = -1;
        int max_time_index;
        for (int i = 0; i < times.size(); i++) {
            if (times[i] > max_time) {
                max_time = times[i];
                max_time_index = i;
            }
            time_strings.push_back(std::to_string(times[i]) + "us (" +
                                   std::to_string(times[i] * 0.001) + "ms)");
        }
        int max_time_string_size = time_strings[max_time_index].size();
        // Format and print
        for (int i = 0; i < time_strings.size(); i++) {
            time_strings[i] +=
                std::string(max_time_string_size - time_strings[i].size(),
                            ' ') +
                ": ";
            std::cout << time_strings[i] << test_names[i] << '\n';
        }
    }
}

int main() {
    test_case("add_node");
    {
        graphw::Graph graph;
        Timer timer;
        for (int i = 0; i < 100000; i++) {
            graph.add_node();
        }
    }

    test_case("add_edge [label]");
    {
        graphw::Graph graph;
        graph.add_node();
        Timer timer;
        for (int i = 1; i <= 10000; i++) {
            graph.add_edge(std::to_string(i - 1), std::to_string(i));
        }
    }

    test_case("add_edge [node]");
    {
        graphw::Graph graph;
        graphw::Node previous_node = graph.add_node();
        Timer timer;
        for (int i = 1; i <= 10000; i++) {
            graphw::Node node = graph.add_node();
            graph.add_edge(previous_node, node);
            previous_node = node;
        }
    }

    test_case("add_path [label]");
    {
        graphw::Graph graph;
        std::list<std::string> labels_list;
        for (int i = 0; i < 10000; i++) {
            labels_list.push_back(std::to_string(i));
        }
        Timer timer;
        graph.add_path(labels_list);
    }

    test_case("add_path [node]");
    {
        graphw::Graph graph;
        std::list<graphw::Node> nodes_list;
        for (int i = 0; i < 10000; i++) {
            graphw::Node node = graph.add_node(std::to_string(i));
            nodes_list.push_back(node);
        }
        Timer timer;
        graph.add_path(nodes_list);
    }

    test_case("add_cycle [label]");
    {
        graphw::Graph graph;
        std::list<std::string> labels_list;
        for (int i = 0; i < 10000; i++) {
            labels_list.push_back(std::to_string(i));
        }
        Timer timer;
        graph.add_cycle(labels_list);
    }

    test_case("add_cycle [node]");
    {
        graphw::Graph graph;
        std::list<graphw::Node> nodes_list;
        for (int i = 0; i < 10000; i++) {
            graphw::Node node = graph.add_node(std::to_string(i));
            nodes_list.push_back(node);
        }
        Timer timer;
        graph.add_cycle(nodes_list);
    }

    test_case("add_balanced_tree");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_balanced_tree(2, 11);
    }

    test_case("add_barbell");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_barbell(50, 50);
    }

    test_case("add_binomial_tree");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_binomial_tree(12);
    }

    test_case("add_complete");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_complete(60);
    }

    test_case("add_complete_multipartite");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_complete_multipartite({10, 20, 20, 30});
    }

    test_case("add_circular_ladder");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_circular_ladder(2000);
    }

    test_case("add_circulant");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_circulant(1000, {10, 20, 30, 40, 50});
    }

    test_case("add_empty");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_empty(10000);
    }

    test_case("add_full_mary_tree");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_full_mary_tree(2, 5000);
    }

    test_case("add_ladder");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_ladder(2000);
    }

    test_case("add_lollipop");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_lollipop(60, 50);
    }

    test_case("add_star");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_star(10000);
    }

    test_case("add_turan");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_turan(60, 30);
    }

    test_case("add_wheel");
    {
        graphw::Graph graph;
        Timer timer;
        graph.add_wheel(5000);
    }

    print_results();
}
