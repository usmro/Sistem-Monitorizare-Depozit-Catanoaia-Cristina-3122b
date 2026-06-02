#pragma once
#include "Produs.h"

class ProdusPerisabil : public Produs {
private:
    std::string dataExpirare;    // "YYYY-MM-DD"
    double      temperaturaStorare;

    static std::string dataAziStr();           // returneaza data de azi ca string
    static std::string dataViitoareStr(int n); // returneaza data de azi + n zile

public:
    ProdusPerisabil(int id, const std::string& nume, int cantitate, double pret,
                    int pragAlerta, const std::string& locatie,
                    const std::string& dataExpirare, double temp);

    std::string getDataExpirare()       const { return dataExpirare; }
    double      getTemperaturaStorare() const { return temperaturaStorare; }

    bool esteExpirat()         const; // data expirare < azi
    bool expiraInNZile(int n)  const; // nu e expirat, dar expira in cel mult n zile

    std::string getTip()  const override { return "perisabil"; }
    void        afisare() const override;
    std::string toCSV()   const override;
};
