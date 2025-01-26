#pragma once
#include <ctime>
#include <iostream>
enum Status : char { todo, inprogress, done };
typedef std::uint16_t Id;

class Task {
   public:
    Task(Id id, std::string description);
    Task(Id id, std::string description, Status status);
    Task(Id id, std::string description, Status status, std::time_t createdAt, std::time_t updatedAt);

   private:
    Id id;
    Status status;
    std::string description;
    std::time_t createdAt;
    std::time_t updatedAt;
    void updateUpdatedAtToNow();

   public:
    operator std::string () const;
    Id getId() { return this->id; }
    Status getStatus() { return this->status; }
    std::string getDescription() { return this->description; }
    std::time_t getCreatedAt() { return this->createdAt; }
    std::time_t getUpdatedAt() { return this->updatedAt; }
    void updateStatus(Status status);
    void updateDescription(std::string description);
};
