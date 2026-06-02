#pragma once
#include "Produs.h"

class ProdusElectronic : public Produs {
private:
    std::string serie;
    int         garantieLuni;

public:
    ProdusElectronic(int id, const std::string& nume, int cantitate, double pret,
                     int pragAlerta, const std::string& locatie,
                     const std::string& serie, int garantieLuni);

    std::string getSerie()        const { return serie; }
    int         getGarantieLuni() const { return garantieLuni; }

    std::string getTip()  const override { return "electronic"; }
    void        afisare() const override;
    std::string toCSV()   const override;
};
