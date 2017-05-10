#ifndef EMULATORAVR_FUNCTION_H
#define EMULATORAVR_FUNCTION_H
#include <sstream>
#include <vector>

typedef uint8_t byte;
enum comand_type {VALUE_SETTING, WITH_CONDITION};

struct FunctionDefenition{
    FunctionDefenition(std::size_t machine_cycles, std::string &arguments, std::string &description);
    FunctionDefenition(const FunctionDefenition &another) {}

    std::size_t machine_cycles;
    std::vector<std::string> argument_names;
    std::vector <std::pair<std::string, comand_type>> commands;
};
#endif //EMULATORAVR_FUNCTION_H
