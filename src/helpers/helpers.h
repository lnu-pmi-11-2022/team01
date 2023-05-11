#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

// Random number generator.
unsigned int randomGenerator();

// Function that colors the string.
string colorString(
    const string& str,
    const string& color = "default",
    const string& backgroundColor = "default",
    const string& style = "default"
);

// Function that clears the console.
void clearConsole();

// Function that converts milliseconds to time string.
string millisecondsToTimeString(unsigned long long milliseconds);

// Function that generates a progress bar string.
string generateProgressBarString(unsigned int percentage, unsigned int maxNumberOfBlocks);

// Function that stops the function execution flow until the user presses enter.
void waitForEnter(const string& message = "Press enter to continue...");

#endif
