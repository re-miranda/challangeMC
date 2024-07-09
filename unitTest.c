#include "unitTest.h"

int main(void) {
    const char  line[] = "test,test1,test2, test space, hi, ,";

    processCsvFile(NULL, NULL, NULL);
    processCsv(line, NULL, NULL);
    return (0);
}
