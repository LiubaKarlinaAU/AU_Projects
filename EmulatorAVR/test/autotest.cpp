#include <iostream>
#include "autotest.h"

int Test::failedNum = 0;
int Test::totalNum = 0;

void Test::showFinalResult()
{
    if (!failedNum)
        printf("All test passed.\n");
    else
        printf("Failed %d of %d tests \n", failedNum, totalNum);
}

void Test::check(bool expr, const char *func, const char  *filename, int lineNum)
{
    totalNum++;
    if(!expr)
    {
        printf("Test failed: %s in %s : %d  line.\n", func, filename, lineNum);
        failedNum++;
    }
}
