#include "ProdusISBN.h"
#include <iostream>
#include <iomanip>

ProdusISBN::ProdusISBN(int id, const std::string& nume, int cantitate, double pret,
                        int pragAlerta, const std::string& locatie,
                        const std::string& isbn, const std::string& autor,
                        const std::string& editura, int anPublicare)
    : Produs(id, nume, cantitate, pret, pragAlerta, locatie),
      isbn(isbn), autor(autor), editura(editura), anPublicare(anPublicare) {}

void ProdusISBN::afisare() const {
    afisareCampuriComune();
    std::cout << std::left
              << " isbn:" << std::setw(15) << isbn
              << " " << autor << " (" << anPublicare << ")";
    if (subPrag()) std::cout << " [!PRAG]";
    std::cout << "\n";
}

std::string ProdusISBN::toCSV() const {
    return Produs::toCSV() + "," + isbn + "," + autor + "," +
           editura + "," + std::to_string(anPublicare);
}
