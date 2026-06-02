#include "Sofer.h"
#include <iostream>
#include <iomanip>

Sofer::Sofer(int id, const std::string& nume, const std::string& nrPermis,
             const std::string& categorie)
    : Persoana(id, nume), nrPermis(nrPermis), categorie(categorie) {}

void Sofer::afisare() const {
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(25) << nume
              << std::setw(15) << nrPermis
              << std::setw(6)  << categorie << "\n";
}

std::string Sofer::toCSV() const {
    return std::to_string(id) + "," + nume + "," + nrPermis + "," + categorie;
}
