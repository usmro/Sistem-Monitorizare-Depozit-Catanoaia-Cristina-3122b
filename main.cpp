#include <iostream>
#include <map>
#include <string>

using namespace std;

class Entitate {
public:
    int id;

    Entitate(int idNou = 0) {
        id = idNou;
    }
};

class Produs : public Entitate {
public:
    string nume;
    int cantitate;
    double pret;

    Produs() : Entitate(0) {
        nume = "";
        cantitate = 0;
        pret = 0;
    }

    Produs(int idNou, string numeNou, int cantitateNoua, double pretNou)
        : Entitate(idNou) {
        nume = numeNou;
        cantitate = cantitateNoua;
        pret = pretNou;
    }
};

class Depozit {
private:
    map<int, Produs> produse;

public:
    void adaugaProdus(Produs p) {
        if (produse.find(p.id) != produse.end()) {
            cout << "Produsul cu ID " << p.id << " exista deja.\n";
            return;
        }
        produse[p.id] = p;
        cout << "Produs adaugat: " << p.nume << "\n";
    }

    void eliminaProdus(int id) {
        if (produse.find(id) == produse.end()) {
            cout << "Nu exista produs cu ID " << id << "\n";
            return;
        }
        produse.erase(id);
        cout << "Produs eliminat cu ID " << id << "\n";
    }

    void afisareToate() {
        cout << "\nToate produsele:\n";
        if (produse.empty()) {
            cout << "Depozitul este gol.\n";
            return;
        }

        map<int, Produs>::iterator it;
        for (it = produse.begin(); it != produse.end(); ++it) {
            cout << "ID " << it->second.id
                 << " nume" << it->second.nume
                 << " cantitate " << it->second.cantitate
                 << " pret " << it->second.pret << "\n";
        }
    }
};

int main() {
    Depozit d;

    d.adaugaProdus(Produs(1, "Lapte", 20, 8.5));
    d.adaugaProdus(Produs(2, "Paine", 5, 4));
    d.adaugaProdus(Produs(3, "Orez", 40, 12));

    d.afisareToate();

    d.eliminaProdus(2);

    d.afisareToate();

    d.eliminaProdus(99);

    return 0;
}
