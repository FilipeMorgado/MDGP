#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

#define PROB 0.001
#define TMAX 100
#define TMIN 0.01

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
void gera_vizinho(int a[], int b[], int n)
{
    int p1, p2;
    copia(b, a, n);

    // Elemento a trocar
    p1 = random_l_h(0, n - 1);

    // Encontra posicao com valor 0
    do {
        p2 = random_l_h(0, n - 1);
    }while (b[p2] == a[p1]);

    // Troca os valores das posicoes encontradas
    troca(b, p1, p2);
}

// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int trepa_colinas(int sol[], int** mat, struct info d, int num_iter)
{
    int* nova_sol, custo, custo_viz, i, invalidos = 1;
    
    nova_sol = malloc(sizeof(int) * d.m);
    if (nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, d, invalidos);

    for (i = 0; i < num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, d.m);

        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, d, invalidos);

        // Aceita vizinho se o custo aumentar (problema de maximizacao)
        if (custo_viz > custo)
        {
            copia(sol, nova_sol, d.m);
            custo = custo_viz;
        }
    }
    //Libertacao de memoria alocada
    free(nova_sol);
    
    return custo;
}


// Trepa colinas 2 vizinhanças
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int trepa_colinas2viz(int sol[], int** mat, struct info d, int num_iter)
{
    int *nova_sol,*nova_sol2, custo, custo_viz, custo_viz2, i , invalidos = 1;

    nova_sol = malloc(sizeof(int) * d.m);
    nova_sol2 = malloc(sizeof(int) * d.m);

    if (nova_sol == NULL || nova_sol2 == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, d, invalidos);

    for (i = 0; i < num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, d.m);
        gera_vizinho(sol, nova_sol2, d.m);

        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat,d, invalidos);
        custo_viz2 = calcula_fit(nova_sol2, mat, d, invalidos);

        // Aceita vizinho se o custo aumentar (problema de maximizacao)
        // Aceita também caso seja igual, para evitar maximos locais (planicies)
        if (custo_viz >= custo && custo_viz >= custo_viz2)
        {
            copia(sol, nova_sol, d.m);
            custo = custo_viz;
        }else if (custo_viz2 >= custo && custo_viz2 >= custo_viz)
        {
            copia(sol, nova_sol2, d.m);
            custo = custo_viz2;
        }
    }        
    //Libertacao de memoria alocada
    free(nova_sol);
    free(nova_sol2);

    return custo;
}


// Trepa colinas Probabilistico
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int trepaColinas_probabilistico(int sol[], int** mat, struct info d, int num_iter){
    int* nova_sol, custo, custo_viz, i, invalidos = 1;


    nova_sol = malloc(sizeof(int) * d.m);
    if (nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, d, invalidos);

    for (i = 0; i < num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, d.m);

        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, d, invalidos);

        // Aceita vizinho se o custo aumentar (problema de maximizacao)
        // Aceita também caso seja igual, para evitar maximos locais (planicies)
        if (custo_viz >= custo)
        {
            copia(sol, nova_sol, d.m);
            custo = custo_viz;
        }
        else
            //Com uma probabilidade de "PROB" o algoritmo aceita soluções piores
            //de modo a evitar máximos locais
            if (rand_01() < PROB) {
                copia(sol, nova_sol, d.m);
                custo = custo_viz;
            }
    }
    //Libertacao de memoria alocada
    free(nova_sol);

    return custo;
}

