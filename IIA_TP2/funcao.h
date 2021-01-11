#pragma once
// Qualidade da solucao
int calcula_fit(int a[], int** mat, struct info d, int *invalidos);
int calcula_fit_penalizado(int a[], int** mat, struct info d, int* invalidos);
int calcula_div(int sol[], int** mat, int m, int start, int count);

