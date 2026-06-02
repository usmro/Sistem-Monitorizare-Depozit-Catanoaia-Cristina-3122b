#pragma once
#include "Depozit.h"
#include <string>

class UI {
private:
    Depozit& depozit;

    void clearScreen() const;
    void pauza() const;
    void linie(int latime = 60) const;
    void titlu(const std::string& text) const;

    // Sub-meniuri
    void dashboard() const;
    void menuProduse();
    void menuIntrareMarfa();
    void menuIesireMarfa();
    void menuRapoarte() const;
    void menuExport() const;
    void menuSoferiVehicule();

    // Helpers introducere date
    Produs*  introduceProdus();
    Sofer*   introduceSofer();
    Vehicul* introduceVehicul();
    int      alegereSofer() const;
    int      alegeVehicul() const;
    int      alegereProdus() const;

public:
    explicit UI(Depozit& d) : depozit(d) {}
    void run();
};
