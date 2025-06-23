#include <iostream>
#include "Graph.h"
#include "Dijkstra.h"
#include "FileIO.h"

void showMenu() {
    std::cout << "\n=== SISTEM RUTE TRANSPORTASI ===\n";
    std::cout << "1. Tambah Lokasi\n";
    std::cout << "2. Tambah Rute\n";
    std::cout << "3. Hapus Lokasi\n";
    std::cout << "4. Hapus Rute\n";
    std::cout << "5. Tampilkan Graf\n";
    std::cout << "6. Cari Rute Terbaik\n";
    std::cout << "7. Cari Rute Berdasarkan Preferensi\n";
    std::cout << "8. Load Lokasi dari File\n";
    std::cout << "9. Load Rute dari File\n";
    std::cout << "10. Simpan Graf ke File\n";

    std::cout << "0. Keluar\n";
    std::cout << "Pilih menu: ";
}

int main() {
    Graph graph;
    int pilihan;

    do {
        showMenu();
        std::cin >> pilihan;
        std::cin.ignore();

        if (pilihan == 1) {
            std::string nama;
            std::cout << "Nama lokasi: ";
            std::getline(std::cin, nama);
            graph.addLocation(nama);
        }
        else if (pilihan == 2) {
            std::string asal, tujuan;
            double jarak, waktu, biaya;
            int transit;

            std::cout << "Lokasi asal     : ";
            std::getline(std::cin, asal);
            std::cout << "Lokasi tujuan   : ";
            std::getline(std::cin, tujuan);
            std::cout << "Jarak (km)      : ";
            std::cin >> jarak;
            std::cout << "Waktu (menit)   : ";
            std::cin >> waktu;
            std::cout << "Biaya (Rp)      : ";
            std::cin >> biaya;
            std::cout << "Jumlah transit  : ";
            std::cin >> transit;
            std::cin.ignore();

            graph.addRoute(asal, tujuan, jarak, waktu, biaya, transit);
        }
        else if (pilihan == 3) {
            std::string nama;
            std::cout << "Nama lokasi yang ingin dihapus: ";
            std::getline(std::cin, nama);
            graph.removeLocation(nama);
        }
        else if (pilihan == 4) {
            std::string asal, tujuan;
            std::cout << "Asal rute: ";
            std::getline(std::cin, asal);
            std::cout << "Tujuan rute: ";
            std::getline(std::cin, tujuan);
            graph.removeRoute(asal, tujuan);
        }
        else if (pilihan == 5) {
            graph.displayGraph();
        }

        else if (pilihan == 6) {
            std::string asal, tujuan, mode;
            std::cout << "Lokasi asal    : ";
            std::getline(std::cin, asal);
            std::cout << "Lokasi tujuan  : ";
            std::getline(std::cin, tujuan);
            std::cout << "Mode (jarak/waktu/biaya): ";
            std::getline(std::cin, mode);

            PathResult result = findShortestPath(graph, asal, tujuan, mode);
            if (!result.path.empty()) {
                std::cout << "Rute terbaik: ";
                for (size_t i = 0; i < result.path.size(); ++i) {
                    std::cout << result.path[i];
                    if (i < result.path.size() - 1) std::cout << " → ";
                }
                std::cout << "\nTotal " << mode << ": " << result.totalCost;
                if (mode == "biaya") std::cout << " rupiah";
                else if (mode == "waktu") std::cout << " menit";
                else if (mode == "jarak") std::cout << " km";
                std::cout << "\n";
            }
        }

        else if (pilihan == 7) {
            std::string asal, tujuan;
            std::cout << "Lokasi asal   : ";
            std::getline(std::cin, asal);
            std::cout << "Lokasi tujuan : ";
            std::getline(std::cin, tujuan);

            Preference pref;
            pref.inputPrioritas();
            pref.tampilkan();

            PathResult result = findBestRouteWithPreference(graph, asal, tujuan, pref);
            if (!result.path.empty()) {
                std::cout << "Rute terbaik (berdasarkan preferensi): ";
                for (size_t i = 0; i < result.path.size(); ++i) {
                    std::cout << result.path[i];
                    if (i < result.path.size() - 1) std::cout << " → ";
                }
                std::cout << "\nTotal skor gabungan: " << result.totalCost << "\n";
            }
        }

        else if (pilihan == 8) {
            std::string file;
            std::cout << "Masukkan nama file lokasi (contoh: locations.csv): ";
            std::getline(std::cin, file);
            loadLocationsFromFile(file, graph);
        }

        else if (pilihan == 9) {
            std::string file;
            std::cout << "Masukkan nama file rute (contoh: routes.csv): ";
            std::getline(std::cin, file);
            loadRoutesFromFile(file, graph);
        }

        else if (pilihan == 10) {
            std::string file;
            std::cout << "Simpan ke file (contoh: output.csv): ";
            std::getline(std::cin, file);
            saveGraphToFile(file, graph);
        }


        else if (pilihan == 0) {
            std::cout << "Keluar dari program.\n";
        }
        else {
            std::cout << "Pilihan tidak valid.\n";
        }

    } while (pilihan != 0);

    return 0;
}
