#include <iostream>
#include "implementations/mga_1/mga_1.h"
#include "helpers/helpers.h"

int main() {
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

    cout << colorString("Main menu:", "yellow", "black", "bold") << "\n";
    cout << "  1. Run the first maze generation algorithm\n";
    cout << "  2. Run the second maze generation algorithm\n";
    cout << "  3. Exit\n";

    cout << "\n" << colorString("Choose your option:", "yellow", "black", "bold") << "\n";
    cout << colorString("-->", "yellow", "black", "bold") << " ";
    cin >> choice;

    // Switch between the options.
    switch (choice) {
      case 1:
        mga1();
        break;
      case 2:
        // TODO: Start the second maze generation algorithm.
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
