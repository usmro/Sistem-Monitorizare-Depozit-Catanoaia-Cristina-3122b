#pragma once
#include "Depozit.h"
#include <string>
#include <vector>

class FileManager {
public:
    // Incarcare si salvare stoc principal
    static void salveazaStoc(const Depozit& d, const std::string& cale = "data/stoc.csv");
    static void incarcaStoc(Depozit& d, const std::string& cale = "data/stoc.csv");

    // Incarcare si salvare soferi/vehicule
    static void salveazaSoferiVehicule(const Depozit& d,
                                        const std::string& caleSoferi   = "data/soferi.csv",
                                        const std::string& caleVehicule = "data/vehicule.csv");
    static void incarcaSoferiVehicule(Depozit& d,
                                       const std::string& caleSoferi   = "data/soferi.csv",
                                       const std::string& caleVehicule = "data/vehicule.csv");

    // Export rapoarte
    static void exportStoc(const Depozit& d, const std::string& cale);
    static void exportIstoric(const Depozit& d, const std::string& cale);

private:
    static std::vector<std::string> split(const std::string& line, char delim);
    static Produs* produsFromCSV(const std::string& line);
};
