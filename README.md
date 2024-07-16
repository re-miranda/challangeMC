# Biblioteca para processar CSV

Esta biblioteca implementa funções de processamento de CSV aplicando seleção de colunas e filtros de comparação de valor.
Capaz de receber dados vindos de string ou arquivos.

## Instalação

Para instalar a biblioteca:

* Compile a biblioteca:
    ```sh
    make
    ```

* Compile sem o make (opcional):
    ```sh
    gcc -shared -o libcsv.so libcsv.c helper.c
    ```

## Utilização

Inclua o arquivo header libcsv.h em seu código font e faça link com o arquivo libcsv.so gerado:

```c
#include "libcsv.h"
```

### Funções

#### `void processCsv(const char csv[], const char selectedColumns[], const char rowFilterDefinitions[])`

Process the CSV data by applying filters and selecting columns.

- **Parameters**:
  - `csv`: The CSV data to be processed.
  - `selectedColumns`: The columns to be selected from the CSV data.
  - `rowFilterDefinitions`: The filters to be applied to the CSV data.

#### `void processCsvFile(const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[])`

Process the CSV data by applying filters and selecting columns from a file.

- **Parameters**:
  - `csvFilePath`: The file path of the CSV to be processed.
  - `selectedColumns`: The columns to be selected from the CSV data.
  - `rowFilterDefinitions`: The filters to be applied to the CSV data.

## Exemplos

### Exemplo 1: Processar dados CSV de uma String

```c
#include "libcsv.h"

int main() {
    const char *csvData = "Name,Age,Location\nJohn,30,USA\nJane,25,UK\n";
    const char *selectedColumns = "Name,Location";
    const char *rowFilterDefinitions = "Age>20";

    processCsv(csvData, selectedColumns, rowFilterDefinitions);

    return 0;
}
```

### Exemplo 2: Processar dados CSV de um Arquivo

```c
#include "libcsv.h"

int main() {
    const char *csvFilePath = "data.csv";
    const char *selectedColumns = "Name,Location";
    const char *rowFilterDefinitions = "Age>20";

    processCsvFile(csvFilePath, selectedColumns, rowFilterDefinitions);

    return 0;
}
```

### Exemplo 3:

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