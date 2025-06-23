#ifndef ROUTE_SIMULATOR_H
#define ROUTE_SIMULATOR_H

#include "GraphManager.h"
#include "DijkstraAlgorithm.h"
#include "PreferenceManager.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class RouteSimulator {
private:
    const GraphManager& graph;
    const DijkstraAlgorithm& dijkstra;
    
public:
    RouteSimulator(const GraphManager& graph, const DijkstraAlgorithm& dijkstra) 
        : graph(graph), dijkstra(dijkstra) {}
    
    virtual ~RouteSimulator() = default;
    
    void simulateJourney(const PathResult& result, const std::string& mode) const {
        if (result.path.empty()) {
            std::cout << "❌ Tidak ada rute untuk disimulasikan." << std::endl;
            return;
        }
        
        std::cout << "\n✅ SIMULASI PERJALANAN DIMULAI!" << std::endl;
        std::cout << "🗺️  Rute: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            std::cout << result.path[i];
            if (i < result.path.size() - 1) std::cout << " - ";
        }
        std::cout << std::endl;
        
        std::cout << "📊 Total " << mode << ": " << std::fixed << std::setprecision(1) << result.totalCost;
        if (mode == "biaya") std::cout << " rupiah";
        else if (mode == "waktu") std::cout << " menit";
        else if (mode == "jarak") std::cout << " km";
        std::cout << std::endl;
        
        displayDetailedJourney(result.path);
    }
    
    void simulateJourneyWithPreference(const PathResult& result, const PreferenceManager& pref) const {
        if (result.path.empty()) {
            std::cout << "❌ Tidak ada rute untuk disimulasikan." << std::endl;
            return;
        }
        
        std::cout << "\n✅ SIMULASI PERJALANAN DENGAN PREFERENSI!" << std::endl;
        std::cout << "🗺️  Rute: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            std::cout << result.path[i];
            if (i < result.path.size() - 1) std::cout << " - ";
        }
        std::cout << std::endl;
        
        std::cout << "📊 Total skor preferensi: " << std::fixed << std::setprecision(2) << result.totalCost << std::endl;
        
        pref.tampilkan();
        displayDetailedJourney(result.path);
    }
    
private:
    void displayDetailedJourney(const std::vector<std::string>& path) const {
        std::cout << "\n📋 DETAIL PERJALANAN LANGKAH DEMI LANGKAH:" << std::endl;
        std::cout << "═══════════════════════════════════════════════════" << std::endl;
        
        double totalDistance = 0, totalTime = 0, totalCost = 0;
        
        for (size_t i = 0; i < path.size() - 1; ++i) {
            const std::string& from = path[i];
            const std::string& to = path[i + 1];
            
            const std::vector<Route>& routes = graph.getRoutesFrom(from);
            for (const Route& route : routes) {
                if (route.getDestination() == to) {
                    std::cout << "🔸 Langkah " << (i + 1) << ": " << from << " → " << to << std::endl;
                    std::cout << "   📏 Jarak: " << std::fixed << std::setprecision(1) << route.getDistance() << " km" << std::endl;
                    std::cout << "   ⏱️  Waktu: " << route.getTime() << " menit" << std::endl;
                    std::cout << "   💰 Biaya: Rp " << (int)route.getCost() << std::endl;
                    
                    totalDistance += route.getDistance();
                    totalTime += route.getTime();
                    totalCost += route.getCost();
                    std::cout << std::endl;
                    break;
                }
            }
        }
        
        displayTotalSummary(totalDistance, totalTime, totalCost);
    }
    
    void displayTotalSummary(double totalDistance, double totalTime, double totalCost) const {
        std::cout << "═══════════════════════════════════════════════════" << std::endl;
        std::cout << "📊 RINGKASAN TOTAL PERJALANAN:" << std::endl;
        std::cout << "📏 Total Jarak: " << std::fixed << std::setprecision(1) << totalDistance << " km" << std::endl;
        std::cout << "⏱️  Total Waktu: " << totalTime << " menit (" << std::fixed << std::setprecision(1) << (totalTime/60.0) << " jam)" << std::endl;
        std::cout << "💰 Total Biaya: Rp " << (int)totalCost << std::endl;
        
        estimateArrivalTime(totalTime);
    }
    
    void estimateArrivalTime(double totalMinutes) const {
        int hours = static_cast<int>(totalMinutes / 60);
        int minutes = static_cast<int>(totalMinutes) % 60;
        
        std::cout << "🕐 Estimasi waktu tempuh: " << hours << " jam " << minutes << " menit" << std::endl;
        
        if (totalMinutes < 60) {
            std::cout << "⚡ Perjalanan singkat - dalam kota" << std::endl;
        } else if (totalMinutes < 180) {
            std::cout << "🚗 Perjalanan sedang - antar kota terdekat" << std::endl;
        } else if (totalMinutes < 360) {
            std::cout << "🛣️  Perjalanan jauh - lintas provinsi" << std::endl;
        } else {
            std::cout << "✈️  Perjalanan sangat jauh - pertimbangkan transportasi udara" << std::endl;
        }
    }
};

#endif
