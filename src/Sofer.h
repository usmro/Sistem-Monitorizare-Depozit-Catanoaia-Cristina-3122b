#pragma once
#include "Persoana.h"

class Sofer : public Persoana {
private:
    std::string nrPermis;
    std::string categorie; // B, C, CE

public:
    Sofer(int id, const std::string& nume, const std::string& nrPermis,
          const std::string& categorie);

    std::string getRol()       const override { return "sofer"; }
    std::string getNrPermis()  const { return nrPermis; }
    std::string getCategorie() const { return categorie; }

    void        afisare() const;
    std::string toCSV()   const;
};
