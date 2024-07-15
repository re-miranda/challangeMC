#include "unitTest.h"

int main(void) {
    // const char  line[] = "col1,col2,col3,col4,col5,col6,col7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl2c1,l2c2,l2c3,l2c4,l2c5,l2c6,l2c7\nl3c1,l3c2,l3c3,l3c4,l3c5,l3c6,l3c7\n";
    // const char  selHeaders[] = "col2,col3,col4,col6";
    // const char  selHeadersWrong[] = "ol2,col,xcol4,col6x";
    // const char  selfilter[] = "col2>1\ncol3>1\ncol4>1\ncol6<3\ncol3>1";
    // const char  selfilterWrong[] = "col2>1\ncol3>1\ncol4>1\ncol6x3";

    // processCsvFile("data.csverror", NULL, NULL);
    // processCsvFile("data.csv", NULL, NULL);
    // processCsv(line, NULL, NULL);

    // processCsvFile("data.csverror", selHeaders, NULL);
    // processCsvFile("data.csv", selHeaders, NULL);
    // processCsv(line, selHeaders, NULL);

    // processCsvFile("data.csverror", selHeadersWrong, NULL);
    // processCsvFile("data.csv", selHeadersWrong, NULL);
    // processCsv(line, selHeadersWrong, NULL);

    // processCsvFile("data.csverror", selHeaders, selfilter);
    // processCsvFile("data.csv", selHeaders, selfilter);
    // processCsv(line, selHeaders, selfilter);

    // processCsvFile("data.csverror", selHeaders, selfilterWrong);
    // processCsvFile("data.csv", selHeaders, selfilterWrong);
    // processCsv(line, selHeaders, selfilterWrong);


    printf("\nShould be: \"header1,header3\\n4,6\"\n");

    const char csv[] = "header1,header2,header3\n1,2,3\n4,5,6\n7,8,9";
    processCsv(csv, "header1,header3", "header1>1\nheader3<8");

    const char csv_file[] = "example.csv";
    processCsvFile(csv_file, "header1,header3", "header1>1\nheader3<8");


    printf("\nShould be: \"header1,header3,header4\\n\"\n");

    const char csv2[] = "header1,header2,header3,header4\n1,2,3,4\n5,6,7,8\n9,10,11,12";
    processCsv(csv2, "header1,header3,header4", "header1>1\nheader3<10");

    const char csv_file2[] = "example2.csv";
    processCsvFile(csv_file2, "header1,header3,header4", "header1>1\nheader3<10");
    return (0);
}
