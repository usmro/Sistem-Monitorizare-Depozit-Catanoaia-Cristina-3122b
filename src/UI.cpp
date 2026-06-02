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

void UI::linie(int w) const { std::cout << "  " << std::string(w, '-') << "\n"; }

void UI::titlu(const std::string& text) const {
    int w = 58;
    std::cout << "\n  +" << std::string(w, '-') << "+\n";
    int pad = (w - (int)text.size()) / 2;
    std::cout << "  |" << std::string(pad, ' ') << text
              << std::string(w - pad - (int)text.size(), ' ') << "|\n";
    std::cout << "  +" << std::string(w, '-') << "+\n\n";
}

// ---- Dashboard --------------------------------------------------------------

void UI::dashboard() const {
    clearScreen();
    titlu("DASHBOARD DEPOZIT");

    int    total    = depozit.getTotalProduse();
    int    subPrag  = depozit.getProduseSuBPragCount();
    int    expirate = depozit.getProduseExpirateCount();
    double valoare  = depozit.getValoareTotalaStoc();
    int    tranz    = depozit.getTranzactiiCount();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Total produse stocate   : " << total   << "\n";
    std::cout << "  Produse sub prag alerta : " << subPrag  << (subPrag  > 0 ? "  [!]" : "") << "\n";
    std::cout << "  Produse expirate        : " << expirate << (expirate > 0 ? "  [X]" : "") << "\n";
    std::cout << "  Valoare totala stoc     : " << valoare  << " RON\n";
    std::cout << "  Tranzactii inregistrate : " << tranz    << "\n";
    linie();
    auto ui = depozit.getUltimaIntrare();
    auto ue = depozit.getUltimaIesire();
    if (ui) std::cout << "  Ultima INTRARE : " << ui->produsNume << " x" << ui->cantitate
                      << "  [" << ui->timestamp << "]\n";
    if (ue) std::cout << "  Ultima IESIRE  : " << ue->produsNume << " x" << ue->cantitate
                      << "  [" << ue->timestamp << "]"
                      << (ue->destinatie.empty() ? "" : "  -> " + ue->destinatie) << "\n";
    if (!ui && !ue) std::cout << "  (nicio tranzactie inregistrata)\n";
    std::cout << "\n";
}

// ---- Helpers ----------------------------------------------------------------

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
              << "  [1] Generic\n  [2] Perisabil\n  [3] Electronic\n  [4] ISBN (carte)\n"
              << "  Alegere: ";
    int tip; std::cin >> tip;
    std::cin.ignore();

    int id = depozit.getNextProdusId();
    std::cout << "  Nume: ";
    std::string nume; std::getline(std::cin, nume);
    std::cout << "  Cantitate: "; int cant; std::cin >> cant;
    std::cout << "  Pret (RON): "; double pret; std::cin >> pret;
    std::cout << "  Prag alerta: "; int prag; std::cin >> prag;
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
        std::string serie; std::getline(std::cin, serie);
        std::cout << "  Garantie (luni): "; int gar; std::cin >> gar;
        return new ProdusElectronic(id, nume, cant, pret, prag, loc, serie, gar);
    }
    if (tip == 4) {
        std::cout << "  ISBN (13 cifre): ";
        std::string isbn; std::getline(std::cin, isbn);
        std::cout << "  Autor: "; std::string autor; std::getline(std::cin, autor);
        std::cout << "  Editura: "; std::string editura; std::getline(std::cin, editura);
        std::cout << "  An publicare: "; int an; std::cin >> an;
        return new ProdusISBN(id, nume, cant, pret, prag, loc, isbn, autor, editura, an);
    }
    return new Produs(id, nume, cant, pret, prag, loc);
}

Sofer* UI::introduceSofer() {
    int id = depozit.getNextSoferId();
    std::cin.ignore();
    std::cout << "  Nume sofer: "; std::string n; std::getline(std::cin, n);
    std::cout << "  Nr. permis: "; std::string p; std::getline(std::cin, p);
    std::cout << "  Categorie (B/C/CE): "; std::string c; std::getline(std::cin, c);
    return new Sofer(id, n, p, c);
}

Vehicul* UI::introduceVehicul() {
    int id = depozit.getNextVehiculId();
    std::cin.ignore();
    std::cout << "  Nr. inmatriculare: "; std::string nr; std::getline(std::cin, nr);
    std::cout << "  Capacitate (kg): "; double cap; std::cin >> cap;
    std::cin.ignore();
    std::cout << "  Tip (camion/utilitara/furgoneta): "; std::string tip; std::getline(std::cin, tip);
    return new Vehicul(id, nr, cap, tip);
}

