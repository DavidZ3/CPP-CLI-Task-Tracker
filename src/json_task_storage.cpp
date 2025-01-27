#include "json_task_storage.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "task.h"

const std::string unescapeQuotes(const std::string& s){
    std::string unescapedString;
    bool prevEsc = false;
    for(auto c : s){
        if(prevEsc && c == '\"'){
            unescapedString.pop_back();
        }
        if(c == '\\'){
            prevEsc = true;
        }else{
            prevEsc = false;
        }
        unescapedString += c;
    }
    return unescapedString;
}

JsonTaskStorage::JsonTaskStorage(const std::string& jsonPath)
    : jsonPath(jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.good()) {
        file.close();
        writeTasks(std::vector<Task>());
    }
};

void JsonTaskStorage::writeTasks(std::vector<Task> tasks) {
    std::ofstream file(jsonPath);
    file << "{\n\t\"tasks\": [\n";
    for (auto task : tasks) {
        file << "\t\t" << std::string(task);
    }
    file << "\t]\n}";
};

// THIS IS CURSED!!
// We're hardcode assumming the format since we control it but if anything
// formats it. Even if it's a new line we're done.
std::vector<Task> JsonTaskStorage::loadTasks() {
    std::ifstream file(jsonPath);
    std::vector<Task> tasks;
    std::string pattern =
    R"(\s*\{\s*)" // 1
    R"("id"\s*:\s*(\d+)\s*)" //2
    R"(,\s*"status"\s*:\s*(\d+)\s*)" // 3
    R"(,\s*"description"\s*:\s*"(.*?)\"\s*)" // 4
    R"(,\s*"createdAt"\s*:\s*(\d+)\s*)" // 5
    R"(,\s*"updatedAt"\s*:\s*(\d+)\s*\},?\s*)"; // 6

    std::regex taskRePattern(pattern);

    std::string line;
    while (std::getline(file, line)) {
        std::smatch matchs;
        if (std::regex_match(line, matchs, taskRePattern)) {
            Task t(std::stoi(matchs[1]), unescapeQuotes(matchs[3]),
                   (Status)std::stoi(matchs[2]), (time_t)std::stoi(matchs[4]),
                   (time_t)std::stoi(matchs[5]));
            tasks.push_back(t);
        }
    }
    return tasks;
}
