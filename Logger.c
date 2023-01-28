//
// Created by Admin on 27.12.2022.
//

#include "Logger.h"
#include <stdio.h>
#include <windows.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

char* GetTime();

static int GenAppDataLogFolder(char Path[]){ //Creates the AppData Log Folder if it does not already exist.
    char path[MAX_PATH];
    path[MAX_PATH] = (char) Path;
    sprintf(path, "C:\\Users\\%s\\AppData\\Local\\MultiTools", getenv("USERNAME"));
    DIR* dir = opendir(path);
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        CreateDirectory(path, NULL);
        LogMessageAppend("Created Directory.");
    } else {
        if (GetLastError() == ERROR_ALREADY_EXISTS){
            LogMessageAppend("Directory already exists.");
        }
        else if (GetLastError() == ERROR_PATH_NOT_FOUND){
            LogMessageAppend("Directory Path not found.");
        }
        else{
            DWORD ERR = GetLastError();
            char * err_msg;
            sprintf(err_msg, "%lu", ERR);
            LogMessageAppend(err_msg);
        }
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

static void Append_GenAppDataLogFile(char Message[]){ //Creates AppData Log FILE, Message will be appended to EOF
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
        //TODO: Fix file now writing to log
        char * ParsedMessageWithTimestamp;
        ParsedMessageWithTimestamp = GetTime();
        strcat(ParsedMessageWithTimestamp, ParsedMessage);
        fputs(ParsedMessageWithTimestamp, log_file);
        fclose(log_file);
    }
}

char * GetTime(){
    SYSTEMTIME t;
    GetLocalTime(&t);
    char *time;
    time = malloc(sizeof(char) * 8);
    sprintf(time, "%d:%d:%d:%d", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
    return time;
}

static void GenAppDataLogFile(char Message[]){

    char _appdatapath[MAX_PATH];
    sprintf(_appdatapath, "C:\\Users\\%s\\AppData\\Local\\Temp\\MultiTools", getenv("USERNAME"));
    char _appdatalogfilepath[MAX_PATH];
    sprintf(_appdatalogfilepath, "C:\\Users\\%s\\AppData\\Local\\Temp\\MultiTools\\log.txt", getenv("USERNAME"));

    if (!GenAppDataLogFolder(_appdatapath)){
        printf("Error, to create a log file, the MultiTools Folder is required...\n");
    }
    else{
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