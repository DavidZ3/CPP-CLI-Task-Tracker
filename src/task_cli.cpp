#include "task_cli.h"

#include <array>
#include <iostream>
#include <string>
#include <time.h>

#include "task_manager.h"
// This is some cooked stuff since localtime is not thread-safe BUT _s is only windows
// and _r is only linux. WTF!
#if defined(_MSC_VER)
#   define localtime_r(T,Tm) (localtime_s(Tm,T) ? NULL : Tm)
#endif

// Function to format a time_t value into a date or time string.
std::string DateTime(time_t time, const char* format) {
    char buffer[90];
    struct tm timeinfo;
    localtime_r(&time, &timeinfo);
    strftime(buffer, sizeof(buffer), format, &timeinfo);
    return buffer;
}

TaskCLI::TaskCLI(int argc, const char* const argv[])
    : argc(argc), argv(argv), exitCode(0), manager("./tasks.json") {
    commandToMethod = {
        {"add", std::bind(&TaskCLI::processAdd, this)},
        {"delete", std::bind(&TaskCLI::processDelete, this)},
        {"list", std::bind(&TaskCLI::processList, this)},
        {"mark-done", std::bind(&TaskCLI::processMarkDone, this)},
        {"mark-in-progress", std::bind(&TaskCLI::processMarkInProgress, this)},
        {"update", std::bind(&TaskCLI::processUpdate, this)}};
}

void TaskCLI::printInvalidUsageMsg() {
    std::cerr << "Invalid Usage. Usage:\n"
              << "\t" << argv[0] << " add <description>\n"
              << "\t" << argv[0] << " delete <id>\n"
              << "\t" << argv[0] << " list [todo | in-progress | done]\n"
              << "\t" << argv[0] << " mark-done <id>\n"
              << "\t" << argv[0] << " mark-in-progress <id>\n"
              << "\t" << argv[0] << " update <id> <description>\n";
}

void TaskCLI::processInput() {
    if (argc < 2) {
        printInvalidUsageMsg();
        exitCode = 1;
    }
    if (commandToMethod.count(argv[1])) {
        commandToMethod.at(argv[1])();
    } else {
        printInvalidUsageMsg();
        exitCode = 1;
    }
};

bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void TaskCLI::processAdd() {
    if (argc != 3) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }

    std::string s(argv[2]);
    Id id = manager.addTask(s);
    std::cout << "Task added successfully (ID: " << id << ")" << "\n";
}

void TaskCLI::processDelete() {
    if (argc != 3) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }

    std::string idStr(argv[2]);
    if (!is_number(idStr)) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }
    Id id(std::stoi(idStr));
    manager.removeTask(id);
}

void TaskCLI::processList() {
    std::vector<Task> tasks;
    if (argc >= 3) {
        Status status;
        std::string statusStr(argv[2]);
        if (!statusStr.compare("todo")) {
            status = Status::todo;
        } else if (!statusStr.compare("in-progress")) {
            status = Status::inprogress;
        } else if (!statusStr.compare("done")) {
            status = Status::done;
        } else {
            exitCode = 1;
            printInvalidUsageMsg();
            return;
        }
        tasks = manager.getTasksByStatus(status);
    } else {
        tasks = manager.getTasks();
    }
    std::array<std::string, 3> statusArray = {"todo", "in-progress", "done"};
    for (auto t : tasks) {
        const char* format = "%Y-%m-%d %H:%M:%S";
        std::string dateCreatedStr = DateTime(t.getCreatedAt(), format);
        std::string dateUpdatedStr = DateTime(t.getUpdatedAt(), format);
        std::cout << "Id: " << t.getId()
                  << "\tDescription: \"" << t.getDescription() << "\"\n"
                  << "\tStatus: " << statusArray[t.getStatus()] << "\n"
                  << "\tDate Created: " << dateCreatedStr << "\n"
                  << "\tDate Updated: " << dateUpdatedStr << "\n\n";
    }
}

void TaskCLI::processMarkDone() {
    if (argc != 3) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }

    std::string idStr(argv[2]);
    if (!is_number(idStr)) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }
    Id id(std::stoi(idStr));
    manager.updateTaskStatus(id, Status::done);
}

void TaskCLI::processMarkInProgress() {
    if (argc != 3) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }

    std::string idStr(argv[2]);
    if (!is_number(idStr)) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }
    Id id(std::stoi(idStr));
    manager.updateTaskStatus(id, Status::inprogress);
}

void TaskCLI::processUpdate() {
    if (argc != 4) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }

    std::string idStr(argv[2]);
    if (!is_number(idStr)) {
        exitCode = 1;
        printInvalidUsageMsg();
        return;
    }

    Id id(std::stoi(idStr));
    manager.updateTaskDescription(id, std::string(argv[3]));
}

int main(int argc, char* argv[]) {
    TaskCLI t(argc, argv);
    t.processInput();
    return t.getExitCode();
}
