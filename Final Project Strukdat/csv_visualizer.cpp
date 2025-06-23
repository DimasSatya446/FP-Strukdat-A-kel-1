#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "graph.h"
#include "dijkstra.h"
#include "fileio.h"
using namespace std;

// Function to load locations with coordinates from CSV file
void loadLocationsWithCoordinates(const string& filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open locations file: " << filename << endl;
        return;
    }

    string line;
    cout << "📍 Loading locations from " << filename << "..." << endl;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string name, xStr, yStr;
        
        getline(ss, name, ',');
        getline(ss, xStr, ',');
        getline(ss, yStr);
        
        try {
            double x = stod(xStr);
            double y = stod(yStr);
            graph.addLocation(name, x, y);
        } catch (const exception& e) {
            cerr << "Error parsing line: " << line << endl;
        }
    }
    
    file.close();
    cout << "✅ Locations loaded successfully!" << endl;
}

class ASCIIGraphVisualizer {
private:
    Graph& graph;
    static const int WIDTH = 100;
    static const int HEIGHT = 50;
    char canvas[HEIGHT][WIDTH];
    
public:
    ASCIIGraphVisualizer(Graph& g) : graph(g) {
        clearCanvas();
    }
    
    void clearCanvas() {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                canvas[i][j] = ' ';
            }
        }
    }
    
    pair<int, int> scaleCoordinate(double x, double y) {
        // Find min/max coordinates
        double minX = 999999, maxX = -999999;
        double minY = 999999, maxY = -999999;
        
        auto& locations = graph.getLocations();
        for (const auto& loc : locations) {
            minX = min(minX, loc.second.getX());
            maxX = max(maxX, loc.second.getX());
            minY = min(minY, loc.second.getY());
            maxY = max(maxY, loc.second.getY());
        }
        
        // Scale to canvas size with margin
        int margin = 5;
        int scaledX = margin + (int)((x - minX) / (maxX - minX + 1) * (WIDTH - 2 * margin));
        int scaledY = margin + (int)((y - minY) / (maxY - minY + 1) * (HEIGHT - 2 * margin));
        
        // Clamp to canvas bounds
        scaledX = max(0, min(WIDTH - 1, scaledX));
        scaledY = max(0, min(HEIGHT - 1, scaledY));
        
        return {scaledX, scaledY};
    }
    
    void drawLine(int x1, int y1, int x2, int y2, char symbol = '.') {
        // Simple line drawing
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;
        
        int x = x1, y = y1;
        
        while (true) {
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                if (canvas[y][x] == ' ') canvas[y][x] = symbol;
            }
            
            if (x == x2 && y == y2) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }
    
    void drawLocation(const string& name, double x, double y, char symbol = 'O') {
        auto [scaledX, scaledY] = scaleCoordinate(x, y);
        
        if (scaledX >= 0 && scaledX < WIDTH && scaledY >= 0 && scaledY < HEIGHT) {
            canvas[scaledY][scaledX] = symbol;
        }
    }
    
    void drawRoute(const string& from, const string& to, char symbol = '.') {
        auto& locations = graph.getLocations();
        
        if (locations.find(from) == locations.end() || locations.find(to) == locations.end()) {
            return;
        }
        
        const Location& fromLoc = locations.at(from);
        const Location& toLoc = locations.at(to);
        
        auto [x1, y1] = scaleCoordinate(fromLoc.getX(), fromLoc.getY());
        auto [x2, y2] = scaleCoordinate(toLoc.getX(), toLoc.getY());
        
        drawLine(x1, y1, x2, y2, symbol);
    }
    
    void highlightPath(const vector<string>& path) {
        // Draw path with special symbol
        for (size_t i = 0; i < path.size() - 1; ++i) {
            drawRoute(path[i], path[i + 1], '#');
        }
        
        // Mark path nodes
        auto& locations = graph.getLocations();
        for (const string& node : path) {
            if (locations.find(node) != locations.end()) {
                const Location& loc = locations.at(node);
                drawLocation(node, loc.getX(), loc.getY(), '*');
            }
        }
    }
    
    void render() {
        cout << "\n" << string(WIDTH + 2, '=') << endl;
        
        for (int i = 0; i < HEIGHT; i++) {
            cout << "|";
            for (int j = 0; j < WIDTH; j++) {
                cout << canvas[i][j];
            }
            cout << "|" << endl;
        }
        
        cout << string(WIDTH + 2, '=') << endl;
    }
    
    void visualize() {
        clearCanvas();
        
        cout << "\n🎨 GENERATING ASCII VISUALIZATION..." << endl;
        
        // Draw all routes first
        auto& locations = graph.getLocations();
        for (const auto& locPair : locations) {
            const string& sourceName = locPair.first;
            const vector<Route>& routes = graph.getRoutesFrom(sourceName);
            
            for (const Route& route : routes) {
                drawRoute(sourceName, route.getDestination(), '.');
            }
        }
        
        // Find and highlight shortest path
        PathResult result = findShortestPath(graph, "Jakarta", "Makassar", "jarak");
        if (!result.path.empty()) {
            highlightPath(result.path);
        }
        
        // Draw all locations on top
        for (const auto& locPair : locations) {
            const Location& loc = locPair.second;
            char symbol = 'O';
            
            // Special symbols for start and end
            if (locPair.first == "Jakarta") symbol = 'S';
            else if (locPair.first == "Makassar") symbol = 'E';
            
            drawLocation(locPair.first, loc.getX(), loc.getY(), symbol);
        }
        
        render();
        
        // Print detailed info
        cout << "\n🗺️  TRANSPORTATION NETWORK VISUALIZATION" << endl;
        cout << "📊 Data loaded from: locations.csv & output.csv" << endl;
        cout << "🎯 Showing shortest path: Jakarta → Makassar" << endl;
        
        cout << "\n📍 LEGEND:" << endl;
        cout << "S = Start (Jakarta)" << endl;
        cout << "E = End (Makassar)" << endl;
        cout << "O = Other cities" << endl;
        cout << ". = Regular routes" << endl;
        cout << "# = Shortest path routes" << endl;
        cout << "* = Cities on shortest path" << endl;
        
        if (!result.path.empty()) {
            cout << "\n🚀 SHORTEST PATH ANALYSIS:" << endl;
            cout << "Route: ";
            for (size_t i = 0; i < result.path.size(); ++i) {
                cout << result.path[i];
                if (i < result.path.size() - 1) cout << " → ";
            }
            cout << "\nTotal distance: " << fixed << setprecision(1) << result.totalCost << " km" << endl;
            
            // Show route details
            cout << "\n📋 ROUTE DETAILS:" << endl;
            for (size_t i = 0; i < result.path.size() - 1; ++i) {
                const string& from = result.path[i];
                const string& to = result.path[i + 1];
                
                const vector<Route>& routes = graph.getRoutesFrom(from);
                for (const Route& route : routes) {
                    if (route.getDestination() == to) {
                        cout << "  🚗 " << from << " → " << to 
                             << " | " << route.getDistance() << " km"
                             << " | " << route.getTime() << " min"
                             << " | Rp " << (int)route.getCost() << endl;
                        break;
                    }
                }
            }
        }
        
        // Print all cities with coordinates
        cout << "\n📍 CITY COORDINATES:" << endl;
        for (const auto& locPair : locations) {
            const Location& loc = locPair.second;
            cout << "  " << setw(12) << left << locPair.first 
                 << ": (" << setw(3) << (int)loc.getX() 
                 << ", " << setw(3) << (int)loc.getY() << ")" << endl;
        }
    }
};

int main() {
    Graph g;

    cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                     TRANSPORTATION NETWORK VISUALIZER                       ║" << endl;
    cout << "║                           (Loading from CSV files)                          ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << endl;
    
    // Load data from CSV files
    loadLocationsWithCoordinates("locations.csv", g);
    
    cout << "\n🛤️  Loading routes from output.csv..." << endl;
    loadRoutesFromFile("output.csv", g);

    // Show network summary
    auto& locations = g.getLocations();
    int totalRoutes = 0;
    for (const auto& loc : locations) {
        totalRoutes += g.getRoutesFrom(loc.first).size();
    }
    
    cout << "\n📊 NETWORK SUMMARY:" << endl;
    cout << "   Cities: " << locations.size() << endl;
    cout << "   Routes: " << totalRoutes << endl;
    cout << "   Algorithm: Dijkstra's shortest path" << endl;

    // Create and run visualizer
    ASCIIGraphVisualizer visualizer(g);
    visualizer.visualize();

    return 0;
}
