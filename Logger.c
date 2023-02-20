//
// Created by Admin on 27.12.2022.
//

#include "Logger.h"
#include <stdio.h>
#include <windows.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

BOOL DirectoryExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

/*static void DebugDirectory(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);
    char * Error_CAUSE = dwAttrib;
}*/

static void GenAppDataLogFolder(char path[]) //Creates the AppData Log Folder if it does not already exist.
{
    char *Path =  path;

    if (DirectoryExists(Path) == 0) {
        CreateDirectory(Path, NULL);
        LogMessage("Created Directory.");
    }
    else { //Failed creating LogFolder
        //DebugDirectory(Path);
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
    fprintf(file, "\nTest");
    fclose(file);

    free(LogFolderPath);
    free(LogFilePath);
}