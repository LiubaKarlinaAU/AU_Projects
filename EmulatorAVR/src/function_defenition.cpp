#include "function_defenition.h"

FunctionDefenition::FunctionDefenition(std::size_t machine_cycles, std::string &arguments,
                                       std::string &description) : machine_cycles(machine_cycles)
{
    std::istringstream iss_arguments(arguments);
    std::istringstream iss_description(description);
    std::string each;

    while (std::getline(iss_arguments, each, ',')) {
        argument_names.push_back(each);
    }

    while (std::getline(iss_description, each, ';')) {
        if (each.size() > 1 && each[0] == 'i' && each[1] == 'f')
            commands.push_back(std::make_pair(each, WITH_CONDITION));
        else
            commands.push_back(std::make_pair(each, VALUE_SETTING));
    }
}
