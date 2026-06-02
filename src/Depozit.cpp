#include "Depozit.h"
#include "ProdusPerisabil.h"
#include "ProdusElectronic.h"
#include "ProdusISBN.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>

Depozit::~Depozit() {
    for (auto& kv : produse)   delete kv.second;
    for (auto& kv : soferi)    delete kv.second;
    for (auto& kv : staff)     delete kv.second;
    for (auto& kv : vehicule)  delete kv.second;
    for (auto& kv : furnizori) delete kv.second;
}

std::string Depozit::timestampCurent() const {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

// ---- Produse ----------------------------------------------------------------

void Depozit::adaugaProdus(Produs* p) {
    if (produse.count(p->getId())) throw ProdusExistentException(p->getId());
    produse[p->getId()] = p;
    if (p->getId() >= nextProdusId) nextProdusId = p->getId() + 1;
}

void Depozit::eliminaProdus(int id) {
    auto it = produse.find(id);
    if (it == produse.end()) throw ProdusInexistentException(id);
    delete it->second;
    produse.erase(it);
}

Produs* Depozit::cautaDupaId(int id) const {
    auto it = produse.find(id);
    if (it == produse.end()) throw ProdusInexistentException(id);
    return it->second;
}

std::vector<Produs*> Depozit::cautaDupaNume(const std::string& query) const {
    std::vector<Produs*> rez;
    std::string q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);
    for (auto& kv : produse) {
        std::string n = kv.second->getNume();
        std::transform(n.begin(), n.end(), n.begin(), ::tolower);
        if (n.find(q) != std::string::npos) rez.push_back(kv.second);
    }
    return rez;
}

std::vector<Produs*> Depozit::cautaDupaLocatie(const std::string& locatie) const {
    std::vector<Produs*> rez;
    for (auto& kv : produse)
        if (kv.second->getLocatie() == locatie) rez.push_back(kv.second);
    return rez;
}

Produs* Depozit::cautaDupaISBN(const std::string& isbn) const {
    for (auto& kv : produse) {
        auto* pi = dynamic_cast<ProdusISBN*>(kv.second);
        if (pi && pi->getIsbn() == isbn) return pi;
    }
    throw DepozitException("Produs cu ISBN " + isbn + " nu a fost gasit");
}

Produs* Depozit::cautaDupaSerie(const std::string& serie) const {
    for (auto& kv : produse) {
        auto* pe = dynamic_cast<ProdusElectronic*>(kv.second);
        if (pe && pe->getSerie() == serie) return pe;
    }
    throw DepozitException("Produs electronic cu seria " + serie + " nu a fost gasit");
}

std::vector<Produs*> Depozit::getProduseSuBPrag() const {
    std::vector<Produs*> rez;
    for (auto& kv : produse)
        if (kv.second->subPrag()) rez.push_back(kv.second);
    return rez;
}

std::vector<Produs*> Depozit::getProduseExpirate() const {
    std::vector<Produs*> rez;
    for (auto& kv : produse) {
        auto* pp = dynamic_cast<ProdusPerisabil*>(kv.second);
        if (pp && pp->esteExpirat()) rez.push_back(pp);
    }
    return rez;
}

std::vector<Produs*> Depozit::getProduseExpiraCurand(int nZile) const {
    std::vector<Produs*> rez;
    for (auto& kv : produse) {
        auto* pp = dynamic_cast<ProdusPerisabil*>(kv.second);
        if (pp && pp->expiraInNZile(nZile)) rez.push_back(pp);
    }
    // sorteaza dupa data expirare (string ISO => comparatie lexicografica corecta)
    std::sort(rez.begin(), rez.end(), [](Produs* a, Produs* b) {
        return dynamic_cast<ProdusPerisabil*>(a)->getDataExpirare() <
               dynamic_cast<ProdusPerisabil*>(b)->getDataExpirare();
    });
    return rez;
}

std::vector<Produs*> Depozit::getRecomandare() const {
    std::vector<Produs*> rez;
    for (auto& kv : produse)
        if (kv.second->subPrag()) rez.push_back(kv.second);
    // sorteaza dupa cantitate ascendent: cele mai urgente primele
    std::sort(rez.begin(), rez.end(), [](Produs* a, Produs* b) {
        return a->getCantitate() < b->getCantitate();
    });
    return rez;
}

