#pragma once
#include <cstdint>
#include <ctime>
#include <iostream>
enum Status : char { todo, inprogress, done };
typedef std::uint16_t Id;

class Task {
   public:
    Task(Id id, std::string description);
    Task(Id id, std::string description, Status status);
    Task(Id id, std::string description, Status status, std::time_t createdAt,
         std::time_t updatedAt);

   private:
    Id id;
    Status status;
    std::string description;
    std::time_t createdAt;
    std::time_t updatedAt;
    void updateUpdatedAtToNow();

   public:
    operator std::string() const;
    Id getId() const { return this->id; }
    Status getStatus() const { return this->status; }
    std::string getDescription() const { return this->description; }
    std::time_t getCreatedAt() const { return this->createdAt; }
    std::time_t getUpdatedAt() const { return this->updatedAt; }
    void updateStatus(Status status);
    void updateDescription(std::string description);
};
