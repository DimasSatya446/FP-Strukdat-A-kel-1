#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
#include <set>
#include <iomanip>
using namespace std;

class SimpleLocation {
public:
    string name;
    double x, y;
    SimpleLocation(const string& n, double x, double y) : name(n), x(x), y(y) {}
    
    double distanceTo(const SimpleLocation& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx*dx + dy*dy);
    }
};

class SimpleRoute {
public:
    string source, destination;
    double distance, time, cost;
    
    SimpleRoute(const string& s, const string& d, double dist, double t, double c)
        : source(s), destination(d), distance(dist), time(t), cost(c) {}
};

// Simple Graph class for visualizer
class SimpleGraph {
public:
    map<string, SimpleLocation> locations;
    vector<SimpleRoute> routes;
    
    void loadLocations(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open " << filename << endl;
            return;
        }

        string line;
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
                locations.emplace(name, SimpleLocation(name, x, y));
            } catch (const exception& e) {
                cerr << "Error parsing: " << line << endl;
            }
        }
        file.close();
        cout << "Loaded " << locations.size() << " locations." << endl;
    }
    
    void loadRoutes(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Could not open " << filename << " (this is optional)" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string source, dest, distStr, timeStr, costStr;
            
            getline(ss, source, ',');
            getline(ss, dest, ',');
            getline(ss, distStr, ',');
            getline(ss, timeStr, ',');
            getline(ss, costStr);
            
            try {
                double dist = stod(distStr);
                double time = stod(timeStr);
                double cost = stod(costStr);
                
                routes.emplace_back(source, dest, dist, time, cost);
            } catch (const exception& e) {
                cerr << "Error parsing route: " << line << endl;
            }
        }
        file.close();
        cout << "Loaded " << routes.size() << " routes." << endl;
    }
};

// PathResult struct for visualizer
struct SimplePathResult {
    vector<string> path;
    double totalCost;
};

// Dijkstra algorithm for SimpleGraph
SimplePathResult findShortestPathSimple(const SimpleGraph& graph, const string& start,
                                       const string& end, const string& mode) {
    map<string, double> cost;
    map<string, string> previous;
    set<string> visited;

    auto cmp = [&cost](const string& a, const string& b) {
        return cost[a] > cost[b];
    };
    priority_queue<string, vector<string>, decltype(cmp)> pq(cmp);

    for (const auto& pair : graph.locations) {
        cost[pair.first] = numeric_limits<double>::infinity();
    }

    cost[start] = 0;
    pq.push(start);

    while (!pq.empty()) {
        string current = pq.top();
        pq.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        for (const auto& route : graph.routes) {
            if (route.source != current) continue;
            
            string neighbor = route.destination;
            double weight = 0;

            if (mode == "jarak") weight = route.distance;
            else if (mode == "waktu") weight = route.time;
            else if (mode == "biaya") weight = route.cost;
            else {
                cerr << "❌ Mode tidak dikenal!" << endl;
                return {};
            }

            double newCost = cost[current] + weight;
            if (newCost < cost[neighbor]) {
                cost[neighbor] = newCost;
                previous[neighbor] = current;
                pq.push(neighbor);
            }
        }
    }

    vector<string> path;
    string temp = end;
    while (temp != start && previous.find(temp) != previous.end()) {
        path.push_back(temp);
        temp = previous[temp];
    }

    if (temp == start) {
        path.push_back(start);
        reverse(path.begin(), path.end());
        return { path, cost[end] };
    }

    cout << "❌ Rute tidak ditemukan dari " << start << " ke " << end << endl;
    return { {}, 0 };
}

