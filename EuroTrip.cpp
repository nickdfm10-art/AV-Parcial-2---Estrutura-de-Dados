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

// ------------------------------ Validacoes ---------------------------
// BFS para verificar se o grafo e conexo
bool ehConexo(const Grafo& g) {
    vector<bool> vis(g.n, false);
    queue<int> q;
    q.push(0); vis[0] = true;
    int cont = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (size_t k = 0; k < g.adj[u].size(); k++) {
            int v = g.adj[u][k].first;
            if (!vis[v]) { vis[v] = true; cont++; q.push(v); }
        }
    }
    return cont == g.n;
}

int grauMinimo(const Grafo& g) {
    int m = (int)1e9;
    for (int i = 0; i < g.n; i++) m = min(m, (int)g.adj[i].size());
    return m;
}

// ------------------------------- Dijkstra ----------------------------
struct ResultadoDijkstra {
    vector<double> dist;
    vector<int> anterior;   // vertice anterior no caminho minimo
    vector<int> arestaAnt;  // aresta usada para chegar no vertice
};

ResultadoDijkstra dijkstra(const Grafo& g, int origem, Criterio c) {
    ResultadoDijkstra r;
    r.dist.assign(g.n, INF);
    r.anterior.assign(g.n, -1);
    r.arestaAnt.assign(g.n, -1);
    // fila de prioridade de minimo: (distancia acumulada, vertice)
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int> > > pq;
    r.dist[origem] = 0;
    pq.push({0, origem});
    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > r.dist[u]) continue;           // entrada desatualizada
        for (size_t k = 0; k < g.adj[u].size(); k++) {
            int v = g.adj[u][k].first;
            int id = g.adj[u][k].second;
            double nd = d + g.arestas[id].peso[c];
            if (nd < r.dist[v]) {              // relaxamento
                r.dist[v] = nd;
                r.anterior[v] = u;
                r.arestaAnt[v] = id;
                pq.push({nd, v});
            }
        }
    }
    return r;
}

// Reconstroi a lista de arestas do caminho origem -> destino
vector<int> caminhoArestas(const ResultadoDijkstra& r, int destino) {
    vector<int> arestas;
    for (int v = destino; r.anterior[v] != -1; v = r.anterior[v])
        arestas.push_back(r.arestaAnt[v]);
    reverse(arestas.begin(), arestas.end());
    return arestas;
}

void imprimirCaminho(const Grafo& g, int origem, int destino, Criterio c) {
    ResultadoDijkstra r = dijkstra(g, origem, c);
    vector<int> arestas = caminhoArestas(r, destino);
    double tot[3] = {0, 0, 0};
    cout << "  Caminho: " << g.nomes[origem];
    int atual = origem;
    for (int id : arestas) {
        const Aresta& a = g.arestas[id];
        int prox = (a.u == atual) ? a.v : a.u;
        cout << " -> " << g.nomes[prox] << " [" << a.transporte << "]";
        for (int k = 0; k < 3; k++) tot[k] += a.peso[k];
        atual = prox;
    }
    cout << "\n  Totais deste caminho: custo = " << tot[CUSTO] << " EUR | distancia = "
         << tot[DISTANCIA] << " km | tempo = " << tot[TEMPO] << " h\n";
}

// ----------------------------- Floyd-Warshall -------------------------
// Matriz de adjacencia (peso direto) e matriz de caminhos minimos entre todos os pares
vector<vector<double>> matrizDireta(const Grafo& g, Criterio c) {
    vector<vector<double>> m(g.n, vector<double>(g.n, INF));
    for (int i = 0; i < g.n; i++) m[i][i] = 0;
    for (const Aresta& a : g.arestas) {
        m[a.u][a.v] = a.peso[c];
        m[a.v][a.u] = a.peso[c];
    }
    return m;
}

vector<vector<double>> floydWarshall(vector<vector<double>> d) {
    int n = (int)d.size();
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (d[i][k] + d[k][j] < d[i][j]) d[i][j] = d[i][k] + d[k][j];
    return d;
}

void imprimirMatriz(const Grafo& g, const vector<vector<double>>& m, const string& titulo) {
    cout << "\n" << titulo << "\n";
    cout << setw(11) << " ";
    for (int j = 0; j < g.n; j++) cout << setw(10) << g.nomes[j].substr(0, 9);
    cout << "\n";
    for (int i = 0; i < g.n; i++) {
        cout << setw(11) << g.nomes[i].substr(0, 10);
        for (int j = 0; j < g.n; j++) {
            if (m[i][j] == INF) cout << setw(10) << "-";
            else cout << setw(10) << fixed << setprecision(1) << m[i][j];
        }
        cout << "\n";
    }
}

// ------------------------------- Kruskal -----------------------------
struct UniaoBusca {                    // Union-Find com compressao de caminho
    vector<int> pai, rk;
    UniaoBusca(int n) : pai(n), rk(n, 0) { iota(pai.begin(), pai.end(), 0); }
    int achar(int x) { return pai[x] == x ? x : pai[x] = achar(pai[x]); }
    bool unir(int a, int b) {
        a = achar(a); b = achar(b);
        if (a == b) return false;      // ja estao no mesmo componente (formaria ciclo)
        if (rk[a] < rk[b]) swap(a, b);
        pai[b] = a;
        if (rk[a] == rk[b]) rk[a]++;
        return true;
    }
};

vector<int> kruskal(const Grafo& g, Criterio c) {
    vector<int> ordem(g.arestas.size());
    iota(ordem.begin(), ordem.end(), 0);
    stable_sort(ordem.begin(), ordem.end(), [&](int a, int b) {  // stable: empates em ordem de insercao
        return g.arestas[a].peso[c] < g.arestas[b].peso[c];
    });
    UniaoBusca uf(g.n);
    vector<int> agm;
    for (int id : ordem)
        if (uf.unir(g.arestas[id].u, g.arestas[id].v)) agm.push_back(id);
    return agm;                         // n-1 arestas
}

void imprimirAGM(const Grafo& g, const vector<int>& agm, Criterio c) {
    double tot[3] = {0, 0, 0};
    for (int id : agm) {
        const Aresta& a = g.arestas[id];
        cout << "  " << setw(10) << left << g.nomes[a.u] << " -- " << setw(10) << g.nomes[a.v]
             << right << "  custo=" << setw(4) << a.peso[CUSTO] << "  dist=" << setw(5) << a.peso[DISTANCIA]
             << "  tempo=" << setw(4) << a.peso[TEMPO] << "  [" << a.transporte << "]\n";
        for (int k = 0; k < 3; k++) tot[k] += a.peso[k];
    }
    cout << "  TOTAL da AGM por " << NOME_CRITERIO[c] << ": custo = " << tot[CUSTO]
         << " EUR | distancia = " << tot[DISTANCIA] << " km | tempo = " << tot[TEMPO] << " h\n";
}
