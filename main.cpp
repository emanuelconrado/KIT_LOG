#include "Data.h"
#include "Solution.h"
#include "Function.h"
#include <fstream>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <thread>

void mult_thread(int argc, char * current, double &time, double &best_cost){
    timeval start, end;

    gettimeofday(&start, NULL);

    auto data = Data(argc, current);
    data.read();
    size_t n = data.getDimension();

    int maxIter = 50;
    int MaxIterIls = data.getDimension();

    if(MaxIterIls >= 150){
        MaxIterIls = MaxIterIls/2;
    }

    Solution s = ILS(maxIter, MaxIterIls, data);

    //cout << "Solucao s = ";

    double cost = 0.0;

    for (int i = 0; i < s.sequence.size() - 1; i++) {
        //cout << s.sequence[i] << " -> ";
        cost += data.getDistance(s.sequence[i], s.sequence[i+1]);
    }
    //cout << "1" << endl;
    //cout << "Custo de S: " << cost << endl;

    best_cost = cost;

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double time_taken = seconds * 1000.0 + microseconds/1000.0;

    time = time_taken;
}

int main(int argc, char** argv) {

    std::ifstream file(argv[1]);
    std::ifstream file_name(argv[1]);
    std::string line_name;
    vector<string> instancias;
    std::string line;

    std:: ofstream outFile("test_benchmark.txt");

    if(!outFile){
        cout << "Output file not opened!" << endl;
        return EXIT_FAILURE;
    }

    outFile << "Processador: 12 × AMD Ryzen 5 4500 6-Core Processor" << endl;
    outFile << endl;

    int count = 0;

    string dir = "/home/Duel/Documentos/KIT_LOG/instances/";


    struct timeval start, end;

    if(!file){
        cout << "File not opened!" << endl;
        return 1;
    }

    while(getline(file_name, line_name)){
        instancias.push_back(line_name);
    }

    char ** current = (char **) malloc(instancias.size() * sizeof(char*));
    if(current == NULL){
        perror("Allocate failed!");
        return EXIT_FAILURE;
    }



    while(getline(file, line)){
        current[count] = (char *)malloc((dir.size() + instancias[count].size()) * sizeof(char));
        if(current[count] == NULL){
            perror("Allocate failed!");
            return EXIT_FAILURE;
        }

        for(int i = 0 ; i < dir.size(); i++){
            current[count][i] = dir[i];
        }

        int j = 0;

        for(int i = dir.size(); i < dir.size() + instancias[count].size(); i++){
            current[count][i] = instancias[count][j];
            j++;
        }

        count++;
    }

    count = 0;

    while(count < instancias.size()){

        double total_time = 0.0;
        double total_cost = 0.0;

        std::vector<std::thread> threads;
        std::vector<double> times(10, 0.0);
        std::vector<double> costs(10, 0.0);

        //

        for (int j = 0; j < 10; j++){
            threads.push_back(std::thread(mult_thread, argc, current[count], std::ref(times[j]), std::ref(costs[j])));
        }

        for (int j = 0; j < 10; j++){
            threads[j].join();
        }

        for(int k = 0; k < 10; k++){
            total_time += times[k];
            total_cost += costs[k];
        }

        cout << "Intância: " << instancias[count] << endl;
        cout << "Média de tempo: " << (total_time/10)/1000 << endl;
        cout << "Média de custo: " << (total_cost/10) << endl;

        outFile << "Instância: " << instancias[count] << endl;
        outFile << "Média de tempo: " << (total_time/10)/1000 << endl;
        outFile << "Média de custo: " << (total_cost/10) << endl;

        count++;
    }

    outFile.close();
    file.close();
    file_name.close();
    return 0;
}