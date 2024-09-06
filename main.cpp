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
bool bestImprovementSwap(Solution *s, Data data);

void calcularCustoInsercao(Solution& s, vector<int>& CL, Data& data, vector<InsertionInfo>& custoInsercao);
void ordenarEmOrdemCrescente(vector<InsertionInfo>& custoInsercao);

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    Construcao(data);

    /*
    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();

    cout << "Exemplo de Solucao s = ";
    double cost = 0.0;
    for (size_t i = 1; i < n; i++) {
        cout << i << " -> ";
        cost += data.getDistance(i, i+1);
    }
    */

    /*
    cost += data.getDistance(n, 1);
    cout << n << " -> " << 1 << endl;
    cout << "Custo de S: " << cost << endl;
    */

    return 0;
}


void BuscaLocal(Solution *s, Data data){
    vector<int> NL = {1,2,3,4,5};
    bool improved = false;

    while(!NL.empty()){
        int n = rand() % NL.size();
        switch(NL[n]){
        case 1:
            improved = bestImprovementSwap(s, data);
            break;
        }

        if(improved)
            NL = {1,2,3,4,5};
        else
            NL.erase(NL.begin() + n);
    }
}
Solution Perturbacao(Solution best){
    Solution s;
    return s;
}

Solution Construcao(Data &data){
    Solution s;
    s.sequence = escolher3NosAleatorios(data);
    vector<int> CL = nosRestantes(s, data);

    while(!CL.empty()){
        vector<InsertionInfo> custoInsercao;
        calcularCustoInsercao(s,CL,data,custoInsercao);
        ordenarEmOrdemCrescente(custoInsercao);

        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolução(s, custoInsercao[selecionado], CL);
    }


    //Print sequencia incial:
    for (int i = 0; i < s.sequence.size(); i++)
    {
        cout << s.sequence[i] << "-> ";
    }

    cout << endl;

    return s;
}

vector<int> nosRestantes(Solution s, Data& data){
    vector<int> restante;

    for(int i = 1; i < data.getDimension(); i++){
        int count = 0;
        for(int j = 0; j < s.sequence.size() - 1; j++){
            if(s.sequence[j] == i){
                count++;
            }
        }

        if(count == 0){
            restante.push_back(i);
        }
    }

    return restante;
}

vector<int> escolher3NosAleatorios(Data& data){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(2, data.getDimension());

    vector<int> sequence;
    sequence.push_back(1);

    for(int i = 1; i < 4; i++){
        int ramdom_number = dis(gen);
        sequence.push_back(ramdom_number);
    }

    sequence.push_back(1);
    return sequence;
}

Solution ILS(int maxItr, int MaxIterIls, Data& data){
    
    Solution bestOfAll;
    bestOfAll.cost = 100000000;

    for(int i = 0; i < maxItr; i++){
        Solution s = Construcao(data);
        Solution best = s;

        int interIls = 0;

        while (interIls <= MaxIterIls)
        {
            BuscaLocal(&s, data);
            if(s.cost < best.cost){
                best = s;
                interIls = 0;
            }

            s = Perturbacao(best);
            interIls++;
        }
        if(best.cost < bestOfAll.cost)
            bestOfAll = best;
        
    }

    return bestOfAll;
}

void calcularCustoInsercao(Solution& s, vector<int>& CL, Data& data, vector<InsertionInfo>& custoInsercao){
    custoInsercao = vector<InsertionInfo> ((s.sequence.size() - 1) * CL.size());

    int l = 0;
    for(int a = 0; a < s.sequence.size() - 1; a++){
        int i = s.sequence[a];
        int j = s.sequence[a + 1];
        for(auto k : CL){
            custoInsercao[l].custo = data.getDistance(i,k) + data.getDistance(j,k) - data.getDistance(i,j);
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = a;
            l++;
        }
    }
}


void ordenarEmOrdemCrescente(vector<InsertionInfo>& custoInsercao){

    std::sort(custoInsercao.begin(), custoInsercao.end(), [](const InsertionInfo& a, const InsertionInfo& b){return a.custo < b.custo;});
}

void inserirNaSolução(Solution& s, InsertionInfo noInserido, vector<int>& CL){
    
    s.sequence.insert(s.sequence.begin() + (noInserido.arestaRemovida + 1),noInserido.noInserido);

    for(int i = 0; i < CL.size(); i++){
        if(CL[i] == noInserido.noInserido){
            CL.erase(CL.begin() + i);
        }
    }
}

bool bestImprovementSwap(Solution *s, Data data){
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s->sequence.size() - 1; i++){
        int vi = s->sequence[i];
        int vi_next = s->sequence[i+1];
        int vi_prev = s->sequence[i-1];

        for(int j = i+1; j < s->sequence.size() - 1; j++){
            int vj = s->sequence[j];
            int vj_next = s->sequence[j+1];
            int vj_prev = s->sequence[j-1];

            double delta;

            if(vj_prev == vi){
                delta = - data.getDistance(vi_prev,vi) - data.getDistance(vj, vj_next)
                + data.getDistance(vi_prev, vj) + data.getDistance(vi, vj_next);
            }else{
                delta = - data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next)
                + data.getDistance(vi_prev, vj) + data.getDistance(vj, vi_next)
                - data.getDistance(vj_prev, vj) - data.getDistance(vj, vj_next)
                + data.getDistance(vj_prev, vi) + data.getDistance(vi, vj_next);
            }

    
            if (delta < bestDelta)
            {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestDelta < 0){
        cout << s->sequence[best_i] << " e " << s->sequence[best_j] << endl;
        swap(s->sequence[best_i], s->sequence[best_j]);
        s->cost = s->cost + bestDelta;
        return true;
    }

    return false;
}