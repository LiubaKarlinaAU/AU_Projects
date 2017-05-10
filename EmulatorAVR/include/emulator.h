#ifndef EMULATORAVR_EMULATOR_H
#define EMULATORAVR_EMULATOR_H

#include <fstream>

#include "function_defenition.h"
#include "microcontroller.h"

class Emulator{
public:
    Emulator(std::string log_filename);
    ~Emulator();

    void load(const std::string filename);
    void run_code(const std::string filename);

    void show_information();
private:
    Microcontroller *controller;
    MicrocontrollerOutput *mc_output;
    std::unordered_map<std::string, FunctionDefenition *> function_defenitions;
    std::vector <std::pair<std::string, std::string>> executable_code;
    std::uint32_t code_cycles = 0;

    void create_system(std::ifstream &fis);
    void load_functions(std::ifstream &fis);
};

#endif //EMULATORAVR_EMULATOR_H
