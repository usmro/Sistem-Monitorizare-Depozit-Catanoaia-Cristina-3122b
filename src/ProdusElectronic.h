#pragma once
#include "Produs.h"

class ProdusElectronic : public Produs {
private:
    std::string serie;
    int         garantieLuni;
    double      voltaj; // ex: 220.0, 12.0, 5.0

public:
    ProdusElectronic(int id, const std::string& nume, int cantitate, double pret,
                     int pragAlerta, const std::string& locatie,
                     const std::string& serie, int garantieLuni, double voltaj = 220.0);

    std::string getSerie()        const { return serie; }
    int         getGarantieLuni() const { return garantieLuni; }
    double      getVoltaj()       const { return voltaj; }

    std::string getTip()  const override { return "electronic"; }
    void        afisare() const override;
    std::string toCSV()   const override;
};
