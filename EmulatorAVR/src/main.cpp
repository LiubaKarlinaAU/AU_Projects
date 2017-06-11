#include <iostream>
#include "emulator.h"

namespace EmulatorInterface{
    Emulator* create_emulator()
    {
        std::string filename;
        std::cout << "Write a filename for saving after running emulator data: ";
        std::cin >> filename;
        return new Emulator(filename);
    }

    void output_menu()
    {
        std::cout << std::endl << "Your options are:" << std::endl;
        std::cout << "0 - Exit\n" <<"1 - Show menu\n" <<"2 - Load function and run code\n" << "3 - Output  information" << std::endl << std::endl;
    }

    void run()
    {
        Emulator *emulator = create_emulator();
        output_menu();
        try {
            bool run = true;
            std::string filename;
            while (run) {
                std::cout << "To see menu write number 1." << std::endl;
                std::cout << "Your choice is: ";
                size_t choice;
                std::cin >> choice;
                std::cout << std::endl;
                 switch (choice) {
                     case 0:
                         run = false;
                         break;
                     case 1:
                         output_menu();
                         break;
                     case 2:
                         std::cout << "Write a filename for loading emulator function: ";
                         std::cin >> filename;
                         emulator->load(filename);
                         std::cout << "Write a filename with emulator code for run: ";
                         std::cin >> filename;
                         emulator->run_code(filename);
                         std::cout << std::endl;
                         break;
                     case 3:
                         emulator->show_information();
                         break;
                     default:
                         std::cout << "Wrong choice, try again\n";
                 }
            }
        } catch (const EmulatorException &e) {
            std::cout << e.msg << std::endl;
            std::cout << "Exception from " << e.filename << std::endl;
            if (emulator != NULL) {
                delete emulator;
                emulator = NULL;
            }
        }

        if (emulator != NULL)
            delete emulator;
    }
}

int main() {
    try {
        EmulatorInterface::run();
    } catch(const EmulatorException &e) {
        std::cout << e.msg << std::endl;
        std::cout << "Exception from " << e.filename << std::endl;
    }
    return 0;
}
