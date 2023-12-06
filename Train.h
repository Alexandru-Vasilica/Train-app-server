//
// Created by alexv on 12/2/23.
//

#ifndef TEMA2_TRAIN_H
#define TEMA2_TRAIN_H

#include<vector>
#include<string>
#include <pugixml.hpp>

using namespace std;

class TrainRoute;

class Train {
public:
    int number;
    vector<TrainRoute> routes;
    string origin,destination;
    explicit Train(pugi::xml_node train);
    void print() const;
    vector<const TrainRoute *> get_routes(const string &location) const;
    string route_to_string(int idx) const;

};


#endif //TEMA2_TRAIN_H