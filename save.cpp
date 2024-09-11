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

        cout << "Vi: " << *vi_select << " Vj: " << *vj_select << endl;

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