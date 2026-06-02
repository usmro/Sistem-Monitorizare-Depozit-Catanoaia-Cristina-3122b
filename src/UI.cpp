#include "UI.h"
#include "Consola.h"
#include "FileManager.h"
#include "ProdusPerisabil.h"
#include "ProdusElectronic.h"
#include "ProdusISBN.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <conio.h>

// ---- Utilitare --------------------------------------------------------------

void UI::pauza() const {
    Consola::culoare(C::GRI_I);
    std::cout << "\n  Apasa orice tasta pentru a continua...";
    Consola::reset();
    _getch();
}

// ---- Afisare tabel colorat --------------------------------------------------

void UI::afisareProdus(Produs* p) const {
    auto* pp = dynamic_cast<ProdusPerisabil*>(p);
    if (pp && pp->esteExpirat())
        Consola::culoare(C::ROSU);
    else if (p->subPrag())
        Consola::culoare(C::GALBEN);
    else
        Consola::culoare(C::ALB);
    p->afisare();
    Consola::reset();
}

void UI::afisareTabelProduse(const std::vector<Produs*>& lista) const {
    if (lista.empty()) { Consola::scrieAtentie("Niciun produs de afisat."); return; }
    Consola::culoare(C::CYAN);
    depozit.afisareTabelHeader();
    Consola::reset();
    for (auto* p : lista) afisareProdus(p);
}

void UI::afisareTabelToate() const {
    if (depozit.getTotalProduse() == 0) {
        Consola::scrieAtentie("Depozitul este gol.");
        return;
    }
    Consola::culoare(C::CYAN);
    depozit.afisareTabelHeader();
    Consola::reset();
    for (auto& kv : depozit.getProduse()) afisareProdus(kv.second);
}

// ---- Dashboard --------------------------------------------------------------

void UI::dashboard() const {
    int    total    = depozit.getTotalProduse();
    int    subPrag  = depozit.getProduseSuBPragCount();
    int    expirate = depozit.getProduseExpirateCount();
    double valoare  = depozit.getValoareTotalaStoc();
    int    tranzAzi = depozit.getTranzactiiAziCount();
    int    tranzTot = depozit.getTranzactiiCount();

    Consola::titlu("DASHBOARD DEPOZIT");

    auto stat = [](const std::string& label, const std::string& valStr, WORD col) {
        Consola::culoare(C::GRI);
        std::cout << "  " << std::left << std::setw(28) << label << ": ";
        Consola::culoare(col);
        std::cout << valStr << "\n";
        Consola::reset();
    };

    stat("Total produse stocate",   std::to_string(total),    C::ALB);
    stat("Produse sub prag alerta", std::to_string(subPrag)  + (subPrag  > 0 ? " [!]" : ""),
         subPrag  > 0 ? C::GALBEN : C::VERDE);
    stat("Produse expirate",        std::to_string(expirate) + (expirate > 0 ? " [X]" : ""),
         expirate > 0 ? C::ROSU   : C::VERDE);
    stat("Tranzactii azi",          std::to_string(tranzAzi), C::CYAN);
    stat("Tranzactii total",        std::to_string(tranzTot), C::GRI);

    std::cout << "  " << std::left << std::setw(28) << "Valoare totala stoc" << ": ";
    Consola::culoare(C::GALBEN);
    std::cout << std::fixed << std::setprecision(2) << valoare << " RON\n";
    Consola::reset();

    Consola::linie();

    auto ui = depozit.getUltimaIntrare();
    auto ue = depozit.getUltimaIesire();
    if (ui) {
        std::cout << "  Ultima INTRARE : ";
        Consola::scrie(ui->produsNume + " x" + std::to_string(ui->cantitate), C::VERDE);
        Consola::culoare(C::GRI_I);
        std::cout << "  [" << ui->timestamp << "]\n";
        Consola::reset();
    }
    if (ue) {
        std::cout << "  Ultima IESIRE  : ";
        Consola::scrie(ue->produsNume + " x" + std::to_string(ue->cantitate), C::ROSU_I);
        Consola::culoare(C::GRI_I);
        std::cout << "  [" << ue->timestamp << "]";
        if (!ue->destinatie.empty()) std::cout << " -> " << ue->destinatie;
        std::cout << "\n";
        Consola::reset();
    }
    if (!ui && !ue) {
        Consola::culoare(C::GRI_I);
        std::cout << "  (nicio tranzactie)\n";
        Consola::reset();
    }
    std::cout << "\n";
}

