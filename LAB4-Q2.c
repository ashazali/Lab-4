#include <stdio.h>    // Standard I/O library for input and output operations
#include <stdlib.h>   // Standard library for memory management and utilities

#define WIDTH 512     // Define the width of the image as 512 pixels
#define HEIGHT 512    // Define the height of the image as 512 pixels

// Function to read pixel data from a PGM text file
int readPGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (!file) { // If the file could not be opened
        printf("Error: Cannot open file %s\n", filename); // Print error message
        return -1; // Return error code
    }

    char format[3]; // Array to hold the PGM file format
    fscanf(file, "%s\n", format); // Read the format (e.g., P2)
    if (format[0] != 'P' || format[1] != '2') { // Check if the format is "P2" (text-based PGM)
        printf("Error: Invalid PGM format\n"); // Print error message for invalid format
        fclose(file); // Close the file
        return -2; // Return error code
    }

    // Skip over comments in the file
    char c = getc(file); // Read the first character
    while (c == '#') { // While the character is a comment (starts with #)
        while (getc(file) != '\n'); // Skip to the end of the comment line
        c = getc(file); // Get the next character
    }
    ungetc(c, file); // Push the last read character back for later processing

    // Read image dimensions and maximum pixel value
    int w, h, maxVal; // Declare variables for width, height, and max value
    fscanf(file, "%d %d\n%d\n", &w, &h, &maxVal); // Read the dimensions and max value
    if (w != width || h != height) { // If the image dimensions don't match the expected size
        printf("Error: Image dimensions mismatch\n"); // Print error message
        fclose(file); // Close the file
        return -3; // Return error code
    }

    // Read the pixel values from the file into the pixels array
    for (int i = 0; i < width * height; i++) { // Iterate over each pixel
        int val; // Temporary variable to store pixel value
        fscanf(file, "%d", &val); // Read the pixel value
        *(pixels + i) = (unsigned char)val; // Store the pixel value in the array
    }

    fclose(file); // Close the file
    return 0; // Return success code
}

// Function to write pixel data into a PGM text file
int writePGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "w"); // Open the file for writing
    if (!file) { // If the file could not be created
        printf("Error: Cannot create file %s\n", filename); // Print error message
        return -1; // Return error code
    }

    // Write the header information
    fprintf(file, "P2\n"); // Write the format identifier
    fprintf(file, "# Created by LSB Steganography\n"); // Write a comment indicating the tool used
    fprintf(file, "%d %d\n", width, height); // Write the image dimensions
    fprintf(file, "255\n"); // Write the maximum pixel value

    // Write the pixel data
    for (int i = 0; i < width * height; i++) { // Loop through all pixels
        fprintf(file, "%d", pixels[i]); // Write the pixel value
        if ((i + 1) % width == 0) // If it's the end of a row
            fprintf(file, "\n"); // Write a newline character
        else
            fprintf(file, " "); // Otherwise, write a space between pixels
    }

    fclose(file); // Close the file
    return 0; // Return success code
}

// Function to write pixel data into a binary PGM file
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "wb"); // Open the file for binary writing
    if (!file) { // If the file could not be created
        printf("Error: Cannot create file %s\n", filename); // Print error message
        return -1; // Return error code
    }

    // Write the header information
    fprintf(file, "P5\n"); // Write the binary format identifier
    fprintf(file, "# Created by LSB Steganography\n"); // Write a comment
    fprintf(file, "%d %d\n", width, height); // Write the image dimensions
    fprintf(file, "255\n"); // Write the maximum pixel value

    // Write pixel data in binary format
    fwrite(pixels, sizeof(unsigned char), width * height, file); // Write all pixel data at once

    fclose(file); // Close the file
    return 0; // Return success code
}

// Function to embed the secret image into the cover image using LSB technique
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height) {
    for (int i = 0; i < width * height; i++) { // Iterate over all pixels
        coverPixels[i] &= 0xF0; // Clear the lower 4 bits of the cover image pixel

        unsigned char secretMSB = (secretPixels[i] & 0xF0) >> 4; // Get the 4 most significant bits of the secret image

        coverPixels[i] |= secretMSB; // Embed the secret MSBs into the cover image
    }
}

