#ifndef EMULATORAVR_AUTOTEST_H
#define EMULATORAVR_AUTOTEST_H

class Test {
protected:
    static int failedNum;
    static int totalNum;
public:
    static void check(bool expr, const char *func, const char  *filename, int lineNum);
    static void showFinalResult();
    virtual void runAllTests() = 0;
};

#endif

