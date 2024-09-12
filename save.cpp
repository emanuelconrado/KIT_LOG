gettimeofday(&start, NULL);

            auto data = Data(argc, current[count]);
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

            best_cost += cost;

            gettimeofday(&end, NULL);
            long seconds = end.tv_sec - start.tv_sec;
            long microseconds = end.tv_usec - start.tv_usec;
            double time_taken = seconds * 1000.0 + microseconds/1000.0;

            time += time_taken;