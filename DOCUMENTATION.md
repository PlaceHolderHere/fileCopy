* argv Inputs
    * Required Inputs (In a specific order):
    1. A path to a reference directory or the directory you wish to copy.
    2. A path to an output directory or the directory you wish your copy to be located.

    * Optional inputs/Flags (in any order):
    3. customName -> the first inputted string that doesn't start with "-" will be used as the name of the output directory.
    4. -l -> Causes the program to log every folder and file created/copied in the console.
    5. -f -> Causes the program to copy files from the reference directory to the output directory whether or not a file at the output path already exists, effectively overriding the information stored in that output file.

# FUNCTIONS
    * copyDir()
        - copyDir() is a function that copies all files and subdirectories in a referenceDirectory to an outputDirectory, including empty subdirectories 
        - copyDir()  calls itself recursively to search through the contents of each subdirectory and copy them to the output directory.
        - copyDir() calls copyFile() to copy a file
        * Defaults
        1. by default, copyDir() only prints error messages in the console(this can be changed with the logFlag).
        2. by default, copyDir() returns an error if it attempts to write to an output file that already exists (this can be changed with the forceCopy flag).
        * Flags
        1. -l -> this disables default #1 and causes the program to log every file copied and directory created to the console.
        2. -f -> this disables default #2 and causes the program to write to files in the output path even if they already exist in the output directory.
        * INPUTS
            1. char referenceDirectoryPath[] -> The file path of the directory you want to copy.
            2. char destinationDirectoryPath[] -> The file path of the directory you want your copy to be placed.
            3. int currentPathSize -> The initial size of the currentFilePathBuffer.
            4. int outputPathSize -> The initial size of the outputFilePathBuffer.
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