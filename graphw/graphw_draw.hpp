#ifndef GRAPHW_DRAW
#define GRAPHW_DRAW

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <string>
#include <utility>

#include "graphw.hpp"

namespace graphw {
struct Position {
    int x;
    int y;
};

struct PositionFloat {
    float x;
    float y;
};

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

// Operator overloading equality for color comparison
bool operator==(const graphw::Color &color1, const graphw::Color &color2) {
    return (
        color1.red == color2.red &&
        color1.green == color2.green &&
        color1.blue == color2.blue);
};

bool initialized = false;
bool save_as_bmp = false;
std::string bmp_file_path;
const int default_window_width = 640;
const int default_window_height = 480;
int window_width = default_window_width;
int window_height = default_window_height;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
Color background_color = {0xFF, 0xFF, 0xFF};
Color edge_color = {0x00, 0x00, 0x00};
Color node_color = {0x00, 0x00, 0x00};

// Initialize SDL2
void init() {
    if (!initialized) {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        window = SDL_CreateWindow(
            "graphw",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            window_width,
            window_height,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        initialized = true;
    }
}

// Close SDL2
void close() {
    if (save_as_bmp) {
        // Resize window to default dimensions
        SDL_SetWindowSize(window, default_window_width, default_window_height);
        SDL_Surface *surface = SDL_CreateRGBSurface(
            0,
            default_window_width,
            default_window_height,
            32,
            0x00ff0000,
            0x0000ff00,
            0x000000ff,
            0xff000000);
        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
        SDL_SaveBMP(surface, bmp_file_path.c_str());
        SDL_FreeSurface(surface);
        save_as_bmp = false;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;
    SDL_Quit();
}

inline void set_background_color(uint8_t red, uint8_t green, uint8_t blue) {
    background_color = {red, green, blue};
}

inline void set_edge_color(uint8_t red, uint8_t green, uint8_t blue) {
    edge_color = {red, green, blue};
}

inline void set_node_color(uint8_t red, uint8_t green, uint8_t blue) {
    node_color = {red, green, blue};
}

// Set the file path to save graph as png
void set_save(std::string file_path) {
    save_as_bmp = true;
    bmp_file_path = file_path;
}

// Draws a circle given center x, center y, and radius
void draw_circle(int cx, int cy, int radius) {
    int x = radius - 1;
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = tx - (radius * 2);
    while (x >= y) {
        int dx[8] = {x, x, -x, -x, y, y, -y, -y};
        int dy[8] = {-y, y, -y, y, -x, x, -x, x};
        for (int k = 0; k < 8; k++) {
            SDL_RenderDrawPoint(renderer, cx + dx[k], cy + dy[k]);
        }
        if (error <= 0) {
            y++;
            error += ty;
            ty += 2;
        }
        if (error > 0) {
            x--;
            tx += 2;
            error += tx - (radius * 2);
        }
    }
}

void set_window_title(ArcDiagram &ad) {
    SDL_SetWindowTitle(window, "Arc Diagram");
}

void set_window_title(CircularLayout &cl) {
    SDL_SetWindowTitle(window, "Circular Layout");
}

void set_window_title(RandomLayout &rl) {
    SDL_SetWindowTitle(window, "Random Layout");
}

void set_window_title(SpiralLayout &sl) {
    SDL_SetWindowTitle(window, "Spiral Layout");
}

void set_window_title(ForceDirectedLayout &fdl) {
    SDL_SetWindowTitle(window, "Force Directed Layout");
}

// Render an Arc Diagram graph
std::vector<Position> render(ArcDiagram &ad) {
    std::vector<Position> node_positions;
    // Initialize checkpoint positions and measurements
    int center_x = (int)(window_width / 2);
    int center_y = (int)(window_height / 2);
    int node_radius = (int)((window_width / (ad.number_of_nodes() * 2)) / 2);
    // Draw line and node circles
    SDL_SetRenderDrawColor(renderer, node_color.red, node_color.green, node_color.blue, 0xFF);
    for (int i = 0; i <= ad.number_of_nodes(); i++) {
        if (i < ad.number_of_nodes()) {
            int node_x = (node_radius * 2) + (i * (4 * node_radius));
            int node_y = center_y;
            node_positions.push_back({node_x, node_y});
            draw_circle(
                node_x,
                node_y,
                node_radius);
        }
        if (i != 0 && i != ad.number_of_nodes()) {
            int start_x = (int)(((4 * node_radius) * (i - 1)) + (3 * node_radius));
            SDL_RenderDrawLine(
                renderer,
                start_x,
                center_y,
                (int)(start_x + (2 * node_radius)),
                center_y);
        }
    }
    // Draw edges
    std::set<std::pair<int, int> > edges;
    int current_edge = 0;
    SDL_SetRenderDrawColor(renderer, edge_color.red, edge_color.green, edge_color.blue, 0xFF);
    for (int i = 0; i < ad.graph.size(); i++) {
        for (int j = 0; j < ad.graph[i].size(); j++) {
            // Draw edge from node i to j
            int node1_id = i;
            int node2_id = (int)(ad.graph[i][j].id);
            int node1_x = (int)(((4 * node_radius) * node1_id) + (2 * node_radius));
            int node2_x = (int)(((4 * node_radius) * node2_id) + (2 * node_radius));
            if (!ad.directed()) {
                int min_node = std::min(node1_id, node2_id);
                int max_node = std::max(node1_id, node2_id);
                if (!edges.insert(std::make_pair(min_node, max_node)).second) {
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
            while (x >= y) {
                int dx[8] = {x, x, -x, -x, y, y, -y, -y};
                int dy[8] = {-y, y, -y, y, -x, x, -x, x};
                for (int k = 0; k < 8; k++) {
                    if ((above && (cy + dy[k] >= cy)) || (!above && (cy + dy[k] <= cy))) {
                        SDL_RenderDrawPoint(renderer, cx + dx[k], cy + dy[k]);
                    }
                }
                if (error <= 0) {
                    y++;
                    error += ty;
                    ty += 2;
                }
                if (error > 0) {
                    x--;
                    tx += 2;
                    error += tx - (edge_radius * 2);
                }
            }
            current_edge++;
        }
    }
    return node_positions;
}

// Render a Circular Layout graph
std::vector<Position> render(CircularLayout &cl) {
    std::vector<Position> node_positions;
    const int circle_padding = 5;
    const int node_radius = cl.node_radius();
    // Initialize checkpoint positions and measurements
    int center_x = (int)(window_width / 2);
    int center_y = (int)(window_height / 2);
    int circle_radius = (int)((std::min(window_width, window_height) / 2) - circle_padding - node_radius);
    float circle_circumference = 2 * M_PI * (float)circle_radius;
    // Draw nodes
    SDL_SetRenderDrawColor(renderer, node_color.red, node_color.green, node_color.blue, 0xFF);
    for (int i = 0; i < cl.number_of_nodes(); i++) {
        float angle = ((float)i / cl.number_of_nodes()) * (2 * M_PI);
        int node_x = center_x + (circle_radius * cos(angle));
        int node_y = center_y + (circle_radius * sin(angle));
        draw_circle(node_x, node_y, node_radius);
        Position position = {
            node_x,
            node_y};
        node_positions.push_back(position);
    }
    // Draw edges
    SDL_SetRenderDrawColor(renderer, edge_color.red, edge_color.green, edge_color.blue, 0xFF);
    for (int i = 0; i < cl.graph.size(); i++) {
        for (int j = 0; j < cl.graph[i].size(); j++) {
            // Draw edge (line) from node i to j
            int node1_id = i;
            int node2_id = (int)(cl.graph[i][j].id);
            SDL_RenderDrawLine(
                renderer,
                node_positions[node1_id].x,
                node_positions[node1_id].y,
                node_positions[node2_id].x,
                node_positions[node2_id].y);
        }
    }
    return node_positions;
}

// Render a Spiral Layout graph
std::vector<Position> render(SpiralLayout &sl) {
    std::vector<Position> node_positions;
    const int node_radius = sl.node_radius();
    const float resolution = sl.resolution();
    const float chord = 1;
    const float step = sl.equidistant() ? 0.5 : 1.0;
    // Initialize checkpoint positions and measurements
    int center_x = (int)(window_width / 2);
    int center_y = (int)(window_height / 2);
    int min_dimension = std::min(window_width, window_height);
    int padding = 4 * node_radius;
    // Get initial node positions
    // Store node_positions as pair of floats
    std::vector<std::pair<float, float> > node_positions_float;
    if (sl.equidistant()) {
        // Calculate node positions with equal distances
        float theta = resolution;
        for (int i = 0; i <= sl.number_of_nodes(); i++) {
            float radius = step * theta;
            theta += chord / radius;
            float node_x = (cos(theta) * radius);
            float node_y = (sin(theta) * radius);
            if (i > 0) {
                node_positions_float.push_back(std::make_pair(
                    node_x,
                    node_y));
            }
        }
    } else {
        // Calculate node positions without equal distances
        float angle = 0.0;
        float dist = 0.0;
        float radius = sl.number_of_nodes();
        while (dist * sqrt((cos(angle) * cos(angle)) + (sin(angle) * sin(angle))) < radius) {
            float node_x = (cos(angle) * dist);
            float node_y = (sin(angle) * dist);
            node_positions_float.push_back(std::make_pair(
                node_x,
                node_y));
            dist += step;
            angle += resolution;
        }
    }
    // Get node max coordinate (x or y dependent on window dimensions)
    float max = 0;
    for (int i = 0; i < node_positions_float.size(); i++) {
        float coordinate;
        if (window_width == min_dimension) {
            // Window width is smaller than window height
            // Record absolute x coordinate
            coordinate = abs(node_positions_float[i].first);
        } else {
            // Window height is smaller than window width
            // Record absolute y coordinate
            coordinate = abs(node_positions_float[i].second);
        }
        max = std::max(coordinate, max);
    }
    // Adjust node positions with calculated factor
    float factor = ((float)(min_dimension - padding) / max) / 2;
    SDL_SetRenderDrawColor(renderer, node_color.red, node_color.green, node_color.blue, 0xFF);
    for (int i = 0; i < node_positions_float.size(); i++) {
        node_positions_float[i].first = (node_positions_float[i].first * factor) + center_x;
        node_positions_float[i].second = (node_positions_float[i].second * factor) + center_y;
        int node_x = node_positions_float[i].first;
        int node_y = node_positions_float[i].second;
        node_positions.push_back({node_x, node_y});
        draw_circle(
            node_x,
            node_y,
            node_radius);
    }
    // Draw edges
    SDL_SetRenderDrawColor(renderer, edge_color.red, edge_color.green, edge_color.blue, 0xFF);
    for (int i = 0; i < sl.graph.size(); i++) {
        for (int j = 0; j < sl.graph[i].size(); j++) {
            // Draw edge (line) from node i to j
            int node1_id = i;
            int node2_id = (int)(sl.graph[i][j].id);
            SDL_RenderDrawLine(
                renderer,
                node_positions_float[node1_id].first,
                node_positions_float[node1_id].second,
                node_positions_float[node2_id].first,
                node_positions_float[node2_id].second);
        }
    }
    return node_positions;
}

// Render Random Layout graph
std::vector<Position> render_random(
    RandomLayout &rl,
    std::vector<std::pair<float, float> > &random_positions,
    bool first_render) {
    std::vector<Position> node_positions;
    const int node_radius = rl.node_radius();
    // Draw nodes
    SDL_SetRenderDrawColor(renderer, node_color.red, node_color.green, node_color.blue, 0xFF);
    for (int i = 0; i < rl.number_of_nodes(); i++) {
        if (first_render) {
            random_positions.push_back(std::make_pair(
                (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)),
                (static_cast<float>(rand()) / static_cast<float>(RAND_MAX))));
        }
        int node_x = (int)((random_positions[i].first * (window_width - node_radius)) + node_radius);
        int node_y = (int)((random_positions[i].second * (window_height - node_radius)) + node_radius);
        draw_circle(node_x, node_y, node_radius);
        Position position = {
            node_x,
            node_y};
        node_positions.push_back(position);
    }
    // Draw edges
    SDL_SetRenderDrawColor(renderer, edge_color.red, edge_color.green, edge_color.blue, 0xFF);
    for (int i = 0; i < rl.graph.size(); i++) {
        for (int j = 0; j < rl.graph[i].size(); j++) {
            // Draw edge (line) from node i to j
            int node1_id = i;
            int node2_id = (int)(rl.graph[i][j].id);
            SDL_RenderDrawLine(
                renderer,
                node_positions[node1_id].x,
                node_positions[node1_id].y,
                node_positions[node2_id].x,
                node_positions[node2_id].y);
        }
    }
    first_render = false;
    return node_positions;
}

// Render Force Directed Layout graph
std::vector<Position> render_random(
    ForceDirectedLayout &fd,
    std::vector<std::pair<float, float> > &random_positions,
    bool first_render) {
    std::vector<Position> node_positions;
    std::vector<PositionFloat> movement(fd.number_of_nodes(), {0.0, 0.0});
    const int node_radius = fd.node_radius();
    const int iterations = fd.iterations();
    float temperature = 0.1;
    // Change in temperature per iteration
    float dt = temperature / (float)(iterations + 1);
    // Optimal distance
    float k = (float)(sqrt(1.0 / fd.number_of_nodes()));
    if (first_render) {
        // Set random positions
        for (int i = 0; i < fd.number_of_nodes(); i++) {
            random_positions.push_back(std::make_pair(
                (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)),
                (static_cast<float>(rand()) / static_cast<float>(RAND_MAX))));
        }
        for (int iter = 0; iter < iterations; iter++) {
            for (int i = 0; i < fd.number_of_nodes(); i++) {
                // Calculate repulsion
                for (int j = (i + 1); j < fd.number_of_nodes(); j++) {
                    float delta_x = random_positions[i].first - random_positions[j].first;
                    float delta_y = random_positions[i].second - random_positions[j].second;
                    float distance = sqrt((delta_x * delta_x) + (delta_y * delta_y));
                    float repulsion = (k * k) / distance;
                    // Update movement vector for node i
                    movement[i].x += (delta_x / distance) * repulsion;
                    movement[i].y += (delta_y / distance) * repulsion;
                    // Update movemnt vector for node j
                    movement[j].x -= (delta_x / distance) * repulsion;
                    movement[j].y -= (delta_y / distance) * repulsion;
                }
                // Calculate attraction
                for (int j = 0; j < fd.graph[i].size(); j++) {
                    int neighbor_id = fd.graph[i][j].id;
                    if (neighbor_id > i) {
                        continue;
                    }
                    float delta_x = random_positions[i].first - random_positions[neighbor_id].first;
                    float delta_y = random_positions[i].second - random_positions[neighbor_id].second;
                    float distance = sqrt((delta_x * delta_x) + (delta_y * delta_y));
                    float attraction = (distance * distance) / k;
                    // Update movement vector for node i
                    movement[i].x -= (delta_x / distance) * attraction;
                    movement[i].y -= (delta_y / distance) * attraction;
                    // Update movemnt vector for node j
                    movement[neighbor_id].x += (delta_x / distance) * attraction;
                    movement[neighbor_id].y += (delta_y / distance) * attraction;
                }
            }
            for (int i = 0; i < fd.number_of_nodes(); i++) {
                // Limit maximum movement to temperature
                float movement_distance = sqrt((movement[i].x * movement[i].x) + (movement[i].y * movement[i].y));
                float capped_movement = std::min(movement_distance, temperature);
                float capped_movement_x = (movement[i].x / movement_distance) * capped_movement;
                float capped_movement_y = (movement[i].y / movement_distance) * capped_movement;
                random_positions[i].first += capped_movement_x;
                random_positions[i].second += capped_movement_y;
            }
            // Cool down temperature as a better configuration approaches
            temperature -= dt;
        }
        float x_min = std::numeric_limits<float>::max();
        float x_max = std::numeric_limits<float>::lowest();
        float y_min = std::numeric_limits<float>::max();
        float y_max = std::numeric_limits<float>::lowest();
        for (int i = 0; i < random_positions.size(); i++) {
            if (random_positions[i].first < x_min) {
                x_min = random_positions[i].first;
            }
            if (random_positions[i].first > x_max) {
                x_max = random_positions[i].first;
            }
            if (random_positions[i].second < y_min) {
                y_min = random_positions[i].second;
            }
            if (random_positions[i].second > y_max) {
                y_max = random_positions[i].second;
            }
        }
        float width = x_max - x_min;
        float height = y_max - y_min;
        // Get scale factor
        float x_scale = 1.0 / width;
        float y_scale = 1.0 / height;
        float scale = 0.9 * (x_scale < y_scale ? x_scale : y_scale);
        float center_x = x_max + x_min;
        float center_y = y_max + y_min;
        float offset_x = (center_x / 2.0) * scale;
        float offset_y = (center_y / 2.0) * scale;
        for (int i = 0; i < random_positions.size(); i++) {
            random_positions[i].first = ((random_positions[i].first * scale) - offset_x) + 0.5;
            random_positions[i].second = ((random_positions[i].second * scale) - offset_y) + 0.5;
        }
    }
    SDL_SetRenderDrawColor(renderer, node_color.red, node_color.green, node_color.blue, 0xFF);
    for (int i = 0; i < fd.number_of_nodes(); i++) {
        int node_x = (int)((random_positions[i].first * (window_width - node_radius)) + node_radius);
        int node_y = (int)((random_positions[i].second * (window_height - node_radius)) + node_radius);
        draw_circle(node_x, node_y, node_radius);
        node_positions.push_back({node_x, node_y});
    }
    // Draw edges
    SDL_SetRenderDrawColor(renderer, edge_color.red, edge_color.green, edge_color.blue, 0xFF);
    for (int i = 0; i < fd.graph.size(); i++) {
        for (int j = 0; j < fd.graph[i].size(); j++) {
            // Draw edge (line) from node i to j
            int node1_id = i;
            int node2_id = (int)(fd.graph[i][j].id);
            SDL_RenderDrawLine(
                renderer,
                node_positions[node1_id].x,
                node_positions[node1_id].y,
                node_positions[node2_id].x,
                node_positions[node2_id].y);
        }
    }
    return node_positions;
}

namespace {
// Draw random graph layouts which need to cache random positions
template <typename Graph>
void draw_random(Graph &g, bool force_close = true) {
    // Seed random
    srand(time(NULL));
    init();
    set_window_title(g);
    bool quit = false;
    bool redraw = true;
    SDL_Event event;
    // Cache random positions
    std::vector<std::pair<float, float> > random_positions;
    bool first_render = true;
    while (!quit && SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                window_width = event.window.data1;
                window_height = event.window.data2;
                redraw = true;
            }
        }
        if (redraw) {
            SDL_SetRenderDrawColor(renderer, background_color.red, background_color.green, background_color.blue, 0xFF);
            SDL_RenderClear(renderer);
            // Call render random
            render_random(g, random_positions, first_render);
            SDL_RenderPresent(renderer);
            redraw = false;
            first_render = false;
        }
    }
    if (force_close) {
        close();
    }
}
}  // anonymous namespace

// Draw a given graph
template <typename Graph>
void draw(Graph &g, bool force_close = true) {
    init();
    set_window_title(g);
    bool quit = false;
    bool redraw = true;
    SDL_Event event;
    while (!quit && SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                window_width = event.window.data1;
                window_height = event.window.data2;
                redraw = true;
            }
        }
        if (redraw) {
            SDL_SetRenderDrawColor(renderer, background_color.red, background_color.green, background_color.blue, 0xFF);
            SDL_RenderClear(renderer);
            render(g);
            SDL_RenderPresent(renderer);
            redraw = false;
        }
    }
    if (force_close) {
        close();
    }
}

// Draw random layout graph
inline void draw(RandomLayout &rl, bool force_close = true) {
    draw_random(rl);
}

// Draw force directed layout
inline void draw(ForceDirectedLayout &fd, bool force_close = true) {
    draw_random(fd);
}
}  // namespace graphw
#endif