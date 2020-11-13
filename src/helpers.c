#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "ijvm.h"
#include "helpers.h"

extern IJVM ijvm;

int32_t bytes_to_int(uint8_t *bytes,int offset)
{
    return (int32_t)(
            (uint32_t) bytes[offset + 0] << 24 |
            (uint32_t) bytes[offset + 1] << 16 |
            (uint32_t) bytes[offset + 2] << 8  |
            (uint32_t) bytes[offset + 3]);
}

int16_t bytes_to_short(uint8_t *bytes)
{
    return (int16_t) (bytes[0] << 8 | bytes[1]);
}

uint16_t bytes_to_ushort(uint8_t *bytes)
{
    return (uint16_t) (bytes[0] << 8 | bytes[1]);
}

long get_file_size(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long bufferSize = ftell(fp);
    rewind(fp);

    return bufferSize;
}

void set_ijvm_status(bool status)
{
    ijvm.isRunning = status;
}

bool get_ijvm_status()
{
    return ijvm.isRunning;
}

void print_help()
{
    printf("Usage: ./ijvm binary \n");
}

/**************\
* GUI Helpers  *
\**************/

char* get_appData_path()
{
    char* appDataDirPath;
    
    #if defined(_WIN64) || defined(_WIN32)
        char *appData = getenv("AppData");
        char appName[16] = "\\ijvm-emulator\\";

        appDataDirPath = (char*) malloc(strlen(appData)+strlen(appName)+1);
        strcpy(appDataDirPath,appData);
        strcat(appDataDirPath,appName);

        mkdir(appDataDirPath);

    #elif __APPLE__
        char *homeDir = getenv("HOME");
        char library[44] = "/Library/Application Support/ijvm-emulator/";
        
        appDataDirPath = (char*) malloc(strlen(homeDir)+strlen(library)+1);
        strcpy(appDataDirPath,homeDir);
        strcat(appDataDirPath,library);
        
        mkdir(appDataDirPath,0777);
   
   	#else
   		char* homeDir = getenv("HOME");
   		char appName[17] = "/.ijvm-emulator/";
   		
   		appDataDirPath = (char*) malloc(strlen(homeDir)+strlen(appName)+1);
        strcpy(appDataDirPath,homeDir);
        strcat(appDataDirPath,appName);
   		
   		mkdir(appDataDirPath,0777);
    #endif
    
    return appDataDirPath;
}

char* get_appData_filepath(char* filename)
{
    char *appDataPath = get_appData_path();
    char* fullPath = (char*) malloc(strlen(appDataPath)+strlen(filename)+1);
    
    strcpy(fullPath,appDataPath);
    strcat(fullPath,filename);

    return fullPath;
}

void remove_spaces(char* str)
{
    int count = 0;

    for (int i = 0; str[i]; i++)
        if (str[i] != ' ' && str[i] != '\n'){
			str[count++] = str[i];
        }
    str[count] = '\0';
}
