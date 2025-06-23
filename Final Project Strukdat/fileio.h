#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <sstream>
#include <string>
#include "graph.h"

void loadLocationsFromFile(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Gagal membuka file lokasi: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            graph.addLocation(line);
        }
    }

    file.close();
    std::cout << "Lokasi berhasil dimuat dari file.\n";
}

void loadRoutesFromFile(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Gagal membuka file rute: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string source, dest;
        double dist, time, cost;
        int transit;

        std::getline(ss, source, ',');
        std::getline(ss, dest, ',');
        ss >> dist; ss.ignore();
        ss >> time; ss.ignore();
        ss >> cost; ss.ignore();
        ss >> transit;

        graph.addRoute(source, dest, dist, time, cost, transit);
    }

    file.close();
    std::cout << "Rute berhasil dimuat dari file.\n";
}

// Simpan semua rute (bonus)
void saveGraphToFile(const std::string& filename, const Graph& graph) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Gagal menyimpan ke file: " << filename << "\n";
        return;
    }

    for (const auto& loc : graph.getLocations()) {
        for (const auto& route : graph.getRoutesFrom(loc.first)) {
            file << route.getSource() << "," << route.getDestination() << ","
                 << route.getDistance() << "," << route.getTime() << ","
                 << route.getCost() << "," << route.getTransitCount() << "\n";
        }
    }

    file.close();
    std::cout << "Graf berhasil disimpan ke file.\n";
}

#endif
