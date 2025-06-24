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

using namespace std;

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
        cout << "🚀 Selamat datang di Sistem Rute Transportasi!" << endl;

        int pilihan;
        do {
            showMenu();
            cin >> pilihan;
            
            switch(pilihan) {
                case 1: handleLoadLocations(); break;
                case 2: handleListLocations(); break;
                case 3: handleAddLocation(); break;
                case 4: handleAddRoute(); break;
                case 5: handleRemoveLocation(); break;
                case 6: handleRemoveRoute(); break;
                case 7: handleUpdateRoute(); break;                
                case 8: handleAutoGenerateRoutes(); break;
                case 9: handleMakeBidirectional(); break;
                case 10: handleDisplayGraph(); break;
                case 11: handleFindBestRoute(); break;
                case 12: handleFindRouteWithPreference(); break;
                case 13: handleTextVisualization(); break;
                case 14: handleGraphicalVisualization(); break;
                case 0: cout << "👋 Terima kasih telah menggunakan sistem ini!" << endl; break;
                default: cout << "❌ Pilihan tidak valid." << endl; break;
            }
            
            if (pilihan != 0) {
                cout << "\nTekan Enter untuk melanjutkan...";
                cin.ignore();
                cin.get();
            }
            
        } while (pilihan != 0);
    }

