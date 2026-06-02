#pragma once
#include "Entitate.h"
#include <string>
#include <vector>

// Tag-uri pentru specializarea template
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

public:
    Tranzactie(int id, int soferId, int vehiculId, const std::string& timestamp)
        : Entitate(id), soferId(soferId), vehiculId(vehiculId), timestamp(timestamp) {}

    void adaugaItem(const ItemTranzactie& item) { items.push_back(item); }

    int         getSoferId()   const { return soferId; }
    int         getVehiculId() const { return vehiculId; }
    std::string getTimestamp() const { return timestamp; }
    const std::vector<ItemTranzactie>& getItems() const { return items; }

    // Specializata mai jos
    std::string getTip() const;
};

template<> inline std::string Tranzactie<Intrare>::getTip() const { return "INTRARE"; }
template<> inline std::string Tranzactie<Iesire>::getTip()  const { return "IESIRE";  }