// Recristalizacao Simulada
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int recristalizacao_simulada(int sol[], int** mat, struct info d, int num_iter)
{
    int* nova_sol, * best_sol, custo, custo_best, custo_viz, i, invalidos = 1;
    double eprob, temperatura, r;

    nova_sol = malloc(sizeof(int) * d.m);
    best_sol = malloc(sizeof(int) * d.m);

    // Avalia solucao inicial
    custo = calcula_fit(sol, mat,d, invalidos);

    copia(best_sol, sol, d.m);
    custo_best = custo;
    // Inicializa temperatura
    temperatura = TMAX;
    for (i = 0; i < num_iter; i++) {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, d.m);
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, d, invalidos);
        // Calcular probabilidade: maximizacao
        eprob = exp((custo - custo_viz) / temperatura);
        
        // Se a solução vizinha for melhor que a solução atual, guarda-a como a melhor solução
        if (custo_viz > custo_best)
        {
            custo_best = custo_viz;
            copia(best_sol, sol, d.m);
        }

        // Aceita vizinho se o custo aumentar (problema de maximizacao)
        // Aceita também caso seja igual, para evitar maximos locais (planicies)
        if (custo_viz >= custo)
        {
            custo = custo_viz;
            copia(sol, nova_sol, d.m);
        }
        else
        {
            //Com uma probabilidade de "PROB" o algoritmo aceita soluções piores
            //de modo a evitar máximos locais
            r = rand_01();
            if (r < eprob)
            {
                copia(sol, nova_sol, d.m);
                custo = custo_viz;
            }
        }
        // Arrefecimento
        temperatura -= (TMAX - TMIN) / num_iter;
    }

    copia(sol, best_sol, d.m);
    custo = custo_best;

    //Libertacao de memoria alocada
    free(nova_sol);
    free(best_sol);

    return custo;
}



// Tabu
// Parametros: solucao, matriz de adjacencias, numero de vertices, numero de iteracoes e numero de descidas tabu
// Devolve o custo da melhor solucao encontrada
int pesquisa_tabu(int sol[], int** mat, struct info d, int num_iter, int flagChangeIter)
{
    int* nova_sol, custo, custo_viz, custoGlobal = 0, indexTab = 0, i = 0, inc = 1, invalidos = 1;

    nova_sol = malloc(sizeof(int) * d.m);
    if (nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, d, invalidos);

    for (i = 0; i < num_iter;)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, d.m);

        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, d, invalidos);

        // Aceita vizinho se o custo aumentar (problema de maximizacao)
        if (custo_viz > custo)
        {
            indexTab = 0;
            custo = custo_viz;
            if (custo > custoGlobal)
            {
                custoGlobal = custo_viz;
                copia(sol, nova_sol, d.m);
            }
            inc = 1;
        }
        else
        {
            if (d.numTabuDescidas > indexTab)
            {
                if (flagChangeIter == 0)
                    inc = 1;
                else
                    inc = 0;

                custo = custo_viz;
                indexTab++;
            }
            else
                inc = 1;
        }
        if (custoGlobal > custo)
            custo = custoGlobal;

        if (inc == 1)
            i++;
    }

    free(nova_sol);

    return custo;
}


/* EVOLUTIVO */

// Seleccao por torneio de tamanho t_size
// Argumentos: populacao actual, parametros, pais
// A funcao preenche o vector pais de acordo com o resultados dos torneios
void tournament_geral(pchrom pop, struct info d, pchrom parents)
{
    int i, j, max, *pos;

    pos = malloc(sizeof(int) * d.t_size);
    if (!pos) return;

    // Realiza popsize torneios
    for (i = 0; i < d.popsize; i++)
    {
        // Seleciona tsize soluções diferentes para entrarem em torneio de seleção
        for (j = 0; j < d.t_size; j++)
        {
            pos[j] = random_l_h(0, d.popsize - 1);
        }
        max = pos[0];
        for (j = 1; j < d.t_size; j++)
        {
            // Problema de maximizacao: só sai um pai
            if ((pop + pos[j])->fitness > (pop + max)->fitness)
                max = pos[j];
        }
        atribui(parents + i, *(pop + max), d);
    }
    free(pos);
}

// Operadores geneticos
// Argumentos: pais, estrutura com parametros, descendentes
void genetic_operators(pchrom parents, struct info d, pchrom offspring, int** dist)
{
    // Recombinação com 2 pontos de corte
    recombination(parents, d, offspring, dist);
    //recombinacao 1 ponto de corte
    recombination_crossover(parents, d, offspring, dist);
    // recombinacao uniforme
    recombination_uniforme(parents, d, offspring, dist);
    // Mutação por troca
    mutation(d, offspring);
    // mutacao binaria
    mutation_binary(d, offspring);
}

