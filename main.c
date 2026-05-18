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

const int CHARSIZE = sizeof(char);

struct flags{
    bool useLogs;
    bool useCustomName;
};

struct flags programSettings = {false, false};

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Error! Incomplete inputs, please provide a reference directory and an output directory.\n");
        return -1;
    }
 
    // Program Variables
    const int initialBufferSize = 256;
    
    char *referenceDirectoryPath;
    referenceDirectoryPath = argv[1];
    
    char *outputDirectoryPath;
    outputDirectoryPath = argv[2];
    
    char *outputName = "output";    
    // Optional Inputs
    if (argc > 3){
        for (int i = 3; i < argc; i++){
            // Flags
            if (argv[i][0] == '-'){
                switch(argv[i][1]){
                    case 'l': // logs
                        programSettings.useLogs = true;
                        break;
                }
            }
            // Custom Output Name
            else if (!programSettings.useCustomName){
                programSettings.useCustomName = true;
                outputName = argv[i];
            }
        }
    }

    // Default Value for outputName
    if (!programSettings.useCustomName){
        // Getting the current time for the output directory name
        char currentDateBuffer[256];
        time_t currentTimeSeconds = time(NULL);
        struct tm *currentTime = localtime(&currentTimeSeconds);
        sprintf(currentDateBuffer, "%d-%d-%d-%d-%d-%d", currentTime->tm_year + 1900, currentTime->tm_mon+1, currentTime->tm_mday, currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
        outputName = currentDateBuffer;
    }

    // Updating the output file path
    char outputPath[1024];
    snprintf(outputPath, sizeof(outputPath), "%s\\%s", outputDirectoryPath, outputName);
    
    // Checking for log flag
    printf("Beginning Copy...\n");
    int startSeconds = time(NULL);
    if (copyDir(referenceDirectoryPath, outputPath, initialBufferSize, initialBufferSize) != 0){
        printf("Error! Could not copy directory.\n");
        return -1;
    }
    
    printf("Copy Complete! Operation took %d seconds.\n", time(NULL) - startSeconds);
    return 0;
}


int copyDir(char referenceDirectoryPath[], char destinationDirectoryPath[], int currentPathSize, int outputPathSize){
    // Variable Initialization
    int returnValue = 0;
    struct dirent *refDirEntry;
    struct stat64 currentFileInfo;
    struct stat64 outputFileInfo;
    const int refDirPathSize = strlen(referenceDirectoryPath);
    const int outDirPathSize = strlen(destinationDirectoryPath);
    int currentPathBufferSize = currentPathSize;
    int outputPathBufferSize = outputPathSize;
    int dirEntryPathSize = -1;

    char *currentFilePathBuffer = (char*)malloc(CHARSIZE * currentPathBufferSize);
    if (currentFilePathBuffer == NULL){
        printf("Error! Failed to allocate currentFilePathBuffer.\n");
        returnValue = -1;
        goto closeRefDir;
    }

    char *outputFilePathBuffer = (char*)malloc(CHARSIZE * outputPathBufferSize);
    if (outputFilePathBuffer == NULL){
        printf("Error! Failed to allocate outputFilePathBuffer.\n");
        returnValue = -1;
        goto closeRefDir;
    }

    DIR *referenceDirectory = opendir(referenceDirectoryPath);
    if (referenceDirectory == NULL){
        printf("Error Number: %d\n Error Message:%s\n\nError! Failed to open referenceDirectory.\n", errno, strerror(errno));
        returnValue = -1;
        goto closeRefDir;
    }

    // Checking if output directory exists
    if (stat64(destinationDirectoryPath, &outputFileInfo) != 0){
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
        if (strcmp(refDirEntry->d_name, "..") != 0 && strcmp(refDirEntry->d_name, ".")){ // Filtering Out Parent & Current Working Directory
            dirEntryPathSize = strlen(refDirEntry->d_name);

            // Checking if the currentFilePath will fit in the buffer and reallocaating memory if it doesn't
            // 12 is from a string "%s\\%s" and some extra as a buffer
            if (refDirPathSize + 12 + dirEntryPathSize > currentPathBufferSize){
                currentPathBufferSize += ((refDirPathSize + 12 + dirEntryPathSize) - currentPathBufferSize);
                currentPathBufferSize *= 2;
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
            // 12 is from a string "%s\\%s" and some extra as a buffer
            if (outDirPathSize + 12 + dirEntryPathSize > outputPathBufferSize){
                outputPathBufferSize += ((outDirPathSize + 12 + dirEntryPathSize) - outputPathBufferSize);
                outputPathBufferSize *= 2; 
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
                if (S_ISDIR(currentFileInfo.st_mode)){
                    if (copyDir(currentFilePathBuffer, outputFilePathBuffer, currentPathBufferSize, outputPathBufferSize) != 0){
                        returnValue = -1;
                        goto closeRefDir;
                    }
                }
                // File Copying
                else if(S_ISREG(currentFileInfo.st_mode)){ 
                    // logs
                    if (programSettings.useLogs){    
                        printf("Copying %s...\n", refDirEntry->d_name);
                    }
                    if (copyFile(currentFilePathBuffer, outputFilePathBuffer) != 0){
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
    int buffer_size = 1000000;
    uint8_t buffer[buffer_size];
    const int uint8_size = sizeof(uint8_t);
    int num_of_bytes_read = 0;
    
    // referenceFile Error Handling
    if (referenceFile == NULL){
        printf("Error Number: %d\n Error Message:%s\n\nError! Failed to open referenceFile\n", errno, strerror(errno));
        returnValue = -1;
        goto closeRefFile;
    }

    // outputFile Error Handling
    if (outputFile == NULL){
        printf("Error Number: %d\n Error Message:%s\n\nError! Failed to open outputFile\n", errno, strerror(errno));
        returnValue = -1;
        goto closeAllFiles;
    }
    
    // Copying Files
    while ((num_of_bytes_read = fread(&buffer, uint8_size, buffer_size, referenceFile)) > 0){
        if(fwrite(&buffer, uint8_size, num_of_bytes_read, outputFile) != num_of_bytes_read){
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