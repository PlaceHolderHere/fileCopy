* argv Inputs
    - The program will require 2 inputs:
    1. A path to a reference directory or the directory you wish to copy.
    2. A path to an output directory or the directory you wish your copy to be located.
    Optional inputs:
    3. A name for the output directory.

# FUNCTIONS
    * copyDir()
        - copyDir() is a function that copies all files and subdirectories in a referenceDirectory to an outputDirectory
        - copyDir() does not copy empty subdirectories to the referenceDirectory
        - copyDir() calls itself recursively to search through the contents of each subdirectory and copy them to the output directory
        - copyDir() calls copyFile() when copying files 
        * INPUTS
            1. char referenceDirectoryPath[] -> The file path of the directory you want to copy
            2. char destinationDirectoryPath[] -> The file path of the directory you want your copy to be placed
        * RETURN VALUES
            * 0 -> a return value of 0 indicates the operation was successful.
            * -1 -> a return value of -1 indicates an error occurred during the operation.
    * copyFile()
        - copyFile copies the file at filePath to a given location at destinationPath
        - return values describe whether the operation was successful or not
        * INPUTS
            1. char filePath[] -> The file path of the file you wish to copy
            2. char destinationPath[] -> The file path of where you want your copied file to be copied
        * RETURN VALUES 
            * 0 -> a return value of 0 indicates the operation was successful
            * -1 -> a return value of -1 indicates an error occurred during the operation.