#ifndef EMULATORAVR_AUTOTEST_H
#define EMULATORAVR_AUTOTEST_H

class Test {
protected:
    static int failedNum; // количество тестов, которые сломались
    static int totalNum;  // общее количество тестов
public:
    static void check(bool expr, const char *func, const char  *filename, int lineNum);
    static void showFinalResult();
    virtual void runAllTests() = 0;
};

class EmulatorTest: public Test {
public:
    EmulatorTest();
    void runAllTests();
private:
};

#endif
