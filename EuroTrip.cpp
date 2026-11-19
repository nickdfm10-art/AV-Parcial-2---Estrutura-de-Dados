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

// ------------------------------ Construcao ---------------------------
Grafo construirGrafo() {
    Grafo g;
    // vertices: cidade, pais, pontos turisticos principais
    int sp  = g.addVertice("Sao Paulo", "Brasil",   "Ponto de partida (GRU)");
    int lis = g.addVertice("Lisboa",    "Portugal", "Torre de Belem, Mosteiro dos Jeronimos, Alfama");
    int por = g.addVertice("Porto",     "Portugal", "Ribeira, Livraria Lello, Ponte Dom Luis I");
    int mad = g.addVertice("Madri",     "Espanha",  "Palacio Real, Museu do Prado, Parque do Retiro");
    int bcn = g.addVertice("Barcelona", "Espanha",  "Sagrada Familia, Park Guell, La Rambla");
    int par = g.addVertice("Paris",     "Franca",   "Torre Eiffel, Louvre, Notre-Dame");
    int zrh = g.addVertice("Zurique",   "Suica",    "Lago de Zurique, Altstadt, Uetliberg");
    int muc = g.addVertice("Munique",   "Alemanha", "Marienplatz, Castelo de Neuschwanstein (excursao), Englischer Garten");
    int ber = g.addVertice("Berlim",    "Alemanha", "Portao de Brandemburgo, Muro de Berlim, Ilha dos Museus");
    int rom = g.addVertice("Roma",      "Italia",   "Coliseu, Vaticano, Fontana di Trevi");

    // aresta: (u, v, custo EUR, distancia km, tempo h porta a porta, transporte)
    // Tempo dos voos inclui ~2h de deslocamento/embarque/check-in.
    // Valores sao ESTIMATIVAS de tarifas economicas tipicas (nao sao cotacoes reais).
    g.addAresta(sp, lis, 600, 7900, 11.5, "aviao");
    g.addAresta(sp, mad, 620, 8400, 12.5, "aviao");
    g.addAresta(sp, par, 680, 9400, 13.5, "aviao");
    g.addAresta(sp, rom, 650, 9300, 13.5, "aviao");
    g.addAresta(sp, zrh, 720, 9600, 14.0, "aviao");

    g.addAresta(lis, por,  30,  313,  3.0, "trem");
    g.addAresta(lis, mad,  70,  502,  3.3, "aviao");
    g.addAresta(por, mad,  65,  420,  3.2, "aviao");
    g.addAresta(por, par,  85, 1230,  4.0, "aviao");

    g.addAresta(mad, bcn,  55,  621,  2.6, "trem (AVE)");
    g.addAresta(mad, par,  90, 1050,  4.2, "aviao");
    g.addAresta(bcn, par,  80,  830,  3.7, "aviao");
    g.addAresta(bcn, rom,  70,  860,  3.6, "aviao");
    g.addAresta(bcn, zrh,  85,  860,  3.6, "aviao");

    g.addAresta(par, zrh,  90,  600,  4.0, "trem (TGV Lyria)");
    g.addAresta(par, ber,  85,  880,  3.8, "aviao");
    g.addAresta(par, muc,  80,  690,  3.5, "aviao");

    g.addAresta(zrh, muc,  50,  340,  3.5, "trem");
    g.addAresta(zrh, rom,  75,  690,  3.5, "aviao");
    g.addAresta(zrh, ber,  85,  850,  3.6, "aviao");

    g.addAresta(muc, ber,  50,  505,  4.0, "trem (ICE)");
    g.addAresta(muc, rom,  80,  700,  3.5, "aviao");
    g.addAresta(ber, rom,  95, 1180,  4.8, "aviao");
    return g;
}
