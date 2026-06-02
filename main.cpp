#include "src/Depozit.h"
#include "src/UI.h"
#include "src/FileManager.h"
#include "src/ProdusPerisabil.h"
#include "src/ProdusElectronic.h"
#include "src/ProdusISBN.h"

int main() {
    Depozit depozit;

    FileManager::incarcaStoc(depozit);
    FileManager::incarcaSoferiVehicule(depozit);

    if (depozit.getTotalProduse() == 0) {
        // Date demo la primul start
        depozit.adaugaProdus(new ProdusPerisabil(1, "Lapte UHT 1L",    200, 8.50,  30, "A-01-01", "2026-09-01", 4.0));
        depozit.adaugaProdus(new ProdusPerisabil(2, "Iaurt 400g",        80, 5.20,  20, "A-01-02", "2026-07-15", 4.0));
        depozit.adaugaProdus(new ProdusPerisabil(3, "Lapte Proaspat 1L",  8, 7.00,  15, "A-01-03", "2026-06-05", 4.0)); // expira curand
        depozit.adaugaProdus(new Produs         (4, "Orez 1kg",          150, 12.00, 25, "B-02-01"));
        depozit.adaugaProdus(new Produs         (5, "Zahar 1kg",          90, 10.50, 15, "B-02-02"));
        depozit.adaugaProdus(new ProdusElectronic(6,"Laptop Dell 15",    10, 3299.99, 2, "C-01-01", "SN-DELL-001", 24));
        depozit.adaugaProdus(new ProdusElectronic(7,"Mouse Logitech",    25,  149.99, 5, "C-01-02", "SN-LOG-055",  12));
        depozit.adaugaProdus(new ProdusISBN      (8,"C++ Primer",        12,   89.99, 3, "D-01-01",
                                                    "9780321714114", "Lippman", "Addison-Wesley", 2012));

        depozit.adaugaSofer  (new Sofer  (1, "Ion Popescu",    "B-123456", "C"));
        depozit.adaugaSofer  (new Sofer  (2, "Maria Ionescu",  "B-654321", "B"));
        depozit.adaugaVehicul(new Vehicul(1, "B-01-ABC", 5000, "camion"));
        depozit.adaugaVehicul(new Vehicul(2, "B-02-XYZ", 1200, "utilitara"));
        depozit.adaugaStaff  (new StaffDepozit(1, "Admin Principal", "manager",  "admin"));
        depozit.adaugaStaff  (new StaffDepozit(2, "Operator Stoc",   "operator", "op1"));
        depozit.adaugaFurnizor(new Furnizor(1, "Lacto SA",      "0721-111222", "Str. Laptelui 1, Cluj"));
        depozit.adaugaFurnizor(new Furnizor(2, "TechDistrib SRL","0722-333444", "Bd. Industriei 5, Buc."));

        FileManager::salveazaStoc(depozit);
        FileManager::salveazaSoferiVehicule(depozit);
    }

    UI ui(depozit);
    ui.run();

    return 0;
}
