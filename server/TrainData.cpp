//
// Created by alexv on 12/2/23.
//

#include "TrainData.h"

TrainData::TrainData(pugi::xml_document& doc) {
    this->doc=&doc;
    for (pugi::xml_node train: doc.child("XmlIf").child("XmlMts").child("Mt").child("Trenuri").children("Tren")){
        trains.insert(pair<int,Train*>(train.attribute("Numar").as_int(),new Train(train)));
    }
}

vector<string> TrainData::get_routes(const string &location) const {
    vector<const TrainRoute*> arrivals,departures;
    vector<string> output;
//    cout<<location<<endl;
    for(auto &[number,train]:trains) {
        for(auto route:train->get_routes(location))
            if(route->origin==location){
                departures.push_back(route);
            }else{
                arrivals.push_back(route);
            }
    }
//    cout<<"Outputing..."<<endl;
    sort(departures.begin(),departures.end(),TrainData::sort_by_departure);
    sort(arrivals.begin(),arrivals.end(),TrainData::sort_by_arrival);
    output.push_back("Sosiri:");
    for(auto route:arrivals){
        output.push_back(route->arrival_to_string());
    }
    if(output.size()<=1){
        string msg="Nu exista sosiri in "+location;
        output.push_back(msg);
    }
    output.push_back("Plecari:");
    int len=output.size();
    for(auto route:departures){
        output.push_back(route->departure_to_string());
    }
    if(output.size()==len){
        string msg="Nu exista plecari din "+location;
        output.push_back(msg);
    }
    return output;
}


void TrainData::print() const {
    for(auto &[number,train]: trains){
        train->print();
    }
}

TrainData::~TrainData() {
    for(auto [number,train]: trains)
        delete train;
}

vector<string> TrainData::get_arrivals(const string &location, int current_time) const {
    vector<string> output;
    vector<pair<int,const TrainRoute*>> query;
    for (auto &[number,train]:trains){
        for(auto &route:train->get_arrivals(location,current_time)){
            query.push_back(route);
        }
    }
    sort(query.begin(),query.end(),TrainData::sort_by_estimated_time);
    output.reserve(query.size());
    for(auto &route:query){
        output.push_back(route.second->arrival_to_string(route.first));
    }
    if(output.empty()){
        string msg="Nu exista sosiri in urmatoarea ora in "+location;
        output.push_back(msg);
    }
    return output;
}

vector<string> TrainData::get_departures(const string &location, int current_time) const {
    vector<pair<int,const TrainRoute*>> query;
    vector<string> output;
    for (auto &[number,train]:trains){
        for(auto &route:train->get_departures(location,current_time)){
            query.push_back(route);
        }
    }
    sort(query.begin(),query.end(),TrainData::sort_by_estimated_time);
    output.reserve(query.size());
    for(auto &route:query){
        output.push_back(route.second->departure_to_string(route.first));
    }
    if(output.empty()){
        string msg="Nu exista plecari in urmatoarea ora din "+location;
        output.push_back(msg);
    }
    return output;
}

bool TrainData::sort_by_arrival(const TrainRoute *a, const TrainRoute *b) {
    return a->time_of_arrival<b->time_of_arrival;
}

bool TrainData::sort_by_departure(const TrainRoute *a, const TrainRoute *b) {
    return a->time_of_departure<b->time_of_departure;
}

void TrainData::set_delay(int train_number, int delay) {
    auto train=trains.find(train_number);
    if(train==trains.end()){
        throw string("Nu exista trenuri cu numarul dat");
    }
    train->second->set_delay(delay);
    doc->save_file(ROUTE_PATH);
}

void TrainData::set_early(int train_number, string &location, int early) {
    auto train=trains.find(train_number);
    if(train==trains.end()){
        throw string("Nu exista trenuri cu numarul dat");
    }
    train->second->set_early(location,early);
    doc->save_file(ROUTE_PATH);
}

bool TrainData::sort_by_estimated_time(pair<int, const TrainRoute *> a, pair<int, const TrainRoute *> b) {
    return a.first<b.first;
}



