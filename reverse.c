#include <stdio.h>
#include<stdlib.h>
#include<string.h>


int main(){
    // double score_list[][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    // double *p;
    // p =scores(score_list, 2);
    // for (int i = 0; i < 4; ++i)
    // {
    //     /* code */
    //     printf("%5.2f\n", *(p+i));
    // }
    
    double number[] = {1,2,3,4,5};
    double *p = number;
    int length = sizeof(number) / sizeof(double);
    printf("%d\n", length);
    for (int i = 0; i < length; ++i)
    {
        /* code */
        printf("%4.2f\n", p[i]);
    }
}
