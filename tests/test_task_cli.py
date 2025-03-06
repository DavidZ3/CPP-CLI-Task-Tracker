import os
import json
import unittest
import subprocess

class TestTaskCLI(unittest.TestCase):
    def setUp(self) -> None:
        if os.path.exists('tasks.json'):
            os.rename('tasks.json', 'tasks.json.bak')

    def tearDown(self) -> None:
        if os.path.exists('tasks.json'):
            os.remove('tasks.json')
        if os.path.exists('tasks.json.bak'):
            os.rename('tasks.json.bak', 'tasks.json')

    def testEmpty(self) -> None:
        command = ['task_cli.exe', 'list']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)

        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)
        print(f'data: {data}')
        self.assertIn('tasks', data)

    def testAdd(self) -> None:
        command = ['task_cli.exe', 'add', 'task1']
        print(f'Running command: {command}')
        p = subprocess.run(command, check=True, capture_output=True)

        print(f'stderr: {p.stderr.decode()}')
        print(f'stdout: {p.stdout.decode()}')

        with open('tasks.json', 'r', encoding='utf-8') as f:
            data = json.load(f)

        print(f'data: {data}')
        self.assertIn('tasks', data)
        self.assertEqual(0, data['tasks'][0]['id'])
        self.assertEqual('task1', data['tasks'][0]['description'])
        self.assertEqual(0, data['tasks'][0]['status'])
