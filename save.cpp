Solution Perturbacao(Solution best){
    while(true){

        int count = 0;

        //[0]     [1]       [2]     [3]
        //PosSeg1 QtAresta1 PosSeg2 QtAresta2
        vector<int> segment;



        //Gerando local e segmento aleatório
        while(count < 4){
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(2, best.sequence.size() - 2);
            int selecionado = dis(gen);

            int ramdom_number = 1 + rand() % (selecionado);
            segment.push_back(ramdom_number);
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