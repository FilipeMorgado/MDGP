#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"



// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
void gera_vizinho(int a[], int b[], int n)
{
    int p1, p2;

    copia(b, a, n);

    // Elemento a trocar
    p1 = random_l_h(0, n - 1);

    // Encontra posicao com valor 0
    do
        p2 = random_l_h(0, n - 1);
    while (b[p2] == a[p1]);

    // Troca
    troca(b, p1, p2);
}



// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int trepa_colinas(int sol[], int** mat, int m, int g, int num_iter)
{
    int* nova_sol, custo, custo_viz, i;

    
    nova_sol = malloc(sizeof(int) * m);
    if (nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, m, g);

    for (i = 0; i < num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, m);

        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, m, g);

        // Aceita vizinho se o custo aumentar (problema de maximizacao)
        if (custo_viz > custo)
        {
            copia(sol, nova_sol, m);
            custo = custo_viz;
        }
    }

    free(nova_sol);

    return custo;
}


// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int trepa_colinasv2(int sol[], int** mat, int m, int g, int num_iter)
{
    int *nova_sol,*nova_sol2, custo, custo_viz, custo_viz2, i;


    nova_sol = malloc(sizeof(int) * m);
    nova_sol2 = malloc(sizeof(int) * m);
    if (nova_sol == NULL || nova_sol2 == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, m, g);


    for (i = 0; i < num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, m);
        gera_vizinho(sol, nova_sol2, m);

        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, m, g);
        custo_viz2 = calcula_fit(nova_sol2, mat, m, g);


        if (custo_viz >= custo && custo_viz >= custo_viz2)
        {
            copia(sol, nova_sol, m);
            custo = custo_viz;
        }else if (custo_viz2 >= custo && custo_viz2 >= custo_viz)
        {
            copia(sol, nova_sol2, m);
            custo = custo_viz2;
        }
        else {
        if (rand_01() < 0.001) {
            copia(sol, nova_sol, m);
            custo = custo_viz;
        }
    }       

        // Aceita vizinho se o custo aumentar (problema de maximizacao)
        /*if (custo_viz > custo)
        {
            copia(sol, nova_sol, m);
            custo = custo_viz;
        }*/
    }

    free(nova_sol);
    free(nova_sol2);

    return custo;
}