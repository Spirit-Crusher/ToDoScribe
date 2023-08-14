### Description

ToDoScribe is a free and open-source CLI To-Do List manager.

### Features

- [x] Add, edit and remove tasks. Edits include changing name and priority level.
- [x] Show all current tasks by priority level.
- [ ] Save everything to a file so that tasks can be reloaded.


### To-dos

- [x] Create task sorting function.
- [x] Improve deletion/editing by enabling task selection making it so the user can interact with any task.
- [x] Display the total amount of tasks.
- [x] Show the task for deletion on the screen in the confirmation dialog.

### Bugs and code improvements

- [ ] Solve the need for having a buffer in basically every user input instance.
- [ ] Certain sized strings (8 chars+'\n') appear to interact poorly with the getTaskName function.
- [x] Only one task shows in the screen. It's some bug in the sortTasks function (pointers moment). Double pointers might be the solution.
- [x] The sortTasks function doesn't switch tasks properly (URGENT).
- [x] MemLeaks (apparently solved)

### Installation

You will need git and gcc installed for grabbing the source files and compiling the program, respectively.
Just follow these simple commands and you're good to go.

$ ```sudo {package manager install command} gcc git```

$ ```git clone https://github.com/Spirit-Crusher/ToDoScribe.git```

$ ```cd ToDoScribe```

$ ```gcc ToDoScribe.c -o ToDoScribe```
