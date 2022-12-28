//
// Created by Admin on 27.12.2022.
//

#include "Logger.h"
#include <stdio.h>
#include <windows.h>

static int GenAppDataLogFolder(){
    char path[MAX_PATH];
    sprintf(path, "C:\\Users\\%s\\AppData\\Local\\Temp\\MultiTools", getenv("USERNAME"));
    if (!CreateDirectory(path, NULL))
    {
        printf("Error creating directory: %lu\n", GetLastError());
        return 0;
    }
    return 1;
}
static void GenAppDataLogFile(char Message[]);
void LogMessage(char Message[]){
    char *ParsedMessage = Message;
    GenAppDataLogFile(ParsedMessage);
}

void LogMessageAppend(char Message[]){
    char *ParsedMessage = Message;
    //TODO: Append Version of -> GenAppDataLogFile(ParsedMessage);
}

static void _Append_GenAppDataLogFile(){

}

static void GenAppDataLogFile(char Message[]){

    if (!GenAppDataLogFolder()){
        printf("Error, to create a log file, the MultiTools Folder is required...");
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