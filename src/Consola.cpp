#include "Consola.h"
#include <iostream>
#include <limits>

HANDLE Consola::handle() {
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

// ---- Culori / cursor --------------------------------------------------------

void Consola::culoare(WORD attr) {
    SetConsoleTextAttribute(handle(), attr);
}

void Consola::reset() {
    SetConsoleTextAttribute(handle(), C::NORMAL);
}

void Consola::curata() {
    HANDLE h = handle();
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(h, &csbi);
    COORD home  = { 0, 0 };
    DWORD n;
    DWORD cells = (DWORD)csbi.dwSize.X * (DWORD)csbi.dwSize.Y;
    FillConsoleOutputCharacter (h, ' ',              cells, home, &n);
    FillConsoleOutputAttribute (h, csbi.wAttributes, cells, home, &n);
    SetConsoleCursorPosition   (h, home);
}

int Consola::larimeConsola() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(handle(), &csbi)) return 80;
    int w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    if (w < 40)  w = 40;
    if (w > 200) w = 200;
    return w;
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

// ---- Output colorat ---------------------------------------------------------

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

void Consola::scrieOK     (const std::string& s) { scrieLn("  [OK]  " + s, C::VERDE);  }
void Consola::scrieEroare (const std::string& s) { scrieLn("  [ERR] " + s, C::ROSU);   }
void Consola::scrieAtentie(const std::string& s) { scrieLn("  [!]   " + s, C::GALBEN); }
void Consola::scrieInfo   (const std::string& s) { scrieLn("  "       + s, C::CYAN);   }

// ---- Elemente vizuale -------------------------------------------------------

void Consola::titlu(const std::string& text, int w) {
    if (w < 0) { w = larimeConsola() - 4; if (w < 20) w = 20; }
    std::string bara(w, '=');
    int pad = (w - (int)text.size() - 2) / 2;
    if (pad < 0) pad = 0;
    int right = w - 2 - pad - (int)text.size();
    if (right < 0) right = 0;
    std::string row = "| " + std::string(pad, ' ') + text + std::string(right, ' ') + "|";

    culoare(C::TITLU_BG);
    std::cout << "  +" << bara << "+\n";
    std::cout << "  " << row  << "\n";
    std::cout << "  +" << bara << "+\n";
    reset();
    std::cout << "\n";
}

void Consola::linie(int w) {
    if (w < 0) { w = larimeConsola() - 4; if (w < 20) w = 20; }
    culoare(C::GRI_I);
    std::cout << "  " << std::string(w, '-') << "\n";
    reset();
}

// ---- Sunete -----------------------------------------------------------------

void Consola::sunetStart()  { Beep(523,120); Beep(659,120); Beep(784,180); }
void Consola::sunetOK()     { Beep(880,80);  Beep(1047,120); }
void Consola::sunetEroare() { Beep(300,200); Beep(200,300); }

// ---- Input helpers cu validare ----------------------------------------------

int Consola::readInt(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        culoare(C::CYAN);
        std::cout << "  " << prompt;
        reset();
        int val;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (val >= minVal && val <= maxVal) return val;
            if (minVal != INT_MIN || maxVal != INT_MAX)
                scrieEroare("Valoare in afara intervalului [" +
                            std::to_string(minVal) + ", " + std::to_string(maxVal) + "].");
            else
                scrieEroare("Input invalid.");
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            scrieEroare("Input invalid. Introduceti un numar intreg.");
        }
    }
}

double Consola::readDouble(const std::string& prompt, double minVal, double maxVal) {
    while (true) {
        culoare(C::CYAN);
        std::cout << "  " << prompt;
        reset();
        double val;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (val >= minVal && val <= maxVal) return val;
            scrieEroare("Valoare in afara intervalului.");
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            scrieEroare("Input invalid. Introduceti un numar.");
        }
    }
}

std::string Consola::readString(const std::string& prompt) {
    culoare(C::CYAN);
    std::cout << "  " << prompt;
    reset();
    std::string val;
    std::getline(std::cin, val);
    return val;
}

bool Consola::confirma(const std::string& prompt) {
    culoare(C::GALBEN);
    std::cout << "  " << prompt << " [d/N]: ";
    reset();
    std::string r;
    std::getline(std::cin, r);
    return !r.empty() && (r[0] == 'd' || r[0] == 'D' || r[0] == 'y' || r[0] == 'Y');
}

