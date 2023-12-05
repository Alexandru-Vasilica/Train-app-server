//
// Created by alexv on 12/2/23.
//

#include "Train.h"
#include "TrainRoute.h"

TrainRoute::TrainRoute(pugi::xml_node route, Train *train) {
    this->train=train;
    this->origin=route.attribute("DenStaOrigine").as_string();
    this->destination=route.attribute("DenStaDestinatie").as_string();
    this->time_of_departure=route.attribute("OraP").as_int();
    this->time_of_arrival=route.attribute("OraS").as_int();
}

void TrainRoute::print() const {
    cout<<origin<<" - "<<destination<<": "<<time_of_departure<<" -> "<<time_of_arrival<<endl;
}

string TrainRoute::arrival_to_string() const {
    std::stringstream formattedString;
    formattedString<<"Trenul nr. "<<train->number<<" soseste in "<<destination<<" la "<<time_of_arrival<<" si pleca catre "<<train->destination;
    string output=formattedString.str();
    return output;
}

string TrainRoute::departure_to_string() const {
    std::stringstream formattedString;
    formattedString<<"Trenul nr. "<<train->number<<" pleaca din "<<origin<<" la "<<time_of_departure<<" catre "<<train->destination;
    string output=formattedString.str();
    return output;
}

Train::Train(pugi::xml_node train) {
    number=stoi(train.attribute("Numar").value());
    for (pugi::xml_node route: train.child("Trase").child("Trasa").children("ElementTrasa")){
        routes.push_back(TrainRoute(route,this));
    }
    origin=routes.front().origin;
    destination=routes.back().destination;
}

vector<const TrainRoute *> Train::get_routes(const string &location) const {
    vector<const TrainRoute*>output;
    for( auto  &route:routes){
        if(route.origin==location || route.destination==location) {
            output.push_back(&route);
        }
    }
    return output;
}

void Train::print() const {
    cout<<"--------------Trenul nr. "<<number<<"--------------"<<endl;
    for(auto &route:routes){
        route.print();
    }
}

string Train::route_to_string(int idx) const {
    string output="Tren nr. "+std::to_string(number)+" "+routes[idx].origin+" - "+destination+": "+std::to_string(routes[idx].time_of_departure)+"(urmatoarea oprire: "+routes[idx].destination+")\n";
    return output;
}