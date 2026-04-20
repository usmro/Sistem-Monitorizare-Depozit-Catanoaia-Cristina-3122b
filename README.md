# Sistem monitorizare stocuri

Tema POO facuta simplu, pentru incepator.

## Ce am implementat
- clasa de baza Entitate (are id)
- clasa Produs (mosteneste Entitate)
- clasa Depozit cu map<int, Produs>
- operatii de baza: adaugare produs si eliminare produs
- afisare produse din depozit

Aceasta este varianta partiala (de baza).

## Fisiere
- main.cpp
- docs/analiza_tema.md

## Compilare si rulare (PowerShell)
```powershell
g++ .\main.cpp -std=c++17 -o depozit
.\depozit.exe
```

Daca nu merge comanda g++, inseamna ca nu ai compilator instalat.