// Function to extract the secret image from the stego image
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height) {
    for (int i = 0; i < width * height; i++) { // Loop through all pixels
        unsigned char lsb = coverPixels[i] & 0x0F; // Extract the 4 least significant bits (LSB)

        outputPixels[i] = lsb << 4; // Shift the LSBs to the most significant bits (MSB) position
    }
}

// Main function that orchestrates the entire process
int main() {
    const char *cover_image = "baboon.pgm"; // Name of the cover image
    const char *secret_image = "farm.pgm"; // Name of the secret image
    const char *stego_image = "stego_image_bin.pgm"; // Output stego image
    const char *extracted_secret = "extracted_secret.pgm"; // Output extracted secret image

    unsigned char *coverPixels = NULL, *secretPixels = NULL, *outputPixels = NULL; // Pointers to hold pixel data
    int coverWidth = WIDTH, coverHeight = HEIGHT; // Cover image dimensions
    int secretWidth = WIDTH, secretHeight = HEIGHT; // Secret image dimensions

    // Allocate memory for the cover image pixels
    coverPixels = (unsigned char *)malloc(coverWidth * coverHeight * sizeof(unsigned char));
    if (coverPixels == NULL) { // Check if memory allocation failed
        printf("Error: Memory allocation for cover image failed\n");
        return -1; // Return error code
    }

    // Read the cover image pixel data
    if (readPGMText(cover_image, coverPixels, coverWidth, coverHeight) != 0) {
        free(coverPixels); // Free memory in case of error
        return -2; // Return error code
    }

    // Allocate memory for the secret image pixels
    secretPixels = (unsigned char *)malloc(secretWidth * secretHeight * sizeof(unsigned char));
    if (secretPixels == NULL) { // Check if memory allocation failed
        printf("Error: Memory allocation for secret image failed\n");
        free(coverPixels); // Free memory
        return -3; // Return error code
    }

    // Read the secret image pixel data
    if (readPGMText(secret_image, secretPixels, secretWidth, secretHeight) != 0) {
        free(coverPixels); // Free memory
        free(secretPixels); // Free memory
        return -4; // Return error code
    }

    // Check if cover and secret images have matching dimensions
    if (coverWidth != secretWidth || coverHeight != secretHeight) {
        printf("Error: Image dimensions do not match\n");
        free(coverPixels); // Free memory
        free(secretPixels); // Free memory
        return -5; // Return error code
    }

    // Embed the secret image into the cover image using LSB steganography
    embedLSB(coverPixels, secretPixels, coverWidth, coverHeight);

    // Write the stego image to a binary PGM file
    if (writePGMBinary(stego_image, coverPixels, coverWidth, coverHeight) != 0) {
        free(coverPixels); // Free memory
        free(secretPixels); // Free memory
        return -6; // Return error code
    }

    // Allocate memory for the output pixels (extracted secret)
    outputPixels = (unsigned char *)malloc(coverWidth * coverHeight * sizeof(unsigned char));
    if (outputPixels == NULL) { // Check if memory allocation failed
        printf("Error: Memory allocation for output image failed\n");
        free(coverPixels); // Free memory
        free(secretPixels); // Free memory
        return -7; // Return error code
    }

    // Extract the secret image from the stego image
    extractLSB(coverPixels, outputPixels, coverWidth, coverHeight);

    // Write the extracted secret image to a text-based PGM file
    if (writePGMText(extracted_secret, outputPixels, coverWidth, coverHeight) != 0) {
        free(coverPixels); // Free memory
        free(secretPixels); // Free memory
        free(outputPixels); // Free memory
        return -8; // Return error code
    }

    // Free allocated memory before program ends
    free(coverPixels);
    free(secretPixels);
    free(outputPixels);

    printf("Steganography process completed successfully!\n"); // Success message
    return 0; // Return success code
}