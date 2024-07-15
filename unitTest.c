#include "unitTest.h"

int main(void) {
    const char  line[] = "col1,col2,col3,col4,col5,col6,col7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl2c1,l2c2,l2c3,l2c4,l2c5,l2c6,l2c7\nl3c1,l3c2,l3c3,l3c4,l3c5,l3c6,l3c7\n";
    const char  selHeaders[] = "col2,col3,col4,col6";
    const char  selHeadersWrong[] = "ol2,col,xcol4,col6x";
    const char  selfilter[] = "col2>1\ncol3>1\ncol4>1\ncol6<3\ncol3>1";
    const char  selfilterWrong[] = "col2>1\ncol3>1\ncol4>1\ncol6x3";
    const char  selfilterWrong2[] = "co2>1\ncold3>1\ncol4>1\ncol6x3";

    printf("Should be: error\n");
    processCsvFile("data.csverror", NULL, NULL);
    printf("\nShould be: only csv char\n");
    processCsvFile("data.csv", NULL, NULL);
    printf("\nShould be: only csv file\n");
    processCsv(line, NULL, NULL);

    printf("\nShould be: only csv char and selected\n");
    processCsvFile("data.csv", selHeaders, NULL);
    printf("\nShould be: only csv file and selected\n");
    processCsv(line, selHeaders, NULL);

    printf("\nShould be: only csv char and selected wrong\n");
    processCsvFile("data.csv", selHeadersWrong, NULL);
    printf("\nShould be: only csv file and selected wrong\n");
    processCsv(line, selHeadersWrong, NULL);

    printf("\nShould be: csv char full params\n");
    processCsvFile("data.csv", selHeaders, selfilter);
    printf("\nShould be: csv file full params\n");
    processCsv(line, selHeaders, selfilter);

    printf("\nShould be: csv char full params wrong filter\n");
    processCsvFile("data.csv", selHeaders, selfilterWrong);
    printf("\nShould be: csv file full params wrong filter\n");
    processCsv(line, selHeaders, selfilterWrong);

    printf("\nShould be: csv char full params wrong filter2\n");
    processCsvFile("data.csv", selHeaders, selfilterWrong2);
    printf("\nShould be: csv file full params wrong filter2\n");
    processCsv(line, selHeaders, selfilterWrong2);


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


    printf("\nShould be: \"header1,header2,header3,header4\\n\"\n");

    processCsvFile(csv_file2, "", "header1>1\nheader3<10");
    processCsvFile(csv_file2, NULL, "header1>1\nheader3<10");
    return (0);
}
