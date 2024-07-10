#include "unitTest.h"

int main(void) {
    const char  line[] = "col1,col2,col3,col4,col5,col6,col7\n";

    processCsvFile("data.csverror", NULL, NULL);
    processCsvFile("data.csv", NULL, NULL);
    processCsv(line, NULL, NULL);
    return (0);
}
