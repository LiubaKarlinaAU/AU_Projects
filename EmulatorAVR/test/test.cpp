#include "emulator_test.h"

int main() {
    EmulatorTest test = EmulatorTest();
    test.runAllTests();
    Test::showFinalResult();
    return 0;
}

