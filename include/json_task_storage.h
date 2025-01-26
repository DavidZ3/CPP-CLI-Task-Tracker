#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#include "task.h"
/*
There are multiple says of designing this.
We could either load everything into memory, use a cache, or only read what we
need and store sorted in allow for bin search. For simplicity, and because there
won't be that many tasks, we'll just keep everything in mem and sync where
needed.
*/

class JsonTaskStorage {
   public:
    JsonTaskStorage(const std::string& jsonPath);
    std::vector<Task> loadTasks();
    void writeTasks(std::vector<Task> tasks);

   private:
   std::string jsonPath;
};