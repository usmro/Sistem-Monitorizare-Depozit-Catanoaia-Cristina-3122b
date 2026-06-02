#include "Vehicul.h"
#include <iostream>
#include <iomanip>

Vehicul::Vehicul(int id, const std::string& nrInmatriculare, double capacitateKg,
                 const std::string& tip)
    : Entitate(id), nrInmatriculare(nrInmatriculare),
      capacitateKg(capacitateKg), tip(tip) {}

void Vehicul::afisare() const {
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(12) << nrInmatriculare
              << std::setw(14) << std::fixed << std::setprecision(0) << capacitateKg << "kg"
              << std::setw(12) << tip << "\n";
}

std::string Vehicul::toCSV() const {
    return std::to_string(id) + "," + nrInmatriculare + "," +
           std::to_string(capacitateKg) + "," + tip;
}
