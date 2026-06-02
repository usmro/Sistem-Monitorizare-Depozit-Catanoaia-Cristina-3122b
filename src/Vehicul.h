#pragma once
#include "Entitate.h"
#include <string>

class Vehicul : public Entitate {
private:
    std::string nrInmatriculare;
    double      capacitateKg;
    std::string tip; // camion, utilitara, furgoneta

public:
    Vehicul(int id, const std::string& nrInmatriculare, double capacitateKg,
            const std::string& tip);

    std::string getNrInmatriculare() const { return nrInmatriculare; }
    double      getCapacitateKg()   const { return capacitateKg; }
    std::string getTip()            const { return tip; }

    void        afisare() const;
    std::string toCSV()   const;
};
