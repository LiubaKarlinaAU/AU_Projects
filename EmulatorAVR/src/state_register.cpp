#include "state_register.h"
#include "microcontroller.h"

namespace {
    byte hash(char symbol) {
        switch (symbol) {
            case 'C':
                return 0;
            case 'Z':
                return 1;
            case 'N':
                return 2;
            case 'V':
                return 3;
            case 'S':
                return 4;
            case 'H':
                return 5;
            case 'T':
                return 6;
            case 'I':
                return 7;
            default:
                throw EmulatorException(__FILE__, "Wrong status registr flag.");
        }
    }
}

void StateRegister::set_flag(char type, byte new_value)
{
    flags[hash(type)] = new_value;
}

byte StateRegister::get_flag(char type)
{
    return flags[hash(type)];
}


