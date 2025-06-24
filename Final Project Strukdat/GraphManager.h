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
using namespace std;

class GraphManager {
private:
    unordered_map<string, Location> locations;
    unordered_map<string, vector<Route>> adjacencyList;

public:
    GraphManager() = default;
    virtual ~GraphManager() = default;
    
    bool addLocation(const string& name, double x, double y) {
        if (locations.find(name) != locations.end()) {
            cout << "❌ Lokasi '" << name << "' sudah ada." << endl;
            return false;
        }
        
        locations.emplace(name, Location(name, x, y));
        adjacencyList[name] = vector<Route>();
        cout << "✅ Lokasi '" << name << "' di koordinat (" << x << ", " << y << ") berhasil ditambahkan." << endl;
        return true;
    }
    
    bool removeLocation(const string& name) {
        if (locations.find(name) == locations.end()) {
            cout << "❌ Lokasi '" << name << "' tidak ditemukan." << endl;
            return false;
        }
        
        locations.erase(name);
        adjacencyList.erase(name);
        
        for (auto& pair : adjacencyList) {
            auto& routes = pair.second;
            routes.erase(remove_if(routes.begin(), routes.end(),
                [&name](const Route& route) {
                    return route.getDestination() == name;
                }), routes.end());
        }
        
        cout << "✅ Lokasi '" << name << "' berhasil dihapus." << endl;
        return true;
    }
    
    bool addRoute(const string& sourceName, const string& destName,
                  double time, double cost) {
        if (locations.find(sourceName) == locations.end() || 
            locations.find(destName) == locations.end()) {
            cout << "❌ Lokasi asal atau tujuan tidak ditemukan." << endl;
            return false;
        }

        for (const auto& r : adjacencyList[sourceName]) {
            if (r.getDestination() == destName) {
                cout << "❌ Rute dari '" << sourceName << "' ke '" << destName << "' sudah ada." << endl;
                return false;
            }
        }

        double distance = locations.at(sourceName).distanceTo(locations.at(destName));
        
        adjacencyList[sourceName].emplace_back(sourceName, destName, distance, time, cost);
        cout << "✅ Rute dari '" << sourceName << "' ke '" << destName << "' berhasil ditambahkan." << endl;
        cout << "📏 Jarak otomatis: " << fixed << setprecision(1) << distance << " km" << endl;
        return true;
    }
    
    bool addBidirectionalRoute(const string& sourceName, const string& destName,
                              double time, double cost) {
        bool success1 = addRoute(sourceName, destName, time, cost);
        bool success2 = addRoute(destName, sourceName, time, cost);
        
        if (success1 && success2) {
            cout << "🔄 Rute bidirectional berhasil dibuat antara '" << sourceName << "' dan '" << destName << "'" << endl;
            return true;
        } else if (success1 || success2) {
            cout << "⚠️  Rute bidirectional sebagian berhasil dibuat." << endl;
            return true;
        }
        return false;
    }
    
    bool removeRoute(const string& sourceName, const string& destName) {
        if (adjacencyList.find(sourceName) == adjacencyList.end()) {
            cout << "❌ Lokasi asal tidak ditemukan." << endl;
            return false;
        }
        
        auto& routes = adjacencyList[sourceName];
        auto it = remove_if(routes.begin(), routes.end(),
            [&destName](const Route& route) {
                return route.getDestination() == destName;
            });
        
        if (it == routes.end()) {
            cout << "❌ Rute dari '" << sourceName << "' ke '" << destName << "' tidak ditemukan." << endl;
            return false;
        }
        
        routes.erase(it, routes.end());
        cout << "✅ Rute dari '" << sourceName << "' ke '" << destName << "' berhasil dihapus." << endl;
        return true;
    }
    
    bool updateRoute(const string& sourceName, const string& destName,
                     double newTime, double newCost) {
        if (adjacencyList.find(sourceName) == adjacencyList.end()) {
            cout << "❌ Lokasi asal tidak ditemukan." << endl;
            return false;
        }
        
        auto& routes = adjacencyList[sourceName];
        for (auto& route : routes) {
            if (route.getDestination() == destName) {
                route.setTime(newTime);
                route.setCost(newCost);
                cout << "✅ Rute berhasil diperbarui." << endl;
                return true;
            }
        }
        
        cout << "❌ Rute tidak ditemukan." << endl;
        return false;
    }
    
    const unordered_map<string, Location>& getLocations() const {
        return locations;
    }
    
    const vector<Route>& getRoutesFrom(const string& sourceName) const {
        static const vector<Route> empty_routes;
        auto it = adjacencyList.find(sourceName);
        if (it == adjacencyList.end()) {
            return empty_routes;
        }
        return it->second;
    }
    
    void displayGraph() const {
        cout << "\n🗺️  === REPRESENTASI GRAF RUTE ===" << endl;
        if (locations.empty()) {
            cout << "Graf kosong. Tidak ada lokasi atau rute." << endl;
            return;
        }

        for (const auto& pair : adjacencyList) {
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
    }
    
    void listLocations() const {
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
    
    bool loadLocationsFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "❌ Gagal membuka file: " << filename << endl;
            return false;
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
        return true;
    }
    
    void saveLocationsToCSV(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "❌ Gagal membuka file untuk menulis: " << filename << endl;
            return;
        }
        
        for (const auto& pair : locations) {
            const Location& loc = pair.second;
            file << loc.getName() << "," << loc.getX() << "," << loc.getY() << endl;
        }
        
        file.close();
        cout << "✅ Lokasi berhasil disimpan ke " << filename << endl;
    }
    
    size_t getLocationCount() const {
        return locations.size();
    }
    
    bool hasLocation(const string& name) const {
        return locations.find(name) != locations.end();
    }
};

#endif