class GraphVisualizer {
private:
    sf::RenderWindow window;
    SimpleGraph& graph;
    float scale;
    float offsetX, offsetY;
    sf::Font font;
    
public:
    GraphVisualizer(SimpleGraph& g) : 
        window(sf::VideoMode(1200, 800), "Transportation Graph Visualizer"),
        graph(g), scale(2.0f), offsetX(100), offsetY(100) {
        
        // Load default font (SFML built-in)
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            // Fallback - create simple visualization without text
            cout << "Warning: Could not load font. Text will not be displayed." << endl;
        }
    }
    
    sf::Vector2f getScreenPosition(double x, double y) {
        return sf::Vector2f(x * scale + offsetX, y * scale + offsetY);
    }
    
    void drawLocation(const string& name, double x, double y, sf::Color color = sf::Color::Blue) {
        sf::Vector2f pos = getScreenPosition(x, y);
        
        // Draw circle for location
        sf::CircleShape circle(15);
        circle.setFillColor(color);
        circle.setOutlineThickness(2);
        circle.setOutlineColor(sf::Color::Black);
        circle.setPosition(pos.x - 15, pos.y - 15);
        window.draw(circle);
        
        // Draw location name
        if (font.getInfo().family != "") {
            sf::Text text;
            text.setFont(font);
            text.setString(name);
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::Black);
            text.setPosition(pos.x - 20, pos.y + 20);
            window.draw(text);
        }
        
        // Draw coordinates
        if (font.getInfo().family != "") {
            sf::Text coordText;
            coordText.setFont(font);
            coordText.setString("(" + to_string((int)x) + "," + to_string((int)y) + ")");
            coordText.setCharacterSize(10);
            coordText.setFillColor(sf::Color::Red);
            coordText.setPosition(pos.x - 25, pos.y + 35);
            window.draw(coordText);
        }
    }
    
    void drawRoute(const string& from, const string& to, sf::Color color = sf::Color::Green) {
        auto& locations = graph.locations;
        
        if (locations.find(from) == locations.end() || locations.find(to) == locations.end()) {
            return;
        }
        
        const SimpleLocation& fromLoc = locations.at(from);
        const SimpleLocation& toLoc = locations.at(to);
        
        sf::Vector2f fromPos = getScreenPosition(fromLoc.x, fromLoc.y);
        sf::Vector2f toPos = getScreenPosition(toLoc.x, toLoc.y);
        
        // Draw line
        sf::Vertex line[] = {
            sf::Vertex(fromPos, color),
            sf::Vertex(toPos, color)
        };
        window.draw(line, 2, sf::Lines);
        
        // Draw arrow head
        float dx = toPos.x - fromPos.x;
        float dy = toPos.y - fromPos.y;
        float length = sqrt(dx*dx + dy*dy);
        if (length > 0) {
            dx /= length;
            dy /= length;
            
            sf::Vector2f arrowPos = toPos - sf::Vector2f(dx * 20, dy * 20);
            sf::Vector2f perpendicular(-dy, dx);
            
            sf::Vertex arrow[] = {
                sf::Vertex(toPos, color),
                sf::Vertex(arrowPos + perpendicular * 5.0f, color),
                sf::Vertex(arrowPos - perpendicular * 5.0f, color),
                sf::Vertex(toPos, color)
            };
            window.draw(arrow, 4, sf::LinesStrip);
        }
        
        // Draw distance label
        if (font.getInfo().family != "") {
            sf::Vector2f midPos = (fromPos + toPos) / 2.0f;
            double distance = fromLoc.distanceTo(toLoc);
            
            sf::Text distText;
            distText.setFont(font);
            distText.setString(to_string((int)distance) + "km");
            distText.setCharacterSize(10);
            distText.setFillColor(sf::Color::Magenta);
            distText.setPosition(midPos.x, midPos.y - 10);
            window.draw(distText);
        }
    }
    
    void highlightPath(const vector<string>& path, sf::Color color = sf::Color::Red) {
        for (size_t i = 0; i < path.size() - 1; ++i) {
            drawRoute(path[i], path[i + 1], color);
        }
        
        // Highlight path nodes
        for (const string& node : path) {
            auto& locations = graph.locations;
            if (locations.find(node) != locations.end()) {
                const SimpleLocation& loc = locations.at(node);
                drawLocation(node, loc.x, loc.y, sf::Color::Yellow);
            }
        }
    }
    
    void drawLegend() {
        if (font.getInfo().family == "") return;
        
        sf::Text legend;
        legend.setFont(font);
        legend.setString("Legend:\nBlue = Location\nGreen = Start Point\nMagenta = End Point\nGray = Route\nRed = Shortest Path");
        legend.setCharacterSize(14);
        legend.setFillColor(sf::Color::Black);
        legend.setPosition(10, 10);
        
        sf::RectangleShape bg(sf::Vector2f(220, 120));
        bg.setFillColor(sf::Color(255, 255, 255, 200));
        bg.setPosition(5, 5);
        
        window.draw(bg);
        window.draw(legend);
    }
    
    void run() {
        // Baca pilihan lokasi dari file temp_path.txt
        SimplePathResult shortestPath;
        string startCity = "", endCity = "";
        
        ifstream pathFile("temp_path.txt");
        if (pathFile.is_open()) {
            string line;
            if (getline(pathFile, line)) {
                stringstream ss(line);
                getline(ss, startCity, ',');
                getline(ss, endCity);
                pathFile.close();
            }
        }
        
        // Jika ada pilihan lokasi yang valid, cari jalur tercepat
        if (!startCity.empty() && !endCity.empty() && 
            graph.locations.find(startCity) != graph.locations.end() && 
            graph.locations.find(endCity) != graph.locations.end()) {
            
            cout << "🔍 Mencari jalur tercepat dari " << startCity << " ke " << endCity << "..." << endl;
            shortestPath = findShortestPathSimple(graph, startCity, endCity, "jarak");
            
            if (!shortestPath.path.empty()) {
                cout << "✅ Jalur tercepat ditemukan: ";
                for (size_t i = 0; i < shortestPath.path.size(); ++i) {
                    cout << shortestPath.path[i];
                    if (i < shortestPath.path.size() - 1) cout << " → ";
                }
                cout << " (Total: " << fixed << setprecision(1) << shortestPath.totalCost << " km)" << endl;
            }
        } else {
            cout << "ℹ️  Tidak ada jalur khusus yang dipilih, menampilkan seluruh graf." << endl;
        }
        
        cout << "=== GRAPH VISUALIZATION ===" << endl;
        cout << "Controls:" << endl;
        cout << "- Mouse wheel: Zoom in/out" << endl;
        cout << "- Mouse drag: Pan view" << endl;
        cout << "- ESC: Exit" << endl;
        if (!shortestPath.path.empty()) {
            cout << "🔴 Jalur merah menunjukkan rute tercepat dari " << startCity << " ke " << endCity << endl;
        }
        
        sf::Vector2i lastMousePos;
        bool dragging = false;
        
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
                
                // Mouse controls
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.delta > 0) {
                        scale *= 1.1f;
                    } else {
                        scale *= 0.9f;
                    }
                }
                
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        dragging = true;
                        lastMousePos = sf::Mouse::getPosition(window);
                    }
                }
                
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        dragging = false;
                    }
                }
                
                if (event.type == sf::Event::MouseMoved && dragging) {
                    sf::Vector2i currentPos = sf::Mouse::getPosition(window);
                    offsetX += currentPos.x - lastMousePos.x;
                    offsetY += currentPos.y - lastMousePos.y;
                    lastMousePos = currentPos;
                }
            }
            
            // Clear screen
            window.clear(sf::Color::White);
            
            // Draw all routes first (so they appear behind nodes)
            for (const SimpleRoute& route : graph.routes) {
                drawRoute(route.source, route.destination, sf::Color(128, 128, 128)); // Gray color
            }
            
            // Draw shortest path (highlighted in red)
            if (!shortestPath.path.empty()) {
                highlightPath(shortestPath.path, sf::Color::Red);
            }
            
            // Draw all locations (so they appear on top)
            for (const auto& locPair : graph.locations) {
                const SimpleLocation& loc = locPair.second;
                sf::Color nodeColor = sf::Color::Blue;
                
                // Highlight start and end nodes
                if (!shortestPath.path.empty()) {
                    if (locPair.first == shortestPath.path.front()) nodeColor = sf::Color::Green;
                    if (locPair.first == shortestPath.path.back()) nodeColor = sf::Color::Magenta;
                }
                
                drawLocation(locPair.first, loc.x, loc.y, nodeColor);
            }
            
            // Draw legend
            drawLegend();
            
            // Display
            window.display();
        }
    }
};

