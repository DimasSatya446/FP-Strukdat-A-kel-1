#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <iostream>
#include <string>

struct Preference {
    int waktuPriority;
    int biayaPriority;
    int jarakPriority;

    double weightWaktu;
    double weightBiaya;
    double weightJarak;

    void inputPrioritas() {
        std::cout << "Masukkan prioritas (1 = paling penting, 3 = paling tidak penting)\n";
        std::cout << "Prioritas Waktu : ";
        std::cin >> waktuPriority;
        std::cout << "Prioritas Biaya : ";
        std::cin >> biayaPriority;
        std::cout << "Prioritas Jarak : ";
        std::cin >> jarakPriority;
        std::cin.ignore();

        // konversi ke bobot
        int max_score = 3;
        int w1 = max_score - waktuPriority + 1;
        int w2 = max_score - biayaPriority + 1;
        int w3 = max_score - jarakPriority + 1;
        int total = w1 + w2 + w3;

        weightWaktu = static_cast<double>(w1) / total;
        weightBiaya = static_cast<double>(w2) / total;
        weightJarak = static_cast<double>(w3) / total;
    }

    void tampilkan() const {
        std::cout << "Bobot berdasarkan preferensi:\n";
        std::cout << "- Waktu : " << weightWaktu << "\n";
        std::cout << "- Biaya : " << weightBiaya << "\n";
        std::cout << "- Jarak : " << weightJarak << "\n";
    }
};

#endif
