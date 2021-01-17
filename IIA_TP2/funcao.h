/*
*   TP2 -> Introdução a Inteligência Artíficial - 2020-2021
*   Trabalho realizado por:
*       Filipe Morgado.:  2019137625
*       André Domingues.: 2019127839
*/
#pragma once
int calcula_fit(int a[], int** mat, struct info d);
int calcula_fit_penalizado(int a[], int** mat, struct info d);
int calcula_div(int sol[], int** mat, int m, int start, int count);

