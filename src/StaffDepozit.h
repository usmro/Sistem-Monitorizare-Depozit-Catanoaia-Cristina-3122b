#pragma once
#include "Persoana.h"

class StaffDepozit : public Persoana {
private:
    std::string rol;      // manager, operator, supervisor
    std::string username;

public:
    StaffDepozit(int id, const std::string& nume,
                 const std::string& rol, const std::string& username);

    std::string getRol()      const override { return rol; }
    std::string getUsername() const { return username; }

    void        afisare() const;
    std::string toCSV()   const;
};
