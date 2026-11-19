# Eurotrip – Análise de Grafos e Otimização de Rotas

Projeto desenvolvido para a disciplina de **Estruturas de Dados Não Lineares**, com o objetivo de aplicar conceitos de grafos, caminhos mínimos e árvore geradora mínima em um cenário de planejamento de viagens pela Europa.

## 1. Sobre o projeto

O Eurotrip simula uma rede de deslocamentos entre cidades europeias, considerando São Paulo (Brasil) como ponto de partida.

O sistema utiliza um grafo não direcionado e ponderado para representar as conexões entre as cidades. Cada conexão possui três critérios de peso:

- **Custo (€):** estimativa do valor do deslocamento.
- **Distância (km):** distância entre as cidades.
- **Tempo (h):** tempo estimado de viagem.

O objetivo é analisar diferentes possibilidades de deslocamento e identificar os caminhos mais econômicos, mais curtos e mais rápidos.

## 2. Modelagem do grafo

O grafo é composto por:

- **10 vértices:** representando as cidades.
- **23 arestas:** representando os deslocamentos diretos.
- **6 países europeus**, além do Brasil como origem da viagem.

### Cidades utilizadas

| País | Cidade |
|---|---|
| Brasil | São Paulo |
| Portugal | Lisboa e Porto |
| Espanha | Madri e Barcelona |
| França | Paris |
| Suíça | Zurique |
| Alemanha | Munique e Berlim |
| Itália | Roma |

Cada cidade também possui informações sobre seus principais pontos turísticos.

### Tipos de transporte

O sistema considera deslocamentos realizados por avião e trem, incluindo cinco trechos ferroviários de alta velocidade:

- Lisboa – Porto;
- Madri – Barcelona;
- Paris – Zurique;
- Zurique – Munique;
- Munique – Berlim.

Os demais deslocamentos cadastrados são realizados por avião.

**Observação:** os valores de custo, distância e tempo são estimativas utilizadas para fins acadêmicos, não representando cotações ou informações de viagem em tempo real. O tempo dos voos considera aproximadamente duas horas adicionais para deslocamento, embarque e check-in.

## 3. Estruturas de dados utilizadas

O projeto foi desenvolvido em C++, utilizando as seguintes estruturas:

- **Struct Aresta:** armazena os vértices de origem e destino, os três pesos da conexão e o meio de transporte.
- **Struct Grafo:** organiza os vértices, países, pontos turísticos, arestas e lista de adjacência.
- **Lista de adjacência:** representa as conexões entre os vértices.
- **Lista de arestas:** armazena as conexões do grafo, sendo utilizada principalmente pelo algoritmo de Kruskal.
- **Fila de prioridade:** utilizada pelo algoritmo de Dijkstra.
- **Matrizes:** utilizadas para representar os pesos diretos e os caminhos mínimos entre os pares de cidades.
- **Union-Find (União-Busca):** utilizada para identificar componentes e evitar ciclos durante a construção da árvore geradora mínima.

## 4. Algoritmos implementados

### 4.1. Busca em Largura (BFS)

Utilizada para verificar se o grafo é conexo, ou seja, se existe um caminho entre os vértices.

Também é realizada a verificação do grau mínimo dos vértices, considerando o requisito de grau mínimo igual ou superior a 3.

### 4.2. Dijkstra

Calcula o caminho mínimo entre São Paulo e a cidade de destino escolhida.

O algoritmo é executado três vezes, considerando diferentes critérios:

- Menor custo;
- Menor distância;
- Menor tempo.

O destino padrão é Berlim, mas o usuário pode informar outra cidade ao executar o programa.

### 4.3. Floyd-Warshall

Calcula os menores caminhos entre todos os pares de cidades do grafo.

O algoritmo é aplicado separadamente para custo, distância e tempo, apresentando as respectivas matrizes de caminhos mínimos.

### 4.4. Kruskal

