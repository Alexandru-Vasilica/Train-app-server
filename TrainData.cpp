//
// Created by alexv on 12/2/23.
//

#include "TrainData.h"

TrainData::TrainData(pugi::xml_document& doc) {
    for (pugi::xml_node train: doc.child("XmlIf").child("XmlMts").child("Mt").child("Trenuri").children("Tren")){
        trains.insert(pair<int,Train*>(train.attribute("Numar").as_int(),new Train(train)));
    }
}

//vector<string> TrainData::get_routes(const string &location) const {
//    vector<const TrainRoute*> query;
//    vector<string> output;
//    cout<<location<<endl;
//    for(auto &[number,train]:trains) {
//            for(auto route:train->get_routes(location))
//                query.push_back(route);
//        }
////    cout<<"Outputing..."<<endl;
//    for(auto route:query){
//        if(route->origin==location){
//            output.push_back(route->departure_to_string());
//        }else{
//            output.push_back(route->arrival_to_string());
//        }
//    }
//    return output;
//}

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
    output.push_back("Plecari:");
    for(auto route:departures){
        output.push_back(route->departure_to_string());
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
    for (auto &[number,train]:trains){
        for(auto &route:train->get_arrivals(location,current_time)){
            output.push_back(route->arrival_to_string());
        }
    }
    return output;
}

vector<string> TrainData::get_departures(const string &location, int current_time) const {
    vector<string> output;
    for (auto &[number,train]:trains){
        for(auto &route:train->get_departures(location,current_time)){
            output.push_back(route->departure_to_string());
        }
    }
    return output;
}

bool TrainData::sort_by_arrival(const TrainRoute *a, const TrainRoute *b) {
    return a->time_of_arrival<b->time_of_arrival;
}

bool TrainData::sort_by_departure(const TrainRoute *a, const TrainRoute *b) {
    return a->time_of_departure<b->time_of_departure;
}


