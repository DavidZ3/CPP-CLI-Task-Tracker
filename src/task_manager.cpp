#include "task_manager.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "json_task_storage.h"
#include "task.h"
TaskManager::TaskManager(const std::string& jsonPath) : storage(jsonPath) {
    JsonTaskStorage storage(jsonPath);
    tasks = storage.loadTasks();
    auto maxId = std::max_element(
        tasks.begin(), tasks.end(), [](Task const& lTask, Task const& rTask) {
            return (int)lTask.getId() < (int)rTask.getId();
        });
    if (maxId != tasks.end()){
        nextId = (Id)maxId->getId() + 1;
    }else{
        nextId = 0;
    }
};

Id TaskManager::addTask(const std::string& description) {
    tasks.push_back(Task(nextId, std::move(description)));
    storage.writeTasks(tasks);
    nextId += 1;
    return nextId - 1;
}

std::vector<Task>::iterator TaskManager::locateTaskById(Id id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [id](const Task& t) { return t.getId() == id; });
    return it;
}

// Returns true if element found else false
bool TaskManager::removeTask(Id id) {
    auto it = locateTaskById(id);
    if (it == tasks.end()) {
        return false;
    }
    tasks.erase(it);
    storage.writeTasks(tasks);
    return true;
}

// Returns true if element found else false
bool TaskManager::updateTaskDescription(Id id, const std::string& description) {
    auto it = locateTaskById(id);
    if (it == tasks.end()) {
        return false;
    }
    it->updateDescription(std::move(description));
    storage.writeTasks(tasks);
    return true;
}

// Returns true if element found else false
bool TaskManager::updateTaskStatus(Id id, Status status) {
    auto it = locateTaskById(id);
    if (it == tasks.end()) {
        return false;
    }
    it->updateStatus(status);
    storage.writeTasks(tasks);
    return true;
}

std::vector<Task> TaskManager::getTasksByStatus(Status status) const {
    std::vector<Task> tasksByStatus;
    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(tasksByStatus),
                 [status](const Task& t) { return t.getStatus() == status; });
    return tasksByStatus;
}
