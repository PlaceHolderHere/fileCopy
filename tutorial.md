How to use FileCopy:

When calling FileCopy from your terminal, you need to input at least 2 arguments which are both file paths.

Required Inputs:
    - The program will not work properly without the following inputs in this order:
    1. Reference File Path - This is the file path that leads to the directory you want to have copied. To get this input, find the folder you want to copy and follow "How to get a file path" below.
        
    2. Output File Path - This is the file path that leads to the folder/drive where you want the copy to be stored. To get this input, find the folder you want to copy and follow "How to get a file path" below.

Program Flags:
    - Program Flags are optional inputs that change how fileCopy creates copies
    3. -l -> Causes the program to log all files and directories created
    4. -f -> Causes the program to rewrite existing file data in the output directory
    5. -a -> Causes the program to perform a fileCopy on all files, even if the output file is newer than the reference file.
    6. -n -> Causes the program to use a custom name for the output directory, specified as the next input after the "-n" flag.

Using FileCopy:
    1. Download filecopy.exe and get it's file path (follow "How to get a file path).
    2. Open Command Prompt or Terminal and input the following command
    3. [Path to filecopy.exe] [Reference File Path] [Output File Path]
        - replace the text within the square brackets based on the needed inputs listed above and don't include the square brackets
    3.1 Optionally, include program flags to alter the program's behavior after the required inputs.
        - for the "-n" flag, input the following:
        - -n [custom output name]
    4. Hit enter to run the program, it should say when it starts and stops copying.

How to get a file path:
    1. Find and select the folder/file. 
    2. Right click the folder/file.
    3. Click on "Copy as Path" which will save the file path to your clipboard.
    4. Hit ctrl+v on your keyboard to paste the filepath.