// ---- Helpers introducere date -----------------------------------------------

int UI::alegereProdus() const {
    afisareTabelToate();
    return Consola::readInt("ID produs: ");
}

int UI::alegereSofer() const {
    depozit.afisareSoferi();
    return Consola::readInt("ID sofer: ");
}

int UI::alegeVehicul() const {
    depozit.afisareVehicule();
    return Consola::readInt("ID vehicul: ");
}

Produs* UI::introduceProdus() {
    std::vector<std::string> tipuri = {
        "[1] Generic", "[2] Perisabil", "[3] Electronic", "[4] ISBN (carte)"
    };
    Consola::scrieInfo("Selecteaza tipul produsului:");
    int tip = Consola::meniu(tipuri);
    std::cout << "\n";

    int id          = depozit.getNextProdusId();
    std::string n   = Consola::readString("Nume: ");
    int c           = Consola::readInt   ("Cantitate: ", 0);
    double p        = Consola::readDouble("Pret (RON): ", 0.0);
    int pr          = Consola::readInt   ("Prag alerta: ", 0);
    std::string loc = Consola::readString("Locatie (ex: A-03-12, Enter=N/A): ");
    if (loc.empty()) loc = "N/A";

    if (tip == 1) { // Perisabil
        std::string data = Consola::readString("Data expirare (YYYY-MM-DD): ");
        double temp      = Consola::readDouble("Temperatura stocare (C): ", -50.0, 200.0);
        return new ProdusPerisabil(id, n, c, p, pr, loc, data, temp);
    }
    if (tip == 2) { // Electronic
        std::string s = Consola::readString("Numar serie: ");
        int g         = Consola::readInt   ("Garantie (luni): ", 0);
        double v      = Consola::readDouble("Voltaj (V, ex: 220): ", 0.0, 600.0);
        return new ProdusElectronic(id, n, c, p, pr, loc, s, g, v);
    }
    if (tip == 3) { // ISBN
        std::string isbn = Consola::readString("ISBN (13 cifre): ");
        std::string a    = Consola::readString("Autor: ");
        std::string e    = Consola::readString("Editura: ");
        int an           = Consola::readInt   ("An publicare: ", 1000, 2100);
        return new ProdusISBN(id, n, c, p, pr, loc, isbn, a, e, an);
    }
    return new Produs(id, n, c, p, pr, loc); // tip == 0 → Generic
}

Sofer* UI::introduceSofer() {
    int id          = depozit.getNextSoferId();
    std::string n   = Consola::readString("Nume: ");
    std::string pm  = Consola::readString("Nr. permis: ");
    std::string cat = Consola::readString("Categorie (B/C/CE): ");
    return new Sofer(id, n, pm, cat);
}

Vehicul* UI::introduceVehicul() {
    int id          = depozit.getNextVehiculId();
    std::string nr  = Consola::readString("Nr. inmatriculare: ");
    double cap      = Consola::readDouble("Capacitate (kg): ", 0.0, 100000.0);
    std::string tip = Consola::readString("Tip (camion/utilitara/furgoneta): ");
    return new Vehicul(id, nr, cap, tip);
}

StaffDepozit* UI::introduceStaff() {
    int id        = depozit.getNextStaffId();
    std::string n = Consola::readString("Nume: ");
    std::string r = Consola::readString("Rol (manager/operator/supervisor): ");
    std::string u = Consola::readString("Username: ");
    return new StaffDepozit(id, n, r, u);
}

