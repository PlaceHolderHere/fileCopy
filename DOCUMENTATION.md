* argv Inputs
    - The program will require 2 inputs:
    1. A path to a reference directory or the directory you wish to copy.
    2. A path to an output directory or the directory you wish your copy to be located.
    Optional inputs:
    3. A name for the output directory.

# FUNCTIONS
    * copyDir() and copyDirLogless()
        - copyDir() & copyDirLogless() is a function that copies all files and subdirectories in a referenceDirectory to an outputDirectory
        - copyDir() & copyDirLogless() copies all files and subdirectories in a main directory, including empty ones
        - copyDir() & copyDirLogless() calls itself recursively to search through the contents of each subdirectory and copy them to the output directory.
        - copyDir() & copyDirLogless() calls copyFile() when copying files
        - copyDirLogless() only prints to the terminal upon an error occurring 
        - when copyDir() copies a file, it prints the file name to the terminal
        - when copyDir() creates a directory, it prints the location of the newly created directory in the terminal
        * INPUTS
            1. char referenceDirectoryPath[] -> The file path of the directory you want to copy
            2. char destinationDirectoryPath[] -> The file path of the directory you want your copy to be placed
        * RETURN VALUES
            * 0 -> a return value of 0 indicates the operation was successful.
            * -1 -> a return value of -1 indicates an error occurred during the operation.
    * copyFile()
        - copyFile() reads each byte of a file at filePath[] and writes it to a given location at destinationPath[]
        - copyFile() reads and writes in 1MB chunks and has a corresponding buffer of the same size
        * INPUTS
            1. char filePath[] -> The file path of the file you wish to copy
            2. char destinationPath[] -> The file path of where you want your copied file to be copied
        * RETURN VALUES 
            * 0 -> a return value of 0 indicates the operation was successful
            * -1 -> a return value of -1 indicates an error occurred during the operation.