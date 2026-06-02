#pragma once
#include "Entitate.h"
#include <string>

class Sofer : public Entitate {
private:
    std::string nume;
    std::string nrPermis;
    std::string categorie; // B, C, CE

public:
    Sofer(int id, const std::string& nume, const std::string& nrPermis,
          const std::string& categorie);

    std::string getNume()      const { return nume; }
    std::string getNrPermis()  const { return nrPermis; }
    std::string getCategorie() const { return categorie; }

    void        afisare() const;
    std::string toCSV()   const;
};
