# Documentatie — Sistem de Monitorizare a Stocurilor unui Depozit
**Tema 3122B | POO C++ | Autor: crist**

---

## 1. Descrierea proiectului

Aplicatia simuleaza sistemul informatic al unui depozit real. Urmareste produsele
(stoc, locatie fizica, alerte), inregistreaza intrari si iesiri de marfa cu sofer
si vehicul, si genereaza rapoarte pentru personalul de depozit.

---

## 2. Ierarhia de clase (Diagrama UML)

```
Entitate
│  - id: int
│  + getId(): int
│
├── Produs
│   - nume, cantitate, pret, pragAlerta, locatie, dataUltimaModificare
│   + operator+=, operator-=
│   + subPrag(), afisare(), toCSV()
│   │
│   ├── ProdusPerisabil
│   │   - dataExpirare: string, temperaturaStorare: double
│   │   + esteExpirat(), expiraInNZile(n)
│   │
│   ├── ProdusElectronic
│   │   - serie: string, garantieLuni: int
│   │
│   └── ProdusISBN
│       - isbn, autor, editura, anPublicare
│
├── Persoana  [ABSTRACT]
│   - nume: string
│   + getRol(): string  [pur virtual]
│   │
│   ├── Sofer
│   │   - nrPermis, categorie (B/C/CE)
│   │   + getRol() = "sofer"
│   │
│   └── StaffDepozit
│       - rol (manager/operator), username
│       + getRol() = rol
│
├── Vehicul
│   - nrInmatriculare, capacitateKg, tip
│
├── Furnizor
│   - nume, contact, adresa
│   - produseIds: vector<int>
│   + adaugaProdusAsociat(id)
│
├── Tranzactie<T>  [TEMPLATE]
│   - soferId, vehiculId, items: vector<ItemTranzactie>, timestamp
│   + getTip(): "INTRARE" sau "IESIRE"
│   │
│   ├── Tranzactie<Intrare>  ->  getTip() = "INTRARE"
│   └── Tranzactie<Iesire>   ->  getTip() = "IESIRE"
│
└── Depozit
    - produse: unordered_map<int, Produs*>
    - soferi, staff, vehicule, furnizori: unordered_map<int, *>
    - istoricTranzactii: vector<EntryTranzactie>
```

### Relatii intre clase

```
Depozit "contine" --> Produs* (1..*)
Depozit "contine" --> Sofer* (0..*)
Depozit "contine" --> Vehicul* (0..*)
Depozit "contine" --> Furnizor* (0..*)
Depozit "contine" --> StaffDepozit* (0..*)
EntryTranzactie "refera" --> Sofer (prin soferId)
EntryTranzactie "refera" --> Vehicul (prin vehiculId)
EntryTranzactie "refera" --> Produs (prin produsId)
Furnizor "asociaza" --> Produs (prin produseIds: vector<int>)
```

---

## 3. Concepte POO folosite

### 3.1 Mostenire (Inheritance)

Ierarhia are doua ramuri principale:

**Ramura Produs:**
```cpp
class Produs : public Entitate { ... };
class ProdusPerisabil : public Produs { ... };
class ProdusElectronic : public Produs { ... };
class ProdusISBN : public Produs { ... };
```
`Depozit` stocheaza `Produs*` (pointer la baza) si apeleaza `afisare()` si
`toCSV()` virtual — astfel functioneaza corect indiferent de tipul real.

**Ramura Persoana:**
```cpp
class Persoana : public Entitate { virtual string getRol() = 0; };
class Sofer : public Persoana { string getRol() override { return "sofer"; } };
class StaffDepozit : public Persoana { string getRol() override { return rol; } };
```

### 3.2 Polimorfism (Polymorphism)

Metoda `afisare()` este virtuala in `Produs` si suprascriisa in fiecare subtip:
- `Produs::afisare()` — afiseaza campurile de baza
- `ProdusPerisabil::afisare()` — adauga data expirare si temperatura
- `ProdusElectronic::afisare()` — adauga serie si garantie
- `ProdusISBN::afisare()` — adauga ISBN si autor

La apelul `p->afisare()` (unde `p` este `Produs*`), se executa metoda tipului
real al obiectului — acesta este polimorfismul dinamic (vtable).

```cpp
for (auto& kv : produse)
    kv.second->afisare(); // apeleaza metoda corecta pt fiecare subtip
```

### 3.3 Encapsulare (Encapsulation)

Toti membrii claselor sunt `private` sau `protected`. Accesul extern se face
doar prin metode publice (`getNume()`, `getCantitate()`, etc.). Exemple:

```cpp
class Produs {
private:
    int cantitate; // nu poate fi modificat direct din afara
public:
    int getCantitate() const { return cantitate; }
    Produs& operator+=(int qty); // singura modalitate de modificare
};
```

Aceasta previne modificari accidentale ale cantitatii fara validare.

### 3.4 Clasa abstracta (Abstract Class)

`Persoana` este abstracta deoarece contine metoda pur virtuala `getRol()`.
Nu poate fi instantiata direct — trebuie mostenita si `getRol()` implementat:

```cpp
class Persoana : public Entitate {
public:
    virtual std::string getRol() const = 0; // pur virtual => clasa abstracta
};

// Corect: Sofer implementeaza getRol()
Sofer s(1, "Ion", "B-001", "C");

// Eroare de compilare: Persoana p(...) — nu se poate instantia
```

