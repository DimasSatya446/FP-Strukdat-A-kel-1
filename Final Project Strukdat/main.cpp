#include "GraphManager.h"
#include "DijkstraAlgorithm.h"
#include "RouteSimulator.h"
#include "AutoRouteGenerator.h"
#include "VisualizationManager.h"
#include "PreferenceManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>

class TransportationSystem {
private:
    GraphManager graphManager;
    DijkstraAlgorithm dijkstraAlgorithm;
    RouteSimulator routeSimulator;
    AutoRouteGenerator autoGenerator;
    VisualizationManager visualizationManager;
    PreferenceManager preferenceManager;

public:
    TransportationSystem() 
        : dijkstraAlgorithm(graphManager),
          routeSimulator(graphManager, dijkstraAlgorithm),
          autoGenerator(graphManager),
          visualizationManager(graphManager, dijkstraAlgorithm) {}

    void run() {
        std::cout << "🚀 Selamat datang di Sistem Rute Transportasi!" << std::endl;
        
        int pilihan;
        do {
            showMenu();
            std::cin >> pilihan;
            
            switch(pilihan) {
                case 1: handleLoadLocations(); break;
                case 2: handleListLocations(); break;
                case 3: handleAddLocation(); break;
                case 4: handleAddRoute(); break;
                case 5: handleRemoveLocation(); break;
                case 6: handleRemoveRoute(); break;
                case 7: handleUpdateRoute(); break;
                case 8: handleAutoGenerateRoutes(); break;
                case 9: handleDisplayGraph(); break;
                case 10: handleFindBestRoute(); break;
                case 11: handleFindRouteWithPreference(); break;
                case 12: handleTextVisualization(); break;
                case 13: handleGraphicalVisualization(); break;
                case 0: std::cout << "👋 Terima kasih telah menggunakan sistem ini!" << std::endl; break;
                default: std::cout << "❌ Pilihan tidak valid." << std::endl; break;
            }
            
            if (pilihan != 0) {
                std::cout << "\nTekan Enter untuk melanjutkan...";
                std::cin.ignore();
                std::cin.get();
            }
            
        } while (pilihan != 0);
    }

private:
    void showMenu() {
        std::cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                        SISTEM RUTE TRANSPORTASI                             ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << "=== MANAJEMEN DATA ===" << std::endl;
        std::cout << "1.  📂 Load Lokasi dari File CSV" << std::endl;
        std::cout << "2.  🏙️  Tampilkan Daftar Lokasi" << std::endl;
        std::cout << "3.  ➕ Tambah Lokasi Baru" << std::endl;
        std::cout << "4.  🛤️  Tambah Rute Baru" << std::endl;
        std::cout << "5.  🗑️  Hapus Lokasi" << std::endl;
        std::cout << "6.  🗑️  Hapus Rute" << std::endl;
        std::cout << "7.  ✏️  Update Rute" << std::endl;
        std::cout << "8.  🤖 Auto-Generate Rute" << std::endl;
        std::cout << "\n=== PENCARIAN & SIMULASI ===" << std::endl;
        std::cout << "9.  🗺️  Tampilkan Graf Lengkap" << std::endl;
        std::cout << "10. 🚀 Cari Rute Terbaik (Single Mode)" << std::endl;
        std::cout << "11. 🎯 Cari Rute dengan Preferensi Multi-kriteria" << std::endl;
        std::cout << "\n=== VISUALISASI ===" << std::endl;
        std::cout << "12. 📊 Visualisasi Teks (Graf & Tree)" << std::endl;
        std::cout << "13. 🎨 Visualisasi Grafis (SFML)" << std::endl;
        std::cout << "\n0.  ❌ Keluar" << std::endl;
        std::cout << "\nPilih menu: ";
        std::cout.flush();
    }
    
