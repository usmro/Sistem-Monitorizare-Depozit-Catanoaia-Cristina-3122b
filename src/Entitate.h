#pragma once

class Entitate {
protected:
    int id;
public:
    explicit Entitate(int id = 0) : id(id) {}
    int getId() const { return id; }
    virtual ~Entitate() = default;
};