// ---- Meniu cu sageti + mouse (hover / click / scroll) + cifre directe -------

int Consola::meniu(const std::vector<std::string>& optiuni) {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    int n    = (int)optiuni.size();
    int sel  = 0;

    // Full-width highlight (minus a small margin so we never trigger line-wrap)
    int WIDE = larimeConsola() - 2;
    if (WIDE < 46) WIDE = 46;

    // Enable mouse input; disable quick-edit so mouse events reach us
    DWORD oldMode;
    GetConsoleMode(hInput, &oldMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    ascundeCursor();
    COORD start = pozCursor();

    // Helper: draw a single option (highlighted or normal)
    auto desenOpt = [&](int i, bool selectat) {
        gotoxy(0, start.Y + i);
        culoare(selectat ? C::SEL : C::NORMAL);
        std::string ln = "  " + optiuni[i];
        ln += std::string(std::max(0, WIDE - (int)ln.size()), ' ');
        std::cout << ln;
    };

    // Initial draw
    for (int i = 0; i < n; i++) desenOpt(i, i == 0);
    reset();
    gotoxy(0, start.Y + n + 1);
    culoare(C::GRI_I);
    std::cout << "  [Sageti/Mouse] Naviga   [Enter/Click] Alege   [Esc] Inapoi\n";
    reset();

    INPUT_RECORD rec;
    DWORD numRead;

    while (true) {
        ReadConsoleInput(hInput, &rec, 1, &numRead);

        // ---- Tastatura ----
        if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown) {
            WORD vk  = rec.Event.KeyEvent.wVirtualKeyCode;
            char ch  = rec.Event.KeyEvent.uChar.AsciiChar;
            int prev = sel;

            if      (vk == VK_UP    && sel > 0)     sel--;
            else if (vk == VK_DOWN  && sel < n - 1) sel++;
            else if (vk == VK_RETURN) {
                arataCursor();
                SetConsoleMode(hInput, oldMode);
                return sel;
            } else if (vk == VK_ESCAPE) {
                arataCursor();
                SetConsoleMode(hInput, oldMode);
                return n - 1;
            } else if (ch >= '0' && ch <= '9') {
                // Direct digit shortcut: match "[N]" prefix
                for (int i = 0; i < n; i++) {
                    if (optiuni[i].size() >= 3 &&
                        optiuni[i][0] == '[' && optiuni[i][1] == ch) {
                        desenOpt(sel, false);
                        desenOpt(i,   true);
                        reset();
                        arataCursor();
                        SetConsoleMode(hInput, oldMode);
                        return i;
                    }
                }
                continue;
            } else {
                continue;
            }

            if (sel != prev) {
                desenOpt(prev, false);
                desenOpt(sel,  true);
                reset();
            }

        // ---- Mouse ----
        } else if (rec.EventType == MOUSE_EVENT) {
            auto& me  = rec.Event.MouseEvent;
            int optIdx = (int)me.dwMousePosition.Y - (int)start.Y;

            if (me.dwEventFlags == MOUSE_MOVED) {
                // Hover: highlight option under cursor
                if (optIdx >= 0 && optIdx < n && optIdx != sel) {
                    int prev = sel;
                    sel = optIdx;
                    desenOpt(prev, false);
                    desenOpt(sel,  true);
                    reset();
                }
            } else if (me.dwEventFlags == 0 &&
                       (me.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
                // Left-button click: select immediately
                if (optIdx >= 0 && optIdx < n) {
                    desenOpt(sel, false);
                    sel = optIdx;
                    desenOpt(sel, true);
                    reset();
                    arataCursor();
                    SetConsoleMode(hInput, oldMode);
                    return sel;
                }
            } else if (me.dwEventFlags == MOUSE_WHEELED) {
                // Scroll wheel: navigate
                SHORT delta = (SHORT)(me.dwButtonState >> 16);
                int prev = sel;
                if      (delta > 0 && sel > 0)     sel--;
                else if (delta < 0 && sel < n - 1) sel++;
                else continue;
                desenOpt(prev, false);
                desenOpt(sel,  true);
                reset();
            }
        }
    }
}
