#pragma once
#include "Entitate.h"
#include <string>
#include <vector>

class Furnizor : public Entitate {
private:
    std::string      nume;
    std::string      contact;
    std::string      adresa;
    std::vector<int> produseIds; // ID-urile produselor pe care le furnizeaza

public:
    Furnizor(int id, const std::string& nume,
             const std::string& contact, const std::string& adresa = "");

    std::string getNume()    const { return nume; }
    std::string getContact() const { return contact; }
    std::string getAdresa()  const { return adresa; }
    const std::vector<int>& getProduseIds() const { return produseIds; }

    void adaugaProdusAsociat(int produsId) { produseIds.push_back(produsId); }

    void        afisare() const;
    std::string toCSV()   const;
};
