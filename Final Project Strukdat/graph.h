#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>

class Location {
private:
    std::string name;
public:
    Location(const std::string& name) : name(name) {}
    std::string getName() const {
        return name;
    }
    bool operator<(const Location& other) const {
        return name < other.name;
    }
};

class Route {
private:
    std::string source;
    std::string destination;
    double distance;
    double time;
    double cost;
    int transit_count;
public:
    Route(const std::string& source, const std::string& destination,
          double distance, double time, double cost, int transit_count)
        : source(source), destination(destination), distance(distance),
          time(time), cost(cost), transit_count(transit_count) {}

    std::string getSource() const { return source; }
    std::string getDestination() const { return destination; }
    double getDistance() const { return distance; }
    double getTime() const { return time; }
    double getCost() const { return cost; }
    int getTransitCount() const { return transit_count; }

    void update(double newDistance, double newTime, double newCost, int newTransit) {
        distance = newDistance;
        time = newTime;
        cost = newCost;
        transit_count = newTransit;
    }
};

class Graph {
private:
    std::map<std::string, Location> locations;
    std::map<std::string, std::vector<Route>> adjList;

public:
    void addLocation(const std::string& name) {
        if (locations.find(name) == locations.end()) {
            locations.emplace(name, Location(name));
            adjList[name] = {};
            std::cout << "Lokasi '" << name << "' berhasil ditambahkan." << std::endl;
        } else {
            std::cout << "Lokasi '" << name << "' sudah ada." << std::endl;
        }
    }

    void removeLocation(const std::string& name) {
        if (locations.erase(name)) {
            adjList.erase(name);
            for (auto& pair : adjList) {
                auto& routes = pair.second;
                routes.erase(std::remove_if(routes.begin(), routes.end(),
                            [&name](const Route& r) { return r.getDestination() == name; }),
                            routes.end());
            }
            std::cout << "Lokasi '" << name << "' berhasil dihapus." << std::endl;
        } else {
            std::cout << "Lokasi '" << name << "' tidak ditemukan." << std::endl;
        }
    }

    void addRoute(const std::string& sourceName, const std::string& destName,
                  double distance, double time, double cost, int transit_count) {
        if (locations.find(sourceName) == locations.end() || locations.find(destName) == locations.end()) {
            std::cout << "Lokasi asal atau tujuan tidak ditemukan." << std::endl;
            return;
        }

        for (const auto& r : adjList[sourceName]) {
            if (r.getDestination() == destName) {
                std::cout << "Rute dari '" << sourceName << "' ke '" << destName << "' sudah ada." << std::endl;
                return;
            }
        }

        adjList[sourceName].emplace_back(sourceName, destName, distance, time, cost, transit_count);
        std::cout << "Rute dari '" << sourceName << "' ke '" << destName << "' berhasil ditambahkan." << std::endl;
    }

    void removeRoute(const std::string& sourceName, const std::string& destName) {
        if (adjList.find(sourceName) == adjList.end()) {
            std::cout << "Lokasi asal tidak ditemukan." << std::endl;
            return;
        }

        auto& routes = adjList[sourceName];
        auto before = routes.size();
        routes.erase(std::remove_if(routes.begin(), routes.end(),
                    [&destName](const Route& r) { return r.getDestination() == destName; }),
                    routes.end());

        if (before == routes.size()) {
            std::cout << "Rute dari '" << sourceName << "' ke '" << destName << "' tidak ditemukan." << std::endl;
        } else {
            std::cout << "Rute berhasil dihapus." << std::endl;
        }
    }

    void updateRoute(const std::string& sourceName, const std::string& destName,
                     double newDistance, double newTime, double newCost, int newTransit) {
        auto& routes = adjList[sourceName];
        for (auto& r : routes) {
            if (r.getDestination() == destName) {
                r.update(newDistance, newTime, newCost, newTransit);
                std::cout << "Rute dari '" << sourceName << "' ke '" << destName << "' berhasil diperbarui." << std::endl;
                return;
            }
        }
        std::cout << "Rute tidak ditemukan." << std::endl;
    }

    const std::map<std::string, Location>& getLocations() const {
        return locations;
    }

    const std::vector<Route>& getRoutesFrom(const std::string& sourceName) const {
        if (adjList.find(sourceName) == adjList.end()) {
            static const std::vector<Route> empty_routes;
            return empty_routes;
        }
        return adjList.at(sourceName);
    }

    void displayGraph() const {
        std::cout << "\n--- Representasi Graf Rute ---" << std::endl;
        if (locations.empty()) {
            std::cout << "Graf kosong. Tidak ada lokasi atau rute." << std::endl;
            return;
        }

        for (const auto& pair : adjList) {
            const std::string& sourceName = pair.first;
            const std::vector<Route>& routes = pair.second;
            std::cout << "Lokasi: " << sourceName << std::endl;
            if (routes.empty()) {
                std::cout << "  Tidak ada rute keluar dari lokasi ini." << std::endl;
            } else {
                for (const auto& route : routes) {
                    std::cout << "  -> Tujuan: " << route.getDestination()
                              << " | Jarak: " << route.getDistance() << " km"
                              << " | Waktu: " << route.getTime() << " menit"
                              << " | Biaya: Rp " << route.getCost()
                              << " | Transit: " << route.getTransitCount() << std::endl;
                }
            }
        }
        std::cout << "------------------------------" << std::endl;
    }

    void listLocations() const {
        std::cout << "Daftar Lokasi: ";
        for (const auto& pair : locations) {
            std::cout << pair.first << " ";
        }
        std::cout << std::endl;
    }
};

#endif