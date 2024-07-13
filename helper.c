#include "helper.h"

void    getRowFilterDefinitions(s_header Columns[], const char rowFilterDefinitions[]) {
    for ( s_header *inColumn = Columns; inColumn->name != NULL; ++inColumn ) {
        inColumn->filter = NULL;
    }
    (void)rowFilterDefinitions;
    return ;
}

int assertIsSelectedHeader( const char header[], const char selectedColumns[] ) {
    if (selectedColumns == NULL)
        return (0);
    if (strstr(selectedColumns, header) == NULL)
        return (0);
    return (1);
}

int assertFilterAllows( const char cell[], s_filter filter[]) {
    int         allow_flag;

    if (filter == NULL)
        return (1);
    allow_flag = 1;
    while (filter != NULL) {
        (void)cell;
        filter = filter->next_filter;
    }
    return (allow_flag);
}
