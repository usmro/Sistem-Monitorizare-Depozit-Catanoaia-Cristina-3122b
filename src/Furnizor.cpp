#include "Furnizor.h"
#include <iostream>
#include <iomanip>

Furnizor::Furnizor(int id, const std::string& nume,
                   const std::string& contact, const std::string& adresa)
    : Entitate(id), nume(nume), contact(contact), adresa(adresa) {}

void Furnizor::afisare() const {
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(25) << nume
              << std::setw(20) << contact
              << std::setw(30) << adresa
              << " (" << produseIds.size() << " produse)\n";
}

std::string Furnizor::toCSV() const {
    std::string produse;
    for (int i = 0; i < (int)produseIds.size(); i++) {
        if (i > 0) produse += "|";
        produse += std::to_string(produseIds[i]);
    }
    return std::to_string(id) + "," + nume + "," + contact + "," + adresa + "," + produse;
}
