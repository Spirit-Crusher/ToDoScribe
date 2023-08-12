//     _____    ___      ___         _ _                 ___
//    |_   _|__|   \ ___/ __| __ _ _(_) |__  ___   ___  | _ )_  _   _ _ __ 
//     | |/ _ \ |) / _ \__ \/ _| '_| | '_ \/ -_) |___| | _ \ || | | '_/ _|
//    |_|\___/___/\___/___/\__|_| |_|_.__/\___|       |___/\_, | |_| \__|
//                                                           |__/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "ToDoLib.h"


void stdinCleaner()
{
    char buffer;
    scanf("%c", &buffer); //scanf sucks, see alternatives
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
            exit(0);
        } 
        bufferLen = strlen(buffer);
        input = realloc(input, inputLen+bufferLen+1);
        strcat(input, buffer);
        inputLen += bufferLen;
    } while(bufferLen == CHUNK-1 && buffer[bufferLen-2] != '\n');

    return input;
}

void welcomeScreen()
{
    printf(" _____    ___      ___         _ _                 ___\n");
    printf("|_   _|__|   \\ ___/ __| __ _ _(_) |__  ___   ___  | _ )_  _   _ _ __ \n");
    printf("  | |/ _ \\ |) / _ \\__ \\/ _| '_| | '_ \\/ -_) |___| | _ \\ || | | '_/ _|\n");
    printf("  |_|\\___/___/\\___/___/\\__|_| |_|_.__/\\___|       |___/\\_, | |_| \\__|\n");
    printf("                                                       |__/\n\n");
}

Task* taskSort(Task** currentTask, Task* taskList)
{
    if(taskList != NULL && taskList->prioLvl < (*currentTask)->prioLvl) //high value => low prio
    {
        (*currentTask)->nextTask = taskList->nextTask;
        taskList->nextTask = *currentTask;
    }
    else
    {
        (*currentTask)->nextTask = taskList;
        taskList = *currentTask;
    }

    return taskList;
}

void optionMenuShow()
{
    printf("\n  (t) New task   (d) Delete   (e) Edit   (q) Quit\n");
}

Task* createTask(Task* taskList) 
{
    int nc, prio = 2;

    Task* newTask = (Task*) malloc(sizeof(Task));
    newTask->prioLvl = prio;

    printf("New task: ");
    newTask->TaskName = getTaskName();

    printf("Priority Level (1-Critical   2-Normal [default]   3-Low): ");
    nc = scanf("%d", &prio);
    if (nc == 1) newTask->prioLvl = prio;

    taskList = taskSort(&newTask, taskList);
    
    return taskList;
}

void showCurrentTasks(Task* taskList)
{
    while (taskList != NULL)
    {
        printf("Task: %sPriority: ", taskList->TaskName);
        switch (taskList->prioLvl)
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
        printf("\n\n");
        taskList = taskList->nextTask;
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

Task* deleteTask(Task* taskList) 
{
    Task* taskForDeletion;
    char confirmation = 'n';

    printf("Are you sure you want to delete this task? (y/N): ");
    confirmation = getchar();
    if (confirmation == 'y' || confirmation == 'Y')
    {
        taskForDeletion = taskList;    //atm it only deletes the last created task
        taskList = taskList->nextTask;
        free(taskForDeletion);
    }

    return taskList;
}

void editTask(Task* taskList)
{
    int nc, prio;
    char option;

    //show the task that is being edited
    printf("(1) Edit name   (2) Edit priority   (3) Back\n");
    option = getchar();
    switch (option)
    {
    case '1':
        printf("New name: ");
        stdinCleaner();
        taskList->TaskName = getTaskName();
        break;
    case '2':
        printf("\nNew priority level: ");
        nc = scanf(" %d", &prio);
        if (nc == 1) taskList->prioLvl = prio;
        break;
    default:
        break;
    }
}



int main() 
{
    Task* taskListStart = NULL;
    char keyPressed;
    bool exitApp = false;

    while(!exitApp)
    {
        system("clear");
        welcomeScreen();
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
            taskListStart = deleteTask(taskListStart);
            break;
        case 'e':
            stdinCleaner();
            system("clear");
            editTask(taskListStart);
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