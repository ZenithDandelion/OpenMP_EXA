/*
* @Author: Zenith Dandelion
* @Date:   2016-07-13 21:45:10
* @Last Modified by:   Zenith Dandelion
* @Last Modified time: 2016-07-14 01:47:14
*/


#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

using namespace std;

int matrix_A[10000][10000];
int matrix_B[10000][10000];
long long matrix_C[10000][10000];

__int64 counter_begin;
__int64 counter_end;
__int64 diff;

int main()
{
    int m,p,n;
    int i,j,k;

    // 矩阵大小，简单起见，m,p,n相等
    cout<<"请输入0-10000的数字: "<<endl;
    cin>>n;
    m = p = n ;

    srand((unsigned)time(NULL));

    // 随机生成矩阵
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < p; j++)
            matrix_A[i][j] = rand()%100000;
    }

    for(int i = 0; i < p; i++)
    {
        for(int j = 0; j < n; j++)
            matrix_B[i][j] = rand()%100000;
    }


    QueryPerformanceCounter((LARGE_INTEGER *)&counter_begin);
    for(int i = 0; i < m; i++)
    {

        for(int j = 0; j < n; j++)
        {
            long long temp = 0L;
            for(int k = 0; k < p; k++)
            {
                temp += matrix_A[i][k]*matrix_B[k][j];
            }
            matrix_C[i][j] = temp;
        }
    }
    QueryPerformanceCounter((LARGE_INTEGER *)&counter_end);
    diff=counter_end-counter_begin;
    cout<<"普通计算时间 = "<<diff<<endl;

    // OpenMP
    omp_set_num_threads(4);
    QueryPerformanceCounter((LARGE_INTEGER *)&counter_begin);
#pragma omp parallel shared(matrix_A,matrix_B,matrix_C) private(i,j,k)
{
    #pragma omp for schedule(dynamic)
    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            long long temp = 0L;
            for(k = 0; k < p; k++)
            {
                temp += matrix_A[i][k]*matrix_B[k][j];
            }
            matrix_C[i][j] = temp;
        }
    }
}
    QueryPerformanceCounter((LARGE_INTEGER *)&counter_end);
    diff=counter_end-counter_begin;
    cout<<"并行计算时间 = "<<diff<<endl;


/*
    cout<<endl<<endl<<"A矩阵"<<endl;
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < p; j++)
        {
//            printf("%d ",matrix_A[i][j]);
        }
//        printf("\n");
    }

    cout<<endl<<endl<<"B矩阵"<<endl;
    for(int i = 0; i < p; i++)
    {
        for(int j = 0; j < n; j++)
        {
//            printf("%d ",matrix_B[i][j]);
        }
//        printf("\n");
    }

    cout<<endl<<endl<<"C矩阵"<<endl;
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
//            printf("%ld ",matrix_C[i][j]);
        }
//        printf("\n");
    }
    cout<<endl<<endl<<"END"<<endl;
*/

    return 0;
}



/*

利用OpenMP，A矩阵m×k，B矩阵k×n，求C=A×B

OUTPUT:

请输入0-10000的数字:
1000
普通计算时间 = 5866
并行计算时间 = 3593

Process returned 0 (0x0)   execution time : 13.653 s

*/