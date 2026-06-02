#pragma once
#include "Entitate.h"
#include <string>
#include <vector>

struct Intrare {};
struct Iesire  {};

struct ItemTranzactie {
    int         produsId;
    std::string produsNume;
    int         cantitate;
    double      pretUnitar;
};

template<typename T>
class Tranzactie : public Entitate {
private:
    int                         soferId;
    int                         vehiculId;
    std::vector<ItemTranzactie> items;
    std::string                 timestamp;
    std::string                 status; // "finalizata" | "anulata"

public:
    Tranzactie(int id, int soferId, int vehiculId, const std::string& timestamp,
               const std::string& status = "finalizata")
        : Entitate(id), soferId(soferId), vehiculId(vehiculId),
          timestamp(timestamp), status(status) {}

    void adaugaItem(const ItemTranzactie& item) { items.push_back(item); }
    void anuleaza() { status = "anulata"; }

    int         getSoferId()   const { return soferId; }
    int         getVehiculId() const { return vehiculId; }
    std::string getTimestamp() const { return timestamp; }
    std::string getStatus()    const { return status; }
    bool        esteFinalizata() const { return status == "finalizata"; }
    const std::vector<ItemTranzactie>& getItems() const { return items; }

    std::string getTip() const;
};

template<> inline std::string Tranzactie<Intrare>::getTip() const { return "INTRARE"; }
template<> inline std::string Tranzactie<Iesire>::getTip()  const { return "IESIRE";  }
