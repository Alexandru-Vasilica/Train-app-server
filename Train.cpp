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

void TrainRoute::set_early(int early) {
    this->early=early;
    node.attribute("Estimare").set_value(early);
}

string TrainRoute::departure_to_string(int estimated_time) const {
    std::stringstream formattedString;
    formattedString<<"Trenul nr. "<<train->number<<" pleaca din "<<origin<<" la "<<TrainRoute::estimated_time_to_string(estimated_time)<<"(ora din program "<<this->departure_time_to_string() <<") catre "<<destination<<"(statie finala "<<train->destination<<")";
    string output=formattedString.str();
    return output;
}

string TrainRoute::arrival_to_string(int estimated_time) const {
    std::stringstream formattedString;
    formattedString<<"Trenul nr. "<<train->number<<" soseste in "<<destination<<" la "<<TrainRoute::estimated_time_to_string(estimated_time)<<"(ora din program "<<this->arrival_time_to_string() <<") dinspre "<<origin<<"(statie finala "<<train->destination<<")";
    string output=formattedString.str();
    return output;
}

string TrainRoute::estimated_time_to_string(int estimated_time) {
    int hours = estimated_time / 3600;
    int minutes = (estimated_time % 3600) / 60;
    int seconds = estimated_time % 60;

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

vector<pair<int, const TrainRoute *>> Train::get_arrivals(const string &location, int current_time) {
    vector<pair<int, const TrainRoute *>> output;
    pthread_rwlock_rdlock(&rwlock);
//    cout<<"Train number locked: "<<number<<endl;
//    while(true){
//
//    }
    for( auto  &route:routes){
        int estimated_time=route.time_of_arrival+delay*60-route.early*60;
        if(route.destination==location && ((current_time<=estimated_time && estimated_time<=current_time+3600) || estimated_time<=current_time+3600-SECONDS_IN_DAY)) {
            output.push_back(pair(estimated_time,&route));
        }
    }
    pthread_rwlock_unlock(&rwlock);
    return output;
}

vector<pair<int, const TrainRoute *>> Train::get_departures(const string &location, int current_time) {
    vector<pair<int, const TrainRoute *>> output;
    pthread_rwlock_rdlock(&rwlock);
    for( auto  &route:routes){
        int estimated_time=route.time_of_departure+delay*60-route.early*60;
        if(route.origin==location && ((current_time<=estimated_time) && (estimated_time<=current_time+3600) || estimated_time<=current_time+3600-SECONDS_IN_DAY)) {
            output.push_back(pair(estimated_time,&route));
        }
    }
    pthread_rwlock_unlock(&rwlock);
    return output;
}

void Train::set_delay(int delay) {
    pthread_rwlock_wrlock(&rwlock);
    this->delay=delay;
    node.attribute("Intarziere").set_value(delay);
    pthread_rwlock_unlock(&rwlock);
}

void Train::set_early(string &location, int early) {
    bool found=false;
    TrainRoute* target;
    for(auto &route:routes){
        if(route.destination==location){
            found=true;
            target=&route;
            break;
        }
    }
    if(!found){
        string output="Trenul cu numarul "+ to_string(number)+" nu opreste in statia dumneavoastra!";
        throw output;
    }
    pthread_rwlock_wrlock(&rwlock);
    if(delay!=0){
        delay=0;
        node.attribute("Intarziere").set_value(0);
    }
    target->set_early(early);
    pthread_rwlock_unlock(&rwlock);
}
