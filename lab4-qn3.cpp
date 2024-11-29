#include<iostream>     // Includes the input-output stream for reading and writing data
#include<iomanip>      // Includes the library for input-output manipulation (e.g., for formatting)
#include <cstdlib>     // Includes the standard library for general utilities (not directly used here, but useful in many cases)
using namespace std;   // Allows us to use standard C++ functions without prefixing them with `std::`

// Function to print a floating-point number in fixed-point notation
void print_real(float number, int fieldspace, int precision) {
    // Prints the number in fixed-point notation with a specified field width and precision
    cout << fixed << setw(fieldspace) << setprecision(precision) << number << endl;
}

// Template function to update m1 and m2 according to the given rules
template <typename T>  // Defines a template function that works with any data type T
void update_scale(T& m1, T& m2, T m3 = 10) {  // Takes m1, m2 by reference and m3 with a default value of 10
    T original_m1 = m1;   // Store the original value of m1 for future calculations
    T original_m2 = m2;   // Store the original value of m2 for future calculations

    // Update m1: Sum of m1 and m2, multiplied by m3
    m1 = (original_m1 + original_m2) * m3;

    // Update m2: Difference of m1 and m2, multiplied by m3
    m2 = (original_m1 - original_m2) * m3;
}

int main(void) {   // Main function to execute the program
    float a, b;   // Declare two floating-point variables a and b

    cout << "Please input two real numbers: ";  // Prompt user to input two numbers
    cin >> a >> b;   // Read the input values into a and b

    // Print the values of a and b before calling the update_scale function
    print_real(a, 7, 3);   // Print a with a width of 7 and precision of 3
    print_real(b, 7, 3);   // Print b with a width of 7 and precision of 3

    // Call the update_scale function with a and b to modify their values
    update_scale(a, b);   // Pass a and b by reference to the function

    // Print the updated values of a and b after calling the update_scale function
    print_real(a, 7, 3);   // Print the updated value of a
    print_real(b, 7, 3);   // Print the updated value of b

    return 0;   // Return 0 to indicate successful execution
}
