#include "UI.h"
#include "FileManager.h"
#include "ProdusPerisabil.h"
#include "ProdusElectronic.h"
#include "ProdusISBN.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <chrono>
#include <string>

// ---- Utilitare --------------------------------------------------------------

void UI::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void UI::pauza() const {
    std::cout << "\n  Apasa Enter pentru a continua...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void UI::linie(int latime) const {
    std::cout << "  " << std::string(latime, '-') << "\n";
}

void UI::titlu(const std::string& text) const {
    int latime = 58;
    std::cout << "\n  +" << std::string(latime, '-') << "+\n";
    int pad = (latime - (int)text.size()) / 2;
    std::cout << "  |" << std::string(pad, ' ') << text
              << std::string(latime - pad - (int)text.size(), ' ') << "|\n";
    std::cout << "  +" << std::string(latime, '-') << "+\n\n";
}

// ---- Dashboard --------------------------------------------------------------

void UI::dashboard() const {
    clearScreen();
    titlu("DASHBOARD DEPOZIT");

    int    total   = depozit.getTotalProduse();
    int    subPrag = depozit.getProduseSuBPragCount();
    double valoare = depozit.getValoareTotalaStoc();
    int    tranz   = depozit.getTranzactiiCount();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Total produse stocate  : " << total   << "\n";
    std::cout << "  Produse sub prag alerta: " << subPrag << (subPrag > 0 ? "  [!]" : "") << "\n";
    std::cout << "  Valoare totala stoc    : " << valoare << " RON\n";
    std::cout << "  Tranzactii inregistrate: " << tranz   << "\n";

    linie();

    auto ui = depozit.getUltimaIntrare();
    auto ue = depozit.getUltimaIesire();
    if (ui) std::cout << "  Ultima INTRARE : " << ui->produsNume
                      << " x" << ui->cantitate << "  [" << ui->timestamp << "]\n";
    if (ue) std::cout << "  Ultima IESIRE  : " << ue->produsNume
                      << " x" << ue->cantitate << "  [" << ue->timestamp << "]\n";
    if (!ui && !ue) std::cout << "  (nicio tranzactie inregistrata)\n";
    std::cout << "\n";
}

// ---- Helpers: introducere date ----------------------------------------------

int UI::alegereProdus() const {
    depozit.afisareToate();
    std::cout << "\n  ID produs: ";
    int id; std::cin >> id;
    return id;
}

int UI::alegereSofer() const {
    depozit.afisareSoferi();
    std::cout << "\n  ID sofer: ";
    int id; std::cin >> id;
    return id;
}

int UI::alegeVehicul() const {
    depozit.afisareVehicule();
    std::cout << "\n  ID vehicul: ";
    int id; std::cin >> id;
    return id;
}

Produs* UI::introduceProdus() {
    std::cout << "  Tip produs:\n"
              << "  [1] Generic\n"
              << "  [2] Perisabil\n"
              << "  [3] Electronic\n"
              << "  [4] ISBN (carte)\n"
              << "  Alegere: ";
    int tip; std::cin >> tip;
    std::cin.ignore();

    int id = depozit.getNextProdusId();
    std::cout << "  Nume: ";
    std::string nume; std::getline(std::cin, nume);

    std::cout << "  Cantitate: ";
    int cant; std::cin >> cant;

    std::cout << "  Pret (RON): ";
    double pret; std::cin >> pret;

    std::cout << "  Prag alerta: ";
    int prag; std::cin >> prag;
    std::cin.ignore();

    std::cout << "  Locatie (ex: A-03-12): ";
    std::string loc; std::getline(std::cin, loc);
    if (loc.empty()) loc = "N/A";

    if (tip == 2) {
        std::cout << "  Data expirare (YYYY-MM-DD): ";
        std::string data; std::getline(std::cin, data);
        std::cout << "  Temperatura stocare (grade C): ";
        double temp; std::cin >> temp;
        return new ProdusPerisabil(id, nume, cant, pret, prag, loc, data, temp);
    }
    if (tip == 3) {
        std::cout << "  Numar serie: ";
        std::string serie; std::cin.ignore(); std::getline(std::cin, serie);
        std::cout << "  Garantie (luni): ";
        int gar; std::cin >> gar;
        return new ProdusElectronic(id, nume, cant, pret, prag, loc, serie, gar);
    }
    if (tip == 4) {
        std::cout << "  ISBN (13 cifre): ";
        std::string isbn; std::cin.ignore(); std::getline(std::cin, isbn);
        std::cout << "  Autor: ";
        std::string autor; std::getline(std::cin, autor);
        std::cout << "  Editura: ";
        std::string editura; std::getline(std::cin, editura);
        std::cout << "  An publicare: ";
        int an; std::cin >> an;
        return new ProdusISBN(id, nume, cant, pret, prag, loc, isbn, autor, editura, an);
    }
    return new Produs(id, nume, cant, pret, prag, loc);
}

