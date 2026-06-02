# POO Project Instructions — Tema 3122B
## Sistem de Monitorizare a Stocurilor unui Depozit

---

## 1. Contextul de Business (cum funcționează un depozit real)

Un depozit real urmărește marfa prin trei fluxuri principale:

```
FURNIZOR → [Intrare marfă] → DEPOZIT → [Ieșire marfă] → CLIENT
                ↑                              ↑
           Șofer + Mașină                Șofer + Mașină
```

- **Intrare (recepție)**: O mașină aduce marfă de la un furnizor. Se înregistrează: ce produs, câte bucăți, cine a adus, cu ce mașină, când.
- **Stocare**: Produsele sunt plasate într-o locație fizică (zonă → raft → poziție). Fiecare produs are o locație unică sau partajată.
- **Ieșire (livrare)**: Marfa este scoasă din stoc pentru un client/comandă. Se scade cantitatea, se înregistrează tranzacția.
- **Inventar**: La orice moment staff-ul vede stocul curent, alertele, istoricul mișcărilor.

---

## 2. Ierarhia de clase (arhitectura POO)

```
Entitate (baza — id, timestamps)
│
├── Produs  (abstract — id, nume, cantitate, pret, pragAlertă, locatie)
│   ├── ProdusPerisabil   — data_expirare, temperatura_stocare  → FEFO rotation
│   ├── ProdusElectronic  — serie, garantie_luni, voltaj
│   └── ProdusISBN        — isbn, autor, editura, an_publicare
│
├── Persoana (abstract)
│   ├── Sofer             — permis, categorie (B/C/CE)
│   └── StaffDepozit      — rol (manager/operator)
│
├── Vehicul               — nr_inmatriculare, capacitate_kg, tip (camion/utilitara)
│
├── Tranzactie<T>         — template: T = Intrare | Iesire
│   — sofer_id, vehicul_id, lista produse+cantitati, timestamp, status
│
└── Depozit               — colectie produse, locatii, istoric tranzactii
    — unordered_map<int, Produs*> pentru O(1) la 1M produse
```

---

## 3. Cerințe Obligatorii

### 3.1 Clase de bază
- `Produs`: ID, nume, cantitate, pret, pragAlertă, locatie în depozit
- `Depozit`: container STL (`unordered_map<int, Produs*>`) cu ID ca cheie
- Operatori `+=` și `-=` pe `Produs` pentru actualizare cantitate
- Excepții: ID duplicat, produs inexistent, cantitate negativă/insuficientă

### 3.2 Tipuri de produse (moștenire)
- **`ProdusPerisabil`**: câmp `data_expirare` (string sau struct Data), temperatură stocare. Raport produse expirate sau care expiră în N zile.
- **`ProdusElectronic`**: număr de serie, garanție în luni.
- **`ProdusISBN`**: câmp ISBN (13 cifre), autor, editură.

### 3.3 Fluxul de marfă
- **Intrare marfă**: înregistrează sosirea (produs, cantitate, sofer, vehicul, data)
- **Ieșire marfă**: înregistrează plecarea (produs, cantitate, sofer, vehicul, data, destinatie)
- Ambele actualizează stocul și salvează în istoric

### 3.4 Șoferi și Mașini
- `Sofer`: id, nume, nr permis, categorie
- `Vehicul`: id, nr înmatriculare, capacitate (kg), tip
- Asociere: o tranzacție are un șofer și un vehicul

### 3.5 Interfața consolă (producție-like)
```
╔══════════════════════════════════╗
║    DEPOZIT MANAGER v1.0          ║
╠══════════════════════════════════╣
║  [1] Dashboard general           ║
║  [2] Gestionare produse          ║
║  [3] Intrare marfă               ║
║  [4] Ieșire marfă                ║
║  [5] Rapoarte                    ║
║  [6] Export date                 ║
║  [0] Ieșire                      ║
╚══════════════════════════════════╝
```
- Meniuri navigabile cu cifre
- Afișare tabelară aliniată cu `setw`/`iomanip`
- Ecran curat între meniuri (`system("cls")` sau secvențe ANSI)

### 3.6 Persistență fișiere
- Salvare/încărcare stoc din fișier CSV sau binar (`fstream`)
- Format CSV exemplu: `id,tip,nume,cantitate,pret,prag,locatie,...`
- La pornire: încarcă datele; la ieșire: salvează automat

### 3.7 Export marfă
- Export raport stoc curent → `stoc_YYYYMMDD.csv`
- Export istoric tranzacții → `tranzactii_YYYYMMDD.csv`
- Câmpuri minime export: ID, tip produs, nume, cantitate, pret, locatie, data ultima miscare

---

## 4. Cerințe Facultative (bonus)

