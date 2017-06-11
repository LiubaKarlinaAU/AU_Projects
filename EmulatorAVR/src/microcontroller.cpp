#include <fstream>
#include <iostream>

#include "microcontroller.h"

namespace {
   std::vector<std::string> all_registr_names = { "R0",   "R1",   "R2",   "R3",  "R4",   "R5",   "R6",  "R7",
                                                  "R8",   "R9",  "R10",  "R11", "R12",  "R13",  "R14", "R15",
                                                 "R16",  "R17",  "R18",  "R19", "R20",  "R21",  "R22", "R23",
                                                 "R24",  "R25",  "R26",  "R27", "R28",  "R29",  "R30", "R31"};
    std::vector<char> status_registr_names = { 'C', 'Z', 'N', 'V', 'S', 'H', 'T', 'I' };

    void small_submenu()
    {
        std::cout << "Your options in submenu are:" << std::endl;
        std::cout << "0 - Return to main menu\n" << "1 - See menu\n" <<
                     "2 - Show all emulator information\n"  <<
                     "3 - Show general registr\n" << "4 - Show status registr\n" <<
                     "5 - Show particular general registr" << std::endl << std::endl;
    }

}

MicrocontrollerOutput::MicrocontrollerOutput(Microcontroller *controller) :
                                             microcontroller(controller) {}


std::size_t MicrocontrollerOutput::get_registr(std::string reg_name)
{
    return static_cast<std::size_t>(*microcontroller->general_registrs[reg_name]);
}

void MicrocontrollerOutput::show_information(size_t code_cycles)
{
    size_t choice;
    small_submenu();
    bool do_cycle = true;

    while (do_cycle) {
        std::cout << "Write number 1 to see menu again." << std::endl;
        std::cout << "Input number, please: ";
        std::cin >> choice;
        std::cout << std::endl;
        switch (choice) {
            case 0:
                do_cycle = false;
                break;
            case 1:
                small_submenu();
                break;
            case 2:
                show_all_information(code_cycles);
                break;
            case 3:
                show_general_registrs();
                break;
            case 4:
                show_status_registr();
                break;
            case 5: {
                std::cout << "Write registr name in form R*: ";
                std::string reg_name;
                std::cin >> reg_name;
                if (microcontroller->general_registrs.find(reg_name) != microcontroller->general_registrs.end())
                    std::cout << reg_name << "=" << static_cast<std::size_t>(*microcontroller->general_registrs[reg_name]) << std::endl;
                else
                    std::cout << "Wrong choice, try again." << std::endl;
            }
                break;
            default:
                std::cout << "Wrong choice, try again." << std::endl;
        }
    }
}

void MicrocontrollerOutput::show_all_information(size_t code_cycles)
{
    std::cout << "Result of running code is:"<< std::endl;
    std::cout << "Microcontroller AVR with RAM = " << microcontroller->RAM->size() - 96 << " and EEPROM = "
              << microcontroller->EEPROM->size() << std::endl;

    show_general_registrs();
    show_status_registr();

    std::cout << "Machine cycles: " << code_cycles << std::endl;
}

void MicrocontrollerOutput::show_status_registr()
{
    std::cout << "Status Registr meaning: " << std::endl;

    for (char flag_name : status_registr_names) {
        std::cout << flag_name << "=" << static_cast<int>(microcontroller->SREG.get_flag(flag_name)) << "    ";
    }

    std::cout << std::endl << std::endl;
}

void MicrocontrollerOutput::show_general_registrs()
{
    std::cout << "General purpose registr meaning:" << std::endl;

    int in_one_line = 0;
    for (auto &reg_name : all_registr_names) {
         std::cout << reg_name << "=" << static_cast<int>(*microcontroller->general_registrs[reg_name]) << "    ";

        in_one_line++;
        if (in_one_line == 8) {
            std::cout << std::endl;
            in_one_line = 0;
        }
    }
    std::cout << std::endl;
}

Microcontroller::Microcontroller(std::string storage_filename) :
                                    PC(0), another_order(false), storage_filename(storage_filename), RAM(NULL), EEPROM(NULL)
{}

void Microcontroller::tune_up_settings(std::size_t ram_size, std::size_t eeprom_size)
{
    if (ram_size < 96) {
        ram_size = 96;
    }

    if (eeprom_size < 64) {
        eeprom_size = 64;
    }

    RAM = new std::vector<byte>(ram_size + 96);
    EEPROM = new std::vector<byte>(eeprom_size);

    auto iterator = RAM->begin();
    for (std::string &reg_name : all_registr_names)
    {
        general_registrs[reg_name] = &*iterator;
        iterator++;
    }

    SP = &*RAM->end();
}

void Microcontroller::save_memory()
{

    std::ofstream ofs(storage_filename, std::ios::binary | std::ios::out);
    if (!ofs.is_open())
        throw EmulatorException(__FILE__, "Can not open file for saving microcontroller data.");
    if (EEPROM){
        std::size_t eeprom_size = EEPROM->size();
        ofs.write(reinterpret_cast<char *>(&eeprom_size), sizeof(size_t));
        for (auto &it : *EEPROM)
            ofs.write(reinterpret_cast<char *>(&it), sizeof(byte));
    }
    ofs.close();
}

Microcontroller::~Microcontroller()
{
    save_memory();
    delete RAM;
    delete EEPROM;
}