void Depozit::afisareTabelHeader() const {
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(12) << "TIP"
              << std::setw(24) << "NUME"
              << std::setw(10) << "CANT."
              << std::setw(10) << "PRET"
              << std::setw(8)  << "PRAG"
              << std::setw(12) << "LOCATIE"
              << "DETALII\n"
              << std::string(92, '-') << "\n";
}

void Depozit::afisareToate() const {
    if (produse.empty()) { std::cout << "  Depozitul este gol.\n"; return; }
    afisareTabelHeader();
    for (auto& kv : produse) kv.second->afisare();
}

// ---- Intrare / Iesire -------------------------------------------------------

void Depozit::intrareMarfa(int produsId, int cantitate, int soferId, int vehiculId) {
    Produs*  p = cautaDupaId(produsId);
    Sofer*   s = cautaSofer(soferId);
    Vehicul* v = cautaVehicul(vehiculId);

    Tranzactie<Intrare> t(0, soferId, vehiculId, timestampCurent());
    t.adaugaItem({produsId, p->getNume(), cantitate, p->getPret()});

    *p += cantitate;

    istoricTranzactii.push_back({
        t.getTip(), soferId, s->getNume(),
        vehiculId, v->getNrInmatriculare(),
        produsId, p->getNume(), cantitate, t.getTimestamp(), ""
    });
}

void Depozit::iesireMarfa(int produsId, int cantitate, int soferId, int vehiculId,
                           const std::string& destinatie) {
    Produs*  p = cautaDupaId(produsId);
    Sofer*   s = cautaSofer(soferId);
    Vehicul* v = cautaVehicul(vehiculId);

    Tranzactie<Iesire> t(0, soferId, vehiculId, timestampCurent());
    t.adaugaItem({produsId, p->getNume(), cantitate, p->getPret()});

    *p -= cantitate;

    istoricTranzactii.push_back({
        t.getTip(), soferId, s->getNume(),
        vehiculId, v->getNrInmatriculare(),
        produsId, p->getNume(), cantitate, t.getTimestamp(), destinatie
    });
}

void Depozit::iesireMarfaFEFO(const std::string& numeProdus, int cantitate,
                                int soferId, int vehiculId, const std::string& destinatie) {
    // Gaseste toate ProdusPerisabil cu numele dat, nevandute si neexpirate
    std::vector<ProdusPerisabil*> candidati;
    for (auto& kv : produse) {
        auto* pp = dynamic_cast<ProdusPerisabil*>(kv.second);
        if (pp && pp->getNume() == numeProdus && pp->getCantitate() > 0 && !pp->esteExpirat())
            candidati.push_back(pp);
    }
    if (candidati.empty())
        throw DepozitException("Niciun lot valid de '" + numeProdus + "' disponibil pentru FEFO");

    // Sorteaza dupa data expirare — primul expirat se ia primul (FEFO)
    std::sort(candidati.begin(), candidati.end(), [](ProdusPerisabil* a, ProdusPerisabil* b) {
        return a->getDataExpirare() < b->getDataExpirare();
    });

    iesireMarfa(candidati[0]->getId(), cantitate, soferId, vehiculId, destinatie);
}

// ---- Soferi -----------------------------------------------------------------

void Depozit::adaugaSofer(Sofer* s) {
    if (soferi.count(s->getId()))
        throw DepozitException("Sofer cu ID " + std::to_string(s->getId()) + " exista deja");
    soferi[s->getId()] = s;
    if (s->getId() >= nextSoferId) nextSoferId = s->getId() + 1;
}

Sofer* Depozit::cautaSofer(int id) const {
    auto it = soferi.find(id);
    if (it == soferi.end()) throw EntitateInexistentaException("Sofer", id);
    return it->second;
}

void Depozit::afisareSoferi() const {
    if (soferi.empty()) { std::cout << "  Niciun sofer inregistrat.\n"; return; }
    std::cout << std::left
              << std::setw(6) << "ID" << std::setw(25) << "NUME"
              << std::setw(15) << "NR. PERMIS" << "CAT.\n"
              << std::string(55, '-') << "\n";
    for (auto& kv : soferi) kv.second->afisare();
}

// ---- Staff ------------------------------------------------------------------

