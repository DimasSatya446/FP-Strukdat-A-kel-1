# 🚀 Sistem Rute Transportasi Berbasis Graf (OOP Architecture)

[![C++17](https://img.shields.io/badge/C++17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-8CC445?style=for-the-badge&logo=sfml&logoColor=white)](https://www.sfml-dev.org/)
[![OOP](https://img.shields.io/badge/OOP-Architecture-blue?style=for-the-badge)](https://en.wikipedia.org/wiki/Object-oriented_programming)
[![Dijkstra](https://img.shields.io/badge/Algorithm-Dijkstra-red?style=for-the-badge)](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)

> **Aplikasi C++ dengan arsitektur OOP yang menerapkan algoritma Dijkstra untuk pencarian rute transportasi optimal dengan visualisasi interaktif**

## 📋 Daftar Isi

- [🌟 Fitur Utama](#-fitur-utama)
- [🛠️ Teknologi](#️-teknologi)
- [📦 Instalasi](#-instalasi)
- [🚀 Cara Penggunaan](#-cara-penggunaan)
- [📊 Struktur Data](#-struktur-data)
- [🎯 Algoritma](#-algoritma)
- [📸 Screenshot](#-screenshot)
- [🤝 Kontribusi](#-kontribusi)

## 🌟 Fitur Utama

### 🏗️ **Arsitektur OOP Modern**
- **🔄 Inheritance & Polymorphism**: Implementasi konsep OOP lengkap
- **🔒 Encapsulation**: Data dan method terlindungi dengan baik
- **🎯 Abstraction**: Interface yang jelas antar komponen
- **📦 Modular Design**: Setiap class memiliki tanggung jawab spesifik

### 🗺️ **Manajemen Lokasi (CRUD)**
- **📂 Import dari CSV**: Load lokasi massal dari file CSV dengan format `nama,x,y`
- **➕ Create**: Menambahkan lokasi baru dengan koordinat geografis
- **� Read**: Menampilkan semua lokasi dengan koordinat yang tersimpan
- **✏️ Update**: Mengubah data lokasi yang sudah ada
- **🗑️ Delete**: Menghapus lokasi dan semua rute terkait

### 🛤️ **Manajemen Rute (CRUD)**
- **🔗 Create Route**: Menghubungkan dua lokasi dengan input waktu tempuh dan biaya
- **📏 Auto Distance**: Jarak dihitung otomatis berdasarkan koordinat Euclidean
- **📖 Read Routes**: Menampilkan semua rute dengan detail lengkap
- **✏️ Update Route**: Mengubah waktu tempuh dan biaya rute
- **🗑️ Delete Route**: Menghapus rute spesifik
- **🤖 Auto-Generate**: Membuat rute realistis secara otomatis dengan 3 mode:
  - 🏎️ **Mode Cepat**: Waktu = jarak × 2, Biaya = jarak × 5000
  - 🚗 **Mode Normal**: Waktu = jarak × 3, Biaya = jarak × 7500  
  - 🚌 **Mode Ekonomis**: Waktu = jarak × 5, Biaya = jarak × 3000

### 🎯 **Algoritma Pencarian Rute**
- **📏 Single-Mode Optimization**: 
  - Jarak terpendek (Dijkstra dengan bobot jarak)
  - Waktu tercepat (Dijkstra dengan bobot waktu)
  - Biaya termurah (Dijkstra dengan bobot biaya)
- **🎯 Multi-Criteria Optimization**: 
  - Preferensi dinamis berdasarkan prioritas user
  - Skor gabungan: `W1×waktu + W2×biaya + W3×jarak`
  - Bobot dihitung dari ranking prioritas user

### 🎮 **Simulasi Perjalanan**
- **� Detailed Journey**: Langkah-per-langkah perjalanan
- **📊 Summary Statistics**: Total jarak, waktu, biaya
- **� Time Estimation**: Estimasi waktu tempuh dengan kategori
- **� Smart Recommendations**: Saran transportasi berdasarkan jarak

### 🎨 **Visualisasi Graf & Tree**
- **📊 Text Visualization**: 
  - Adjacency Matrix representation
  - Tree structure display
  - ASCII-based graph layout
- **🖼️ Graphical Visualization**: 
  - SFML-based interactive display
  - Path highlighting dengan warna
  - Real-time graph rendering

## 🛠️ Teknologi & Arsitektur

### **Core Technologies**
- **C++17**: Bahasa pemrograman dengan fitur modern
- **STL Containers**: `vector`, `map`, `unordered_map`, `set`, `priority_queue`
- **SFML**: Simple and Fast Multimedia Library untuk visualisasi grafis
- **OOP Principles**: Inheritance, Polymorphism, Encapsulation, Abstraction

### **Struktur Data Lanjutan**
- **🔍 Priority Queue**: Untuk implementasi algoritma Dijkstra
- **🗂️ Unordered Map**: Hash table untuk akses O(1) ke lokasi
- **📊 Adjacency List**: Representasi graf yang efisien
- **🌳 Tree Structure**: Untuk visualisasi hierarki graf

### **Design Patterns**
- **🏗️ Facade Pattern**: TransportationSystem sebagai interface utama
- **� Strategy Pattern**: Berbagai mode pencarian rute
- **🎯 Observer Pattern**: Update visualisasi berdasarkan perubahan data
- **� Composition Pattern**: Agregasi komponen dalam TransportationSystem

### **Algoritma & Kompleksitas**
- **🔍 Dijkstra Algorithm**: O((V + E) log V) untuk shortest path
- **📏 Euclidean Distance**: O(1) untuk kalkulasi jarak
- **🤖 Auto Route Generation**: O(V²) untuk nearest neighbors
- **🎯 Multi-criteria Decision**: Weighted scoring algorithm

## 📦 Instalasi & Kompilasi

### **Prerequisites**
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libsfml-dev

# Fedora/Red Hat
sudo dnf install gcc-c++
sudo dnf install SFML-devel

# Arch Linux
sudo pacman -S gcc
sudo pacman -S sfml
```

### **Compile & Run**

1. **Clone Repository**
```bash
git clone <repository-url>
cd "Final Project Strukdat"
```

2. **Compile Main Program**
```bash
g++ main.cpp -o main_oop -std=c++17
```

3. **Compile Graph Visualizer**
```bash
g++ graph_visualizer.cpp -o graph_visualizer -lsfml-graphics -lsfml-window -lsfml-system
```

4. **Run Application**
```bash
./main_oop
```

### **File Structure (Clean OOP)**
```
Final Project Strukdat/
│
├── main.cpp                   # Entry point dengan TransportationSystem
├── Location.h                 # Location class definition
├── Route.h                   # Route class definition  
├── GraphManager.h           # Graf management dengan CRUD
├── DijkstraAlgorithm.h     # Algoritma pencarian rute
├── PreferenceManager.h     # Multi-criteria preferences
├── RouteSimulator.h       # Simulasi dan output perjalanan
├── AutoRouteGenerator.h   # Auto-generate rute realistis
├── VisualizationManager.h # Text & graphical visualization
├── graph_visualizer.cpp  # SFML visualization engine
├── locations.csv        # Data lokasi (format: nama,x,y)
├── temp_*.csv          # File temporary untuk export
└── README.md          # Dokumentasi lengkap
```

## 🚀 Cara Penggunaan

### **1. Load Data Lokasi**
```
Menu: [1] Load Lokasi dari File CSV
File: locations.csv
Format: nama_kota,koordinat_x,koordinat_y
```

### **2. CRUD Lokasi**
```
Menu: [3] Tambah Lokasi Baru    # Create
Menu: [2] Tampilkan Daftar      # Read
Menu: [5] Hapus Lokasi         # Delete
```

### **3. CRUD Rute**
```
Menu: [4] Tambah Rute Baru     # Create
Menu: [9] Tampilkan Graf       # Read
Menu: [7] Update Rute          # Update
Menu: [6] Hapus Rute          # Delete
```

### **4. Auto-Generate Rute**
```
Menu: [8] Auto-Generate Rute Realistis
- Pilih mode kendaraan (Cepat/Normal/Ekonomis)
- Pilih metode koneksi:
  • 3 kota terdekat per lokasi
  • Kota dalam radius tertentu
```

### **5. Pencarian Rute (Single-Mode)**
```
Menu: [10] Cari Rute Terbaik
- Pilih lokasi awal dan tujuan
- Pilih prioritas optimasi:
  • Jarak terpendek (Dijkstra dengan bobot jarak)
  • Waktu tercepat (Dijkstra dengan bobot waktu)
  • Biaya termurah (Dijkstra dengan bobot biaya)
```

### **6. Pencarian Rute (Multi-Criteria)**
```
Menu: [11] Cari Rute dengan Preferensi Multi-kriteria
- Input prioritas (1=paling penting, 3=paling tidak penting):
  • Prioritas Waktu: [1-3]
  • Prioritas Biaya: [1-3] 
  • Prioritas Jarak: [1-3]
- Sistem menghitung bobot dan skor gabungan
```

### **7. Visualisasi**
```
Menu: [12] Visualisasi Teks (Graf & Tree)
- Adjacency Matrix representation
- Tree structure display

Menu: [13] Visualisasi Grafis (SFML)
- Interactive graph display
- Path highlighting
```

## 📊 Struktur OOP & Class Hierarchy

### **Core Classes**

#### **📍 Location Class**
```cpp
class Location {
private:
    std::string name;     // Nama lokasi
    double x, y;          // Koordinat geografis
public:
    // Encapsulation: getter/setter methods
    // Method: distanceTo() untuk kalkulasi jarak Euclidean
}
```

#### **🛤️ Route Class**
```cpp
class Route {
private:
    std::string source, destination;  // Lokasi asal & tujuan
    double distance, time, cost;      // Bobot graf
public:
    // Method: getWeightByMode() untuk multi-criteria
}
```

#### **🗺️ GraphManager Class**
```cpp
class GraphManager {
private:
    std::unordered_map<std::string, Location> locations;
    std::unordered_map<std::string, std::vector<Route>> adjacencyList;
public:
    // CRUD operations untuk lokasi dan rute
    // CSV import/export functionality
}
```

#### **🔍 DijkstraAlgorithm Class**
```cpp
class DijkstraAlgorithm {
private:
    const GraphManager& graph;  // Composition pattern
public:
    PathResult findShortestPath(...);           // Single-mode
    PathResult findBestRouteWithPreference(...); // Multi-criteria
}
```

#### **🎯 PreferenceManager Class**
```cpp
class PreferenceManager {
private:
    int waktuPriority, biayaPriority, jarakPriority;
    double weightWaktu, weightBiaya, weightJarak;
public:
    double calculateScore(...);  // Multi-criteria scoring
}
```

#### **🎮 RouteSimulator Class**
```cpp
class RouteSimulator {
public:
    void simulateJourney(...);              // Detailed output
    void simulateJourneyWithPreference(...); // Multi-criteria output
}
```

#### **🎨 VisualizationManager Class**
```cpp
class VisualizationManager {
public:
    void displayTextVisualization();    // ASCII matrices & trees
    void runGraphicalVisualization();   // SFML graphics
}
```

#### **🤖 AutoRouteGenerator Class**
```cpp
class AutoRouteGenerator {
public:
    enum class TransportMode { FAST, NORMAL, ECONOMY };
    enum class ConnectionMethod { NEAREST_NEIGHBORS, RADIUS_BASED };
    void generateRoutes(...);
}
```

#### **🚀 TransportationSystem Class (Facade)**
```cpp
class TransportationSystem {
private:
    GraphManager graphManager;
    DijkstraAlgorithm dijkstraAlgorithm;
    RouteSimulator routeSimulator;
    // ... semua komponen sistem
public:
    void run();  // Main interface
}
```

### **Enhanced Menu Interface**
```
╔══════════════════════════════════════════════════════════════════════════════╗
║                        SISTEM RUTE TRANSPORTASI                             ║
╚══════════════════════════════════════════════════════════════════════════════╝
=== MANAJEMEN DATA ===
1.  📂 Load Lokasi dari File CSV
2.  🏙️  Tampilkan Daftar Lokasi
3.  ➕ Tambah Lokasi Baru
4.  🛤️  Tambah Rute Baru
5.  🗑️  Hapus Lokasi
6.  🗑️  Hapus Rute
7.  ✏️  Update Rute
8.  🤖 Auto-Generate Rute

=== PENCARIAN & SIMULASI ===
9.  🗺️  Tampilkan Graf Lengkap
10. 🚀 Cari Rute Terbaik (Single Mode)
11. 🎯 Cari Rute dengan Preferensi Multi-kriteria

=== VISUALISASI ===
12. 📊 Visualisasi Teks (Graf & Tree)
13. 🎨 Visualisasi Grafis (SFML)

0.  ❌ Keluar
```

## 🎯 Implementasi Algoritma & Kriteria Penilaian

### **✅ Representasi Graf**
- **Adjacency List**: `std::unordered_map<std::string, std::vector<Route>>`
- **Weighted Graph**: Setiap edge memiliki bobot jarak, waktu, dan biaya
- **Dynamic Graph**: Support CRUD operations pada vertices dan edges

### **✅ Algoritma Pencarian Rute Terbaik (Dijkstra)**
```cpp
// Single-mode Dijkstra
PathResult findShortestPath(start, end, mode) {
    priority_queue<string> pq;  // Min-heap berdasarkan cost
    map<string, double> cost;   // Distance array
    map<string, string> previous; // Parent tracking
    // Kompleksitas: O((V + E) log V)
}

// Multi-criteria Dijkstra  
PathResult findBestRouteWithPreference(start, end, preference) {
    double skor = W1×waktu + W2×biaya + W3×jarak;
    // Bobot dihitung dari ranking prioritas user
}
```

### **✅ Desain Berorientasi Objek (OOP)**
- **Inheritance**: Base classes dengan virtual destructors
- **Polymorphism**: Berbagai mode pencarian rute
- **Encapsulation**: Private data dengan public interfaces
- **Abstraction**: Clear separation of concerns per class

### **✅ Estimasi Waktu Tempuh & Biaya**
```cpp
// Auto-calculation berdasarkan koordinat
double distance = sqrt((x1-x2)² + (y1-y2)²);
double waktu = distance × timeMultiplier;
double biaya = distance × costMultiplier;
```

### **✅ Preferensi Dinamis Pengguna**
- **Input Ranking**: User memberikan prioritas 1-3 untuk setiap kriteria
- **Weight Calculation**: `W = (max_score - priority + 1) / total`
- **Multi-criteria Score**: Kombinasi linear dari semua kriteria

### **✅ CRUD Lokasi & Rute**
- **Create**: Add location/route dengan validasi duplikasi
- **Read**: Display dengan format yang user-friendly
- **Update**: Modify existing routes dengan konfirmasi
- **Delete**: Remove dengan cleanup dependencies

### **✅ Simulasi Perjalanan (Output Teks)**
```
✅ SIMULASI PERJALANAN DIMULAI!
🗺️  Rute: Jakarta - Bandung - Yogyakarta - Surabaya

📋 DETAIL PERJALANAN LANGKAH DEMI LANGKAH:
🔸 Langkah 1: Jakarta → Bandung
   📏 Jarak: 223.6 km | ⏱️ Waktu: 180 menit | 💰 Biaya: Rp 50000

📊 RINGKASAN TOTAL: 847.2 km, 720 menit, Rp 200000
🕐 Estimasi: 12 jam 0 menit
```

### **✅ Struktur Data Tambahan**
- **Priority Queue**: `std::priority_queue` untuk Dijkstra algorithm
- **Unordered Map**: `std::unordered_map` untuk O(1) location lookup
- **Vector**: `std::vector` untuk adjacency lists
- **Set**: `std::set` untuk visited nodes tracking

### **✅ Visualisasi Teks untuk Graf & Tree**
```
� MATRIKS ADJACENCY:
     Jakarta  Bandung  Surabaya
Jkt     0      223.6      ∞
Bdg   223.6      0      423.1
Sby     ∞      423.1      0

🌳 STRUKTUR TREE GRAF:
├── Jakarta
    ├── Bandung [223.6km, 180min, Rp50000]
    └── Semarang [312.4km, 240min, Rp75000]
├── Bandung
    └── Yogyakarta [198.2km, 150min, Rp45000]
```

## 🤝 Kontribusi

### **Cara Berkontribusi**
1. Fork repository ini
2. Buat branch fitur (`git checkout -b feature/AmazingFeature`)
3. Commit perubahan (`git commit -m 'Add some AmazingFeature'`)
4. Push ke branch (`git push origin feature/AmazingFeature`)
5. Buat Pull Request

### **Development Setup**
```bash
# Clone dan setup development environment
git clone <your-fork>
cd "Final Project Strukdat"

# Compile dalam mode debug
g++ -g -DDEBUG main.cpp -o main_debug

# Run tests
./test_csv
```

## 📄 File Structure (Clean OOP)

```
Final Project Strukdat/
│
├── main.cpp                   # Program utama
├── graph_visualizer.cpp       # Visualisasi SFML
├── locations.csv             # Data lokasi
├── temp_locations.csv        # Export data untuk visualizer
├── temp_routes.csv          # Export rute untuk visualizer
├── temp_path.txt           # Path highlight untuk visualizer
├── dijkstra.h             # Header algoritma Dijkstra
├── graph.h               # Header struktur graf
└── README.md            # Dokumentasi ini
```

## 🏆 Fitur Unggulan & Keunggulan

### **🎯 Compliance dengan Kriteria Penilaian**
- ✅ **Representasi Graf**: Adjacency List dengan weighted edges
- ✅ **Algoritma Dijkstra**: Single-mode & multi-criteria optimization  
- ✅ **OOP Konsisten**: Inheritance, Polymorphism, Encapsulation, Abstraction
- ✅ **Estimasi Waktu & Biaya**: Auto-calculation berdasarkan jarak geografis
- ✅ **Preferensi Dinamis**: Multi-criteria decision dengan user ranking
- ✅ **CRUD Lengkap**: Create, Read, Update, Delete untuk lokasi & rute
- ✅ **Simulasi Perjalanan**: Step-by-step output dengan detail lengkap
- ✅ **Struktur Data Lanjutan**: Priority Queue, Unordered Map, Set
- ✅ **Visualisasi Teks**: Adjacency Matrix & Tree structure display

### **🚀 Advanced Features**
- **Real-time Visualization**: SFML-based graphical display
- **Smart Auto-Generation**: Realistic route creation algorithms
- **Multi-format Export**: CSV files untuk data exchange
- **Interactive Menu**: User-friendly interface dengan emoji icons
- **Error Handling**: Comprehensive validation dan error messages
- **Modular Architecture**: Easy to extend dan maintain
- **Performance Optimized**: Efficient algorithms dengan kompleksitas optimal

### **📈 Technical Excellence**
- **Modern C++17**: Menggunakan fitur-fitur terbaru C++
- **Memory Safe**: RAII principles dengan smart pointers concept
- **Exception Safe**: Proper error handling di seluruh aplikasi
- **Const Correctness**: Immutable references untuk read-only operations
- **STL Compliance**: Menggunakan standard library secara optimal

## 📞 Tim Pengembang

**Final Project Struktur Data - Kelompok 1**
- 🎓 **Program Studi**: Teknik Informatika
- 📚 **Mata Kuliah**: Struktur Data dan Algoritma
- � **Universitas**: [Nama Universitas]
- 📅 **Tahun**: 2025

### **Anggota Tim**
- 👨‍💻 **[Nama 1]** - [NIM] - System Architecture & Algorithm Design
- 👩‍💻 **[Nama 2]** - [NIM] - OOP Implementation & Data Structure
- �‍💻 **[Nama 3]** - [NIM] - Visualization & User Interface
- 👩‍💻 **[Nama 4]** - [NIM] - Testing & Documentation

---

⭐ **Jangan lupa beri star jika project ini membantu!** ⭐

**Built with ❤️ using Modern C++ & OOP Principles**
