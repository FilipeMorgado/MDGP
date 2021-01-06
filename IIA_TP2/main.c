#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

#define DEFAULT_RUNS 50

enum TipoAlgoritmo
{
    //Local
	algTrepaColinas,
    algTrepaColinas2viz,
    algTrepaColinasProb,
    algRecristalizacaoSimulada,
    //Evolucionario
    algEvolucionarioGenetico,
    //Hibrido
    algHibridoTrepaSimples,
    algHibridoTrepa2viz,
    algHibridoTrepaProbabilistico
};

int main(int argc, char* argv[])
{
	char    nome_fich[100];
	int     m, g, num_iter, k, runs, custo, best_custo = 0;
	int  * sol, * best, **dist, i;
	float   mbf = 0.0;
    char nome_alg[100];

    //Local
	enum TipoAlgoritmo algoritmo;
	struct info parameters;

    /*Revolucionario*/
    int gen_actual;
    chrom best_run, best_ever;
    pchrom pop = NULL, parents = NULL;

	// Lê os argumentos de entrada
    while (1) {
        mbf = 0.0;
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

        algoritmo = algRecristalizacaoSimulada;
        num_iter = 5000;

        /* Evolutivo */
        parameters.numTabuDescidas = 5;
        parameters.numGenerations = 2100;
        parameters.popsize = 100;
        parameters.pm_swap = 0.025;
        parameters.pr = 0.25;
        parameters.t_size = 3;

        dist = init_dados(nome_fich, &m, &g);

        parameters.m = m; //Nr Elementos
        parameters.g = g; //Nr Sub-conjuntos

        printf("Elementos: %d\n", m);
        printf("Sub-conjuntos: %d\n", g);


        switch (algoritmo)
        {
        case algTrepaColinas:
        case algTrepaColinas2viz:
        case algTrepaColinasProb:
        case algRecristalizacaoSimulada:
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
                case algTrepaColinas2viz:
                    // Trepa colinas melhorado + probabilistico
                    strcpy(nome_alg, "Trepa Colinas 2 vizinhos");
                    custo = trepa_colinasv2(sol, dist, m, g, num_iter);
                    break;
                case  algTrepaColinasProb:
                    strcpy(nome_alg, "Trepa Colinas Probabilistico");
                    custo = tc_prob(sol, dist, m, g, num_iter);
                    break;
                case algRecristalizacaoSimulada:
                    // Trepa colinas Recristalização Simulada
                    strcpy(nome_alg, "Recristalizacao simulada");
                    custo = tc_simulated_annealing(sol, dist, m, g, num_iter);
                    break;
                default:
                    exit(0);
                }
                // Escreve resultados da repeticao k
                printf("\nRepeticao %d:", k);
                escreve_sol(sol, m, g);
                printf("Custo final: %2d\n", custo);

                mbf += custo;
                if (k == 0 || best_custo < custo)
                {
                    best_custo = custo;
                    copia(best, sol, m);
                }
            }
            // Escreve resultados globais
            printf("\n\nMBF: %f\n", mbf / k);
            printf("\nMelhor solucao encontrada");
            escreve_sol(best, m, g);
            printf("Custo final: %2d\n", best_custo);
            // Libertar memoria
            free(sol);
            free(best);
            break;

        //Evolucionario
        case algEvolucionarioGenetico:
        //Hibridos
        case algHibridoTrepaSimples:
        case algHibridoTrepa2viz:
        case algHibridoTrepaProbabilistico:
            strcpy(nome_alg, "Evolucionario");

            best_run.sol = calloc(m, sizeof(int));
            best_ever.sol = calloc(m, sizeof(int));

            for (k = 0; k < runs; k++)
            {
                pop = init_pop(parameters, dist);

                // AVALIAR a qualidade da populacao
                evaluate(pop, parameters, dist);

                // ALTERAR PARA COMO A AULA FOI FEITO
                atribuicao(&best_run, pop[0], parameters);

                // Inicializar a melhor solucao encontrada
                get_best(pop, parameters, &best_run);

                // Reservar espaco para os pais
                parents = init_parents(parameters);
                gen_actual = 1;


                // Main evolutionary loop
                while (gen_actual <= parameters.numGenerations)
                {
                    // Torneio binario para encontrar os progenitores (ficam armazenados no vector parents)
                    sized_tournament(pop, parameters, parents);

                    // Aplicar operadores geneticos aos pais (os descendentes ficam armazenados no vector pop)
                    genetic_operators(parents, parameters, pop, dist);

                    // Reavaliar a qualidade da populacao
                    evaluate(pop, parameters, dist);

                    // Actualizar a melhor solucao encontrada
                    get_best(pop, parameters, &best_run);

                    gen_actual++;
                }

                // HIBRIDO: tentar melhorar solução com pesquisa local
                if (algoritmo == algHibridoTrepaSimples)
                {
                    strcpy(nome_alg, "Genetico por torneio + trepa colinas");
                    // Trepa colinas simples
                    best_run.fitness = trepa_colinas(best_run.sol, dist, m, g, 1000/*Melhores resultados*/);
                }
                if (algoritmo == algHibridoTrepa2viz)
                {
                    strcpy(nome_alg, "Genetico por torneio + trepa colinas 2 vizinhos");
                    // Trepa colinas Melhorado
                    best_run.fitness = trepa_colinasv2(best_run.sol, dist, m, g, 1000/*Melhores resultados*/);
                }
                if (algoritmo == algHibridoTrepaProbabilistico)
                {
                    strcpy(nome_alg, "Genetico por torneio + trepa colinas Probabilistico");
                    // Trepa colinas Probabilistico
                    best_run.fitness = tc_prob(best_run.sol, dist, m, g, 1000/*Melhores resultados*/);
                }

                // Escreve resultados da repeticao que terminou
                printf("\nRepeticao %d:", k);
                escreve_sol(best_run.sol, m, g);
                printf("Custo: %2d\n", best_run.fitness);

                mbf += best_run.fitness;
                if (k == 0 || best_ever.fitness < best_run.fitness)
                {
                    atribuicao(&best_ever, best_run, parameters);
                }

                // Libertação de memoria da população
                for (i = 0; i < parameters.popsize; i++)
                    free(pop[i].sol);
                free(pop);

                // Libertação de memoria dos pais
                for (i = 0; i < parameters.popsize; i++)
                    free(parents[i].sol);
                free(parents);
            }

            // Escreve resultados globais
            printf("\n\nMBF: %f\n", mbf / k);
            printf("\nMelhor solucao encontrada");
            escreve_sol(best_ever.sol, m, g);
            printf("Custo final: %2d\n", best_ever.fitness);

            free(best_run.sol);
            free(best_ever.sol);
            break;
        }

        for (i = 0; i < (m - 1); i++)
            free(dist[i]);
        free(dist);
    }
    return 0;
}