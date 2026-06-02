#include "../src/Depozit.h"
#include "../src/ProdusPerisabil.h"
#include "../src/ProdusElectronic.h"
#include "../src/ProdusISBN.h"
#include "../src/Exceptions.h"
#include <iostream>
#include <cassert>
#include <chrono>

int passed = 0, failed = 0;

#define TEST(name, expr) \
    do { \
        if (expr) { std::cout << "  [OK] " << name << "\n"; passed++; } \
        else      { std::cout << "  [FAIL] " << name << "\n"; failed++; } \
    } while(0)

#define TEST_THROWS(name, expr) \
    do { \
        bool threw = false; \
        try { expr; } catch (...) { threw = true; } \
        TEST(name, threw); \
    } while(0)

void testOperatori() {
    std::cout << "\n--- Operatori += / -= ---\n";
    Produs p(1, "Test", 50, 10.0, 5);
    p += 30;
    TEST("operator+= creste cantitatea", p.getCantitate() == 80);
    p -= 20;
    TEST("operator-= scade cantitatea", p.getCantitate() == 60);
    TEST_THROWS("operator-= arunca exceptie la insuficient", p -= 9999);
    TEST_THROWS("operator+= arunca exceptie la negativ", p += -1);
}

void testSubPrag() {
    std::cout << "\n--- Sub prag de alerta ---\n";
    Produs p(1, "Test", 5, 10.0, 10);
    TEST("produs cu cant <= prag e sub prag", p.subPrag());
    p += 10;
    TEST("produs cu cant > prag nu e sub prag", !p.subPrag());
}

void testTipuriProduse() {
    std::cout << "\n--- Tipuri de produse ---\n";
    ProdusPerisabil pp(1, "Lapte", 10, 8.5, 5, "A-01", "2026-12-31", 4.0);
    TEST("ProdusPerisabil tip = 'perisabil'", pp.getTip() == "perisabil");
    TEST("ProdusPerisabil data expirare",     pp.getDataExpirare() == "2026-12-31");

    ProdusElectronic pe(2, "Laptop", 2, 3000, 1, "C-01", "SN-001", 24);
    TEST("ProdusElectronic tip = 'electronic'", pe.getTip() == "electronic");
    TEST("ProdusElectronic garantie",           pe.getGarantieLuni() == 24);

    ProdusISBN pi(3, "C++ Primer", 5, 90, 2, "D-01",
                  "9780321714114", "Lippman", "Addison", 2012);
    TEST("ProdusISBN tip = 'isbn'",    pi.getTip() == "isbn");
    TEST("ProdusISBN isbn corect",     pi.getIsbn() == "9780321714114");
}

void testDepozit() {
    std::cout << "\n--- Depozit: adauga / elimina / cauta ---\n";
    Depozit d;
    d.adaugaProdus(new Produs(1, "Orez", 100, 5.0, 10));
    d.adaugaProdus(new Produs(2, "Faina", 50, 4.0, 8));

    TEST("getTotalProduse dupa 2 inserari", d.getTotalProduse() == 2);
    TEST("cautaDupaId gaseste produsul",    d.cautaDupaId(1)->getNume() == "Orez");

    TEST_THROWS("adauga duplicat arunca exceptie", d.adaugaProdus(new Produs(1, "X", 1, 1, 1)));
    TEST_THROWS("cautaDupaId inexistent arunca",   d.cautaDupaId(999));

    d.eliminaProdus(1);
    TEST("eliminaProdus scade totalul",    d.getTotalProduse() == 1);
    TEST_THROWS("elimina inexistent arunca exceptie", d.eliminaProdus(1));
}

void testCautareDupaNume() {
    std::cout << "\n--- Cautare dupa nume ---\n";
    Depozit d;
    d.adaugaProdus(new Produs(1, "Lapte UHT", 10, 8.5, 5));
    d.adaugaProdus(new Produs(2, "Lapte Proaspat", 5, 7.0, 3));
    d.adaugaProdus(new Produs(3, "Orez", 100, 5.0, 10));

    auto rez = d.cautaDupaNume("lapte");
    TEST("cautaDupaNume gaseste 2 produse cu 'lapte'", rez.size() == 2);

    auto rez2 = d.cautaDupaNume("xyz_inexistent");
    TEST("cautaDupaNume returneaza gol cand nu gaseste", rez2.empty());
}

void testIntrareIesire() {
    std::cout << "\n--- Intrare / Iesire marfa ---\n";
    Depozit d;
    d.adaugaProdus(new Produs(1, "Orez", 50, 5.0, 10));
    d.adaugaSofer  (new Sofer  (1, "Ion", "B-001", "C"));
    d.adaugaVehicul(new Vehicul(1, "B-01-ABC", 5000, "camion"));

    d.intrareMarfa(1, 100, 1, 1);
    TEST("intrare creste cantitatea la 150", d.cautaDupaId(1)->getCantitate() == 150);
    TEST("intrare inregistrata in istoric",  d.getTranzactiiCount() == 1);

    d.iesireMarfa(1, 30, 1, 1);
    TEST("iesire scade cantitatea la 120",   d.cautaDupaId(1)->getCantitate() == 120);
    TEST("iesire inregistrata in istoric",   d.getTranzactiiCount() == 2);

    TEST_THROWS("iesire depaseste stocul arunca exceptie", d.iesireMarfa(1, 99999, 1, 1));
}

void testPerformanta() {
    std::cout << "\n--- Test performanta: 1 milion produse ---\n";
    Depozit d;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= 1000000; i++)
        d.adaugaProdus(new Produs(i, "P_" + std::to_string(i), 10, 1.0, 2));
    auto end = std::chrono::high_resolution_clock::now();
    long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  Inserare 1M produse: " << ms << " ms\n";
    TEST("1M produse stocate corect", d.getTotalProduse() == 1000000);

    start = std::chrono::high_resolution_clock::now();
    Produs* p = d.cautaDupaId(500000);
    end = std::chrono::high_resolution_clock::now();
    long long us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "  Cautare in 1M produse: " << us << " microsecunde\n";
    TEST("cautare O(1) gaseste produsul", p != nullptr && p->getId() == 500000);
}

int main() {
    std::cout << "=========================================\n";
    std::cout << "   TESTE UNITARE — Sistem Depozit\n";
    std::cout << "=========================================\n";

    testOperatori();
    testSubPrag();
    testTipuriProduse();
    testDepozit();
    testCautareDupaNume();
    testIntrareIesire();
    testPerformanta();

    std::cout << "\n=========================================\n";
    std::cout << "  Rezultat: " << passed << " OK, " << failed << " FAIL\n";
    std::cout << "=========================================\n";

    return (failed == 0) ? 0 : 1;
}
