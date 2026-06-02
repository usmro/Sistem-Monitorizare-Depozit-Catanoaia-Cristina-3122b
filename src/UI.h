#pragma once
#include "Depozit.h"
#include "StaffDepozit.h"
#include "Furnizor.h"
#include <string>

class UI {
private:
    Depozit& depozit;

    void clearScreen() const;
    void pauza() const;
    void linie(int w = 60) const;
    void titlu(const std::string& text) const;

    void dashboard() const;
    void menuProduse();
    void menuIntrareMarfa();
    void menuIesireMarfa();
    void menuRapoarte() const;
    void menuExport() const;
    void menuSoferiVehicule();
    void menuFurnizori();

    Produs*       introduceProdus();
    Sofer*        introduceSofer();
    Vehicul*      introduceVehicul();
    StaffDepozit* introduceStaff();
    Furnizor*     introduceFurnizor();
    int alegereSofer()  const;
    int alegeVehicul()  const;
    int alegereProdus() const;

public:
    explicit UI(Depozit& d) : depozit(d) {}
    void run();
};
