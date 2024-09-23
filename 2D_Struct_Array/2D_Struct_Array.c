#include <stdio.h>
#include <stdarg.h>

#define ROW_NUM 2
#define COL_NUM 3

typedef struct {
	int para1;
	int para2;
} LogPara;

void printfArray(LogPara **para, int rowNum, int colNum)
{
 
    for (int row = 0; row < rowNum; row++) {
        for (int col = 0; col < colNum; col++) {
            printf("row %d col %d: para1 %d para2 %d \n", row, col,
				(*((LogPara *)para + row * colNum + col)).para1, (*((LogPara *)para + row * colNum + col)).para2);
        }
    }
}

int main()
{
    LogPara para[ROW_NUM][COL_NUM] = {{{1, 2}, {3, 4}, {5, 6}}, {{7, 8}, {9, 10}, {11, 12}}};
    printfArray((LogPara **)para, ROW_NUM, COL_NUM);
    return 0;
}


