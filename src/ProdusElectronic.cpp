#include "ProdusElectronic.h"
#include <iostream>
#include <iomanip>

ProdusElectronic::ProdusElectronic(int id, const std::string& nume, int cantitate,
                                    double pret, int pragAlerta, const std::string& locatie,
                                    const std::string& serie, int garantieLuni, double voltaj)
    : Produs(id, nume, cantitate, pret, pragAlerta, locatie),
      serie(serie), garantieLuni(garantieLuni), voltaj(voltaj) {}

void ProdusElectronic::afisare() const {
    afisareCampuriComune();
    std::cout << std::left
              << " serie:" << std::setw(12) << serie
              << " gar:" << std::setw(4) << garantieLuni << "luni"
              << " " << std::fixed << std::setprecision(0) << voltaj << "V";
    if (subPrag()) std::cout << " [!PRAG]";
    std::cout << "\n";
}

std::string ProdusElectronic::toCSV() const {
    return Produs::toCSV() + "," + serie + "," +
           std::to_string(garantieLuni) + "," + std::to_string(voltaj);
}
