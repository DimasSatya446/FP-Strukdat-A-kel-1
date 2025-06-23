#ifndef AUTO_ROUTE_GENERATOR_H
#define AUTO_ROUTE_GENERATOR_H

#include "GraphManager.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

class AutoRouteGenerator {
private:
    GraphManager& graph;
    
public:
    explicit AutoRouteGenerator(GraphManager& graph) : graph(graph) {}
    virtual ~AutoRouteGenerator() = default;
    
    enum class TransportMode {
        FAST,    // Mode Cepat
        NORMAL,  // Mode Normal  
        ECONOMY  // Mode Ekonomis
    };
    
    enum class ConnectionMethod {
        NEAREST_NEIGHBORS,  // 3 kota terdekat
        RADIUS_BASED       // Dalam radius tertentu
    };
    
    void generateRoutes(TransportMode mode, ConnectionMethod method, double radius = 0) {
        std::cout << "\n🤖 AUTO-GENERATE RUTE REALISTIS" << std::endl;
        
        const auto& locations = graph.getLocations();
        if (locations.size() < 2) {
            std::cout << "❌ Minimal diperlukan 2 lokasi untuk membuat rute." << std::endl;
            return;
        }
        
        auto [timeMultiplier, costMultiplier] = getMultipliers(mode);
        
        std::cout << "🎯 Akan membuat rute antar kota yang berdekatan secara geografis..." << std::endl;
        std::cout << "📊 Total lokasi: " << locations.size() << std::endl;
        
        int routesCreated = 0;
        int routesSkipped = 0;
        
        if (method == ConnectionMethod::NEAREST_NEIGHBORS) {
            std::tie(routesCreated, routesSkipped) = generateNearestNeighborRoutes(timeMultiplier, costMultiplier);
        } else {
            std::tie(routesCreated, routesSkipped) = generateRadiusBasedRoutes(timeMultiplier, costMultiplier, radius);
        }
        
        displayGenerationSummary(routesCreated, routesSkipped, locations.size());
    }
    
private:
    std::pair<double, double> getMultipliers(TransportMode mode) const {
        switch(mode) {
            case TransportMode::FAST:
                std::cout << "✅ Mode Cepat dipilih (waktu = jarak x 2, biaya = jarak x 5000)" << std::endl;
                return {2.0, 5000.0};
            case TransportMode::NORMAL:
                std::cout << "✅ Mode Normal dipilih (waktu = jarak x 3, biaya = jarak x 7500)" << std::endl;
                return {3.0, 7500.0};
            case TransportMode::ECONOMY:
                std::cout << "✅ Mode Ekonomis dipilih (waktu = jarak x 5, biaya = jarak x 3000)" << std::endl;
                return {5.0, 3000.0};
            default:
                return {3.0, 7500.0};
        }
    }
    
    std::pair<int, int> generateNearestNeighborRoutes(double timeMultiplier, double costMultiplier) {
        const auto& locations = graph.getLocations();
        int routesCreated = 0;
        int routesSkipped = 0;
        
        std::cout << "\n🌐 Menggunakan metode koneksi berdasarkan jarak terdekat (max 3 koneksi per kota)" << std::endl;
        
        for (const auto& source : locations) {
            std::vector<std::pair<double, std::string>> distances;
            
            for (const auto& dest : locations) {
                if (source.first != dest.first) {
                    double distance = source.second.distanceTo(dest.second);
                    distances.push_back({distance, dest.first});
                }
            }
            
            std::sort(distances.begin(), distances.end());
            int maxConnections = std::min(3, static_cast<int>(distances.size()));
            
            for (int i = 0; i < maxConnections; ++i) {
                std::string destName = distances[i].second;
                
                if (routeExists(source.first, destName)) {
                    routesSkipped++;
                    continue;
                }
                
                double distance = distances[i].first;
                double time = distance * timeMultiplier;
                double cost = distance * costMultiplier;
                
                if (graph.addRoute(source.first, destName, time, cost)) {
                    routesCreated++;
                }
            }
        }
        
        return {routesCreated, routesSkipped};
    }
    
    std::pair<int, int> generateRadiusBasedRoutes(double timeMultiplier, double costMultiplier, double maxRadius) {
        const auto& locations = graph.getLocations();
        int routesCreated = 0;
        int routesSkipped = 0;
        
        std::cout << "\n📏 Menggunakan metode koneksi kota dalam radius " << maxRadius << " km" << std::endl;
        
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
                }
            }
        }
        
        return {routesCreated, routesSkipped};
    }
    
    bool routeExists(const std::string& sourceName, const std::string& destName) const {
        const std::vector<Route>& existingRoutes = graph.getRoutesFrom(sourceName);
        for (const Route& route : existingRoutes) {
            if (route.getDestination() == destName) {
                return true;
            }
        }
        return false;
    }
    
    void displayGenerationSummary(int routesCreated, int routesSkipped, size_t locationCount) const {
        std::cout << "\n✅ SELESAI!" << std::endl;
        std::cout << "📊 Statistik:" << std::endl;
        std::cout << "   🆕 Rute baru dibuat: " << routesCreated << std::endl;
        std::cout << "   ⏭️  Rute dilewati (sudah ada): " << routesSkipped << std::endl;
        std::cout << "   📍 Total lokasi: " << locationCount << std::endl;
        
        if (locationCount > 0) {
            std::cout << "   🔗 Rata-rata koneksi per kota: " << std::fixed << std::setprecision(1) 
                     << static_cast<double>(routesCreated) / locationCount << std::endl;
        }
    }
};

#endif