Furnizor* UI::introduceFurnizor() {
    int id        = depozit.getNextFurnizorId();
    std::string n = Consola::readString("Nume furnizor: ");
    std::string c = Consola::readString("Contact (tel/email): ");
    std::string a = Consola::readString("Adresa: ");
    return new Furnizor(id, n, c, a);
}

// ---- Meniu Produse ----------------------------------------------------------

void UI::menuProduse() {
    bool activ = true;
    while (activ) {
        Consola::curata();
        Consola::titlu("GESTIONARE PRODUSE");
        std::vector<std::string> opts = {
            "[1] Vizualizare toate produsele",
            "[2] Cautare rapida (ID/nume/ISBN/serie/locatie)",
            "[3] Cauta dupa nume",
            "[4] Cauta dupa ISBN",
            "[5] Cauta dupa serie electronica",
            "[6] Cauta dupa locatie",
            "[7] Adauga produs nou",
            "[8] Elimina produs",
            "[0] Inapoi"
        };
        int idx = Consola::meniu(opts);

        Consola::curata();
        try {
            if (idx == 0) {
                Consola::titlu("TOATE PRODUSELE");
                afisareTabelToate();
            } else if (idx == 1) {
                std::string q = Consola::readString("Cauta (ID / cuvant / ISBN / serie / locatie): ");
                auto rez = depozit.cauta(q);
                Consola::titlu("REZULTATE: \"" + q + "\"");
                afisareTabelProduse(rez);
                std::cout << "\n  ";
                Consola::scrie(std::to_string(rez.size()) + " rezultat(e)", C::CYAN);
                std::cout << "\n";
            } else if (idx == 2) {
                std::string q = Consola::readString("Termen cautare (partial): ");
                afisareTabelProduse(depozit.cautaDupaNume(q));
            } else if (idx == 3) {
                std::string isbn = Consola::readString("ISBN (13 cifre): ");
                Consola::titlu("REZULTAT ISBN");
                afisareTabelProduse({depozit.cautaDupaISBN(isbn)});
            } else if (idx == 4) {
                std::string serie = Consola::readString("Numar serie: ");
                afisareTabelProduse({depozit.cautaDupaSerie(serie)});
            } else if (idx == 5) {
                std::string loc = Consola::readString("Locatie (ex: A-03-12): ");
                afisareTabelProduse(depozit.cautaDupaLocatie(loc));
            } else if (idx == 6) {
                Consola::titlu("ADAUGA PRODUS");
                Produs* p = introduceProdus();
                depozit.adaugaProdus(p);
                Consola::sunetOK();
                Consola::scrieOK("Produs adaugat cu ID " + std::to_string(p->getId()));
                FileManager::salveazaStoc(depozit);
            } else if (idx == 7) {
                int id = Consola::readInt("ID produs de eliminat: ");
                if (Consola::confirma("Confirma stergerea produsului ID " +
                                      std::to_string(id) + "?")) {
                    depozit.eliminaProdus(id);
                    Consola::sunetOK();
                    Consola::scrieOK("Produs eliminat.");
                    FileManager::salveazaStoc(depozit);
                } else {
                    Consola::scrieInfo("Operatiune anulata.");
                }
            } else {
                activ = false; continue;
            }
        } catch (const DepozitException& e) {
            Consola::sunetEroare();
            Consola::scrieEroare(e.what());
        }
        pauza();
    }
}

// ---- Meniu Intrare ----------------------------------------------------------

void UI::menuIntrareMarfa() {
    Consola::curata();
    Consola::titlu("INTRARE MARFA IN DEPOZIT");
    try {
        Consola::scrieInfo("--- Selecteaza produs ---");
        std::cout << "\n";
        int pid  = alegereProdus();
        int cant = Consola::readInt("Cantitate intrata: ", 1);

        std::cout << "\n";
        Consola::scrieInfo("--- Selecteaza sofer ---");
        std::cout << "\n";
        int sid = alegereSofer();

        std::cout << "\n";
        Consola::scrieInfo("--- Selecteaza vehicul ---");
        std::cout << "\n";
        int vid = alegeVehicul();

        depozit.intrareMarfa(pid, cant, sid, vid);
        Consola::sunetOK();
        Consola::scrieOK("Intrare inregistrata cu succes.");
        FileManager::salveazaStoc(depozit);
    } catch (const DepozitException& e) {
        Consola::sunetEroare();
        Consola::scrieEroare(e.what());
    }
    pauza();
}