    void handleLoadLocations() {
        std::string filename;
        std::cout << "📂 Masukkan nama file CSV (contoh: locations.csv): ";
        std::cin.ignore();
        std::getline(std::cin, filename);
        graphManager.loadLocationsFromCSV(filename);
    }
    
    void handleListLocations() {
        graphManager.listLocations();
    }
    
    void handleAddLocation() {
        std::cout << "\n➕ TAMBAH LOKASI BARU" << std::endl;
        
        std::string namaLokasi;
        double x, y;
        
        std::cout << "Nama lokasi: ";
        std::cin.ignore();
        std::getline(std::cin, namaLokasi);
        
        std::cout << "Koordinat X: ";
        std::cin >> x;
        std::cout << "Koordinat Y: ";
        std::cin >> y;
        
        if (graphManager.addLocation(namaLokasi, x, y)) {
            std::ofstream file("locations.csv", std::ios::app);
            if (file.is_open()) {
                file << namaLokasi << "," << x << "," << y << std::endl;
                file.close();
                std::cout << "✅ Lokasi berhasil disimpan ke file locations.csv" << std::endl;
            } else {
                std::cout << "⚠️  Lokasi ditambahkan ke sistem tapi gagal menyimpan ke file CSV" << std::endl;
            }
        }
    }
    
    void handleAddRoute() {
        if (!hasLocations()) return;
        
        graphManager.listLocations();
        
        std::vector<std::string> locationNames = getLocationNames();
        
        std::cout << "\n🔗 TAMBAH RUTE BARU" << std::endl;
        
        std::string asal = selectLocation("📍 Pilih lokasi asal:", locationNames);
        if (asal.empty()) return;
        
        std::string tujuan = selectLocation("🎯 Pilih lokasi tujuan:", locationNames, asal);
        if (tujuan.empty()) return;
        
        std::cout << "\n✅ Terpilih: " << asal << " ➜ " << tujuan << std::endl;
        
        double waktu, biaya;
        std::cout << "Waktu tempuh (menit): ";
        std::cin >> waktu;
        std::cout << "Biaya perjalanan (Rp): ";
        std::cin >> biaya;
        
        graphManager.addRoute(asal, tujuan, waktu, biaya);
    }
    
    void handleRemoveLocation() {
        if (!hasLocations()) return;
        
        graphManager.listLocations();
        
        std::vector<std::string> locationNames = getLocationNames();
        std::string lokasi = selectLocation("🗑️  Pilih lokasi yang akan dihapus:", locationNames);
        
        if (!lokasi.empty()) {
            graphManager.removeLocation(lokasi);
        }
    }
    
    void handleRemoveRoute() {
        if (!hasLocations()) return;
        
        graphManager.listLocations();
        
        std::vector<std::string> locationNames = getLocationNames();
        
        std::string asal = selectLocation("📍 Pilih lokasi asal rute yang akan dihapus:", locationNames);
        if (asal.empty()) return;
        
        std::string tujuan = selectLocation("🎯 Pilih lokasi tujuan rute yang akan dihapus:", locationNames, asal);
        if (tujuan.empty()) return;
        
        graphManager.removeRoute(asal, tujuan);
    }
    
    void handleUpdateRoute() {
        if (!hasLocations()) return;
        
        graphManager.listLocations();
        
        std::vector<std::string> locationNames = getLocationNames();
        
        std::string asal = selectLocation("📍 Pilih lokasi asal rute yang akan diupdate:", locationNames);
        if (asal.empty()) return;
        
        std::string tujuan = selectLocation("🎯 Pilih lokasi tujuan rute yang akan diupdate:", locationNames, asal);
        if (tujuan.empty()) return;
        
        double waktu, biaya;
        std::cout << "Waktu tempuh baru (menit): ";
        std::cin >> waktu;
        std::cout << "Biaya perjalanan baru (Rp): ";
        std::cin >> biaya;
        
        graphManager.updateRoute(asal, tujuan, waktu, biaya);
    }
    
