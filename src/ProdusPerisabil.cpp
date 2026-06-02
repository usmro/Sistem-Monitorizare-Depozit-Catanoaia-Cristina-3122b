#include "ProdusPerisabil.h"
#include <iostream>
#include <iomanip>
#include <ctime>

std::string ProdusPerisabil::dataAziStr() {
    time_t now = time(nullptr);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return std::string(buf);
}

std::string ProdusPerisabil::dataViitoareStr(int n) {
    time_t viitor = time(nullptr) + (time_t)n * 86400;
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&viitor));
    return std::string(buf);
}

ProdusPerisabil::ProdusPerisabil(int id, const std::string& nume, int cantitate,
                                  double pret, int pragAlerta, const std::string& locatie,
                                  const std::string& dataExpirare, double temp)
    : Produs(id, nume, cantitate, pret, pragAlerta, locatie),
      dataExpirare(dataExpirare), temperaturaStorare(temp) {}

bool ProdusPerisabil::esteExpirat() const {
    return dataExpirare < dataAziStr();
}

bool ProdusPerisabil::expiraInNZile(int n) const {
    std::string azi    = dataAziStr();
    std::string viitor = dataViitoareStr(n);
    // nu e deja expirat, dar expira pana la data viitoare
    return dataExpirare >= azi && dataExpirare <= viitor;
}

void ProdusPerisabil::afisare() const {
    afisareCampuriComune();
    std::cout << " exp:" << std::setw(12) << dataExpirare
              << " " << std::fixed << std::setprecision(1) << temperaturaStorare << "C";
    if (esteExpirat())      std::cout << " [EXPIRAT]";
    else if (subPrag())     std::cout << " [!PRAG]";
    std::cout << "\n";
}

std::string ProdusPerisabil::toCSV() const {
    return Produs::toCSV() + "," + dataExpirare + "," + std::to_string(temperaturaStorare);
}