// Chama a funcao cx_order que implementa a recombinacao por ordem (com probabilidade pr)
// Argumentos: pais, estrutura com parametros, descendentes
// A funcao preenche o vector descendentes com o resultado das operacoes de crossover
// recombinaçao 2 pontos de corte
void recombination(pchrom parents, struct info d, pchrom offspring, int** dist)
{
    int i;

    for (i = 0; i < d.popsize; i += 2)
    {
        if (rand_01() < d.pr)
        {
            // Recombinar
            rec_ordenar((parents + i)->sol, (parents + i + 1)->sol, (offspring + i)->sol, (offspring + i + 1)->sol, d);
        }
        else
        {
            // Sem recombinacao
            atribui(offspring + i, *(parents + i), d);
            atribui(offspring + i + 1, *(parents + i + 1), d);
        }

        (offspring + i)->fitness = (offspring + i + 1)->fitness = 0;
    }
}
// recombinacaço com 1 ponto de corte
void recombination_crossover(pchrom parents, struct info d, pchrom offspring, int** dist)
{
    int i;

    for (i = 0; i < d.popsize; i += 2)
    {
        if (rand_01() < d.pr)
        {
            // Recombinar
            rec_ordenar_crossover((parents + i)->sol, (parents + i + 1)->sol, (offspring + i)->sol, (offspring + i + 1)->sol, d);
        }
        else
        {
            // Sem recombinacao
            atribui(offspring + i, *(parents + i), d);
            atribui(offspring + i + 1, *(parents + i + 1), d);
        }

        (offspring + i)->fitness = (offspring + i + 1)->fitness = 0;
    }
}
// recombinacao uniforme
void recombination_uniforme(pchrom parents, struct info d, pchrom offspring, int** dist)
{
    int i;

    for (i = 0; i < d.popsize; i += 2)
    {
        if (flip() < d.pr)
        {
            // Recombinar
            rec_ordenar_crossover((parents + i)->sol, (parents + i + 1)->sol, (offspring + i)->sol, (offspring + i + 1)->sol, d);
        }
        else
        {
            // Sem recombinacao
            atribui(offspring + i, *(parents + i), d);
            atribui(offspring + i + 1, *(parents + i + 1), d);
        }

        (offspring + i)->fitness = (offspring + i + 1)->fitness = 0;
    }
}


// Chama as funcoes que implementam as operacoes de mutacao (de acordo com as respectivas probabilidades)
// Argumentos: estrutura de parametros e descendentes
// Na versao disponibilizada, apenas a mutacao swap esta implementada
void mutation(struct info d, pchrom offspring)
{
    int i;
    for (i = 0; i < d.popsize; i++)
    {
        if (rand_01() < d.pm_swap)
            mutation_swap(d, (offspring + i)->sol);
    }
}

void mutation_binary(struct info d, pchrom offspring) {
    
    int i, j;
    for (i = 0; i < d.popsize; i++)
        for (j = 0; j < d.m; j++)
            if (rand_01() < d.pm_swap)
                offspring[i].sol[j] = !offspring[i].sol[j];
}


