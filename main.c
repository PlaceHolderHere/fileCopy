#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

int copyFile(char filePath[], char destinationPath[]);
int copyDir(char referenceDirectoryPath[], char destinationDirectoryPath[]);  // Work in Progress

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Error! Incomplete inputs, please provide a reference directory and an output directory.\n");
        return -1;
    }
 
    // Reference Directory
    char *referenceDirectoryPath;
    referenceDirectoryPath = argv[1];
    char *outputDirectoryPath;
    outputDirectoryPath = argv[2];

    copyDir(referenceDirectoryPath, outputDirectoryPath);

    return 0;
}

// Working in Progress
int copyDir(char referenceDirectoryPath[], char destinationDirectoryPath[]){
    // Variable Initialization
    struct dirent *refDirEntry;
    struct stat currentFileInfo;
    struct stat outputFileInfo;
    char currentFilePath[1024];
    char outputFilePath[1024];

    DIR *referenceDirectory = opendir(referenceDirectoryPath);
    if (referenceDirectory == NULL){
        printf("Error! Failed to open referenceDirectory.\n");
        return -1;
    }

    // Checking if output directory exists
    if (stat(destinationDirectoryPath, &outputFileInfo) != 0){
        if (errno == ENOENT){
            printf("Creating a folder at %s\n", destinationDirectoryPath);
            if (mkdir(destinationDirectoryPath) != 0){
                printf("Error! Failed to copy directory.\n");
                return -1;
            }
        }
        else{
            printf("Error! Could not Access the output directory.\n");
            return -1;
        }
    }

    // Reading the Reference Directory
    while ((refDirEntry = readdir(referenceDirectory)) != NULL){
        if (strcmp(refDirEntry->d_name, "..") != 0 && strcmp(refDirEntry->d_name, ".")){ // Filtering Out Parent & Current Working Directory
            snprintf(currentFilePath, sizeof(currentFilePath), "%s\\%s", referenceDirectoryPath, refDirEntry->d_name); // currentFilePath
            snprintf(outputFilePath, sizeof(outputFilePath), "%s\\%s", destinationDirectoryPath, refDirEntry->d_name); // outputFilePath
            
            // Separating Directories and Files
            if (stat(currentFilePath, &currentFileInfo) == 0){
                if (S_ISDIR(currentFileInfo.st_mode)){
                    copyDir(currentFilePath, outputFilePath);
                }
                // File Copying
                else if(S_ISREG(currentFileInfo.st_mode)){ 
                    printf("Copying %s...\n", refDirEntry->d_name);
                    if (copyFile(currentFilePath, outputFilePath) != 0){
                        printf("Error! Failed to copy %s\n", refDirEntry->d_name);
                        return -1;
                    }   
                }   
            }
        }
    }
    if (closedir(referenceDirectory) != 0){
        printf("Error! Failed to close referenceDirectory\n");
        return -1;
    }
    return 0;
}
int copyFile(char filePath[], char destinationPath[]){
    FILE *referenceFile = fopen(filePath, "rb");
    if (referenceFile == NULL){
        printf("Error! Failed to open referenceFile\n");
        return -1;
    }

    FILE *outputFile = fopen(destinationPath, "wb");
    if (outputFile == NULL){
        printf("Error! Failed to open outputFile\n");
        return -1;
    }

    int buffer_size = 1;
    uint8_t buffer[buffer_size];

    while (fread(&buffer, buffer_size, 1, referenceFile) != 0){
        if(fwrite(&buffer, buffer_size, 1, outputFile) == 0){
            printf("Error! Failed to complete file copying\n");
            return -1;
        }
    }

    if (fclose(referenceFile) != 0){
        printf("Error! Failed to close the referenceFile\n");
        return -1;
    }
    if (fclose(outputFile) != 0){
        printf("Error! Failed to close the outputFile\n");
        return -1;
    }

    return 0;
}