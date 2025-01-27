#pragma once
#include <iostream>
#include <vector>

#include "json_task_storage.h"
#include "task.h"

class TaskManager {
   public:
    TaskManager(const std::string& jsonPath);
    Id addTask(const std::string& description);
    bool removeTask(Id id);
    bool updateTaskDescription(Id id, const std::string& description);
    bool updateTaskStatus(Id id, Status status);

   private:
    Id nextId;
    std::vector<Task> tasks;
    JsonTaskStorage storage;
    std::vector<Task>::iterator locateTaskById(Id id);
};
