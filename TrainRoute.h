//
// Created by alexv on 12/2/23.
//

#ifndef TEMA2_TRAINROUTE_H
#define TEMA2_TRAINROUTE_H

#include <string>
#include <vector>
#include <iostream>
#include <pugixml.hpp>
#include <sstream>
#include <iomanip>
#define SECONDS_IN_DAY 86400

using namespace std;
class Train;

class TrainRoute {
public:
    Train* train;
    string origin;
    string destination;
    pugi::xml_node node;
    int time_of_arrival;
    int time_of_departure;
    int early;
    explicit TrainRoute(pugi::xml_node route,Train* train);
    void print() const;
    string arrival_to_string() const;
    string departure_to_string() const;
    string departure_time_to_string() const;
    string arrival_time_to_string() const;
};


#endif //TEMA2_TRAINROUTE_H
