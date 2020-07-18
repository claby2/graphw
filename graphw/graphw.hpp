#ifndef GRAPHW
#define GRAPHW

#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace graphw {
    struct Node {
        int id;
        std::string label;
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

            // Remove all nodes and edges from the graph
            void clear() {
                graph.clear();
                labels.clear();
                identities.clear();
                edges = 0;
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

            // Return whether graph is directed
            inline bool directed() {
                return directed_;
            }

        protected:
            std::set<std::string> labels;
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
