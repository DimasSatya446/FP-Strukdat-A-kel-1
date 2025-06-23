#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstdlib>
using namespace std;

class Location {
private:
    string name;
    double x, y;
public:
    Location(const string& name, double x = 0, double y = 0) 
        : name(name), x(x), y(y) {}
    
    string getName() const { return name; }
    double getX() const { return x; }
    double getY() const { return y; }
    
    double distanceTo(const Location& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx*dx + dy*dy);
    }
    
    bool operator<(const Location& other) const {
        return name < other.name;
    }
};

class Route {
private:
    string source, destination;
    double distance, time, cost;
public:
    Route(const string& source, const string& destination,
          double distance, double time, double cost)
        : source(source), destination(destination), distance(distance),
          time(time), cost(cost) {}

    string getSource() const { return source; }
    string getDestination() const { return destination; }
    double getDistance() const { return distance; }
    double getTime() const { return time; }
    double getCost() const { return cost; }
};

class Graph {
private:
    map<string, Location> locations;
    map<string, vector<Route>> adjList;

public:
    void addLocation(const string& name, double x, double y) {
        if (locations.find(name) == locations.end()) {
            locations.emplace(name, Location(name, x, y));
            adjList[name] = {};
            cout << "✅ Lokasi '" << name << "' di koordinat (" << x << ", " << y << ") berhasil ditambahkan." << endl;
        } else {
            cout << "❌ Lokasi '" << name << "' sudah ada." << endl;
        }
    }    void addRoute(const string& sourceName, const string& destName,
                  double time, double cost) {
        if (locations.find(sourceName) == locations.end() || locations.find(destName) == locations.end()) {
            cout << "❌ Lokasi asal atau tujuan tidak ditemukan." << endl;
            return;
        }

        for (const auto& r : adjList[sourceName]) {
            if (r.getDestination() == destName) {
                cout << "❌ Rute dari '" << sourceName << "' ke '" << destName << "' sudah ada." << endl;
                return;
            }
        }

        double distance = locations.at(sourceName).distanceTo(locations.at(destName));
        
        adjList[sourceName].emplace_back(sourceName, destName, distance, time, cost);
        cout << "✅ Rute dari '" << sourceName << "' ke '" << destName << "' berhasil ditambahkan." << endl;
        cout << "📏 Jarak otomatis: " << fixed << setprecision(1) << distance << " km" << endl;
    }

    const map<string, Location>& getLocations() const { return locations; }
    
    const vector<Route>& getRoutesFrom(const string& sourceName) const {
        if (adjList.find(sourceName) == adjList.end()) {
            static const vector<Route> empty_routes;
            return empty_routes;
        }
        return adjList.at(sourceName);
    }

    void displayGraph() const {
        cout << "\n🗺️  === REPRESENTASI GRAF RUTE ===" << endl;
        if (locations.empty()) {
            cout << "Graf kosong. Tidak ada lokasi atau rute." << endl;
            return;
        }

        for (const auto& pair : adjList) {
            const string& sourceName = pair.first;
            const vector<Route>& routes = pair.second;
            const Location& loc = locations.at(sourceName);
            
            cout << "🏙️  " << sourceName 
                  << " [📍 (" << loc.getX() << ", " << loc.getY() << ")]" << endl;
              if (routes.empty()) {
                cout << "   ❌ Tidak ada rute keluar dari lokasi ini." << endl;
            } else {
                for (const Route& route : routes) {
                    cout << "   🚗 → " << route.getDestination()
                          << " | 📏 " << fixed << setprecision(1) << route.getDistance() << " km"
                          << " | ⏱️  " << route.getTime() << " menit"
                          << " | 💰 Rp " << (int)route.getCost() << endl;
                }
            }
            cout << endl;
        }
    }    void listLocations() const {
        cout << "\n📍 === DAFTAR LOKASI ===" << endl;
        if (locations.empty()) {
            cout << "❌ Tidak ada lokasi yang tersedia." << endl;
            return;
        }
        
        int count = 1;
        for (const auto& pair : locations) {
            const Location& loc = pair.second;
            cout << "[" << count++ << "] 🏙️  " << pair.first 
                 << " 📍 (" << loc.getX() << ", " << loc.getY() << ")" << endl;
        }
        cout << "═══════════════════════════════════" << endl;
    }
    
