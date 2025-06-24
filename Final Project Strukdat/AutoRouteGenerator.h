#ifndef AUTO_ROUTE_GENERATOR_H
#define AUTO_ROUTE_GENERATOR_H

#include "GraphManager.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

class AutoRouteGenerator {
private:
    GraphManager& graph;
    
public:
    explicit AutoRouteGenerator(GraphManager& graph) : graph(graph) {}
    virtual ~AutoRouteGenerator() = default;
    
    enum class TransportMode {
        FAST,    
        NORMAL,    
        ECONOMY  
    };
    
    enum class ConnectionMethod {
        NEAREST_NEIGHBORS,  // 3 kota terdekat
        RADIUS_BASED       // Dalam radius tertentu
    };
    
    void generateRoutes(TransportMode mode, ConnectionMethod method, double radius = 0) {
        cout << "\n🤖 AUTO-GENERATE RUTE REALISTIS" << endl;
        
        const auto& locations = graph.getLocations();
        if (locations.size() < 2) {
            cout << "❌ Minimal diperlukan 2 lokasi untuk membuat rute." << endl;
            return;
        }
        
        auto multipliers = getMultipliers(mode);
        double timeMultiplier = multipliers.first;
        double costMultiplier = multipliers.second;
        
        cout << "🎯 Akan membuat rute antar kota yang berdekatan secara geografis..." << endl;
        cout << "📊 Total lokasi: " << locations.size() << endl;
        
        int routesCreated = 0;
        int routesSkipped = 0;
          if (method == ConnectionMethod::NEAREST_NEIGHBORS) {
            auto result = generateNearestNeighborRoutes(timeMultiplier, costMultiplier);
            routesCreated = result.first;
            routesSkipped = result.second;
        } else {
            auto result = generateRadiusBasedRoutes(timeMultiplier, costMultiplier, radius);
            routesCreated = result.first;
            routesSkipped = result.second;
        }
        
        displayGenerationSummary(routesCreated, routesSkipped, locations.size());
    }
    
    void ensureBidirectionalGraph() {
        cout << "\n🔄 MENGUBAH GRAF MENJADI BIDIRECTIONAL" << endl;
        
        const auto& locations = graph.getLocations();
        int routesAdded = 0;
        
        for (const auto& source : locations) {
            const vector<Route>& routes = graph.getRoutesFrom(source.first);
            
            for (const Route& route : routes) {
                string destName = route.getDestination();
                
                // Cek apakah rute balik sudah ada
                if (!routeExists(destName, source.first)) {
                    // Buat rute balik dengan nilai yang sama
                    if (graph.addRoute(destName, source.first, route.getTime(), route.getCost())) {
                        routesAdded++;
                    }
                }
            }
        }
        
        cout << "✅ Selesai! " << routesAdded << " rute balik ditambahkan." << endl;
        if (routesAdded == 0) {
            cout << "🎯 Graf sudah bidirectional!" << endl;
        }
    }
    
private:
    pair<double, double> getMultipliers(TransportMode mode) const {
        switch(mode) {
            case TransportMode::FAST:
                cout << "✅ Mode Cepat dipilih (waktu = jarak x 2, biaya = jarak x 5000)" << endl;
                return {2.0, 5000.0};
            case TransportMode::NORMAL:
                cout << "✅ Mode Normal dipilih (waktu = jarak x 3, biaya = jarak x 7500)" << endl;
                return {3.0, 7500.0};
            case TransportMode::ECONOMY:
                cout << "✅ Mode Ekonomis dipilih (waktu = jarak x 5, biaya = jarak x 3000)" << endl;
                return {5.0, 3000.0};
            default:
                return {3.0, 7500.0};
        }
    }
    
    pair<int, int> generateNearestNeighborRoutes(double timeMultiplier, double costMultiplier) {
        const auto& locations = graph.getLocations();
        int routesCreated = 0;
        int routesSkipped = 0;
        
        cout << "\n🌐 Menggunakan metode koneksi berdasarkan jarak terdekat (max 3 koneksi per kota)" << endl;
        
        for (const auto& source : locations) {
            vector<pair<double, string>> distances;
            
            for (const auto& dest : locations) {
                if (source.first != dest.first) {
                    double distance = source.second.distanceTo(dest.second);
                    distances.push_back({distance, dest.first});
                }
            }
            
            sort(distances.begin(), distances.end());
            int maxConnections = min(3, static_cast<int>(distances.size()));
            
            for (int i = 0; i < maxConnections; ++i) {
                string destName = distances[i].second;
                
                if (routeExists(source.first, destName)) {
                    routesSkipped++;
                    continue;
                }
                
                double distance = distances[i].first;
                double time = distance * timeMultiplier;
                double cost = distance * costMultiplier;
                  if (graph.addRoute(source.first, destName, time, cost)) {
                    routesCreated++;
                    // Tambah rute balik untuk membuat graf bidirectional
                    if (graph.addRoute(destName, source.first, time, cost)) {
                        routesCreated++;
                    }
                }
            }
        }
        
        return {routesCreated, routesSkipped};
    }
    
    pair<int, int> generateRadiusBasedRoutes(double timeMultiplier, double costMultiplier, double maxRadius) {
        const auto& locations = graph.getLocations();
        int routesCreated = 0;
        int routesSkipped = 0;
        
        cout << "\n📏 Menggunakan metode koneksi kota dalam radius " << maxRadius << " km" << endl;
        
        for (const auto& source : locations) {
            for (const auto& dest : locations) {
                if (source.first == dest.first) continue;
                
                double distance = source.second.distanceTo(dest.second);
                if (distance > maxRadius) continue;
                
                if (routeExists(source.first, dest.first)) {
                    routesSkipped++;
                    continue;
                }
                
                double time = distance * timeMultiplier;
                double cost = distance * costMultiplier;
                  if (graph.addRoute(source.first, dest.first, time, cost)) {
                    routesCreated++;
                    // Tambah rute balik untuk membuat graf bidirectional  
                    if (graph.addRoute(dest.first, source.first, time, cost)) {
                        routesCreated++;
                    }
                }
            }
        }
        
        return {routesCreated, routesSkipped};
    }
    
    bool routeExists(const string& sourceName, const string& destName) const {
        const vector<Route>& existingRoutes = graph.getRoutesFrom(sourceName);
        for (const Route& route : existingRoutes) {
            if (route.getDestination() == destName) {
                return true;
            }
        }
        return false;
    }
    
    void displayGenerationSummary(int routesCreated, int routesSkipped, size_t locationCount) const {
        cout << "\n✅ SELESAI!" << endl;
        cout << "📊 Statistik:" << endl;
        cout << "   🆕 Rute baru dibuat: " << routesCreated << endl;
        cout << "   ⏭️  Rute dilewati (sudah ada): " << routesSkipped << endl;
        cout << "   📍 Total lokasi: " << locationCount << endl;
        
        if (locationCount > 0) {
            cout << "   🔗 Rata-rata koneksi per kota: " << fixed << setprecision(1) 
                     << static_cast<double>(routesCreated) / locationCount << endl;
        }
    }
};

#endif
