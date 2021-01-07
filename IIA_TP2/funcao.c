#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

// Calcula a qualidade de uma solucao
// Recebe: solucao, matriz, nr de elementos, nr de sub-conjuntos
// Devolve a fitness
int calcula_fit(int a[], int** mat, int m, int g)
{
    int* sol = 0, total = 0, i, ind = 0, subc = 0;

    // Coloca a solucao num vector
    sol = malloc(sizeof(int) * m);
    for (subc = 0; subc < g; subc++)
        for (i = 0; i < m; i++)
            if (a[i] == subc) {
                sol[ind] = i;
                ind++;
            }
    // qualidade = diversidade(0) + diversidade(1) + etc
    for (i = 0; i < g; i++)
        total += calcula_div(sol, mat, m, i * (m / g), m / g);
    free(sol);
    return total;
}

//Calculo da diversidade 
//A qualidade das soluções é calculada somando as 
//diversidades do subconjunto 1 + subconjunto 2 + subconjunto 3 etc
int calcula_div(int sol[], int** mat, int m, int start, int count)
{
    int div = 0, i, j;

    for (i = start; i < (start + count); i++)
        for (j = i + 1; j < (start + count); j++)
            div += obter_distancia(mat, m, sol[i], sol[j]);
    return div;
}