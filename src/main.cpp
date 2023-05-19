#include <iostream>
#include "implementations/mga_1/mga_1.h"
#include "implementations/mga_2/mga_2.cpp"
#include "helpers/helpers.h"

int main(int argc, char** argv) {
  // Clear the console.
  clearConsole();

  // Prompt the user to choose an option.
  int choice = 0;
  while (choice != 3) {
    // Print the header.
    cout << colorString(
        " ██████   ██████   █████████    █████████  \n"
        " ░██████ ██████   ███░░░░░███  ███░░░░░███ \n"
        " ░███░█████░███  ███     ░░░  ░███    ░███ \n"
        " ░███░░███ ░███ ░███          ░███████████ \n"
        " ░███ ░░░  ░███ ░███    █████ ░███░░░░░███ \n"
        " ░███      ░███ ░░███  ░░███  ░███    ░███ \n"
        " █████     █████ ░░█████████  █████   █████\n"
        " ░░░░░     ░░░░░   ░░░░░░░░░  ░░░░░   ░░░░░","green", "black", "bold") << "\n\n";
    cout << colorString("WELCOME TO THE MAZE GENERATION ALGORITHMS!", "green", "black", "bold") << "\n\n";

    // Get the user's choice.
    cout << colorString("Main menu:", "yellow", "black", "bold") << "\n";
    cout << "  1. Run the first maze generation algorithm\n";
    cout << "  2. Run the second maze generation algorithm\n";
    cout << "  3. Exit\n";

    cout << "\n" << colorString("Choose your option:", "yellow", "black", "bold") << "\n";
    cout << colorString("-->", "yellow", "black", "bold") << " ";

    while (!(cin >> choice) || (choice < 1 || choice > 3)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "\n" << colorString("Invalid choice. Please try again.", "red", "black", "bold") << "\n";
      cout << colorString("-->", "yellow", "black", "bold") << " ";
    }

    // Get the executable path.
    string executablePath = argv[0];

    // Switch between the options.
    switch (choice) {
      case 1:
        mga1(executablePath);
        break;
      case 2:
        mga2();
        break;
      case 3:
        break;
      default:
        cout << "Invalid choice. Please try again.\n";
        break;
    }

    // Clear the console.
    clearConsole();
  }

  return 0;
}