// Function to load locations with coordinates from CSV file
void loadLocationsWithCoordinates(const string& filename, SimpleGraph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open locations file: " << filename << endl;
        cerr << "Creating default locations..." << endl;
        
        // Fallback: create default locations
        graph.locations.emplace("Jakarta", SimpleLocation("Jakarta", 100, 200));
        graph.locations.emplace("Bandung", SimpleLocation("Bandung", 300, 350));
        graph.locations.emplace("Surabaya", SimpleLocation("Surabaya", 600, 150));
        graph.locations.emplace("Semarang", SimpleLocation("Semarang", 400, 250));
        graph.locations.emplace("Yogyakarta", SimpleLocation("Yogyakarta", 350, 400));
        graph.locations.emplace("Malang", SimpleLocation("Malang", 650, 300));
        return;
    }

    string line;
    cout << "Loading locations from " << filename << "..." << endl;
    
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
            graph.locations.emplace(name, SimpleLocation(name, x, y));
        } catch (const exception& e) {
            cerr << "Error parsing line: " << line << endl;
        }
    }
    
    file.close();
    cout << "Locations loaded successfully!" << endl;
}

int main() {
    SimpleGraph g;

    cout << "=== LOADING DATA FROM FILES ===" << endl;
    
    // Load locations with coordinates from CSV
    loadLocationsWithCoordinates("temp_locations.csv", g);
    
    cout << "\nLoading routes from temp_routes.csv..." << endl;
    // Load routes from CSV file
    g.loadRoutes("temp_routes.csv");

    // Tampilkan graf dalam teks
    cout << "=== TEXT REPRESENTATION ===" << endl;
    cout << "Locations loaded: " << g.locations.size() << endl;
    cout << "Routes loaded: " << g.routes.size() << endl;
    
    for (const auto& loc : g.locations) {
        cout << "- " << loc.first << " at (" << loc.second.x << ", " << loc.second.y << ")" << endl;
    }

    // Launch graphical visualization
    GraphVisualizer visualizer(g);
    visualizer.run();

    return 0;
}
