#include <stdio.h>   // Standard I/O library for file handling operations
#include <stdlib.h>  // Library for utility functions, including memory allocation and string conversion
#include <ctype.h>   // Standard library for character handling (not used in this code but included)


/* Function Prototypes */
int isValidFloat(const char *str);  // Function to check whether a string can be interpreted as a valid float
void processFile(const char *inputFile, const char *outputFile);  // Function to read the input file and write valid floats to an output file

int main() {
    const char *inputFile = "data.txt";  // Name of the file to read input from
    const char *outputFile = "valid_data.txt";  // Name of the file to store valid float data

    processFile(inputFile, outputFile);  // Initiate file processing

    return 0;  // Successful program termination
}

/*
 * Function to check if the string represents a valid floating-point number
 */
int isValidFloat(const char *str) {
    char *endPtr;  // Pointer to hold the address of the first invalid character after parsing

    strtod(str, &endPtr);  // Convert the string to a float, the pointer endPtr will point to the first invalid character

    // Return 1 (true) if the string is not empty and has been completely parsed as a valid float
    return (*str != '\0' && *endPtr == '\0');
}

/*
 * Function to open the input file, read each word, and write valid floats to the output file
 */
void processFile(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");  // Open the input file for reading
    FILE *output = fopen(outputFile, "w");  // Open the output file for writing

    char currentWord[100];  // Buffer to store each word read from the file
    int invalidCount = 0;  // Counter for invalid float entries

    // If either of the files failed to open, print an error message and exit
    if (input == NULL || output == NULL) {
        perror("File opening failed");  // Display an error message if file opening fails
        exit(EXIT_FAILURE);  // Exit with a failure status
    }

    // Read each word from the input file
    while (fscanf(input, "%s", currentWord) != EOF) {
        // Validate if the current word is a valid float
        if (isValidFloat(currentWord)) {  // If it's a valid float
            fprintf(output, "%s\n", currentWord);  // Write the valid float to the output file
        } else {
            invalidCount++;  // Increment the count of invalid float entries
        }
    }

    // If there was an error during file reading, handle it
    if (ferror(input)) {
        perror("Error reading the input file");  // Print an error message
        fclose(input);  // Close both files before exiting
        fclose(output);
        exit(EXIT_FAILURE);  // Exit with a failure status
    }

    // Close the input and output files after completing the operations
    fclose(input);
    fclose(output);

    // Output the number of invalid float values encountered
    printf("Invalid float entries encountered: %d\n", invalidCount);
}