# Biblioteca processadora de CSV libcsv.so

Esta biblioteca implementa funções de processamento de CSV aplicando seleção de colunas e filtros de comparação de valor. É capaz de receber dados de string e arquivos.

```c
/**
 * Process the CSV data by applying filters and selecting columns.
 *
 * @param csv The CSV data to be processed.
 * @param selectedColumns The columns to be selected from the CSV data.
 * @param rowFilterDefinitions The filters to be applied to the CSV data.
 *
 * @return void
 */
void processCsv(const char csvData[], const char selectedColumns[], const char rowFilterDefinitions[]);

/**
 * Process the CSV file by applying filters and selecting columns.
 *
 * @param csvFilePath The path to the CSV file to be processed.
 * @param selectedColumns The columns to be selected from the CSV data.
 * @param rowFilterDefinitions The filters to be applied to the CSV data.
 *
 * @return void
 */
void processCsvFile(const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[]);
```

# Instalação

Compile os arquivos utilizando o comando:
gcc -shared -o libcsv.so libcsv.c helper.c

# Utilização

Com a compilação bem sucedida, será gerado um arquivo libcsv.so. Utilize-o incluindo o header libcsv.h em seu projeto e também o arquivo gerado libcsv.so ao compilar seu projeto.

Exemplo:

Dado o csv:
```csv
header1,header2,header3
1,2,3
4,5,6
7,8,9
```

```c
const char csv[] = "header1,header2,header3\n1,2,3\n4,5,6\n7,8,9";
processCsv(csv, "header1,header3", "header1>1\nheader3<8");

// header1,header3
// 4,6

const char csv_file[] = "path/to/csv_file.csv";
processCsvFile(csv_file, "header1,header3", "header1>1\nheader3<8");

// header1,header3
// 4,6
```
---