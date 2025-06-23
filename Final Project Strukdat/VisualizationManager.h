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

class VisualizationManager {
private:
    const GraphManager& graph;
    const DijkstraAlgorithm& dijkstra;
    
public:
    VisualizationManager(const GraphManager& graph, const DijkstraAlgorithm& dijkstra) 
        : graph(graph), dijkstra(dijkstra) {}
    
    virtual ~VisualizationManager() = default;
    
    void displayTextVisualization() const {
        std::cout << "\n🎨 === VISUALISASI GRAF TEKSTUAL ===" << std::endl;
        
        if (graph.getLocations().empty()) {
            std::cout << "❌ Graf kosong - tidak ada data untuk divisualisasikan." << std::endl;
            return;
        }
        
        displayAdjacencyMatrix();
        displayTreeStructure();
    }
    
    void runGraphicalVisualization() {
        if (graph.getLocations().empty()) {
            std::cout << "❌ Tidak ada data untuk divisualisasikan. Load lokasi terlebih dahulu." << std::endl;
            return;
        }
        
        std::cout << "\n🎨 VISUALISASI GRAF GRAFIS" << std::endl;
        
        std::string startCity = "";
        std::string endCity = "";
        
        if (promptForPathHighlight(startCity, endCity)) {
            std::cout << "✅ Akan menampilkan jalur tercepat dari " << startCity << " ke " << endCity << std::endl;
        }
        
        if (exportDataForVisualization(startCity, endCity)) {
            launchGraphicalVisualizer();
        }
    }
    
private:
    void displayAdjacencyMatrix() const {
        const auto& locations = graph.getLocations();
        std::vector<std::string> locationNames;
        
        for (const auto& loc : locations) {
            locationNames.push_back(loc.first);
        }
        
        std::cout << "\n📊 MATRIKS ADJACENCY (Jarak):" << std::endl;
        std::cout << "     ";
        for (const auto& name : locationNames) {
            std::cout << std::setw(8) << name.substr(0, 6);
        }
        std::cout << std::endl;
        
        for (const auto& source : locationNames) {
            std::cout << std::setw(4) << source.substr(0, 4) << " ";
            
            for (const auto& dest : locationNames) {
                if (source == dest) {
                    std::cout << std::setw(8) << "0";
                } else {
                    double distance = findDirectDistance(source, dest);
                    if (distance > 0) {
                        std::cout << std::setw(8) << std::fixed << std::setprecision(1) << distance;
                    } else {
                        std::cout << std::setw(8) << "∞";
                    }
                }
            }
            std::cout << std::endl;
        }
    }
    
    void displayTreeStructure() const {
        std::cout << "\n🌳 STRUKTUR TREE GRAF:" << std::endl;
        
        for (const auto& loc : graph.getLocations()) {
            const std::string& sourceName = loc.first;
            const std::vector<Route>& routes = graph.getRoutesFrom(sourceName);
            
            std::cout << "├── " << sourceName << std::endl;
            
            for (size_t i = 0; i < routes.size(); ++i) {
                const Route& route = routes[i];
                bool isLast = (i == routes.size() - 1);
                
                std::cout << (isLast ? "    └── " : "    ├── ") 
                         << route.getDestination() 
                         << " [" << std::fixed << std::setprecision(1) << route.getDistance() << "km, "
                         << route.getTime() << "min, Rp" << (int)route.getCost() << "]" << std::endl;
            }
            
            if (routes.empty()) {
                std::cout << "    └── (tidak ada koneksi)" << std::endl;
            }
            std::cout << std::endl;
        }
    }
    
    double findDirectDistance(const std::string& source, const std::string& dest) const {
        const std::vector<Route>& routes = graph.getRoutesFrom(source);
        for (const Route& route : routes) {
            if (route.getDestination() == dest) {
                return route.getDistance();
            }
        }
        return -1;
    }
    
    bool promptForPathHighlight(std::string& startCity, std::string& endCity) {
        std::cout << "🔍 Pilih lokasi untuk menampilkan jalur tercepat (opsional):" << std::endl;
        std::cout << "[1] Ya, pilih lokasi awal dan tujuan" << std::endl;
        std::cout << "[2] Tidak, tampilkan graf saja" << std::endl;
        std::cout << "Pilihan (1-2): ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            return selectStartAndEndLocations(startCity, endCity);
        }
        
