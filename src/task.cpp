#include "task.h"

#include <ctime>

Task::Task(Id id, std::string description)
    : id(id),
      description(std::move(description)),
      status(Status::todo),
      createdAt(std::time(nullptr)),
      updatedAt(createdAt) {};

Task::Task(Id id, std::string description, Status status) {
    Task(id, description);
    this->status = status;
};

void Task::updateUpdatedAtToNow() { this->updatedAt = std::time(nullptr); }

void Task::updateStatus(Status status) {
    this->updateUpdatedAtToNow();
    this->status = status;
}

void Task::updateDescription(std::string description) {
    this->updateUpdatedAtToNow();
    this->description = description;
}