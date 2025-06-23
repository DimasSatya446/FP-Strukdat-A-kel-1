#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <cmath>

class Location {
private:
    std::string name;
    double x, y;

public:
    Location() : name(""), x(0), y(0) {}
    Location(const std::string& name, double x = 0, double y = 0) 
        : name(name), x(x), y(y) {}
    
    virtual ~Location() = default;
    
    std::string getName() const { return name; }
    double getX() const { return x; }
    double getY() const { return y; }
    
    void setName(const std::string& name) { this->name = name; }
    void setCoordinates(double x, double y) { this->x = x; this->y = y; }
    
    double distanceTo(const Location& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx*dx + dy*dy);
    }
    
    bool operator<(const Location& other) const {
        return name < other.name;
    }
    
    bool operator==(const Location& other) const {
        return name == other.name;
    }
};

#endif
