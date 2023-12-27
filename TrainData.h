//
// Created by alexv on 12/2/23.
//

#ifndef TEMA2_TRAINDATA_H
#define TEMA2_TRAINDATA_H

#include "Train.h"
#include"TrainRoute.h"
#include<vector>
#include<iostream>
#include <unordered_map>
#include <algorithm>

using namespace std;

class TrainData {
private:
    static bool sort_by_arrival(const TrainRoute *a, const TrainRoute *b);
    static bool sort_by_departure(const TrainRoute *a, const TrainRoute *b);
public:
    unordered_map<int,Train*> trains;

    explicit TrainData(pugi::xml_document& doc);

    void print() const;

    vector<string> get_routes(const string &location) const;

    vector<string> get_arrivals(const string &location) const;

    vector<string> get_departures(const string &location) const;



    virtual ~TrainData();


};

#endif //TEMA2_TRAINDATA_H
