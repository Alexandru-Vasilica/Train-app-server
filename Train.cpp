//
// Created by alexv on 12/2/23.
//

#include "Train.h"
#include "TrainRoute.h"

TrainRoute::TrainRoute(pugi::xml_node route, Train *train) {
    this->train=train;
    this->node=route;
    this->origin=route.attribute("DenStaOrigine").as_string();
    this->destination=route.attribute("DenStaDestinatie").as_string();
    this->time_of_departure=route.attribute("OraP").as_int();
    this->time_of_arrival=route.attribute("OraS").as_int();
    this->early=route.attribute("Estimare").as_int();

}

void TrainRoute::print() const {
    cout<<node.attribute("Secventa").value()<<" "<<origin<<" - "<<destination<<": "<<time_of_departure<<" -> "<<time_of_arrival<<endl;
}

string TrainRoute::arrival_to_string() const {
    std::stringstream formattedString;
    formattedString<<"Trenul nr. "<<train->number<<" soseste in "<<destination<<" la "<<this->arrival_time_to_string()<<" dinspre "<<origin<<"(statie finala "<<train->destination<<")";
    string output=formattedString.str();
    return output;
}

string TrainRoute::departure_to_string() const {
    std::stringstream formattedString;
    formattedString<<"Trenul nr. "<<train->number<<" pleaca din "<<origin<<" la "<<this->departure_time_to_string()<<" catre "<<destination<<"(statie finala "<<train->destination<<")";
    string output=formattedString.str();
    return output;
}

string TrainRoute::departure_time_to_string() const {
    int hours = time_of_departure / 3600;
    int minutes = (time_of_departure % 3600) / 60;
    int seconds = time_of_departure % 60;

    std::stringstream time;
    time << setw(2) << setfill('0') << hours << ":"
                   << setw(2) << setfill('0') << minutes << ":"
                   << setw(2) << setfill('0') << seconds;

    return time.str();
}

string TrainRoute::arrival_time_to_string() const {
    int hours = time_of_arrival / 3600;
    int minutes = (time_of_arrival % 3600) / 60;
    int seconds = time_of_arrival % 60;

    std::stringstream time;
    time << setw(2) << setfill('0') << hours << ":"
                   << setw(2) << setfill('0') << minutes << ":"
                   << setw(2) << setfill('0') << seconds;

    return time.str();
}

Train::Train(pugi::xml_node train) {
    number=stoi(train.attribute("Numar").value());
    node=train;
    for (pugi::xml_node route: train.child("Trase").child("Trasa").children("ElementTrasa")){
        routes.push_back(TrainRoute(route,this));
    }
    origin=routes.front().origin;
    destination=routes.back().destination;
    delay=train.attribute("Intarziere").as_int();
    rwlock = PTHREAD_RWLOCK_INITIALIZER;
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
    cout<<"--------------Trenul nr. "<<number<<" "<<node.attribute("Numar").value()<<"--------------"<<endl;
    for(auto &route:routes){
        route.print();
    }
}

string Train::route_to_string(int idx) const {
    string output="Tren nr. "+std::to_string(number)+" "+routes[idx].origin+" - "+destination+": "+std::to_string(routes[idx].time_of_departure)+"(urmatoarea oprire: "+routes[idx].destination+")\n";
    return output;
}

vector<const TrainRoute *> Train::get_arrivals(const string &location) const {
    vector<const TrainRoute*>output;
    for( auto  &route:routes){
        if(route.destination==location) {
            output.push_back(&route);
        }
    }
    return output;
}

vector<const TrainRoute *> Train::get_departures(const string &location) const {
    vector<const TrainRoute*>output;
    for( auto  &route:routes){
        if(route.origin==location) {
            output.push_back(&route);
        }
    }
    return output;
}
