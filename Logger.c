//
// Created by Admin on 27.12.2022.
//

#include "Logger.h"
#include <stdio.h>
#include <windows.h>

static int GenAppDataLogFolder(){ //Creates the AppData Log Folder if it does not already exist.
    char path[MAX_PATH];
    sprintf(path, "C:\\Users\\%s\\AppData\\Local\\Temp\\MultiTools", getenv("USERNAME"));
    if (!CreateDirectory(path, NULL))
    {
        if (GetLastError() == 183){
            printf("Directory already exists.\n");
            return 1;
        }
        printf("Error creating directory: %lu\n", GetLastError());
        return 0;
    }
    return 1;
}

static void GenAppDataLogFile(char Message[]);
void LogMessage(char Message[]){ //Log the
    char *ParsedMessage = Message;
    GenAppDataLogFile(ParsedMessage);
}

static void Append_GenAppDataLogFile(char Message[]);
void LogMessageAppend(char Message[]){
    char *ParsedMessage = Message;
    Append_GenAppDataLogFile(ParsedMessage);
}

static void Append_GenAppDataLogFile(char Message[]){
    if (!GenAppDataLogFolder()){
        printf("Error, to create a log file, the MultiTools Folder is required...\n");
    }
    else{
        char _appdatapath[MAX_PATH];
        sprintf(_appdatapath, "C:\\Users\\%s\\AppData\\Local\\MultiTools", getenv("USERNAME"));
        char _appdatalogfilepath[MAX_PATH];
        sprintf(_appdatalogfilepath, "C:\\Users\\%s\\AppData\\Local\\MultiTools\\log.txt", getenv("USERNAME"));
        FILE *log_file;
        log_file = fopen(_appdatalogfilepath, "w");
        if(log_file == NULL)
        {
            printf("Something went wrong when trying to read the file.\n");
        }

        //Adding new line to last array position if it's not already '\n'
        char *ParsedMessage = Message;
        u_int len_ParsedMessage = strlen(ParsedMessage);
        if (ParsedMessage[len_ParsedMessage] != '\n'){
            ParsedMessage[len_ParsedMessage + 1] = '\n';
        }
        else{
            fputs(ParsedMessage, log_file);
            fclose(log_file);
        }
    }
}

static void GenAppDataLogFile(char Message[]){
    if (!GenAppDataLogFolder()){
        printf("Error, to create a log file, the MultiTools Folder is required...\n");
    }
    else{
        char _appdatapath[MAX_PATH];
        sprintf(_appdatapath, "C:\\Users\\%s\\AppData\\Local\\Temp\\MultiTools", getenv("USERNAME"));
        char _appdatalogfilepath[MAX_PATH];
        sprintf(_appdatalogfilepath, "C:\\Users\\%s\\AppData\\Local\\Temp\\MultiTools\\log.txt", getenv("USERNAME"));
        FILE *file;
        file = fopen(_appdatalogfilepath, "w");
        if(file == NULL)
        {
            printf("Something went wrong when trying to read the file.\n");

        }

        //Adding new line to last array position if it's not already '\n'
        char *ParsedMessage = Message;
        u_int len_ParsedMessage = strlen(ParsedMessage);
        if (ParsedMessage[len_ParsedMessage] != '\n'){
            ParsedMessage[len_ParsedMessage + 1] = '\n';
        }
        else{
            fputs(ParsedMessage, file);
            fclose(file);
        }
    }
}