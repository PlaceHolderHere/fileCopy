#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

int copyFile(char filePath[], char destinationPath[]);
int copyDir(char referenceDirectoryPath[], char destinationDirectoryPath[], int currentPathSize, int outputPathSize);

// Global Constants
const int CHARSIZE = sizeof(char);
const int UINT8_SIZE = sizeof(uint8_t);
const int initialBufferSize = 256;

int numOfFilesCopied = 0;

struct flags{
    bool useLogs;
    bool useCustomName;
    bool forceCopy;
    bool copyAll;
};

struct flags programSettings = {false, false, false, false};

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Error! Incomplete inputs, please provide a reference directory and an output directory.\n");
        return -1;
    }
    
    // Required Inputs
    char *referenceDirectoryPath;
    referenceDirectoryPath = argv[1];
    
    char *outputDirectoryPath;
    outputDirectoryPath = argv[2];
    
    char *outputName;

    // Optional Inputs/Flags
    for (int i = 3; i < argc; i++){
        if (argv[i][0] == '-'){
            switch(argv[i][1]){
                case 'l': // logs
                    programSettings.useLogs = true;
                    break;
                case 'f': // force copy
                    programSettings.forceCopy = true;
                    break;
                case 'a': // copy all
                    programSettings.copyAll = true;
                    break;
                case 'n': // custom name
                    programSettings.useCustomName = true;
                    outputName = argv[i+1];
                    i++; // Skipping the next argument since it's the custom name
                    break;
            }
        }
    }

    // Default Value for outputName
    // Get the current time and date
    if (!programSettings.useCustomName){
        char currentDateBuffer[256];
        time_t currentTimeSeconds = time(NULL);
        struct tm *currentTime = localtime(&currentTimeSeconds);
        sprintf(currentDateBuffer, "%d-%d-%d-%d-%d-%d", currentTime->tm_year + 1900, currentTime->tm_mon+1, currentTime->tm_mday, currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
        outputName = currentDateBuffer;
    }

    // Updating the output file path
    char outputPath[1024];
    snprintf(outputPath, sizeof(outputPath), "%s\\%s", outputDirectoryPath, outputName);
    
    // Starting file copying
    printf("Beginning Copy...\n");
    int startSeconds = time(NULL);
    if (copyDir(referenceDirectoryPath, outputPath, initialBufferSize, initialBufferSize) != 0){
        printf("Error! Could not copy directory.\n");
        return -1;
    }
    
    printf("Copy Complete! Operation took %d seconds.\n", time(NULL) - startSeconds);
    printf("Number of files copied: %d\n", numOfFilesCopied);
    return 0;
}


