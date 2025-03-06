#include "task.h"

#include <ctime>
#include <string>

const std::string escapeQuotes(const std::string& s){
    std::string escapedString;
    for(auto c : s){
        if(c == '\"'){
            escapedString += "\\";
        }
        escapedString += c;
    }
    return escapedString;
}

Task::Task(Id id, std::string description)
    : id(id),
      description(std::move(description)),
      status(Status::todo),
      createdAt(std::time(nullptr)),
      updatedAt(createdAt) {};

Task::Task(Id id, std::string description, Status status): Task(id, std::move(description)){
    this->status = status;
}

Task::Task(Id id, std::string description, Status status, std::time_t createdAt,
           std::time_t updatedAt)
    : Task(id, std::move(description), status) {
    this->createdAt = createdAt;
    this->updatedAt = updatedAt;
};

void Task::updateUpdatedAtToNow() { this->updatedAt = std::time(nullptr); }

Task::operator std::string() const {
    std::string output = "{\"id\": " + std::to_string(id) +
                         ", \"status\": " + std::to_string(status) +
                         ", \"description\": \"" + escapeQuotes(description) + "\"" +
                         ", \"createdAt\": " + std::to_string(createdAt) +
                         ", \"updatedAt\": " + std::to_string(updatedAt) + "}";
    return output;
};

void Task::updateStatus(Status status) {
    this->updateUpdatedAtToNow();
    this->status = status;
}

void Task::updateDescription(std::string description) {
    this->updateUpdatedAtToNow();
    this->description = description;
}