Sofer* UI::introduceSofer() {
    int id = depozit.getNextSoferId();
    std::cin.ignore();
    std::cout << "  Nume sofer: ";
    std::string nume; std::getline(std::cin, nume);
    std::cout << "  Nr. permis: ";
    std::string perm; std::getline(std::cin, perm);
    std::cout << "  Categorie (B/C/CE): ";
    std::string cat; std::getline(std::cin, cat);
    return new Sofer(id, nume, perm, cat);
}

Vehicul* UI::introduceVehicul() {
    int id = depozit.getNextVehiculId();
    std::cin.ignore();
    std::cout << "  Nr. inmatriculare: ";
    std::string nr; std::getline(std::cin, nr);
    std::cout << "  Capacitate (kg): ";
    double cap; std::cin >> cap;
    std::cin.ignore();
    std::cout << "  Tip (camion/utilitara/furgoneta): ";
    std::string tip; std::getline(std::cin, tip);
    return new Vehicul(id, nr, cap, tip);
}

// ---- Meniu Produse ----------------------------------------------------------

void UI::menuProduse() {
    int opt;
    do {
        clearScreen();
        titlu("GESTIONARE PRODUSE");
        std::cout << "  [1] Vizualizare toate produsele\n"
                  << "  [2] Cauta dupa ID\n"
                  << "  [3] Cauta dupa nume\n"
                  << "  [4] Adauga produs nou\n"
                  << "  [5] Elimina produs\n"
                  << "  [0] Inapoi\n\n"
                  << "  Alegere: ";
        std::cin >> opt;

        clearScreen();
        try {
            if (opt == 1) {
                titlu("TOATE PRODUSELE");
                depozit.afisareToate();
            } else if (opt == 2) {
                std::cout << "  ID produs: ";
                int id; std::cin >> id;
                titlu("REZULTAT CAUTARE");
                depozit.cautaDupaId(id)->afisare();
            } else if (opt == 3) {
                std::cin.ignore();
                std::cout << "  Termen cautare: ";
                std::string q; std::getline(std::cin, q);
                auto rez = depozit.cautaDupaNume(q);
                titlu("REZULTATE CAUTARE: " + q);
                if (rez.empty()) std::cout << "  Niciun rezultat.\n";
                else { depozit.afisareTabelHeader(); for (auto* p : rez) p->afisare(); }
            } else if (opt == 4) {
                titlu("ADAUGA PRODUS");
                Produs* p = introduceProdus();
                depozit.adaugaProdus(p);
                std::cout << "\n  Produs adaugat cu ID " << p->getId() << "\n";
                FileManager::salveazaStoc(depozit);
            } else if (opt == 5) {
                std::cout << "  ID produs de eliminat: ";
                int id; std::cin >> id;
                depozit.eliminaProdus(id);
                std::cout << "  Produs eliminat.\n";
                FileManager::salveazaStoc(depozit);
            }
        } catch (const DepozitException& e) {
            std::cout << "\n  EROARE: " << e.what() << "\n";
        }
        if (opt != 0) pauza();
    } while (opt != 0);
}

