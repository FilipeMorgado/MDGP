#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"


#define DEFAULT_RUNS 100


enum TipoAlgoritmo
{
	algTrepaColinas,
    algTrepaColinasMelhorado
};


int main(int argc, char* argv[])
{
	char    nome_fich[100];
	int     m, g, num_iter, k, runs, custo, custo_best = 0;
	int  * sol, * best, **dist, i;
	float   mbf = 0.0;
    char nome_alg[100];

	enum TipoAlgoritmo algoritmo;
	struct info parameters;

	// Lê os argumentos de entrada
	if (argc == 3)
	{
		runs = atoi(argv[2]);
		strcpy(nome_fich, argv[1]);
	}
	else
		// Se o número de execuções do processo não for colocado nos argumentos de entrada, define-o com um valor por defeito
		if (argc == 2)
		{
			runs = DEFAULT_RUNS;
			strcpy(nome_fich, argv[1]);
		}
	// Se o nome do ficheiro de informações não for colocado nos argumentos de entrada, pede-o novamente
		else
		{
			runs = DEFAULT_RUNS;
			printf("Nome do Ficheiro: ");
			gets(nome_fich);
		}

	// Se o número de execuções do processo for menor ou igual a 0, termina o programa
	if (runs <= 0)
		return 0;

	init_rand();

	algoritmo = algTrepaColinas;
	num_iter = 1000;


	dist = init_dados(nome_fich, &m, &g);

	parameters.m = m; //Nr Elementos
	parameters.g = g; //Nr Sub-conjuntos

	printf("Elementos: %d\n", m);
	printf("Sub-conjuntos: %d\n", g);


    switch (algoritmo)
    {
    case algTrepaColinas:
        /*case algTrepaColinasProb:*/
        sol = calloc(m, sizeof(int));
        best = calloc(m, sizeof(int));

        if (sol == NULL || best == NULL)
        {
            printf("Erro na alocacao de memoria");
            exit(1);
        }

        for (k = 0; k < runs; k++)
        {
            // Gerar solucao inicial
            gera_sol_inicial(sol, m, g);

            switch (algoritmo)
            {
            case algTrepaColinas:
                // Trepa colinas simples
                strcpy(nome_alg, "Trepa Colinas");
                custo = trepa_colinas(sol, dist, m, g, num_iter);
                break;
            case algTrepaColinasMelhorado:
                // Trepa colinas melhorado
                strcpy(nome_alg, "Trepa Colinas");
                custo = trepa_colinasv2(sol, dist, m, g, num_iter);
                break;
            default:
                exit(0);
            }
            // Escreve resultados da repeticao k
            printf("\nRepeticao %d:", k);
            escreve_sol(sol, m, g);
            printf("Custo final: %2d\n", custo);

            mbf += custo;
            if (k == 0 || custo_best < custo)
            {
                custo_best = custo;
                copia(best, sol, m);
            }
        }

        // Escreve resultados globais
        printf("\n\nMBF: %f\n", mbf / k);
        printf("\nMelhor solucao encontrada");
        escreve_sol(best, m, g);
        printf("Custo final: %2d\n", custo_best);
        // Libertar memoria
        free(sol);
        free(best);
        break;
    }

    for (i = 0; i < (m - 1); i++)
        free(dist[i]);
    free(dist);
    return 0;

}