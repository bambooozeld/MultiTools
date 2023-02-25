//
// Created by Admin on 27.12.2022.
//

#include "Logger.h"
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>


struct FunctionCodes {
    int fc_DirectoryExists;
    int fc_DebugDirectory;
    int fc_GenAppDataLogFolder;
    int fc_GetTime;
    int fc_LogMessage;
    int fc_LogMessageP;
};

struct globals {
    bool vdirectoryExists;
    bool vdirectoryErrors;
};

void SetupFunctionCodes(void) {
    struct FunctionCodes FunctionCodes;
    struct FunctionCodes *fc = &FunctionCodes;
    fc->fc_DirectoryExists = 0x50;
    fc->fc_DebugDirectory = 0x51;
    fc->fc_GenAppDataLogFolder = 0x52;
    fc->fc_GetTime = 0x53;
    fc->fc_LogMessage = 0x54;
    fc->fc_LogMessageP = 0x55;
}


static bool DirectoryExists(LPCTSTR szPath)
{
    struct globals structglobals;
    struct globals *globals = &structglobals;
    DWORD dwAttrib = GetFileAttributes(szPath);
    if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
        LogMessage("Error at function 0x50: INVALID_FILE_ATTRIBUTES");
        globals->vdirectoryExists = false;
        return false;
    }
    else if (dwAttrib == FILE_ATTRIBUTE_DIRECTORY){
        globals->vdirectoryExists = true;
        return true;
    }
    else{
        char * msg = NULL;
        sprintf(msg, "Unexpected/Unhandled condition at func 0x50: %lu", dwAttrib);
        printf("%s", msg);
        globals->vdirectoryExists = false;
        globals->vdirectoryErrors = true;
        return false;
    }
}

static void DebugDirectory(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);
    printf("Error creating directory. \nError Code: %ld\n", dwAttrib);
}

static void GenAppDataLogFolder(char path[]) //Creates the AppData Log Folder if it does not already exist.
{
    char *Path =  path;
    bool vdirectoryExists = DirectoryExists(Path);
    struct globals structglobals;
    struct globals *globals = &structglobals;
    if (vdirectoryExists == false) {
        CreateDirectory(Path, NULL);
    }
    else if (globals->vdirectoryExists == true && globals->vdirectoryErrors == false){
        //Do nothing, directory exists and no errors
    }
    else if (globals->vdirectoryExists == false && globals->vdirectoryErrors == true) {
        DebugDirectory(Path);
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

void LogMessage(char Message[]){ //Creates Log File if not existent, content is written to the log file
    char *username = getenv("USERNAME");
    char *LogFolderPath = malloc(256);
    char *LogFilePath = malloc(256);
    sprintf(LogFolderPath, "C:\\Users\\%s\\AppData\\Local\\MultiTools", username);
    sprintf(LogFilePath, "C:\\Users\\%s\\AppData\\Local\\MultiTools\\log.txt", username);

    GenAppDataLogFolder(LogFolderPath);
    FILE *file;
    file = fopen(LogFilePath, "a");
    fprintf(file, "%s", Message);
    fclose(file);

    free(LogFolderPath);
    free(LogFilePath);
}

void LogMessageP(char * Message) {
    char *username = getenv("USERNAME");
    char *LogFolderPath = malloc(256);
    char *LogFilePath = malloc(256);
    sprintf(LogFolderPath, "C:\\Users\\%s\\AppData\\Local\\MultiTools", username);
    sprintf(LogFilePath, "C:\\Users\\%s\\AppData\\Local\\MultiTools\\log.txt", username);

    GenAppDataLogFolder(LogFolderPath);
    FILE *file;
    file = fopen(LogFilePath, "a");
    fprintf(file, "%s", Message);
    fclose(file);

    free(LogFolderPath);
    free(LogFilePath);
}