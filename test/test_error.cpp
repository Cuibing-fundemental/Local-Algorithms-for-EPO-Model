#include "graph_io.hpp"
#include "alg.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <random>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <edge_list_file>\n";
        return 1;
    }

    try {
        // 从文件构建CSR图
        CSRGraph graph = build_csr_from_file(argv[1],true);
                
        Opinion s(graph.num_vertices(),1);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.1, 1.0);
        
        for(size_t i=0; i<graph.num_vertices(); ++i){
            s[i] = dis(gen);
        }
        double epsilon = 1e-2;

        auto z_exc = ExactSol(graph,s,1,true);
        auto z_1 = MyIterator_NEW_SOR(graph,s,epsilon,1.5);
        auto z_2 = ChebyshevIterator(graph,s,epsilon);
        auto z_3 = HeavyBallIterator(graph,s,epsilon);
        auto res = NaiveIterator(graph,s,1e-6);
        auto z_4 = res.second;

        double e1,e2,e3,e4;
        e1 = e2 = e3 = e4 = 0;
        for(size_t i=0; i < graph.num_vertices(); i++){
            if (graph.out_degree(i) == 0)
                continue;
            e1 = e1 < fabs(z_exc[i]-z_1[i])/z_exc[i] ? fabs(z_exc[i]-z_1[i])/z_exc[i] : e1;
            e2 = e2 < fabs(z_exc[i]-z_2[i])/z_exc[i] ? fabs(z_exc[i]-z_2[i])/z_exc[i] : e2;
            e3 = e3 < fabs(z_exc[i]-z_3[i])/z_exc[i] ? fabs(z_exc[i]-z_3[i])/z_exc[i] : e3;
            e4 = e4 < fabs(z_exc[i]-z_4[i])/z_exc[i] ? fabs(z_exc[i]-z_4[i])/z_exc[i] : e4;
        }
        printf("Relative Error:\n");
        printf("MyIter_NEW_SOR: %.4e\n",e1);
        printf("ChebyshevIterator: %.4e\n",e2);
        printf("HeavyBallIterator: %.4e\n",e3);
        printf("NaiveIterator: %.4e\n",e4);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}