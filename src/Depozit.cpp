#include "Depozit.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>

Depozit::~Depozit() {
    for (auto& kv : produse)  delete kv.second;
    for (auto& kv : soferi)   delete kv.second;
    for (auto& kv : vehicule) delete kv.second;
}

std::string Depozit::timestampCurent() const {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

// ---- Produse ----------------------------------------------------------------

void Depozit::adaugaProdus(Produs* p) {
    if (produse.count(p->getId()))
        throw ProdusExistentException(p->getId());
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
    std::vector<Produs*> rezultate;
    for (auto& kv : produse) {
        std::string numeMin = kv.second->getNume();
        std::transform(numeMin.begin(), numeMin.end(), numeMin.begin(), ::tolower);
        std::string qMin = query;
        std::transform(qMin.begin(), qMin.end(), qMin.begin(), ::tolower);
        if (numeMin.find(qMin) != std::string::npos)
            rezultate.push_back(kv.second);
    }
    return rezultate;
}

std::vector<Produs*> Depozit::getProduseSuBPrag() const {
    std::vector<Produs*> rezultate;
    for (auto& kv : produse)
        if (kv.second->subPrag()) rezultate.push_back(kv.second);
    return rezultate;
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
              << std::string(90, '-') << "\n";
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

    // Demonstrarea template-ului Tranzactie<Intrare>
    Tranzactie<Intrare> t(0, soferId, vehiculId, timestampCurent());
    t.adaugaItem({produsId, p->getNume(), cantitate, p->getPret()});

    *p += cantitate;

    istoricTranzactii.push_back({
        t.getTip(), soferId, s->getNume(),
        vehiculId, v->getNrInmatriculare(),
        produsId, p->getNume(), cantitate, t.getTimestamp()
    });
}

void Depozit::iesireMarfa(int produsId, int cantitate, int soferId, int vehiculId) {
    Produs*  p = cautaDupaId(produsId);
    Sofer*   s = cautaSofer(soferId);
    Vehicul* v = cautaVehicul(vehiculId);

    Tranzactie<Iesire> t(0, soferId, vehiculId, timestampCurent());
    t.adaugaItem({produsId, p->getNume(), cantitate, p->getPret()});

    *p -= cantitate; // arunca exceptie daca insuficient

    istoricTranzactii.push_back({
        t.getTip(), soferId, s->getNume(),
        vehiculId, v->getNrInmatriculare(),
        produsId, p->getNume(), cantitate, t.getTimestamp()
    });
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
              << std::setw(6)  << "ID"
              << std::setw(25) << "NUME"
              << std::setw(15) << "NR. PERMIS"
              << "CAT.\n"
              << std::string(55, '-') << "\n";
    for (auto& kv : soferi) kv.second->afisare();
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
              << std::setw(6)  << "ID"
              << std::setw(14) << "NR. INMATR."
              << std::setw(14) << "CAPACITATE"
              << "TIP\n"
              << std::string(48, '-') << "\n";
    for (auto& kv : vehicule) kv.second->afisare();
}

// ---- Dashboard stats --------------------------------------------------------

int Depozit::getTotalProduse() const { return (int)produse.size(); }

int Depozit::getProduseSuBPragCount() const {
    int cnt = 0;
    for (auto& kv : produse) if (kv.second->subPrag()) cnt++;
    return cnt;
}

double Depozit::getValoareTotalaStoc() const {
    double total = 0;
    for (auto& kv : produse)
        total += kv.second->getPret() * kv.second->getCantitate();
    return total;
}

int Depozit::getTranzactiiCount() const { return (int)istoricTranzactii.size(); }

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
