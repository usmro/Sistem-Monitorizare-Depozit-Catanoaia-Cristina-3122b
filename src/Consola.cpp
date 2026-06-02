#include "Consola.h"
#include <iostream>
#include <conio.h>

HANDLE Consola::handle() {
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

void Consola::culoare(WORD attr) {
    SetConsoleTextAttribute(handle(), attr);
}

void Consola::reset() {
    SetConsoleTextAttribute(handle(), C::NORMAL);
}

void Consola::curata() {
    system("cls");
}

void Consola::gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(handle(), pos);
}

COORD Consola::pozCursor() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(handle(), &csbi);
    return csbi.dwCursorPosition;
}

void Consola::ascundeCursor() {
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(handle(), &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(handle(), &cci);
}

void Consola::arataCursor() {
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(handle(), &cci);
    cci.bVisible = TRUE;
    SetConsoleCursorInfo(handle(), &cci);
}

void Consola::scrie(const std::string& s, WORD attr) {
    culoare(attr);
    std::cout << s;
    reset();
}

void Consola::scrieLn(const std::string& s, WORD attr) {
    culoare(attr);
    std::cout << s << "\n";
    reset();
}

void Consola::scrieOK(const std::string& s) {
    scrieLn("  [OK] " + s, C::VERDE);
}

void Consola::scrieEroare(const std::string& s) {
    scrieLn("  [ERR] " + s, C::ROSU);
}

void Consola::scrieAtentie(const std::string& s) {
    scrieLn("  [!] " + s, C::GALBEN);
}

void Consola::scrieInfo(const std::string& s) {
    scrieLn("  " + s, C::CYAN);
}

void Consola::titlu(const std::string& text, int w) {
    std::string bara(w, '=');
    int pad = (w - (int)text.size() - 2) / 2;
    if (pad < 0) pad = 0;
    std::string linie = "| " + std::string(pad, ' ') + text +
                        std::string(w - 2 - pad - (int)text.size(), ' ') + "|";

    culoare(C::TITLU_BG);
    std::cout << "  +" << bara << "+\n";
    std::cout << "  " << linie << "\n";
    std::cout << "  +" << bara << "+\n";
    reset();
    std::cout << "\n";
}

void Consola::linie(int w) {
    culoare(C::GRI_I);
    std::cout << "  " << std::string(w, '-') << "\n";
    reset();
}

// ---- Sunete -----------------------------------------------------------------

void Consola::sunetStart() {
    Beep(523, 120); Beep(659, 120); Beep(784, 180);
}

void Consola::sunetOK() {
    Beep(880, 80); Beep(1047, 120);
}

void Consola::sunetEroare() {
    Beep(300, 200); Beep(200, 300);
}

// ---- Meniu cu sageti --------------------------------------------------------

int Consola::meniu(const std::vector<std::string>& optiuni) {
    const int WIDE = 46; // latimea afisata a fiecarei optiuni
    int n   = (int)optiuni.size();
    int sel = 0;

    ascundeCursor();

    // Salveaza pozitia de start
    COORD start = pozCursor();

    // Afiseaza optiunile initial
    for (int i = 0; i < n; i++) {
        culoare(i == 0 ? C::SEL : C::NORMAL);
        std::string linie = "  " + optiuni[i];
        linie += std::string(std::max(0, WIDE - (int)linie.size()), ' ');
        std::cout << linie << "\n";
    }
    reset();
    culoare(C::GRI_I);
    std::cout << "\n  [Sageti] Naviga  [Enter] Alege  [Esc] Inapoi\n";
    reset();

    while (true) {
        int ch = _getch();

        if (ch == 0 || ch == 224) { // tasta speciala (sageti)
            ch = _getch();
            int prev = sel;
            if      (ch == 72 && sel > 0)     sel--; // sus
            else if (ch == 80 && sel < n - 1) sel++; // jos
            else continue;

            // Redeseneaza doar liniile modificate
            gotoxy(0, start.Y + prev);
            culoare(C::NORMAL);
            std::string lp = "  " + optiuni[prev];
            lp += std::string(std::max(0, WIDE - (int)lp.size()), ' ');
            std::cout << lp;

            gotoxy(0, start.Y + sel);
            culoare(C::SEL);
            std::string ls = "  " + optiuni[sel];
            ls += std::string(std::max(0, WIDE - (int)ls.size()), ' ');
            std::cout << ls;
            reset();

        } else if (ch == 13) { // Enter
            arataCursor();
            return sel;

        } else if (ch == 27) { // Escape -> ultima optiune (Inapoi/Iesire)
            arataCursor();
            return n - 1;

        } else if (ch >= '0' && ch <= '9') { // cifra directa
            for (int i = 0; i < n; i++) {
                if (optiuni[i].size() >= 3 &&
                    optiuni[i][0] == '[' && optiuni[i][1] == (char)ch) {
                    arataCursor();
                    return i;
                }
            }
        }
    }
}
