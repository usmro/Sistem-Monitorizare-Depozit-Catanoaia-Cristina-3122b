# Sistem de Monitorizare a Stocurilor unui Depozit
**Tema 3122B — Programare Orientata pe Obiecte (C++)**

---

## Cerinte de sistem

- Compilator: `g++` cu suport C++17
- Pe Windows: compilatorul din **CodeBlocks MinGW** (`C:\Program Files\CodeBlocks\MinGW\bin\g++.exe`)
- Pe Linux/Mac: `g++` instalat din package manager

---

## Compilare si rulare

### Varianta rapida (Windows cu CodeBlocks MinGW)
```
make
depozit.exe
```

### Comanda directa (Windows)
```
"C:\Program Files\CodeBlocks\MinGW\bin\g++.exe" -std=c++17 main.cpp src/Produs.cpp src/ProdusPerisabil.cpp src/ProdusElectronic.cpp src/ProdusISBN.cpp src/Sofer.cpp src/StaffDepozit.cpp src/Vehicul.cpp src/Furnizor.cpp src/Depozit.cpp src/UI.cpp src/FileManager.cpp -o depozit.exe
depozit.exe
```

### Comanda directa (Linux/Mac)
```
g++ -std=c++17 main.cpp src/*.cpp -o depozit
./depozit
```

### Rulare teste unitare
```
make test
```

---

## Structura proiect

```
/
├── main.cpp              — entry point, date demo la primul start
├── Makefile              — build system
├── src/
│   ├── Entitate.h        — clasa de baza (id)
│   ├── Exceptions.h      — exceptii custom
│   ├── Persoana.h        — clasa abstracta (mostenita de Sofer, StaffDepozit)
│   ├── Produs.h/.cpp     — produs de baza + operatori += / -=
│   ├── ProdusPerisabil   — data expirare, FEFO, verificare expirare
│   ├── ProdusElectronic  — serie, garantie
│   ├── ProdusISBN        — isbn, autor, editura
│   ├── Sofer.h/.cpp      — sofer (mosteneste Persoana)
│   ├── StaffDepozit      — angajat depozit (mosteneste Persoana)
│   ├── Vehicul.h/.cpp    — vehicul de transport
│   ├── Furnizor.h/.cpp   — furnizor cu lista produse asociate
│   ├── Tranzactie.h      — template<Intrare/Iesire>
│   ├── Depozit.h/.cpp    — gestiunea centrala (unordered_map O(1))
│   ├── UI.h/.cpp         — interfata consola meniuri
│   └── FileManager       — save/load CSV, export rapoarte
├── data/                 — fisiere CSV generate automat
├── tests/
│   └── test_depozit.cpp  — 27 teste unitare
└── docs/
    └── documentatie.md   — documentatie completa
```

---

## Functionalitati principale

| Functionalitate | Descriere |
|---|---|
| Tipuri produse | Generic, Perisabil, Electronic, ISBN |
| Intrare marfa | Inregistreaza cu sofer + vehicul + timestamp |
| Iesire marfa | Normal (dupa ID) sau **FEFO** (primul care expira) |
| Alerte | Produse sub prag, produse expirate, expira curand |
| Recomandare | Lista produse de reordonat sortata dupa cantitate |
| Cautare | Dupa ID, nume, ISBN, serie electronica, locatie fizica |
| Dashboard | Statistici live la fiecare deschidere a meniului |
| Persistenta | Save/load automat CSV la pornire si inchidere |
| Export | Stoc si istoric tranzactii cu data ultima miscare |
| Performanta | Test cu 1 milion produse, cautare O(1) |

---

## Exemple de rulare

```
[1] Gestionare produse  -> vizualizare, cautare, adaugare, eliminare
[2] Intrare marfa       -> selectezi produs + cantitate + sofer + vehicul
[3] Iesire marfa        -> mod normal sau FEFO pentru perisabile
[4] Soferi/Vehicule     -> management personal si flota
[5] Furnizori           -> inregistrare si asociere produse
[6] Rapoarte            -> expirate, sub prag, recomandare, istoricul
[7] Export              -> genereaza CSV in data/
```

---

## Git

- Branch principal: `main`
- Branch de lucru: `develop`
- Repository classroom: https://classroom.github.com/classrooms/86118538-usmro-poo
