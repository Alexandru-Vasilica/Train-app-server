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
#include <exception>

#define ROUTE_PATH "../server-dev/rute-final.xml"

using namespace std;

class TrainData {
private:
    static bool sort_by_arrival(const TrainRoute *a, const TrainRoute *b);
    static bool sort_by_departure(const TrainRoute *a, const TrainRoute *b);
    static bool sort_by_estimated_time(pair<int,const TrainRoute*> a, pair<int,const TrainRoute*> b);
public:
    unordered_map<int,Train*> trains;
    pugi::xml_document* doc;
    explicit TrainData(pugi::xml_document& doc);

    void print() const;

    vector<string> get_routes(const string &location) const;

    vector<string> get_arrivals(const string &location, int current_time) const;

    vector<string> get_departures(const string &location, int current_time) const;

    void set_delay(int train_number,int delay);
    void set_early(int train_number, string &location, int early);

    virtual ~TrainData();


};

#endif //TEMA2_TRAINDATA_H
