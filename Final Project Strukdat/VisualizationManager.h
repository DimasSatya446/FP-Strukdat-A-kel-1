#ifndef VISUALIZATION_MANAGER_H
#define VISUALIZATION_MANAGER_H

#include "GraphManager.h"
#include "DijkstraAlgorithm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

using namespace std;
class VisualizationManager {
private:
    const GraphManager& graph;
    const DijkstraAlgorithm& dijkstra;
    
public:
    VisualizationManager(const GraphManager& graph, const DijkstraAlgorithm& dijkstra) 
        : graph(graph), dijkstra(dijkstra) {}
    
    virtual ~VisualizationManager() = default;
    
    void displayTextVisualization() const {
        cout << "\n🎨 === VISUALISASI GRAF TEKSTUAL ===" << endl;
        
        if (graph.getLocations().empty()) {
            cout << "❌ Graf kosong - tidak ada data untuk divisualisasikan." << endl;
            return;
        }
        
        displayAdjacencyMatrix();
        displayTreeStructure();
    }
    
    void runGraphicalVisualization() {
        if (graph.getLocations().empty()) {
            cout << "❌ Tidak ada data untuk divisualisasikan. Load lokasi terlebih dahulu." << endl;
            return;
        }
        
        cout << "\n🎨 VISUALISASI GRAF GRAFIS" << endl;
        
        string startCity = "";
        string endCity = "";
        
        if (promptForPathHighlight(startCity, endCity)) {
            cout << "✅ Akan menampilkan jalur tercepat dari " << startCity << " ke " << endCity << endl;
        }
        
        if (exportDataForVisualization(startCity, endCity)) {
            launchGraphicalVisualizer();
        }
    }
    
private:
    void displayAdjacencyMatrix() const {
        const auto& locations = graph.getLocations();
        vector<string> locationNames;
        
        for (const auto& loc : locations) {
            locationNames.push_back(loc.first);
        }
        
        cout << "\n📊 MATRIKS ADJACENCY (Jarak):" << endl;
        cout << "     ";
        for (const auto& name : locationNames) {
            cout << setw(8) << name.substr(0, 6);
        }
        cout << endl;
        
        for (const auto& source : locationNames) {
            cout << setw(4) << source.substr(0, 4) << " ";
            
            for (const auto& dest : locationNames) {
                if (source == dest) {
                    cout << setw(8) << "0";
                } else {
                    double distance = findDirectDistance(source, dest);
                    if (distance > 0) {
                        cout << setw(8) << fixed << setprecision(1) << distance;
                    } else {
                        cout << setw(8) << "∞";
                    }
                }
            }
            cout << endl;
        }
    }
    
    void displayTreeStructure() const {
        cout << "\n🌳 STRUKTUR TREE GRAF:" << endl;
        
        for (const auto& loc : graph.getLocations()) {
            const string& sourceName = loc.first;
            const vector<Route>& routes = graph.getRoutesFrom(sourceName);
            
            cout << "├── " << sourceName << endl;
            
            for (size_t i = 0; i < routes.size(); ++i) {
                const Route& route = routes[i];
                bool isLast = (i == routes.size() - 1);
                
                cout << (isLast ? "    └── " : "    ├── ") 
                         << route.getDestination() 
                         << " [" << fixed << setprecision(1) << route.getDistance() << "km, "
                         << route.getTime() << "min, Rp" << (int)route.getCost() << "]" << endl;
            }
            
            if (routes.empty()) {
                cout << "    └── (tidak ada koneksi)" << endl;
            }
            cout << endl;
        }
    }
    
    double findDirectDistance(const string& source, const string& dest) const {
        const vector<Route>& routes = graph.getRoutesFrom(source);
        for (const Route& route : routes) {
            if (route.getDestination() == dest) {
                return route.getDistance();
            }
        }
        return -1;
    }
    
    bool promptForPathHighlight(string& startCity, string& endCity) {
        cout << "🔍 Pilih lokasi untuk menampilkan jalur tercepat (opsional):" << endl;
        cout << "[1] Ya, pilih lokasi awal dan tujuan" << endl;
        cout << "[2] Tidak, tampilkan graf saja" << endl;
        cout << "Pilihan (1-2): ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            return selectStartAndEndLocations(startCity, endCity);
        }
        
        return false;
    }
    
    bool selectStartAndEndLocations(string& startCity, string& endCity) {
        graph.listLocations();
        
        vector<string> locationNames;
        for (const auto& loc : graph.getLocations()) {
            locationNames.push_back(loc.first);
        }
        
        cout << "\n🚀 Pilih lokasi awal untuk jalur tercepat:" << endl;
        for (size_t i = 0; i < locationNames.size(); ++i) {
            cout << "[" << (i+1) << "] " << locationNames[i] << endl;
        }
        cout << "Pilihan lokasi awal (1-" << locationNames.size() << "): ";
        
        int startChoice;
        cin >> startChoice;
        if (startChoice >= 1 && startChoice <= static_cast<int>(locationNames.size())) {
            startCity = locationNames[startChoice - 1];
            
            cout << "\n🏁 Pilih lokasi tujuan:" << endl;
            for (size_t i = 0; i < locationNames.size(); ++i) {
                if (i != startChoice - 1) {
                    cout << "[" << (i+1) << "] " << locationNames[i] << endl;
                }
            }
            cout << "Pilihan lokasi tujuan (1-" << locationNames.size() << ", kecuali " << startChoice << "): ";
            
            int destChoice;
            cin >> destChoice;
            if (destChoice >= 1 && destChoice <= static_cast<int>(locationNames.size()) && destChoice != startChoice) {
                endCity = locationNames[destChoice - 1];
                return true;
            }
        }
        
        cout << "❌ Pilihan tidak valid, hanya menampilkan graf biasa" << endl;
        return false;
    }
    
    bool exportDataForVisualization(const string& startCity, const string& endCity) {
        if (!exportLocations() || !exportRoutes() || !exportPathSelection(startCity, endCity)) {
            return false;
        }
        
        cout << "✅ Data berhasil diekspor untuk visualisasi." << endl;
        return true;
    }
    
    bool exportLocations() const {
        ofstream tempFile("temp_locations.csv");
        if (!tempFile.is_open()) {
            cout << "❌ Gagal membuat file temp_locations.csv" << endl;
            return false;
        }
        
        for (const auto& loc : graph.getLocations()) {
            tempFile << loc.first << "," << loc.second.getX() << "," << loc.second.getY() << endl;
        }
        tempFile.close();
        return true;
    }
    
    bool exportRoutes() const {
        ofstream routeFile("temp_routes.csv");
        if (!routeFile.is_open()) {
            cout << "❌ Gagal membuat file temp_routes.csv" << endl;
            return false;
        }
        
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
        return true;
    }
    
    bool exportPathSelection(const string& startCity, const string& endCity) const {
        ofstream pathFile("temp_path.txt");
        if (!pathFile.is_open()) {
            cout << "❌ Gagal membuat file temp_path.txt" << endl;
            return false;
        }
        
        pathFile << startCity << "," << endCity << endl;
        pathFile.close();
        return true;
    }
    
    void launchGraphicalVisualizer() const {
        cout << "🚀 Meluncurkan visualizer grafis..." << endl;
        int result = system("./graph_visualizer");
        if (result != 0) {
            cout << "❌ Gagal menjalankan visualizer. Pastikan graph_visualizer sudah dikompilasi." << endl;
            cout << "💡 Kompilasi manual: g++ graph_visualizer.cpp -o graph_visualizer -lsfml-graphics -lsfml-window -lsfml-system" << endl;
        }
    }
};

#endif