        return false;
    }
    
    bool selectStartAndEndLocations(std::string& startCity, std::string& endCity) {
        graph.listLocations();
        
        std::vector<std::string> locationNames;
        for (const auto& loc : graph.getLocations()) {
            locationNames.push_back(loc.first);
        }
        
        std::cout << "\n🚀 Pilih lokasi awal untuk jalur tercepat:" << std::endl;
        for (size_t i = 0; i < locationNames.size(); ++i) {
            std::cout << "[" << (i+1) << "] " << locationNames[i] << std::endl;
        }
        std::cout << "Pilihan lokasi awal (1-" << locationNames.size() << "): ";
        
        int startChoice;
        std::cin >> startChoice;
        if (startChoice >= 1 && startChoice <= static_cast<int>(locationNames.size())) {
            startCity = locationNames[startChoice - 1];
            
            std::cout << "\n🏁 Pilih lokasi tujuan:" << std::endl;
            for (size_t i = 0; i < locationNames.size(); ++i) {
                if (i != startChoice - 1) {
                    std::cout << "[" << (i+1) << "] " << locationNames[i] << std::endl;
                }
            }
            std::cout << "Pilihan lokasi tujuan (1-" << locationNames.size() << ", kecuali " << startChoice << "): ";
            
            int destChoice;
            std::cin >> destChoice;
            if (destChoice >= 1 && destChoice <= static_cast<int>(locationNames.size()) && destChoice != startChoice) {
                endCity = locationNames[destChoice - 1];
                return true;
            }
        }
        
        std::cout << "❌ Pilihan tidak valid, hanya menampilkan graf biasa" << std::endl;
        return false;
    }
    
    bool exportDataForVisualization(const std::string& startCity, const std::string& endCity) {
        if (!exportLocations() || !exportRoutes() || !exportPathSelection(startCity, endCity)) {
            return false;
        }
        
        std::cout << "✅ Data berhasil diekspor untuk visualisasi." << std::endl;
        return true;
    }
    
    bool exportLocations() const {
        std::ofstream tempFile("temp_locations.csv");
        if (!tempFile.is_open()) {
            std::cout << "❌ Gagal membuat file temp_locations.csv" << std::endl;
            return false;
        }
        
        for (const auto& loc : graph.getLocations()) {
            tempFile << loc.first << "," << loc.second.getX() << "," << loc.second.getY() << std::endl;
        }
        tempFile.close();
        return true;
    }
    
    bool exportRoutes() const {
        std::ofstream routeFile("temp_routes.csv");
        if (!routeFile.is_open()) {
            std::cout << "❌ Gagal membuat file temp_routes.csv" << std::endl;
            return false;
        }
        
        for (const auto& locPair : graph.getLocations()) {
            const std::string& sourceName = locPair.first;
            const std::vector<Route>& routes = graph.getRoutesFrom(sourceName);
            
            for (const Route& route : routes) {
                routeFile << route.getSource() << "," << route.getDestination() << ","
                         << route.getDistance() << "," << route.getTime() << ","
                         << route.getCost() << std::endl;
            }
        }
        routeFile.close();
        return true;
    }
    
    bool exportPathSelection(const std::string& startCity, const std::string& endCity) const {
        std::ofstream pathFile("temp_path.txt");
        if (!pathFile.is_open()) {
            std::cout << "❌ Gagal membuat file temp_path.txt" << std::endl;
            return false;
        }
        
        pathFile << startCity << "," << endCity << std::endl;
        pathFile.close();
        return true;
    }
    
    void launchGraphicalVisualizer() const {
        std::cout << "🚀 Meluncurkan visualizer grafis..." << std::endl;
        int result = system("./graph_visualizer");
        if (result != 0) {
            std::cout << "❌ Gagal menjalankan visualizer. Pastikan graph_visualizer sudah dikompilasi." << std::endl;
            std::cout << "💡 Kompilasi manual: g++ graph_visualizer.cpp -o graph_visualizer -lsfml-graphics -lsfml-window -lsfml-system" << std::endl;
        }
    }
};

#endif
