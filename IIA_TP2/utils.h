#pragma once

int** init_dados(char* nome, int* m, int* g);
void gera_sol_inicial(int* sol, int m, int g);
void escreve_sol(int* sol, int m, int g);

int obter_distancia(int** dist, int m, int a, int b);
void copia(int a[], int b[], int n);
void troca(int* sol, int a, int b);


void init_rand();
int random_l_h(int min, int max);
float rand_01();