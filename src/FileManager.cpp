#include "FileManager.h"
#include "ProdusPerisabil.h"
#include "ProdusElectronic.h"
#include "ProdusISBN.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::string> FileManager::split(const std::string& line, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delim)) tokens.push_back(token);
    return tokens;
}

// CSV format: tip,id,nume,cantitate,pret,prag,locatie,dataUltimaModif[,campuri_extra]
Produs* FileManager::produsFromCSV(const std::string& line) {
    auto f = split(line, ',');
    if (f.size() < 7) return nullptr;
    try {
        std::string tip  = f[0];
        int    id        = std::stoi(f[1]);
        std::string nume = f[2];
        int    cant      = std::stoi(f[3]);
        double pret      = std::stod(f[4]);
        int    prag      = std::stoi(f[5]);
        std::string loc  = f[6];
        // f[7] = dataUltimaModificare (optional, ignorat la incarcare — se seteaza la instantiere)

        if (tip == "perisabil" && f.size() >= 10)
            return new ProdusPerisabil(id, nume, cant, pret, prag, loc, f[8], std::stod(f[9]));
        if (tip == "electronic" && f.size() >= 10) {
            double volt = (f.size() >= 11) ? std::stod(f[10]) : 220.0;
            return new ProdusElectronic(id, nume, cant, pret, prag, loc, f[8], std::stoi(f[9]), volt);
        }
        if (tip == "isbn" && f.size() >= 12)
            return new ProdusISBN(id, nume, cant, pret, prag, loc, f[8], f[9], f[10], std::stoi(f[11]));
        return new Produs(id, nume, cant, pret, prag, loc);
    } catch (...) { return nullptr; }
}

// ---- Stoc -------------------------------------------------------------------

void FileManager::salveazaStoc(const Depozit& d, const std::string& cale) {
    std::ofstream f(cale);
    if (!f) { std::cerr << "Nu pot scrie: " << cale << "\n"; return; }
    for (auto& kv : d.getProduse())
        f << kv.second->toCSV() << "\n";
}

void FileManager::incarcaStoc(Depozit& d, const std::string& cale) {
    std::ifstream f(cale);
    if (!f) return;
    std::string line;
    int ok = 0, erori = 0;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        Produs* p = produsFromCSV(line);
        if (p) { try { d.adaugaProdus(p); ok++; } catch (...) { delete p; erori++; } }
        else erori++;
    }
    std::cout << "  Stoc incarcat: " << ok << " produse";
    if (erori) std::cout << ", " << erori << " erori ignorate";
    std::cout << "\n";
}

// ---- Soferi & Vehicule ------------------------------------------------------

void FileManager::salveazaSoferiVehicule(const Depozit& d,
                                          const std::string& caleSoferi,
                                          const std::string& caleVehicule) {
    std::ofstream fs(caleSoferi);
    if (fs) for (auto& kv : d.getSoferi())   fs << kv.second->toCSV() << "\n";
    std::ofstream fv(caleVehicule);
    if (fv) for (auto& kv : d.getVehicule()) fv << kv.second->toCSV() << "\n";
}

void FileManager::incarcaSoferiVehicule(Depozit& d,
                                         const std::string& caleSoferi,
                                         const std::string& caleVehicule) {
    {
        std::ifstream f(caleSoferi);
        std::string line;
        while (std::getline(f, line)) {
            if (line.empty()) continue;
            auto fields = split(line, ',');
            if (fields.size() < 4) continue;
            try { d.adaugaSofer(new Sofer(std::stoi(fields[0]), fields[1], fields[2], fields[3])); }
            catch (...) {}
        }
    }
    {
        std::ifstream f(caleVehicule);
        std::string line;
        while (std::getline(f, line)) {
            if (line.empty()) continue;
            auto fields = split(line, ',');
            if (fields.size() < 4) continue;
            try { d.adaugaVehicul(new Vehicul(std::stoi(fields[0]), fields[1], std::stod(fields[2]), fields[3])); }
            catch (...) {}
        }
    }
}

// ---- Export rapoarte --------------------------------------------------------

void FileManager::exportStoc(const Depozit& d, const std::string& cale) {
    std::ofstream f(cale);
    if (!f) { std::cerr << "Nu pot crea: " << cale << "\n"; return; }
    f << "tip,id,nume,cantitate,pret,pragAlerta,locatie,data_ultima_miscare\n";
    for (auto& kv : d.getProduse()) {
        Produs* p = kv.second;
        f << p->getTip() << ","
          << p->getId() << ","
          << p->getNume() << ","
          << p->getCantitate() << ","
          << p->getPret() << ","
          << p->getPragAlerta() << ","
          << p->getLocatie() << ","
          << p->getDataUltimaModificare() << "\n";
    }
    std::cout << "  Export stoc -> " << cale << "\n";
}

void FileManager::exportIstoric(const Depozit& d, const std::string& cale) {
    std::ofstream f(cale);
    if (!f) { std::cerr << "Nu pot crea: " << cale << "\n"; return; }
    f << "tip,sofer_id,sofer,vehicul_id,vehicul_nr,produs_id,produs,cantitate,timestamp,destinatie\n";
    for (auto& t : d.getIstoric())
        f << t.tip << "," << t.soferId << "," << t.soferNume << ","
          << t.vehiculId << "," << t.vehiculNr << ","
          << t.produsId << "," << t.produsNume << ","
          << t.cantitate << "," << t.timestamp << ","
          << t.destinatie << "\n";
    std::cout << "  Export istoric -> " << cale << "\n";
}