// ---- Meniu Intrare Marfa ----------------------------------------------------

void UI::menuIntrareMarfa() {
    clearScreen();
    titlu("INTRARE MARFA IN DEPOZIT");
    try {
        std::cout << "--- Selecteaza produs ---\n";
        int pid = alegereProdus();
        std::cout << "\n  Cantitate intrata: ";
        int cant; std::cin >> cant;
        std::cout << "\n--- Selecteaza sofer ---\n";
        int sid = alegereSofer();
        std::cout << "\n--- Selecteaza vehicul ---\n";
        int vid = alegeVehicul();

        depozit.intrareMarfa(pid, cant, sid, vid);
        std::cout << "\n  Intrare inregistrata cu succes.\n";
        FileManager::salveazaStoc(depozit);
    } catch (const DepozitException& e) {
        std::cout << "\n  EROARE: " << e.what() << "\n";
    }
    pauza();
}

// ---- Meniu Iesire Marfa -----------------------------------------------------

void UI::menuIesireMarfa() {
    clearScreen();
    titlu("IESIRE MARFA DIN DEPOZIT");
    try {
        std::cout << "--- Selecteaza produs ---\n";
        int pid = alegereProdus();
        std::cout << "\n  Cantitate iesita: ";
        int cant; std::cin >> cant;
        std::cout << "\n--- Selecteaza sofer ---\n";
        int sid = alegereSofer();
        std::cout << "\n--- Selecteaza vehicul ---\n";
        int vid = alegeVehicul();

        depozit.iesireMarfa(pid, cant, sid, vid);
        std::cout << "\n  Iesire inregistrata cu succes.\n";
        FileManager::salveazaStoc(depozit);
    } catch (const DepozitException& e) {
        std::cout << "\n  EROARE: " << e.what() << "\n";
    }
    pauza();
}

// ---- Meniu Rapoarte ---------------------------------------------------------

void UI::menuRapoarte() const {
    int opt;
    do {
        clearScreen();
        titlu("RAPOARTE");
        std::cout << "  [1] Produse sub prag de alerta\n"
                  << "  [2] Istoric tranzactii\n"
                  << "  [3] Test performanta (1 milion produse)\n"
                  << "  [0] Inapoi\n\n"
                  << "  Alegere: ";
        std::cin >> opt;

        clearScreen();
        if (opt == 1) {
            titlu("PRODUSE SUB PRAG");
            auto lista = depozit.getProduseSuBPrag();
            if (lista.empty()) std::cout << "  Toate produsele sunt peste prag.\n";
            else {
                depozit.afisareTabelHeader();
                for (auto* p : lista) p->afisare();
                std::cout << "\n  Total: " << lista.size() << " produse sub prag.\n";
            }
        } else if (opt == 2) {
            titlu("ISTORIC TRANZACTII");
            const auto& ist = depozit.getIstoric();
            if (ist.empty()) { std::cout << "  Nicio tranzactie.\n"; }
            else {
                std::cout << std::left
                          << std::setw(10) << "TIP"
                          << std::setw(20) << "SOFER"
                          << std::setw(14) << "VEHICUL"
                          << std::setw(22) << "PRODUS"
                          << std::setw(8)  << "CANT."
                          << "TIMESTAMP\n"
                          << std::string(90, '-') << "\n";
                for (auto& t : ist)
                    std::cout << std::left
                              << std::setw(10) << t.tip
                              << std::setw(20) << t.soferNume
                              << std::setw(14) << t.vehiculNr
                              << std::setw(22) << t.produsNume
                              << std::setw(8)  << t.cantitate
                              << t.timestamp << "\n";
            }
        } else if (opt == 3) {
            titlu("TEST PERFORMANTA — 1 MILION PRODUSE");
            std::cout << "  Generez 1.000.000 produse...\n";

            Depozit testDepozit;
            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 1; i <= 1000000; i++)
                testDepozit.adaugaProdus(new Produs(i, "Produs_" + std::to_string(i),
                                                     100, 9.99, 10));

            auto end = std::chrono::high_resolution_clock::now();
            auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "  Inserare: " << ms << " ms\n";

            start = std::chrono::high_resolution_clock::now();
            testDepozit.cautaDupaId(500000);
            end = std::chrono::high_resolution_clock::now();
            auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            std::cout << "  Cautare ID 500000: " << us << " microsecunde\n";

            std::cout << "  Total produse: " << testDepozit.getTotalProduse() << "\n";
        }
        if (opt != 0) pauza();
    } while (opt != 0);
}

