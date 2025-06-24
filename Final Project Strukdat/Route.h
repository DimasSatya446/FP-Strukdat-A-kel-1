#ifndef ROUTE_H
#define ROUTE_H

#include "Location.h"
#include <string>

using namespace std;

class Route {
private:
    string source;
    string destination;
    double distance;
    double time;
    double cost;

public:
    Route() : source(""), destination(""), distance(0), time(0), cost(0) {}
    Route(const string& source, const string& destination,
          double distance, double time, double cost)
        : source(source), destination(destination), distance(distance),
          time(time), cost(cost) {}
    
    virtual ~Route() = default;
    
    string getSource() const { return source; }
    string getDestination() const { return destination; }
    double getDistance() const { return distance; }
    double getTime() const { return time; }
    double getCost() const { return cost; }
    
    void setSource(const string& source) { this->source = source; }
    void setDestination(const string& destination) { this->destination = destination; }
    void setDistance(double distance) { this->distance = distance; }
    void setTime(double time) { this->time = time; }
    void setCost(double cost) { this->cost = cost; }
    
    double getWeightByMode(const string& mode) const {
        if (mode == "jarak") return distance;
        else if (mode == "waktu") return time;
        else if (mode == "biaya") return cost;
        return 0;
    }
};

#endif
