#pragma once
#include <stdexcept>
#include <string>

class DepozitException : public std::runtime_error {
public:
    explicit DepozitException(const std::string& msg) : std::runtime_error(msg) {}
};

class ProdusExistentException : public DepozitException {
public:
    explicit ProdusExistentException(int id)
        : DepozitException("Produs cu ID " + std::to_string(id) + " exista deja") {}
};

class ProdusInexistentException : public DepozitException {
public:
    explicit ProdusInexistentException(int id)
        : DepozitException("Produs cu ID " + std::to_string(id) + " nu exista") {}
};

class CantitateInsuficientaException : public DepozitException {
public:
    CantitateInsuficientaException(int id, int cerut, int disponibil)
        : DepozitException("Cantitate insuficienta pt ID " + std::to_string(id) +
                           " (cerut: " + std::to_string(cerut) +
                           ", disponibil: " + std::to_string(disponibil) + ")") {}
};

class EntitateInexistentaException : public DepozitException {
public:
    EntitateInexistentaException(const std::string& tip, int id)
        : DepozitException(tip + " cu ID " + std::to_string(id) + " nu exista") {}
};
