#include "libcsv.h"
#include "helper.h"

void    processCsvLine( const char csvLine[], s_header columns[]);
size_t  processCsvColumns( const char csvLine[], s_header columns[], const char selectedColumns[]);

void processCsv( const char csvData[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    FILE    *tmp_file;

    tmp_file = fopen("./tmp", "w+");
    if ( tmp_file == NULL )
        return ;
    fputs(csvData, tmp_file);
    fseek(tmp_file, 0, SEEK_SET);
    processCsvFile("./tmp", selectedColumns, rowFilterDefinitions);
    fclose(tmp_file);
    remove("./tmp");
    return ;
}

void processCsvFile( const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    FILE    *stream;

    char    *line;
    size_t  len;
    ssize_t readBytes;

    s_header    columns[MAX_SIZE];
    size_t      columns_size;

    stream = fopen(csvFilePath, "r");
    if (stream == NULL){
        write(1, &"Failed to open file\n", 20);
        return ;
    }

    line = NULL;
    len = 0;
    memset(columns, 0, sizeof(columns));

    readBytes = getline(&line, &len, stream);
    if (readBytes > 0) {
        columns_size = processCsvColumns(line, columns, selectedColumns);
        if (columns_size > 0) {
            getRowFilterDefinitions(columns, rowFilterDefinitions);
            readBytes = getline(&line, &len, stream);
            printf("output:\n");
            for (size_t in = 0; in < columns_size; ++in)
                printf("%s ", columns[in].name);
            printf("\n");
            while (readBytes > 0) {
                processCsvLine(line, columns);
                readBytes = getline(&line, &len, stream);
            }
        }
        for (size_t in = 0; in < columns_size; ++in) {
            free(columns[in].name);
            if (columns[in].filter)
                free(columns[in].filter);
        }
    }
    if (line)
        free(line);
    fclose(stream);
    return ;
}

void    processCsvLine( const char csvLine[], s_header columns[]) {
    char    *csvLineCopy;
    FILE    *outputLine;
    char    *outputLinePtr;
    char    *csvLineCopyContextPtr;
    size_t  outputLineLen;
    char    *cell;
    size_t  headerIndex;
    int     first_run_flag;
    int     discart_flag;

    if (!csvLine)
        return ;
    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return ;
    csvLineCopyContextPtr = csvLineCopy;
    outputLine = open_memstream(&outputLinePtr, &outputLineLen);
    if (!outputLine)
        return ;
    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    cell  = strtok_r(csvLineCopy, ",", &csvLineCopyContextPtr);
    headerIndex = 0;
    first_run_flag = 1;
    discart_flag = 0;
    while (cell) {
        if ( columns[headerIndex].selected == 1 ) {
            if ( !assertFilterAllows(cell, columns[headerIndex].filter) )
                discart_flag = 1;
            if (!first_run_flag)
                fputs(",", outputLine);
            first_run_flag = 0;
            fputs(cell, outputLine);
        }
        printf("%s(%i) ", cell, discart_flag);
        cell  = strtok_r(NULL, ",", &csvLineCopyContextPtr);
        ++headerIndex;
    }
    printf("\n");
    fclose(outputLine);
    if (!discart_flag)
        if (outputLinePtr[0] != 0)
            printf("%s\n", outputLinePtr);
    if (cell)
        free(cell);
    if (outputLinePtr)
        free(outputLinePtr);
    if (csvLineCopy)
        free(csvLineCopy);
    return ;
}

size_t    processCsvColumns(const char csvLine[], s_header columns[], const char selectedColumns[]) {
    char  *csvLineCopy;
    char  *cell;
    size_t  index;

    if (!csvLine)
        return (-1);
    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return (-1);
    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    cell  = strtok(csvLineCopy, ",");
    index = 0;
    while (cell) {
        if (index >= MAX_SIZE)
            break ;
        columns[index].name = strdup(cell);
        columns[index].selected = assertIsSelectedHeader(cell, selectedColumns);
        columns[index].filter = NULL;
        index++;
        cell  = strtok(NULL, ",");
    }
    free(csvLineCopy);
    if ( cell ) {
        free(cell);
        return (-3);
    }
    return (index);
}