// Mutacao swap
// Argumentos: estrutura de parametros e solucao a alterar
void mutation_swap(struct info d, int a[])
{
    int x, y, z;
    
    x = random_l_h(0, d.m - 1);
    do {
        y = random_l_h(0, d.m - 1);
    } while (x == y || a[x] == a[y]);

    z = a[x];
    a[x] = a[y];
    a[y] = z;
}
// 
void rec_ordenar_crossover(int p1[], int p2[], int d1[], int d2[], struct info d)
{
    int i, aceites, * tab1, * tab2, * conj;
    double prob = 0.5, r;

    tab1 = (int*)malloc(d.m * sizeof(int));
    if (!tab1)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    tab2 = (int*)malloc(d.m * sizeof(int));
    if (!tab2)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    conj = (int*)malloc(d.g * sizeof(int));
    if (!conj)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Primeiro descendente
    i = 0;
    aceites = 0;
    while (1)
    {
        // Verificar
        if (aceites >= d.m)
            break;

        // Ultimo
        if (i >= d.m)
        {
            i = 0;
            continue;
        }
        r = rand_01();
        if (r < prob)
        {
            // Verifica se ja ultrapassa do limite
            if (conj[p1[i]] >= (d.m / d.g) || tab1[i] == 1)
            {
                if (conj[p2[i]] >= (d.m / d.g) || tab2[i] == 1)
                {
                    // Ultimo: Proteccao
                    if (i == d.m - 1)
                        i = 0;
                    else
                        i++;
                    continue;
                }
                else
                {
                    d1[aceites] = p2[i];
                    tab2[i] = 1;
                }
            }
            else
            {
                d1[aceites] = p1[i];
                tab1[i] = 1;
            }
        }
        else
        {
            // Verifica se ja ultrapassa do limite
            if (conj[p2[i]] >= (d.m / d.g) || tab2[i] == 1)
            {
                if (conj[p1[i]] >= (d.m / d.g) || tab1[i] == 1)
                {
                    // Ultimo: Proteccao
                    if (i == d.m - 1)
                        i = 0;
                    else
                        i++;
                    continue;
                }
                else
                {
                    d1[aceites] = p1[i];
                    tab1[i] = 1;
                }
            }
            else
            {
                d1[aceites] = p2[i];
                tab2[i] = 1;
            }
        }

        conj[d1[aceites]]++;
        aceites++;
        i++;
    }
    
    // Liberta memória
    free(conj);
    free(tab1);
    free(tab2);
}




// Recombinacao por ordem
// Argumentos: pai1, pai2, descendente1, descendente2, estrutura com parametros
void rec_ordenar(int p1[], int p2[], int d1[], int d2[], struct info d)
{
    int i, aceites, * tab1, * tab2, * conj;
    double prob = 0.5, r;

    tab1 = (int*)malloc(d.m * sizeof(int));
    if (!tab1)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    tab2 = (int*)malloc(d.m * sizeof(int));
    if (!tab2)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    conj = (int*)malloc(d.g * sizeof(int));
    if (!conj)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Primeiro descendente
    i = 0;
    aceites = 0;
    while (1)
    {
        // Verificar
        if (aceites >= d.m)
            break;

        // Ultimo
        if (i >= d.m)
        {
            i = 0;
            continue;
        }
        r = rand_01();
        if (r < prob)
        {
            // Verifica se ja ultrapassa do limite
            if (conj[p1[i]] >= (d.m / d.g) || tab1[i] == 1)
            {
                if (conj[p2[i]] >= (d.m / d.g) || tab2[i] == 1)
                {
                    // Ultimo: Proteccao
                    if (i == d.m - 1)
                        i = 0;
                    else
                        i++;
                    continue;
                }
                else
                {
                    d1[aceites] = p2[i];
                    tab2[i] = 1;
                }
            }
            else
            {
                d1[aceites] = p1[i];
                tab1[i] = 1;
            }
        }
        else
        {
            // Verifica se ja ultrapassa do limite
            if (conj[p2[i]] >= (d.m / d.g) || tab2[i] == 1)
            {
                if (conj[p1[i]] >= (d.m / d.g) || tab1[i] == 1)
                {
                    // Ultimo: Proteccao
                    if (i == d.m - 1)
                        i = 0;
                    else
                        i++;
                    continue;
                }
                else
                {
                    d1[aceites] = p1[i];
                    tab1[i] = 1;
                }
            }
            else
            {
                d1[aceites] = p2[i];
                tab2[i] = 1;
            }
        }

        conj[d1[aceites]]++;
        aceites++;
        i++;
    }
    // Segundo descendente
    i = 0;
    aceites = 0;
    while (i < d.m)
    {
        // Proteccao
        if (aceites >= d.m)
            break;

        // Restantes para o descendente 2
        if (tab1[i] == 0)
        {
            d2[aceites++] = p1[i];
        }
        if (tab2[i] == 0)
        {
            d2[aceites++] = p2[i];
        }
        i++;
    }
    // Liberta memória
    free(conj);
    free(tab1);
    free(tab2);
}