#pragma once
#include "Produs.h"
#include "Sofer.h"
#include "Vehicul.h"
#include "Tranzactie.h"
#include <unordered_map>
#include <vector>
#include <string>

struct EntryTranzactie {
    std::string tip;        // "INTRARE" / "IESIRE"
    int         soferId;
    std::string soferNume;
    int         vehiculId;
    std::string vehiculNr;
    int         produsId;
    std::string produsNume;
    int         cantitate;
    std::string timestamp;
};

class Depozit {
private:
    std::unordered_map<int, Produs*>  produse;
    std::unordered_map<int, Sofer*>   soferi;
    std::unordered_map<int, Vehicul*> vehicule;
    std::vector<EntryTranzactie>      istoricTranzactii;

    int nextProdusId  = 1;
    int nextSoferId   = 1;
    int nextVehiculId = 1;

    std::string timestampCurent() const;

public:
    Depozit() = default;
    ~Depozit();

    // --- Produse ---
    void     adaugaProdus(Produs* p);
    void     eliminaProdus(int id);
    Produs*  cautaDupaId(int id) const;
    std::vector<Produs*> cautaDupaNume(const std::string& query) const;
    std::vector<Produs*> getProduseSuBPrag() const;
    void     afisareToate() const;
    void     afisareTabelHeader() const;
    int      getNextProdusId() { return nextProdusId; }

    // --- Intrare / Iesire marfa ---
    void intrareMarfa(int produsId, int cantitate, int soferId, int vehiculId);
    void iesireMarfa (int produsId, int cantitate, int soferId, int vehiculId);

    // --- Soferi ---
    void    adaugaSofer(Sofer* s);
    Sofer*  cautaSofer(int id) const;
    void    afisareSoferi() const;
    int     getNextSoferId() { return nextSoferId; }

    // --- Vehicule ---
    void     adaugaVehicul(Vehicul* v);
    Vehicul* cautaVehicul(int id) const;
    void     afisareVehicule() const;
    int      getNextVehiculId() { return nextVehiculId; }

    // --- Dashboard stats ---
    int    getTotalProduse()        const;
    int    getProduseSuBPragCount() const;
    double getValoareTotalaStoc()   const;
    int    getTranzactiiCount()     const;
    const  EntryTranzactie* getUltimaIntrare() const;
    const  EntryTranzactie* getUltimaIesire()  const;

    // --- Acces direct pentru FileManager ---
    const std::unordered_map<int, Produs*>&  getProduse()   const { return produse; }
    const std::unordered_map<int, Sofer*>&   getSoferi()    const { return soferi; }
    const std::unordered_map<int, Vehicul*>& getVehicule()  const { return vehicule; }
    const std::vector<EntryTranzactie>&      getIstoric()   const { return istoricTranzactii; }
};