// ---- Meniu Iesire -----------------------------------------------------------

void UI::menuIesireMarfa() {
    Consola::curata();
    Consola::titlu("IESIRE MARFA DIN DEPOZIT");
    try {
        std::vector<std::string> moduri = {
            "[1] Normal (alege dupa ID produs)",
            "[2] FEFO (automat - primul care expira)"
        };
        Consola::scrieInfo("Mod iesire:");
        std::cout << "\n";
        int mod = Consola::meniu(moduri);

        std::cout << "\n";
        Consola::scrieInfo("--- Selecteaza sofer ---");
        std::cout << "\n";
        int sid = alegereSofer();

        std::cout << "\n";
        Consola::scrieInfo("--- Selecteaza vehicul ---");
        std::cout << "\n";
        int vid = alegeVehicul();

        std::string dest = Consola::readString("Destinatie (Enter pt a sari): ");

        if (mod == 0) { // Normal
            std::cout << "\n";
            Consola::scrieInfo("--- Selecteaza produs ---");
            std::cout << "\n";
            int pid  = alegereProdus();
            int cant = Consola::readInt("Cantitate iesita: ", 1);
            depozit.iesireMarfa(pid, cant, sid, vid, dest);
        } else { // FEFO
            std::string n = Consola::readString("Nume produs perisabil: ");
            int cant      = Consola::readInt   ("Cantitate: ", 1);
            depozit.iesireMarfaFEFO(n, cant, sid, vid, dest);
        }
        Consola::sunetOK();
        Consola::scrieOK("Iesire inregistrata cu succes.");
        FileManager::salveazaStoc(depozit);
    } catch (const DepozitException& e) {
        Consola::sunetEroare();
        Consola::scrieEroare(e.what());
    }
    pauza();
}

// ---- Meniu Rapoarte ---------------------------------------------------------

