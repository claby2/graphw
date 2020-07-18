#ifndef GRAPHW_DRAW
#define GRAPHW_DRAW

#define SDL_MAIN_HANDLED

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <cmath>
#include <set>
#include <utility>

#include <SDL2/SDL.h>

#include "graphw.hpp"

namespace graphw {
    bool initialized = false;
    int window_width = 640;
    int window_height = 480;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    struct Position {
        int x;
        int y;
    };

    // Initialize SDL2
    void init() {
        if(!initialized) {
            SDL_Init(SDL_INIT_VIDEO);
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
            window = SDL_CreateWindow(
                "graphw",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                window_width,
                window_height,
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
            );
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            initialized = true;
        }
    }

    // Close SDL2
    void close() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        renderer = nullptr;
        window = nullptr;
        SDL_Quit();
    }

    // Draws a circle given center x, centery, and radius
    void draw_circle(int cx, int cy, int radius) {
        int x = radius - 1;
        int y = 0;
        int tx = 1;
        int ty = 1;
        int error = tx - (radius * 2);
        while(x >= y) {
            int dx[8] = {x, x, -x, -x, y, y, -y, -y};
            int dy[8] = {-y, y, -y, y, -x, x, -x, x};
            for(int k = 0; k < 8; k++) {
                SDL_RenderDrawPoint(renderer, cx + dx[k], cy + dy[k]);
            }
            if(error <= 0) {
                y++;
                error += ty;
                ty += 2;
            }
            if(error > 0) {
                x--;
                tx += 2;
                error += tx - (radius * 2);
            }
        }
    }

    // Draw an Arc Diagram graph
    void draw(ArcDiagram &ad) {
        init();
        SDL_SetWindowTitle(window, "Arc Diagram");
        bool quit = false;
        bool redraw = true;
        SDL_Event event;
        while(!quit && SDL_WaitEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            } else if(event.type == SDL_WINDOWEVENT) {
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window_width = event.window.data1;
                    window_height = event.window.data2;
                    redraw = true;
                }
            }
            // Draw arc diagram
            if(redraw) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);
                // Initialize checkpoint positions and measurements
                int center_x = (int)(window_width / 2);
                int center_y = (int)(window_height / 2);
                int node_radius = (int)((window_width / (ad.number_of_nodes() * 2)) / 2);
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                // Draw line and node circles
                for(int i = 0; i <= ad.number_of_nodes(); i++) {
                    if(i < ad.number_of_nodes()) {
                        draw_circle(
                            (node_radius * 2) + (i * (4 * node_radius)),
                            center_y,
                            node_radius
                        );
                    }
                    if(i != 0 && i != ad.number_of_nodes()) {
                        int start_x = (int)(((4 * node_radius) * (i - 1)) + (3 * node_radius));
                        SDL_RenderDrawLine(
                            renderer, 
                            start_x, 
                            center_y, 
                            (int)(start_x + (2 * node_radius)), 
                            center_y
                        );
                    }
                }
                // Draw edges
                std::set<std::pair<int, int> > edges;
                int current_edge = 0;
                for(int i = 0; i < ad.graph.size(); i++) {
                    for(int j = 0; j < ad.graph[i].size(); j++) {
                        // Draw edge from node i to j
                        int node1_id = i;
                        int node2_id = (int)(ad.graph[i][j].id);
                        int node1_x = (int)(((4 * node_radius) * node1_id) + (2 * node_radius));
                        int node2_x = (int)(((4 * node_radius) * node2_id) + (2 * node_radius));
                        if(!ad.directed()) {
                            int min_node = std::min(node1_id, node2_id);
                            int max_node = std::max(node1_id, node2_id);
                            if(!edges.insert(std::make_pair(min_node, max_node)).second) {
                                // Edge has already been drawn
                                continue;
                            }
                        }
                        bool above = (current_edge * 2 < ad.number_of_edges()) ? false : true;
                        int edge_radius = (int)(abs(node1_x - node2_x) / 2);
                        int cx = (int)((node1_x + node2_x) / 2);
                        int cy = center_y + (above ? node_radius : -1 * node_radius);
                        int x = edge_radius - 1;
                        int y = 0;
                        int tx = 1;
                        int ty = 1;
                        int error = tx - (edge_radius * 2);
                        // Draw edge as semi circle
                        while(x >= y) {
                            int dx[8] = {x, x, -x, -x, y, y, -y, -y};
                            int dy[8] = {-y, y, -y, y, -x, x, -x, x};
                            for(int k = 0; k < 8; k++) {
                                if((above && (cy + dy[k] >= cy)) || (!above && (cy + dy[k] <= cy))) {
                                    SDL_RenderDrawPoint(renderer, cx + dx[k], cy + dy[k]);
                                }
                            }
                            if(error <= 0) {
                                y++;
                                error += ty;
                                ty += 2;
                            }
                            if(error > 0) {
                                x--;
                                tx += 2;
                                error += tx - (edge_radius * 2);
                            }
                        }
                        current_edge++;
                    }
                }
                redraw = false;
                SDL_RenderPresent(renderer);
            }
        }
    }

    // Draw a Circular Layout graph
    void draw(CircularLayout &cl) {
        init();
        SDL_SetWindowTitle(window, "Circular Layout");
        bool quit = false;
        bool redraw = true;
        SDL_Event event;
        const int circle_padding = 5;
        const int node_radius = cl.node_radius();
        while(!quit && SDL_WaitEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            } else if(event.type == SDL_WINDOWEVENT) {
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window_width = event.window.data1;
                    window_height = event.window.data2;
                    redraw = true;
                }
            }
            // Draw circular layout
            if(redraw) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);
                // Initialize checkpoint positions and measurements
                int center_x = (int)(window_width / 2);
                int center_y = (int)(window_height / 2);
                int circle_radius = (int)((std::min(window_width, window_height) / 2) - circle_padding - node_radius);
                float circle_circumference = 2 * M_PI * (float)circle_radius;
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                // Draw nodes
                std::vector<Position> node_positions;
                for(int i = 0; i < cl.number_of_nodes(); i++) {
                    float angle = ((float)i / cl.number_of_nodes()) * (2 * M_PI);
                    int node_x = center_x + (circle_radius * cos(angle));
                    int node_y = center_y + (circle_radius * sin(angle));
                    draw_circle(node_x, node_y, node_radius);
                    Position position = {
                        node_x,
                        node_y
                    };
                    node_positions.push_back(position);
                }
                // Draw edges
                for(int i = 0; i < cl.graph.size(); i++) {
                    for(int j = 0; j < cl.graph[i].size(); j++) {
                        // Draw edge (line) from node i to j
                        int node1_id = i;
                        int node2_id = (int)(cl.graph[i][j].id);
                        SDL_RenderDrawLine(
                            renderer,
                            node_positions[node1_id].x,
                            node_positions[node1_id].y,
                            node_positions[node2_id].x,
                            node_positions[node2_id].y
                        );
                    }
                }
                redraw = false;
                SDL_RenderPresent(renderer);
            }
        }
    }

    // Draw Random Layout graph
    void draw(RandomLayout &rl) {
        srand(time(NULL));
        init();
        SDL_SetWindowTitle(window, "Random Layout");
        bool quit = false;
        bool redraw = true;
        bool first_render = true;
        SDL_Event event;
        const int node_radius = rl.node_radius();
        std::vector<std::pair<float, float> > random_positions;
        while(!quit && SDL_WaitEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            } else if(event.type == SDL_WINDOWEVENT) {
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window_width = event.window.data1;
                    window_height = event.window.data2;
                    redraw = true;
                }
            }
            // Draw random layout
            if(redraw) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0XFF);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                // Draw nodes
                std::vector<Position> node_positions;
                for(int i = 0; i < rl.number_of_nodes(); i++) {
                    if(first_render) {
                        random_positions.push_back(std::make_pair(
                            (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)),
                            (static_cast<float>(rand()) / static_cast<float>(RAND_MAX))
                        ));
                    }
                    int node_x = (int)((random_positions[i].first * (window_width - node_radius)) + node_radius);
                    int node_y = (int)((random_positions[i].second * (window_height - node_radius)) + node_radius);
                    draw_circle(node_x, node_y, node_radius);
                    Position position = {
                        node_x,
                        node_y
                    };
                    node_positions.push_back(position);
                }
                // Draw edges
                for(int i = 0; i < rl.graph.size(); i++) {
                    for(int j = 0; j < rl.graph[i].size(); j++) {
                        // Draw edge (line) from node i to j
                        int node1_id = i;
                        int node2_id = (int)(rl.graph[i][j].id);
                        SDL_RenderDrawLine(
                            renderer,
                            node_positions[node1_id].x,
                            node_positions[node1_id].y,
                            node_positions[node2_id].x,
                            node_positions[node2_id].y
                        );
                    }
                }
                redraw = false;
                first_render = false;
                SDL_RenderPresent(renderer);
            }
        }
    }

    // Draw a Spiral Layout graph
    void draw(SpiralLayout &sl) {
        init();
        SDL_SetWindowTitle(window, "Spiral Layout");
        bool quit = false;
        bool redraw = true;
        SDL_Event event;
        const int node_radius = sl.node_radius();
        const float resolution = sl.resolution();
        const float chord = 1;
        const float step = sl.equidistant() ? 0.5 : 1.0;
        while(!quit && SDL_WaitEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            } else if(event.type == SDL_WINDOWEVENT) {
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window_width = event.window.data1;
                    window_height = event.window.data2;
                    redraw = true;
                }
            }
            // Draw spiral layout
            if(redraw) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);
                // Initialize checkpoint positions and measurements
                int center_x = (int)(window_width / 2);
                int center_y = (int)(window_height / 2);
                int min_dimension = std::min(window_width, window_height);
                int padding = 4 * node_radius;
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                // Get initial node positions
                // Store node_positions as pair of floats
                std::vector<std::pair<float, float> > node_positions;
                if(sl.equidistant()) {
                    // Calculate node positions with equal distances
                    float theta = resolution;
                    for(int i = 0; i <= sl.number_of_nodes(); i++) {
                        float radius = step * theta;
                        theta += chord / radius;
                        float node_x = (cos(theta) * radius);
                        float node_y = (sin(theta) * radius);
                        if(i > 0) {
                            node_positions.push_back(std::make_pair(
                                node_x,
                                node_y
                            ));
                        }
                    }
                } else {
                    // Calculate node positions without equal distances
                    float angle = 0.0;
                    float dist = 0.0;
                    float radius = sl.number_of_nodes();
                    while(dist * sqrt((cos(angle) * cos(angle)) + (sin(angle) * sin(angle))) < radius) {
                        float node_x = (cos(angle) * dist);
                        float node_y = (sin(angle) * dist);
                        node_positions.push_back(std::make_pair(
                            node_x,
                            node_y
                        ));
                        dist += step;
                        angle += resolution;
                    }
                }
                // Get node max coordinate (x or y dependent on window dimensions)
                float max = 0;
                for(int i = 0; i < node_positions.size(); i++) {
                    float coordinate;
                    if(window_width == min_dimension) {
                        // Window width is smaller than window height
                        // Record absolute x coordinate
                        coordinate = abs(node_positions[i].first);
                    } else {
                        // Window height is smaller than window width
                        // Record absolute y coordinate
                        coordinate = abs(node_positions[i].second);
                    }
                    max = std::max(coordinate, max);
                }
                // Adjust node positions with calculated factor
                float factor = ((float)(min_dimension - padding) / max) / 2;
                for(int i = 0; i < node_positions.size(); i++) {
                    node_positions[i].first = (node_positions[i].first * factor) + center_x;
                    node_positions[i].second = (node_positions[i].second * factor) + center_y;
                    int node_x = node_positions[i].first;
                    int node_y = node_positions[i].second;
                    draw_circle(
                        node_x, 
                        node_y, 
                        node_radius
                    );
                }
                // Draw edges
                for(int i = 0; i < sl.graph.size(); i++) {
                    for(int j = 0; j < sl.graph[i].size(); j++) {
                        // Draw edge (line) from node i to j
                        int node1_id = i;
                        int node2_id = (int)(sl.graph[i][j].id);
                        SDL_RenderDrawLine(
                            renderer,
                            node_positions[node1_id].first,
                            node_positions[node1_id].second,
                            node_positions[node2_id].first,
                            node_positions[node2_id].second
                        );
                    }
                }
                redraw = false;
                SDL_RenderPresent(renderer);
            }
        }
    }
} // namespace graphw
#endif