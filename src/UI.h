#pragma once
#include "Depozit.h"
#include "StaffDepozit.h"
#include "Furnizor.h"
#include "Consola.h"
#include <string>

class UI {
private:
    Depozit& depozit;

    void pauza() const;

    // Afisare tabel colorat
    void afisareProdus(Produs* p) const;
    void afisareTabelProduse(const std::vector<Produs*>& lista) const;
    void afisareTabelToate() const;

    // Sub-meniuri
    void dashboard() const;
    void menuProduse();
    void menuIntrareMarfa();
    void menuIesireMarfa();
    void menuRapoarte() const;
    void menuExport() const;
    void menuSoferiVehicule();
    void menuFurnizori();

    // Helpers introducere date
    Produs*       introduceProdus();
    Sofer*        introduceSofer();
    Vehicul*      introduceVehicul();
    StaffDepozit* introduceStaff();
    Furnizor*     introduceFurnizor();
    int           alegereProdus() const;
    int           alegereSofer()  const;
    int           alegeVehicul()  const;

public:
    explicit UI(Depozit& d) : depozit(d) {}
    void run();
};
