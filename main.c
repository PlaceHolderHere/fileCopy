#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

int copyFile(char filePath[], char destinationPath[]);
int copyDir(char referenceDirectoryPath[], char destinationDirectoryPath[]);
int copyDirLogless(char referenceDirectoryPath[], char destinationDirectoryPath[]);

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
    char *outputName = "output";
    if (argc > 3){
        outputName = argv[3];
    }
    char *logFlag;
    if (argc > 4){
        logFlag = argv[4];
    }

    // Updating the output file path
    char outputPath[1024];
    snprintf(outputPath, sizeof(outputPath), "%s\\%s", outputDirectoryPath, outputName);
    
    // Checking for log flag
    if (strcmp(logFlag, "-l") == 0){
        if (copyDir(referenceDirectoryPath, outputPath) != 0){
            printf("Error! Could not copy directory.\n");
            return -1;
        }
    }
    else{
        if (copyDirLogless(referenceDirectoryPath, outputPath) != 0){
            printf("Error! Could not copy directory.\n");
            return -1;
        }
    }

    return 0;
}


int copyDir(char referenceDirectoryPath[], char destinationDirectoryPath[]){
    // Variable Initialization
    int returnValue = 0;
    struct dirent *refDirEntry;
    struct stat currentFileInfo;
    struct stat outputFileInfo;
    char currentFilePath[1024];
    char outputFilePath[1024];

    DIR *referenceDirectory = opendir(referenceDirectoryPath);
    if (referenceDirectory == NULL){
        printf("Error! Failed to open referenceDirectory.\n");
        returnValue = -1;
        goto closeRefDir;
    }

    // Checking if output directory exists
    if (stat(destinationDirectoryPath, &outputFileInfo) != 0){
        if (errno == ENOENT){
            printf("Creating a folder at %s\n", destinationDirectoryPath);
            if (mkdir(destinationDirectoryPath) != 0){
                printf("Error! Failed to copy directory.\n");
                returnValue = -1;
                goto closeRefDir;
            }
        }
        else{
            printf("Error! Could not Access the output directory.\n");
            returnValue = -1;
            goto closeRefDir;
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
                    if (copyDir(currentFilePath, outputFilePath) != 0){
                        returnValue = -1;
                        goto closeRefDir;
                    }
                }
                // File Copying
                else if(S_ISREG(currentFileInfo.st_mode)){ 
                    printf("Copying %s...\n", refDirEntry->d_name);
                    if (copyFile(currentFilePath, outputFilePath) != 0){
                        printf("Error! Failed to copy %s\n", refDirEntry->d_name);
                        returnValue = -1;
                        goto closeRefDir;
                    }   
                }   
            }
        }
    }

    closeRefDir:
    closedir(referenceDirectory);
    return returnValue;
}


int copyDirLogless(char referenceDirectoryPath[], char destinationDirectoryPath[]){
    // Variable Initialization
    int returnValue = 0;
    struct dirent *refDirEntry;
    struct stat currentFileInfo;
    struct stat outputFileInfo;
    char currentFilePath[1024];
    char outputFilePath[1024];

    DIR *referenceDirectory = opendir(referenceDirectoryPath);
    if (referenceDirectory == NULL){
        printf("Error! Failed to open referenceDirectory.\n");
        returnValue = -1;
        goto closeRefDir;
    }

    // Checking if output directory exists
    if (stat(destinationDirectoryPath, &outputFileInfo) != 0){
        if (errno == ENOENT){
            if (mkdir(destinationDirectoryPath) != 0){
                printf("Error! Failed to copy directory.\n");
                returnValue = -1;
                goto closeRefDir;
            }
        }
        else{
            printf("Error! Could not Access the output directory.\n");
            returnValue = -1;
            goto closeRefDir;
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
                    if (copyDir(currentFilePath, outputFilePath) != 0){
                        returnValue = -1;
                        goto closeRefDir;
                    }
                }
                // File Copying
                else if(S_ISREG(currentFileInfo.st_mode)){ 
                    if (copyFile(currentFilePath, outputFilePath) != 0){
                        printf("Error! Failed to copy %s\n", refDirEntry->d_name);
                        returnValue = -1;
                        goto closeRefDir;
                    }   
                }   
            }
        }
    }

    closeRefDir:
    closedir(referenceDirectory);
    return returnValue;
}


int copyFile(char filePath[], char destinationPath[]){
    int returnValue = 0;
    FILE *referenceFile = fopen(filePath, "rb");
    FILE *outputFile = fopen(destinationPath, "wb");
    int buffer_size = 65536;
    uint8_t buffer[buffer_size];
    const int uint8_size = sizeof(uint8_t);
    int num_of_bytes_read = 0;
    
    // referenceFile Error Handling
    if (referenceFile == NULL){
        printf("Error! Failed to open referenceFile\n");
        returnValue = -1;
        goto closeRefFile;
    }

    // outputFile Error Handling
    if (outputFile == NULL){
        printf("Error! Failed to open outputFile\n");
        returnValue = -1;
        goto closeAllFiles;
    }
    
    // Copying Files
    while ((num_of_bytes_read = fread(&buffer, uint8_size, buffer_size, referenceFile)) > 0){
        if(fwrite(&buffer, uint8_size, num_of_bytes_read, outputFile) != num_of_bytes_read){
            printf("Error! Failed to complete file copying\n");
            returnValue = -1;
            goto closeAllFiles;
        }
    }

    // Exit Handling
    closeAllFiles:
    fclose(outputFile);

    closeRefFile:
    fclose(referenceFile);

    return returnValue;
}