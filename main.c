#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

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