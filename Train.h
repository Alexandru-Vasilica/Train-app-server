//
// Created by alexv on 12/2/23.
//

#ifndef TEMA2_TRAIN_H
#define TEMA2_TRAIN_H

#include<vector>
#include<string>
#include <pugixml.hpp>
#include <pthread.h>

using namespace std;

class TrainRoute;

class Train {
public:
    int number;
    pugi::xml_node node;
    pthread_rwlock_t  rwlock;
    vector<TrainRoute> routes;
    string origin,destination;
    int delay;
    explicit Train(pugi::xml_node train);
    void print() const;
    vector<const TrainRoute *> get_routes(const string &location) const;
    vector<const TrainRoute *> get_arrivals(const string &location, int current_time) const;
    vector<const TrainRoute *> get_departures(const string &location, int current_time) const;
    string route_to_string(int idx) const;

};


#endif //TEMA2_TRAIN_H