Utilizado para encontrar a Árvore Geradora Mínima (AGM), conectando todos os vértices com o menor peso total possível, sem formar ciclos.

O algoritmo utiliza a estrutura Union-Find para auxiliar na identificação de ciclos e também é executado considerando os três critérios de otimização:

- Menor custo total;
- Menor distância total;
- Menor tempo total.

## 5. Resultados obtidos

Considerando São Paulo como origem e Berlim como destino, foram obtidos os seguintes resultados:

### Caminhos mínimos – Dijkstra

| Critério | Caminho encontrado | Custo | Distância | Tempo |
|---|---|---:|---:|---:|
| Menor custo | São Paulo → Roma → Berlim | €745 | 10.480 km | 18,3 h |
| Menor distância | São Paulo → Paris → Berlim | €765 | 10.280 km | 17,3 h |
| Menor tempo | São Paulo → Paris → Berlim | €765 | 10.280 km | 17,3 h |

Os resultados demonstram que a rota mais econômica não é necessariamente a mais rápida ou a mais curta.

A rota via Paris custa €20 a mais, mas economiza uma hora de viagem e 200 km em relação à rota via Roma.

### Árvore Geradora Mínima – Kruskal

| Critério de otimização | Custo total | Distância total | Tempo total |
|---|---:|---:|---:|
| Menor custo | €1.075 | 12.479 km | 38,6 h |
| Menor distância | €1.095 | Não informado | 39,0 h |
| Menor tempo | €1.110 | Não informado | 38,0 h |

A árvore geradora mínima por custo apresentou o menor gasto total, enquanto a otimização por tempo apresentou o menor tempo acumulado.

**Importante:** a Árvore Geradora Mínima representa uma rede de conexões que liga todas as cidades com peso total mínimo. Ela não representa necessariamente um roteiro de viagem a ser percorrido por um único turista.

## 6. Como executar o projeto

### Pré-requisitos

- Compilador C++ com suporte ao padrão C++11 ou superior.
- GCC/G++ instalado.
- Terminal ou ambiente de desenvolvimento compatível.

### Compilação

Na pasta onde está localizado o arquivo `eurotrip.cpp`, execute:

```bash
g++ -std=c++11 -O2 -o eurotrip eurotrip.cpp
```

### Execução

Para executar utilizando Berlim como destino padrão:

```bash
./eurotrip
```

Para escolher outra cidade como destino:

```bash
./eurotrip Roma
```

Também é possível utilizar outras cidades cadastradas no grafo, como Lisboa, Porto, Madri, Barcelona, Paris, Zurique e Munique.

No Windows, utilizando MinGW, o executável poderá ser gerado como `eurotrip.exe`.

## 7. Considerações finais

O desenvolvimento do Eurotrip permitiu aplicar conceitos fundamentais de estruturas de dados não lineares, demonstrando como os grafos podem ser utilizados para representar redes de deslocamento e solucionar problemas de otimização.

A implementação dos algoritmos Dijkstra, Floyd-Warshall e Kruskal possibilitou analisar diferentes estratégias de otimização, considerando custo, distância e tempo.

Os resultados demonstram que a escolha de uma rota depende diretamente da prioridade estabelecida pelo usuário, evidenciando a importância dos algoritmos de grafos em problemas de tomada de decisão.

## 8. Declaração de uso de Inteligência Artificial

**Ferramenta utilizada:** Claude (Anthropic).

**Aplicação:** geração inicial do código C++, dos dados estimados do grafo.

**Responsabilidade:** o grupo revisou o material produzido e é responsável pela compreensão e explicação das estruturas de dados, algoritmos e decisões adotadas no projeto.

## 9. Integrantes

- Henrique Chittolina
- Nicolas Machado
- Tiago Schmitz

**Disciplina:** Estruturas de Dados Não Lineares  
**Professor:** Thiago Koman Schmidt

---

Desenvolvido para fins acadêmicos.
