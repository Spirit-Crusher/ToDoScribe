//     _____    ___      ___         _ _                 ___
//    |_   _|__|   \ ___/ __| __ _ _(_) |__  ___   ___  | _ )_  _   _ _ __ 
//     | |/ _ \ |) / _ \__ \/ _| '_| | '_ \/ -_) |___| | _ \ || | | '_/ _|
//    |_|\___/___/\___/___/\__|_| |_|_.__/\___|       |___/\_, | |_| \__|
//                                                           |__/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ToDoLib.h"

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

void optionMenuShow()
{
    printf("\n  (t) New task   (d) Delete   (e) Edit   (q) Quit\n");
}

Task* createNewTask(Task* taskList) 
{
    int nc, prio = 2;

    Task* newTask = (Task*) malloc(sizeof(Task));
    newTask->prioLvl = prio;
    newTask->nextTask = taskList;

    printf("New task: ");
    newTask->TaskName = getTaskName();

    printf("Priority Level (1-Critical   2-Normal [default]   3-Low): ");
    nc = scanf("%d", &prio);
    if (nc == 1) newTask->prioLvl = prio;
    
    return newTask;
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

Task* taskDelete(Task* taskList) 
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

int main() 
{
    Task* taskListStart = NULL;
    char keyPressed, buffer;
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
            scanf("%c", &buffer); //scanf sucks, see alternatives
            fflush(stdin);
            system("clear");
            taskListStart = createNewTask(taskListStart);
            break;
        case 'd':
            scanf("%c", &buffer); //scanf sucks, see alternatives
            fflush(stdin);
            system("clear");
            taskListStart = taskDelete(taskListStart);
            break;
        case 'e':
            scanf("%c", &buffer); //scanf sucks, see alternatives
            fflush(stdin);
            system("clear");
            //taskEdit();
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