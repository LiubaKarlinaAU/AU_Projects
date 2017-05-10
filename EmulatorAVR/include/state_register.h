#ifndef EMULATORAVR_STATE_REGISTER_H
#define EMULATORAVR_STATE_REGISTER_H

#include <cstdint>
#include <string>

typedef uint8_t byte;

class EmulatorException {
public:
    std::string filename;
    std::string msg;

    EmulatorException(std::string filename, std::string msg) : filename(filename), msg(msg)
    {}
};

class StateRegister{
public:
    void set_flag(char flag, byte new_value);
    byte get_flag(char flag);

private:
    byte flags[8] = {};
};

#endif //EMULATORAVR_STATE_REGISTER_H
