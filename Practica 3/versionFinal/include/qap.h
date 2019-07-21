#ifndef _QAP_H
#define _QAP_H

#define N_BUF 255
#define N_CROMOSOMAS 50

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
void GeneraSolucionAleatoria(int *sol, int n);
void greedy(QAP qap, int *sol,int &csol);
void greedy(QAP qap, int *sol,int &csol, float &t);
void inicializar(int **poblacion, int **padres, int n, int npadres);
void evaluar(QAP qap, int **poblacion, int *costes_poblacion, int *sol, int &csol);
void seleccion(QAP qap, int **poblacion, int *costes_poblacion,int**padres, int *costes_padres, int npadres);
void reemplazamientoG(QAP qap, int **poblacion, int **padres, int *costes_padres, int *sol, int csol);
void reemplazamientoE(QAP qap, int **poblacion, int *costes_poblacion, int **padres, int *costes_padres, int *sol, int csol);
void crucePosicion(QAP qap, int**padres,int ncruces);
void mutacionG(QAP qap, int **padres, int nmutaciones);
void mutacion(QAP qap, int **padres, int npadres, float pm);
void cruceOX(QAP qap, int**padres, int npadres, int ncruces, int nsubcadena);
void AGGposicion(QAP qap, int *sol, int &csol, float pc, float pm, float &t);
void AGEposicion(QAP qap, int *sol, int &csol, float pc, float pm, float &t);
void AGGox(QAP qap, int *sol, int &csol, float pc, float pm, float &t);
void AGEox(QAP qap, int *sol, int &csol, float pc, float pm, float &t);

#endif