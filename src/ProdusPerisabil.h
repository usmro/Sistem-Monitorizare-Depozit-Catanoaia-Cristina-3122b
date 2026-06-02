#pragma once
#include "Produs.h"

class ProdusPerisabil : public Produs {
private:
    std::string dataExpirare;   // "YYYY-MM-DD"
    double      temperaturaStorare; // grade Celsius

public:
    ProdusPerisabil(int id, const std::string& nume, int cantitate, double pret,
                    int pragAlerta, const std::string& locatie,
                    const std::string& dataExpirare, double temp);

    std::string getDataExpirare()      const { return dataExpirare; }
    double      getTemperaturaStorare() const { return temperaturaStorare; }

    std::string getTip()  const override { return "perisabil"; }
    void        afisare() const override;
    std::string toCSV()   const override;
};
