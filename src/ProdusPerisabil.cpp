#include "ProdusPerisabil.h"
#include <iostream>
#include <iomanip>

ProdusPerisabil::ProdusPerisabil(int id, const std::string& nume, int cantitate,
                                  double pret, int pragAlerta, const std::string& locatie,
                                  const std::string& dataExpirare, double temp)
    : Produs(id, nume, cantitate, pret, pragAlerta, locatie),
      dataExpirare(dataExpirare), temperaturaStorare(temp) {}

void ProdusPerisabil::afisare() const {
    afisareCampuriComune();
    std::cout << std::left
              << " exp:" << std::setw(12) << dataExpirare
              << " temp:" << std::fixed << std::setprecision(1) << temperaturaStorare << "C";
    if (subPrag()) std::cout << " [!PRAG]";
    std::cout << "\n";
}

std::string ProdusPerisabil::toCSV() const {
    return Produs::toCSV() + "," + dataExpirare + "," + std::to_string(temperaturaStorare);
}
