#include "libcsv.h"
#include "helper.h"

void processCsv( const char csvData[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    FILE    *tmpFile;

    tmpFile = fopen("./tmp", "w+");
    if (tmpFile == NULL)
        return ;
    fputs(csvData, tmpFile);
    fseek(tmpFile, 0, SEEK_SET);
    processCsvFile("./tmp", selectedColumns, rowFilterDefinitions);
    fclose(tmpFile);
    remove("./tmp");
    return ;
}

void processCsvFile( const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    FILE        *stream;
    char        *line;
    s_header    columns[MAX_SIZE];
    size_t      columnsSize;

    memset(columns, 0, sizeof(columns));
    stream = fopen(csvFilePath, "r");
    if (stream == NULL)
        return ;
    line = getCsvLine(stream);
    if (line != NULL) {
        columnsSize = processCsvColumns(line, columns, selectedColumns);
        if (columnsSize > 0 && parseInputs(columns, selectedColumns, rowFilterDefinitions)) {
            getRowFilterDefinitions(columns, rowFilterDefinitions);
            line = getCsvLine(stream);
            outputColumns(columns, columnsSize);
            while (line != NULL) {
                processCsvLine(line, columns);
                line = getCsvLine(stream);
            }
        }
        freeColumns(columns, columnsSize);
    }
    if (line)
        free(line);
    fclose(stream);
    return ;
}
