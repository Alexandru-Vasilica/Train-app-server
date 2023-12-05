//
// Created by alexv on 12/2/23.
//

#include "TrainData.h"

TrainData::TrainData(const string &xmlFile) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(xmlFile.c_str());
    if (!result){
        perror("Eroare deschidere fisier xml\n");
        exit(1);
    }
    for (pugi::xml_node train: doc.child("XmlIf").child("XmlMts").child("Mt").child("Trenuri").children("Tren")){
        trains.push_back(new Train(train));
    }
}

vector<string> TrainData::get_routes(const string &location) const {
    vector<const TrainRoute*> query;
    vector<string> output;
    cout<<location<<endl;
    for(auto &train:trains) {
            for(auto route:train->get_routes(location))
                query.push_back(route);
        }
    cout<<"Outputing..."<<endl;
    for(auto route:query){
        if(route->origin==location){
            output.push_back(route->departure_to_string());
        }else{
            output.push_back(route->arrival_to_string());
        }
    }
    return output;
}


void TrainData::print() const {
    for(auto &train: trains){
        train->print();
    }
}

TrainData::~TrainData() {
    for(auto train: trains)
        delete train;
}

