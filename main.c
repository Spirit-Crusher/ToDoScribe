//     _____    ___      ___         _ _                 ___
//    |_   _|__|   \ ___/ __| __ _ _(_) |__  ___   ___  | _ )_  _   _ _ __ 
//      | |/ _ \ |) / _ \__ \/ _| '_| | '_ \/ -_) |___| | _ \ || | | '_/ _|
//     |_|\___/___/\___/___/\__|_| |_|_.__/\___|       |___/\_, | |_| \__|
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
    printf("\n\n");
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

/* void sortTasks(Task** taskListStart)
{
    bool switched, first;
    Task **currentTask, **followingTask;
    Task **tempTask;

    do
    {
        switched = false;
        first = true;   //signals if its the first inside loop iteration
        currentTask = taskListStart;
        followingTask = &(*taskListStart)->nextTask;
        while (*followingTask != NULL)
        {
            if((*currentTask)->prioLvl > (*followingTask)->prioLvl)
            {
                (*currentTask)->nextTask = (*followingTask)->nextTask;
                if(*followingTask != NULL) (*followingTask)->nextTask = *currentTask;
                if(!first && *tempTask != NULL) (*tempTask)->nextTask = *followingTask;
            }
            tempTask = currentTask;
            currentTask = followingTask;
            if(*followingTask != NULL) followingTask = &(*followingTask)->nextTask;
            first = false;
        }
    } while(switched);
} */

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
    int nc, prio = 2;

    Task* newTask = (Task*) malloc(sizeof(Task));
    newTask->prioLvl = prio;

    printf("New task: ");
    newTask->TaskName = getTaskName();

    printf("Priority Level (1-Critical   2-Normal [default]   3-Low): ");
    nc = scanf("%d", &prio);
    if (nc == 1) newTask->prioLvl = prio;

    newTask->nextTask = taskList;
    taskList = newTask;

    totalTasks++;
    
    return taskList;
}

void showCurrentTasks(Task* taskList)
{
    int currentPrio, taskNumber = 1;
    Task* currentTask;

    for(currentPrio = 1; currentPrio <= 3; currentPrio++)
    {
        currentTask = taskList;
        while (currentTask != NULL)
        {
            if(currentTask->prioLvl == currentPrio)
            {
                printf(" (%d) ", taskNumber);
                taskNumber++;
                printTask(currentTask);
            }
            currentTask = currentTask->nextTask;
        }
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
    Task* taskForDeletion = taskList, *currentTask = taskList/* , *previousTask = NULL */;
    char confirmation = 'n';
/*     char* providedName;

    showCurrentTasks(taskList);
    printf("Which task do you want to delete? (name)\n");
    providedName = getTaskName();

    while (currentTask != NULL)
    {
        if(!strcmp(currentTask->TaskName, providedName))
        {
            taskForDeletion = currentTask;
            break;
        }
        else currentTask = currentTask->nextTask;

        if(currentTask == NULL)
        {
            printf("\n[WARNING] No task with provided name\n");
            return taskList;
        }
    } */
    
    printTask(taskForDeletion);

    printf("Are you sure you want to delete this task? (y/N): ");
    confirmation = getchar();
    if (confirmation == 'y' || confirmation == 'Y')
    {
        taskList = taskList->nextTask;
        free(taskForDeletion);
        totalTasks--;
    }

    return taskList;
}

void editTask(Task* taskList)
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
        showTotalTasks();
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