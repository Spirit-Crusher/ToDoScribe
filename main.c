//     _____    ___      ___         _ _                 ___
//    |_   _|__|   \ ___/ __| __ _ _(_) |__  ___   ___  | _ )_  _   _ _ __ 
//      | |/ _ \ |) / _ \__ \/ _| '_| | '_ \/ -_) |___| | _ \ || | | '_/ _|
//      |_|\___/___/\___/___/\__|_| |_|_.__/\___|       |___/\_, | |_| \__|
//                                                           |__/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "ToDoLib.h"

int totalTasks = 0;

void welcomeScreen()
{
    printf(" _____    ___      ___         _ _                 ___\n");
    printf("|_   _|__|   \\ ___/ __| __ _ _(_) |__  ___   ___  | _ )_  _   _ _ __ \n");
    printf("  | |/ _ \\ |) / _ \\__ \\/ _| '_| | '_ \\/ -_) |___| | _ \\ || | | '_/ _|\n");
    printf("  |_|\\___/___/\\___/___/\\__|_| |_|_.__/\\___|       |___/\\_, | |_| \\__|\n");
    printf("                                                       |__/\n\n");
}

void stdinCleaner()
{
    char buffer;
    scanf("%c", &buffer); //scanf sucks, see alternatives
}

void printTask (Task* task)
{
    printf("Task: %s     Priority: ", task->TaskName);
    switch (task->prioLvl)
    {
    case 1:
        printf("Critical\n");
        break;
    case 2:
        printf("Normal\n");
        break;
    case 3:
        printf("Low\n");
        break;
    }
    printf("\n");
}

char* getTaskName() 
{
    char* input = NULL;
    char buffer[CHUNK];
    size_t inputLen = 0, bufferLen = 0;

    do
    {
        if(fgets(buffer, CHUNK, stdin) == NULL)
        {
            printf("[ERROR] Input malfunction\n");
            sleep(2);
            exit(0);
        } 
        bufferLen = strlen(buffer);
        input = realloc(input, inputLen+bufferLen+1);
        strcat(input, buffer);
        inputLen += bufferLen;
    } while(bufferLen == CHUNK-1 && buffer[bufferLen-2] != '\n');

    return input;
}

Task* swapTasks(Task* Task1, Task* Task2)
{
    Task* temp = Task2->nextTask;
    Task2->nextTask = Task1;
    Task1->nextTask = temp;
    return Task2;
}

void sortTasks(Task** head)
{
    Task** currentTask;
    int i, j;
    bool swapped;
 
    for (i = 0; i <= totalTasks; i++) {
 
        currentTask = head;
        swapped = false;
 
        for (j = 0; j < totalTasks - i - 1; j++) {
 
            Task* Task1 = *currentTask;
            Task* Task2 = Task1->nextTask;
 
            if (Task1->prioLvl > Task2->prioLvl) {
 
                /* update the link after swapping */
                *currentTask = swapTasks(Task1, Task2);
                swapped = true;
            }
 
            currentTask = &(*currentTask)->nextTask;
        }
        /* break if the loop ended without any swap */
        if (!swapped)
            break;
    }
}

void optionMenuShow()
{
    printf("\n  (t) New task   (d) Delete   (e) Edit   (q) Quit\n");
}

void showTotalTasks()
{
    printf("Total Tasks: %d\n\n", totalTasks);
}

Task* createTask(Task* taskList) 
{
    char prio = '2';

    Task* newTask = (Task*) malloc(sizeof(Task));
    newTask->prioLvl = prio-'0';

    printf("New task: ");
    newTask->TaskName = getTaskName();

    printf("Priority Level (1-Critical   2-Normal [default]   3-Low): ");
    prio = getchar();

    switch (prio)
    {
    case '1':
        newTask->prioLvl = prio-'0';    //converting char to int is needed (prio-'0) achieves it
        break;
    case '2':
        newTask->prioLvl = prio-'0';
        break;
    case '3':
        newTask->prioLvl = prio-'0';
        break;
    default:
        break;
    }

    newTask->nextTask = taskList;
    taskList = newTask;

    totalTasks++;
    
    return taskList;
}

void showCurrentTasks(Task* taskList)
{
    int taskNumber;
    Task* currentTask = taskList;

    for (taskNumber = 1; currentTask != NULL; taskNumber++)
    {
        printf(" (%d) ", taskNumber);
        printTask(currentTask);
        currentTask = currentTask->nextTask;
    }
}

