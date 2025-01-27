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
    std::vector<Task> getTasks() const {return tasks;};
    std::vector<Task> getTasksByStatus(Status status) const;

   private:
    Id nextId;
    std::vector<Task> tasks;
    JsonTaskStorage storage;
    std::vector<Task>::iterator locateTaskById(Id id);
};
