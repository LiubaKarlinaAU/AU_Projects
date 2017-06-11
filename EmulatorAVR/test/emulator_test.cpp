#include "emulator_test.h"

#define DO_CHECK(expr) check(expr, __FUNCTION__, __FILE__, __LINE__);

EmulatorTest::EmulatorTest() : Test()
{
}

void EmulatorTest::runAllTests()
{
    try {
        gcdTest();
    	fibonacciTest();
    	factorialTest();
    	nullLoadFileTest();
        nullCodeFileTest();
    } catch (const EmulatorException &e) {
        std::cout << "Testing failed with exception." << std::endl;
        std::cout << e.msg << std::endl;
        std::cout << "Exception from " << e.filename << std::endl;
    }
}

void EmulatorTest::gcdTest()
{
    Emulator system("log");
        
    std::string code_filename = "examples/gcd_code";
    std::string commands_filename = "examples/gcd_commands";

    system.load(commands_filename);
    system.run_code(code_filename);

    DO_CHECK(system.get_registr_value("R0") == 2);
}

void EmulatorTest::fibonacciTest()
{
    Emulator system("log");

    std::string code_filename = "examples/fibonacci_code";
    std::string commands_filename = "examples/fibonacci_commands";

    system.load(commands_filename);
    system.run_code(code_filename);

    DO_CHECK(system.get_registr_value("R0") == 13 && system.get_registr_value("R1") == 21);
}

void EmulatorTest::factorialTest()
{
    Emulator system("log");

    std::string code_filename = "examples/factorial_code";
    std::string commands_filename = "examples/factorial_commands";

    system.load(commands_filename);
    system.run_code(code_filename);

    DO_CHECK(system.get_registr_value("R0") == 240);
}

void EmulatorTest::nullLoadFileTest()
{
    try {
         Emulator system("log");

         system.load("abc");
         DO_CHECK(false); 
    } catch (const EmulatorException &e) {
    }
}


void EmulatorTest::nullCodeFileTest()
{
    try {
         Emulator system("log");

         system.run_code("abc");
         DO_CHECK(false); 
    } catch (const EmulatorException &e) {
    }
}
