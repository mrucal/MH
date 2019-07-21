#ifndef _QAP_H
#define _QAP_H

#define N_BUF 255

#include <vector>

struct QAP{
  int n;
  int **flujo;
  int **distancia;
};

struct Tabu{
	int i;
	int j;
	int posi;
	int posj;

	Tabu(){
		i=j=posi=posj=0;
	}
	Tabu(int r, int s, int posr, int poss){
		i=r;
		j=s;
		posi=posr;
		posj=poss;
	}

	bool operator==(const Tabu &t){
		return (t.i==i && t.j==j && t.posi==posi && t.posj==posj) ||
				(t.i==j && t.j==i && t.posi==posj && t.posj==posi);
	}
};

QAP leerCaso(const char *fichero);
void pintaVector(int *sol,int n);
void costo(QAP qap, int *sol, int &csol);
void greedy(QAP qap, int *sol,int &csol, float &t);
void copia_solucion(int *s1, int *s2, int n);
bool GeneraVecinos(int *sol, int n, int &r, int &s, int *vecino, bool reiniciar);
void GeneraVecinoAleatorio(int *sol, int n, int &r, int &s, int *vecino);
void costo_factorizado(QAP qap, int *sol, int r, int s, int &csol);
void GeneraAleatorio(int *sol, int n);
void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol, float &t);
float calcularTemperatura(float T, float Tinicial, float Tfinal, int n_enfriamientos);
void EnfiramientoSimulado(QAP qap, int *sol, int &csol, float mi, float theta, float max_vecinos, float max_exitos, float &t);
bool esTabu(Tabu t, std::vector<Tabu> lt);
void actualizaListaTabu(Tabu t, std::vector<Tabu> &lt, int tenencia, int &i);
void actualizaFrecuencia(int **frec, int *sact, int n);
void reinicializarDiversidad(int *sact,int **frec,int n);
void reinicializar(int *sact, int *mejor, int **frec,int n,std::vector<Tabu> &lt, int &tenencia);
void BusquedaTabu(QAP qap, int *sol, int &csol, float &t);

#endif