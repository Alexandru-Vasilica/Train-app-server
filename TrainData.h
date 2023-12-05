//
// Created by alexv on 12/2/23.
//

#ifndef TEMA2_TRAINDATA_H
#define TEMA2_TRAINDATA_H

#include "Train.h"
#include"TrainRoute.h"
#include<vector>
#include<iostream>

using namespace std;

class TrainData {
public:
    vector<Train*> trains;

    explicit TrainData(const string &xmlFile);

    void print() const;

    vector<string> get_routes(const string &location) const;

    virtual ~TrainData();


};

#endif //TEMA2_TRAINDATA_H
