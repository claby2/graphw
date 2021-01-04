#ifndef GRAPHW
#define GRAPHW

#include <math.h>

#include <algorithm>
#include <exception>
#include <list>
#include <queue>
#include <set>
#include <stdexcept>
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

    Graph(bool new_directed_ = false) : edges(0), directed_(new_directed_) {}

    // Add node with given label, node will not be added if
    // the label already exists. Returns the node created.
    Node add_node(std::string label = "") {
        int id = (int)(graph.size());
        if (label.empty()) {
            // No label was set, make label equal id
            label = std::to_string(id);
        }
        Node node = get_node(label);
        if (labels.insert(label).second) {
            // Label does not exist
            // Add node to graph
            std::vector<Node> neighbors;
            graph.push_back(neighbors);
            identities[label] = id;
            labels_vector.push_back(label);
        } else {
            // Node already exists
            throw GraphwError("Could not add node with duplicate label");
        }
        return node;
    }

    // Add an edge between two nodes given two node labels
    void add_edge(std::string label1, std::string label2) {
        int nodes_created = 0;
        // Increment edges counter
        edges++;
        // Check if labels exist
        if (labels.find(label1) == labels.end()) {
            // Label1 does not exist, add node with label1
            add_node(label1);
            nodes_created++;
        }
        if (labels.find(label2) == labels.end()) {
            // Label2 does not exist, add node with label2
            add_node(label2);
            nodes_created++;
        }
        // Check if edge already exists if both labels existed before
        if (nodes_created > 0 || (!are_neighbors(label1, label2))) {
            // Define new nodes based one label1 and label2
            Node node1 = {identities[label1], label1};
            Node node2 = {identities[label2], label2};
            graph[node1.id].push_back(node2);
            if (!directed_) {
                graph[node2.id].push_back(node1);
            }
        }
    }

    // Add an edge between two nodes given two node instances
    void add_edge(Node node1_, Node node2_) {
        int nodes_created = 0;
        // Increment edges counter
        edges++;
        Node node1 = node1_;
        Node node2 = node2_;
        if (!does_node_exist(node1)) {
            node1 = add_node(node1_.label);
            nodes_created++;
        }
        if (!does_node_exist(node2)) {
            node2 = add_node(node2_.label);
            nodes_created++;
        }
        // Check if edge already exists if both labels existed before
        if (nodes_created > 0 || (!are_neighbors(node1.label, node2.label))) {
            graph[node1.id].push_back(node2);
            if (!directed_) {
                graph[node2.id].push_back(node1);
            }
        }
    }

    // Add a path given a list of labels representing nodes
    void add_path(const std::list<std::string>& labels_list) {
        int current_label = 0;
        std::string previous_label;
        for (auto const& label : labels_list) {
            if (labels.find(label) == labels.end()) {
                // Label does not exist, add node with label
                add_node(label);
            }
            if (current_label > 0) {
                add_edge(previous_label, label);
            }
            current_label++;
            previous_label = label;
        }
    }

    // Add a path given a list of nodes
    void add_path(const std::list<Node>& nodes_list) {
        int current_node = 0;
        Node previous_node;
        for (auto const& node : nodes_list) {
            if (!does_node_exist(node)) {
                add_node(node.label);
            }
            if (current_node > 0) {
                add_edge(previous_node, node);
            }
            current_node++;
            previous_node = node;
        }
    }

    // Add a cycle given a list of labels representing nodes
    void add_cycle(const std::list<std::string>& labels_list) {
        if (labels_list.size() > 0) {
            int current_label = 0;
            std::string previous_label;
            std::string first_label;
            for (auto const& label : labels_list) {
                if (labels.find(label) == labels.end()) {
                    // Label does not exist, add node with label
                    add_node(label);
                }
                if (current_label > 0) {
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
    void add_cycle(const std::list<Node>& nodes_list) {
        if (nodes_list.size() > 0) {
            int current_node = 0;
            Node previous_node;
            Node first_node;
            for (auto const& node : nodes_list) {
                if (!does_node_exist(node)) {
                    add_node(node.label);
                }
                if (current_node > 0) {
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
        if (height == 0) {
            add_node();
        } else if (children == 1) {
            node_amount = height + 1;
            for (int i = initial_size; i < (initial_size + node_amount); i++) {
                if (i > initial_size) {
                    add_edge(std::to_string(i - 1), std::to_string(i));
                }
            }
        } else {
            // Formula for the sum of a geometric sequence
            node_amount =
                (int)((1 - pow(children, height + 1)) / (1 - children));
            add_full_mary_tree(children, node_amount);
        }
    }

    // Add a barbell graph
    void add_barbell(int m1, int m2) {
        if (m1 < 2) {
            throw GraphwError("Invalid graph properties, m1 should be >=2");
        }
        if (m2 < 0) {
            throw GraphwError("Invalid graph properties, m2 should be >=0");
        }
        int initial_size;
        // Create first clique
        add_complete(m1);
        if (m2 > 0) {
            // Update initial size
            initial_size = graph.size();
            std::list<std::string> path_nodes;
            for (int i = initial_size; i < (initial_size + m2); i++) {
                path_nodes.push_back(std::to_string(i));
            }
            add_path(path_nodes);
            // Connect first clique with path
            add_edge(std::to_string(initial_size - 1),
                     std::to_string(initial_size));
        }
        // Update initial size
        initial_size = graph.size();
        // Create second clique
        add_complete(m1);
        // Connect path with second clique
        add_edge(std::to_string(initial_size - 1),
                 std::to_string(initial_size));
    }

    // Add a binomial tree
    void add_binomial_tree(int order) {
        if (order < 1) {
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
            for (int i = initial_size; i < (initial_size + order); i++) {
                // Capture current edges_vector size
                int initial_edges_size = edges_vector.size();
                for (int j = initial_size;
                     j < (initial_size + initial_edges_size); j++) {
                    int u = edges_vector[j - initial_size].id1 + n;
                    int v = edges_vector[j - initial_size].id2 + n;
                    add_edge(std::to_string(u), std::to_string(v));
                    edges_vector.push_back({u, v});
                }
                add_edge(std::to_string(0), std::to_string(n));
                edges_vector.push_back({0, n});
                n *= 2;
            }
        }
    }

    // Add a complete graph
    void add_complete(int n) {
        if (n < 0) {
            throw GraphwError("Negative number of nodes not valid: " +
                              std::to_string(n));
        } else if (n > 0) {
            // No need to do anything if n = 0, therefore n > 0
            if (n == 1) {
                add_node();
            } else {
                int initial_size = graph.size();
                for (int i = initial_size; i < (initial_size + n); i++) {
                    for (int j = (i + 1); j < (initial_size + n); j++) {
                        add_edge(std::to_string(i), std::to_string(j));
                    }
                }
            }
        }
    }

    // Add a complete multipartite graph
    void add_complete_multipartite(const std::list<int>& subset_sizes) {
        int initial_size = graph.size();
        // sizes_vector is vector format of subset_sizes list for operator[]
        std::vector<int> sizes_vector;
        // Calculate amount of nodes
        int node_amount = 0;
        for (auto const& size : subset_sizes) {
            if (size > 0) {
                node_amount += size;
                sizes_vector.push_back(size);
            } else if (size < 0) {
                // Size is negative
                throw GraphwError("Negative number of nodes not valid");
            }
            // Ignore if size is 0
        }
        // Add initial nodes to preserve node ordering
        for (int i = initial_size; i < (initial_size + node_amount); i++) {
            add_node(std::to_string(i));
        }
        // Add edges
        int current_subset = 0;
        int subset_counter = 0;
        int first_neighbor;
        for (int i = initial_size; i < (initial_size + node_amount); i++) {
            if (subset_counter >= sizes_vector[current_subset]) {
                current_subset++;
                subset_counter = 0;
            }
            if (current_subset == sizes_vector.size() - 1) {
                // Currently on last subset, simply add nodes
                continue;
            } else {
                if (subset_counter == 0) {
                    // First node in subset
                    first_neighbor = i + sizes_vector[current_subset];
                }
                for (int j = first_neighbor; j < (initial_size + node_amount);
                     j++) {
                    add_edge(std::to_string(i), std::to_string(j));
                }
            }
            subset_counter++;
        }
    }

    // Add a circular ladder graph
    void add_circular_ladder(int n) {
        if (n < 0) {
            throw GraphwError("Negative number of nodes not valid: " +
                              std::to_string(n));
        } else if (n > 0) {
            int initial_size = graph.size();
            add_ladder(n);
            if (n > 2) {
                add_edge(std::to_string(initial_size),
                         std::to_string(initial_size + (n - 1)));
                add_edge(std::to_string(initial_size + n),
                         std::to_string(initial_size + ((2 * n) - 1)));
            }
        }
    }

    // Add a circulant graph
    void add_circulant(int n, const std::list<int>& offsets) {
        if (n < 0) {
            throw GraphwError("Negative number of nodes not valid: " +
                              std::to_string(n));
        } else if (n > 0) {
            int initial_size = graph.size();
            // Add nodes
            add_empty(n);
            // Add edges
            for (int i = initial_size; i < (initial_size + n); i++) {
                for (auto const& offset : offsets) {
                    add_edge(std::to_string(i),
                             std::to_string((i + abs(offset)) % n));
                }
            }
        }
    }

    // Add an empty graph
    void add_empty(int n) {
        if (n < 0) {
            throw GraphwError("Negative number of nodes not valid: " +
                              std::to_string(n));
        }
        int initial_size = graph.size();
        // Add nodes
        for (int i = initial_size; i < (initial_size + n); i++) {
            add_node(std::to_string(i));
        }
    }

    // Add a full m-ary tree
    void add_full_mary_tree(int m, int n) {
        if (n < 0) {
            throw GraphwError("Negative number of nodes not valid: " +
                              std::to_string(n));
        }
        if (m == 0) {
            add_empty(n);
        } else {
            int initial_size = graph.size();
            int current_node = initial_size;
            for (int i = initial_size;
                 (i < (initial_size + n)) && (current_node < n); i++) {
                for (int j = 1; (j <= m) && ((current_node + j) < n); j++) {
                    // Add edge
                    add_edge(std::to_string(i),
                             std::to_string(current_node + j));
                }
                current_node += m;
            }
        }
    }

    // Add a ladder graph
    void add_ladder(int n) {
        if (n < 0) {
            throw GraphwError("Negative number of nodes not valid: " +
                              std::to_string(n));
        } else if (n > 0) {
            int initial_size = graph.size();
            // Add nodes
            add_empty(2 * n);
            // Add edges
            for (int i = initial_size; i < (initial_size + n); i++) {
                if (i > initial_size) {
                    add_edge(std::to_string(i - 1), std::to_string(i));
                }
                // Add edge rung
                add_edge(std::to_string(i), std::to_string(i + n));
            }
            for (int i = (initial_size + n); i < (initial_size + (2 * n));
                 i++) {
                if (i > (initial_size + n)) {
                    add_edge(std::to_string(i - 1), std::to_string(i));
                }
            }
        }
    }

    // Add a lollipop graph
    void add_lollipop(int m, int n) {
        if (m < 2) {
            throw GraphwError("Invalid graph properties, m should be >=2");
        }
        if (n < 0) {
            throw GraphwError("Invalid graph properties, n should be >=0");
        }
        int initial_size = graph.size();
        // Create clique
        add_complete(m);
        if (n > 0) {
            // Connect clique with first path node
            add_edge(std::to_string((initial_size + m) - 1),
                     std::to_string(initial_size + m));
            std::list<std::string> path_nodes;
            for (int i = m; i < (m + n); i++) {
                path_nodes.push_back(std::to_string(i));
            }
            // Add path segment
            add_path(path_nodes);
        }
    }

    // Add a star graph
    void add_star(int k) {
        if (k < 0) {
            throw GraphwError("Negative number of nodes not valid: " +
                              std::to_string(k));
        } else if (k == 0) {
            // If k is 0, simply add a node
            add_node();
        } else {
            int initial_size = graph.size();
            // Add nodes
            for (int i = (initial_size + 1); i < (initial_size + k + 1); i++) {
                add_edge(std::to_string(initial_size), std::to_string(i));
            }
        }
    }

    // Add a Turan graph
    void add_turan(int n, int r) {
        if (r < 1 || r > n) {
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
        for (int i = 0; i < subset1_size; i++) {
            subset_sizes_list.push_back(subset1_value);
        }
        for (int i = 0; i < subset2_size; i++) {
            subset_sizes_list.push_back(subset2_value);
        }
        // Add subset_sizes_list to create complete multipartite graph
        add_complete_multipartite(subset_sizes_list);
    }

    // Add a wheel graph
    void add_wheel(int n) {
        if (n < 0) {
            throw GraphwError("Negative number of nodes not valid: " +
                              std::to_string(n));
        }
        if (n != 0) {
            int initial_size = graph.size();
            // To construct wheel graph, first construct a star graph
            add_star(n - 1);
            if (n > 2) {
                // If there are at least 2 nodes, create additional edges
                // Create cycle between nodes
                for (int i = initial_size; i < (initial_size + n); i++) {
                    if (i > initial_size) {
                        add_edge(std::to_string(i - 1), std::to_string(i));
                    }
                }
                // Add additional edge between last node and the node after the
                // center node
                add_edge(std::to_string(initial_size + n - 1),
                         std::to_string(initial_size + 1));
            }
        }
    }

    // Return the adjacency list of the graph with labels representing each node
    std::string get_adjacency_list(const std::string& delimiter = " ") {
        std::string adjacency_list;
        // Fill adjacency list
        for (int i = 0; i < graph.size(); i++) {
            std::string line;
            line += labels_vector[i] + delimiter;
            for (int j = 0; j < graph[i].size(); j++) {
                // Add neighbors to line
                line += graph[i][j].label + delimiter;
            }
            adjacency_list += line + '\n';
            line.clear();
        }
        return adjacency_list;
    }

    // Return the density of the graph
    float density() const {
        float size = (float)(graph.size());
        if (directed_) {
            return (float)(edges / (size * (size - 1)));
        }
        return (float)((2 * edges) / (size * (size - 1)));
    }

    // Return the degree of a node with given label
    int degree(std::string label) {
        if (labels.find(label) == labels.end()) {
            // Label does not exist
            throw GraphwError("Given label does not exist");
        }
        int node_id = identities[label];
        return graph[node_id].size();
    }

    // Return the average degree of the graph
    float average_degree() {
        int total = 0;
        for (int i = 0; i < graph.size(); i++) {
            total += degree(labels_vector[i]);
        }
        return ((float)total / (float)graph.size());
    }

    // Return a list of labels representing the neighbor of a given node label
    std::list<std::string> get_neighbors(std::string label) {
        if (labels.find(label) == labels.end()) {
            // Label does not exist
            throw GraphwError("Given label does not exist");
        }
        int node_id = identities[label];
        std::list<std::string> neighbors;
        for (int i = 0; i < graph[node_id].size(); i++) {
            neighbors.push_back(graph[node_id][i].label);
        }
        return neighbors;
    }

    // Return a list of labels representing the non-neighbors of a given node
    // label
    std::list<std::string> get_non_neighbors(std::string label) {
        if (labels.find(label) == labels.end()) {
            // Label does not exist
            throw GraphwError("Given label does not exist");
        }
        // Neighbors of the given node label
        std::list<std::string> neighbors = get_neighbors(label);
        // List which will hold all non neighbors
        std::list<std::string> non_neighbors;
        for (int i = 0; i < graph.size(); i++) {
            bool is_neighbor = false;
            std::string current_label = labels_vector[i];
            if (current_label != label) {
                // Current label being compared is not equal to given label
                is_neighbor =
                    std::any_of(neighbors.begin(), neighbors.end(),
                                [current_label](const std::string& neighbor) {
                                    return current_label == neighbor;
                                });
            } else {
                // Current label being compared is equal to given label
                // Mark it as a neighbor to avoid pushing it into non neighbors
                // list
                is_neighbor = true;
            }
            if (!is_neighbor) {
                non_neighbors.push_back(current_label);
            }
        }
        return non_neighbors;
    }

    // Return a list of labels representing the common neighbor(s) given two
    // node labels
    std::list<std::string> get_common_neighbors(std::string label1,
                                                std::string label2) {
        if (labels.find(label1) == labels.end() ||
            labels.find(label2) == labels.end()) {
            // At least one of the labels do not exist
            throw GraphwError("Given label does not exist");
        }
        // List of label1's neighbors
        std::list<std::string> neighbors1 = get_neighbors(label1);
        // List of label2's neighbors
        std::list<std::string> neighbors2 = get_neighbors(label2);
        // Compare lists
        std::list<std::string> common_neighbors(
            std::min(neighbors1.size(), neighbors2.size()));
        auto it = std::copy_if(
            neighbors1.begin(), neighbors1.end(), common_neighbors.begin(),
            [neighbors2](const std::string& label_a) {
                return std::any_of(neighbors2.begin(), neighbors2.end(),
                                   [label_a](const std::string& label_b) {
                                       return label_a == label_b;
                                   });
            });
        common_neighbors.resize(std::distance(common_neighbors.begin(), it));
        return common_neighbors;
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
    inline bool directed() const { return directed_; }

    // Set the graph to be directed or not
    inline void set_directed(bool new_directed_) { directed_ = new_directed_; }

    // Return number of nodes
    inline int number_of_nodes() const { return (int)(graph.size()); }

    // Return number of edges
    inline int number_of_edges() const { return edges; }

   private:
    std::set<std::string> labels;
    std::vector<std::string> labels_vector;
    std::unordered_map<std::string, int> identities;
    int edges;
    bool directed_;

    // Return a node struct instance given
    // a string label
    inline Node get_node(std::string label) const {
        return {(int)(graph.size()), label};
    }

    // Checks if given node definition matches
    // graph existing node definition
    bool does_node_exist(Node node) {
        auto it = identities.find(node.label);
        if (it == identities.end()) {
            return false;
        }
        return node.id == it->second;
    }

    // Return if two labels are node neighbors
    bool are_neighbors(const std::string& label1, const std::string& label2) {
        std::list<std::string> neighbors = get_neighbors(label1);
        if (std::any_of(neighbors.begin(), neighbors.end(),
                        [label2](const std::string& neighbor) {
                            return neighbor == label2;
                        })) {
            return true;
        }
        neighbors = get_neighbors(label2);
        if (std::any_of(neighbors.begin(), neighbors.end(),
                        [label1](const std::string& neighbor) {
                            return neighbor == label1;
                        })) {
            return true;
        }
        return false;
    }
};

class ArcDiagram : public Graph {
   public:
    using Graph::Graph;
};

class CircularLayout : public Graph {
   public:
    CircularLayout(bool new_directed_ = false, int new_node_radius_ = 20)
        : directed_(new_directed_), node_radius_(new_node_radius_) {}

    // Return the node radius
    inline int node_radius() const { return node_radius_; }

    // Set the node radius from given int
    inline void set_node_radius(int new_node_radius_) {
        node_radius_ = new_node_radius_;
    }

   private:
    bool directed_;
    int node_radius_;
};

class RandomLayout : public Graph {
   public:
    RandomLayout(bool new_directed_ = false, int new_node_radius_ = 20)
        : directed_(new_directed_), node_radius_(new_node_radius_) {}

    // Return the node radius
    inline int node_radius() const { return node_radius_; }

    // Set the node radius from given int
    inline void set_node_radius(int new_node_radius_) {
        node_radius_ = new_node_radius_;
    }

   private:
    bool directed_;
    int node_radius_;
};

class SpiralLayout : public Graph {
   public:
    SpiralLayout(bool new_directed_ = false, int new_node_radius_ = 10,
                 float new_resolution_ = 0.35, bool new_equidistant_ = false)
        : directed_(new_directed_),
          node_radius_(new_node_radius_),
          resolution_(new_resolution_),
          equidistant_(new_equidistant_) {}

    // Return the node radius
    inline int node_radius() const { return node_radius_; }

    // Set the node radius from given int
    inline void set_node_radius(int new_node_radius_) {
        node_radius_ = new_node_radius_;
    }

    // Return the current resolution
    inline float resolution() const { return resolution_; }

    // Set the resolution from given float
    inline void set_resolution(float new_resolution_) {
        resolution_ = new_resolution_;
    }

    // Return whether spiral layout is set to be equidistant
    inline bool equidistant() const { return equidistant_; }

    // Set the spiral to be equidistant or not
    inline void set_equidistant(bool new_equidistant_) {
        equidistant_ = new_equidistant_;
    }

   private:
    bool directed_;
    int node_radius_;
    float resolution_;
    bool equidistant_;
};

class ForceDirectedLayout : public Graph {
   public:
    ForceDirectedLayout(bool new_directed_ = false, int new_node_radius_ = 20,
                        int new_iterations_ = 300)
        : directed_(new_directed_),
          node_radius_(new_node_radius_),
          iterations_(new_iterations_) {}

    // Return the node radius
    inline int node_radius() const { return node_radius_; }

    // Set the node radius from given int
    inline void set_node_radius(int new_node_radius_) {
        node_radius_ = new_node_radius_;
    }

    // Return the number of iterations
    inline int iterations() const { return iterations_; }

    // Set the iterations from given int
    inline void set_iterations(int new_iterations_) {
        iterations_ = new_iterations_;
    }

   private:
    bool directed_;
    int node_radius_;
    int iterations_;
};
}  // namespace graphw
#endif