StaffDepozit* UI::introduceStaff() {
    int id = depozit.getNextStaffId();
    std::cin.ignore();
    std::cout << "  Nume: "; std::string n; std::getline(std::cin, n);
    std::cout << "  Rol (manager/operator/supervisor): "; std::string r; std::getline(std::cin, r);
    std::cout << "  Username: "; std::string u; std::getline(std::cin, u);
    return new StaffDepozit(id, n, r, u);
}

Furnizor* UI::introduceFurnizor() {
    int id = depozit.getNextFurnizorId();
    std::cin.ignore();
    std::cout << "  Nume furnizor: "; std::string n; std::getline(std::cin, n);
    std::cout << "  Contact (tel/email): "; std::string c; std::getline(std::cin, c);
    std::cout << "  Adresa: "; std::string a; std::getline(std::cin, a);
    return new Furnizor(id, n, c, a);
}

// ---- Meniu Produse ----------------------------------------------------------

void UI::menuProduse() {
    int opt;
    do {
        clearScreen();
        titlu("GESTIONARE PRODUSE");
        std::cout << "  [1] Vizualizare toate\n"
                  << "  [2] Cauta dupa ID\n"
                  << "  [3] Cauta dupa nume\n"
                  << "  [4] Cauta dupa ISBN\n"
                  << "  [5] Cauta dupa numar serie\n"
                  << "  [6] Cauta dupa locatie\n"
                  << "  [7] Adauga produs nou\n"
                  << "  [8] Elimina produs\n"
                  << "  [0] Inapoi\n\n  Alegere: ";
        std::cin >> opt;
        clearScreen();
        try {
            if (opt == 1) {
                titlu("TOATE PRODUSELE");
                depozit.afisareToate();
            } else if (opt == 2) {
                std::cout << "  ID produs: "; int id; std::cin >> id;
                titlu("REZULTAT");
                depozit.cautaDupaId(id)->afisare();
            } else if (opt == 3) {
                std::cin.ignore();
                std::cout << "  Termen cautare: "; std::string q; std::getline(std::cin, q);
                auto rez = depozit.cautaDupaNume(q);
                titlu("REZULTATE: " + q);
                if (rez.empty()) std::cout << "  Niciun rezultat.\n";
                else { depozit.afisareTabelHeader(); for (auto* p : rez) p->afisare(); }
            } else if (opt == 4) {
                std::cin.ignore();
                std::cout << "  ISBN (13 cifre): "; std::string isbn; std::getline(std::cin, isbn);
                titlu("REZULTAT ISBN");
                depozit.cautaDupaISBN(isbn)->afisare();
            } else if (opt == 5) {
                std::cin.ignore();
                std::cout << "  Numar serie: "; std::string serie; std::getline(std::cin, serie);
                titlu("REZULTAT SERIE");
                depozit.cautaDupaSerie(serie)->afisare();
            } else if (opt == 6) {
                std::cin.ignore();
                std::cout << "  Locatie (ex: A-03-12): "; std::string loc; std::getline(std::cin, loc);
                auto rez = depozit.cautaDupaLocatie(loc);
                titlu("PRODUSE IN LOCATIA: " + loc);
                if (rez.empty()) std::cout << "  Niciun produs in aceasta locatie.\n";
                else { depozit.afisareTabelHeader(); for (auto* p : rez) p->afisare(); }
            } else if (opt == 7) {
                titlu("ADAUGA PRODUS");
                Produs* p = introduceProdus();
                depozit.adaugaProdus(p);
                std::cout << "\n  Produs adaugat cu ID " << p->getId() << "\n";
                FileManager::salveazaStoc(depozit);
            } else if (opt == 8) {
                std::cout << "  ID produs de eliminat: "; int id; std::cin >> id;
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

// ---- Meniu Intrare ----------------------------------------------------------

void UI::menuIntrareMarfa() {
    clearScreen();
    titlu("INTRARE MARFA IN DEPOZIT");
    try {
        std::cout << "--- Selecteaza produs ---\n";
        int pid = alegereProdus();
        std::cout << "\n  Cantitate intrata: "; int cant; std::cin >> cant;
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

// ---- Meniu Iesire -----------------------------------------------------------

void UI::menuIesireMarfa() {
    clearScreen();
    titlu("IESIRE MARFA DIN DEPOZIT");
    try {
        std::cout << "  Mod iesire:\n"
                  << "  [1] Normal (dupa ID produs)\n"
                  << "  [2] FEFO (automat - primul care expira)\n"
                  << "  Alegere: ";
        int mod; std::cin >> mod;

        std::cout << "\n--- Selecteaza sofer ---\n";
        int sid = alegereSofer();
        std::cout << "\n--- Selecteaza vehicul ---\n";
        int vid = alegeVehicul();
        std::cin.ignore();
        std::cout << "\n  Destinatie (optional, Enter pt a sari): ";
        std::string dest; std::getline(std::cin, dest);

        if (mod == 1) {
            std::cout << "\n--- Selecteaza produs ---\n";
            int pid = alegereProdus();
            std::cout << "\n  Cantitate iesita: "; int cant; std::cin >> cant;
            depozit.iesireMarfa(pid, cant, sid, vid, dest);
        } else {
            std::cin.ignore();
            std::cout << "  Nume produs perisabil: "; std::string n; std::getline(std::cin, n);
            std::cout << "  Cantitate: "; int cant; std::cin >> cant;
            depozit.iesireMarfaFEFO(n, cant, sid, vid, dest);
        }
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
                  << "  [2] Produse expirate\n"
                  << "  [3] Produse care expira curand (7 zile)\n"
                  << "  [4] Recomandare recomandare (sortate dupa cantitate)\n"
                  << "  [5] Istoric tranzactii\n"
                  << "  [6] Test performanta (1 milion produse)\n"
                  << "  [0] Inapoi\n\n  Alegere: ";
        std::cin >> opt;
        clearScreen();

        if (opt == 1) {
            titlu("PRODUSE SUB PRAG");
            auto lista = depozit.getProduseSuBPrag();
            if (lista.empty()) std::cout << "  Toate produsele sunt peste prag.\n";
            else { depozit.afisareTabelHeader(); for (auto* p : lista) p->afisare();
                   std::cout << "\n  Total: " << lista.size() << " produse.\n"; }

        } else if (opt == 2) {
            titlu("PRODUSE EXPIRATE");
            auto lista = depozit.getProduseExpirate();
            if (lista.empty()) std::cout << "  Niciun produs expirat.\n";
            else { depozit.afisareTabelHeader(); for (auto* p : lista) p->afisare();
                   std::cout << "\n  Total: " << lista.size() << " produse expirate!\n"; }

        } else if (opt == 3) {
            std::cout << "  Expira in cate zile? ";
            int n; std::cin >> n;
            clearScreen();
            titlu("EXPIRA IN " + std::to_string(n) + " ZILE");
            auto lista = depozit.getProduseExpiraCurand(n);
            if (lista.empty()) std::cout << "  Niciun produs nu expira in acest interval.\n";
            else { depozit.afisareTabelHeader(); for (auto* p : lista) p->afisare(); }

        } else if (opt == 4) {
            titlu("RECOMANDARE RECOMANDARE");
            auto lista = depozit.getRecomandare();
            if (lista.empty()) std::cout << "  Niciun produs nu necesita recomandare.\n";
            else {
                std::cout << "  Produse sortate dupa cantitate ramasa (cele mai urgente primele):\n\n";
                depozit.afisareTabelHeader();
                for (auto* p : lista) p->afisare();
                std::cout << "\n  Total: " << lista.size() << " produse de reordonat.\n";
            }

        } else if (opt == 5) {
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
                          << std::setw(22) << "TIMESTAMP"
                          << "DESTINATIE\n"
                          << std::string(100, '-') << "\n";
                for (auto& t : ist)
                    std::cout << std::left
                              << std::setw(10) << t.tip
                              << std::setw(20) << t.soferNume
                              << std::setw(14) << t.vehiculNr
                              << std::setw(22) << t.produsNume
                              << std::setw(8)  << t.cantitate
                              << std::setw(22) << t.timestamp
                              << t.destinatie << "\n";
            }

        } else if (opt == 6) {
            titlu("TEST PERFORMANTA — 1 MILION PRODUSE");
            std::cout << "  Se genereaza 1.000.000 produse...\n";
            Depozit testD;
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 1; i <= 1000000; i++)
                testD.adaugaProdus(new Produs(i, "P_" + std::to_string(i), 100, 9.99, 10));
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  Inserare 1M produse : " << ms << " ms\n";

            start = std::chrono::high_resolution_clock::now();
            testD.cautaDupaId(500000);
            auto us = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  Cautare ID 500000   : " << us << " microsecunde (O(1))\n";
            std::cout << "  Total produse       : " << testD.getTotalProduse() << "\n";
        }
        if (opt != 0) pauza();
    } while (opt != 0);
}

// ---- Meniu Export -----------------------------------------------------------

void UI::menuExport() const {
    clearScreen();
    titlu("EXPORT DATE");
    time_t now = time(nullptr);
    char buf[12]; strftime(buf, sizeof(buf), "%Y%m%d", localtime(&now));
    std::string data(buf);
    FileManager::exportStoc(depozit, "data/stoc_" + data + ".csv");
    FileManager::exportIstoric(depozit, "data/tranzactii_" + data + ".csv");
    std::cout << "\n  Fisierele au fost salvate in folderul data/\n";
    pauza();
}

// ---- Meniu Soferi, Vehicule, Staff ------------------------------------------

void UI::menuSoferiVehicule() {
    int opt;
    do {
        clearScreen();
        titlu("SOFERI, VEHICULE SI STAFF");
        std::cout << "  [1] Lista soferi\n  [2] Adauga sofer\n"
                  << "  [3] Lista vehicule\n  [4] Adauga vehicul\n"
                  << "  [5] Lista staff\n  [6] Adauga staff\n"
                  << "  [0] Inapoi\n\n  Alegere: ";
        std::cin >> opt;
        clearScreen();
        try {
            if      (opt == 1) { titlu("SOFERI");   depozit.afisareSoferi(); }
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
            } else if (opt == 5) { titlu("STAFF DEPOZIT"); depozit.afisareStaff(); }
            else if (opt == 6) {
                titlu("ADAUGA STAFF");
                StaffDepozit* s = introduceStaff();
                depozit.adaugaStaff(s);
                std::cout << "\n  Staff adaugat cu ID " << s->getId() << "\n";
            }
        } catch (const DepozitException& e) {
            std::cout << "\n  EROARE: " << e.what() << "\n";
        }
        if (opt != 0) pauza();
    } while (opt != 0);
}

// ---- Meniu Furnizori --------------------------------------------------------

void UI::menuFurnizori() {
    int opt;
    do {
        clearScreen();
        titlu("FURNIZORI");
        std::cout << "  [1] Lista furnizori\n"
                  << "  [2] Adauga furnizor\n"
                  << "  [3] Asociaza produs la furnizor\n"
                  << "  [0] Inapoi\n\n  Alegere: ";
        std::cin >> opt;
        clearScreen();
        try {
            if (opt == 1) { titlu("FURNIZORI"); depozit.afisareFurnizori(); }
            else if (opt == 2) {
                titlu("ADAUGA FURNIZOR");
                Furnizor* f = introduceFurnizor();
                depozit.adaugaFurnizor(f);
                std::cout << "\n  Furnizor adaugat cu ID " << f->getId() << "\n";
            } else if (opt == 3) {
                depozit.afisareFurnizori();
                std::cout << "\n  ID furnizor: "; int fid; std::cin >> fid;
                std::cout << "  ID produs de asociat: "; int pid; std::cin >> pid;
                depozit.cautaDupaId(pid); // verifica ca exista
                depozit.cautaFurnizor(fid)->adaugaProdusAsociat(pid);
                std::cout << "  Produs asociat furnizorului.\n";
            }
        } catch (const DepozitException& e) {
            std::cout << "\n  EROARE: " << e.what() << "\n";
        }
        if (opt != 0) pauza();
    } while (opt != 0);
}

// ---- Run --------------------------------------------------------------------

void UI::run() {
    int opt;
    do {
        clearScreen();
        dashboard();
        std::cout << "  +------------------------------------+\n"
                  << "  |       DEPOZIT MANAGER v2.0         |\n"
                  << "  +------------------------------------+\n"
                  << "  |  [1] Gestionare produse            |\n"
                  << "  |  [2] Intrare marfa                 |\n"
                  << "  |  [3] Iesire marfa                  |\n"
                  << "  |  [4] Soferi, Vehicule, Staff       |\n"
                  << "  |  [5] Furnizori                     |\n"
                  << "  |  [6] Rapoarte                      |\n"
                  << "  |  [7] Export date                   |\n"
                  << "  |  [0] Iesire                        |\n"
                  << "  +------------------------------------+\n\n"
                  << "  Alegere: ";
        std::cin >> opt;

        if      (opt == 1) menuProduse();
        else if (opt == 2) menuIntrareMarfa();
        else if (opt == 3) menuIesireMarfa();
        else if (opt == 4) menuSoferiVehicule();
        else if (opt == 5) menuFurnizori();
        else if (opt == 6) menuRapoarte();
        else if (opt == 7) menuExport();
    } while (opt != 0);

    FileManager::salveazaStoc(depozit);
    FileManager::salveazaSoferiVehicule(depozit);
    std::cout << "\n  Date salvate. La revedere!\n";
}
