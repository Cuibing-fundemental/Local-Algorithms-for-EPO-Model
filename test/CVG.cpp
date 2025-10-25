#include "graph_io.hpp"
#include "algtest.hpp"
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
        std::mt19937 gen(1234);
        std::uniform_real_distribution<> dis(0.5, 1.0);
        
        for(size_t i=0; i<graph.num_vertices(); ++i){
            s[i] = dis(gen);
        }
        double epsilon = 1e-6;

        //GSSOR_CVG(graph,s,epsilon,1.7);
        HB_CVG(graph,s,epsilon);
        //CH_CVG(graph,s,epsilon);
        //GD_CVG(graph,s,epsilon);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}