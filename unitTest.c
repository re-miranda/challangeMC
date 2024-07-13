#include "unitTest.h"

int main(void) {
    const char  line[] = "col1,col2,col3,col4,col5,col6,col7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl2c1,l2c2,l2c3,l2c4,l2c5,l2c6,l2c7\nl3c1,l3c2,l3c3,l3c4,l3c5,l3c6,l3c7\n";
    const char  selHeaders[] = "col2,col3,col4,col6";

    processCsvFile("data.csverror", NULL, NULL);
    processCsvFile("data.csv", NULL, NULL);
    processCsv(line, NULL, NULL);

    processCsvFile("data.csverror", selHeaders, NULL);
    processCsvFile("data.csv", selHeaders, NULL);
    processCsv(line, selHeaders, NULL);
    return (0);
}
