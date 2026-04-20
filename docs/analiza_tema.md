# Analiza tema - Sistem de Monitorizare a Stocurilor

## 1. Intelegere cerinte (pe scurt)
Aplicatia trebuie sa urmareasca produsele dintr-un depozit:
- fiecare produs are date de baza (ID, nume, cantitate, pret, prag alerta)
- depozitul tine o colectie de produse intr-un container STL
- se pot face operatii de adaugare, eliminare si actualizare stoc
- se genereaza raport cu produsele care au stoc sub prag

## 2. Clase propuse

### Clasa de baza: Entitate
Responsabilitate:
- pastreaza ID-ul comun pentru obiecte

Atribute:
- `id: int`

Metode:
- constructor
- `getId()`

### Clasa derivata: Produs (mostenire din Entitate)
Responsabilitate:
- modeleaza un produs din depozit

Atribute:
- `nume: string`
- `cantitate: int`
- `pret: double`
- `pragAlerta: int`

Metode:
- constructor
- getteri
- `operator+=(int)` pentru restock
- `operator-=(int)` pentru vanzare
- `esteSubPrag()`

### Clasa Depozit
Responsabilitate:
- gestioneaza colectia de produse

Container STL:
- `map<int, Produs> produse`

Metode:
- `adaugaProdus(const Produs&)`
- `eliminaProdus(int id)`
- `actualizeazaCantitate(int id, int delta)`
- `raportSubPrag() const`
- `afiseazaToate() const`

## 3. Excepții
- `LocOcupatException`: la adaugare cu ID deja existent
- `IndexInvalidException`: la acces/eliminare ID inexistent
- `invalid_argument`: la cantitati negative sau operatii invalide

## 4. Diagrama UML simpla
```text
+------------------+
|     Entitate     |
+------------------+
| - id: int        |
+------------------+
| + getId(): int   |
+------------------+
          ^
          |
+------------------------------+
|            Produs            |
+------------------------------+
| - nume: string               |
| - cantitate: int             |
| - pret: double               |
| - pragAlerta: int            |
+------------------------------+
| + operator+=(int): Produs&   |
| + operator-=(int): Produs&   |
| + esteSubPrag(): bool        |
+------------------------------+

+----------------------------------------+
|                 Depozit                |
+----------------------------------------+
| - produse: map<int, Produs>            |
+----------------------------------------+
| + adaugaProdus(p: Produs): void        |
| + eliminaProdus(id: int): void         |
| + actualizeazaCantitate(id, delta):void|
| + raportSubPrag(): vector<Produs>      |
+----------------------------------------+
```

## 5. Discutie erori comune (student incepator)
- Confuzie intre `map` si `vector`:
  in `map`, cauti dupa cheie (`id`), nu dupa pozitie.
- Uitarea validarii cantitatii:
  nu trebuie sa existe cantitate negativa.
- Lipsa tratarii exceptiilor:
  daca ID-ul nu exista, programul nu trebuie sa crape fara mesaj.
- Folosirea `operator[]` pe `map` la citire:
  acesta poate crea element nou; la cautare e mai sigur `find`.
- Delta interpretata gresit:
  `delta > 0` inseamna restock, `delta < 0` inseamna vanzare.

## 6. Stadiu livrabil
- Documentatie initiala: gata (acest fisier)
- Cod partial rulabil: in `main.cpp`
