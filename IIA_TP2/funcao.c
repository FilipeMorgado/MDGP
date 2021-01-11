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
int calcula_fit(int a[], int** mat, struct info d,int *invalidos)
{
    int* sol = 0, total = 0, i, ind = 0, subc = 0, cont;
    int n = d.m / d.g;

    // Coloca a solucao num vector
    sol = malloc(sizeof(int) * d.m);
    for (subc = 0; subc < d.g; subc++) {
        for (i = 0, cont = 0; i < d.m; i++) {
            if (a[i] == subc) {
                sol[ind] = i;
                ind++;
                ++cont;
            }
        }
        if (cont != n) {
            return 0;
        }
    }
        

    // qualidade = diversidade(0) + diversidade(1) + etc
    for (i = 0; i < d.g; i++)
        total += calcula_div(sol, mat, d.m, i * (d.m / d.g), d.m / d.g);

    free(sol);
    return total;
}


int calcula_fit_penalizado(int a[], int** mat, struct info d, int* invalidos)
{
    int* sol = 0, total = 0, i, flagDeMerda = 0, ind = 0, subc = 0, cont = 0, ro = 0;
    int n = d.m / d.g;
    // Coloca a solucao num vector
    sol = malloc(sizeof(int) * d.m);
    for (subc = 0; subc < d.g; subc++) {
        for (i = 0, cont = 0; i < d.m; i++) {
            if (a[i] == subc) {
                sol[ind] = i;
                ind++;
                ++cont;
            }
        }
        if (cont != n) {
            /*printf("\nSOLUCAO INVALIDA: \n");
            escreve_sol(a, m, g);*/
            flagDeMerda = 1;
        }
    }
    if(flagDeMerda == 1){
        for (i = 0; i < d.g; i++)
            total += calcula_div(sol, mat, d.m, i * (d.m / d.g), d.m / d.g);
        /*printf("\nSOLUCAO INVALIDA: \n");
        escreve_sol(a, m, g);*/
        *invalidos = 0;
        ro = ceil(n / d.g);
        free(sol);
        //printf("\nTOTAL: %d", total * m * 10);
        return total - ro * d.m * 100;
    }

    // qualidade = diversidade(0) + diversidade(1) + etc
    for (i = 0; i < d.g; i++)
        total += calcula_div(sol, mat, d.m, i * (d.m / d.g),d.m / d.g);

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