#include "src/Depozit.h"
#include "src/UI.h"
#include "src/FileManager.h"
#include "src/ProdusPerisabil.h"
#include "src/ProdusElectronic.h"
#include "src/ProdusISBN.h"
#include <iostream>

int main() {
    Depozit depozit;

    // Incarcare date salvate anterior
    FileManager::incarcaStoc(depozit);
    FileManager::incarcaSoferiVehicule(depozit);

    // Date demo daca depozitul e gol (primul start)
    if (depozit.getTotalProduse() == 0) {
        depozit.adaugaProdus(new ProdusPerisabil(1, "Lapte UHT 1L",   200, 8.50,  30, "A-01-01", "2026-09-01", 4.0));
        depozit.adaugaProdus(new ProdusPerisabil(2, "Iaurt 400g",      80, 5.20,  20, "A-01-02", "2026-07-15", 4.0));
        depozit.adaugaProdus(new Produs         (3, "Orez 1kg",       150, 12.00, 25, "B-02-01"));
        depozit.adaugaProdus(new Produs         (4, "Zahar 1kg",       90, 10.50, 15, "B-02-02"));
        depozit.adaugaProdus(new ProdusElectronic(5,"Laptop Dell 15", 10, 3299.99, 2, "C-01-01", "SN-DELL-001", 24));
        depozit.adaugaProdus(new ProdusElectronic(6,"Mouse Logitech",  25, 149.99, 5, "C-01-02", "SN-LOG-055",  12));
        depozit.adaugaProdus(new ProdusISBN      (7,"C++ Primer",      12,  89.99, 3, "D-01-01",
                                                   "9780321714114", "Lippman", "Addison-Wesley", 2012));

        depozit.adaugaSofer(new Sofer  (1, "Ion Popescu",   "B-123456", "C"));
        depozit.adaugaSofer(new Sofer  (2, "Maria Ionescu", "B-654321", "B"));
        depozit.adaugaVehicul(new Vehicul(1, "B-01-ABC", 5000, "camion"));
        depozit.adaugaVehicul(new Vehicul(2, "B-02-XYZ", 1200, "utilitara"));

        FileManager::salveazaStoc(depozit);
        FileManager::salveazaSoferiVehicule(depozit);
    }

    UI ui(depozit);
    ui.run();

    return 0;
}
