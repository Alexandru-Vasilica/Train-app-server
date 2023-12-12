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

using namespace std;

class TrainData {
public:
    unordered_map<int,Train*> trains;

    explicit TrainData(const string &xmlFile);

    void print() const;

    vector<string> get_routes(const string &location) const;

    vector<string> get_arrivals(const string &location) const;

    vector<string> get_departures(const string &location) const;

    virtual ~TrainData();


};

#endif //TEMA2_TRAINDATA_H
