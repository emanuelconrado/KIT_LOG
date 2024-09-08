#ifndef FUNCTION_H
#define FUNCTION_H


#include "Data.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "Solution.h"
#include <random>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;



struct InsertionInfo{
    int noInserido; // no k a ser inserido
    int arestaRemovida;  // aresta {i,j} na qual o no k sera inserido
    double custo; // delta ao inserir k na aresta {i,j}
};

void BuscaLocal(Solution *s, Data data);
Solution Perturbacao(Solution best);
Solution ILS(int maxItr, int MaxIterIls, Data& data);
vector<int> escolher3NosAleatorios(Data& data);
vector<int> nosRestantes(Solution s, Data& data);
Solution Construcao(Data &data);
void inserirNaSolução(Solution& s, InsertionInfo noInserido, vector<int>& CL);

//Vizinhaça

bool bestImprovementSwap(Solution *s, Data data);
bool bestImprovement2Opt(Solution *s, Data data);
bool bestImprovementOrOpt(Solution *s, Data data, int choice);

//

void calcularCustoInsercao(Solution& s, vector<int>& CL, Data& data, vector<InsertionInfo>& custoInsercao);
void ordenarEmOrdemCrescente(vector<InsertionInfo>& custoInsercao);


#endif