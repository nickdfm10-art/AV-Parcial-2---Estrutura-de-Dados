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

// ------------------------------- Estruturas --------------------------
struct Aresta {
    int u, v;            // extremidades
    double peso[3];      // peso[CUSTO], peso[DISTANCIA], peso[TEMPO]
    string transporte;   // aviao, trem...
};

struct Grafo {
    int n = 0;
    vector<string> nomes;                 // nome de cada vertice
    vector<string> pais;                  // pais de cada vertice
    vector<string> pontos;                // pontos turisticos de cada vertice
    vector<Aresta> arestas;               // lista de arestas (usada no Kruskal)
    vector<vector<pair<int,int>>> adj;    // lista de adjacencia: (vizinho, indice da aresta)

    int addVertice(const string& nome, const string& p, const string& turismo) {
        nomes.push_back(nome);
        pais.push_back(p);
        pontos.push_back(turismo);
        adj.emplace_back();
        return n++;
    }

    void addAresta(int u, int v, double custo, double dist, double tempo, const string& transp) {
        Aresta a{u, v, {custo, dist, tempo}, transp};
        int id = (int)arestas.size();
        arestas.push_back(a);
        adj[u].push_back({v, id});   // grafo nao direcionado: nos dois sentidos
        adj[v].push_back({u, id});
    }

    int indice(const string& nome) const {
        for (int i = 0; i < n; i++) if (nomes[i] == nome) return i;
        return -1;
    }
};
