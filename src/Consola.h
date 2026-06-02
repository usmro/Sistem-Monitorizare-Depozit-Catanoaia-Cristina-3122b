#pragma once
#include <string>
#include <vector>
#include <windows.h>

// Atribute culoare (fg = foreground, bg = background)
// Valori directe compatibile cu SetConsoleTextAttribute
namespace C {
    // Foreground
    const WORD NEGRU      = 0;
    const WORD ALBASTRU   = 1;
    const WORD VERDE_I    = 2;
    const WORD CYAN_I     = 3;
    const WORD ROSU_I     = 4;
    const WORD GRI        = 7;
    const WORD GRI_I      = 8;
    const WORD ALBASTRU_D = 9;
    const WORD VERDE      = 10;
    const WORD CYAN       = 11;
    const WORD ROSU       = 12;
    const WORD GALBEN     = 14;
    const WORD ALB        = 15;

    // Combinatii bg*16 + fg
    const WORD SEL        = 0x70; // negru pe gri (selectat in meniu)
    const WORD TITLU_BG   = 0x30; // negru pe cyan inchis
    const WORD NORMAL     = 0x07; // gri pe negru (default)
}

class Consola {
public:
    static void culoare(WORD attr);
    static void reset();
    static void curata();
    static void gotoxy(int x, int y);
    static COORD pozCursor();
    static void ascundeCursor();
    static void arataCursor();

    // Output colorat
    static void scrie     (const std::string& s, WORD attr);
    static void scrieLn   (const std::string& s, WORD attr);
    static void scrieOK   (const std::string& s); // verde
    static void scrieEroare(const std::string& s); // rosu
    static void scrieAtentie(const std::string& s); // galben
    static void scrieInfo (const std::string& s); // cyan

    // Elemente vizuale
    static void titlu(const std::string& text, int latime = 60);
    static void linie(int latime = 60);

    // Sunete
    static void sunetStart();
    static void sunetOK();
    static void sunetEroare();

    // Meniu navigabil cu sageti + cifre directe
    // optiuni ex: {"[1] Gestionare produse", "[2] Intrare", "[0] Iesire"}
    // Returneaza index-ul ales (0-based)
    static int meniu(const std::vector<std::string>& optiuni);

private:
    static HANDLE handle();
};
