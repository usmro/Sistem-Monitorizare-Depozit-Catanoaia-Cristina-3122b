# Analiza tema - Sistem de monitorizare a stocurilor

## 1. Ce cere tema
Trebuie sa fac un program simplu pentru depozit:
- tin produse in memorie
- fiecare produs are id, nume, cantitate, pret
- pot adauga produs
- pot elimina produs
- pot afisa lista de produse

## 2. Clasele

### Entitate
- are doar id

### Produs (mosteneste Entitate)
- nume
- cantitate
- pret

### Depozit
- are map<int, Produs>
- adauga produs
- elimina produs
- afiseaza produse

## 3. Observatii
- in aceasta varianta partiala nu folosesc exceptii
- in aceasta varianta partiala nu folosesc operatori supraincarcati

## 4. UML simplu
```text
Entitate
 - id: int

Produs : Entitate
 - nume: string
 - cantitate: int
 - pret: double

Depozit
 - produse: map<int, Produs>
 + adaugaProdus(...)
 + eliminaProdus(...)
 + afisareToate()
```

## 5. Erori comune
- id duplicat la adaugare
- id inexistent la stergere

## 6. Livrabil
- documentatie: fisierul acesta
- cod partial rulabil (varianta de baza): main.cpp
