//
// Created by alexv on 12/3/23.
//

#ifndef TEMA2_COMMANDS_H
#define TEMA2_COMMANDS_H
#include "TrainData.h"
#include <string>
#include <vector>
#include <unistd.h>

#define RESPONSE_LINE_BUFFER_SIZE 100
typedef enum
{
    RUTE = 0,
    SOSIRI,
    PLECARI,
    INTARZIERE,
    STATIE,
    QUIT,
    HELP,
    ERROR
} command_type;

using namespace std;

class Command{
protected:
    int fd;
    TrainData* data;
    void sendResponse(vector<string> response);
public:
    virtual void execute()=0;
    virtual command_type get_type()=0;
};

class RoutesCommand:public Command{
private:
    string location;
public:
    RoutesCommand(int fd,TrainData* data,string location);
    void execute() override;
    command_type get_type() override;
};


class ArrivalsCommand: public Command{
private:
    string location;
public:
    ArrivalsCommand(int fd,TrainData* data,string location);
    void execute() override;
    command_type get_type() override;
};

class DeparturesCommand:public Command{
private:
    string location;
public:
    DeparturesCommand(int fd,TrainData* data,string location);
    void execute() override;
    command_type get_type() override;
};

class DelayCommand:public Command{
private:
    int train_number;
    int delay;
public:
    DelayCommand(int fd, TrainData *data, int number, int delay);
    void execute() override;
    command_type get_type() override;
};

class QuitCommand:public Command{
private:
    fd_set* fds;
public:
    QuitCommand(int fd,TrainData* data,fd_set* fds);
    void execute() override;
    command_type get_type() override;
};

class ErrorCommand:public Command{
private:
    string message;
public:
    ErrorCommand(int fd,TrainData* data,string message);
    void execute() override;
    command_type get_type() override;
};


#endif //TEMA2_COMMANDS_H
