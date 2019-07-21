#ifndef _QAP_H
#define _QAP_H

#define N_BUF 255
#define N_CROMOSOMAS 10

#include <vector>

using namespace std;

struct QAP{
  int n;
  int **flujo;
  int **distancia;
};

QAP leerCaso(const char *nombre_fichero);
void pintaVector(int *sol,int n);
void copia_solucion(int *s1, int *s2, int n);
void costo(QAP qap, int *sol, int &csol);
void costo_factorizado(QAP qap, int *sol, int r, int s, int &csol);
bool GeneraVecinos(int *sol, int n, int &r, int &s, int *vecino, bool reiniciar);
void GeneraVecinoAleatorio(int *sol, int n, int &r, int &s, int *vecino);
void GeneraVecinoAleatorio(int *sol, int n);
void GeneraSolucionAleatoria(int *sol, int n);
void greedy(QAP qap, int *sol,int &csol);
void greedy(QAP qap, int *sol,int &csol, float &t);
void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol, int &n_evaluaciones);
void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol, float &t, int &n_evaluaciones);
void inicializar(int **poblacion, int **padres, int n, int npadres);
void evaluar(QAP qap, int **poblacion, int *costes_poblacion, int *sol, int &csol, bool aplicar_local);
void seleccion(QAP qap, int **poblacion, int *costes_poblacion,int**padres, int *costes_padres, int npadres);
void reemplazamientoT(QAP qap, int **poblacion, int *costes_poblacion, int **padres, int *costes_padres, int *sol, int csol, int &n_evaluaciones, bool aplicar_local);
void reemplazamientoS(QAP qap, int **poblacion, int *costes_poblacion, int **padres, int *costes_padres, int *sol, int csol, int &n_evaluaciones, float pls, bool aplicar_local);
void reemplazamientoM(QAP qap, int **poblacion, int *costes_poblacion, int **padres, int *costes_padres, int *sol, int csol, int &n_evaluaciones, bool aplicar_local);
void crucePosicion(QAP qap, int**padres,int ncruces);
void mutacionG(QAP qap, int **padres, int nmutaciones);
void MemeticoTodos(QAP qap, int *sol, int &csol, float pc, float pm, float &t);
void MemeticoSubconjunto(QAP qap, int *sol, int &csol, float pc, float pm, float &t);
void MemeticoMejores(QAP qap, int *sol, int &csol, float pc, float pm, float &t);

#endif