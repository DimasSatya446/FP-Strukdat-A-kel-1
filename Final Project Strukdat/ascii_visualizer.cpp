#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "graph.h"
#include "dijkstra.h"
using namespace std;

class ASCIIGraphVisualizer {
private:
    Graph& graph;
    static const int WIDTH = 80;
    static const int HEIGHT = 40;
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
    
    void drawLine(int x1, int y1, int x2, int y2, char symbol = '-') {
        // Simple line drawing using Bresenham's algorithm (simplified)
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
        cout << "|" << string(WIDTH, ' ') << "|" << endl;
        
        for (int i = 0; i < HEIGHT; i++) {
            cout << "|";
            for (int j = 0; j < WIDTH; j++) {
                cout << canvas[i][j];
            }
            cout << "|" << endl;
        }
        
        cout << "|" << string(WIDTH, ' ') << "|" << endl;
        cout << string(WIDTH + 2, '=') << endl;
    }
    
    void visualize() {
        clearCanvas();
        
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
        PathResult result = findShortestPath(graph, "Jakarta", "Malang", "jarak");
        if (!result.path.empty()) {
            highlightPath(result.path);
        }
        
        // Draw all locations on top
        for (const auto& locPair : locations) {
            const Location& loc = locPair.second;
            char symbol = 'O';
            
            // Special symbols for start and end
            if (locPair.first == "Jakarta") symbol = 'S';
            else if (locPair.first == "Malang") symbol = 'E';
            
            drawLocation(locPair.first, loc.getX(), loc.getY(), symbol);
        }
        
        render();
        
        // Print legend and info
        cout << "\nLEGEND:" << endl;
        cout << "S = Start (Jakarta)" << endl;
        cout << "E = End (Malang)" << endl;
        cout << "O = Other locations" << endl;
        cout << ". = Regular routes" << endl;
        cout << "# = Shortest path routes" << endl;
        cout << "* = Nodes on shortest path" << endl;
        
        if (!result.path.empty()) {
            cout << "\nSHORTEST PATH (Jakarta -> Malang):" << endl;
            for (size_t i = 0; i < result.path.size(); ++i) {
                cout << result.path[i];
                if (i < result.path.size() - 1) cout << " -> ";
            }
            cout << "\nTotal distance: " << fixed << setprecision(1) << result.totalCost << " km" << endl;
        }
        
        // Print location coordinates
        cout << "\nLOCATION COORDINATES:" << endl;
        for (const auto& locPair : locations) {
            const Location& loc = locPair.second;
            cout << locPair.first << ": (" << loc.getX() << ", " << loc.getY() << ")" << endl;
        }
    }
};

int main() {
    Graph g;

    cout << "=== BUILDING TRANSPORTATION NETWORK ===" << endl;

    // Tambah lokasi dengan koordinat yang lebih tersebar
    g.addLocation("Jakarta", 100, 200);
    g.addLocation("Bandung", 300, 350);
    g.addLocation("Surabaya", 600, 150);
    g.addLocation("Semarang", 400, 250);
    g.addLocation("Yogyakarta", 350, 400);
    g.addLocation("Malang", 650, 300);
    g.addLocation("Medan", 50, 50);
    g.addLocation("Palembang", 200, 500);

    cout << "\n=== ADDING ROUTES ===" << endl;

    // Tambah rute untuk membuat graf yang terhubung
    g.addRoute("Jakarta", "Bandung", 180, 50000, 0);
    g.addRoute("Jakarta", "Semarang", 200, 60000, 0);
    g.addRoute("Jakarta", "Medan", 400, 120000, 1);
    g.addRoute("Bandung", "Yogyakarta", 120, 40000, 0);
    g.addRoute("Bandung", "Palembang", 300, 80000, 1);
    g.addRoute("Semarang", "Surabaya", 150, 45000, 0);
    g.addRoute("Semarang", "Yogyakarta", 90, 30000, 0);
    g.addRoute("Yogyakarta", "Malang", 180, 55000, 0);
    g.addRoute("Surabaya", "Malang", 60, 20000, 0);
    g.addRoute("Bandung", "Semarang", 100, 35000, 0);

    cout << "\n=== GRAPH DETAILS ===" << endl;
    g.displayGraph();

    cout << "\n=== ASCII VISUALIZATION ===" << endl;
    ASCIIGraphVisualizer visualizer(g);
    visualizer.visualize();

    return 0;
}
