#pragma once
#include <string>
#include <ctime>

class Entitate {
protected:
    int         id;
    std::string dataCreare; // timestamp la momentul instantierii

public:
    explicit Entitate(int id = 0) : id(id) {
        time_t now = time(nullptr);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        dataCreare = buf;
    }
    int         getId()        const { return id; }
    std::string getDataCreare() const { return dataCreare; }
    virtual ~Entitate() = default;
};
