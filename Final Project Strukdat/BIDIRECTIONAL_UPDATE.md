# Update Graf Bidirectional - Final Project Strukdat

## Masalah yang Diperbaiki
Sebelumnya, graf yang dibuat hanya satu arah (directional). Ketika ada rute dari Jakarta ke Surabaya, tidak otomatis ada rute balik dari Surabaya ke Jakarta. Ini menyebabkan algoritma Dijkstra tidak dapat menemukan jalur dalam beberapa kasus.

## Perubahan yang Dibuat

### 1. GraphManager.h
- **Ditambah method `addBidirectionalRoute()`**: Method baru untuk membuat rute dua arah sekaligus
- Ketika dipanggil, method ini akan membuat rute dari A ke B dan juga dari B ke A dengan parameter yang sama

### 2. AutoRouteGenerator.h  
- **Modifikasi `generateNearestNeighborRoutes()`**: Setiap rute yang dibuat otomatis juga membuat rute balik
- **Modifikasi `generateRadiusBasedRoutes()`**: Sama seperti di atas, setiap rute dibuat bidirectional
- **Ditambah method `ensureBidirectionalGraph()`**: Method untuk mengubah graf existing menjadi bidirectional
- **Fix structured bindings**: Mengganti sintaks C++17 dengan yang kompatibel untuk C++11/14

### 3. main.cpp
- **Update menu interface**: Menambah nomor menu baru untuk "Jadikan Graf Bidirectional"
- **Modifikasi `handleAddRoute()`**: Menambah opsi untuk memilih apakah rute yang ditambah manual harus bidirectional
- **Ditambah `handleMakeBidirectional()`**: Handler untuk mengubah graf existing menjadi bidirectional

## Cara Menggunakan

### Untuk Rute Baru (Manual)
1. Pilih menu "4. Tambah Rute Baru"
2. Pilih lokasi asal dan tujuan
3. Masukkan waktu dan biaya
4. Ketika ditanya "Buat rute dua arah? (y/n)", jawab 'y' untuk bidirectional

### Untuk Auto-Generate Rute
- Semua rute yang dibuat melalui "8. Auto-Generate Rute" otomatis bidirectional

### Untuk Graf yang Sudah Ada
1. Pilih menu "9. Jadikan Graf Bidirectional"
2. Konfirmasi dengan 'y'
3. Sistem akan menambahkan rute balik untuk semua rute yang belum memiliki rute balik

## Testing
Sekarang Anda dapat:
1. Load lokasi dari CSV
2. Auto-generate rute atau jadikan graf bidirectional
3. Coba pencarian rute dari Surabaya ke Jakarta (seharusnya berhasil)
4. Coba pencarian rute dari Jakarta ke Surabaya (seharusnya tetap berhasil)

## Technical Details
- Graf tetap menggunakan adjacency list representation
- Setiap edge disimpan dua kali (A→B dan B→A) sebagai entri terpisah
- Cost dan time untuk rute balik sama dengan rute maju
- Jarak dihitung otomatis berdasarkan koordinat lokasi