- `Tranzactie<T>` template unde T este `Intrare` sau `Iesire`
- `Furnizor`: id, nume, contact, lista produse furnizate
- Metodă recomandare recomandă produse de re-comandat, sortate după cantitate rămasă
- Organizare depozit pe **zone** (A, B, C) și **rafturi** — localizare rapidă
- Rotație **FEFO** (First Expired First Out) pentru perisabile — la ieșire scoate primul care expiră
- Teste unitare simple (fără biblioteci externe) pentru logica tranzacțiilor

---

## 5. Performanță — Test 1 Milion de Produse

Problema: `map<int,Produs>` → O(log n) per operație → lent la scară mare.  
Soluție: `unordered_map<int, Produs*>` → O(1) amortizat.

```cpp
// Identificare produs rapid:
unordered_map<int, Produs*> indexPrimary;   // după ID
unordered_map<string, Produs*> indexNume;   // după nume (căutare alternativă)

// Test performanță de inclus în main sau tests/:
void testPerformanta() {
    Depozit d;
    for (int i = 0; i < 1000000; i++)
        d.adaugaProdus(new Produs(i, "Produs_" + to_string(i), 100, 9.99, 10));
    // măsoara timp cu <chrono>
}
```

---

## 6. Dashboard pentru Staff

Afișat la pornire și la opțiunea [1]:

```
╔═══════════════════════════════════════════════════╗
║              DASHBOARD DEPOZIT                    ║
╠═══════════════════════════════════════════════════╣
║  Total produse stocate  :  15,432                 ║
║  Produse sub prag alertă:      47   ⚠             ║
║  Produse expirate       :       3   ✗             ║
║  Tranzactii azi         :      89                 ║
║  Valoare totală stoc    :  234,891.50 RON          ║
╠═══════════════════════════════════════════════════╣
║  Ultima intrare : Lapte UHT  x500  [2026-06-01]   ║
║  Ultima iesire  : Orez 5kg   x120  [2026-06-01]   ║
╚═══════════════════════════════════════════════════╝
```

---

## 7. Identificarea Produselor (când ai mai multe)

Strategii de identificare implementate:
1. **ID numeric** — primar, unic, generat automat (auto-increment)
2. **Nume + tip** — căutare full-text simplă
3. **ISBN** — pentru `ProdusISBN` (exact match 13 cifre)
4. **Serie** — pentru `ProdusElectronic`
5. **Locatie** (zonă-raft-poziție) — `"A-03-12"` → găsești fizic marfa

Funcție de căutare unificată:
```cpp
vector<Produs*> cauta(string query);  // returnează toate potrivirile
```

---

## 8. Structura Proiectului

```
/
├── src/
│   ├── main.cpp
│   ├── Entitate.h / .cpp
│   ├── Produs.h / .cpp
│   ├── ProdusPerisabil.h / .cpp
│   ├── ProdusElectronic.h / .cpp
│   ├── ProdusISBN.h / .cpp
│   ├── Depozit.h / .cpp
│   ├── Sofer.h / .cpp
│   ├── Vehicul.h / .cpp
│   ├── Tranzactie.h          ← template, tot în header
│   ├── UI.h / .cpp           ← meniuri, dashboard, afișare tabelară
│   └── FileManager.h / .cpp  ← save/load CSV, export
├── data/
│   ├── stoc.csv              ← fișier de date principal
│   └── tranzactii.csv
├── tests/
│   └── test_depozit.cpp
├── docs/
│   └── documentatie.md (sau .pdf)
├── Makefile
└── README.md
```

---

## 9. Etape de Dezvoltare

| Etapă | Termen | Livrabil |
|-------|--------|----------|
| 0 | 1 oct | Alegere temă |
| I | 15 oct | Diagramă UML + schiță clase |
| II | 29 oct | `Produs`, `Depozit`, meniu de bază |
| III | 12 nov | Subtipuri produs, Sofer, Vehicul, intrare/ieșire marfă |
| IV | 26 nov | Persistență fișiere, export CSV, dashboard, documentare |
| V | 10 dec | Test 1M produse, teste unitare, finalizare raport |
| VI | 9 ian | **Predare finală** |

---

## 10. Criterii de Notare

| % | Criteriu |
|---|----------|
| 10% | Predare în termen (executabil + cod + documentație) |
| 10% | Prezentare (structură, termeni tehnici, coerență) |
| 30% | Documentație (UML, explicații POO, posibile îmbunătățiri) |
| 30% | Calitate cod C++ (stil, modularitate, comentarii) |
| 20% | Teste de funcționare (inclusiv test 1M produse) |

---

## 11. Git

- Repository: https://classroom.github.com/classrooms/86118538-usmro-poo
- Minim **5 commit-uri descriptive**
- Branch `develop` pentru lucru; `main` pentru versiuni stabile
- `README.md` cu: cerințe sistem, instrucțiuni build (`make` sau `g++`), exemple de rulare
