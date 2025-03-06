#pragma once
#include <cstdint>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "task_manager.h"

class TaskCLI {
   public:
    TaskCLI(int argc, const char* const argv[]);
    bool getExitCode() const { return exitCode; };
    void processInput();

   private:
    void processAdd();
    void processDelete();
    void processList();
    void processMarkDone();
    void processMarkInProgress();
    void processUpdate();
    void printInvalidUsageMsg();
    int argc;
    const char* const* argv;
    uint8_t exitCode;
    std::unordered_map<std::string, std::function<void()>> commandToMethod;
    TaskManager manager;
};
