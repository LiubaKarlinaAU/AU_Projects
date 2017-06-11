#ifndef EMULATORAVR_EMULATOR_TEST_H
#define EMULATORAVR_EMULATOR_TEST_H

#include <iostream>

#include "autotest.h"
#include "emulator.h"

class EmulatorTest: public Test {
public:
    EmulatorTest();
    void runAllTests();
private:
    void gcdTest();
    void fibonacciTest();
    void factorialTest();
    void nullLoadFileTest();
    void nullCodeFileTest();
};

#endif
