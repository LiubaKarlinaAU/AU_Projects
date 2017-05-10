#include <regex>
#include <iostream>

#include "function_execution.h"

namespace{
    enum var_type {REGISTR_GENERAL_PURPOSE, CONSTANT, RAM_MEMORY_ADDRESS, SREG_FLAG, STACK, SP, NUMBER, PC};
    enum operation_type {PLUS, MINUS, MULTIPLY, DIVIDE, PERSENT};
    byte priority[] = {0, 0, 2, 2, 1};

    std::stack<byte> values;
    std::stack<operation_type> operations;

    var_type get_variable_type(const std::string &str)
    {
        if (str == "STACK")
            return STACK;
        if (str == "SP")
            return SP;
        if (str == "PC")
            return PC;
        if (str == "C" || str == "Z" || str == "N" || str == "V" || str == "S" || str == "H" || str == "T" || str == "I")
            return SREG_FLAG;
        if (std::regex_match(str, std::regex("a-z")))
            return CONSTANT;
        if (std::regex_match(str, std::regex("R[0-9]+")))
            return REGISTR_GENERAL_PURPOSE;
        if ((std::regex_match(str, std::regex("[0-9]+"))))
            return NUMBER;
        return RAM_MEMORY_ADDRESS;
    }

    operation_type get_operation_type(char c)
    {
        if (c == '+')
            return PLUS;
        if (c == '-')
            return MINUS;
        if (c == '*')
            return MULTIPLY;
        if (c == '/')
            return DIVIDE;
        if (c == '%')
            return PERSENT;

        throw EmulatorException(__FILE__, "Incorrect operation in exspression.");
    }

    void make_operation()
    {
        byte b = values.top();
        values.pop();
        byte a = values.top();
        values.pop();
        operation_type op_type = operations.top();
        operations.pop();

        switch (op_type)
        {
            case PLUS:
                values.push(a + b);
                break;
            case MINUS:
                values.push(a - b);
                break;
            case MULTIPLY:
                values.push(a * b);
                break;
            case DIVIDE:
                values.push(a / b);
                break;
            case PERSENT:
                values.push(a % b);
                break;
        }
    }

    bool is_operation(char c)
    {
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
            return true;
        return false;
    }
}

FunctionExecution::FunctionExecution(std::string &function_arguments,
                                     const FunctionDefenition *function_def, Microcontroller *mc) :
                                     controller(mc), func_defenition(function_def) {
    make_variable_map(function_arguments);
}

void FunctionExecution::make_variable_map(std::string &function_arguments)
{
    std::istringstream iss_arguments(function_arguments);
    constants.reserve(func_defenition->argument_names.size());
    for (auto &var_name : func_defenition->argument_names) {
        std::string each;
        std::getline(iss_arguments, each, ',');
        if (each != "_") {
            switch (get_variable_type(each))
            {
                case REGISTR_GENERAL_PURPOSE:
                    variables[var_name] = controller->general_registrs[each];
                    break;
                case NUMBER:
                    constants.push_back(atoi(each.c_str()));
                    variables[var_name] = &constants[constants.size() - 1];
                    break;
                case PC:
                    variables[var_name] = &controller->PC;
                    break;
                default:
                    throw EmulatorException(__FILE__, "Wrong variable in function code.");
            }
        }
    }
}

void FunctionExecution::set_value(const std::string &variable, byte new_value)
{
    if (variables.find(variable) != variables.end())
        *variables[variable] = new_value;
    else {
        switch (get_variable_type(variable))
        {
            case REGISTR_GENERAL_PURPOSE:
                *controller->general_registrs[variable] = new_value;
                break;
            case SREG_FLAG:
                controller->SREG.set_flag(variable[0], new_value);
            case STACK:
                *controller->SP = new_value;
                controller->SP++;
                break;
            case PC:
                controller->PC = new_value;
                controller->another_order = true;
                break;
            default:
                throw EmulatorException(__FILE__, "Wrong variable in function code.");
        }
    }
}

byte FunctionExecution::get_value(std::string variable)
{
    if (variables.find(variable) != variables.end())
        return *variables[variable];
    else {
        switch (get_variable_type(variable))
        {
            case REGISTR_GENERAL_PURPOSE:
                return *controller->general_registrs[variable];
            case SREG_FLAG:
                return controller->SREG.get_flag(variable[0]);
            case STACK:
                controller->SP--;
                return *(controller->SP);
            case NUMBER:
                return atoi(variable.c_str());
            case PC:
                return controller->PC;
            default:
                throw EmulatorException(__FILE__, "Wrong variable in function code.");
        }
    }
}

byte FunctionExecution::evaluate(std::string exspression)
{
    byte answer = 0;
    uint i = 0, prev = 0;

    for (; i != exspression.size(); ++i)
    {
          char c = exspression[i];
          if (is_operation(c))
           {
               values.push(get_value(std::string(exspression, prev, i - prev)));
               operation_type op_type;
               op_type = get_operation_type(c);
               prev = i + 1;
               while (operations.size() && priority[operations.top()] >= priority[op_type])
                   make_operation();
               operations.push(op_type);
           }
    }

    values.push(get_value(std::string(exspression, prev, i)));
    while (operations.size())
        make_operation();

    answer = values.top();
    values.pop();
    return answer;
}

void FunctionExecution::value_setting(std::istringstream &stream)
{
    std::string variable;
    std::string exspression;
    getline(stream, variable, '=');
    getline(stream, exspression, '=');
    set_value(variable, evaluate(exspression));
}

bool FunctionExecution::is_truth(std::string condition)
{
    for (size_t i = 0; i < condition.size(); ++i)
    {
        if (condition[i] == '&' && condition[i+1] == '&')
            return is_truth(condition.substr(0, i)) && is_truth(condition.substr(i + 2));
        else if (condition[i] == '|' && condition[i+1] == '|')
            return is_truth(condition.substr(0, i)) || is_truth(condition.substr(i + 2));
    }

    for (size_t i = 0; i < condition.size(); ++i) {
        if (condition[i] == '<')
            return evaluate(condition.substr(0, i)) < evaluate(condition.substr(i + 1));
        else if (condition[i] == '>')
            return evaluate(condition.substr(0, i)) > evaluate(condition.substr(i + 1));
        else if (condition[i] == '=' && condition[i + 1] == '=')
            return evaluate(condition.substr(0, i)) == evaluate(condition.substr(i + 2));
        else if (condition[i] == '!' && condition[i + 1] == '=')
            return evaluate(condition.substr(0, i)) != evaluate(condition.substr(i + 2));

    }
    return evaluate(condition);
}

void FunctionExecution::parse_condition(std::istringstream &stream)
{
    std::string first_part;
    std::string condition;
    std::string command;
    std::getline(stream, first_part, ')');
    stream >> command;
    condition = first_part.substr(3);
    std::istringstream comand_stream(command.substr(0));
    if (is_truth(condition)) {
        value_setting(comand_stream);
    }
}

std::uint32_t FunctionExecution::run()
{
    for (auto &command : func_defenition->commands)
    {
        std::istringstream istream(command.first);
        if (command.second == VALUE_SETTING) {
            value_setting(istream);
             }
        else if (command.second == WITH_CONDITION) {
            parse_condition(istream);
        }
    }

    return func_defenition->machine_cycles;
}

