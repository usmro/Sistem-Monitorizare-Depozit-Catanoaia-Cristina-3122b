#pragma once
#include "Entitate.h"
#include <string>

// Clasa abstracta — nu poate fi instantiata direct
class Persoana : public Entitate {
protected:
    std::string nume;
public:
    Persoana(int id, const std::string& nume) : Entitate(id), nume(nume) {}
    std::string getNume() const { return nume; }
    virtual std::string getRol() const = 0; // pur virtual => clasa abstracta
    virtual ~Persoana() = default;
};
