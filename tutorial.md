How to use FileCopy:

When calling FileCopy from your terminal, you need to input at least 2 arguments which are both file paths.

Inputs:
- The program will not work properly without the following inputs:
1. Reference File Path - This is the file path that leads to the directory you want to have copied. To get this on windows do the following:
    1.1. Find and Select the folder you wish to copy in file explorer  
    1.2. Right Click the Folder
    1.3. Click on "Copy as Path" which will save it to your clipboard which you can paste with ctrl+v
2. Output File Path - This is the file path that leads to the folder/drive where you want the copy to be stored. You can follow the same instructions for Reference File Path (1.1-1.3) to get your Output File Path.

Optional Inputs:
- The following inputs are optional and not needed for the program to work:
3. Output Directory Name - FileCopy defaults to using "output" as the output directory name, but you can input a different name if you wish.
4. Log flag - adding "-l" as your 4th input causes the program to log every file it copies and every directory it creates.

Using FileCopy:
- The following steps to using FileCopy are:
1. Download filecopy.exe
2. Locate where filecopy.exe and Follow steps 1.2 and 1.3 to copy it's file path.
3. Open Command Prompt or Terminal and input the following command
4. [Path to filecopy.exe] [Reference File Path] [Output File Path] [Optional: Output Directory Name] [Optional: Log Flag]
- replace the text within the square brackets based on the needed inputs listed above and don't include the square brackets
- Optional inputs can be skipped
5. Hit enter to run the program, it should say when it starts and stops copying.