private:
    void showMenu() {
        cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗" << endl;
        cout << "║                        SISTEM RUTE TRANSPORTASI                             ║" << endl;
        cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << endl;
        cout << "=== MANAJEMEN DATA ===" << endl;
        cout << "1.  📂 Load Lokasi dari File CSV" << endl;
        cout << "2.  🏙️  Tampilkan Daftar Lokasi" << endl;
        cout << "3.  ➕ Tambah Lokasi Baru" << endl;
        cout << "4.  🛤️  Tambah Rute Baru" << endl;
        cout << "5.  🗑️  Hapus Lokasi" << endl;
        cout << "6.  🗑️  Hapus Rute" << endl;
        cout << "7.  ✏️  Update Rute" << endl;
        cout << "8.  🤖 Auto-Generate Rute" << endl;
        cout << "9.  🔄 Jadikan Graf Bidirectional" << endl;        cout << "\n=== PENCARIAN & SIMULASI ===" << endl;
        cout << "10. 🗺️  Tampilkan Graf Lengkap" << endl;
        cout << "11. 🚀 Cari Rute Terbaik (Single Mode)" << endl;
        cout << "12. 🎯 Cari Rute dengan Preferensi Multi-kriteria" << endl;
        cout << "\n=== VISUALISASI ===" << endl;
        cout << "13. 📊 Visualisasi Teks (Graf & Tree)" << endl;
        cout << "14. 🎨 Visualisasi Grafis (SFML)" << endl;
        cout << "\n0.  ❌ Keluar" << endl;
        cout << "\nPilih menu: ";
        cout.flush();
    }
    
    void handleLoadLocations() {
        string filename;
        cout << "📂 Masukkan nama file CSV (contoh: locations.csv): ";
        cin.ignore();
        getline(cin, filename);
        graphManager.loadLocationsFromCSV(filename);
    }
    
    void handleListLocations() {
        graphManager.listLocations();
    }
    
    void handleAddLocation() {
        cout << "\n➕ TAMBAH LOKASI BARU" << endl;
        
        string namaLokasi;
        double x, y;
        
        cout << "Nama lokasi: ";
        cin.ignore();
        getline(cin, namaLokasi);
        
        cout << "Koordinat X: ";
        cin >> x;
        cout << "Koordinat Y: ";
        cin >> y;
        
        if (graphManager.addLocation(namaLokasi, x, y)) {
            ofstream file("locations.csv", ios::app);
            if (file.is_open()) {
                file << namaLokasi << "," << x << "," << y << endl;
                file.close();
                cout << "✅ Lokasi berhasil disimpan ke file locations.csv" << endl;
            } else {
                cout << "⚠️  Lokasi ditambahkan ke sistem tapi gagal menyimpan ke file CSV" << endl;
            }
        }
    }
      void handleAddRoute() {
        if (!hasLocations()) return;
        
        graphManager.listLocations();
        
        vector<string> locationNames = getLocationNames();
        
        cout << "\n🔗 TAMBAH RUTE BARU" << endl;
        
        string asal = selectLocation("📍 Pilih lokasi asal:", locationNames);
        if (asal.empty()) return;
        
        string tujuan = selectLocation("🎯 Pilih lokasi tujuan:", locationNames, asal);
        if (tujuan.empty()) return;
        
        cout << "\n✅ Terpilih: " << asal << " ➜ " << tujuan << endl;
        
        double waktu, biaya;
        cout << "Waktu tempuh (menit): ";
        cin >> waktu;
        cout << "Biaya perjalanan (Rp): ";
        cin >> biaya;
        
        char bidirectional;
        cout << "Buat rute dua arah? (y/n): ";
        cin >> bidirectional;
        
        if (bidirectional == 'y' || bidirectional == 'Y') {
            graphManager.addBidirectionalRoute(asal, tujuan, waktu, biaya);
        } else {
            graphManager.addRoute(asal, tujuan, waktu, biaya);
        }
    }
    
    void handleRemoveLocation() {
        if (!hasLocations()) return;
        
        graphManager.listLocations();
        
        vector<string> locationNames = getLocationNames();
        string lokasi = selectLocation("🗑️  Pilih lokasi yang akan dihapus:", locationNames);
        
        if (!lokasi.empty()) {
            graphManager.removeLocation(lokasi);
        }
    }
    
    void handleRemoveRoute() {
        if (!hasLocations()) return;
        
        graphManager.listLocations();
        
        vector<string> locationNames = getLocationNames();
        
        string asal = selectLocation("📍 Pilih lokasi asal rute yang akan dihapus:", locationNames);
        if (asal.empty()) return;
        
        string tujuan = selectLocation("🎯 Pilih lokasi tujuan rute yang akan dihapus:", locationNames, asal);
        if (tujuan.empty()) return;
        
        graphManager.removeRoute(asal, tujuan);
    }
    
    void handleUpdateRoute() {
        if (!hasLocations()) return;
        
        graphManager.listLocations();
        
        vector<string> locationNames = getLocationNames();
        
        string asal = selectLocation("📍 Pilih lokasi asal rute yang akan diupdate:", locationNames);
        if (asal.empty()) return;
        
        string tujuan = selectLocation("🎯 Pilih lokasi tujuan rute yang akan diupdate:", locationNames, asal);
        if (tujuan.empty()) return;
        
        double waktu, biaya;
        cout << "Waktu tempuh baru (menit): ";
        cin >> waktu;
        cout << "Biaya perjalanan baru (Rp): ";
        cin >> biaya;
        
        graphManager.updateRoute(asal, tujuan, waktu, biaya);
    }
    
    void handleAutoGenerateRoutes() {
        if (graphManager.getLocationCount() < 2) {
            cout << "❌ Minimal diperlukan 2 lokasi untuk auto-generate rute." << endl;
            return;
        }
        
        cout << "\nPilih mode transportasi:" << endl;
        cout << "[1] 🏎️  Mode Cepat (waktu = jarak x 2, biaya = jarak x 5000)" << endl;
        cout << "[2] 🚗 Mode Normal (waktu = jarak x 3, biaya = jarak x 7500)" << endl;
        cout << "[3] 🚌 Mode Ekonomis (waktu = jarak x 5, biaya = jarak x 3000)" << endl;
        cout << "Pilihan mode (1-3): ";
        
        int modeChoice;
        cin >> modeChoice;
        
        AutoRouteGenerator::TransportMode mode;
        switch(modeChoice) {
            case 1: mode = AutoRouteGenerator::TransportMode::FAST; break;
            case 2: mode = AutoRouteGenerator::TransportMode::NORMAL; break;
            case 3: mode = AutoRouteGenerator::TransportMode::ECONOMY; break;
            default: 
                cout << "❌ Pilihan tidak valid, menggunakan mode Normal" << endl;
                mode = AutoRouteGenerator::TransportMode::NORMAL;
                break;
        }
        
        cout << "\n🔧 Pilih metode koneksi:" << endl;
        cout << "[1] 🌐 Koneksi berdasarkan jarak terdekat (max 3 koneksi per kota)" << endl;
        cout << "[2] 📏 Koneksi kota dalam radius tertentu" << endl;
        cout << "Pilihan metode (1-2): ";
        
        int connectionChoice;
        cin >> connectionChoice;
        
        if (connectionChoice == 1) {
            autoGenerator.generateRoutes(mode, AutoRouteGenerator::ConnectionMethod::NEAREST_NEIGHBORS);
        } else if (connectionChoice == 2) {
            cout << "Masukkan radius maksimal koneksi (km): ";
            double radius;
            cin >> radius;
            autoGenerator.generateRoutes(mode, AutoRouteGenerator::ConnectionMethod::RADIUS_BASED, radius);
        } else {
            cout << "❌ Pilihan tidak valid." << endl;
        }
    }
    
    void handleDisplayGraph() {
        graphManager.displayGraph();
    }
    
    void handleFindBestRoute() {
        if (!hasRoutesAvailable()) return;
        
        graphManager.listLocations();
        
        vector<string> locationNames = getLocationNames();
        
        cout << "\n🎯 PENCARIAN RUTE TERBAIK (SINGLE MODE)" << endl;
        
        string asal = selectLocation("🚀 Pilih lokasi awal:", locationNames);
        if (asal.empty()) return;
        
        string tujuan = selectLocation("🏁 Pilih lokasi tujuan:", locationNames, asal);
        if (tujuan.empty()) return;
        
        cout << "\n✅ Rute terpilih: " << asal << " ➜ " << tujuan << endl;
        
        cout << "\n⚙️  Pilih mode prioritas:" << endl;
        cout << "[1] 📏 Jarak terpendek" << endl;
        cout << "[2] ⏱️  Waktu tercepat" << endl;
        cout << "[3] 💰 Biaya termurah" << endl;
        cout << "Pilihan mode (1-3): ";
        
        int modeChoice;
        cin >> modeChoice;
        
        string mode, modeDesc;
        switch(modeChoice) {
            case 1: mode = "jarak"; modeDesc = "jarak terpendek"; break;
            case 2: mode = "waktu"; modeDesc = "waktu tercepat"; break;
            case 3: mode = "biaya"; modeDesc = "biaya termurah"; break;
            default: 
                cout << "❌ Pilihan tidak valid." << endl;
                return;
        }
        
        cout << "\n🔍 Mencari rute dengan prioritas " << modeDesc << "..." << endl;
        
        PathResult result = dijkstraAlgorithm.findShortestPath(asal, tujuan, mode);
        routeSimulator.simulateJourney(result, mode);
    }
    
    void handleFindRouteWithPreference() {
        if (!hasRoutesAvailable()) return;
        
        cout << "\n🎯 PENCARIAN RUTE DENGAN PREFERENSI MULTI-KRITERIA" << endl;
        cout << "Sistem akan menggunakan skor gabungan dari waktu, biaya, dan jarak." << endl;
        
        preferenceManager.inputPrioritas();
        
        graphManager.listLocations();
        
        vector<string> locationNames = getLocationNames();
        
        string asal = selectLocation("🚀 Pilih lokasi awal:", locationNames);
        if (asal.empty()) return;
        
        string tujuan = selectLocation("🏁 Pilih lokasi tujuan:", locationNames, asal);
        if (tujuan.empty()) return;
        
        cout << "\n✅ Rute terpilih: " << asal << " ➜ " << tujuan << endl;
        cout << "\n🔍 Mencari rute optimal berdasarkan preferensi..." << endl;
        
        PathResult result = dijkstraAlgorithm.findBestRouteWithPreference(asal, tujuan, preferenceManager);
        routeSimulator.simulateJourneyWithPreference(result, preferenceManager);
    }
    
    void handleTextVisualization() {
        visualizationManager.displayTextVisualization();
    }
    
    void handleGraphicalVisualization() {
        visualizationManager.runGraphicalVisualization();
    }
    
    void handleMakeBidirectional() {
        if (!hasLocations()) return;
        
        cout << "\n🔄 JADIKAN GRAF BIDIRECTIONAL" << endl;
        cout << "Ini akan menambahkan rute balik untuk setiap rute yang ada." << endl;
        cout << "Lanjutkan? (y/n): ";
        
        char confirm;
        cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            autoGenerator.ensureBidirectionalGraph();
        } else {
            cout << "❌ Operasi dibatalkan." << endl;
        }
    }
    
    bool hasLocations() {
        if (graphManager.getLocationCount() == 0) {
            cout << "❌ Tidak ada lokasi yang tersedia. Load lokasi dari file terlebih dahulu." << endl;
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
        
        cout << "❌ Tidak ada rute yang tersedia. Sambungkan lokasi terlebih dahulu." << endl;
        return false;
    }
    
    vector<string> getLocationNames() {
        vector<string> locationNames;
        for (const auto& loc : graphManager.getLocations()) {
            locationNames.push_back(loc.first);
        }
        return locationNames;
    }
    
    string selectLocation(const string& prompt, const vector<string>& locationNames, 
                              const string& excludeLocation = "") {
        cout << "\n" << prompt << endl;
        
        int displayIndex = 1;
        for (size_t i = 0; i < locationNames.size(); ++i) {
            if (locationNames[i] != excludeLocation) {
                cout << "[" << displayIndex++ << "] " << locationNames[i] << endl;
            }
        }
        
        int maxChoice = excludeLocation.empty() ? locationNames.size() : locationNames.size() - 1;
        cout << "Pilihan (1-" << maxChoice << "): ";
        
        int choice;
        cin >> choice;
        
        if (choice < 1 || choice > maxChoice) {
            cout << "❌ Pilihan tidak valid." << endl;
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
