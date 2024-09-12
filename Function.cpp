#include "Function.h"


//Implementação das funções:

//Busca pela melhor solução dado uma vizinhança

void BuscaLocal(Solution& s, Data &data){
    vector<int> NL = {1,2,3,4,5};
    bool improved = false;

    while(!NL.empty()){

        int n = rand() % NL.size();

        switch(NL[n]){
        case 1:
            improved = bestImprovementSwap(s, data);
            break;
        case 2:
            improved = bestImprovement2Opt(s, data);
            break;
        case 3:
            improved = bestImprovementOrOpt(s, data, 1);
            break;
        case 4:
            improved = bestImprovementOrOpt(s, data, 2);
            break;
        case 5:
            improved = bestImprovementOrOpt(s, data, 3);
            break;
        }

        if(improved)
            NL = {1,2,3,4,5};
        else
            NL.erase(NL.begin() + n);
    }

    calculaCusto(s, data);
}

//Ainda não sei

Solution Perturbacao(Solution best){
    random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(1, ceil(best.sequence.size()/10.0));


    while(true){

        int count = 0;

        //[0]     [1]       [2]     [3]
        //PosSeg1 QtAresta1 PosSeg2 QtAresta2
        vector<int> segment;

        //Gerando local e segmento aleatório
        while(count < 2){
            int selecionado = dis(gen);
            int random_number = 1 + rand() % selecionado;
            segment.push_back(random_number);
            segment.push_back(selecionado);
            count++;
        }

        //Comparando onde está o segmento (quem está mais a direita)

        //Quando segmento [0] está mais a direita
        if(segment[0] > segment[2] && segment[0] + segment[1] < best.sequence.size() - 1 && segment[2] + segment[3] <= segment[0]){
            auto vi = best.sequence.begin() + segment[2];
            auto vi_last = best.sequence.begin() + (segment[2] + segment[3]);
            auto vj = best.sequence.begin() + segment[0];
            auto vj_last = best.sequence.begin() + (segment[0] + segment[1]);

            rotate(vi, vi_last, vj_last);

            vj = vj_last - (segment[3] + segment[1]);
            vj_last = vj_last - segment[3];

            rotate(vi, vj, vj_last);

            return best;
        }

        //Quando segmento [2] está mais a direita
        if(segment[2] > segment[0] && segment[2] + segment[3] < best.sequence.size() - 1 && segment[0] + segment[1] <= segment[2]){
            auto vi = best.sequence.begin() + segment[0];
            auto vi_last = best.sequence.begin() + (segment[0] + segment[1]);
            auto vj = best.sequence.begin() + segment[2];
            auto vj_last = best.sequence.begin() + (segment[2] + segment[3]);

            rotate(vi, vi_last, vj_last);

            vj = vj_last - (segment[1] + segment[3]);
            vj_last = vj_last - segment[1];

            rotate(vi, vj, vj_last);

            return best;
        }
    }
}

//Contruindo a solução

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

    calculaCusto(s, data);
    return s;
}

//Retorna um vetor com os nos restante

