# Sistem Monitorizare Stocuri - proiect POO (varianta simpla)

Proiect C++ foarte simplu pentru gestionarea stocurilor dintr-un depozit.

## Ce contine
- Clasa de baza `Entitate`
- Clasa derivata `Produs`
- Clasa `Depozit` care foloseste `std::map<int, Produs>`
- Operatori supraincarcati `+=` si `-=` pentru cantitate
- Excepții simple pentru ID duplicat si ID inexistent
- Raport pentru produse sub pragul de alerta

## Fisiere
- `docs/analiza_tema.md` - analiza cerintelor + diagrama UML simpla
- `main.cpp` - implementare minima rulabila

## Cum rulezi (PowerShell)
```powershell
g++ .\main.cpp -std=c++17 -Wall -Wextra -o depozit
.\depozit.exe
```

Daca nu ai `g++`, poti folosi compilatorul din IDE-ul tau (Visual Studio / Code::Blocks / Dev-C++).
