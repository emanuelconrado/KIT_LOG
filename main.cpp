#include "Data.h"
#include "Solution.h"
#include "Function.h"

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    int maxIter = 50;
    int MaxIterIls = data.getDimension();

    if(MaxIterIls >= 150){
        MaxIterIls = MaxIterIls/2;
    }

    Solution s = ILS(maxIter, MaxIterIls, data);

    cout << "Solucao s = ";

    double cost1 = 0.0;

    for (int i = 0; i < s.sequence.size() - 1; i++) {
        cout << s.sequence[i] << " -> ";
        cost1 += data.getDistance(s.sequence[i], s.sequence[i+1]);
    }

    cout << "1" << endl;
    cout << "Custo de S: " << cost1 << endl;

    return 0;
}