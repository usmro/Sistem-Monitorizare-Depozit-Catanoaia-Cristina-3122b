#pragma once
#include "Entitate.h"
#include <string>

class Produs : public Entitate {
protected:
    std::string nume;
    int cantitate;
    double pret;
    int pragAlerta;
    std::string locatie; // format "ZONA-RAFT-POZ" ex: "A-03-12"

    void afisareCampuriComune() const; // fara newline, pt mostenire

public:
    Produs(int id, const std::string& nume, int cantitate, double pret,
           int pragAlerta, const std::string& locatie = "N/A");

    std::string getNume()    const { return nume; }
    int         getCantitate() const { return cantitate; }
    double      getPret()    const { return pret; }
    int         getPragAlerta() const { return pragAlerta; }
    std::string getLocatie() const { return locatie; }
    bool        subPrag()    const { return cantitate <= pragAlerta; }

    Produs& operator+=(int qty);
    Produs& operator-=(int qty);

    virtual std::string getTip()  const { return "generic"; }
    virtual void        afisare() const;
    virtual std::string toCSV()   const;

    virtual ~Produs() = default;
};
