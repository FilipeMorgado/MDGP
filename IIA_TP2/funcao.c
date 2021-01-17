/*
*   TP2 -> Introdução a Inteligência Artíficial - 2020-2021
*   Trabalho realizado por:
*       Filipe Morgado.:  2019137625
*       André Domingues.: 2019127839
*/
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
int calcula_fit(int a[], int** mat, struct info d)
{
    int* sol = 0, total = 0, i = 0, ind = 0, subc = 0, cont;
    int n = d.m / d.g;

    // Coloca a solucao num vector
    sol = malloc(sizeof(int) * d.m);
    if (sol) {
        for (subc = 0; subc < d.g; subc++) {
            for (i = 0, cont = 0; i < d.m; i++) {
                if (a[i] == subc) {
                    sol[ind] = i;
                    ind++;
                    ++cont;
                }
            }
            if (cont != n) {
                free(sol);
                return 0;
            }
        }
    }

    // Calculo da qualidade. qualidade = diversidade(0) + diversidade(1) + etc
    for (i = 0; i < d.g; i++)
        total += calcula_div(sol, mat, d.m, i * (d.m / d.g), d.m / d.g);

    free(sol);
    return total;
}

// Calcula a qualidade de uma solucao com penalizacao
// Recebe: solucao, matriz, nr de elementos, nr de sub-conjuntos
// Devolve a fitness
int calcula_fit_penalizado(int a[], int** mat, struct info d)
{
    int* sol = 0, total = 0, i, flag = 0, ind = 0, subc = 0, cont = 0;
    float ro = 0;
    int n = (d.m / d.g);
    ro = (float)ceil(n / d.g);
    // Coloca a solucao num vector
    sol = malloc(sizeof(int) * d.m);
    if (sol) {
        for (subc = 0; subc < d.g; subc++) {
            for (i = 0, cont = 0; i < d.m; i++) {
                if (a[i] == subc) {
                    sol[ind] = i;
                    ind++;
                    ++cont;
                }
            }
            if (cont != n) {
                flag = 1;
            }
        }
    }

    if(flag == 1){
        for (i = 0; i < d.g; i++)
            total += calcula_div(sol, mat, d.m, i * (d.m / d.g), d.m / d.g);
        //*invalidos = 0;
        free(sol);
        return (int)(total - ro * d.m * 100);
    }

    for (i = 0; i < d.g; i++)
        total += calcula_div(sol, mat, d.m, i * (d.m / d.g),d.m / d.g);
    //*invalidos = 1;
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