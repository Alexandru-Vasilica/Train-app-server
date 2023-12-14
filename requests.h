#include "Commands.h"
#define COMMNANDLINE_BUFFER_SIZE 200

struct request
{
    int fd;
    char location[20];
    command_type type;
    int argc;
    char argv[5][COMMNANDLINE_BUFFER_SIZE];
};

Command* get_request(struct request req,TrainData& source, fd_set &fds){
    Command* command= nullptr;
//    source.print();
    switch (req.type) {
        case RUTE:
            command=new RoutesCommand(req.fd,&source,req.location);
            break;
        case SOSIRI:
            command=new ArrivalsCommand(req.fd,&source,req.location);
            break;
        case PLECARI:
            command=new DeparturesCommand(req.fd,&source,req.location);
            break;
        case INTARZIERE:
            command= new DelayCommand(req.fd, &source, stoi(req.argv[0]), stoi(req.argv[1]));
            break;
        case ESTIMARE:
            command= new EstimateCommand(req.fd, &source, stoi(req.argv[0]), stoi(req.argv[1]));
            break;
        case QUIT:
            command=new QuitCommand(req.fd,&source,&fds);
            break;
    }

    return command;
}