void UI::menuRapoarte() const {
    bool activ = true;
    while (activ) {
        Consola::curata();
        Consola::titlu("RAPOARTE");
        std::vector<std::string> opts = {
            "[1] Produse sub prag de alerta",
            "[2] Produse expirate",
            "[3] Produse care expira curand",
            "[4] Recomandare reaprovizionare",
            "[5] Istoric tranzactii",
            "[6] Test performanta (1 milion produse)",
            "[0] Inapoi"
        };
        int idx = Consola::meniu(opts);
        Consola::curata();

        if (idx == 0) {
            Consola::titlu("PRODUSE SUB PRAG");
            auto lista = depozit.getProduseSuBPrag();
            if (lista.empty()) Consola::scrieOK("Toate produsele sunt peste prag.");
            else {
                afisareTabelProduse(lista);
                std::cout << "\n  ";
                Consola::scrie(std::to_string(lista.size()) + " produse sub prag.", C::GALBEN);
                std::cout << "\n";
            }
        } else if (idx == 1) {
            Consola::titlu("PRODUSE EXPIRATE");
            auto lista = depozit.getProduseExpirate();
            if (lista.empty()) Consola::scrieOK("Niciun produs expirat.");
            else {
                afisareTabelProduse(lista);
                std::cout << "\n  ";
                Consola::scrie(std::to_string(lista.size()) + " produse expirate!", C::ROSU);
                std::cout << "\n";
            }
        } else if (idx == 2) {
            int n = Consola::readInt("Expira in cate zile? ", 1, 3650);
            Consola::curata();
            Consola::titlu("EXPIRA IN " + std::to_string(n) + " ZILE");
            afisareTabelProduse(depozit.getProduseExpiraCurand(n));
        } else if (idx == 3) {
            Consola::titlu("RECOMANDARE REAPROVIZIONARE");
            auto lista = depozit.getRecomandare();
            if (lista.empty()) Consola::scrieOK("Niciun produs nu necesita reaprovizionare.");
            else {
                Consola::scrieAtentie("Sortate dupa cantitate ramasa (cele mai urgente primele):\n");
                afisareTabelProduse(lista);
            }
        } else if (idx == 4) {
            Consola::titlu("ISTORIC TRANZACTII");
            const auto& ist = depozit.getIstoric();
            if (ist.empty()) {
                Consola::scrieInfo("Nicio tranzactie inregistrata.");
            } else {
                Consola::culoare(C::CYAN);
                std::cout << std::left
                          << std::setw(10) << "TIP"
                          << std::setw(20) << "SOFER"
                          << std::setw(14) << "VEHICUL"
                          << std::setw(22) << "PRODUS"
                          << std::setw(8)  << "CANT."
                          << std::setw(22) << "TIMESTAMP"
                          << "DESTINATIE\n";
                Consola::linie(100);
                for (auto& t : ist) {
                    Consola::culoare(t.tip == "INTRARE" ? C::VERDE : C::ROSU_I);
                    std::cout << std::left << std::setw(10) << t.tip;
                    Consola::reset();
                    std::cout << std::left
                              << std::setw(20) << t.soferNume
                              << std::setw(14) << t.vehiculNr
                              << std::setw(22) << t.produsNume
                              << std::setw(8)  << t.cantitate
                              << std::setw(22) << t.timestamp
                              << t.destinatie << "\n";
                }
            }
        } else if (idx == 5) {
            Consola::titlu("TEST PERFORMANTA - 1 MILION PRODUSE");
            Consola::scrieAtentie("Se genereaza 1.000.000 produse...");
            std::cout << "\n";
            Depozit testD;
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 1; i <= 1000000; i++)
                testD.adaugaProdus(new Produs(i, "P_" + std::to_string(i), 100, 9.99, 10));
            long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start).count();

            std::cout << "  Inserare 1M produse : ";
            Consola::scrieLn(std::to_string(ms) + " ms", C::GALBEN);

            start = std::chrono::high_resolution_clock::now();
            testD.cautaDupaId(500000);
            long long us = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  Cautare ID 500000   : ";
            Consola::scrieLn(std::to_string(us) + " microsecunde (O(1))", C::VERDE);
            // testD iese din scope → destructor elibereaza toate cele 1M obiecte
        } else {
            activ = false; continue;
        }
        pauza();
    }
}

// ---- Meniu Export -----------------------------------------------------------

void UI::menuExport() const {
    Consola::curata();
    Consola::titlu("EXPORT DATE");
    time_t now = time(nullptr);
    char buf[12];
    struct tm tmbuf;
    localtime_s(&tmbuf, &now);
    strftime(buf, sizeof(buf), "%Y%m%d", &tmbuf);
    std::string data(buf);
    FileManager::exportStoc(depozit, "data/stoc_" + data + ".csv");
    FileManager::exportIstoric(depozit, "data/tranzactii_" + data + ".csv");
    Consola::sunetOK();
    Consola::scrieOK("Fisierele au fost salvate in folderul data/");
    pauza();
}

// ---- Meniu Soferi / Vehicule / Staff ----------------------------------------

