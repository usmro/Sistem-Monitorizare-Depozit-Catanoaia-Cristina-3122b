#include "StaffDepozit.h"
#include <iostream>
#include <iomanip>

StaffDepozit::StaffDepozit(int id, const std::string& nume,
                             const std::string& rol, const std::string& username)
    : Persoana(id, nume), rol(rol), username(username) {}

void StaffDepozit::afisare() const {
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(25) << nume
              << std::setw(14) << rol
              << std::setw(16) << username << "\n";
}

std::string StaffDepozit::toCSV() const {
    return std::to_string(id) + "," + nume + "," + rol + "," + username;
}
