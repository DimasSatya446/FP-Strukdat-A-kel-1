#ifndef ROUTE_SIMULATOR_H
#define ROUTE_SIMULATOR_H

#include "GraphManager.h"
#include "DijkstraAlgorithm.h"
#include "PreferenceManager.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

class RouteSimulator {
private:
    const GraphManager& graph;
    const DijkstraAlgorithm& dijkstra;
    
public:
    RouteSimulator(const GraphManager& graph, const DijkstraAlgorithm& dijkstra) 
        : graph(graph), dijkstra(dijkstra) {}
    
    virtual ~RouteSimulator() = default;
    
    void simulateJourney(const PathResult& result, const string& mode) const {
        if (result.path.empty()) {
            cout << "❌ Tidak ada rute untuk disimulasikan." << endl;
            return;
        }
        
        cout << "\n✅ SIMULASI PERJALANAN DIMULAI!" << endl;
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
        
        displayDetailedJourney(result.path);
    }
    
    void simulateJourneyWithPreference(const PathResult& result, const PreferenceManager& pref) const {
        if (result.path.empty()) {
            cout << "❌ Tidak ada rute untuk disimulasikan." << endl;
            return;
        }
        
        cout << "\n✅ SIMULASI PERJALANAN DENGAN PREFERENSI!" << endl;
        cout << "🗺️  Rute: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            cout << result.path[i];
            if (i < result.path.size() - 1) cout << " - ";
        }
        cout << endl;
        
        cout << "📊 Total skor preferensi: " << fixed << setprecision(2) << result.totalCost << endl;
        
        pref.tampilkan();
        displayDetailedJourney(result.path);
    }
    
private:
    void displayDetailedJourney(const vector<string>& path) const {
        cout << "\n📋 DETAIL PERJALANAN LANGKAH DEMI LANGKAH:" << endl;
        cout << "═══════════════════════════════════════════════════" << endl;
        
        double totalDistance = 0, totalTime = 0, totalCost = 0;
        
        for (size_t i = 0; i < path.size() - 1; ++i) {
            const string& from = path[i];
            const string& to = path[i + 1];
            
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
        
        displayTotalSummary(totalDistance, totalTime, totalCost);
    }
    
    void displayTotalSummary(double totalDistance, double totalTime, double totalCost) const {
        cout << "═══════════════════════════════════════════════════" << endl;
        cout << "📊 RINGKASAN TOTAL PERJALANAN:" << endl;
        cout << "📏 Total Jarak: " << fixed << setprecision(1) << totalDistance << " km" << endl;
        cout << "⏱️  Total Waktu: " << totalTime << " menit (" << fixed << setprecision(1) << (totalTime/60.0) << " jam)" << endl;
        cout << "💰 Total Biaya: Rp " << (int)totalCost << endl;
        
        estimateArrivalTime(totalTime);
    }
    
    void estimateArrivalTime(double totalMinutes) const {
        int hours = static_cast<int>(totalMinutes / 60);
        int minutes = static_cast<int>(totalMinutes) % 60;
        
        cout << "🕐 Estimasi waktu tempuh: " << hours << " jam " << minutes << " menit" << endl;
        
        if (totalMinutes < 60) {
            cout << "⚡ Perjalanan singkat - dalam kota" << endl;
        } else if (totalMinutes < 180) {
            cout << "🚗 Perjalanan sedang - antar kota terdekat" << endl;
        } else if (totalMinutes < 360) {
            cout << "🛣️  Perjalanan jauh - lintas provinsi" << endl;
        } else {
            cout << "✈️  Perjalanan sangat jauh - pertimbangkan transportasi udara" << endl;
        }
    }
};

#endif