    void handleAutoGenerateRoutes() {
        if (graphManager.getLocationCount() < 2) {
            std::cout << "❌ Minimal diperlukan 2 lokasi untuk auto-generate rute." << std::endl;
            return;
        }
        
        std::cout << "\nPilih mode transportasi:" << std::endl;
        std::cout << "[1] 🏎️  Mode Cepat (waktu = jarak x 2, biaya = jarak x 5000)" << std::endl;
        std::cout << "[2] 🚗 Mode Normal (waktu = jarak x 3, biaya = jarak x 7500)" << std::endl;
        std::cout << "[3] 🚌 Mode Ekonomis (waktu = jarak x 5, biaya = jarak x 3000)" << std::endl;
        std::cout << "Pilihan mode (1-3): ";
        
        int modeChoice;
        std::cin >> modeChoice;
        
        AutoRouteGenerator::TransportMode mode;
        switch(modeChoice) {
            case 1: mode = AutoRouteGenerator::TransportMode::FAST; break;
            case 2: mode = AutoRouteGenerator::TransportMode::NORMAL; break;
            case 3: mode = AutoRouteGenerator::TransportMode::ECONOMY; break;
            default: 
                std::cout << "❌ Pilihan tidak valid, menggunakan mode Normal" << std::endl;
                mode = AutoRouteGenerator::TransportMode::NORMAL;
                break;
        }
        
        std::cout << "\n🔧 Pilih metode koneksi:" << std::endl;
        std::cout << "[1] 🌐 Koneksi berdasarkan jarak terdekat (max 3 koneksi per kota)" << std::endl;
        std::cout << "[2] 📏 Koneksi kota dalam radius tertentu" << std::endl;
        std::cout << "Pilihan metode (1-2): ";
        
        int connectionChoice;
        std::cin >> connectionChoice;
        
        if (connectionChoice == 1) {
            autoGenerator.generateRoutes(mode, AutoRouteGenerator::ConnectionMethod::NEAREST_NEIGHBORS);
        } else if (connectionChoice == 2) {
            std::cout << "Masukkan radius maksimal koneksi (km): ";
            double radius;
            std::cin >> radius;
            autoGenerator.generateRoutes(mode, AutoRouteGenerator::ConnectionMethod::RADIUS_BASED, radius);
        } else {
            std::cout << "❌ Pilihan tidak valid." << std::endl;
        }
    }
    
    void handleDisplayGraph() {
        graphManager.displayGraph();
    }
    
    void handleFindBestRoute() {
        if (!hasRoutesAvailable()) return;
        
        graphManager.listLocations();
        
        std::vector<std::string> locationNames = getLocationNames();
        
        std::cout << "\n🎯 PENCARIAN RUTE TERBAIK (SINGLE MODE)" << std::endl;
        
        std::string asal = selectLocation("🚀 Pilih lokasi awal:", locationNames);
        if (asal.empty()) return;
        
        std::string tujuan = selectLocation("🏁 Pilih lokasi tujuan:", locationNames, asal);
        if (tujuan.empty()) return;
        
        std::cout << "\n✅ Rute terpilih: " << asal << " ➜ " << tujuan << std::endl;
        
        std::cout << "\n⚙️  Pilih mode prioritas:" << std::endl;
        std::cout << "[1] 📏 Jarak terpendek" << std::endl;
        std::cout << "[2] ⏱️  Waktu tercepat" << std::endl;
        std::cout << "[3] 💰 Biaya termurah" << std::endl;
        std::cout << "Pilihan mode (1-3): ";
        
        int modeChoice;
        std::cin >> modeChoice;
        
        std::string mode, modeDesc;
        switch(modeChoice) {
            case 1: mode = "jarak"; modeDesc = "jarak terpendek"; break;
            case 2: mode = "waktu"; modeDesc = "waktu tercepat"; break;
            case 3: mode = "biaya"; modeDesc = "biaya termurah"; break;
            default: 
                std::cout << "❌ Pilihan tidak valid." << std::endl;
                return;
        }
        
        std::cout << "\n🔍 Mencari rute dengan prioritas " << modeDesc << "..." << std::endl;
        
        PathResult result = dijkstraAlgorithm.findShortestPath(asal, tujuan, mode);
        routeSimulator.simulateJourney(result, mode);
    }
    
