#include "emulator.h"
#include "function_execution.h"

void Emulator::create_system(std::ifstream &fis)
{
    size_t ram_size, eeprom_size;
    fis >> ram_size;
    fis >> eeprom_size;
    controller->tune_up_settings(ram_size, eeprom_size);
}

Emulator::Emulator(std::string log_filename) {
    controller = new Microcontroller(log_filename);
    mc_output = new MicrocontrollerOutput(controller);
}

void Emulator::load(const std::string filename)
{
    std::ifstream input(filename);
    if (!input.is_open())
        throw EmulatorException(__FILE__, "Can't open a file for loadinf emulator function.");
    create_system(input);
    load_functions(input);
}

void Emulator::show_information()
{
    mc_output->show_information(code_cycles);
}

void Emulator::run_code(const std::string filename)
{
    std::ifstream fis(filename);
    if (!fis.is_open())
        throw EmulatorException(__FILE__, "Can't open file for loading emulator functions.");

    std::string function_name, function_arguments;
    controller->PC = 0;

    while(!fis.eof()  || controller->PC != executable_code.size()) {
        if (controller->PC == executable_code.size()) {
            fis >> function_name;
            if (!fis.eof()) {
                fis >> function_arguments;
                executable_code.push_back(std::make_pair(function_name, function_arguments));
                controller->PC = executable_code.size();
                FunctionExecution current_function(function_arguments, function_defenitions[function_name], controller);
                code_cycles += current_function.run();
            }
        }
        else {
            if (controller->PC < executable_code.size())
                while(controller->PC < executable_code.size())
                {
                    auto &it = executable_code[controller->PC];
                    FunctionExecution current_function(it.second, function_defenitions[it.first], controller);
                    code_cycles += current_function.run();
                    if (controller->another_order)
                        controller->another_order = false;
                    else
                        controller->PC++;
                }
            else {
                while (!fis.eof() && controller->PC > executable_code.size()) {
                    fis >> function_name;
                    if (!fis.eof()) {
                        fis >> function_arguments;
                        executable_code.push_back(std::make_pair(function_name, function_arguments));

                    }
                }
                controller->PC = executable_code.size();
            }
        }
    }
}

void Emulator::load_functions(std::ifstream &fis)
{
    std::string function_name, function_description, function_arguments;
    std::size_t function_machine_cycles;

    while(!fis.eof())
    {
        fis >> function_name;
        if (!fis.eof())
        {
            fis >> function_machine_cycles;
            fis >> function_arguments;
            fis >> function_description;
            function_defenitions[function_name] = new FunctionDefenition(function_machine_cycles,function_arguments, function_description);
        }
    }
}

Emulator::~Emulator()
{
    delete mc_output;
    delete controller;

    for (auto &it : function_defenitions)
        delete it.second;
}
