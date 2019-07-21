#ifndef _QUICKSORT_H
#define _QUICKSORT_H

#define UMBRAL_QS 10

void intercambia (int &x, int &y);
void insercion(int *v, int inicial, int final_, int *indices);
void quicksort(int *v, int n, int *indices);
void quicksort_lims(int *v, int inicial, int final_, int *indices);
void dividir_qs(int *v, int inicial, int final_, int *pp, int *indices);

#endif