#pragma once
#include "Produs.h"

class ProdusISBN : public Produs {
private:
    std::string isbn;       // 13 cifre
    std::string autor;
    std::string editura;
    int         anPublicare;

public:
    ProdusISBN(int id, const std::string& nume, int cantitate, double pret,
               int pragAlerta, const std::string& locatie,
               const std::string& isbn, const std::string& autor,
               const std::string& editura, int anPublicare);

    std::string getIsbn()        const { return isbn; }
    std::string getAutor()       const { return autor; }
    std::string getEditura()     const { return editura; }
    int         getAnPublicare() const { return anPublicare; }

    std::string getTip()  const override { return "isbn"; }
    void        afisare() const override;
    std::string toCSV()   const override;
};
