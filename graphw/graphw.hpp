#ifndef GRAPHW
#define GRAPHW

#include <math.h>

#include <algorithm>
#include <exception>
#include <list>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace graphw {
    struct Node {
        int id;
        std::string label;
    };

    struct GraphwError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    class Graph {
        public:
            std::vector<std::vector<Node> > graph;

            Graph(bool new_directed_ = false) 
                : directed_(new_directed_), edges(0){ }

            // Add node with given label, node will not be added if
            // the label already exists. Returns the node created.
            Node add_node(std::string label = "") {
                int id = (int)(graph.size());
                if(label.empty()) {
                    // No label was set, make label equal id
                    label = std::to_string(id);
                }
                Node node = get_node(label);
                if(labels.insert(label).second) {
                    // Label does not exist
                    // Add node to graph
                    std::vector<Node> neighbors;
                    graph.push_back(neighbors);
                    identities[label] = id;
                    labels_vector.push_back(label);
                    return node;
                }
                // Set node with empty values;
                node = {
                    0,
                    ""
                };
                return node;
            }

            // Add an edge between two nodes given two node labels
            void add_edge(std::string label1, std::string label2) {
                // Increment edges counter
                edges++;
                // Check if labels exist
                if(labels.find(label1) == labels.end()) {
                    // Label1 does not exist, add node with label1
                    add_node(label1);
                }
                if(labels.find(label2) == labels.end()) {
                    // Label2 does not exist, add node with label2
                    add_node(label2);
                }
                // Define new nodes based one label1 and label2
                Node node1 = {
                    identities[label1],
                    label1
                };
                Node node2 = {
                    identities[label2],
                    label2
                };
                graph[node1.id].push_back(node2);
                if(!directed_) {
                    graph[node2.id].push_back(node1);
                }
            }

            // Add an edge between two nodes given two node instances
            void add_edge(Node node1_, Node node2_) {
                // Increment edges counter
                edges++;
                Node node1 = node1_;
                Node node2 = node2_;
                if(!does_node_exist(node1)) {
                    node1 = add_node(node1_.label);
                }
                if(!does_node_exist(node2)) {
                    node2 = add_node(node2_.label);
                }
                graph[node1.id].push_back(node2);
                if(!directed_) {
                    graph[node2.id].push_back(node1);
                }
            }

            // Add a path given a list of labels representing nodes
            void add_path(const std::list<std::string> & labels_list) {
                int current_label = 0;
                std::string previous_label;
                for(auto const& label : labels_list) {
                    if(labels.find(label) == labels.end()) {
                        // Label does not exist, add node with label
                        add_node(label);
                    }
                    if(current_label > 0) {
                        add_edge(previous_label, label);
                    }
                    current_label++;
                    previous_label = label;
                }
            }

            // Add a path given a list of nodes
            void add_path(const std::list<Node> & nodes_list) {
                int current_node = 0;
                Node previous_node;
                for(auto const& node : nodes_list) {
                    if(!does_node_exist(node)) {
                        add_node(node.label);
                    }
                    if(current_node > 0) {
                        add_edge(previous_node, node);
                    }
                    current_node++;
                    previous_node = node;
                }
            }

            // Add a cycle given a list of labels representing nodes
            void add_cycle(const std::list<std::string> & labels_list) {
                if(labels_list.size() > 0) {
                    int current_label = 0;
                    std::string previous_label;
                    std::string first_label;
                    for(auto const& label : labels_list) {
                        if(labels.find(label) == labels.end()) {
                            // Label does not exist, add node with label
                            add_node(label);
                        }
                        if(current_label > 0) {
                            add_edge(previous_label, label);
                        } else {
                            first_label = label;
                        }
                        current_label++;
                        previous_label = label;
                    }
                    add_edge(previous_label, first_label);
                }
            }

            // Add a cycle given a list of nodes
            void add_cycle(const std::list<Node> & nodes_list) {
                if(nodes_list.size() > 0) {
                    int current_node = 0;
                    Node previous_node;
                    Node first_node;
                    for(auto const& node : nodes_list) {
                        if(!does_node_exist(node)) {
                            add_node(node.label);
                        }
                        if(current_node > 0) {
                            add_edge(previous_node, node);
                        } else {
                            first_node = node;
                        }
                        current_node++;
                        previous_node = node;
                    }
                    add_edge(previous_node, first_node);
                }
            }

            // Add a balanced tree
            void add_balanced_tree(int children, int height) {
                int initial_size = graph.size();
                // Calculate number of nodes in tree
                int node_amount;
                if(children == 1) {
                    node_amount = height + 1;
                } else {
                    // Formula for the sum of a geometric sequence
                    node_amount = (int)((1 - pow(children, height)) / (1 - children));
                }
                int current_node = initial_size;
                for(int i = initial_size; i < (initial_size + node_amount); i++) {
                    for(int j = 1; j <= children; j++) {
                        // Add edge
                        add_edge(std::to_string(i), std::to_string(current_node + j));
                    }
                    current_node += children;
                }
            }

            // Add a barbell graph
            void add_barbell(int m1, int m2) {
                if(m1 < 2) {
                    throw GraphwError("Invalid graph properties, m1 should be >=2");
                }
                if(m2 < 0) {
                    throw GraphwError("Invalid graph properties, m2 should be >=0");
                }
                int initial_size = graph.size();
                // Create clique lambda function
                auto create_clique = [&](int first_node, int m) {
                    for(int i = first_node; i < (first_node + m); i++) {
                        for(int j = i + 1; j < (first_node + m); j++) {
                            add_edge(std::to_string(i), std::to_string(j));
                        }
                    }
                };
                // Create first clique
                create_clique(initial_size, m1);
                if(m2 > 0) {
                    // Update initial size
                    initial_size = graph.size();
                    std::list<std::string> path_nodes;
                    for(int i = initial_size; i < (initial_size + m2); i++) {
                        path_nodes.push_back(std::to_string(i));
                    }
                    add_path(path_nodes);
                    // Connect first clique with path
                    add_edge(std::to_string(initial_size - 1), std::to_string(initial_size));
                }
                // Update initial size
                initial_size = graph.size();
                // Create second clique
                create_clique(initial_size, m1);
                // Connect path with second clique
                add_edge(std::to_string(initial_size - 1), std::to_string(initial_size));
            }

            // Add a binomial tree
            void add_binomial_tree(int order) {
                if(order < 1) {
                    // If order is less than 1, add a single node
                    add_node();
                } else {
                    int initial_size = graph.size();
                    // Edge represents two node ids
                    struct Edge {
                        int id1;
                        int id2;
                    };
                    // edges_vector holds the edges in the current binomial tree
                    std::vector<Edge> edges_vector;
                    int n = 1;
                    for(int i = initial_size; i < (initial_size + order); i++) {
                        // Capture current edges_vector size
                        int initial_edges_size = edges_vector.size();
                        for(int j = initial_size; j < (initial_size + initial_edges_size); j++) {
                            int u = edges_vector[j - initial_size].id1 + n;
                            int v = edges_vector[j - initial_size].id2 + n;
                            add_edge(std::to_string(u), std::to_string(v));
                            Edge edge = {u, v};
                            edges_vector.push_back(edge);
                        }
                        add_edge(std::to_string(0), std::to_string(n));
                        Edge edge = {0, n};
                        edges_vector.push_back(edge);
                        n *= 2;
                    }
                }
            }

            // Add a complete graph
            void add_complete(int n) {
                if(n < 0) {
                    throw GraphwError("Negative number of nodes not valid: " + std::to_string(n));
                } else if(n > 0) {
                    // No need to do anything if n = 0, therefore n > 0
                    if(n == 1) {
                        add_node();
                    } else {
                        int initial_size = graph.size();
                        for(int i = initial_size; i < (initial_size + n); i++) {
                            for(int j = i + 1; j < (initial_size + n); j++) {
                                add_edge(std::to_string(i), std::to_string(j));
                            }
                        }
                    }
                }
            }

            // Add a complete multipartite graph
            void add_complete_multipartite(const std::list<int> & subset_sizes) {
                int initial_size = graph.size();
                // sizes_vector is vector format of subset_sizes list for operator[]
                std::vector<int> sizes_vector;
                // Calculate amount of nodes
                int node_amount = 0;
                for(auto const& size : subset_sizes) {
                    if(size > 0) {
                        node_amount += size;
                        sizes_vector.push_back(size);
                    } else if(size < 0) {
                        // Size is negative
                        throw GraphwError("Negative number of nodes not valid");
                    }
                    // Ignore if size is 0
                }
                // Add initial nodes to preserve node ordering
                for(int i = initial_size; i < (initial_size + node_amount); i++) {
                    add_node(std::to_string(i));
                }
                // Add edges
                int current_subset = 0;
                int subset_counter = 0;
                int first_neighbor;
                for(int i = initial_size; i < (initial_size + node_amount); i++) {
                    if(subset_counter >= sizes_vector[current_subset]) {
                        current_subset++;
                        subset_counter = 0;
                    }
                    if(current_subset == sizes_vector.size() - 1) {
                        // Currently on last subset, simply add nodes
                        continue;
                    } else {
                        if(subset_counter == 0) {
                            // First node in subset
                            first_neighbor = i + sizes_vector[current_subset];
                        }
                        for(int j = first_neighbor; j < (initial_size + node_amount); j++) {
                            add_edge(std::to_string(i), std::to_string(j));
                        }
                    }
                    subset_counter++;
                }
            }

            // Add a circular ladder graph
            void add_circular_ladder(int n) {
                if(n < 0) {
                    throw GraphwError("Negative number of nodes not valid: " + std::to_string(n));
                } else if(n > 0) {
                    int initial_size = graph.size();
                    add_ladder(n);
                    add_edge(std::to_string(initial_size), std::to_string(initial_size + (n - 1)));
                    add_edge(std::to_string(initial_size + n), std::to_string(initial_size + ((2 * n) - 1)));
                }
            }

            // Add a circulant graph
            void add_circulant(int n, const std::list<int> & offsets) {
                if(n < 0) {
                    throw GraphwError("Negative number of nodes not valid: " + std::to_string(n));
                } else if(n > 0) {
                    int initial_size = graph.size();
                    // Add nodes
                    for(int i = initial_size; i < (initial_size + n); i++) {
                        add_node(std::to_string(i));
                    }
                    // Add edges
                    for(int i = initial_size; i < (initial_size + n); i++) {
                        for(auto const& offset : offsets) {
                            add_edge(std::to_string(i), std::to_string((i + abs(offset)) % n));
                        }
                    }
                }
            }

            // Add a Dorogovstev-Golstev-Mendes graph
            void add_dorogovstev_golstev_mendes(int n) {
                int initial_size = graph.size();
                // Edge represents two node ids
                struct Edge {
                    int id1;
                    int id2;
                };
                // edges_vector holds the edges in the current binomial tree
                std::vector<Edge> edges_vector;
                add_edge(std::to_string(initial_size), std::to_string(initial_size + 1));
                edges_vector.push_back({initial_size, initial_size + 1});
                if(n > 0) {
                    int current_node = initial_size + 2;
                    for(int i = (initial_size + 1); i <= (initial_size + n); i++) {
                        int initial_edges_size = edges_vector.size();
                        for(int j = initial_size; j < (initial_size + initial_edges_size); j++) {
                            int node1 = edges_vector[j].id1;
                            int node2 = edges_vector[j].id2;
                            add_edge(std::to_string(current_node), std::to_string(node1));
                            add_edge(std::to_string(current_node), std::to_string(node2));
                            Edge edge1 = {current_node, node1};
                            Edge edge2 = {current_node, node2};
                            edges_vector.push_back(edge1);
                            edges_vector.push_back(edge2);
                            current_node += 1;
                        }
                    }
                }
            }

            // Add an empty graph
            void add_empty(int n) {
                if(n < 0) {
                    throw GraphwError("Negative number of nodes not valid: " + std::to_string(n));
                }
                int initial_size = graph.size();
                // Add nodes
                for(int i = initial_size; i < (initial_size + n); i++) {
                    add_node(std::to_string(i));
                }
            }

            // Add a full m-ary tree
            void add_full_mary_tree(int m, int n) {
                if(n < 0) {
                    throw GraphwError("Negative number of nodes not valid: " + std::to_string(n));
                }
                int initial_size = graph.size();
                add_empty(n);
                int current_node = initial_size;
                for(int i = initial_size; (i < (initial_size + n)) && (current_node < n); i++) {
                    for(int j = 1; (j <= m) && ((current_node + j) < n); j++) {
                        // Add edge
                        add_edge(std::to_string(i), std::to_string(current_node + j));
                    }
                    current_node += m;
                }
            }

            // Add a ladder graph
            void add_ladder(int n) {
                if(n < 0) {
                    throw GraphwError("Negative number of nodes not valid: " + std::to_string(n));
                } else if(n > 0) {
                    int initial_size = graph.size();
                    // Add nodes
                    for(int i = initial_size; i < (initial_size + (2 * n)); i++) {
                        add_node(std::to_string(i));
                    }
                    // Add edges
                    for(int i = initial_size; i < (initial_size + n); i++) {
                        if(i > initial_size) {
                            add_edge(std::to_string(i - 1), std::to_string(i));
                        }
                        // Add edge rung
                        add_edge(std::to_string(i), std::to_string(i + n));
                    }
                    for(int i = (initial_size + n); i < (initial_size + (2 * n)); i++) {
                        if(i > (initial_size + n)) {
                            add_edge(std::to_string(i - 1), std::to_string(i));
                        }
                    }
                }
            }

            // Add a lollipop graph
            void add_lollipop(int m, int n) {
                if(m < 2) {
                    throw GraphwError("Invalid graph properties, m should be >=2");
                }
                if(n < 0) {
                    throw GraphwError("Invalid graph properties, n should be >=0");
                }
                int initial_size = graph.size();
                // Offset m and n to add additional nodes
                m += initial_size;
                n += initial_size;
                // Create clique
                for(int i = initial_size; i < m; i++) {
                    for(int j = i + 1; j < m; j++) {
                        add_edge(std::to_string(i), std::to_string(j));
                    }
                }
                if(n > 0) {
                    // Connect clique with first path node
                    add_edge(std::to_string(m - 1), std::to_string(m));
                    std::list<std::string> path_nodes;
                    for(int i = m; i < (m + n); i++) {
                        path_nodes.push_back(std::to_string(i));
                    }
                    // Add path segment
                    add_path(path_nodes);
                }
            }

            // Add a star graph
            void add_star(int k) {
                if(k < 0) {
                    throw GraphwError("Negative number of nodes not valid: " + std::to_string(k));
                } else if(k == 0) {
                    // If k is 0, simply add a node
                    add_node();
                } else {
                    int initial_size = graph.size();
                    // Add nodes
                    for(int i = (initial_size + 1); i < (initial_size + k + 1); i++) {
                        add_edge(std::to_string(initial_size), std::to_string(i));
                    }
                }
            }

            // Add a Turan graph
            void add_turan(int n, int r) {
                if(r < 1 || r > n) {
                    throw GraphwError("r must satisfy the condition 1 <= r <= n");
                }
                // Calculate subset sizes
                int subset1_size = r - (n % r);
                int subset2_size = n % r;
                // Calculate subset values
                int subset1_value = (int)(floor(n / r));
                int subset2_value = (int)(floor(n / r) + 1);
                // Add subset values to subset_sizes_list
                std::list<int> subset_sizes_list;
                for(int i = 0; i < subset1_size; i++) {
                    subset_sizes_list.push_back(subset1_value);
                }
                for(int i = 0; i < subset2_size; i++) {
                    subset_sizes_list.push_back(subset2_value);
                }
                // Add subset_sizes_list to create complete multipartite graph
                add_complete_multipartite(subset_sizes_list);
            }

            // Add a wheel graph
            void add_wheel(int n) {
                if(n < 0) {
                    throw GraphwError("Negative number of nodes not valid: " + std::to_string(n));
                }
                if(n != 0) {
                    int initial_size = graph.size();
                    // To construct wheel graph, first construct a star graph
                    add_star(n - 1);
                    if(n > 2) {
                        // If there are at least 2 nodes, create additional edges
                        // Create cycle between nodes
                        for(int i = initial_size; i < (initial_size + n); i++) {
                            if(i > initial_size) {
                                add_edge(std::to_string(i - 1), std::to_string(i));
                            }
                        }
                        // Add additional edge between last node and the node after the center node
                        add_edge(std::to_string(initial_size + n - 1), std::to_string(initial_size + 1));
                    }
                }
            }

            // Return the adjacency list of the graph with labels representing each node
            std::string get_adjacency_list(std::string delimiter = " ") {
                std::string adjacency_list;
                // Fill adjacency list
                for(int i = 0; i < graph.size(); i++) {
                    std::string line;
                    line += labels_vector[i] + delimiter;
                    for(int j = 0; j < graph[i].size(); j++) {
                        // Add neighbors to line
                        line += graph[i][j].label + delimiter;
                    }
                    adjacency_list += line + '\n';
                    line.clear();
                }
                return adjacency_list;
            }

            // Remove all nodes and edges from the graph
            void clear() {
                graph.clear();
                labels.clear();
                labels_vector.clear();
                identities.clear();
                edges = 0;
            }

            // Return whether graph is directed
            inline bool directed() {
                return directed_;
            }

            // Set the graph to be directed or not
            inline void set_directed(bool new_directed_) {
                directed_ = new_directed_;
            }

            // Return number of nodes
            inline int number_of_nodes() {
                return (int)(graph.size());
            }

            // Return number of edges
            inline int number_of_edges() {
                return edges;
            }

        protected:
            std::set<std::string> labels;
            std::vector<std::string> labels_vector;
            std::unordered_map<std::string, int> identities;
            int edges;
            bool directed_;

            // Return a node struct instance given
            // a string label
            Node get_node(std::string label) {
                return {
                    (int)(graph.size()),
                    label
                };
            }

            // Checks if given node definition matches
            // graph existing node definition
            bool does_node_exist(Node node) {
                int expected_id = identities[node.label];
                return node.id == expected_id;
            }
    };

    class ArcDiagram : public Graph {
        public:
            using Graph::Graph;
        private:
            bool directed_;
    };

    class CircularLayout : public Graph {
        public:
            CircularLayout(bool new_directed_ = false, int new_node_radius_ = 20) 
                : directed_(new_directed_), node_radius_(new_node_radius_) { }

            // Return the node radius
            inline int node_radius() {
                return node_radius_;
            }

            // Set the node radius from given int
            inline void set_node_radius(int new_node_radius_) {
                node_radius_ = new_node_radius_;
            }
            
        private:
            int node_radius_;
            bool directed_;
    };

    class RandomLayout : public Graph {
        public:
            RandomLayout(bool new_directed_ = false, int new_node_radius_ = 20) 
                : directed_(new_directed_), node_radius_(new_node_radius_) { }

            // Return the node radius
            inline int node_radius() {
                return node_radius_;
            }

            // Set the node radius from given int
            inline void set_node_radius(int new_node_radius_) {
                node_radius_ = new_node_radius_;
            }

        private:
            int node_radius_;
            bool directed_;
    };

    class SpiralLayout : public Graph {
        public:
            SpiralLayout(bool new_directed_ = false, int new_node_radius_ = 10, float new_resolution_ = 0.35, bool new_equidistant_ = false) 
                : directed_(new_directed_), node_radius_(new_node_radius_), resolution_(new_resolution_), equidistant_(new_equidistant_) { }

            // Return the node radius
            inline int node_radius() {
                return node_radius_;
            }

            // Set the node radius from given int
            inline void set_node_radius(int new_node_radius_) {
                node_radius_ = new_node_radius_;
            }

            // Return the current resolution
            inline float resolution() {
                return resolution_;
            }

            // Set the resolution from given float
            inline void set_resolution(float new_resolution_) {
                resolution_ = new_resolution_;
            } 

            // Return whether spiral layout is set to be equidistant
            inline bool equidistant() {
                return equidistant_;
            }

            // Set the spiral to be equidistant or not
            inline void set_equidistant(bool new_equidistant_) {
                equidistant_ = new_equidistant_;
            } 

        private:
            int node_radius_;
            float resolution_;
            bool equidistant_;
            bool directed_;
    };
} // namespace graphw
#endif
