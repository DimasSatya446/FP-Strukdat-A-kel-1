#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include "Location.h"
#include "Route.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

class GraphManager {
private:
    std::unordered_map<std::string, Location> locations;
    std::unordered_map<std::string, std::vector<Route>> adjacencyList;

public:
    GraphManager() = default;
    virtual ~GraphManager() = default;
    
    bool addLocation(const std::string& name, double x, double y) {
        if (locations.find(name) != locations.end()) {
            std::cout << "❌ Lokasi '" << name << "' sudah ada." << std::endl;
            return false;
        }
        
        locations.emplace(name, Location(name, x, y));
        adjacencyList[name] = std::vector<Route>();
        std::cout << "✅ Lokasi '" << name << "' di koordinat (" << x << ", " << y << ") berhasil ditambahkan." << std::endl;
        return true;
    }
    
    bool removeLocation(const std::string& name) {
        if (locations.find(name) == locations.end()) {
            std::cout << "❌ Lokasi '" << name << "' tidak ditemukan." << std::endl;
            return false;
        }
        
        locations.erase(name);
        adjacencyList.erase(name);
        
        for (auto& pair : adjacencyList) {
            auto& routes = pair.second;
            routes.erase(std::remove_if(routes.begin(), routes.end(),
                [&name](const Route& route) {
                    return route.getDestination() == name;
                }), routes.end());
        }
        
        std::cout << "✅ Lokasi '" << name << "' berhasil dihapus." << std::endl;
        return true;
    }
    
    bool addRoute(const std::string& sourceName, const std::string& destName,
                  double time, double cost) {
        if (locations.find(sourceName) == locations.end() || 
            locations.find(destName) == locations.end()) {
            std::cout << "❌ Lokasi asal atau tujuan tidak ditemukan." << std::endl;
            return false;
        }

        for (const auto& r : adjacencyList[sourceName]) {
            if (r.getDestination() == destName) {
                std::cout << "❌ Rute dari '" << sourceName << "' ke '" << destName << "' sudah ada." << std::endl;
                return false;
            }
        }

        double distance = locations.at(sourceName).distanceTo(locations.at(destName));
        
        adjacencyList[sourceName].emplace_back(sourceName, destName, distance, time, cost);
        std::cout << "✅ Rute dari '" << sourceName << "' ke '" << destName << "' berhasil ditambahkan." << std::endl;
        std::cout << "📏 Jarak otomatis: " << std::fixed << std::setprecision(1) << distance << " km" << std::endl;
        return true;
    }
    
    bool removeRoute(const std::string& sourceName, const std::string& destName) {
        if (adjacencyList.find(sourceName) == adjacencyList.end()) {
            std::cout << "❌ Lokasi asal tidak ditemukan." << std::endl;
            return false;
        }
        
        auto& routes = adjacencyList[sourceName];
        auto it = std::remove_if(routes.begin(), routes.end(),
            [&destName](const Route& route) {
                return route.getDestination() == destName;
            });
        
        if (it == routes.end()) {
            std::cout << "❌ Rute dari '" << sourceName << "' ke '" << destName << "' tidak ditemukan." << std::endl;
            return false;
        }
        
        routes.erase(it, routes.end());
        std::cout << "✅ Rute dari '" << sourceName << "' ke '" << destName << "' berhasil dihapus." << std::endl;
        return true;
    }
    
    bool updateRoute(const std::string& sourceName, const std::string& destName,
                     double newTime, double newCost) {
        if (adjacencyList.find(sourceName) == adjacencyList.end()) {
            std::cout << "❌ Lokasi asal tidak ditemukan." << std::endl;
            return false;
        }
        
        auto& routes = adjacencyList[sourceName];
        for (auto& route : routes) {
            if (route.getDestination() == destName) {
                route.setTime(newTime);
                route.setCost(newCost);
                std::cout << "✅ Rute berhasil diperbarui." << std::endl;
                return true;
            }
        }
        
        std::cout << "❌ Rute tidak ditemukan." << std::endl;
        return false;
    }
    
    const std::unordered_map<std::string, Location>& getLocations() const {
        return locations;
    }
    
    const std::vector<Route>& getRoutesFrom(const std::string& sourceName) const {
        static const std::vector<Route> empty_routes;
        auto it = adjacencyList.find(sourceName);
        if (it == adjacencyList.end()) {
            return empty_routes;
        }
        return it->second;
    }
    
    void displayGraph() const {
        std::cout << "\n🗺️  === REPRESENTASI GRAF RUTE ===" << std::endl;
        if (locations.empty()) {
            std::cout << "Graf kosong. Tidak ada lokasi atau rute." << std::endl;
            return;
        }

        for (const auto& pair : adjacencyList) {
            const std::string& sourceName = pair.first;
            const std::vector<Route>& routes = pair.second;
            const Location& loc = locations.at(sourceName);
            
            std::cout << "🏙️  " << sourceName 
                      << " [📍 (" << loc.getX() << ", " << loc.getY() << ")]" << std::endl;
            
            if (routes.empty()) {
                std::cout << "   ❌ Tidak ada rute keluar dari lokasi ini." << std::endl;
            } else {
                for (const Route& route : routes) {
                    std::cout << "   🚗 → " << route.getDestination()
                              << " | 📏 " << std::fixed << std::setprecision(1) << route.getDistance() << " km"
                              << " | ⏱️  " << route.getTime() << " menit"
                              << " | 💰 Rp " << (int)route.getCost() << std::endl;
                }
            }
            std::cout << std::endl;
        }
    }
    
    void listLocations() const {
        std::cout << "\n📍 === DAFTAR LOKASI ===" << std::endl;
        if (locations.empty()) {
            std::cout << "❌ Tidak ada lokasi yang tersedia." << std::endl;
            return;
        }
        
        int count = 1;
        for (const auto& pair : locations) {
            const Location& loc = pair.second;
            std::cout << "[" << count++ << "] 🏙️  " << pair.first 
                     << " 📍 (" << loc.getX() << ", " << loc.getY() << ")" << std::endl;
        }
        std::cout << "═══════════════════════════════════" << std::endl;
    }
    
    bool loadLocationsFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "❌ Gagal membuka file: " << filename << std::endl;
            return false;
        }

        std::string line;
        std::cout << "📂 Memuat lokasi dari " << filename << "..." << std::endl;
        
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::stringstream ss(line);
            std::string name, xStr, yStr;
            
            std::getline(ss, name, ',');
            std::getline(ss, xStr, ',');
            std::getline(ss, yStr);
            
            try {
                double x = std::stod(xStr);
                double y = std::stod(yStr);
                addLocation(name, x, y);
            } catch (const std::exception& e) {
                std::cerr << "❌ Error parsing line: " << line << std::endl;
            }
        }
        
        file.close();
        std::cout << "✅ Lokasi berhasil dimuat dari file!" << std::endl;
        return true;
    }
    
    void saveLocationsToCSV(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "❌ Gagal membuka file untuk menulis: " << filename << std::endl;
            return;
        }
        
        for (const auto& pair : locations) {
            const Location& loc = pair.second;
            file << loc.getName() << "," << loc.getX() << "," << loc.getY() << std::endl;
        }
        
        file.close();
        std::cout << "✅ Lokasi berhasil disimpan ke " << filename << std::endl;
    }
    
    size_t getLocationCount() const {
        return locations.size();
    }
    
    bool hasLocation(const std::string& name) const {
        return locations.find(name) != locations.end();
    }
};

#endif