vector<int> nosRestantes(Solution s, Data& data){
    vector<int> restante;

    for(int i = 1; i <= data.getDimension(); i++){
        int count = 0;
        for(int j = 0; j < s.sequence.size(); j++){
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

//Escolhe 3 nos aleatorios para criar a primeira sequência

vector<int> escolher3NosAleatorios(Data& data){
    random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(2, data.getDimension());

    int count = 0;

    vector<int> sequence;
    sequence.push_back(1);

    while(count < 3){
        int ramdom_number = dis(gen);
        bool rep = false;

        for(int j = 0; j < sequence.size(); j++){
            if (ramdom_number == sequence[j]){
                rep = true;
                break;
            }
        }

        if(!rep){
            sequence.push_back(ramdom_number);
            count++;
        }
    }

    sequence.push_back(1);
    return sequence;
}

//Ainda não sei

void calculaCusto(Solution &s, Data &data){
    s.cost = 0.0;

    for(int i = 0; i < s.sequence.size() - 1; i++){
        s.cost = s.cost + data.getDistance(s.sequence[i], s.sequence[i+1]);
    }
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
            BuscaLocal(s, data);

            if(s.cost < best.cost){
                best = s;
                interIls = 0;
            }

            s = Perturbacao(best);
            interIls++;

        }


        if(best.cost < bestOfAll.cost){
            bestOfAll = best;
        }
    }

    return bestOfAll;
}

//Calcula todas as insentions possíveis e seus pesos

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

//Ordena o custo das insertions em ordem crescente de peso

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

//Troca dois nos quaisques (Todas as possíveis trocas)

bool bestImprovementSwap(Solution &s, Data &data){

    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s.sequence.size() - 2; i++){
        int vi = s.sequence[i];
        int vi_next = s.sequence[i+1];
        int vi_prev = s.sequence[i-1];

        for(int j = (i + 2); j < s.sequence.size() - 1; j++){
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];
            int vj_prev = s.sequence[j-1];

            double delta = - data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next)
                - data.getDistance(vj_prev, vj) - data.getDistance(vj, vj_next)
                + data.getDistance(vj_prev, vi) + data.getDistance(vi, vj_next)
                + data.getDistance(vi_prev, vj) + data.getDistance(vj, vi_next);

    
            if (delta < bestDelta)
            {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestDelta < 0){
        std::swap(s.sequence[best_i], s.sequence[best_j]);
        s.cost = s.cost + bestDelta;
        return true;
    }
    return false;
}


//Troca dois nos não adjacentes

bool bestImprovement2Opt(Solution &s, Data &data){
    double bestDelta = 0;
    int best_i = 0, best_j = 0;

    for(int i = 1; i < s.sequence.size() - 2; i++){
        int vi = s.sequence[i];
        int vi_next = s.sequence[i+1];
        int vi_prev = s.sequence[i-1];

        for(int j = i+1; j < s.sequence.size() - 1; j++){
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];
            int vj_prev = s.sequence[j-1];

            double delta = - data.getDistance(vi_prev, vi) - data.getDistance(vj, vj_next)
            + data.getDistance(vi_prev, vj) + data.getDistance(vi, vj_next);

    
            if (delta < bestDelta)
            {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestDelta < 0){
        reverse(s.sequence.begin() + best_i, s.sequence.begin() + best_j + 1);
        s.cost = s.cost + bestDelta;
        return true;
    }

    return false;
}

bool bestImprovementOrOpt(Solution &s, Data &data, int choice){
    double bestDelta = 0;
    int best_i, best_j;

    vector<int>::iterator vj_select, vi_select;


    for(int i = 0; i < s.sequence.size() - 1; i++){
        auto vi = s.sequence.begin() + i;
        auto vi_next = s.sequence.begin() + (i+1);

            for(int j = 1; j < s.sequence.size() - choice; j++){
                auto vj = s.sequence.begin() + j;
                auto vj_next = s.sequence.begin() + (j+choice);
                auto vj_prev = s.sequence.begin() + (j-1);

                if(vi < vj_prev || vj_next < vi){
                    double delta = - data.getDistance(*vj_prev, *vj) - data.getDistance(*(vj + (choice-1)), *vj_next)
                    + data.getDistance(*vj_prev, *vj_next) - data.getDistance(*vi, *vi_next)
                    + data.getDistance(*vi, *vj) + data.getDistance(*(vj + (choice-1)), *vi_next);

    
                    if (delta < bestDelta)
                    {
                        vj_select = vj;
                        vi_select = vi;
                        bestDelta = delta;
                    }
                }

        }
    }
    if(bestDelta < 0){

        if(vj_select > vi_select){
            rotate(vi_select + 1, vj_select, vj_select + choice);
        }else{
            rotate(vj_select, vj_select + choice, vi_select + 1);
        }

        for(int i = 0; i < s.sequence.size(); i++){

        }

        s.cost = s.cost + bestDelta;
        return true;
    }
    return false;
}