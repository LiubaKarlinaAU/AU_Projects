#ifndef EMULATORAVR_MICROCONTROLLER_H
#define EMULATORAVR_MICROCONTROLLER_H

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "state_register.h"

class Microcontroller{
public:
    friend class FunctionExecution;
    friend class MicrocontrollerOutput;

    byte PC;
    bool another_order;

    Microcontroller(std::string filename);
    ~Microcontroller();

    void tune_up_settings(std::size_t ram_size, std::size_t eeprom_size);

private:
    std::string storage_filename;

    std::unordered_map<std::string, byte *> general_registrs;
    StateRegister SREG;
    byte *SP;

    std::vector<byte> *RAM;
    std::vector<byte> *EEPROM;

    void save_memory();
};

struct MicrocontrollerOutput{
public:
    MicrocontrollerOutput(Microcontroller *controller);

    void show_information(size_t code_cycles);
private:
    Microcontroller *microcontroller;

    void show_all_information(size_t code_cycles);
    void show_general_registrs();
    void show_status_registr();
};

#endif //EMULATORAVR_MICROCONTROLLER_H
