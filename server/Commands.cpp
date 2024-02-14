//
// Created by alexv on 12/3/23.
//

#include "Commands.h"

void Command::sendResponse(vector<string> response) {
    int lines=response.size();
    // cout<<lines<<endl;
    write(fd,&lines,sizeof(int));


    for(int i=0;i<lines;i++){
        write(fd,response[i].c_str(),RESPONSE_LINE_BUFFER_SIZE);
//          cout<<response[i]<<endl;
    }

}

void Command::sendResponse(string response) {
    int lines=1;
    write(fd,&lines,sizeof(int));
    write(fd,response.c_str(),RESPONSE_LINE_BUFFER_SIZE);

}

RoutesCommand::RoutesCommand(int fd, TrainData *data, string location) {
    this->fd=fd;
    this->data=data;
    this->location=location;
}

void RoutesCommand::execute() {
    // cout<<"Executing.."<<endl;
    // cout<<location<<endl;
    auto response=data->get_routes(location);
    // cout<<"Got response"<<endl;
    sendResponse(response);
}

command_type RoutesCommand::get_type() {
    return RUTE;
}


ArrivalsCommand::ArrivalsCommand(int fd, TrainData *data, string location) {
    this->fd=fd;
    this->data=data;
    this->location=location;
    time_t time_data;
    time(&time_data);
    struct tm* local_time;
    local_time = localtime(&time_data);
    this->current_time = local_time->tm_hour * 3600 + local_time->tm_min * 60 + local_time->tm_sec;
}

void ArrivalsCommand::execute() {
    vector<string> output=data->get_arrivals(location,current_time);
    sendResponse(output);
}

command_type ArrivalsCommand::get_type() {
    return SOSIRI;
}

DeparturesCommand::DeparturesCommand(int fd, TrainData *data, string location) {
    this->fd=fd;
    this->data=data;
    this->location=location;
    time_t time_data;
    time(&time_data);
    struct tm* local_time;
    local_time = localtime(&time_data);
    this->current_time = local_time->tm_hour * 3600 + local_time->tm_min * 60 + local_time->tm_sec;
}

void DeparturesCommand::execute() {
    vector<string> output=data->get_departures(location,current_time);
    sendResponse(output);
}

command_type DeparturesCommand::get_type() {
    return PLECARI;
}

DelayCommand::DelayCommand(int fd, TrainData *data, int number, int delay) {
    this->fd=fd;
    this->data=data;
    this->train_number=number;
    this->delay=delay;
}

void DelayCommand::execute() {
    try{
        data->set_delay(train_number,delay);
        sendResponse("Intarzierea a fost inregistrata");
    }catch (string &msg){
        sendResponse(msg);
    }
}

command_type DelayCommand::get_type() {
    return INTARZIERE;
}

QuitCommand::QuitCommand(int fd, TrainData *data, fd_set *fds) {
    this->fd=fd;
    this->data=data;
    this->fds=fds;
}

void QuitCommand::execute() {
    FD_CLR(fd,fds);
    sendResponse("La revedere!");
}

command_type QuitCommand::get_type() {
    return QUIT;
}


EstimateCommand::EstimateCommand(int fd, TrainData *data, int number,string location, int early) {
    this->fd=fd;
    this->data=data;
    this->train_number=number;
    this->early=early;
    this->location=location;
}

void EstimateCommand::execute() {
    try{
        data->set_early(train_number,location,early);
        sendResponse("Estimarea a fost inregistrata");
    }catch (string &msg){
        sendResponse(msg);
    }
}

command_type EstimateCommand::get_type() {
    return ESTIMARE;
}