### 3.5 Template (Sabloane)

`Tranzactie<T>` este un sablon parametrizat cu tipul tranzactiei:

```cpp
template<typename T>
class Tranzactie : public Entitate {
public:
    std::string getTip() const;
};

// Specializari:
template<> inline std::string Tranzactie<Intrare>::getTip() { return "INTRARE"; }
template<> inline std::string Tranzactie<Iesire>::getTip()  { return "IESIRE";  }

// Utilizare in Depozit.cpp:
Tranzactie<Intrare> t1(0, soferId, vehiculId, timestamp);
Tranzactie<Iesire>  t2(0, soferId, vehiculId, timestamp);
```

Avantaj: compilatorul genereaza doua clase distincte, tipul e verificat la compilare.

### 3.6 Supraincarcarea operatorilor (Operator Overloading)

Operatorii `+=` si `-=` sunt supraincaracati pe `Produs` pentru a actualiza
cantitatea in mod expresiv si cu validare automata:

```cpp
Produs& Produs::operator+=(int qty) {
    if (qty < 0) throw DepozitException("Cantitate negativa");
    cantitate += qty;
    dataUltimaModificare = timestampAcum(); // se actualizeaza automat
    return *this;
}

Produs& Produs::operator-=(int qty) {
    if (qty > cantitate) throw CantitateInsuficientaException(id, qty, cantitate);
    cantitate -= qty;
    return *this;
}

// Utilizare:
*produs += 50;  // intrare marfa
*produs -= 20;  // iesire marfa
```

### 3.7 Gestionarea exceptiilor (Exceptions)

Ierarhie de exceptii personalizate derivate din `std::runtime_error`:

```
DepozitException (baza)
├── ProdusExistentException     — ID duplicat la adaugare
├── ProdusInexistentException   — ID inexistent la cautare/stergere
├── CantitateInsuficientaException — operator -= cu qty > stoc
└── EntitateInexistentaException — sofer/vehicul/furnizor inexistent
```

Toate operatiile care pot esua arunca exceptii in loc sa returneze coduri de eroare.
UI-ul prinde exceptiile si le afiseaza utilizatorului:

```cpp
try {
    depozit.iesireMarfa(pid, cant, sid, vid, dest);
} catch (const DepozitException& e) {
    cout << "EROARE: " << e.what() << "\n";
}
```

### 3.8 STL — Containere si algoritmi

- `unordered_map<int, Produs*>` — O(1) cautare dupa ID (vs. `map` O(log n))
- `vector<EntryTranzactie>` — istoric tranzactii in ordine cronologica
- `std::sort` cu lambda comparator — sortare produse dupa cantitate/data expirare
- `std::transform` — conversie string la lowercase pentru cautare case-insensitive
- `dynamic_cast<ProdusPerisabil*>` — RTTI pentru a filtra tipul corect din colectia

---

## 4. Algoritmi implementati

### FEFO (First Expired First Out)

La iesire marfa pentru perisabile, sistemul gaseste automat lotul cu data
expirare cea mai apropiata:

```cpp
// Sorteaza candidatii dupa data expirare (ISO string => comparatie lexicografica corecta)
std::sort(candidati.begin(), candidati.end(), [](ProdusPerisabil* a, ProdusPerisabil* b) {
    return a->getDataExpirare() < b->getDataExpirare();
});
iesireMarfa(candidati[0]->getId(), cantitate, soferId, vehiculId, destinatie);
```

Avantaj: previne expirarea stocului si risipa.

### Cautare O(1) la scara mare

`unordered_map` foloseste hash table intern:
- **Inserare:** O(1) amortizat
- **Cautare:** O(1) amortizat
- **Test verificat:** 1 milion produse inserate in ~500ms, cautare in <1 microsecunda

---

## 5. Persistenta datelor

La pornire, aplicatia incarca automat `data/stoc.csv`:
```
perisabil,1,Lapte UHT 1L,200,8.5,30,A-01-01,2026-06-01 10:00:00,2026-09-01,4.0
electronic,6,Laptop Dell 15,10,3299.99,2,C-01-01,2026-06-01 10:00:00,SN-DELL-001,24
isbn,8,C++ Primer,12,89.99,3,D-01-01,2026-06-01 10:00:00,9780321714114,Lippman,...
```

La inchidere (sau dupa fiecare operatie de modificare), datele se salveaza automat.
O functie factory (`produsFromCSV`) reconstruieste tipul corect dupa primul camp.

---

## 6. Posibile imbunatatiri

1. **Autentificare staff**: login cu username/parola din `StaffDepozit`, sesiuni cu rol
2. **Tranzactii multi-produs**: o singura livrare pentru mai multe produse odata
3. **Notificari automate**: email/SMS cand un produs ajunge sub prag
4. **Interfata grafica**: inlocuire consola cu GUI (Qt sau ncurses)
5. **Baza de date**: inlocuire CSV cu SQLite pentru interogari complexe
6. **Istoric versiuni stoc**: snapshot zilnic pentru analiza trendurilor
7. **Coduri de bare/QR**: identificare rapida produse la scanare
8. **Rapoarte PDF**: export documentatie cu librarie de generare PDF
9. **Multi-depozit**: suport pentru mai multe locatii fizice
10. **API REST**: expunere functionalitatii via HTTP pentru integrare cu alte sisteme
