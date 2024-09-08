bool bestImprovementSwap(Solution *s, Data data){
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s->sequence.size() - 2; i++){
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

        cout << "Trocou: " << s->sequence[best_i] << " e " << s->sequence[best_j] << endl;

        swap(s->sequence[best_i], s->sequence[best_j]);
        s->cost = s->cost + bestDelta;

        for(int i = 0; i < s->sequence.size(); i++){
            cout << s->sequence[i] << "-> ";
        }
        return true;
    }