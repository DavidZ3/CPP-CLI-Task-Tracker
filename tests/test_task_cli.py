""" The API for the task-cli is as follows:
    # Adding a new task
    task-cli add "Buy groceries"
    # Output: Task added successfully (ID: 1)

    # Updating and deleting tasks
    task-cli update 1 "Buy groceries and cook dinner"
    task-cli delete 1

    # Marking a task as in progress or done
    task-cli mark-in-progress 1
    task-cli mark-done 1

    # Listing all tasks
    task-cli list

    # Listing tasks by status
    task-cli list done
    task-cli list todo
    task-cli list in-progress
"""
from enum import Enum
import os
import json
import unittest
import subprocess


class TaskStatus(Enum):
    """ Enum for task status.
    """
    TODO = 0
    IN_PROGRESS = 1
    DONE = 2

class TestTaskCLI(unittest.TestCase):
    """ Test class for the task-cli.
    """
    def setUp(self) -> None:
        if os.path.exists('tasks.json'):
            os.rename('tasks.json', 'tasks.json.bak')

    def tearDown(self) -> None:
        if os.path.exists('tasks.json'):
            os.remove('tasks.json')
        if os.path.exists('tasks.json.bak'):
            os.rename('tasks.json.bak', 'tasks.json')

    # Helper functions
    def _add_task(self, description: str) -> None:
        """ Helper function to add a task.
        """
        command = ['task_cli.exe', 'add', description]
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

    def _test_add(self, description: str, expected_task_num: int) -> None:
        """ Helper function to test adding tasks.
        """
        print(f'Adding task: {description}')
        self._add_task(description)

        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)

        print('Checking task in data')
        self.assertIn('tasks', data)

        print(f'Checking task number: {expected_task_num}')
        self.assertEqual(expected_task_num, len(data['tasks']))

        print(f'Checking task description: {description}')
        self.assertEqual(description, data['tasks'][-1]['description'])

        print('Checking task status: 0')
        self.assertEqual(0, data['tasks'][-1]['status'])

        print('Listing tasks:')
        command = ['task_cli.exe', 'list']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        print(f'Checking task in list: {description}')
        self.assertIn(description, p.stdout.decode(), 'Task not found in list')

    def _list_tasks(self) -> str:
        """ Helper function to list tasks.
        """
        command = ['task_cli.exe', 'list']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')
        return p.stdout.decode()

    # Test functions
    def test_empty(self) -> None:
        """ Test that the list command works with no tasks.
        """
        command = ['task_cli.exe', 'list']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)

        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)
        print(f'data: {data}')
        self.assertIn('tasks', data)

    def test_add_single_ascii(self) -> None:
        """ Test adding a single task with ascii characters.
        """
        self._test_add('task1 :D', 1)

    def test_add_multiple_ascii(self) -> None:
        """ Test adding multiple tasks with ascii characters.
        """
        self._test_add('task1', 1)
        self._test_add('', 2)
        self._test_add('Yet another task', 3)
        self._test_add('123321', 4)
        self._test_add('Yet another task', 5)
        self._test_add(r'Here is some ascii stuff: \r\n\tHi again!', 6)
        self._test_add(r'\0', 7)

    def test_update(self) -> None:
        """ Test updating a task.
        """
        self._add_task('Some Task Description')
        self._add_task('Another Task Description')
        self._add_task('Yet Another Task Description')

        print('Updating task 2')
        command = ['task_cli.exe', 'update', '1', 'An updated task description']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)

        print('Checking task in data')
        self.assertIn('tasks', data)

        print('Checking task number: 3')
        self.assertEqual(3, len(data['tasks']))


        print('Listing tasks:')
        command = ['task_cli.exe', 'list']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        print('Checking updated task 2 in list')
        self.assertEqual('An updated task description',
                         data['tasks'][1]['description'], 'Task not found in list')

        print('Checking task 2 not in list')
        self.assertNotEqual('Another Task Description',
                            data['tasks'][1]['description'], 'Task found in list')

    def test_mark_in_progress(self) -> None:
        """ Test marking a task in progress.
        """
        self._add_task('Some Task Description')

        print('Checking task is not in progress')
        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)
        self.assertEqual(TaskStatus.TODO.value, data['tasks'][0]['status'])

        task_list = self._list_tasks()
        self.assertIn('todo', task_list, 'Task not found in list')

        print('Marking task in progress')
        command = ['task_cli.exe', 'mark-in-progress', '0']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)

        print('Checking task in progress')
        self.assertEqual(TaskStatus.IN_PROGRESS.value, data['tasks'][0]['status'])

        task_list = self._list_tasks()
        self.assertIn('in-progress', task_list, 'Task not found in list')

    def test_mark_done(self) -> None:
        """ Test marking a task done.
        """
        self._add_task('Some Task Description')

        print('Checking task is not done')
        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)
        self.assertEqual(TaskStatus.TODO.value, data['tasks'][0]['status'])

        task_list = self._list_tasks()
        self.assertIn('todo', task_list, 'Task not found in list')

        print('Marking task done')
        command = ['task_cli.exe', 'mark-done', '0']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)

        print('Checking task done')
        self.assertEqual(TaskStatus.DONE.value, data['tasks'][0]['status'])

        task_list = self._list_tasks()
        self.assertIn('done', task_list, 'Task not found in list')

    def test_delete_task(self) -> None:
        """ Test deleting a task.
        """
        self._add_task('Some Task Description')
        self._add_task('Task Description 2')
        self._add_task('Yet Another Task Description')

        print('Deleting task 1')
        command = ['task_cli.exe', 'delete', '1']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)

        print('Checking task in data')
        self.assertIn('tasks', data)

        print('Checking task number: 2')
        self.assertEqual(2, len(data['tasks']))

        print('Listing tasks:')
        command = ['task_cli.exe', 'list']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)
        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')


        print('Checking task 1 not in list')
        self.assertNotIn('Task Description 2', p.stdout.decode(), 'Task found in list')
