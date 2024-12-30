#include <stdio.h>
#include <stdarg.h>
#include "Log_Trace.h"

#define ROW_NUM 2
#define COL_NUM 3

typedef struct {
	int para1;
	int para2;
} DataPara;

void printfArray(DataPara **para, int rowNum, int colNum)
{
    for (int row = 0; row < rowNum; row++) {
        for (int col = 0; col < colNum; col++) {
            LOG_TRACE(NULL, "row %d col %d: para1 %d para2 %d", row, col,
				(*((DataPara *)para + row * colNum + col)).para1, (*((DataPara *)para + row * colNum + col)).para2);
        }
    }
}

int main()
{
    DataPara para[ROW_NUM][COL_NUM] = {{{1, 2}, {3, 4}, {5, 6}}, {{7, 8}, {9, 10}, {11, 12}}};
    printfArray((DataPara **)para, ROW_NUM, COL_NUM);
    return 0;
}


