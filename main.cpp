#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

class LocOcupatException : public std::runtime_error {
public:
    explicit LocOcupatException(const std::string& mesaj)
        : std::runtime_error(mesaj) {}
};

class IndexInvalidException : public std::runtime_error {
public:
    explicit IndexInvalidException(const std::string& mesaj)
        : std::runtime_error(mesaj) {}
};

class Entitate {
protected:
    int id;

public:
    explicit Entitate(int idNou) : id(idNou) {}

    int getId() const {
        return id;
    }
};

class Produs : public Entitate {
private:
    std::string nume;
    int cantitate;
    double pret;
    int pragAlerta;

public:
    Produs(int id, const std::string& nume, int cantitate, double pret, int pragAlerta)
        : Entitate(id), nume(nume), cantitate(cantitate), pret(pret), pragAlerta(pragAlerta) {
        if (cantitate < 0) {
            throw std::invalid_argument("Cantitatea initiala nu poate fi negativa.");
        }
        if (pret < 0) {
            throw std::invalid_argument("Pretul nu poate fi negativ.");
        }
        if (pragAlerta < 0) {
            throw std::invalid_argument("Pragul de alerta nu poate fi negativ.");
        }
    }

    const std::string& getNume() const {
        return nume;
    }

    int getCantitate() const {
        return cantitate;
    }

    double getPret() const {
        return pret;
    }

    int getPragAlerta() const {
        return pragAlerta;
    }

    bool esteSubPrag() const {
        return cantitate < pragAlerta;
    }

    Produs& operator+=(int valoare) {
        if (valoare < 0) {
            throw std::invalid_argument("La operator += valoarea trebuie sa fie pozitiva.");
        }
        cantitate += valoare;
        return *this;
    }

    Produs& operator-=(int valoare) {
        if (valoare < 0) {
            throw std::invalid_argument("La operator -= valoarea trebuie sa fie pozitiva.");
        }
        if (cantitate - valoare < 0) {
            throw std::invalid_argument("Cantitate insuficienta pentru vanzare.");
        }
        cantitate -= valoare;
        return *this;
    }
};

class Depozit {
private:
    std::map<int, Produs> produse;

public:
    void adaugaProdus(const Produs& p) {
        if (produse.find(p.getId()) != produse.end()) {
            throw LocOcupatException("Exista deja un produs cu acest ID.");
        }
        produse.insert({p.getId(), p});
    }

    void eliminaProdus(int id) {
        auto it = produse.find(id);
        if (it == produse.end()) {
            throw IndexInvalidException("Nu exista produs cu acest ID pentru eliminare.");
        }
        produse.erase(it);
    }

    void actualizeazaCantitate(int id, int delta) {
        auto it = produse.find(id);
        if (it == produse.end()) {
            throw IndexInvalidException("Nu exista produs cu acest ID pentru actualizare.");
        }

        if (delta >= 0) {
            it->second += delta;
        } else {
            it->second -= (-delta);
        }
    }

    std::vector<Produs> raportSubPrag() const {
        std::vector<Produs> rezultat;
        for (const auto& pereche : produse) {
            if (pereche.second.esteSubPrag()) {
                rezultat.push_back(pereche.second);
            }
        }
        return rezultat;
    }

    void afiseazaToate() const {
        std::cout << "\n=== STOC CURENT ===\n";
        if (produse.empty()) {
            std::cout << "Depozitul este gol.\n";
            return;
        }

        for (const auto& pereche : produse) {
            const Produs& p = pereche.second;
            std::cout << "ID: " << p.getId()
                      << " | Nume: " << p.getNume()
                      << " | Cantitate: " << p.getCantitate()
                      << " | Pret: " << p.getPret()
                      << " | Prag: " << p.getPragAlerta() << "\n";
        }
    }
};

int main() {
    Depozit depozit;

    try {
        Produs p1(1, "Lapte", 20, 8.5, 10);
        Produs p2(2, "Paine", 5, 4.0, 7);
        Produs p3(3, "Orez", 40, 12.0, 15);

        depozit.adaugaProdus(p1);
        depozit.adaugaProdus(p2);
        depozit.adaugaProdus(p3);

        depozit.afiseazaToate();

        std::cout << "\nActualizare cantitati...\n";
        depozit.actualizeazaCantitate(2, +10); // restock
        depozit.actualizeazaCantitate(1, -15); // vanzare

        depozit.afiseazaToate();

        std::cout << "\n=== RAPORT PRODUSE SUB PRAG ===\n";
        std::vector<Produs> subPrag = depozit.raportSubPrag();
        if (subPrag.empty()) {
            std::cout << "Nu exista produse sub prag.\n";
        } else {
            for (const Produs& p : subPrag) {
                std::cout << "ID " << p.getId() << " - " << p.getNume()
                          << " (cantitate " << p.getCantitate()
                          << ", prag " << p.getPragAlerta() << ")\n";
            }
        }

        // Exemple de erori comune tratate prin exceptii:
        // depozit.adaugaProdus(Produs(1, "Alt produs", 10, 2.5, 3)); // ID duplicat
        // depozit.actualizeazaCantitate(99, -2); // ID inexistent

    } catch (const std::exception& ex) {
        std::cout << "Eroare: " << ex.what() << "\n";
    }

    return 0;
}
