//
//  main.cpp
//  овлрыф
//
//  Created by Мария Волкова on 17.05.16.
//  Copyright © 2016 Маша. All rights reserved.
//

#include <iostream>
#include <math.h>
using namespace std;

double** multMatrix(double **a, double **b, int n);
void printMatrix(double **a, int n);
double** hausholder(double *v, int n);
double normVector(double *v, int n);
double** hessenberg(double **a, int n);

int main()
{
    setlocale(LC_ALL, "Russian");
    
    int n = 6; //задаем размерность матрицы
    double **a = new double*[n];
    
    //инициализируем матрицу нулями
    for (int i = 0; i < n; i++)
    {
        a[i] = new double[n];
        for (int j = 0; j < n; j++)
            a[i][j] = 0;
    }
    
    //задаем нужные значения матрицы
    a[0][0] = 2;  a[0][1] = 3; a[0][2] = 6;  a[0][3] = 2;  a[0][4] = 2;  a[0][5] = 2;
    a[1][0] = 4;  a[1][1] = 0; a[1][2] = 3;  a[1][3] = 4;  a[1][4] = 2;  a[1][5] = 2;
    a[2][0] = 1;  a[2][1] = 5; a[2][2] = 2;  a[2][3] = 2;  a[2][4] = 7;  a[2][5] = 2;
    a[3][0] = 2;  a[3][1] = 2; a[3][2] = 5;  a[3][3] = 2;  a[3][4] = 2;  a[3][5] = 2;
    a[4][0] = 2;  a[4][1] = 2; a[4][2] = 2;  a[4][3] = 2;  a[4][4] = 8;  a[4][5] = 2;
    a[5][0] = 2;  a[5][1] = 2; a[5][2] = 2;  a[5][3] = 2;  a[5][4] = 2;  a[5][5] = 2;
    
    cout << "Исходная матрица: \n";
    printMatrix(a, n);
    
    cout << "Матрица Хессенберга: \n";
    printMatrix(hessenberg(a, n), n);
    cout << "\n";
    
    system("pause");
}

//умножает две квадратные матрицы
double** multMatrix(double **a, double **b, int n)
{
    double **c = new double*[n];
    for (int i = 0; i < n; i++)
    {
        c[i] = new double[n];
        for (int j = 0; j < n; j++)
            c[i][j] = 0;
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                c[i][j] += a[i][k] * b[k][j];
    return c;
}


void printMatrix(double **a, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if ((a[i][j] < 0.0000001 && a[i][j] > 0) || (a[i][j] > -0.0000001 && a[i][j] < 0))
                cout << 0 << " ";
            else
                cout << a[i][j] << " ";
        }
        cout << endl;
    }
}

//преобразования Хаусхолдера
double** hausholder(double *v, int n)
{
    double **result = new double*[n], norm = normVector(v, n);
    for (int i = 0; i < n; i++)
        result[i] = new double[n];
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result[i][j] = ((i == j) ? 1 : 0) - 2 * v[i] * v[j] / ( norm * norm);
    return result;
}

//подсчет нормы вектора
double normVector(double *v, int n)
{
    double norm = 0;
    for (int i = 0; i < n; i++)
        norm += v[i] * v[i];
    return sqrt(norm);
}


double** hessenberg(double **a, int n)
{
    double *x = new double[n], **U = new double*[n*n], **H, **A = new double*[n];
    
    for (int i = 0; i < n; i++)
    {
        A[i] = new double[n];
        U[i] = new double[n];
    }
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = a[i][j];
    
    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            x[j] = A[j + i][i - 1];
        }
        x[0] += normVector(x, n - i);
        H = hausholder(x, n - i);
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                if (j < i || k < i)
                {
                    if (j == k)
                        U[j][k] = 1;
                    else
                        U[j][k] = 0;
                }
                else
                    U[j][k] = H[j - i][k - i];
            }
        }
        
        A = multMatrix(U, A, n);
        A = multMatrix(A, U, n);
    }
    return A;
}
