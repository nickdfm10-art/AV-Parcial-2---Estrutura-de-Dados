// =====================================================================
// Eurotrip - Estruturas de Dados Nao Lineares (Avaliativa Parcial 2)
// Grafo nao direcionado e ponderado: cidades europeias + ponto de partida (Sao Paulo)
// Pesos por aresta: custo (EUR), distancia (km) e tempo (horas)
//
// Compilar: g++ -std=c++11 -O2 -o eurotrip eurotrip.cpp   (funciona tambem em C++14/17)
// Executar: ./eurotrip            (destino padrao: Berlim)
//           ./eurotrip Roma       (destino escolhido pelo usuario)
// =====================================================================
#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

using namespace std;

const double INF = numeric_limits<double>::infinity();

// Criterio usado para escolher qual peso da aresta sera minimizado
enum Criterio { CUSTO = 0, DISTANCIA = 1, TEMPO = 2 };
const char* NOME_CRITERIO[3] = {"CUSTO (EUR)", "DISTANCIA (km)", "TEMPO (h)"};
