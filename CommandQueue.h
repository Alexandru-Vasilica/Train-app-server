//
// Created by alexv on 12/3/23.
//

#ifndef TEMA2_COMMANDQUEUE_H
#define TEMA2_COMMANDQUEUE_H

#include <queue>
#include "Commands.h"

class CommandQueue {
private:
    queue<Command*> commands;
public:
    void add_command(Command* command);
    Command* pop_command();
};


#endif //TEMA2_COMMANDQUEUE_H