    void loadLocationsFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "❌ Gagal membuka file: " << filename << endl;
            return;
        }

        string line;
        cout << "📂 Memuat lokasi dari " << filename << "..." << endl;
        
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
                addLocation(name, x, y);
            } catch (const exception& e) {
                cerr << "❌ Error parsing line: " << line << endl;
            }
        }
        
        file.close();
        cout << "✅ Lokasi berhasil dimuat dari file!" << endl;
    }
};

struct PathResult {
    vector<string> path;
    double totalCost;
};

PathResult findShortestPath(const Graph& graph, const string& start,
                            const string& end, const string& mode) {
    map<string, double> cost;
    map<string, string> previous;
    set<string> visited;

    auto cmp = [&cost](const string& a, const string& b) {
        return cost[a] > cost[b];
    };
    priority_queue<string, vector<string>, decltype(cmp)> pq(cmp);

    for (const auto& pair : graph.getLocations()) {
        cost[pair.first] = numeric_limits<double>::infinity();
    }

    cost[start] = 0;
    pq.push(start);

    while (!pq.empty()) {
        string current = pq.top();
        pq.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        for (const auto& route : graph.getRoutesFrom(current)) {
            string neighbor = route.getDestination();
            double weight = 0;            if (mode == "jarak") weight = route.getDistance();
            else if (mode == "waktu") weight = route.getTime();
            else if (mode == "biaya") weight = route.getCost();
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

void showMenu() {
    cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                        SISTEM RUTE TRANSPORTASI                             ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << endl;
    cout << "1. 📂 Load Lokasi dari File CSV" << endl;
    cout << "2. 🏙️  Tampilkan Daftar Lokasi" << endl;
    cout << "3. ➕ Tambah Lokasi Baru" << endl;
    cout << "4. 🛤️  Sambungkan Dua Lokasi (Tambah Rute)" << endl;
    cout << "5. 🤖 Auto-Generate Rute Realistis" << endl;
    cout << "6. 🗺️  Tampilkan Graf Lengkap" << endl;
    cout << "7. 🚀 Cari Rute Terbaik" << endl;
    cout << "8. 🎨 Visualisasi Graf" << endl;
    cout << "0. ❌ Keluar" << endl;
    cout << "\nPilih menu: ";
    cout.flush();
}

void selectAndConnectLocations(Graph& graph) {
    graph.listLocations();
    
    if (graph.getLocations().empty()) {
        cout << "❌ Tidak ada lokasi yang tersedia. Load lokasi dari file terlebih dahulu." << endl;
        return;
    }
    
    // Convert locations to vector for indexing
    vector<string> locationNames;
    for (const auto& loc : graph.getLocations()) {
        locationNames.push_back(loc.first);
    }
    
    cout << "\n🔗 SAMBUNGKAN DUA LOKASI" << endl;
    
    // Select source location
    cout << "\n📍 Pilih lokasi asal:" << endl;
    for (size_t i = 0; i < locationNames.size(); ++i) {
        cout << "[" << (i+1) << "] " << locationNames[i] << endl;
    }
    cout << "Pilihan lokasi asal (1-" << locationNames.size() << "): ";
    
    int asalChoice;
    cin >> asalChoice;
    if (asalChoice < 1 || asalChoice > (int)locationNames.size()) {
        cout << "❌ Pilihan tidak valid." << endl;
        return;
    }
    string asal = locationNames[asalChoice - 1];
    
    // Select destination location
    cout << "\n🎯 Pilih lokasi tujuan:" << endl;
    for (size_t i = 0; i < locationNames.size(); ++i) {
        if (i != asalChoice - 1) { // Don't show the same location as source
            cout << "[" << (i+1) << "] " << locationNames[i] << endl;
        }
    }
    cout << "Pilihan lokasi tujuan (1-" << locationNames.size() << ", kecuali " << asalChoice << "): ";
    
    int tujuanChoice;
    cin >> tujuanChoice;
    if (tujuanChoice < 1 || tujuanChoice > (int)locationNames.size() || tujuanChoice == asalChoice) {
        cout << "❌ Pilihan tidak valid atau sama dengan lokasi asal." << endl;
        return;
    }
    string tujuan = locationNames[tujuanChoice - 1];
      cout << "\n✅ Terpilih: " << asal << " ➜ " << tujuan << endl;
    
    double waktu, biaya;
    
    cout << "Waktu tempuh (menit): ";
    cin >> waktu;
    cout << "Biaya perjalanan (Rp): ";
    cin >> biaya;
    
    graph.addRoute(asal, tujuan, waktu, biaya);
}

void findBestRoute(Graph& graph) {
    if (graph.getLocations().empty()) {
        cout << "❌ Tidak ada lokasi yang tersedia." << endl;
        return;
    }
    
    // Check if there are any routes
    bool hasRoutes = false;
    for (const auto& loc : graph.getLocations()) {
        if (!graph.getRoutesFrom(loc.first).empty()) {
            hasRoutes = true;
            break;
        }
    }
    
    if (!hasRoutes) {
        cout << "❌ Tidak ada rute yang tersedia. Sambungkan lokasi terlebih dahulu." << endl;
        return;
    }
    
    graph.listLocations();
    
    // Convert locations to vector for indexing
    vector<string> locationNames;
    for (const auto& loc : graph.getLocations()) {
        locationNames.push_back(loc.first);
    }
    
    cout << "\n🎯 PENCARIAN RUTE TERBAIK" << endl;
    
    // Select start location
    cout << "\n🚀 Pilih lokasi awal:" << endl;
    for (size_t i = 0; i < locationNames.size(); ++i) {
        cout << "[" << (i+1) << "] " << locationNames[i] << endl;
    }
    cout << "Pilihan lokasi awal (1-" << locationNames.size() << "): ";
    
    int startChoice;
    cin >> startChoice;
    if (startChoice < 1 || startChoice > (int)locationNames.size()) {
        cout << "❌ Pilihan tidak valid." << endl;
        return;
    }
    string asal = locationNames[startChoice - 1];
    
    // Select destination location
    cout << "\n🏁 Pilih lokasi tujuan:" << endl;
    for (size_t i = 0; i < locationNames.size(); ++i) {
        if (i != startChoice - 1) { // Don't show the same location as start
            cout << "[" << (i+1) << "] " << locationNames[i] << endl;
        }
    }
    cout << "Pilihan lokasi tujuan (1-" << locationNames.size() << ", kecuali " << startChoice << "): ";
    
    int destChoice;
    cin >> destChoice;
    if (destChoice < 1 || destChoice > (int)locationNames.size() || destChoice == startChoice) {
        cout << "❌ Pilihan tidak valid atau sama dengan lokasi awal." << endl;
        return;
    }
    string tujuan = locationNames[destChoice - 1];
    
    cout << "\n✅ Rute terpilih: " << asal << " ➜ " << tujuan << endl;
      cout << "\n⚙️  Pilih mode prioritas:" << endl;
    cout << "[1] 📏 Jarak terpendek" << endl;
    cout << "[2] ⏱️  Waktu tercepat" << endl;
    cout << "[3] 💰 Biaya termurah" << endl;
    cout << "Pilihan mode (1-3): ";
    
    int modeChoice;
    cin >> modeChoice;
    
    string mode;
    string modeDesc;
    switch(modeChoice) {
        case 1: 
            mode = "jarak"; 
            modeDesc = "jarak terpendek";
            break;
        case 2: 
            mode = "waktu"; 
            modeDesc = "waktu tercepat";
            break;
        case 3: 
            mode = "biaya"; 
            modeDesc = "biaya termurah";
            break;
        default: 
            cout << "❌ Pilihan tidak valid." << endl;
            return;
    }
    
    cout << "\n🔍 Mencari rute dengan prioritas " << modeDesc << "..." << endl;
    
    PathResult result = findShortestPath(graph, asal, tujuan, mode);    if (!result.path.empty()) {
        cout << "\n✅ RUTE DITEMUKAN!" << endl;
        
        // Show simple route path
        cout << "🗺️  Rute: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            cout << result.path[i];
            if (i < result.path.size() - 1) cout << " - ";
        }
        cout << endl;
        
        cout << "📊 Total " << mode << ": " << fixed << setprecision(1) << result.totalCost;
        if (mode == "biaya") cout << " rupiah";
        else if (mode == "waktu") cout << " menit";
        else if (mode == "jarak") cout << " km";
        cout << endl;
        
        // Show detailed route with all intermediate stops
        cout << "\n📋 DETAIL PERJALANAN:" << endl;
        cout << "═══════════════════════════════════════════════════" << endl;
        
        double totalDistance = 0, totalTime = 0, totalCost = 0;
        
        for (size_t i = 0; i < result.path.size() - 1; ++i) {
            const string& from = result.path[i];
            const string& to = result.path[i + 1];
            
            const vector<Route>& routes = graph.getRoutesFrom(from);
            for (const Route& route : routes) {
                if (route.getDestination() == to) {
                    cout << "🔸 Langkah " << (i + 1) << ": " << from << " → " << to << endl;
                    cout << "   📏 Jarak: " << fixed << setprecision(1) << route.getDistance() << " km" << endl;
                    cout << "   ⏱️  Waktu: " << route.getTime() << " menit" << endl;
                    cout << "   💰 Biaya: Rp " << (int)route.getCost() << endl;
                    
                    totalDistance += route.getDistance();
                    totalTime += route.getTime();
                    totalCost += route.getCost();
                    cout << endl;
                    break;
                }
            }
        }
        
        cout << "═══════════════════════════════════════════════════" << endl;
        cout << "📊 TOTAL PERJALANAN:" << endl;
        cout << "📏 Total Jarak: " << fixed << setprecision(1) << totalDistance << " km" << endl;
        cout << "⏱️  Total Waktu: " << totalTime << " menit (" << fixed << setprecision(1) << (totalTime/60.0) << " jam)" << endl;
        cout << "💰 Total Biaya: Rp " << (int)totalCost << endl;
        cout << "🎯 Optimasi berdasarkan: " << modeDesc << endl;
    }
}

void runVisualization(Graph& graph) {
    if (graph.getLocations().empty()) {
        cout << "❌ Tidak ada data untuk divisualisasikan. Load lokasi terlebih dahulu." << endl;
        return;
    }
    
    cout << "\n🎨 VISUALISASI GRAF" << endl;
    
    // Pilih lokasi untuk highlight shortest path
    string startCity = "", endCity = "";
    
    cout << "🔍 Pilih lokasi untuk menampilkan jalur tercepat (opsional):" << endl;
    cout << "[1] Ya, pilih lokasi awal dan tujuan" << endl;
    cout << "[2] Tidak, tampilkan graf saja" << endl;
    cout << "Pilihan (1-2): ";
    
    int choice;
    cin >> choice;
    
    if (choice == 1) {
        graph.listLocations();
        
        // Convert locations to vector for indexing
        vector<string> locationNames;
        for (const auto& loc : graph.getLocations()) {
            locationNames.push_back(loc.first);
        }
        
        // Select start location
        cout << "\n🚀 Pilih lokasi awal untuk jalur tercepat:" << endl;
        for (size_t i = 0; i < locationNames.size(); ++i) {
            cout << "[" << (i+1) << "] " << locationNames[i] << endl;
        }
        cout << "Pilihan lokasi awal (1-" << locationNames.size() << "): ";
        
        int startChoice;
        cin >> startChoice;
        if (startChoice >= 1 && startChoice <= (int)locationNames.size()) {
            startCity = locationNames[startChoice - 1];
            
            // Select destination location
            cout << "\n🏁 Pilih lokasi tujuan:" << endl;
            for (size_t i = 0; i < locationNames.size(); ++i) {
                if (i != startChoice - 1) {
                    cout << "[" << (i+1) << "] " << locationNames[i] << endl;
                }
            }
            cout << "Pilihan lokasi tujuan (1-" << locationNames.size() << ", kecuali " << startChoice << "): ";
            
            int destChoice;
            cin >> destChoice;
            if (destChoice >= 1 && destChoice <= (int)locationNames.size() && destChoice != startChoice) {
                endCity = locationNames[destChoice - 1];
                cout << "✅ Akan menampilkan jalur tercepat dari " << startCity << " ke " << endCity << endl;
            } else {
                cout << "❌ Pilihan tidak valid, hanya menampilkan graf biasa" << endl;
                startCity = "";
                endCity = "";
            }
        } else {
            cout << "❌ Pilihan tidak valid, hanya menampilkan graf biasa" << endl;
        }
    }
    
    cout << "\n🎨 Menjalankan visualisasi graf..." << endl;
    
    // Save current graph to temporary file for visualizer
    ofstream tempFile("temp_locations.csv");
    if (tempFile.is_open()) {
        for (const auto& loc : graph.getLocations()) {
            tempFile << loc.first << "," << loc.second.getX() << "," << loc.second.getY() << endl;
        }
        tempFile.close();
        
        ofstream routeFile("temp_routes.csv");
        if (routeFile.is_open()) {
            for (const auto& locPair : graph.getLocations()) {
                const string& sourceName = locPair.first;
                const vector<Route>& routes = graph.getRoutesFrom(sourceName);
                
                for (const Route& route : routes) {
                    routeFile << route.getSource() << "," << route.getDestination() << ","
                             << route.getDistance() << "," << route.getTime() << ","
                             << route.getCost() << endl;
                }
            }
            routeFile.close();
            
            // Save path selection to temp file
            ofstream pathFile("temp_path.txt");
            if (pathFile.is_open()) {
                pathFile << startCity << "," << endCity << endl;
                pathFile.close();
            }
            
            cout << "✅ Data disimpan untuk visualisasi." << endl;
            
            // Jalankan visualizer secara otomatis
            cout << "🚀 Meluncurkan visualizer..." << endl;
            int result = system("./graph_visualizer");
            if (result != 0) {
                cout << "❌ Gagal menjalankan visualizer. Pastikan graph_visualizer sudah dikompilasi." << endl;
                cout << "💡 Jalankan manual: g++ graph_visualizer.cpp -o graph_visualizer -lsfml-graphics -lsfml-window -lsfml-system" << endl;
            }
        }
    }
}

void addNewLocation(Graph& graph) {
    cout << "\n➕ TAMBAH LOKASI BARU" << endl;
    
    string namaLokasi;
    double x, y;
    
    cout << "Nama lokasi: ";
    cin.ignore();
    getline(cin, namaLokasi);
    
    // Check if location already exists
    if (graph.getLocations().find(namaLokasi) != graph.getLocations().end()) {
        cout << "❌ Lokasi '" << namaLokasi << "' sudah ada!" << endl;
        return;
    }
    
    cout << "Koordinat X: ";
    cin >> x;
    cout << "Koordinat Y: ";
    cin >> y;
    
    // Add to graph
    graph.addLocation(namaLokasi, x, y);
    
    // Append to locations.csv file
    ofstream file("locations.csv", ios::app);
    if (file.is_open()) {
        file << namaLokasi << "," << x << "," << y << endl;
        file.close();
        cout << "✅ Lokasi berhasil disimpan ke file locations.csv" << endl;
    } else {
        cout << "⚠️  Lokasi ditambahkan ke sistem tapi gagal menyimpan ke file CSV" << endl;
    }
}

void autoGenerateAllRoutes(Graph& graph) {
    cout << "\n🤖 AUTO-GENERATE RUTE REALISTIS" << endl;
    
    const auto& locations = graph.getLocations();
    if (locations.size() < 2) {
        cout << "❌ Minimal diperlukan 2 lokasi untuk membuat rute." << endl;
        return;
    }
    
    cout << "🎯 Akan membuat rute antar kota yang berdekatan secara geografis..." << endl;
    cout << "📊 Total lokasi: " << locations.size() << endl;
    
    cout << "\nPilih mode untuk menghitung waktu dan biaya:" << endl;
    cout << "[1] 🏎️  Mode Cepat (waktu = jarak x 2, biaya = jarak x 5000)" << endl;
    cout << "[2] 🚗 Mode Normal (waktu = jarak x 3, biaya = jarak x 7500)" << endl;
    cout << "[3] 🚌 Mode Ekonomis (waktu = jarak x 5, biaya = jarak x 3000)" << endl;
    cout << "Pilihan mode (1-3): ";
    
    int modeChoice;
    cin >> modeChoice;
    
    double timeMultiplier, costMultiplier;
    
    switch(modeChoice) {
        case 1:
            timeMultiplier = 2.0;
            costMultiplier = 5000.0;
            cout << "✅ Mode Cepat dipilih" << endl;
            break;
        case 2:
            timeMultiplier = 3.0;
            costMultiplier = 7500.0;
            cout << "✅ Mode Normal dipilih" << endl;
            break;
        case 3:
            timeMultiplier = 5.0;
            costMultiplier = 3000.0;
            cout << "✅ Mode Ekonomis dipilih" << endl;
            break;
        default:
            cout << "❌ Pilihan tidak valid, menggunakan mode Normal" << endl;
            timeMultiplier = 3.0;
            costMultiplier = 7500.0;
            break;
    }
    
    cout << "\n🔧 Pilih metode koneksi:" << endl;
    cout << "[1] 🌐 Koneksi berdasarkan jarak terdekat (max 3 koneksi per kota)" << endl;
    cout << "[2] 📏 Koneksi kota dalam radius tertentu" << endl;
    cout << "Pilihan metode (1-2): ";
    
    int connectionMethod;
    cin >> connectionMethod;
    
    cout << "\n🚀 Memulai pembuatan rute..." << endl;
    int routesCreated = 0;
    int routesSkipped = 0;
    
    if (connectionMethod == 1) {
        // Method 1: Connect each city to its 3 nearest neighbors
        for (const auto& source : locations) {
            // Calculate distances to all other cities
            vector<pair<double, string>> distances;
            for (const auto& dest : locations) {
                if (source.first != dest.first) {
                    double distance = source.second.distanceTo(dest.second);
                    distances.push_back({distance, dest.first});
                }
            }
            
            // Sort by distance and take the 3 closest
            sort(distances.begin(), distances.end());
            int maxConnections = min(3, (int)distances.size());
            
            for (int i = 0; i < maxConnections; ++i) {
                string destName = distances[i].second;
                
                // Check if route already exists
                bool routeExists = false;
                const vector<Route>& existingRoutes = graph.getRoutesFrom(source.first);
                for (const Route& route : existingRoutes) {
                    if (route.getDestination() == destName) {
                        routeExists = true;
                        break;
                    }
                }
                
                if (routeExists) {
                    routesSkipped++;
                    continue;
                }
                
                double distance = distances[i].first;
                double time = distance * timeMultiplier;
                double cost = distance * costMultiplier;
                
                graph.addRoute(source.first, destName, time, cost);
                routesCreated++;
            }
        }
    } else {
        // Method 2: Connect cities within a radius
        cout << "Masukkan radius maksimal koneksi (km): ";
        double maxRadius;
        cin >> maxRadius;
        
        for (const auto& source : locations) {
            for (const auto& dest : locations) {
                if (source.first == dest.first) continue;
                
                double distance = source.second.distanceTo(dest.second);
                if (distance > maxRadius) continue; // Skip if too far
                
                // Check if route already exists
                bool routeExists = false;
                const vector<Route>& existingRoutes = graph.getRoutesFrom(source.first);
                for (const Route& route : existingRoutes) {
                    if (route.getDestination() == dest.first) {
                        routeExists = true;
                        break;
                    }
                }
                
                if (routeExists) {
                    routesSkipped++;
                    continue;
                }
                
                double time = distance * timeMultiplier;
                double cost = distance * costMultiplier;
                
                graph.addRoute(source.first, dest.first, time, cost);
                routesCreated++;
            }
        }
    }
    
    cout << "\n✅ SELESAI!" << endl;
    cout << "📊 Statistik:" << endl;
    cout << "   🆕 Rute baru dibuat: " << routesCreated << endl;
    cout << "   ⏭️  Rute dilewati (sudah ada): " << routesSkipped << endl;
    cout << "   📍 Total lokasi: " << locations.size() << endl;
    cout << "   🔗 Rata-rata koneksi per kota: " << fixed << setprecision(1) 
         << (double)routesCreated / locations.size() << endl;
}

int main() {
    Graph graph;
    int pilihan;

    cout << "🚀 Selamat datang di Sistem Rute Transportasi!" << endl;

    do {
        showMenu();
        cin >> pilihan;        if (pilihan == 1) {
            string filename;
            cout << "📂 Masukkan nama file CSV (contoh: locations.csv): ";
            cin.ignore();
            getline(cin, filename);
            graph.loadLocationsFromCSV(filename);
        }
        else if (pilihan == 2) {
            graph.listLocations();
        }
        else if (pilihan == 3) {
            addNewLocation(graph);
        }
        else if (pilihan == 4) {
            selectAndConnectLocations(graph);
        }
        else if (pilihan == 5) {
            autoGenerateAllRoutes(graph);
        }
        else if (pilihan == 6) {
            graph.displayGraph();
        }
        else if (pilihan == 7) {
            findBestRoute(graph);
        }
        else if (pilihan == 8) {
            runVisualization(graph);
        }
        else if (pilihan == 0) {
            cout << "👋 Terima kasih telah menggunakan sistem ini!" << endl;
        }
        else {
            cout << "❌ Pilihan tidak valid." << endl;
        }

        if (pilihan != 0) {
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore();
            cin.get();
        }

    } while (pilihan != 0);

    return 0;
}
