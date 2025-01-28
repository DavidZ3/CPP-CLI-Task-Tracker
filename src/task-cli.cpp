#include "task-cli.h"

#include <iostream>

#include "task_manager.h"

TaskCLI::TaskCLI(int argc, const char* const argv[])
    : argc(argc), argv(argv), exitCode(0), manager("./tasks.json") {
    commandToMethod = {
        {"add", std::bind(&TaskCLI::processAdd, this)},
        {"delete", std::bind(&TaskCLI::processDelete, this)},
        {"list", std::bind(&TaskCLI::processList, this)},
        {"mark-done", std::bind(&TaskCLI::processMarkDone, this)},
        {"mark-in-progress", std::bind(&TaskCLI::processMarkInProgress, this)},
        {"update", std::bind(&TaskCLI::processUpdate, this)}};
}

void TaskCLI::printInvalidUsageMsg() {
    std::cerr << "Invalid Usage. Usage:\n"
              << "\t" << argv[0] << " add <description>\n"
              << "\t" << argv[0] << " delete <id>\n"
              << "\t" << argv[0] << " list [todo | in-progress | done]\n"
              << "\t" << argv[0] << " mark-done <id>\n"
              << "\t" << argv[0] << " mark-in-progress <id>\n"
              << "\t" << argv[0] << " update <id> <description>\n";
}

void TaskCLI::processInput() {
    if(argc < 2){
        printInvalidUsageMsg();
        exitCode = 1;
    }
    if(commandToMethod.count(argv[1])){
        commandToMethod.at(argv[1])();
    }else{
        printInvalidUsageMsg();
        exitCode = 1;
    }
};

void TaskCLI::processAdd() {
}

void TaskCLI::processDelete() {}

void TaskCLI::processList() {}

void TaskCLI::processMarkDone() {}

void TaskCLI::processMarkInProgress() {}

void TaskCLI::processUpdate() {}

int main(int argc, char* argv[]) {
    TaskCLI t(argc, argv);
    t.processInput();
    return t.getExitCode();
}
