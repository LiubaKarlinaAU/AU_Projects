#ifndef EMULATORAVR_PARSER_H
#define EMULATORAVR_PARSER_H

#include "function_defenition.h"
#include "microcontroller.h"

class FunctionExecution{
public:
    FunctionExecution(std::string &function_arguments, const FunctionDefenition *function_def, Microcontroller *mc);
    std::uint32_t run();

private:
    Microcontroller *controller;
    const FunctionDefenition *func_defenition;
    std::unordered_map<std::string, byte *> variables;
    std::vector<byte> constants;

    void make_variable_map(std::string &function_arguments);
    void value_setting(std::istringstream &stream);

    void parse_condition(std::istringstream &stream);
    bool is_truth(std::string condition);
    byte evaluate(std::string expression);

    void set_value(const std::string &variable, byte new_value);
    byte get_value(std::string variable);
};

#endif //EMULATORAVR_PARSER_H
