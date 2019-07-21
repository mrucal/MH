#ifndef _QAP_H
#define _QAP_H

#define N_BUF 255

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
void MutacionILS(int *sol, int *sol_mutada,int n);
void greedy(QAP qap, int *sol,int &csol);
void greedy(QAP qap, int *sol,int &csol, float &t);
void greedyAleatorio(QAP qap, int *sol, float alfa, int &csol);
void greedyAleatorio(QAP qap, int *sol,int &csol, float &t);
void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol);
void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol, float &t);
void BusquedaMultiarranqueBasica(QAP qap, int *sol, int &csol, float &t);
void GRASP(QAP qap, int *sol, int &csol, float &t);
void BusquedaLocalReiterada(QAP qap, int *sol, int &csol, float &t);

#endif