int copyDir(char referenceDirectoryPath[], char destinationDirectoryPath[], int currentPathSize, int outputPathSize){
    // Variable Initialization
    const int refDirPathSize = strlen(referenceDirectoryPath);
    const int outDirPathSize = strlen(destinationDirectoryPath);
    struct dirent *refDirEntry;
    struct stat64 currentFileInfo;
    struct stat64 outputFileInfo;
    int currentPathBufferSize = currentPathSize;
    int outputPathBufferSize = outputPathSize;
    int dirEntryPathSize = -1;
    int returnValue = 0;

    // Allocating currentFilePathBuffer
    char *currentFilePathBuffer = (char*)malloc(CHARSIZE * currentPathBufferSize);
    if (currentFilePathBuffer == NULL){
        printf("Error! Failed to allocate currentFilePathBuffer.\n");
        returnValue = -1;
        goto freeCurrentFilePathBuffer;
    }

    // Allocating outputFilePathBuffer
    char *outputFilePathBuffer = (char*)malloc(CHARSIZE * outputPathBufferSize);
    if (outputFilePathBuffer == NULL){
        printf("Error! Failed to allocate outputFilePathBuffer.\n");
        returnValue = -1;
        goto freeOutputFilePathBuffer;
    }

    // Opening the referenceDirectory
    DIR *referenceDirectory = opendir(referenceDirectoryPath);
    if (referenceDirectory == NULL){
        printf("Error Number: %d\n Error Message:%s\n\nError! Failed to open referenceDirectory.\n", errno, strerror(errno));
        returnValue = -1;
        goto closeRefDir;
    }

    // Checking if output directory exists
    if (stat64(destinationDirectoryPath, &outputFileInfo) != 0){
        
        // Creating an output directory if it doesn't exist
        if (errno == ENOENT){
            
            // Logs
            if (programSettings.useLogs){
                printf("Creating a folder at %s\n", destinationDirectoryPath);
            }
            
            if (mkdir(destinationDirectoryPath) != 0){
                printf("Error Number: %d\n Error Message:%s\n\nError! Failed to copy directory.\n", errno, strerror(errno));
                returnValue = -1;
                goto closeRefDir;
            }
        }

        else{
            printf("Error Number: %d\n Error Message:%s\n\nError! Could not Access the output directory.\n", errno, strerror(errno));
            returnValue = -1;
            goto closeRefDir;
        }
    }

    // Reading the Reference Directory
    while ((refDirEntry = readdir(referenceDirectory)) != NULL){
        
        // Filtering Out Parent & Current Working Directory
        if (strcmp(refDirEntry->d_name, "..") != 0 && strcmp(refDirEntry->d_name, ".")){
            dirEntryPathSize = strlen(refDirEntry->d_name);

            // Checking if the currentFilePath will fit in the buffer and reallocaating memory if it doesn't
            // 12 bytes is from "%s\\%s" and some extra as a buffer
            if (refDirPathSize + 12 + dirEntryPathSize > currentPathBufferSize){
                currentPathBufferSize += ((refDirPathSize + 12 + dirEntryPathSize) - currentPathBufferSize);
                currentPathBufferSize *= 2;

                // Reallocating currentFilePathBuffer
                char* inTempPointer = (char*)realloc(currentFilePathBuffer, CHARSIZE * currentPathBufferSize);
                if (inTempPointer == NULL){
                    printf("Error! Failed to reallocate memory for currentPathBuffer\n");
                    returnValue = -1;
                    goto closeRefDir;
                }
                
                else{
                    currentFilePathBuffer = inTempPointer;
                    inTempPointer = NULL;
                }
            }

            // Checking if the outputFilePath will fit in the buffer and reallocating memory if it doesn't
            // 12 bytes is from "%s\\%s" and some extra as a buffer
            if (outDirPathSize + 12 + dirEntryPathSize > outputPathBufferSize){
                outputPathBufferSize += ((outDirPathSize + 12 + dirEntryPathSize) - outputPathBufferSize);
                outputPathBufferSize *= 2; 
                
                // Reallocating outputFilePathBuffer
                char* outTempPointer = (char*)realloc(outputFilePathBuffer, CHARSIZE * outputPathBufferSize);
                if (outTempPointer == NULL){
                    printf("Error! Failed to reallocate memory for outputPathBuffer\n");
                    returnValue = -1;
                    goto closeRefDir;
                }
                
                else{
                    outputFilePathBuffer = outTempPointer;
                    outTempPointer = NULL;
                }
            }
            
            // Getting the currentFilePath
            if (snprintf(currentFilePathBuffer, currentPathBufferSize, "%s\\%s", referenceDirectoryPath, refDirEntry->d_name) < 0){
                printf("Error Number: %d\n Error Message:%s\n\nError! Failed to get currentFilePath\n", errno, strerror(errno));
                returnValue = -1;
                goto closeRefDir;
            }; 

            // Getting the outputFilePath
            if (snprintf(outputFilePathBuffer, outputPathBufferSize, "%s\\%s", destinationDirectoryPath, refDirEntry->d_name) < 0){
                printf("Error Number: %d\n Error Message:%s\n\nError! Failed to get outputFilePath\n", errno, strerror(errno));
                returnValue = -1;
                goto closeRefDir;
            }
            
            // Separating Directories and Files
            if (stat64(currentFilePathBuffer, &currentFileInfo) == 0){
                
                // Directories
                if (S_ISDIR(currentFileInfo.st_mode)){
                    if (copyDir(currentFilePathBuffer, outputFilePathBuffer, currentPathBufferSize, outputPathBufferSize) != 0){
                        returnValue = -1;
                        goto closeRefDir;
                    }
                }

                // File Copying
                else if(S_ISREG(currentFileInfo.st_mode)){ 

                    // forceCopy and copyAll flags
                    // Checking if the output file already exists
                    if (!programSettings.forceCopy || !programSettings.copyAll){
                        if (stat64(outputFilePathBuffer, &outputFileInfo) == 0){
                            
                            // If copyAll is disabled, check the last modified time of the current and output file
                            // if the current file is newer than the output file, update the output file
                            if (!programSettings.copyAll){
                                if (currentFileInfo.st_mtime < outputFileInfo.st_mtime){
                                    goto skipCopy;
                                }
                            }

                            // Force quit copying unless forceCopy is enabled
                            else{
                                printf("Error! File already exists at %s, terminating copying.\n", outputFilePathBuffer);
                                returnValue = -1;
                                goto closeRefDir;
                            }
                        }
                    }

                    // logs
                    if (programSettings.useLogs){    
                        printf("Copying %s...\n", refDirEntry->d_name);
                    }
                    
                    // File Copying
                    numOfFilesCopied++;
                    if (copyFile(currentFilePathBuffer, outputFilePathBuffer) != 0){
                        printf("Error! Failed to copy %s\n", refDirEntry->d_name);
                        returnValue = -1;
                        goto closeRefDir;
                    }
                    skipCopy:   
                }   
            }
        }
    }
    
    // Exit Handling
    closeRefDir:
    closedir(referenceDirectory);
    
    freeOutputFilePathBuffer:
    free(outputFilePathBuffer);

    freeCurrentFilePathBuffer:
    free(currentFilePathBuffer);

    return returnValue;
}


int copyFile(char filePath[], char destinationPath[]){
    // Variable Initialization
    const int buffer_size = 1000000;
    uint8_t buffer[buffer_size];
    int returnValue = 0;
    int num_of_bytes_read = 0;
    
    // Opening Reference File
    FILE *referenceFile = fopen(filePath, "rb");
    if (referenceFile == NULL){
        printf("Error Number: %d\n Error Message:%s\n\nError! Failed to open referenceFile\n", errno, strerror(errno));
        returnValue = -1;
        goto closeRefFile;
    }

    // Opening Output File
    FILE *outputFile = fopen(destinationPath, "wb");
    if (outputFile == NULL){
        printf("Error Number: %d\n Error Message:%s\n\nError! Failed to open outputFile\n", errno, strerror(errno));
        returnValue = -1;
        goto closeAllFiles;
    }
    
    // Copying Files
    while ((num_of_bytes_read = fread(&buffer, UINT8_SIZE, buffer_size, referenceFile)) > 0){
        if(fwrite(&buffer, UINT8_SIZE, num_of_bytes_read, outputFile) != num_of_bytes_read){
            printf("Error Number: %d\n Error Message:%s\n\nError! Failed to complete file copying\n", errno, strerror(errno));
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