void memoryRelease(Task* taskList)
{
    Task* auxPointer;
    while (taskList != NULL)
    {
        auxPointer = taskList;
        taskList = taskList->nextTask;
        free(auxPointer);
    }
}

void removeTaskFromList(Task** head, int taskNumber)
{
    Task** selectedTask = head;

    if((*selectedTask)->nextTask == NULL)
    { 
        free(*selectedTask);
        *head = NULL;
    }
    else
    {

        for (int i = 1; i < taskNumber-1; i++) selectedTask = &(*selectedTask)->nextTask;
        
        Task *previousTask, *removedTask;

        previousTask = *selectedTask;
        removedTask = previousTask->nextTask;

        previousTask->nextTask = removedTask->nextTask;
        free(removedTask);
    }
    totalTasks--;
}

void deleteTask(Task** head)
{
    Task* taskForDeletion = *head;
    char confirmation = 'n';
    int selectedNumber, nc;

    showCurrentTasks(*head);
    printf("Which task do you want to delete? (task number): ");
    nc = scanf("%d", &selectedNumber);
    printf("\n");
    if(nc != 1)
    {
        printf("\n[WARNING] Unknown task\n");
        sleep(2);
    }
    else if (selectedNumber > totalTasks || selectedNumber < 1)
    {
        printf("\n[WARNING] Unknown task\n");
        sleep(2);
    } 
    else
    {
        for (int i = 1; i < selectedNumber; i++) taskForDeletion = taskForDeletion->nextTask;
        
        printTask(taskForDeletion);

        printf("Are you sure you want to delete this task? (y/N): ");
        stdinCleaner();
        confirmation = getchar();
        if (confirmation == 'y' || confirmation == 'Y') removeTaskFromList(head, selectedNumber);
    }
}

void editTask(Task** head)
{
    Task** selectedTask = head;
    Task* taskForEditing = *head;
    int selectedNumber, nc, prio;
    char option;

    showCurrentTasks(*head);
    printf("Which task do you want to edit? (task number): ");
    nc = scanf("%d", &selectedNumber);
    printf("\n");
    if(nc != 1) printf("\n[WARNING] Unknown task\n");
    else if (selectedNumber > totalTasks || selectedNumber < 1) printf("\n[WARNING] Unknown task\n");
    else
    {
        for (int i = 1; i < selectedNumber; i++) selectedTask = &(*selectedTask)->nextTask;
        taskForEditing = *selectedTask;

        printTask(taskForEditing);
        printf("(1) Edit name   (2) Edit priority   (3) Back\n");
        stdinCleaner();
        option = getchar();
        switch (option)
        {
        case '1':
            printf("New name: ");
            stdinCleaner();
            taskForEditing->TaskName = getTaskName();
            break;
        case '2':
            printf("\nNew priority level: ");
            nc = scanf(" %d", &prio);
            if (nc == 1) taskForEditing->prioLvl = prio;
            break;
        default:
            break;
        }
    }
}

/* void editTask(Task* taskList)
{
    int nc, prio;
    char option;
    Task* taskForEditing = taskList;       //atm you can only edit the last task

    printTask(taskForEditing);

    printf("(1) Edit name   (2) Edit priority   (3) Back\n");
    option = getchar();
    switch (option)
    {
    case '1':
        printf("New name: ");
        stdinCleaner();
        taskForEditing->TaskName = getTaskName();
        break;
    case '2':
        printf("\nNew priority level: ");
        nc = scanf(" %d", &prio);
        if (nc == 1) taskForEditing->prioLvl = prio;
        break;
    default:
        break;
    }
} */



int main() 
{
    Task* taskListStart = NULL;
    char keyPressed;
    bool exitApp = false;

    while(!exitApp)
    {
        system("clear");
        welcomeScreen();
        showTotalTasks();
        sortTasks(&taskListStart);
        showCurrentTasks(taskListStart);
        optionMenuShow();

        keyPressed = getchar();
        switch (keyPressed)
        {
        case 't':
            stdinCleaner();
            system("clear");
            taskListStart = createTask(taskListStart);
            break;
        case 'd':
            stdinCleaner();
            system("clear");
            if(taskListStart == NULL)
            {
                printf("[INFO] No tasks to be deleted.\n");
                sleep(2);
                break;
            }
            deleteTask(&taskListStart);
            break;
        case 'e':
            stdinCleaner();
            system("clear");
            editTask(&taskListStart);
            break;
        case 'q':
            system("clear");
            exitApp = true;
            break;
        default:
            break;
        }
    }

    memoryRelease(taskListStart);

    return 0;
}