void Depozit::adaugaStaff(StaffDepozit* s) {
    if (staff.count(s->getId()))
        throw DepozitException("Staff cu ID " + std::to_string(s->getId()) + " exista deja");
    staff[s->getId()] = s;
    if (s->getId() >= nextStaffId) nextStaffId = s->getId() + 1;
}

StaffDepozit* Depozit::cautaStaff(int id) const {
    auto it = staff.find(id);
    if (it == staff.end()) throw EntitateInexistentaException("Staff", id);
    return it->second;
}

void Depozit::afisareStaff() const {
    if (staff.empty()) { std::cout << "  Niciun membru staff inregistrat.\n"; return; }
    std::cout << std::left
              << std::setw(6) << "ID" << std::setw(25) << "NUME"
              << std::setw(14) << "ROL" << "USERNAME\n"
              << std::string(62, '-') << "\n";
    for (auto& kv : staff) kv.second->afisare();
}

// ---- Vehicule ---------------------------------------------------------------

void Depozit::adaugaVehicul(Vehicul* v) {
    if (vehicule.count(v->getId()))
        throw DepozitException("Vehicul cu ID " + std::to_string(v->getId()) + " exista deja");
    vehicule[v->getId()] = v;
    if (v->getId() >= nextVehiculId) nextVehiculId = v->getId() + 1;
}

Vehicul* Depozit::cautaVehicul(int id) const {
    auto it = vehicule.find(id);
    if (it == vehicule.end()) throw EntitateInexistentaException("Vehicul", id);
    return it->second;
}

void Depozit::afisareVehicule() const {
    if (vehicule.empty()) { std::cout << "  Niciun vehicul inregistrat.\n"; return; }
    std::cout << std::left
              << std::setw(6) << "ID" << std::setw(14) << "NR. INMATR."
              << std::setw(14) << "CAPACITATE" << "TIP\n"
              << std::string(48, '-') << "\n";
    for (auto& kv : vehicule) kv.second->afisare();
}

// ---- Furnizori --------------------------------------------------------------

void Depozit::adaugaFurnizor(Furnizor* f) {
    if (furnizori.count(f->getId()))
        throw DepozitException("Furnizor cu ID " + std::to_string(f->getId()) + " exista deja");
    furnizori[f->getId()] = f;
    if (f->getId() >= nextFurnizorId) nextFurnizorId = f->getId() + 1;
}

Furnizor* Depozit::cautaFurnizor(int id) const {
    auto it = furnizori.find(id);
    if (it == furnizori.end()) throw EntitateInexistentaException("Furnizor", id);
    return it->second;
}

void Depozit::afisareFurnizori() const {
    if (furnizori.empty()) { std::cout << "  Niciun furnizor inregistrat.\n"; return; }
    std::cout << std::left
              << std::setw(6) << "ID" << std::setw(25) << "NUME"
              << std::setw(20) << "CONTACT" << std::setw(30) << "ADRESA" << "PRODUSE\n"
              << std::string(88, '-') << "\n";
    for (auto& kv : furnizori) kv.second->afisare();
}

// ---- Dashboard --------------------------------------------------------------

int Depozit::getTotalProduse()         const { return (int)produse.size(); }
int Depozit::getTranzactiiCount()      const { return (int)istoricTranzactii.size(); }

int Depozit::getProduseSuBPragCount() const {
    int c = 0;
    for (auto& kv : produse) if (kv.second->subPrag()) c++;
    return c;
}

int Depozit::getProduseExpirateCount() const {
    int c = 0;
    for (auto& kv : produse) {
        auto* pp = dynamic_cast<ProdusPerisabil*>(kv.second);
        if (pp && pp->esteExpirat()) c++;
    }
    return c;
}

double Depozit::getValoareTotalaStoc() const {
    double total = 0;
    for (auto& kv : produse)
        total += kv.second->getPret() * kv.second->getCantitate();
    return total;
}

const EntryTranzactie* Depozit::getUltimaIntrare() const {
    for (int i = (int)istoricTranzactii.size() - 1; i >= 0; i--)
        if (istoricTranzactii[i].tip == "INTRARE") return &istoricTranzactii[i];
    return nullptr;
}

const EntryTranzactie* Depozit::getUltimaIesire() const {
    for (int i = (int)istoricTranzactii.size() - 1; i >= 0; i--)
        if (istoricTranzactii[i].tip == "IESIRE") return &istoricTranzactii[i];
    return nullptr;
}