// ---- Meniu Export -----------------------------------------------------------

void UI::menuExport() const {
    clearScreen();
    titlu("EXPORT DATE");

    time_t now = time(nullptr);
    char buf[12];
    strftime(buf, sizeof(buf), "%Y%m%d", localtime(&now));
    std::string data(buf);

    std::string caleStoc = "data/stoc_" + data + ".csv";
    std::string caleIst  = "data/tranzactii_" + data + ".csv";

    FileManager::exportStoc(depozit, caleStoc);
    FileManager::exportIstoric(depozit, caleIst);

    std::cout << "\n  Fisierele au fost salvate in folderul data/\n";
    pauza();
}

// ---- Meniu Soferi & Vehicule ------------------------------------------------

void UI::menuSoferiVehicule() {
    int opt;
    do {
        clearScreen();
        titlu("SOFERI SI VEHICULE");
        std::cout << "  [1] Lista soferi\n"
                  << "  [2] Adauga sofer\n"
                  << "  [3] Lista vehicule\n"
                  << "  [4] Adauga vehicul\n"
                  << "  [0] Inapoi\n\n"
                  << "  Alegere: ";
        std::cin >> opt;

        clearScreen();
        try {
            if (opt == 1) { titlu("SOFERI"); depozit.afisareSoferi(); }
            else if (opt == 2) {
                titlu("ADAUGA SOFER");
                Sofer* s = introduceSofer();
                depozit.adaugaSofer(s);
                std::cout << "\n  Sofer adaugat cu ID " << s->getId() << "\n";
                FileManager::salveazaSoferiVehicule(depozit);
            } else if (opt == 3) { titlu("VEHICULE"); depozit.afisareVehicule(); }
            else if (opt == 4) {
                titlu("ADAUGA VEHICUL");
                Vehicul* v = introduceVehicul();
                depozit.adaugaVehicul(v);
                std::cout << "\n  Vehicul adaugat cu ID " << v->getId() << "\n";
                FileManager::salveazaSoferiVehicule(depozit);
            }
        } catch (const DepozitException& e) {
            std::cout << "\n  EROARE: " << e.what() << "\n";
        }
        if (opt != 0) pauza();
    } while (opt != 0);
}

// ---- Run (bucla principala) -------------------------------------------------

void UI::run() {
    int opt;
    do {
        clearScreen();
        dashboard();

        std::cout << "  +----------------------------------+\n"
                  << "  |       DEPOZIT MANAGER v1.0       |\n"
                  << "  +----------------------------------+\n"
                  << "  |  [1] Gestionare produse          |\n"
                  << "  |  [2] Intrare marfa                |\n"
                  << "  |  [3] Iesire marfa                 |\n"
                  << "  |  [4] Soferi si vehicule           |\n"
                  << "  |  [5] Rapoarte                     |\n"
                  << "  |  [6] Export date                  |\n"
                  << "  |  [0] Iesire                       |\n"
                  << "  +----------------------------------+\n\n"
                  << "  Alegere: ";
        std::cin >> opt;

        if      (opt == 1) menuProduse();
        else if (opt == 2) menuIntrareMarfa();
        else if (opt == 3) menuIesireMarfa();
        else if (opt == 4) menuSoferiVehicule();
        else if (opt == 5) menuRapoarte();
        else if (opt == 6) menuExport();
    } while (opt != 0);

    FileManager::salveazaStoc(depozit);
    FileManager::salveazaSoferiVehicule(depozit);
    std::cout << "\n  Date salvate. La revedere!\n";
}