void UI::menuSoferiVehicule() {
    bool activ = true;
    while (activ) {
        Consola::curata();
        Consola::titlu("SOFERI, VEHICULE SI STAFF");
        std::vector<std::string> opts = {
            "[1] Lista soferi",
            "[2] Adauga sofer",
            "[3] Lista vehicule",
            "[4] Adauga vehicul",
            "[5] Lista staff depozit",
            "[6] Adauga staff",
            "[0] Inapoi"
        };
        int idx = Consola::meniu(opts);
        Consola::curata();
        try {
            if      (idx == 0) { Consola::titlu("SOFERI");       depozit.afisareSoferi(); }
            else if (idx == 1) {
                Consola::titlu("ADAUGA SOFER");
                Sofer* s = introduceSofer();
                depozit.adaugaSofer(s);
                Consola::sunetOK();
                Consola::scrieOK("Sofer adaugat cu ID " + std::to_string(s->getId()));
                FileManager::salveazaSoferiVehicule(depozit);
            }
            else if (idx == 2) { Consola::titlu("VEHICULE");     depozit.afisareVehicule(); }
            else if (idx == 3) {
                Consola::titlu("ADAUGA VEHICUL");
                Vehicul* v = introduceVehicul();
                depozit.adaugaVehicul(v);
                Consola::sunetOK();
                Consola::scrieOK("Vehicul adaugat cu ID " + std::to_string(v->getId()));
                FileManager::salveazaSoferiVehicule(depozit);
            }
            else if (idx == 4) { Consola::titlu("STAFF DEPOZIT"); depozit.afisareStaff(); }
            else if (idx == 5) {
                Consola::titlu("ADAUGA STAFF");
                StaffDepozit* s = introduceStaff();
                depozit.adaugaStaff(s);
                Consola::sunetOK();
                Consola::scrieOK("Staff adaugat cu ID " + std::to_string(s->getId()));
            }
            else { activ = false; continue; }
        } catch (const DepozitException& e) {
            Consola::sunetEroare();
            Consola::scrieEroare(e.what());
        }
        pauza();
    }
}

// ---- Meniu Furnizori --------------------------------------------------------

void UI::menuFurnizori() {
    bool activ = true;
    while (activ) {
        Consola::curata();
        Consola::titlu("FURNIZORI");
        std::vector<std::string> opts = {
            "[1] Lista furnizori",
            "[2] Adauga furnizor",
            "[3] Asociaza produs la furnizor",
            "[0] Inapoi"
        };
        int idx = Consola::meniu(opts);
        Consola::curata();
        try {
            if (idx == 0) {
                Consola::titlu("FURNIZORI");
                depozit.afisareFurnizori();
            } else if (idx == 1) {
                Consola::titlu("ADAUGA FURNIZOR");
                Furnizor* f = introduceFurnizor();
                depozit.adaugaFurnizor(f);
                Consola::sunetOK();
                Consola::scrieOK("Furnizor adaugat cu ID " + std::to_string(f->getId()));
            } else if (idx == 2) {
                depozit.afisareFurnizori();
                int fid = Consola::readInt("ID furnizor: ");
                int pid = Consola::readInt("ID produs: ");
                depozit.cautaDupaId(pid); // valideaza existenta produsului
                depozit.cautaFurnizor(fid)->adaugaProdusAsociat(pid);
                Consola::sunetOK();
                Consola::scrieOK("Produs asociat furnizorului.");
            } else {
                activ = false; continue;
            }
        } catch (const DepozitException& e) {
            Consola::sunetEroare();
            Consola::scrieEroare(e.what());
        }
        pauza();
    }
}

// ---- Run --------------------------------------------------------------------

void UI::run() {
    Consola::sunetStart();

    std::vector<std::string> optsMain = {
        "[1] Gestionare produse",
        "[2] Intrare marfa",
        "[3] Iesire marfa",
        "[4] Soferi, Vehicule, Staff",
        "[5] Furnizori",
        "[6] Rapoarte",
        "[7] Export date",
        "[0] Iesire"
    };

    bool activ = true;
    while (activ) {
        Consola::curata();
        dashboard();
        Consola::titlu("DEPOZIT MANAGER v2.0");

        int idx = Consola::meniu(optsMain);

        switch (idx) {
            case 0: menuProduse();        break;
            case 1: menuIntrareMarfa();   break;
            case 2: menuIesireMarfa();    break;
            case 3: menuSoferiVehicule(); break;
            case 4: menuFurnizori();      break;
            case 5: menuRapoarte();       break;
            case 6: menuExport();         break;
            case 7: activ = false;        break;
        }
    }

    FileManager::salveazaStoc(depozit);
    FileManager::salveazaSoferiVehicule(depozit);
    Consola::curata();
    Consola::scrieOK("Date salvate. La revedere!");
    std::cout << "\n";
}
