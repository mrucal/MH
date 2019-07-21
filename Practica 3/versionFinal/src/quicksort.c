#include <stdio.h>
#include <stdlib.h>

#include "quicksort.h"

void intercambia (int &x, int &y){
  
  int aux=x;
  x=y;
  y=aux;
  
}

void insercion(int *v, int inicial, int final_, int *indices){  
  
  int i,j;
  
  for (i = inicial + 1; i < final_; i++) 
    for(j=i;(v[j] < v[j-1]) && (j > 0);j--){
      intercambia(v[j],v[j-1]);    
      intercambia(indices[j],indices[j-1]); 
      
    }
    
}

void quicksort(int *v, int n, int *indices){
  for(int i=0;i<n;i++) indices[i]=i;
  quicksort_lims(v, 0, n,indices);
    
}

void quicksort_lims(int *v, int inicial, int final_, int *indices){
  
  int k;
  
  if (final_ - inicial < UMBRAL_QS) {
    
    insercion(v, inicial, final_,indices);
    
  } else {
    
    dividir_qs(v, inicial, final_, &k,indices);
    quicksort_lims(v, inicial, k,indices);
    quicksort_lims(v, k + 1, final_,indices);
    
  }
  
}

void dividir_qs(int *v, int inicial, int final_, int *pp, int *indices){
  
  int pivote;
  int k, l;

  pivote = v[inicial];
  
  for(k=inicial+1;(v[k] <= pivote) && (k < final_-1);k++);
  for(l=final_-1;v[l] > pivote; l--);
    
  while (k < l) {
    intercambia(v[k],v[l]);
    intercambia(indices[k],indices[l]);
    do k++; while (v[k] <= pivote);
    do l--; while (v[l] > pivote);
  }
  
  intercambia(v[inicial],v[l]);
  intercambia(indices[inicial],indices[l]);
  *pp = l;
  
}