    void handleFindRouteWithPreference() {
        if (!hasRoutesAvailable()) return;
        
        std::cout << "\n🎯 PENCARIAN RUTE DENGAN PREFERENSI MULTI-KRITERIA" << std::endl;
        std::cout << "Sistem akan menggunakan skor gabungan dari waktu, biaya, dan jarak." << std::endl;
        
        preferenceManager.inputPrioritas();
        
        graphManager.listLocations();
        
        std::vector<std::string> locationNames = getLocationNames();
        
        std::string asal = selectLocation("🚀 Pilih lokasi awal:", locationNames);
        if (asal.empty()) return;
        
        std::string tujuan = selectLocation("🏁 Pilih lokasi tujuan:", locationNames, asal);
        if (tujuan.empty()) return;
        
        std::cout << "\n✅ Rute terpilih: " << asal << " ➜ " << tujuan << std::endl;
        std::cout << "\n🔍 Mencari rute optimal berdasarkan preferensi..." << std::endl;
        
        PathResult result = dijkstraAlgorithm.findBestRouteWithPreference(asal, tujuan, preferenceManager);
        routeSimulator.simulateJourneyWithPreference(result, preferenceManager);
    }
    
    void handleTextVisualization() {
        visualizationManager.displayTextVisualization();
    }
    
    void handleGraphicalVisualization() {
        visualizationManager.runGraphicalVisualization();
    }
    
    bool hasLocations() {
        if (graphManager.getLocationCount() == 0) {
            std::cout << "❌ Tidak ada lokasi yang tersedia. Load lokasi dari file terlebih dahulu." << std::endl;
            return false;
        }
        return true;
    }
    
    bool hasRoutesAvailable() {
        if (!hasLocations()) return false;
        
        for (const auto& loc : graphManager.getLocations()) {
            if (!graphManager.getRoutesFrom(loc.first).empty()) {
                return true;
            }
        }
        
        std::cout << "❌ Tidak ada rute yang tersedia. Sambungkan lokasi terlebih dahulu." << std::endl;
        return false;
    }
    
    std::vector<std::string> getLocationNames() {
        std::vector<std::string> locationNames;
        for (const auto& loc : graphManager.getLocations()) {
            locationNames.push_back(loc.first);
        }
        return locationNames;
    }
    
    std::string selectLocation(const std::string& prompt, const std::vector<std::string>& locationNames, 
                              const std::string& excludeLocation = "") {
        std::cout << "\n" << prompt << std::endl;
        
        int displayIndex = 1;
        for (size_t i = 0; i < locationNames.size(); ++i) {
            if (locationNames[i] != excludeLocation) {
                std::cout << "[" << displayIndex++ << "] " << locationNames[i] << std::endl;
            }
        }
        
        int maxChoice = excludeLocation.empty() ? locationNames.size() : locationNames.size() - 1;
        std::cout << "Pilihan (1-" << maxChoice << "): ";
        
        int choice;
        std::cin >> choice;
        
        if (choice < 1 || choice > maxChoice) {
            std::cout << "❌ Pilihan tidak valid." << std::endl;
            return "";
        }
        
        int actualIndex = 0;
        int currentDisplay = 1;
        for (size_t i = 0; i < locationNames.size(); ++i) {
            if (locationNames[i] != excludeLocation) {
                if (currentDisplay == choice) {
                    actualIndex = i;
                    break;
                }
                currentDisplay++;
            }
        }
        
        return locationNames[actualIndex];
    }
};

int main() {
    TransportationSystem system;
    system.run();
    return 0;
}
