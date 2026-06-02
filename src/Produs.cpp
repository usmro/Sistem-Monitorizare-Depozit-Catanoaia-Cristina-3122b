#include "Produs.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>

Produs::Produs(int id, const std::string& nume, int cantitate, double pret,
               int pragAlerta, const std::string& locatie)
    : Entitate(id), nume(nume), cantitate(cantitate),
      pret(pret), pragAlerta(pragAlerta), locatie(locatie) {}

Produs& Produs::operator+=(int qty) {
    if (qty < 0) throw DepozitException("Cantitate negativa la adaugare");
    cantitate += qty;
    return *this;
}

Produs& Produs::operator-=(int qty) {
    if (qty < 0) throw DepozitException("Cantitate negativa la scadere");
    if (qty > cantitate) throw CantitateInsuficientaException(id, qty, cantitate);
    cantitate -= qty;
    return *this;
}

void Produs::afisareCampuriComune() const {
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(12) << getTip()
              << std::setw(24) << nume
              << std::setw(10) << cantitate
              << std::setw(10) << std::fixed << std::setprecision(2) << pret
              << std::setw(8)  << pragAlerta
              << std::setw(12) << locatie;
}

void Produs::afisare() const {
    afisareCampuriComune();
    if (subPrag()) std::cout << " [!PRAG]";
    std::cout << "\n";
}

std::string Produs::toCSV() const {
    return getTip() + "," +
           std::to_string(id) + "," +
           nume + "," +
           std::to_string(cantitate) + "," +
           std::to_string(pret) + "," +
           std::to_string(pragAlerta) + "," +
           locatie;
}
