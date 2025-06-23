#ifndef PREFERENCE_MANAGER_H
#define PREFERENCE_MANAGER_H

#include <iostream>
#include <string>
#include <map>

class PreferenceManager {
private:
    int waktuPriority;
    int biayaPriority;
    int jarakPriority;
    
    double weightWaktu;
    double weightBiaya;
    double weightJarak;
    
    void calculateWeights() {
        int max_score = 3;
        int w1 = max_score - waktuPriority + 1;
        int w2 = max_score - biayaPriority + 1;
        int w3 = max_score - jarakPriority + 1;
        int total = w1 + w2 + w3;

        weightWaktu = static_cast<double>(w1) / total;
        weightBiaya = static_cast<double>(w2) / total;
        weightJarak = static_cast<double>(w3) / total;
    }

public:
    PreferenceManager() : waktuPriority(1), biayaPriority(2), jarakPriority(3) {
        calculateWeights();
    }
    
    virtual ~PreferenceManager() = default;
    
    void inputPrioritas() {
        std::cout << "Masukkan prioritas (1 = paling penting, 3 = paling tidak penting)\n";
        std::cout << "Prioritas Waktu : ";
        std::cin >> waktuPriority;
        std::cout << "Prioritas Biaya : ";
        std::cin >> biayaPriority;
        std::cout << "Prioritas Jarak : ";
        std::cin >> jarakPriority;
        std::cin.ignore();
        
        calculateWeights();
    }
    
    void setPriorities(int waktu, int biaya, int jarak) {
        waktuPriority = waktu;
        biayaPriority = biaya;
        jarakPriority = jarak;
        calculateWeights();
    }
    
    void tampilkan() const {
        std::cout << "Bobot berdasarkan preferensi:\n";
        std::cout << "- Waktu : " << weightWaktu << "\n";
        std::cout << "- Biaya : " << weightBiaya << "\n";
        std::cout << "- Jarak : " << weightJarak << "\n";
    }
    
    double getWeightWaktu() const { return weightWaktu; }
    double getWeightBiaya() const { return weightBiaya; }
    double getWeightJarak() const { return weightJarak; }
    
    double calculateScore(double waktu, double biaya, double jarak) const {
        return weightWaktu * waktu + weightBiaya * biaya + weightJarak * jarak;
    }
};

#endif
