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

void BuscaLocal(Solution *s);
Solution Perturbacao(Solution best);
Solution ILS(int maxItr, int MaxIterIls, Data& data);
vector<int> escolher3NosAleatorios(Data& data);
vector<int> nosRestantes(Solution s, Data& data);
Solution Construcao(Data &data);
void inserirNaSolução(Solution& s, InsertionInfo noInserido, vector<int>& CL);

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


void BuscaLocal(Solution *s){

}
Solution Perturbacao(Solution best){
    Solution s;
    return s;
}

Solution Construcao(Data &data){
    Solution s;
    s.sequence = escolher3NosAleatorios(data);
    vector<int> CL = nosRestantes(s, data);

    for(int i = 0; i < s.sequence.size(); i++){
        cout << s.sequence[i] << " ";
    }

    cout << endl;

    while(!CL.empty()){
        vector<InsertionInfo> custoInsercao;
        calcularCustoInsercao(s,CL,data,custoInsercao);
        ordenarEmOrdemCrescente(custoInsercao);

        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        cout << "No inserido: " << custoInsercao[selecionado].noInserido << endl;
        inserirNaSolução(s, custoInsercao[selecionado], CL);
    }

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
            BuscaLocal(&s);
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
    
    cout << endl;
}