# ✅ CPP-CLI Task Tracker

A simple and lightweight **Command Line Interface (CLI)** task tracker built in **C++**, powered by **CMake**, and thoroughly tested using **Python**. Track your tasks effortlessly from the terminal.

> 🛠️ Project spec by [roadmap.sh/projects/task-tracker](https://roadmap.sh/projects/task-tracker)

![Build](https://github.com/DavidZ3/CPP-CLI-Task-Tracker/actions/workflows/cmake-multi-platform.yml/badge.svg)
![Lint](https://github.com/DavidZ3/CPP-CLI-Task-Tracker/actions/workflows/pylint.yml/badge.svg)

---

## 🚀 Features

- 📋 Add, update, delete tasks
- 📌 Track status: `todo`, `in-progress`, `done`
- 📃 List tasks by status
- 🕒 Tracks when tasks are created and updated
- ⚡ Simple, fast, and easy to use

---

## 📸 Example

![Demo](https://github.com/user-attachments/assets/9d720fd7-6847-4aa7-9511-e56f27965653)

---

## 💻 Usage
- task_cli.exe add <description>
- task_cli.exe delete <id>
- task_cli.exe list [todo | in-progress | done]
- task_cli.exe mark-done <id>
- task_cli.exe mark-in-progress <id>
- task_cli.exe update <id> <description>

---

## 📦 Dependencies

| Tool         | Requirement          |
|--------------|----------------------|
| **CMake**    | >= 3.10              |
| **Compiler** | C++14 support        |
| **Python**   | >= 3.7 (for testing) |

---

## 🏗️ Building the Project

```
cmake --build build --config Release
```

## 🧪 Running Tests
```
python -m unittest tests/test_task_cli.py -v
```
🧾 Test coverage includes:
* ✅ Add/update/delete tasks
* 📄 List tasks by status
* 🔄 Status transitions (todo, in-progress, done)
* 🛡️ Safe state handling with tasks.json backup
