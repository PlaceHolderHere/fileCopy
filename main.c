#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>

int copyFile(char filePath[], char destinationPath[]);

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Error! Incomplete inputs, please provide a reference directory and an output directory.");
        return -1;
    }

    // Variable Intialization
    struct dirent *entry;
    struct stat info;
    char currentFilePath[1024];
    
    // Reference Directory
    char *referenceDirectoryPath;
    referenceDirectoryPath = argv[1];
    DIR *referenceDirectory = opendir(referenceDirectoryPath);
    if (referenceDirectory == NULL){
        printf("Error! Failed to open Reference Directory.");
        return -1;
    }

    // Reading the Reference Directory
    while ((entry = readdir(referenceDirectory)) != NULL){
        if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".")){ // Filtering Out Parent & Current Working Directory
            snprintf(currentFilePath, sizeof(currentFilePath), "%s\\%s", referenceDirectoryPath, entry->d_name); // Getting the current file's direct file path
            // Separating Directories and Files
            if (stat(currentFilePath, &info) == 0){
                if (S_ISDIR(info.st_mode)){
                    printf("Directory: %s\n", entry->d_name);
                }
                else if(S_ISREG(info.st_mode)){ 
                    printf("File: %s\n", entry->d_name);
                }   
            }
        }
    }
    return 0;
}

int copyFile(char filePath[], char destinationPath[]){
    FILE *referenceFile = fopen(filePath, "rb");
    if (referenceFile == NULL){
        printf("Error! Failed to open referenceFile");
        return -1;
    }

    FILE *outputFile = fopen(destinationPath, "wb");
    if (outputFile == NULL){
        printf("Error! Failed to open outputFile");
        return -1;
    }

    int buffer_size = 1;
    uint8_t buffer[buffer_size];

    while (fread(&buffer, buffer_size, 1, referenceFile) != 0){
        if(fwrite(&buffer, buffer_size, 1, outputFile) == 0){
            printf("Error! Failed to complete file copying");
            return -1;
        }
    }

    if (fclose(referenceFile) != 0){
        printf("Error! Failed to close the referenceFile");
        return -1;
    }
    if (fclose(outputFile) != 0){
        printf("Error! Failed to close the outputFile");
        return -1;
    }

    return 0;
}