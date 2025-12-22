#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Error! Incomplete inputs, please provide a reference directory and an output directory.");
        return -1;
    }

    // Variable Intialization
    struct dirent *entry;
    
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
        printf("%s\n", entry->d_name);
    }
    return 0;
}