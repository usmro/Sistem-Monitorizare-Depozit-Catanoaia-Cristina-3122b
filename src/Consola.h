#pragma once
#include <string>
#include <vector>
#include <climits>
#include <windows.h>

namespace C {
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
    static int  larimeConsola();

    // Output colorat
    static void scrie      (const std::string& s, WORD attr);
    static void scrieLn    (const std::string& s, WORD attr);
    static void scrieOK    (const std::string& s);
    static void scrieEroare(const std::string& s);
    static void scrieAtentie(const std::string& s);
    static void scrieInfo  (const std::string& s);

    // Elemente vizuale (latime = -1 → latimea terminalului)
    static void titlu(const std::string& text, int latime = -1);
    static void linie(int latime = -1);

    // Sunete
    static void sunetStart();
    static void sunetOK();
    static void sunetEroare();

    // Input helpers cu validare si recuperare la fail
    static int         readInt   (const std::string& prompt,
                                  int    minVal = INT_MIN, int    maxVal = INT_MAX);
    static double      readDouble(const std::string& prompt,
                                  double minVal = -1e18,  double maxVal =  1e18);
    static std::string readString(const std::string& prompt);
    static bool        confirma  (const std::string& prompt);

    // Meniu navigabil: sageti + cifre directe + mouse (hover/click/scroll)
    static int meniu(const std::vector<std::string>& optiuni);

private:
    static HANDLE handle();
};
