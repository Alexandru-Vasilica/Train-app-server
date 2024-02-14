//
// Created by alexv on 12/3/23.
//

#include "CommandQueue.h"

void CommandQueue::add_command(Command* command) {
    if(command->get_type()==ESTIMARE || command->get_type()==INTARZIERE){
        commands.emplace(command);
    }else { commands.push(command); }
}

Command *CommandQueue::pop_command() {
    if(commands.empty()){
        return nullptr;
    }
    Command* output=commands.front();
    commands.pop();
    return output;
}

bool CommandQueue::empty() {
    return commands.empty();
}


