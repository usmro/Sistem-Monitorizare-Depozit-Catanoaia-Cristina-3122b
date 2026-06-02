#pragma once
#include "Produs.h"
#include "Sofer.h"
#include "StaffDepozit.h"
#include "Vehicul.h"
#include "Furnizor.h"
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
    std::string destinatie; // completat doar la IESIRE
};

class Depozit {
private:
    std::unordered_map<int, Produs*>       produse;
    std::unordered_map<int, Sofer*>        soferi;
    std::unordered_map<int, StaffDepozit*> staff;
    std::unordered_map<int, Vehicul*>      vehicule;
    std::unordered_map<int, Furnizor*>     furnizori;
    std::vector<EntryTranzactie>           istoricTranzactii;

    int nextProdusId   = 1;
    int nextSoferId    = 1;
    int nextStaffId    = 1;
    int nextVehiculId  = 1;
    int nextFurnizorId = 1;

    std::string timestampCurent() const;

public:
    Depozit() = default;
    ~Depozit();

    // --- Produse ---
    void     adaugaProdus(Produs* p);
    void     eliminaProdus(int id);
    Produs*  cautaDupaId(int id) const;
    std::vector<Produs*> cautaDupaNume(const std::string& query) const;
    std::vector<Produs*> cautaDupaLocatie(const std::string& locatie) const;
    Produs*  cautaDupaISBN(const std::string& isbn) const;
    Produs*  cautaDupaSerie(const std::string& serie) const;
    std::vector<Produs*> getProduseSuBPrag() const;
    std::vector<Produs*> getProduseExpirate() const;
    std::vector<Produs*> getProduseExpiraCurand(int nZile = 7) const;
    std::vector<Produs*> getRecomandare() const; // sub prag, sortate dupa cantitate
    void     afisareToate() const;
    void     afisareTabelHeader() const;
    int      getNextProdusId() { return nextProdusId; }

    // --- Intrare / Iesire marfa ---
    void intrareMarfa(int produsId, int cantitate, int soferId, int vehiculId);
    void iesireMarfa (int produsId, int cantitate, int soferId, int vehiculId,
                      const std::string& destinatie = "");
    // FEFO: scoate lotul cu data expirare cea mai apropiata
    void iesireMarfaFEFO(const std::string& numeProdus, int cantitate,
                          int soferId, int vehiculId, const std::string& destinatie = "");

    // --- Soferi ---
    void    adaugaSofer(Sofer* s);
    Sofer*  cautaSofer(int id) const;
    void    afisareSoferi() const;
    int     getNextSoferId() { return nextSoferId; }

    // --- Staff ---
    void          adaugaStaff(StaffDepozit* s);
    StaffDepozit* cautaStaff(int id) const;
    void          afisareStaff() const;
    int           getNextStaffId() { return nextStaffId; }

    // --- Vehicule ---
    void     adaugaVehicul(Vehicul* v);
    Vehicul* cautaVehicul(int id) const;
    void     afisareVehicule() const;
    int      getNextVehiculId() { return nextVehiculId; }

    // --- Furnizori ---
    void      adaugaFurnizor(Furnizor* f);
    Furnizor* cautaFurnizor(int id) const;
    void      afisareFurnizori() const;
    int       getNextFurnizorId() { return nextFurnizorId; }

    // --- Dashboard stats ---
    int    getTotalProduse()          const;
    int    getProduseSuBPragCount()   const;
    int    getProduseExpirateCount()  const;
    double getValoareTotalaStoc()     const;
    int    getTranzactiiCount()       const;
    const  EntryTranzactie* getUltimaIntrare() const;
    const  EntryTranzactie* getUltimaIesire()  const;

    // --- Acces direct pentru FileManager ---
    const std::unordered_map<int, Produs*>&       getProduse()   const { return produse; }
    const std::unordered_map<int, Sofer*>&         getSoferi()    const { return soferi; }
    const std::unordered_map<int, StaffDepozit*>&  getStaff()     const { return staff; }
    const std::unordered_map<int, Vehicul*>&       getVehicule()  const { return vehicule; }
    const std::unordered_map<int, Furnizor*>&      getFurnizori() const { return furnizori; }
    const std::vector<EntryTranzactie>&            getIstoric()   const { return istoricTranzactii; }
};
